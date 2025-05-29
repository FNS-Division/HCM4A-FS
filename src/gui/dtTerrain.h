#ifndef __HCM4A_CalcFs4A_dtTerrain_h_
#define __HCM4A_CalcFs4A_dtTerrain_h_

#include <map>
#include "common.h"


// --- pathtype: 1 = 'los', 2 = 'transhorizon'
enum class PathType {los=1, thor=2};
// PathType::los [Line of Sight]
// PathType::thor - [Trans-Horizon]
inline PathType IntToPathType(int t){
	if(t == static_cast<int>(PathType::los))  return PathType::los;
	if(t == static_cast<int>(PathType::thor)) return PathType::thor;
	return PathType::los;
}
inline int PathTypeToInt(PathType t){
	return static_cast<int>(t);
}
inline String PathTypeToStr(PathType t){
	String rStr = "?";
	switch(t){
		case PathType::los:
			rStr = "Line of Sight";
			break;
		case PathType::thor:
			rStr = "Trans-Horizon";
			break;
	}
	return rStr;
}
inline PathType StrToPathType(const String& str){
	if(str == "Line of Sight")	return PathType::los;
	if(str == "Trans-Horizon")	return PathType::thor;
	return PathType::thor;
}
//! --------------------------------------------------------------------------------------------

enum class Zone {A2=0, B=1, A1=4};
// A2 - 0: Land away from the sea, Inland
// B  - 1: Sea
// A1 - 4: Coastal Land
inline Zone IntToZone(int z){
	if(z == static_cast<int>(Zone::A1))  return Zone::A1;
	if(z == static_cast<int>(Zone::A2))  return Zone::A2;
	if(z == static_cast<int>(Zone::B))   return Zone::B;
	return Zone::A2;
}
inline bool IsZone(int iz){
	return (iz >= static_cast<int>(Zone::A2) && iz <= static_cast<int>(Zone::A1));
}
inline bool IsZone(const String& str){
	return (str == "A1")||(str == "A2")||(str == "B");
}
inline int ZoneToInt(Zone z){
	return static_cast<int>(z);
}
inline String ZoneToStr(Zone z){
	String rStr = "A2";
	switch(z){
		case Zone::A1:
			rStr = "A1"; // "Coastal Land"
			break;
		case Zone::A2:
			rStr = "A2"; // "Inland"
			break;
		case Zone::B:
			rStr = "B"; // "Sea"
			break;
	}
	return rStr;
}
inline Zone StrToZone(const String& str){
	if(str == "A1")	return Zone::A1;
	if(str == "A2")	return Zone::A2;
	if(str == "B")	return Zone::B;
	return Zone::A2;
}
//! --------------------------------------------------------------------------------------------

enum class FileProf {Topo=0, Morpho=1};
//! --------------------------------------------------------------------------------------------

struct TrPoint: Moveable<TrPoint>{
	double dist;
    double height;	// Topo
	Zone   zone;	// Morpho
	void Clear(){dist = 0.0; height = 0.0; zone = Zone::A2;}
	String ToString()const{return Format("d: %f, h: %f, z: %s\n", dist, height, ZoneToStr(zone));};
	String ToCSV()const{return Format("%f; %f; %s\n", dist, height, ZoneToStr(zone));};
	TrPoint(){Clear();}
	TrPoint(const double dist, const short height, const Zone zone):dist(dist), height(height), zone(zone){};
	TrPoint(const TrPoint& pnt):dist(pnt.dist), height(pnt.height), zone(pnt.zone){};
};

struct FileCache{
	VectorMap<String, FileIn*> fileMap;	// Terrain:heights
	FileIn* Open(const String& filename){
		int idx = fileMap.Find(filename);
		if(idx == -1){
			FileIn f(filename);
			if(f.IsError() || f.IsEof())
				return NULL;
			fileMap.Add(filename, &f);
			return fileMap[fileMap.GetCount()-1];
		}
		else
			return fileMap[idx];
	}
	FileCache(){};
	~FileCache(){fileMap.clear();}
};


class Terrain: Moveable<Terrain>{
public:
	int    Error(){return error;};
	double	Step(){return step;};
	double	TxAzimuth(){return txAzimuth;};
	double	RxAzimuth(){return rxAzimuth;};
	double	Distance(){return distance;};
	bool	SaveProfileAsCSV(const String& filename);
	vector<TrPoint>* Profile(){return &profile;};
	vector<TrPoint>& GetProfile(){return profile;};
	static double GetDistance(double longitude1, double latitude1, double longitude2, double latitude2);
	String ToString()const;
	void	Initialise(double longitude1, double latitude1, double longitude2, double latitude2);
	void	Fill(std::vector<TrPoint>& profile, double longitude1, double latitude1, double longitude2, double latitude2);
	void	Fill(Vector<TrPoint>& profile, double longitude1, double latitude1, double longitude2, double latitude2);
	Terrain(const String& topoPath, const String& morphPath);
	Terrain();
	~Terrain();
private:
	int    error;
	double	step;
	double	txAzimuth;
	double	rxAzimuth;
	double	distance;
	String	topoPath;
	String	morphPath;
	vector<TrPoint> profile;

	void	FillProfile(double longitude1, double latitude1, double longitude2, double latitude2);
	String	GetFileName(double longitude, double latitude, FileProf fpType);
	short	GetPointHeight(double longitude, double latitude);
	Zone	GetPointZone(double longitude, double latitude);
	double	GetAzimuth(double longitude1, double latitude1, double longitude2, double latitude2);
};

//---------------------------------------------------------------------------
// Error codes:
//				0		- No error
//				1		- Error opening file (no data)
//				2		- Error reading record
//				4		- Morphological data not found, assume we have inland
//				5		- Height is missing (-9999)
//				6		- Distance = 0
//---------------------------------------------------------------------------


Terrain::Terrain(const String& topoPath, const String& morphPath): topoPath(topoPath), morphPath(morphPath){
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
}
//!---------------------------------------------------------------------------------------------

void Terrain::Initialise(double longitude1, double latitude1, double longitude2, double latitude2){
	error = 0;
	distance = GetDistance(longitude1, latitude1, longitude2, latitude2);
	if((longitude1 == longitude2 && latitude1 == latitude2) || distance == 0.0){
		error = 6;
		return;
	}
	txAzimuth = GetAzimuth(longitude1, latitude1, longitude2, latitude2);
	rxAzimuth = GetAzimuth(longitude2, latitude2, longitude1, latitude1);
	// Constructing path profile
	FillProfile(longitude1, latitude1, longitude2, latitude2);
}
//!---------------------------------------------------------------------------------------------

String Terrain::ToString()const{
	String str = "profile:\n";
	for(TrPoint tp: profile){
		str << tp.ToCSV();
	}
	return str;
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
	String filename = GetFileName(longitude, latitude, FileProf::Topo);	// get file name according to coordinates
	FileIn fileh(NativePath(topoPath + C_PATH_SEPAR + filename));	// open terrain file
	if(fileh.IsError() || fileh.IsEof()){
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
	int offset = block_nr * (stepx + 1) * 202 + dot_nr * 2;	// offset of the first point in bytes
	short h1, h2, h3, h4;
	fileh.Seek(offset);
	// read 1st point
	h1 = fileh.Get16();
	if(fileh.IsError() || fileh.IsEof()){
		error = 2;
		return 0;
	}
	// read 2nd point
	h2 = fileh.Get16();
	if(fileh.IsError() || fileh.IsEof()){
		error = 2;
		return 0;
	}
	// move cursor by 1 line - 2 data points
	fileh.SeekCur((stepx + 1 - 2)*2);
	// read 3rd point
	h3 = fileh.Get16();
	if(fileh.IsError() || fileh.IsEof()){
		error = 2;
		return 0;
	}
	// read 4th point
	h4 = fileh.Get16();
	if(fileh.IsError() || fileh.IsEof()){
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

// Get zone of point from Morphological database.
// 0	- inland
// 1	- sea
// 4	- coastal area
Zone Terrain::GetPointZone(double longitude, double latitude){
    int lats  = static_cast<int> (latitude * 3600);							// change format of latitude in to seconds
    int longs = static_cast<int> (longitude * 3600);						// change format of longitude in to seconds
	String filename = GetFileName(longitude, latitude, FileProf::Morpho);	// get file name according to coordinates
	FileIn fileh(NativePath(morphPath + C_PATH_SEPAR + filename));			// open terrain file
	if(fileh.IsError() || fileh.IsEof()){
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
	int offset = block_nr * (stepx + 1)*202 + dot_nr * 2;		// offset of the first point in bytes
	fileh.Seek(offset);
	short h1;
	// read point
	h1 = fileh.Get16();
	if(fileh.IsError() || fileh.IsEof()){
		error = 2;
		return  Zone::A2; // 0 - inland
	}
	//  h1 = h1 & 255;  // filters 1st byte (101 or 5 should be used)
    h1 = h1 & 5;		// filters out unnecessary data
	return IntToZone(h1);
}
//!---------------------------------------------------------------------------------------------


// Gets file name according to given coordinates.
// Returns empty String in case of error
String Terrain::GetFileName(double longitude, double latitude, FileProf fpType){
	char ew = longitude >= 0 ? 'E' : 'W';
	char ns = latitude >= 0 ? 'N' : 'S';
	String en = "";
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
	String slon = IntStr(static_cast<int>(longitude));
	while(slon.GetLength() < 3){
		slon = '0' + slon;
	}
	String slat = IntStr(static_cast<int>(latitude));
	while(slat.GetLength() < 2){
		slat = '0' + slat;
	}
	return NativePath(ew + slon + C_PATH_SEPAR + ew + slon + ns + slat + en);
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
		profile.push_back(TrPoint(0.0, GetPointHeight(longitude1, latitude1), GetPointZone(longitude1, latitude1)));
		profile.push_back(TrPoint(dist, GetPointHeight(longitude2, latitude2), GetPointZone(longitude2, latitude2)));
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
			Zone z = GetPointZone(180 * longN / C_PI , 180 * latN / C_PI);
			TrPoint trPoint(i*step, GetPointHeight(180 * longN / C_PI , 180 * latN / C_PI), GetPointZone(180 * longN / C_PI , 180 * latN / C_PI));
			profile.push_back(trPoint);
		}
}
//!---------------------------------------------------------------------------------------------

void Terrain::Fill(std::vector<TrPoint>& prof, double longitude1, double latitude1, double longitude2, double latitude2){
	prof.clear();
	double txAzim = GetAzimuth(longitude1, latitude1, longitude2, latitude2);
	double dist = GetDistance(longitude1, latitude1, longitude2, latitude2);
	double az = C_PI * txAzim /180;
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
		prof.push_back(TrPoint(0.0, GetPointHeight(longitude1, latitude1), GetPointZone(longitude1, latitude1)));
		prof.push_back(TrPoint(dist, GetPointHeight(longitude2, latitude2), GetPointZone(longitude2, latitude2)));
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
			Zone z = GetPointZone(180 * longN / C_PI , 180 * latN / C_PI);
			TrPoint trPoint(i*step, GetPointHeight(180 * longN / C_PI , 180 * latN / C_PI), GetPointZone(180 * longN / C_PI , 180 * latN / C_PI));
			prof.push_back(trPoint);
		}
}
void Terrain::Fill(Vector<TrPoint>& prof, double longitude1, double latitude1, double longitude2, double latitude2){
	prof.Clear();
	double txAzim = GetAzimuth(longitude1, latitude1, longitude2, latitude2);
	double dist = GetDistance(longitude1, latitude1, longitude2, latitude2);
	double az = C_PI * txAzim /180;
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
		prof.Add(TrPoint(0.0, GetPointHeight(longitude1, latitude1), GetPointZone(longitude1, latitude1)));
		prof.Add(TrPoint(dist, GetPointHeight(longitude2, latitude2), GetPointZone(longitude2, latitude2)));
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
			Zone z = GetPointZone(180 * longN / C_PI , 180 * latN / C_PI);
			TrPoint trPoint(i*step, GetPointHeight(180 * longN / C_PI , 180 * latN / C_PI), GetPointZone(180 * longN / C_PI , 180 * latN / C_PI));
			prof.Add(trPoint);
		}
}
//!---------------------------------------------------------------------------------------------

bool Terrain::SaveProfileAsCSV(const String& filename){
	FileOut f_out(filename);
	if(!f_out) return false;
	for(TrPoint tp: profile){
		f_out.PutLine(tp.ToCSV());
	}
	if(!f_out.IsError()){
		f_out.Close();
		return true;
	}else{
		f_out.Close();
		return false;
	}
}
//!---------------------------------------------------------------------------------------------

// d (km),h(m),Ground cover height (m),zone: A1=Coastal Land/A2=Inland/B=Sea,zone: 1=Coastal Land/2=Inland/3=Sea
struct ProfileRec: Moveable<ProfileRec>{
	int npp;
	double d;		// d (km)
	double h;		// h(m)
	double g;		// Ground cover height (m)
	int zone;		// zone:  1 = Coastal Land /  2 = Inland / 3 = Sea
	String sZone;	// zone: A1 = Coastal Land / A2 = Inland / B = Sea
	void Clear(){npp=0;d=0;h=0;g=0;zone=0;sZone="";};
	ProfileRec(){Clear();};
	ProfileRec(const int npp, const double d, const double h, const double g, const int zone, const String& sZone):
				npp(npp), d(d), h(h), g(g), zone(zone), sZone(sZone){};
};
//!---------------------------------------------------------------------------------------

struct ProfileData: Moveable<ProfileData>{
	Vector<ProfileRec> LoadFromCSV(const String& fname, const char cRazdel = ',');
	Vector<ProfileRec> LoadFromTRS(const String& fname, const char cRazdel = ';');
	Vector<ProfileRec> LoadFromDAT(const String& fname, const char cRazdel = ';');
	int ToSdtVec(const Vector<ProfileRec>& pList, std::vector<double>& dList, std::vector<double>& hList, std::vector<double>& gList, std::vector<int>& zList);
	int ToPointVec(const Vector<ProfileRec>& pList,  std::vector<TrPoint>& trList);
};
Vector<ProfileRec> ProfileData::LoadFromCSV(const String& fname, const char cRazdel){
	Vector<ProfileRec> pList;
	if(!FileExists(fname)){
		LOG(Format("Error: File %s, not found", fname));
		return pList;
	}
	FileIn f_in(fname);
	int lcnt = 0;
	pList.Clear();
    while(!f_in.IsEof()){
        lcnt++;
		auto v = GetCsvLine(f_in, cRazdel, CHARSET_DEFAULT);
        if(lcnt == 1) continue; // skeep first line str, Title: d (km),h(m),Ground cover height (m),zone: A1=Coastal Land/A2=Inland/B=Sea,zone: 1=Coastal Land/2=Inland/3=Sea
		Vector<String> strList;
		for(auto e : v){
			strList.Add(e);
		}
		if(strList.GetCount()){
			ProfileRec pRec;
			pRec.npp = lcnt-1;
			for(int i=0; i<strList.GetCount(); i++){
				if(i==0) pRec.d = (StrDbl(TrimBoth(strList[i])));	// d(km)
				if(i==1) pRec.h = (StrDbl(TrimBoth(strList[i])));	// h(m)
				if(i==2) pRec.g = (StrDbl(TrimBoth(strList[i])));	// Ground cover height (m)
				//if(i==3)// zone: A1=Coastal Land/A2=Inland/B=Sea
				if(i==4) pRec.zone = (StrInt(TrimBoth(strList[i])));	// zone: 1=Coastal Land/2=Inland/3=Sea
			}
			pList.Add(pRec);
		}
    }
	return pList;
}
int ProfileData::ToSdtVec(const Vector<ProfileRec>& pList,
						  std::vector<double>& dList, std::vector<double>& hList, std::vector<double>& gList, std::vector<int>& zList){
	for(int i=0; i<pList.GetCount(); i++){
		dList.push_back(pList[i].d);
		hList.push_back(pList[i].h);
		gList.push_back(pList[i].g);
		zList.push_back(pList[i].zone);
	}
	return dList.size();
}
int ProfileData::ToPointVec(const Vector<ProfileRec>& pList,  std::vector<TrPoint>& trList){
	for(int i=0; i<pList.GetCount(); i++){
		trList.push_back(TrPoint(pList[i].d, pList[i].h, IntToZone(pList[i].zone)));
	}
	return trList.size();
}
//!---------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------


#endif
