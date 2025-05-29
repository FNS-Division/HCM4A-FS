//---------------------------------------------------------------------------
// Error codes
//  1: No borderline data found
//  2: Bad coordinates
//  3: Bad frequency
//  4: Error opening file
//  5: Error reading file

#ifdef _WIN32
	#include <io.h>
#else
	#include <dirent.h>
#endif

#include "annex11.h"
#include "common.h"
#include <cstdlib>

// Calculation of Distance
double GetDistance(double longitude1, double latitude1, double longitude2, double latitude2){
  return EARTH_RADIUS * acos(sin(C_PI * latitude1 / 180) * sin(C_PI * latitude2 / 180) + cos(C_PI * latitude1 / 180) *
		 cos(C_PI * latitude2 / 180) * cos((C_PI * longitude1 / 180 - C_PI * longitude2 / 180)));
}
//!---------------------------------------------------------------------------------------------

// Errors
// 4: Error opening file
// 5: Error reading file
int TestTrigger(const string& filename, double frequency, double longitude, double latitude, int h_asl, bool& affected){
	affected = false;
	FILE* f = fopen(filename.c_str(), "rb");
	if (!f) return 4; // Error opening file
	while(!feof(f)){
	    double longrad= 0.0, latrad = 0.0;
	    size_t a = fread(&longrad, 8, 1, f);
		if(feof(f)){
			fclose(f);
			return 0;
		}
		if(a != 1){
			fclose(f);
			return 5; // Error reading file
		}
		a = fread(&latrad, 8, 1, f);
		if(feof(f)){
			fclose(f);
			return 0;
		}
		if(a != 1){
			fclose(f);
			return 5; // Error reading file
		}
		double distanceT = Trigger(frequency, h_asl);
		double distanceC = GetDistance(longitude,latitude,longrad * 180 / C_PI, latrad * 180 / C_PI);
		if(distanceT >= distanceC){
			affected = true;
			fclose(f);
			return 0;
		}
	}
	affected = false;
	fclose(f);
	return 0;
}
//--------------------------------------------------------------------------

double Trigger(double frequency, int h_asl){
	if(frequency <=  5){
		if(h_asl < 300)
			return 100;
		else
			return 200.0;
	}
	else if(frequency <= 10){
		if(h_asl < 300)
			return 100;
		else
			return 150.0;
	}
	else if (frequency <= 12  ) return 100.0;
	else if (frequency <= 20  ) return  80.0;
	else if (frequency <= 24.5) return  60.0;
	else if (frequency <= 30  ) return  40.0;
	else if (frequency <= 39.5) return  30.0;
	return  20.0;
}
//--------------------------------------------------------------------------

OutTrigRec GetTriggerCoutry(const InTrigRec& inTrigRec){
	OutTrigRec outTrigRec;
	vector<string> files;
	if(!GetBorderFiles(inTrigRec.border_path, files)){
		outTrigRec.error = 1;
		return outTrigRec;
	}
	bool affected;
	int cnt = 0;
	for(int i = 0; i < files.size(); i++){
		outTrigRec.error = TestTrigger(inTrigRec.border_path + path_separator + files[i],
						   inTrigRec.frequency_GHz, inTrigRec.longitude, inTrigRec.latitude, inTrigRec.h_asl, affected);
		if(outTrigRec.error != 0){
			return outTrigRec;
		}
		if(affected){
			string c = GetCoutryFromFileName(files[i]);
			if(!c.empty()){
				cnt++;
				if(cnt==1)
					outTrigRec.ctryList = c;
				else
					outTrigRec.ctryList += S_RAZDEL + c;
			}
		}
	}
	return outTrigRec;
}
//--------------------------------------------------------------------------


bool GetBorderFiles(const string& pathBorder, vector<string>& files){
#ifdef _WIN32
	_finddata_t fileinfo;
	intptr_t ptr = _findfirst((pathBorder + path_separator + "*.ALL").c_str() ,&fileinfo);
	if(ptr == -1) return false;
	int b = 0;
	while(b == 0){
		if(fileinfo.attrib != _A_SUBDIR){
			files.push_back(string(fileinfo.name));
		}
		b = _findnext(ptr, &fileinfo);
	}
	if(_findclose(ptr) != 0) return false;
	return true;

#else
	DIR *dp;
	struct dirent *dirp;
	if((dp  = opendir(pathBorder.c_str())) == NULL) return false;
	while ((dirp = readdir(dp)) != NULL){
		string folder = string(dirp->d_name);
		if(folder[folder.size() - 1] != '.') // skip file names . and ..
			files.push_back(folder);
	}
	closedir(dp);
	return true;
#endif
}
//--------------------------------------------------------------------------

string GetCoutryFromFileName(const string& filename){
	if(filename.size() < 4) return string();
	string out = filename.substr(0, filename.size() - 4);
	return out;
}
//--------------------------------------------------------------------------

