//---------------------------------------------------------------------------
// Error codes:
//				0		- No error
//				1		- Error opening file (no data)
//				2		- Error reading record
//				4		- Morphological data not found, assume we have inland
//				5		- Height is missing (-9999)
//				6		- Distance = 0
//---------------------------------------------------------------------------

#include "terrain.h"
#include <string>
#include <math.h>
#include <fstream>

Terrain::Terrain(const string& topoPath, const string& morphPath): topoPath(topoPath), morphPath(morphPath){
	// Declaration of point zone in the morphological database
	error = 0;
	step = 0.100;	// 100 m. Exact step depends on distance.
}
Terrain::Terrain(): topoPath(""), morphPath(""){
	// Declaration of point zone in the morphological database
	error = 0;
	step = 0.100;	// 100 m. Exact step depends on distance.
}
Terrain::~Terrain(){
	CloseFiles();
}
//!---------------------------------------------------------------------------------------------

void Terrain::Initialise(double longitude1, double latitude1, double longitude2, double latitude2){
	error = 0;
	distance = GetDistance(longitude1, latitude1, longitude2, latitude2);
	if((longitude1 == longitude2 && latitude1 == latitude2)||distance == 0.0){
		error = 6;
		return;
	}
	txAzimuth = GetAzimuth(longitude1, latitude1, longitude2, latitude2);
	rxAzimuth = GetAzimuth(longitude2, latitude2, longitude1, latitude1);
	// Constructing path profile
	FillProfile(longitude1, latitude1, longitude2, latitude2);
}
//!---------------------------------------------------------------------------------------------

// Calculation of Azimuth
double Terrain::GetAzimuth(double longitude1, double latitude1, double longitude2, double latitude2){
	double dist = acos( sin(C_PI * latitude1 / 180) * sin(C_PI * latitude2 / 180) + cos(C_PI * latitude1 / 180) *
				  cos(C_PI * latitude2 / 180) *  cos((C_PI * longitude1 / 180 - C_PI * longitude2 / 180)));
	double rtmp = (sin(C_PI * latitude2 / 180) - sin(C_PI * latitude1 / 180) * cos(dist)) / (cos(C_PI * latitude1 / 180) * sin(dist));
	if(rtmp < -1) rtmp =-1;
	if(rtmp >  1) rtmp = 1;
	rtmp = 180 * acos(rtmp)/C_PI;
	if(longitude1 > longitude2)
		return 360 - rtmp;
	else
		return rtmp;
}
//!---------------------------------------------------------------------------------------------

// Calculation of Distance
double Terrain::GetDistance(double longitude1, double latitude1, double longitude2, double latitude2){
  return EARTH_RADIUS * acos(sin(C_PI * latitude1 / 180) * sin(C_PI * latitude2 / 180) + cos(C_PI * latitude1 / 180) *
		 cos(C_PI * latitude2 / 180) * cos((C_PI * longitude1 / 180 - C_PI * longitude2 / 180)));
}
//!---------------------------------------------------------------------------------------------

short Terrain::GetPointHeight(double longitude, double latitude){
    int lats  = static_cast<int> (latitude * 3600);	// change format of latitude in to seconds
    int longs = static_cast<int> (longitude * 3600);// change format of longitude in to seconds
	string filename = GetFileName(longitude, latitude, FileProf::Topo);	// get file name according to coordinates
	FILE* fileh = OpenTopo(topoPath + path_separator + filename);	// open terrain file
	if(fileh == NULL){
		error = 1;	// file not found
		return 0;
	}
	double blockx = (longs - floor(longitude) * 3600)*12/3600;	// get horizontal block number
	double blocky = (lats -  floor(latitude)  * 3600)*12/3600;	// get vertical block number
	int block_nr = static_cast<int>(blocky) * 12 + static_cast<int>(blockx);	// block number from beginning
	int stepx = latitude < 50 ? 100 : 50;						// if latitude > 50, in the block we have 50 points in x direction
	double dotx = (blockx - floor(blockx)) * stepx;				// point x coordinate inside the block
	double doty = (blocky - floor(blocky))*100;					// point y coordinate inside the block
	int dot_nr = static_cast<int>(doty) * (stepx+1) + static_cast<int>(dotx);	// point number from beginning of the block
	long offset = block_nr * (stepx + 1) * 202 + dot_nr * 2;	// offset of the first point in bytes

	short h1, h2, h3, h4;
	fseek(fileh, offset, SEEK_SET);
	// read 1st point
	if(fread(&h1, 2, 1, fileh)!=1){
		error = 2;
		return 0;
	}
	// read 2nd point
	if(fread(&h2, 2, 1, fileh)!=1){
		error = 2;
		return 0;
	}
	// move cursor by 1 line - 2 data points
	fseek(fileh,(stepx + 1 - 2)*2, SEEK_CUR);
	// read 3rd point
	if(fread(&h3, 2, 1, fileh)!=1){
		error = 2;
		return 0;
	}
	// read 4th point
	if(fread(&h4, 2, 1, fileh)!=1){
		error = 2;
		return 0;
	}
	if(h1 == -9999 || h2 == -9999 || h3 == -9999 || h4 == -9999){
		error = 5;
		return 0;
	}
	double h12 = h1 + (h2-h1)*(dotx - floor(dotx));
	double h34 = h3 + (h4-h3)*(dotx - floor(dotx));
	double h = h12 + (h34-h12)*(doty - floor(doty));// get hight [double]
	if(h - floor(h) >= 0.5)
		return static_cast<short>(h+1);// transform hight to short
	else
		return static_cast<short>(h);
}
//!---------------------------------------------------------------------------------------------

// Opens terrain file, if available.
// Returns NULL if file is not found
FILE* Terrain::OpenTopo(const string& filename){
	if(topoFiles.find(filename) == topoFiles.end()){
		FILE* f = fopen(filename.c_str(), "rb");
		if(f != NULL) topoFiles[filename] = f;
		return f;
	}
	else return topoFiles[filename];
}
//!---------------------------------------------------------------------------------------------

// Opens morphological file, if available.
// Returns NULL if file is not found
FILE* Terrain::OpenMorpho(const string& filename){
	if(morphFiles.find(filename) == morphFiles.end()){
		FILE* f = fopen(filename.c_str(), "rb");
		if(f != NULL) morphFiles[filename] = f;
		return f;
	}
	else return morphFiles[filename];
}
//!---------------------------------------------------------------------------------------------

// Gets file name according to given coordinates.
// Returns empty string in case of error
string Terrain::GetFileName(double longitude, double latitude, FileProf fpType){
	char ew = longitude >= 0 ? 'E' : 'W';
	char ns = latitude >= 0 ? 'N' : 'S';
	string en = "";
	switch(fpType){
		case FileProf::Topo:
			en = fabs(latitude) < 50 ? ".33E" : ".63E";
			break;
		case FileProf::Morpho:
			en = fabs(latitude) < 50 ? ".33M" : ".63M";
			break;
	}
	if(longitude < 0) longitude = fabs(longitude) + 1;
	if(latitude  < 0) latitude  = fabs(latitude) + 1;
	int lo = static_cast<int>(longitude);
	int la = static_cast<int>(latitude);
	char lon[4];
	sprintf(lon, "%i", lo);
	string slon = string(lon);
	while (slon.length() < 3) slon = '0' + slon;
	char lat[3];
	sprintf(lat, "%i", la);
	string slat = string(lat);
	while(slat.length() < 2){
		slat = '0' + slat;
	}
	return ew + slon + path_separator + ew + slon + ns + slat + en;
}
//!---------------------------------------------------------------------------------------------

// Get Profile data
void Terrain::FillProfile(double longitude1, double latitude1, double longitude2, double latitude2){
	profile.clear();
	double az = C_PI * txAzimuth /180;
	double dist = distance;
	double nrd = dist / step;	// double number of steps
	int nr;						// integer number of steps
	if(nrd - floor(nrd) >= 0.5)
		nr = static_cast<int> (dist / step + 1);
	else
		nr = static_cast<int> (dist / step);
	double latA = 2 * C_PI * latitude1 / 360;
	double longA = 2 * C_PI * longitude1 / 360;
	if(nr <= 1){
		// Two points: Start - Finish
		//! because the last error is remembered in the object and the priority for the lack of
		//! height is higher:
		Zone pz1 = GetPointZone(longitude1, latitude1);	//! first, we calculate the zone
		Zone pz2 = GetPointZone(longitude2, latitude2); //! then we calculate the height
		short ph1 = GetPointHeight(longitude1, latitude1);
		short ph2 = GetPointHeight(longitude2, latitude2);
		profile.push_back(TrPoint(0.0, ph1, pz1));
		profile.push_back(TrPoint(dist, ph2, pz2));
	}
	else
		for(int i = 0; i <  nr; i++){
			double gama = dist / EARTH_RADIUS / (nr - 1) * i;
			double latN = asin(sin(latA) * cos(gama) + cos(latA) * sin(gama) * cos(az));
			double rtmp = (cos(gama) - sin(latA) * sin(latN)) / (cos(latA) * cos(latN));
			if(rtmp < -1) rtmp = -1;
			if(rtmp >  1) rtmp =  1;
			double longN = 0.0;
			if(longitude1 < longitude2)
				longN = longA + acos(rtmp);
			else
				longN = longA - acos(rtmp);

			//! because the last error is remembered in the object and the priority for the lack of
			//! height is higher:
			Zone pz = GetPointZone(180 * longN / C_PI , 180 * latN / C_PI);		//! first, we calculate the zone
			short ph = GetPointHeight(180 * longN / C_PI , 180 * latN / C_PI);	//! then we calculate the height
			TrPoint trPoint(i*step, ph, pz);
			profile.push_back(trPoint);

		}
}
//!---------------------------------------------------------------------------------------------


// Get zone of point from Morphological database.
// 0	- inland
// 1	- sea
// 4	- coastal area
Zone Terrain::GetPointZone(double longitude, double latitude){
    int lats  = static_cast<int> (latitude * 3600);							// change format of latitude in to seconds
    int longs = static_cast<int> (longitude * 3600);						// change format of longitude in to seconds
	string filename = GetFileName(longitude, latitude, FileProf::Morpho);	// get file name according to coordinates
	FILE* fileh = OpenMorpho(morphPath + path_separator + filename);		// open terrain file
	if(fileh == NULL){
		error = 4;			// in case of no file return 0
		return Zone::A2;	// 0 - inland
	}
	double blockx = (longs - floor(longitude) * 3600)*12/3600;	// get block number
	double blocky = (lats -  floor(latitude)  * 3600)*12/3600;	// get block number
	int block_nr = static_cast<int>(blocky) * 12 + static_cast<int>(blockx);	// block number from beginning
	int stepx = latitude < 50 ? 100 : 50;						// if latitude > 50, in the block we have 50 points in x direction
	double dotx = (blockx - floor(blockx))*stepx;				// point x coordinate inside the block
	double doty = (blocky - floor(blocky))*100;					// point y coordinate inside the block
	int dot_nr = static_cast<int>(doty)*(stepx+1)+static_cast<int>(dotx); // point number from beginning of the block
	long offset = block_nr * (stepx + 1)*202 + dot_nr * 2;		// offset of the first point in bytes
	fseek(fileh, offset, SEEK_SET);
	short h1;
	// read point
	if(fread(&h1, 2, 1, fileh)!=1){
		error = 2;
		return  Zone::A2; // 0 - inland
	}
	//  h1 = h1 & 255;  // filters 1st byte (101 or 5 should be used)
    h1 = h1 & 5;		// filters out unnecessary data
	return IntToZone(h1);
}
//!---------------------------------------------------------------------------------------------

void Terrain::CloseFiles(){
	for(map<string, FILE*>::iterator i = morphFiles.begin(); i != morphFiles.end(); ++i){
		fclose((*i).second);
	}
	for(map<string, FILE*>::iterator i = topoFiles.begin(); i != topoFiles.end(); ++i){
		fclose((*i).second);
	}
}
//!---------------------------------------------------------------------------------------------

bool Terrain::SaveProfileAsDAT(const string& filename){
	std::ofstream outFile(filename);
	std::stringstream ss;
	ss << "\n";
	for(TrPoint tp: profile){
		ss << tp.ToDAT();
	}
    outFile << ss.rdbuf();
    outFile.close();
    return true;
}
//!---------------------------------------------------------------------------------------------
