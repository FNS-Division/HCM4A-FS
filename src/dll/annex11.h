#ifndef __HCM4A_hcm4a_fs_annex11_h_
#define __HCM4A_hcm4a_fs_annex11_h_

#include <string>
#include <cstring>
#include <vector>
#include "hcmData.h"

using namespace std;

struct InTrigRec{
	double longitude;
	double latitude;
	double frequency_GHz;
	int h_asl;
	string border_path;
	void Clear(){
		longitude = NO_COORD;
		latitude = NO_COORD;
		frequency_GHz = 0;
		h_asl = 0;
		border_path = "";
	}
	string ToString()const{
		std::stringstream ss;
		ss << to_string("InTrigRec:", "");
		ss << to_string(" longitude", longitude);
		ss << to_string(" latitude", latitude);
		ss << to_string(" frequency_GHz", frequency_GHz);
		ss << to_string(" h_asl", h_asl);
		ss << to_string(" border_path", border_path);
		return ss.str();
	}
	InTrigRec(){Clear();}
	InTrigRec(const double longitude, const double latitude, const double frequency_GHz, const int h_asl, const string& border_path):
			  longitude(longitude), latitude(latitude), frequency_GHz(frequency_GHz), h_asl(h_asl), border_path(border_path){}
};
struct OutTrigRec{
	int error;
	string ctryList;
	void Clear(){
		error = 0;
		ctryList = "";
	}
	string ToString()const{
		std::stringstream ss;
		ss << to_string("OutTrigRec:", "");
		ss << to_string(" error", error);
		ss << to_string(" ctryList", ctryList);
		return ss.str();
	}
	OutTrigRec(){Clear();}
	OutTrigRec(const string& ctryList, int error): ctryList(ctryList), error(error){}
};

OutTrigRec GetTriggerCoutry(const InTrigRec& inTrigRec);

double GetDistance(double longitude1, double latitude1, double longitude2, double latitude2);
int TestTrigger(const string& filename, double frequency, double longitude, double latitude, int h_asl, bool& affected);
double Trigger(double frequency, int h_asl);
bool GetBorderFiles(const string& pathBorder, vector<string>& files);
string GetCoutryFromFileName(const string& filename);



#endif
