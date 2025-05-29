#ifndef __HCM4A_hcm4a_fs_hcmData_h_
#define __HCM4A_hcm4a_fs_hcmData_h_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

#include "antData.h"
#include "common.h"
//! --------------------------------------------------------------------------------------------

struct TrMask{
	double frequency;
	double attenuation;
	void Clear(){
		frequency = 0.0;
		attenuation = 0.0;
	}
	string ToPrint()const{
		std::stringstream ss;
		ss << "\t frequency: " << to_string(frequency) << "\t attenuation: " << to_string(attenuation) << "\n";
		return ss.str();
	}
	string ToString()const{
		std::stringstream ss;
		ss << "\t\t\t\t=\t" << to_string(frequency) << "\t" << to_string(attenuation) << "\n";
		return ss.str();
	}
	TrMask(){Clear();}
	TrMask(const double frequency, const double attenuation):frequency(frequency), attenuation(attenuation){}
	TrMask(const TrMask& trMask): frequency(trMask.frequency), attenuation(trMask.attenuation){}
};
//! --------------------------------------------------------------------------------------------

struct HcmRec{
	bool  isTx;					// true: Tx=transmitter, false: Rx=receiver
	bool  isPassive;			// Ptx=passive transmitter, Prx=passive receiver
	double frequency;			// Frequency unit G: GHz
	string name_of_station;
	string country;
	double latitude;
	double longitude;
	short	h_asl;				// Height above sea
	string eqp_manuf_name;
	string eqp_type;
	string capacity;
	string eqclass;
	double channel_spacing;
	double power;
	double atpc;
	double azimuth;
	double elevation;
	string polarization;
	double noise;
	double losses;
	short h_ant;
	string ant_manuf_name;
	string ant_type;
	double gain;
	string cp_pattern;
	string xp_pattern;
	int error;
	vector<TrMask> mask;
	vector<AntPoint> cpData;
	vector<AntPoint> xpData;
	bool IsEmpty(){
		return !frequency && latitude == NO_COORD && longitude == NO_COORD;
	}
	void Clear(){
		isTx = false;
		isPassive = false;
		frequency = 0.0;
		name_of_station = "";
		country = "";
		latitude = NO_COORD;
		longitude = NO_COORD;
		h_asl = 0;
		eqp_manuf_name = "";
		eqp_type = "";
		capacity = "";
		eqclass = "";
		channel_spacing = 0.0;
		power = 0.0;
		atpc = 0.0;
		azimuth = 0.0;
		elevation = 0.0;
		polarization = "";
		noise = 0.0;
		losses = 0.0;
		h_ant = 0;
		ant_manuf_name = "";
		ant_type = "";
		gain = 0.0;
		cp_pattern = "";
		xp_pattern = "";
		error = 0;
		mask.clear();
		cpData.clear();
		xpData.clear();
	}
	string ToPrint()const;
	string ToString()const{
	    std::stringstream ss;
		ss << to_string("isTx", isTx);
		ss << to_string("isPassive", isPassive);
		ss << to_string("frequency", frequency);
		ss << to_string("name_of_station", name_of_station);
		ss << to_string("country", country);
		ss << to_string("latitude", latitude);
		ss << to_string("longitude", longitude);
		ss << to_string("h_asl", h_asl);
		ss << to_string("eqp_manuf_name", eqp_manuf_name);
		ss << to_string("eqp_type", eqp_type);
		ss << to_string("capacity", capacity);
		ss << to_string("eqclass", eqclass);
		ss << to_string("channel_spacing", channel_spacing);
		ss << to_string("power", power);
		ss << to_string("atpc", atpc);
		ss << to_string("azimuth", azimuth);
		ss << to_string("elevation", elevation);
		ss << to_string("polarization", polarization);
		ss << to_string("noise", noise);
		ss << to_string("losses", losses);
		ss << to_string("h_ant", h_ant);
		ss << to_string("ant_manuf_name", ant_manuf_name);
		ss << to_string("ant_type", ant_type);
		ss << to_string("gain", gain);
		ss << to_string("cp_pattern", cp_pattern);
		ss << to_string("xp_pattern", xp_pattern);
		ss << to_string("error", error);
		ss << to_string("mask", mask);
		ss << to_string("cpData", cpData);
		ss << to_string("xpData", xpData);
		ss << to_string("\n\n");
		return ss.str();
	};
	HcmRec(){Clear();}
	HcmRec(const bool isTx, const bool isPassive, const double frequency, const std::string& name_of_station,
			   const std::string& country, const double latitude, const double longitude, const short h_asl,
			   const std::string& eqp_manuf_name, const std::string& eqp_type, const std::string& capacity,
			   const std::string& eqclass, const double channel_spacing, const double power, const double atpc,
			   const double azimuth, const double elevation, const std::string& polarization, const double noise,
			   const double losses, const short h_ant, const std::string& ant_manuf_name, const std::string& ant_type,
			   const double gain, const std::string& cp_pattern, const std::string& xp_pattern, const int error,
			   std::vector<TrMask>& mask, std::vector<AntPoint>& cpData, std::vector<AntPoint>& xpData):
			   isTx(isTx),	isPassive(isPassive), frequency(frequency),	name_of_station(name_of_station), country(country),
			   latitude(latitude),	longitude(longitude), h_asl(h_asl),	eqp_manuf_name(eqp_manuf_name),	eqp_type(eqp_type),
			   capacity(capacity),	eqclass(eqclass), channel_spacing(channel_spacing),	power(power), atpc(atpc),
			   azimuth(azimuth), elevation(elevation),	polarization(polarization),	noise(noise), losses(losses),
			   h_ant(h_ant), ant_manuf_name(ant_manuf_name), ant_type(ant_type), gain(gain), cp_pattern(cp_pattern),
			   xp_pattern(xp_pattern), error(error), mask(mask), cpData(cpData), xpData(xpData){};
	
	HcmRec(const HcmRec& hcmRec):isTx(hcmRec.isTx), isPassive(hcmRec.isPassive), frequency(hcmRec.frequency),
								 name_of_station(hcmRec.name_of_station), country(hcmRec.country),
								 latitude(hcmRec.latitude), longitude(hcmRec.longitude), h_asl(hcmRec.h_asl),
								 eqp_manuf_name(hcmRec.eqp_manuf_name), eqp_type(hcmRec.eqp_type), capacity(hcmRec.capacity),
								 eqclass(hcmRec.eqclass), channel_spacing(hcmRec.channel_spacing), power(hcmRec.power),
								 atpc(hcmRec.atpc), azimuth(hcmRec.azimuth), elevation(hcmRec.elevation), polarization(hcmRec.polarization),
								 noise(hcmRec.noise), losses(hcmRec.losses), h_ant(hcmRec.h_ant), ant_manuf_name(hcmRec.ant_manuf_name),
								 ant_type(hcmRec.ant_type), gain(hcmRec.gain), cp_pattern(hcmRec.cp_pattern), xp_pattern(hcmRec.xp_pattern),
								 error(hcmRec.error), mask(hcmRec.mask), cpData(hcmRec.cpData), xpData(hcmRec.xpData){};
};
//! --------------------------------------------------------------------------------------------


int	    FillHcmData(TStdAnt& stdAnt, const string& hsmString, HcmRec& data, string& report, bool doReport);
int		HcmToTrigger(const string& hsmString, double& frequency_GHz, double& longitude, double& latitude, int& h_asl);
double	ToCoordinate(const string& coordinate, int& error);
bool	AnCompare(const AntPoint& elem1, const AntPoint& elem2);
int		SetCP(HcmRec& data, TStdAnt& stdAnt, string& report);
int		SetXP(HcmRec& data, TStdAnt& stdAnt, string& report);

string	Trim(const string& s);
double	StrToDouble(const string& s, int& error);
int		StrToLong(const string& s, int& error);
//!---------------------------------------------------------------------------------------------


#endif
