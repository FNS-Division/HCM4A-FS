#ifndef __HCM4A_FS_terrain_h_
#define __HCM4A_FS_terrain_h_

#include <iostream>
#include <map>
#include <stdio.h>
#include <vector>
#include "common.h"

using namespace std;


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
inline bool IsZone(const string& str){
	return (str == "A1")||(str == "A2")||(str == "B");
}
inline int ZoneToInt(Zone z){
	return static_cast<int>(z);
}
inline string ZoneToStr(Zone z){
	string rStr = "A2";
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
inline Zone StrToZone(const string& str){
	if(str == "A1")	return Zone::A1;
	if(str == "A2")	return Zone::A2;
	if(str == "B")	return Zone::B;
	return Zone::A2;
}
//! --------------------------------------------------------------------------------------------

enum class FileProf {Topo=0, Morpho=1};
//! --------------------------------------------------------------------------------------------

struct TrPoint{
	double dist;
    double  height;	// Topo
	Zone    zone;	// Morpho
	TrPoint(){
		dist = 0.0;
		height = 0.0;
		zone = Zone::A2;
	}
	string ToString()const{return "d: " + to_string(dist) + ", h: " + to_string(height) + ", z: " + ZoneToStr(zone) + "\n";};
	string ToDAT()const{return to_string(dist) + "; " + to_string(height) + "; " + ZoneToStr(zone) + "\n";};
	TrPoint(const double dist, const short height, const Zone zone):dist(dist), height(height), zone(zone){};
};


class Terrain{
public:
	int    Error(){return error;};
	double	Step(){return step;};
	double	TxAzimuth(){return txAzimuth;};
	double	RxAzimuth(){return rxAzimuth;};
	double	Distance(){return distance;};
	void	CloseFiles();
	bool	SaveProfileAsDAT(const string& filename);
	vector<TrPoint>* Profile(){return &profile;};
	vector<TrPoint>& GetProfile(){return profile;};
	double GetDistance(double longitude1, double latitude1, double longitude2, double latitude2);
	void	Initialise(double longitude1, double latitude1, double longitude2, double latitude2);
	Terrain(const string& topoPath, const string& morphPath);
	Terrain();
	~Terrain();
private:
	int    error;
	double	step;
	double	txAzimuth;
	double	rxAzimuth;
	double	distance;
	string	topoPath;
	string	morphPath;
	map<string, FILE*> topoFiles;	// Terrain:heights
	map<string, FILE*> morphFiles;	// Terrain:zone
	vector<TrPoint> profile;

	FILE*	OpenTopo(const string& filename);
	FILE*	OpenMorpho(const string& filename);
	void	FillProfile(double longitude1, double latitude1, double longitude2, double latitude2);
	string	GetFileName(double longitude, double latitude, FileProf fpType);
	short	GetPointHeight(double longitude, double latitude);
	//short	GetPointHeight2(double longitude, double latitude);
	Zone	GetPointZone(double longitude, double latitude);
	double	GetAzimuth(double longitude1, double latitude1, double longitude2, double latitude2);
};



#endif
