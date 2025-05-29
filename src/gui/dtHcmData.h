#ifndef __HCM4A_CalcFs4A_dtHcmData_h_
#define __HCM4A_CalcFs4A_dtHcmData_h_

#include "common.h"
#include "dtHcmError.h"
#include "dtAntData.h"
#include "dtHcmIN.h"
#include "dtTerrain.h"

using namespace HCMFS;

extern HcmErrorMap appErrorMap;

int StrToDblCoord(const String& vstr, double& coord, HcmError& tError);
double StrToDblCoord(const String& vstr);

int	HcmToTrigger(const String& hsmString, double& frequency_GHz, double& longitude, double& latitude, int& h_asl, HcmError& tError);
//!---------------------------------------------------------------------------------------------

struct DblCoordRec: Moveable<DblCoordRec>{
	double longitude;	// Geo. co-ordinates longitude
	double latitude;	// Geo. co-ordinates latitude
	void Clear(){
		latitude = NO_COORD;
		longitude = NO_COORD;
	}
	String ToString()const{
		return Format("\tlongitude: %f\tlatitude: %f", longitude, latitude);
	}
	unsigned GetHashValue() const { return CombineHash(longitude, latitude); }
    friend bool operator== (const DblCoordRec &c1, const DblCoordRec &c2){return (c1.longitude == c2.longitude && c1.latitude == c2.latitude);};
	DblCoordRec(){Clear();}
	DblCoordRec(double longitude, double latitude): longitude(longitude), latitude(latitude){}
	DblCoordRec(const DblCoordRec& coRec): longitude(coRec.longitude), latitude(coRec.latitude){}
};

struct StrCoordRec: Moveable<StrCoordRec>{
	String longitude;	// Geo. co-ordinates longitude
	String latitude;	// Geo. co-ordinates latitude
	void Clear(){
		longitude = "";
		latitude = "";
	}
	bool IsEmpty(){return longitude.IsEmpty() || latitude.IsEmpty();}
	String ToString()const{
		return Format("\tlongitude: %s\tlatitude: %s", longitude, latitude);
	}
	DblCoordRec ToDblCoord()const{
		return DblCoordRec(StrToDblCoord(longitude), StrToDblCoord(latitude));
	}
	unsigned GetHashValue() const { return CombineHash(longitude, latitude); }
    friend bool operator== (const StrCoordRec &c1, const StrCoordRec &c2){return (c1.longitude == c2.longitude && c1.latitude == c2.latitude);};
	StrCoordRec(){Clear();}
	StrCoordRec(const String& longitude, const String& latitude): longitude(longitude), latitude(latitude){}
	StrCoordRec(const StrCoordRec& coRec): longitude(coRec.longitude), latitude(coRec.latitude){}
};

struct TwoStrCoord: Moveable<TwoStrCoord>{
	StrCoordRec coord_1;
	StrCoordRec coord_2;
	void Clear(){
		coord_1.Clear();
		coord_2.Clear();
	}
	bool IsEmpty(){return coord_1.IsEmpty() || coord_2.IsEmpty();}
	String ToString()const{
		return Format("coord_1: %s\ncoord_2: %s\n", coord_1.ToString(), coord_2.ToString());
	}
	unsigned GetHashValue() const { return CombineHash(coord_1.GetHashValue(), coord_2.GetHashValue()); }
    friend bool operator== (const TwoStrCoord &c1, const TwoStrCoord &c2){return (c1.coord_1 == c2.coord_1 && c1.coord_2 == c2.coord_2) ||
																				 (c1.coord_1 == c2.coord_2 && c1.coord_2 == c2.coord_1) ;};
	TwoStrCoord(){Clear();}
	TwoStrCoord(const StrCoordRec& coord_1, const StrCoordRec& coord_2): coord_1(coord_1), coord_2(coord_2){}
	TwoStrCoord(const TwoStrCoord& twoRec): coord_1(twoRec.coord_1), coord_2(twoRec.coord_2){}
};
//!---------------------------------------------------------------------------------------------

/*
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
*/

struct TerraCache: Moveable<TerraCache>{
	String topoPath;
	String morphPath;
	VectorMap<TwoStrCoord, Terrain> terraMap;
	vector<TrPoint>& GetVec(const TwoStrCoord& twoCoord){
		int idx = terraMap.Find(twoCoord);
		if(idx != -1){
			return terraMap[idx].GetProfile();
		}
		else{
			Terrain terra(topoPath, morphPath);
			terra.Initialise(twoCoord.coord_1.ToDblCoord().longitude, twoCoord.coord_1.ToDblCoord().latitude,
							 twoCoord.coord_2.ToDblCoord().longitude, twoCoord.coord_2.ToDblCoord().latitude);
			terraMap.Add(twoCoord, terra);
			return terraMap[terraMap.GetCount()-1].GetProfile();
		}
	}
	Terrain& GetTerra(const TwoStrCoord& twoCoord){
		int idx = terraMap.Find(twoCoord);
		if(idx != -1){
			LOG("--- Find: " << twoCoord.ToString());
			return terraMap[idx];
		}
		else{
			Terrain terra(topoPath, morphPath);
			terra.Initialise(twoCoord.coord_1.ToDblCoord().longitude, twoCoord.coord_1.ToDblCoord().latitude,
							 twoCoord.coord_2.ToDblCoord().longitude, twoCoord.coord_2.ToDblCoord().latitude);
			terraMap.Add(twoCoord, terra);
			LOG("Add: " << twoCoord.ToString());
			return terraMap[terraMap.GetCount()-1];
		}
	}
	TerraCache(const String& topoPath, const String& morphPath):topoPath(topoPath), morphPath(morphPath){terraMap.Clear();}
};
//!---------------------------------------------------------------------------------------------

struct TrMask: Moveable<TrMask>{
	double frequency;
	double attenuation;
	void Clear(){
		frequency = 0.0;
		attenuation = 0.0;
	}
	String ToPrint()const{
		return Format("\n\t frequency: %f\t attenuation: %f", frequency, attenuation);
	}
	String ToString()const{
		return Format("\t\t\t\t=\t%f\t%f\n", frequency, attenuation);
	}
	std::string ToStdString()const{
		std::stringstream ss;
		ss << "\t frequency: " << to_string(frequency) << "\t attenuation: " << to_string(attenuation) << "\n";
		return ss.str();
	}
	TrMask(){Clear();}
	TrMask(const double frequency, const double attenuation):frequency(frequency), attenuation(attenuation){}
	TrMask(const TrMask& trMask): frequency(trMask.frequency), attenuation(trMask.attenuation){}
};
//! --------------------------------------------------------------------------------------------


// Data exchange record format for the Fixed Service

struct HcmRec: Moveable<HcmRec>{
	bool   isTx;				// true: Tx=transmitter, false: Rx=receiver
	bool   isPassive;			// Ptx=passive transmitter, Prx=passive receiver
	double frequency;			// Frequency unit G: GHz
	String name_of_station;
	String country;
	double latitude;			// Geo. co-ordinates latitude
	double longitude;			// Geo. co-ordinates longitude
	String sLat;				// Geo. co-ordinates latitude
	String sLong;				// Geo. co-ordinates longitude
	int  h_asl;					// Height above sea
	String eqp_manuf_name;		// EQUIPMENT MANUFACTURER NAME
	String eqp_type;			// EQUIPMENT TYPE
	String capacity;			// MAX. CAPACITY OF THE LINK
	String eqclass;				// EQUIPMENT CLASS
	double channel_spacing;		// CHANNEL SPACING
	double power;				// Maximum power
	double atpc;				// ATPC
	double azimuth;				// Azimuth
	double elevation;			// Elevation
	String polarization;		// POLARIZATION
	double noise;				// Noise power level FkTB
	double losses;				// Branches + line losses
	int h_ant;					// Antenna height
	String ant_manuf_name;		// ANTENNA MANUFACTURER NAME
	String ant_type;			// ANTENNA TYPE
	String coord_ref;			// 43
	
	double gain;				// Antenna gain
	String cp_pattern;			// Antenna cop. pattern
	String xp_pattern;			// ANTENNA CROSSPOLAR RADIATION PATTERN
	Vector<TrMask> mask;		// Spectrum mask
	Vector<AntPoint> cpData;
	Vector<AntPoint> xpData;
	HcmError tError;
	void Clear();
	bool IsEmpty(){return frequency <= 0.0 || latitude==NO_COORD || longitude==NO_COORD;};
	String ToString()const;
	String ToPrint()const;
	StrCoordRec GetCoord()const{return StrCoordRec(sLong, sLat);}
	String GetReport()const{return String() << "\n\n" << repStr << "\n\n";};
	//enum class HcmEntity {None, TX, PRX, PTX, RX};
	HcmEntity GetEntity(){return ToHcmEntity(isTx, isPassive);};
	void Set(const HcmRec& hcmRec);
	int Fill(const HcmIN& hcmIN);
	int Fill(const TStdAnt& stdAnt, const HcmIN& hcmIN);
	HcmRec(){Clear();}
	HcmRec(const TStdAnt& stdAnt, const HcmIN& hcmIN){Fill(stdAnt, hcmIN);}
	HcmRec(const HcmIN& hcmIN){Fill(hcmIN);}
	HcmRec(const HcmRec& hcmRec):
		isTx(hcmRec.isTx),
		isPassive(hcmRec.isPassive),
		frequency(hcmRec.frequency),
		name_of_station(hcmRec.name_of_station),
		country(hcmRec.country),
		latitude(hcmRec.latitude),
		longitude(hcmRec.longitude),
		sLat(hcmRec.sLat),
		sLong(hcmRec.sLong),
		h_asl(hcmRec.h_asl),
		eqp_manuf_name(hcmRec.eqp_manuf_name),
		eqp_type(hcmRec.eqp_type),
		capacity(hcmRec.capacity),
		eqclass(hcmRec.eqclass),
		channel_spacing(hcmRec.channel_spacing),
		power(hcmRec.power),
		atpc(hcmRec.atpc),
		azimuth(hcmRec.azimuth),
		elevation(hcmRec.elevation),
		polarization(hcmRec.polarization),
		noise(hcmRec.noise),
		losses(hcmRec.losses),
		h_ant(hcmRec.h_ant),
		ant_manuf_name(hcmRec.ant_manuf_name),
		ant_type(hcmRec.ant_type),
		coord_ref(hcmRec.coord_ref),
		gain(hcmRec.gain),
		cp_pattern(hcmRec.cp_pattern),
		xp_pattern(hcmRec.xp_pattern),
		mask(clone(hcmRec.mask)),
		cpData(clone(hcmRec.cpData)),
		xpData(clone(hcmRec.xpData)),
		isReport(hcmRec.isReport),
		un(hcmRec.un),
		repStr(hcmRec.repStr),
		tError(hcmRec.tError){};
protected:
	bool isReport;
	String un;
	String repStr;
	bool isSetStdCP;
	bool isSetStdXP;

	void ToReport(const String& caption, const String& entry, const String& vstr, String& repStr)const;
	void ToReport(const String& str, String& repStr)const;

	int SetCP(const TStdAnt& stdAnt);
	int SetXP(const TStdAnt& stdAnt);
	int SetTypeOfEntry(const HcmIN& hcmIN);
	int SetFrequency(const HcmIN& hcmIN);
	int SetFreqUnit(const HcmIN& hcmIN);
	int SetCoord(const HcmIN& hcmIN);
	int SetStationHeight(const HcmIN& hcmIN);
	int SetTransSpectrumMask(const HcmIN& hcmIN);
	int SetEqupmentClass(const HcmIN& hcmIN);
	int SetChannelSpacing(const HcmIN& hcmIN);
	int Set78MaskElement(const HcmIN& hcmIN);
	int SetMaxPermTrPower(const HcmIN& hcmIN);
	int SetAtrc(const HcmIN& hcmIN);
	int SetAzimuth(const HcmIN& hcmIN);
	int SetElevation(const HcmIN& hcmIN);
	int SetPolarization(const HcmIN& hcmIN);
	int SetRxPowerLevel(const HcmIN& hcmIN);
	int SetBranchAndLineLoss(const HcmIN& hcmIN);
	int SetAntGroundHeight(const HcmIN& hcmIN);
	int SetAntGain(const HcmIN& hcmIN);
	int SetCpPattern(const HcmIN& hcmIN);
	int SetCpNumOfMaskData(const HcmIN& hcmIN);
	int SetCpPatternData(const HcmIN& hcmIN);
	int SetXpPattern(const HcmIN& hcmIN);
	int SetXpNumOfMaskData(const HcmIN& hcmIN);
	int SetXpPatternData(const HcmIN& hcmIN);
	int SetStdCpXpPatternData(const TStdAnt& stdAnt, const HcmIN& hcmIN);
};
void HcmRec::Clear(){
	isTx = false;
	isPassive = false;
	frequency = 0.0;
	name_of_station = "";
	country = "";
	latitude = NO_COORD;
	longitude = NO_COORD;
	sLat = "";
	sLong = "";
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
	coord_ref = "";
	gain = 0.0;
	cp_pattern = "";
	xp_pattern = "";
	mask.Clear();
	cpData.Clear();
	xpData.Clear();
	
	isReport = true;
	un = "";
	repStr = "";
}
void HcmRec::Set(const HcmRec& hcmRec){
	isTx = hcmRec.isTx;
	isPassive = hcmRec.isPassive;
	frequency = hcmRec.frequency;
	name_of_station = hcmRec.name_of_station;
	country = hcmRec.country;
	latitude = hcmRec.latitude;
	longitude = hcmRec.longitude;
	sLat = hcmRec.sLat;
	sLong = hcmRec.sLong;
	h_asl = hcmRec.h_asl;
	eqp_manuf_name = hcmRec.eqp_manuf_name;
	eqp_type = hcmRec.eqp_type;
	capacity = hcmRec.capacity;
	eqclass = hcmRec.eqclass;
	channel_spacing = hcmRec.channel_spacing;
	power = hcmRec.power;
	atpc = hcmRec.atpc;
	azimuth = hcmRec.azimuth;
	elevation = hcmRec.elevation;
	polarization = hcmRec.polarization;
	noise = hcmRec.noise;
	losses = hcmRec.losses;
	h_ant = hcmRec.h_ant;
	ant_manuf_name = hcmRec.ant_manuf_name;
	ant_type = hcmRec.ant_type;
	coord_ref = hcmRec.coord_ref;
	gain = hcmRec.gain;
	cp_pattern = hcmRec.cp_pattern;
	xp_pattern = hcmRec.xp_pattern;
	mask = clone(hcmRec.mask);
	cpData = clone(hcmRec.cpData);
	xpData = clone(hcmRec.xpData);
	isReport = hcmRec.isReport;
	un = hcmRec.un;
	repStr = hcmRec.repStr;
	tError = hcmRec.tError;
}
String HcmRec::ToString()const{
	String ss = "";
	ss << ToStr("isTx", isTx);
	ss << ToStr("isPassive", isPassive);
	ss << ToStr("frequency", frequency);
	ss << ToStr("name_of_station", name_of_station);
	ss << ToStr("country", country);
	ss << ToStr("latitude", latitude);
	ss << ToStr("longitude", longitude);
	ss << ToStr("sLat", sLat);
	ss << ToStr("sLonge", sLong);
	ss << ToStr("h_asl", h_asl);
	ss << ToStr("eqp_manuf_name", eqp_manuf_name);
	ss << ToStr("eqp_type", eqp_type);
	ss << ToStr("capacity", capacity);
	ss << ToStr("eqclass", eqclass);
	ss << ToStr("channel_spacing", channel_spacing);
	ss << ToStr("power", power);
	ss << ToStr("atpc", atpc);
	ss << ToStr("azimuth", azimuth);
	ss << ToStr("elevation", elevation);
	ss << ToStr("polarization", polarization);
	ss << ToStr("noise", noise);
	ss << ToStr("losses", losses);
	ss << ToStr("h_ant", h_ant);
	ss << ToStr("ant_manuf_name", ant_manuf_name);
	ss << ToStr("ant_type", ant_type);
	ss << ToStr("coord_ref", coord_ref);
	ss << ToStr("gain", gain);
	ss << ToStr("cp_pattern", cp_pattern);
	ss << ToStr("xp_pattern", xp_pattern);
	ss << ToStr("mask", mask);
	ss << ToStr("cpData", cpData);
	ss << ToStr("xpData", xpData);
	ss << ToStr("tError", tError);
	ss << ToStr("\n\n", "");
	return ss;
}
String HcmRec::ToPrint()const{
	String repStr = "";
	String un = "";
	String tmp = "";
	if(!isPassive){
		if(isTx)
			{un = "(Tx)"; tmp = "TX";}
		else
			{un = "(Rx)"; tmp = "RX";}
	}
	else{
		if(isTx)
			{un = "(PTx)"; tmp = "PTX";}
		else
			{un = "(PRx)"; tmp = "PRX";}
	}
	ToReport("Type of entry         ", un, tmp, repStr);
	if(!isPassive){
		ToReport("Frequency             ", un, Format("%.3f", frequency), repStr);
		ToReport("Frequency unit        ", un, "G", repStr);
	}
	ToReport("Longitude             ", un, Format("%.4f", longitude), repStr);
	ToReport("Latitude              ", un, Format("%.4f", latitude), repStr);
	ToReport("Height above sea      ", un, Format("%.3f", h_asl), repStr);
	ToReport("Equip. manufacture    ", un, eqp_manuf_name, repStr);
	ToReport("Equipment type        ", un, eqp_type, repStr);
	ToReport("Maximum capacity      ", un, capacity, repStr);
	ToReport("  " + un + " " + "Spectrum mask:", repStr);

	String tmpStr = "";
	for(auto v: mask){
		tmpStr << v.ToString();
	}
	ToReport(tmpStr, repStr);

	ToReport("Channel spacing       ", un, Format("%.3f", channel_spacing), repStr);
	ToReport("Maximum power         ", un, Format("%.3f", power), repStr);
	ToReport("ATPC                  ", un, Format("%.3f", atpc), repStr);
	ToReport("Azimuth               ", un, Format("%.3f", azimuth), repStr);
	ToReport("Elevation             ", un, Format("%.3f", elevation), repStr);
	ToReport("Polarization          ", un, (polarization), repStr);
	ToReport("Branches + line losses", un, Format("%.3f", losses), repStr);
	ToReport("Antenna height        ", un, Format("%.3f", h_ant), repStr);
	ToReport("Antenna manufacture   ", un, (ant_manuf_name), repStr);
	ToReport("Antenna type          ", un, (ant_type), repStr);
	ToReport("Antenna gain          ", un, Format("%.3f", gain), repStr);
	ToReport("Antenna cop. pattern  ", un, (cp_pattern), repStr);
	ToReport("  " + un + " " + "Antenna cop. data:", repStr);

	tmpStr = "";
	for(auto v: cpData){
		tmpStr << v.ToString();
	}
	ToReport(tmpStr, repStr);
//	ToReport(ToStr("", cpData), repStr);

	ToReport("Antenna cro. pattern  ", un, (xp_pattern), repStr);
	ToReport("  " + un + " " + "Antenna cro. data:", repStr);
	tmpStr = "";
	for(auto v: xpData){
		tmpStr << v.ToString();
	}
	ToReport(tmpStr, repStr);
//	ToReport(ToStr("", xpData), repStr);
	ToReport(tError.ToString(), repStr);
	return repStr;
}



// Converts HCM data String to HcmRec structure
// Data not available = NO_DATA
int HcmRec::SetTypeOfEntry(const HcmIN& hcmIN){
	//--- TYPE OF ENTRY
	HcmField field = hcmIN.GetField(0);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		String upStr = ToUpper(vstr); // make type of entry uppercase
		if(!(upStr == "TX" || upStr == "RX" || upStr == "PTX" || upStr == "PRX")){
			return tError.Set(appErrorMap.Get(1));
		}
		upStr.Find("TX") != -1 ? isTx = true : isTx = false;
		upStr.Find("P")  != -1 ? isPassive = true : isPassive = false;
		if(isTx && isPassive)
			un = "(PTx)";
		else if(isTx && !isPassive)
			un = "(Tx)";
		else if(!isTx && isPassive)
			un = "(PRx)";
		else
			un = "(Rx)";
		ToReport(Format("Type of entry %s\t = %s\n", un, upStr), repStr);
		return tError.Set(appErrorMap.Get(0));
	}
	if(field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(999));
	else
		return tError.Set(appErrorMap.Get(0));
}
int HcmRec::SetFrequency(const HcmIN& hcmIN){
	//--- FREQUENCY
	HcmField field = hcmIN.GetField(1);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		if(!isPassive){
			// Not passive
			ToReport(Format("Frequency %s\t = %s\n", un, vstr), repStr);
			if(vstr.IsEmpty()){
				if(isTx)
					return tError.Set(appErrorMap.Get(1002)); // no Tx frequency
				else
					return tError.Set(appErrorMap.Get(1020)); // no Rx frequency
			}
			Value tmp = StrDbl(vstr);
			if(!tmp.IsNull())
				frequency = tmp;
			else{
				if(isTx)
					return tError.Set(appErrorMap.Get(1037)); // invalid Tx frequency
				else
					return tError.Set(appErrorMap.Get(1040)); // invalid Rx frequency
			}
		}
		else{
			// If passive, frequency is not required
			frequency = NO_DATA;
		}
		return tError.Set(appErrorMap.Get(0));
	}
	if(field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(2));
	else
		return tError.Set(appErrorMap.Get(0));
}
int HcmRec::SetFreqUnit(const HcmIN& hcmIN){
	//--- FREQUENCY UNIT
	HcmField field = hcmIN.GetField(2);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		if(!isPassive){
			if(vstr.IsEmpty()){
				if(isTx)
					return tError.Set(appErrorMap.Get(1003)); // no Tx frequency unit
				else
					return tError.Set(appErrorMap.Get(1021)); // no Rx frequency unit
			}
			ToReport(Format("Frequency unit %s\t = %s\n", un, vstr), repStr);
			// make frequency unit uppercase
			String upStr = ToUpper(TrimBoth(vstr)); // make type of entry uppercase
			if(upStr == "K")
				frequency = frequency / 1E6;
			else if(upStr == "M")
				frequency = frequency / 1000;
			else if(upStr != "G"){
				if(isTx)
					return tError.Set(appErrorMap.Get(1038)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1041)); // for Rx
			}
			if(frequency > 43.5 || frequency < 1){
				if(isTx)
					return tError.Set(appErrorMap.Get(1039)); // Tx frequency outside the range 1 - 43.5
				else
					return tError.Set(appErrorMap.Get(1042)); // Rx frequency outside the range 1 - 43.5
			}
		}
		return tError.Set(appErrorMap.Get(0));
	}
	if(field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(3));
	else
		return tError.Set(appErrorMap.Get(0));
}
int HcmRec::SetCoord(const HcmIN& hcmIN){
	//--- GEOGRAPHICAL CO-ORDINATES
	HcmField field = hcmIN.GetField(10);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		ToReport(Format("Geo. co-ordinates %s \t = %s\n", un, vstr), repStr);
		if(!vstr.IsEmpty()){
			sLong = vstr.Mid(0, 8);
			int err = StrToDblCoord(vstr.Mid(0, 8), longitude, tError);
			if(err != 0 || vstr.GetCount() != 15){
				if(isPassive){
					// For Passive repeater
					if(isTx)
						return tError.Set(appErrorMap.Get(1102)); // Error in PTx coordinates
					else
						return tError.Set(appErrorMap.Get(1103)); // Error in PRx coordinates
				}else{
					// For active
					if(isTx)
						return tError.Set(appErrorMap.Get(1043)); // Error in Tx coordinates
					else
						return tError.Set(appErrorMap.Get(1044)); // Error in Rx coordinates
				}
			}
			sLat = vstr.Mid(8, 7);
			err = StrToDblCoord(vstr.Mid(8, 7), latitude, tError);
			if(err != 0){
				if(isPassive){
					// For Passive repeater
					if(isTx)
						return tError.Set(appErrorMap.Get(1102)); // Error in PTx coordinates
					else
						return tError.Set(appErrorMap.Get(1103)); // Error in PRx coordinates
				}else{
					// For active
					if(isTx)
						return tError.Set(appErrorMap.Get(1043)); // Error in Tx coordinates
					else
						return tError.Set(appErrorMap.Get(1044)); // Error in Rx coordinates
				}
			}
		}else{
			// coordinates are missing
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1079));
				else
					return tError.Set(appErrorMap.Get(1091));
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1004));
				else
					return tError.Set(appErrorMap.Get(1022));
			}
		}
		return tError.Set(appErrorMap.Get(0));
	}
	if(field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(6));
	else
		return tError.Set(appErrorMap.Get(0));
}
int HcmRec::SetStationHeight(const HcmIN& hcmIN){
	//--- HEIGHT OF THE STATION SITE ABOVE SEA LEVEL
	HcmField field = hcmIN.GetField(11);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		ToReport(Format("Height above sea %s \t = %s\n", un, vstr), repStr);
		if(vstr.IsEmpty()){
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1080)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1092)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1005)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1023)); // for Rx
			}
		}
		Value tmp = StrInt(TrimBoth(vstr));
		if(!tmp.IsNull())
			h_asl = tmp;
		else{
			// invalid height
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1104)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1105)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1045)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1046)); // for Rx
			}
		}
		return tError.Set(appErrorMap.Get(0));
	}
	if(field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(7));
	else
		return tError.Set(appErrorMap.Get(0));
}
int HcmRec::SetTransSpectrumMask(const HcmIN& hcmIN){
	//--- TRANSMITTER SPECTRUM MASK OR RECEIVER SELECTIVITY MASK
	HcmField field = hcmIN.GetField(16);
	if(!field.IsEmpty()){
		if(!isPassive){
			ToReport(Format("Spectrum mask %s\n", un), repStr);
			int j = 16;
			while(!(j >= 28)){
				TrMask msk;
				if(hcmIN.GetField(j).IsEmpty() && hcmIN.GetField(j+1).IsEmpty()) break; // no mask data left, if frequency and attenuation is empty
				ToReport(Format("\t\t\t = \t%s\t\t%s\n", hcmIN.GetField(j).GetStr(), hcmIN.GetField(j+1).GetStr()), repStr);
				if(hcmIN.GetField(j).IsEmpty() && !hcmIN.GetField(j+1).IsEmpty()){
					if(isTx)
						return tError.Set(appErrorMap.Get(1006)); // for Tx
					else
						return tError.Set(appErrorMap.Get(1070)); // for Rx
				}
				if(!hcmIN.GetField(j).IsEmpty() && hcmIN.GetField(j+1).IsEmpty()){
					if(isTx)
						return tError.Set(appErrorMap.Get(1007)); // for Tx
					else
						return tError.Set(appErrorMap.Get(1071)); // for Rx
				}
				Value tmp = StrDbl(hcmIN.GetField(j).GetStr());
				if(!tmp.IsNull())
					msk.frequency = tmp;
				else{
					if(isTx)
						return tError.Set(appErrorMap.Get(1047)); // for Tx
					else
						return tError.Set(appErrorMap.Get(1048)); // for Rx
				}
				j++;
				tmp = StrDbl(hcmIN.GetField(j).GetStr());
				if(!tmp.IsNull())
					msk.attenuation = tmp;
				else{
					if(isTx)
						return tError.Set(appErrorMap.Get(1047)); // for Tx
					else
						return tError.Set(appErrorMap.Get(1048)); // for Rx
				}
				if(!(msk.frequency == 0 && msk.attenuation == 0 && mask.GetCount() > 0)){
					// if we get mask point (0,0) not in the first position, omit it.
					mask.Add(msk);
				}
				j++;
			}
		}
		return tError.Set(appErrorMap.Get(0));
	}
	if(field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(8));
	else
		return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetEqupmentClass(const HcmIN& hcmIN){
	//--- EQUIPMENT CLASS
	HcmField field = hcmIN.GetField(28);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		if(!isPassive){
			if(!vstr.IsEmpty()){
				eqclass = vstr;
				if(mask.GetCount() <=0){
					// look at equipment class only if we have no mask data
					if (!(eqclass == "1"  || eqclass == "2"  || eqclass == "3"  ||
						  eqclass == "4H" || eqclass == "4L" || eqclass == "5A" ||
						  eqclass == "5B" || eqclass == "6A" || eqclass == "6B" ||
						  eqclass == "4h" || eqclass == "4l" || eqclass == "5a" ||
						  eqclass == "5b" || eqclass == "6a" || eqclass == "6b")){
						if(isTx)
							return tError.Set(appErrorMap.Get(1241)); // invalid transmitter equipment class
						else
							return tError.Set(appErrorMap.Get(1242)); // invalid receiver equipment class
					}
				}
			}
			else
				eqclass = String(); // data not available
		}
		else
			eqclass = String();	 // data not available
		return tError.Set(appErrorMap.Get(0));
	}
	if(field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(1241));
	else
		return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetChannelSpacing(const HcmIN& hcmIN){
	//--- CHANNEL SPACING
	HcmField field = hcmIN.GetField(30);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		if(!isPassive){
			Value tmp = StrDbl(vstr);
			if(!tmp.IsNull()){
				channel_spacing = tmp;
			}
			else{
				if(isTx)
					return tError.Set(appErrorMap.Get(1024)); // invalid channel spacing
				else
					return tError.Set(appErrorMap.Get(1248));
			}
			if(channel_spacing <=0){
				if(isTx)
					return tError.Set(appErrorMap.Get(1024)); // invalid channel spacing
				else
					return tError.Set(appErrorMap.Get(1248));
			}
		}
		else{
			channel_spacing = NO_DATA;
		}
		return tError.Set(appErrorMap.Get(0));
	}
	else{
		channel_spacing = 0.0;
	}
	if(!isPassive && field.GetDesct().GetMandat()>FldMandat::OFF){
		return tError.Set(appErrorMap.Get(11));
	}
	else{
		return tError.Set(appErrorMap.Get(0));
	}
}

int HcmRec::Set78MaskElement(const HcmIN& hcmIN){
	//--- SET 7-th and 8-th MASK ELEMENTS (2.5 * CHANNEL_SPACING and 3.5 * CHANNEL_SPACING)
	HcmField field = hcmIN.GetField(30);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		if(!isPassive){
			if(mask.GetCount() > 0){
				if(mask.GetCount() < 2){
					// At least 2 pairs have to be present
					if(isTx)
						return tError.Set(appErrorMap.Get(1233));
					else
						return tError.Set(appErrorMap.Get(1234));
				}
				if(mask[mask.GetCount()-1].attenuation < 40){
					// Last point must have attenuation >= 40 dB
					if(isTx)
						return tError.Set(appErrorMap.Get(1235));
					else
						return tError.Set(appErrorMap.Get(1236));
				}
				if(mask[mask.GetCount() - 1].frequency < channel_spacing * 2.5){
					// Add 7th point which = 2.5 x channel spacing
					TrMask msk;
					msk.frequency = channel_spacing * 2.5;
					msk.attenuation = mask[mask.GetCount() - 1].attenuation;
					mask.Add(msk);
				}
				if(mask[mask.GetCount() - 1].frequency < channel_spacing * 3.5){
					// Add 8th point which = 3.5 x channel spacing
					TrMask msk;
					msk.frequency = channel_spacing * 3.5;
					msk.attenuation = mask[mask.GetCount() - 1].attenuation + 5;
					mask.Add(msk);
				}
			}
			ToReport(Format("Channel spacing %s \t = %s\n", un, vstr), repStr);
		}
		return tError.Set(appErrorMap.Get(0));
	}
	if(!isPassive && field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(11));
	else
		return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetMaxPermTrPower(const HcmIN& hcmIN){
	//--- MAXIMUM PERMITTED TRANSMITTER POWER
	HcmField field = hcmIN.GetField(31);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		if(isTx && !isPassive){
			ToReport(Format("Maximum power %s \t = %s\n", un, vstr), repStr);
			if(vstr.IsEmpty())
				return tError.Set(appErrorMap.Get(1008)); // power not present
			Value tmp = StrDbl(vstr);
			if(!tmp.IsNull())
				power = tmp;
			else
				return tError.Set(appErrorMap.Get(1049)); // invalid maximum permitted transmitter power
		}
		return tError.Set(appErrorMap.Get(0));
	}
	if(isTx && !isPassive && field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(12));
	else
		return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetAtrc(const HcmIN& hcmIN){
	//--- ATPC
	HcmField field = hcmIN.GetField(32);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		if(!isPassive && isTx){
			ToReport(Format("ATPC %s \t = %s\n", un, vstr), repStr);
			if(!vstr.IsEmpty()){
				Value tmp = StrDbl(vstr);
				if(!tmp.IsNull())
					atpc = tmp;
				if(tmp.IsNull() || atpc < 0)
					return tError.Set(appErrorMap.Get(1050)); // invalid ATPC
			}
			else
				atpc = 0.0;
		}
		else
			atpc = NO_DATA;
		return tError.Set(appErrorMap.Get(0));
	}
	if(field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(13));
	else
		return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetAzimuth(const HcmIN& hcmIN){
	//--- AZIMUTH
	HcmField field = hcmIN.GetField(33);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		ToReport(Format("Azimuth %s \t = %s\n", un, vstr), repStr);
		if(vstr.IsEmpty()){
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1081)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1093)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1010)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1026)); // for Rx
			}
		}
		Value tmp = StrDbl(vstr);
		if(!tmp.IsNull())
			azimuth = tmp;
		if(tmp.IsNull() || azimuth > 360 || azimuth < 0){
			// invalid azimuth
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1106)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1107)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1051)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1052)); // for Rx
			}
		}
		return tError.Set(appErrorMap.Get(0));
	}
	if(field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(14));
	else
		return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetElevation(const HcmIN& hcmIN){
	//--- ELEVATION
	HcmField field = hcmIN.GetField(34);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		ToReport(Format("Elevation %s \t = %s\n", un, vstr), repStr);
		if(vstr.IsEmpty()){
			// elevation not present
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1082)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1094)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1011)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1027)); // for Rx
			}
		}
		Value tmp = StrDbl(vstr);
		if(!tmp.IsNull())
			elevation = tmp;
		if(tmp.IsNull() || fabs(elevation) > 90){
			// invalid elevation
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1108)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1109)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1053)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1054)); // for Rx
			}
		}
		return tError.Set(appErrorMap.Get(0));
	}
	if(field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(15));
	else
		return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetPolarization(const HcmIN& hcmIN){
	//--- POLARIZATION
	HcmField field = hcmIN.GetField(35);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		ToReport(Format("Polarization %s \t = %s\n", un, vstr), repStr);
		String tmpStr = ToUpper(TrimBoth(vstr));
		if(tmpStr.IsEmpty()){
			// polarization not present
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1083)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1095)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1012)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1028)); // for Rx
			}
		}
		// make polarization uppercase
		if(tmpStr == "H" || tmpStr == "V")
			polarization = tmpStr;
		else{
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1215)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1216)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1217)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1218)); // for Rx
			}
		}
		return tError.Set(appErrorMap.Get(0));
	}
	if(field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(16));
	else
		return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetRxPowerLevel(const HcmIN& hcmIN){
	//--- RECEIVER NOISE POWER LEVEL
	HcmField field = hcmIN.GetField(36);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		if(!isTx && !isPassive){
			ToReport(Format("Noise power level FkTB %s \t = %s\n", un, vstr), repStr);
			if(vstr.IsEmpty())
				return tError.Set(appErrorMap.Get(1029)); // noise power level not present
			Value tmp = StrDbl(vstr);
			if(!tmp.IsNull())
				noise = tmp;
			else
				return tError.Set(appErrorMap.Get(1055)); // invalid receiver noise
		}
		return tError.Set(appErrorMap.Get(0));
	}
	if(!isTx && !isPassive && field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(17));
	else
		return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetBranchAndLineLoss(const HcmIN& hcmIN){
	//--- BRANCHES AND LINE LOSSES
	HcmField field = hcmIN.GetField(37);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		ToReport(Format("Branches + line losses %s \t = %s\n", un, vstr), repStr);
		if(!vstr.IsEmpty()){
			Value tmp = StrDbl(vstr);
			if(!tmp.IsNull())
				losses = tmp;
			if(tmp.IsNull() || losses < 0){
				// invalid branches and line losses
				if(isPassive){
					// for passive case
					if(isTx)
						return tError.Set(appErrorMap.Get(1110)); // for PTx
					else
						return tError.Set(appErrorMap.Get(1111)); // for PRx
				}else{
					// for active case
					if(isTx)
						return tError.Set(appErrorMap.Get(1056)); // for Tx
					else
						return tError.Set(appErrorMap.Get(1057)); // for Rx
				}
			}
		}
		else{
			losses = 0.0;
		}
		return tError.Set(appErrorMap.Get(0));
	}
	if(field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(18));
	else
		return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetAntGroundHeight(const HcmIN& hcmIN){
	//--- HEIGHT OF ANTENNA ABOVE GROUND
	HcmField field = hcmIN.GetField(38);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		ToReport(Format("Antenna height %s \t = %s\n", un, vstr), repStr);
		if(vstr.IsEmpty()){
			// antenna height not present
			if (isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1084)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1096)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1014)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1031)); // for Rx
			}
		}
		Value tmp = StrDbl(vstr);
		if(!tmp.IsNull())
			h_ant = tmp;
		if(tmp.IsNull() || h_ant < 0){
			// invalid height of antenna above ground
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1112)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1113)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1058)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1059)); // for Rx
			}
		}
		return tError.Set(appErrorMap.Get(0));
	}
	if(field.GetDesct().GetMandat()>FldMandat::OFF)
		return tError.Set(appErrorMap.Get(19));
	else
		return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetAntGain(const HcmIN& hcmIN){
	//--- ANTENNA GAIN
	HcmField field = hcmIN.GetField(48);
	if(field.IsEmpty()){
		gain = 0;
		// antenna gain not present
		if(isPassive){
			// for passive case
			if(isTx)
				return tError.Set(appErrorMap.Get(1085)); // for PTx
			else
				return tError.Set(appErrorMap.Get(1097)); // for PRx
		}else{
			// for active case
			if(isTx)
				return tError.Set(appErrorMap.Get(1015)); // for Tx
			else
				return tError.Set(appErrorMap.Get(1032)); // for Rx
		}
	}
	String vstr = field.GetStr();
	ToReport(Format("Antenna gain %s \t = %s\n", un, vstr), repStr);
	Value tmp = StrDbl(vstr);
	if(!tmp.IsNull())
		gain = tmp;
	else{
		// invalid antenna gain
		if(isPassive){
			// for passive case
			if(isTx)
				return tError.Set(appErrorMap.Get(1114)); // for PTx
			else
				return tError.Set(appErrorMap.Get(1115));// for PRx
		}else{
			// for active case
			if(isTx)
				return tError.Set(appErrorMap.Get(1060)); // for Tx
			else
				return tError.Set(appErrorMap.Get(1061)); // for Rx
		}
	}
	return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetCpPattern(const HcmIN& hcmIN){
	//--- ANTENNA COPOLAR RADIATION PATTERN
	isSetStdCP = false;
	HcmField field = hcmIN.GetField(49);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		ToReport(Format("Antenna cop. pattern %s \t = %s\n", un, vstr), repStr);
		// make antenna pattern uppercase
		String upStr = ToUpper(vstr); // make type of entry uppercase
		if(upStr == "CP" || upStr == "VV" || upStr == "HH")
			cp_pattern = upStr;
		else{
			// invalid copolar radiation pattern
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1219)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1220)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1203)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1209)); // for Rx
			}
		}
	}
	else{
		// Put standard antenna CP data, if missing
		isSetStdCP = true;
	}
	return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetCpNumOfMaskData(const HcmIN& hcmIN){
	if(isSetStdCP) return tError.Set(appErrorMap.Get(0));
	//--- Number of  mask data
	HcmField field = hcmIN.GetField(50);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		Value tmp = StrInt(vstr);
		if(tmp.IsNull()){
			// invalid number of copolar radiation pattern data
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1221)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1222)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1204)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1210)); // for Rx
			}
		}
	}
	else{
		if(isPassive){
			// for passive case
			if(isTx)
				return tError.Set(appErrorMap.Get(1221)); // for PTx
			else
				return tError.Set(appErrorMap.Get(1222)); // for PRx
		}else{
			// for active case
			if(isTx)
				return tError.Set(appErrorMap.Get(1204)); // for Tx
			else
				return tError.Set(appErrorMap.Get(1210)); // for Rx
		}
	}
	return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetCpPatternData(const HcmIN& hcmIN){
	if(isSetStdCP) return tError.Set(appErrorMap.Get(0));
	//--- Pattern data
	Vector<String> cpAngles = clone(hcmIN.GetField(51).GetVec());
	Vector<String> cpAttens = clone(hcmIN.GetField(52).GetVec());
	if(cpAngles.GetCount() && cpAttens.GetCount() && cpAngles.GetCount() == cpAttens.GetCount()){
		ToReport(Format("Antenna cop. data %s:\n", un), repStr);
		for(int i = 0; i < cpAngles.GetCount(); i++){
			AntPoint ant;
			ToReport(Format("\t\t\t\t %s:\n", un), repStr);
			ToReport(Format("\t\t\t\t %s\t\t%s\n", cpAngles[i], cpAttens[i]), repStr);
			Value tmp = StrDbl(cpAngles[i]);
			if(!tmp.IsNull())
				ant.angle = tmp;
			else{
				// invalid copolar antenna data or missing
				if(isPassive){
					// for passive case
					if(isTx)
						return tError.Set(appErrorMap.Get(1223)); // for PTx
					else
						return tError.Set(appErrorMap.Get(1224)); // for PRx
				}else{
					// for active case
					if(isTx)
						return tError.Set(appErrorMap.Get(1205)); // for Tx
					else
						return tError.Set(appErrorMap.Get(1211)); // for Rx
				}
			}
			// convert negative value to positive
			if(ant.angle < 0) ant.angle = 360 + ant.angle;
			tmp = StrDbl(cpAttens[i]);
			if(!tmp.IsNull())
				ant.attenuation = tmp;
			else{
				// invalid copolar antenna data or missing
				if(isPassive){
					// for passive case
					if(isTx)
						return tError.Set(appErrorMap.Get(1223)); // for PTx
					else
						return tError.Set(appErrorMap.Get(1224)); // for PRx
				}else{
					// for active case
					if(isTx)
						return tError.Set(appErrorMap.Get(1205)); // for Tx
					else
						return tError.Set(appErrorMap.Get(1211)); // for Rx
				}
			}
			cpData.Add(ant);
		}
		// Sort CP antenna data
		Sort(cpData, [](const AntPoint& a, const AntPoint& b) { return a.angle < b.angle; });
		// If 1st point is missing, add it
		if(cpData[0].angle !=0.0){
			AntPoint ant;
			ant.angle = 0.0;
			ant.attenuation = 0.0;
			cpData.Add(ant);
			Sort(cpData, [](const AntPoint& a, const AntPoint& b) { return a.angle < b.angle; });
		}
	}
	else{
		// invalid copolar antenna data or missing
		if(isPassive){
			// for passive case
			if(isTx)
				return tError.Set(appErrorMap.Get(1223)); // for PTx
			else
				return tError.Set(appErrorMap.Get(1224)); // for PRx
		}else{
			// for active case
			if(isTx)
				return tError.Set(appErrorMap.Get(1205)); // for Tx
			else
				return tError.Set(appErrorMap.Get(1211)); // for Rx
		}
	}
	return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetXpPattern(const HcmIN& hcmIN){
	isSetStdXP = false;
	//--- ANTENNA CROSSPOLAR RADIATION PATTERN
	// index of pattern e.g. VH, CP, HV
	HcmField field = hcmIN.GetField(53);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		ToReport(Format("Antenna cro. pattern %s \t = %s\n", un, vstr), repStr);
		// make crosspolar antenna pattern uppercase
		String upStr = ToUpper(vstr); // make type of entry uppercase
		if(upStr == "XP" || upStr == "HV" || upStr == "VH")
			xp_pattern = upStr;
		else{
			// invalid crosspolar radiation pattern
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1225)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1226)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1206)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1212)); // for Rx
			}
		}
	}
	else{
		isSetStdXP = true;
	}
	return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetXpNumOfMaskData(const HcmIN& hcmIN){
	if(isSetStdXP) return tError.Set(appErrorMap.Get(0));
	//--- Number of  mask data
	HcmField field = hcmIN.GetField(54);
	if(!field.IsEmpty()){
		String vstr = field.GetStr();
		Value tmp = StrInt(vstr);
		if(tmp.IsNull()){
			// invalid number of crosspolar radiation pattern data
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1227)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1228)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1207)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1213)); // for Rx
			}
		}
	}
	else{
		// missing number of crosspolar radiation pattern data
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1227)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1228)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1207)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1213)); // for Rx
			}
	}
	return tError.Set(appErrorMap.Get(0));
}

int HcmRec::SetXpPatternData(const HcmIN& hcmIN){
	if(isSetStdXP) return tError.Set(appErrorMap.Get(0));
	//--- ANTENNA CROSSPOLAR RADIATION PATTERN
	Vector<String> xpAngles = clone(hcmIN.GetField(55).GetVec());
	Vector<String> xpAttens = clone(hcmIN.GetField(56).GetVec());
	if(xpAngles.GetCount() && xpAttens.GetCount() && xpAngles.GetCount() == xpAttens.GetCount()){
		ToReport(Format("Antenna cro. data %s:\n", un), repStr);
		for(int i = 0; i < xpAngles.GetCount(); i++){
			AntPoint ant;
			ToReport(Format("\t\t\t\t %s:\n", un), repStr);
			ToReport(Format("\t\t\t\t %s\t\t%s\n", xpAngles[i], xpAttens[i]), repStr);
			Value tmp = StrDbl(xpAngles[i]);
			if(!tmp.IsNull())
				ant.angle = tmp;
			else{
				// invalid crosspolar antenna data or missing
				if(isPassive){
					// for passive case
					if(isTx)
						return tError.Set(appErrorMap.Get(1229)); // for PTx
					else
						return tError.Set(appErrorMap.Get(1230)); // for PRx
				}else{
					// for active case
					if(isTx)
						return tError.Set(appErrorMap.Get(1208)); // for Tx
					else
						return tError.Set(appErrorMap.Get(1214)); // for Rx
				}
			}
			// convert negative value to positive
			if(ant.angle < 0) ant.angle = 360 + ant.angle;
			tmp = StrDbl(xpAttens[i]);
			if(!tmp.IsNull())
				ant.attenuation = tmp;
			else{
				// invalid crosspolar antenna data or missing
				if(isPassive){
					// for passive case
					if(isTx)
						return tError.Set(appErrorMap.Get(1229)); // for PTx
					else
						return tError.Set(appErrorMap.Get(1230)); // for PRx
				}else{
					// for active case
					if(isTx)
						return tError.Set(appErrorMap.Get(1208)); // for Tx
					else
						return tError.Set(appErrorMap.Get(1214)); // for Rx
				}
			}
			xpData.Add(ant);
		}
		// Sort CP antenna data
		Sort(xpData, [](const AntPoint& a, const AntPoint& b) { return a.angle < b.angle; });
		// If 1st point is missing, add it
		if(xpData[0].angle !=0.0){
			AntPoint ant;
			ant.angle = 0.0;
			ant.attenuation = 0.0;
			xpData.Add(ant);
			Sort(cpData, [](const AntPoint& a, const AntPoint& b) { return a.angle < b.angle; });
		}
	}
	else{
		// invalid crosspolar antenna data or missing
		if(isPassive){
			// for passive case
			if(isTx)
				return tError.Set(appErrorMap.Get(1229)); // for PTx
			else
				return tError.Set(appErrorMap.Get(1230)); // for PRx
		}else{
			// for active case
			if(isTx)
				return tError.Set(appErrorMap.Get(1208)); // for Tx
			else
				return tError.Set(appErrorMap.Get(1214)); // for Rx
		}
	}
	return tError.Set(appErrorMap.Get(0));
}
int HcmRec::SetStdCpXpPatternData(const TStdAnt& stdAnt, const HcmIN& hcmIN){
	if(isSetStdCP){
		ToReport(Format("Antenna cop. pattern %s \t  = CP\n", un), repStr);
		ToReport(Format("Antenna cop. data %s \t :\n", un), repStr);
		int err = SetCP(stdAnt);
		if(err != 0) return err; // ALL data is set
	}
	if(isSetStdXP){
		ToReport(Format("Antenna cro. pattern %s \t  = XP\n", un), repStr);
		ToReport(Format("Antenna cro. data %s \t :\n", un), repStr);
		return SetXP(stdAnt);
		
	}
	return tError.Set(appErrorMap.Get(0));
}
//!---------------------------------------------------------------------------------------------

int HcmRec::Fill(const HcmIN& hcmIN){
	Clear();
	// Take vector values and put them to HcmRec structure
	int res = 0;
	//--- TYPE OF ENTRY
	res = SetTypeOfEntry(hcmIN);
	if(res > 0)return res;
	//--- FREQUENCY
	res = SetFrequency(hcmIN);
	if(res > 0)return res;
	//--- FREQUENCY UNIT
	res = SetFreqUnit(hcmIN);
	if(res > 0)return res;
	//--- NAME OF STATION
	name_of_station = hcmIN.GetField(8).GetStr();
	//--- COUNTRY
	country = hcmIN.GetField(9).GetStr();
	//--- GEOGRAPHICAL CO-ORDINATES
	res = SetCoord(hcmIN);
	if(res > 0)return res;
	//--- HEIGHT OF THE STATION SITE ABOVE SEA LEVEL
	res = SetStationHeight(hcmIN);
	if(res > 0)return res;
	//--- EQUIPMENT MANUFACTURER NAME
	if(!isPassive && isReport)
		ToReport(Format("Equip. manufacture %s \t = %s\n", un, hcmIN.GetField(13).GetStr()), repStr);
	eqp_manuf_name = hcmIN.GetField(13).GetStr();
	//--- EQUIPMENT TYPE
	if(!isPassive && isReport)
		ToReport(Format("Equipment type %s \t = %s\n", un, hcmIN.GetField(14).GetStr()), repStr);
	eqp_type = hcmIN.GetField(14).GetStr();
	//--- MAX. CAPACITY OF THE LINK
	if(!isPassive && isReport)
		ToReport(Format("Maximum capacity %s \t = %s\n", un, hcmIN.GetField(15).GetStr()), repStr);
	capacity = hcmIN.GetField(15).GetStr();
	//--- TRANSMITTER SPECTRUM MASK OR RECEIVER SELECTIVITY MASK
	res = SetTransSpectrumMask(hcmIN);
	if(res > 0)return res;
	//--- EQUIPMENT CLASS
	res = SetEqupmentClass(hcmIN);
	if(res > 0)return res;
	//--- NFD2 UNUSED
	//--- CHANNEL SPACING
	res = SetChannelSpacing(hcmIN);
	if(res > 0)return res;
	//--- SET 7-th and 8-th MASK ELEMENTS (2.5 * CHANNEL_SPACING and 3.5 * CHANNEL_SPACING)
	res = Set78MaskElement(hcmIN);
	if(res > 0)return res;
	//--- MAXIMUM PERMITTED TRANSMITTER POWER
	res = SetMaxPermTrPower(hcmIN);
	if(res > 0)return res;
	//--- ATPC
	res = SetAtrc(hcmIN);
	if(res > 0)return res;
	//--- AZIMUTH
	res = SetAzimuth(hcmIN);
	if(res > 0)return res;
	//--- ELEVATION
	res = SetElevation(hcmIN);
	if(res > 0)return res;
	//--- POLARIZATION
	res = SetPolarization(hcmIN);
	if(res > 0)return res;
	//--- RECEIVER NOISE POWER LEVEL
	res = SetRxPowerLevel(hcmIN);
	if(res > 0)return res;
	//--- BRANCHES AND LINE LOSSES
	res = SetBranchAndLineLoss(hcmIN);
	if(res > 0)return res;
	//--- HEIGHT OF ANTENNA ABOVE GROUND
	res = SetAntGroundHeight(hcmIN);
	if(res > 0)return res;
	//--- ANTENNA MANUFACTURER NAME
	ToReport("Antenna manufacture", un, hcmIN.GetField(44).GetStr(), repStr);
	ant_manuf_name = hcmIN.GetField(44).GetStr();
	coord_ref = hcmIN.GetField(43).GetStr();
	//--- ANTENNA TYPE
	ToReport("Antenna type", un,hcmIN.GetField(45).GetStr(), repStr);
	ant_type = hcmIN.GetField(45).GetStr();
	//--- ANTENNA GAIN
	res = SetAntGain(hcmIN);
	if(res > 0)return res;
	return tError.Set(appErrorMap.Get(0));
}

int HcmRec::Fill(const TStdAnt& stdAnt, const HcmIN& hcmIN){
	// Take vector values and put them to HcmRec structure
	int res = 0;
	res = Fill(hcmIN);
	if(res > 0) return res;
	//--- ANTENNA COPOLAR RADIATION PATTERN
	res = SetCpPattern(hcmIN);
	if(res > 0) return res;
	//--- Number of  mask data
	res = SetCpNumOfMaskData(hcmIN);
	if(res > 0) return res;
	//--- Pattern data
	res = SetCpPatternData(hcmIN);
	if(res > 0) return res;
	//--- ANTENNA CROSSPOLAR RADIATION PATTERN
	res = SetXpPattern(hcmIN);
	if(res > 0) return res;
	//--- Number of  mask data
	res = SetXpNumOfMaskData(hcmIN);
	if(res > 0) return res;
	//--- Pattern data
	res = SetXpPatternData(hcmIN);
	if(res > 0) return res;
	//--- STD
	res = SetStdCpXpPatternData(stdAnt, hcmIN);
	if(res > 0) return res;
	return tError.Set(appErrorMap.Get(0));
}
//!---------------------------------------------------------------------------------------------

void HcmRec::ToReport(const String& caption, const String& entry, const String& vstr, String& repStr)const{
	repStr = repStr + "  " + entry + " " + caption + "\t= " + vstr + "\n";
}
void HcmRec::ToReport(const String& str, String& repStr)const{
	repStr = repStr + str + "\n";
}
//	repStr = repStr + "  " + caption + " " + type + "         = " + vstr + "\n";

//!---------------------------------------------------------------------------------------------
int HcmRec::SetCP(const TStdAnt& stdAnt){
	if(cpData.GetCount() == 0){
		Vector<AntPoint> patt;
		int er = stdAnt.CpPattern(gain, patt);
		if(er==3){
			// standard antenna not found
			if(isPassive){
				// for passive case
				if(isTx)
					return tError.Set(appErrorMap.Get(1086)); // for PTx
				else
					return tError.Set(appErrorMap.Get(1098)); // for PRx
			}else{
				// for active case
				if(isTx)
					return tError.Set(appErrorMap.Get(1016)); // for Tx
				else
					return tError.Set(appErrorMap.Get(1033)); // for Rx
			}
		}
		cpData = clone(patt);
		cp_pattern = "CP";
		// Create report of standard antenna data
		for(int i = 0; i < cpData.GetCount(); i++){
			ToReport(Format("\t\t\t\t\t = %8.3f\t\t%8.3f\n", cpData[i].angle, cpData[i].attenuation), repStr);
		}
	}
	return tError.Set(appErrorMap.Get(0));
}
//----------------------------------------------------------------------------------

int HcmRec::SetXP(const TStdAnt& stdAnt){
	Vector<AntPoint> patt;
	int er = stdAnt.XpPattern(gain, patt);
	if(er == 3){
		// standard antenna not found
		if(isPassive){
			// for passive case
			if(isTx)
				return tError.Set(appErrorMap.Get(1086)); // for PTx
			else
				return tError.Set(appErrorMap.Get(1098)); // for PRx
		}else{
			// for active case
			if(isTx)
				return tError.Set(appErrorMap.Get(1016)); // for Tx
			else
				return tError.Set(appErrorMap.Get(1033)); // for Rx
		}
	}
	xpData = clone(patt);
	xp_pattern = "XP";
	// Create report of standard antenna data
	for(int i = 0; i < xpData.GetCount(); i++){
		ToReport(Format("\t\t\t\t\t = %8.3f\t\t%8.3f\n", xpData[i].angle, xpData[i].attenuation), repStr);
	}
	return tError.Set(appErrorMap.Get(0));
}
//!---------------------------------------------------------------------------------------------


double StrToDblCoord(const String& vstr){
	double coord = NO_COORD;
	int idx = vstr.FindFirstOf("NSWEnswe");
	if(idx == -1){
		return NO_COORD;
	}
	Value degree = StrInt(vstr.Mid(0, idx));
	Value minute = StrInt(vstr.Mid(idx + 1, 2));
	Value second = StrInt(vstr.Mid(idx + 3, 2));
	if(degree.IsNull() || minute.IsNull() || second.IsNull()){
		return NO_COORD;
	}
	coord = (static_cast<double>(second) / 60. + static_cast<double>(minute)) / 60. + static_cast<double>(degree);
	if(vstr.FindFirstOf("EWew") != -1){
		if(static_cast<double>(degree) > 360){
			return NO_COORD;
		}
		if(static_cast<double>(degree) > 180) coord = coord - 360;
	}
	else{
		if(static_cast<double>(degree) > 90){
			return NO_COORD;
		}
	}
	if(vstr.FindFirstOf("SWsw") != -1) coord = -coord;
	return coord;
}

int StrToDblCoord(const String& vstr, double& coord, HcmError& tError){
	int idx = vstr.FindFirstOf("NSWEnswe");
	if(idx == -1){
		coord = 0.0;
		return tError.Set(appErrorMap.Get(101));
	}
	Value degree = StrInt(vstr.Mid(0, idx));
	Value minute = StrInt(vstr.Mid(idx + 1, 2));
	Value second = StrInt(vstr.Mid(idx + 3, 2));
	if(degree.IsNull() || minute.IsNull() || second.IsNull()){
		coord = 0.0;
		return tError.Set(appErrorMap.Get(101));
	}
	coord = (static_cast<double>(second) / 60. + static_cast<double>(minute)) / 60. + static_cast<double>(degree);
	if(vstr.FindFirstOf("EWew") != -1){
		if(static_cast<double>(degree) > 360){
			coord = 0.0;
			return tError.Set(appErrorMap.Get(103));
		}
		if(static_cast<double>(degree) > 180) coord = coord - 360;
	}
	else{
		if(static_cast<double>(degree) > 90){
			coord = 0.0;
			return tError.Set(appErrorMap.Get(102));
		}
	}
	if(vstr.FindFirstOf("SWsw") != -1) coord = -coord;
	return tError.Set(appErrorMap.Get(0));
}
//!---------------------------------------------------------------------------------------------

int	HcmToTrigger(const String& hsmString, double& frequency_GHz, double& longitude, double& latitude, int& h_asl, HcmError& tError){
	// Read HCM String to vector
	Vector<String> vhcm;
	StringStream strm = StringStream() << hsmString;
	auto v = GetCsvLine(strm, C_RAZDEL, CHARSET_DEFAULT);
	for(auto e : v){
		vhcm.Add(e);
	}
	if(vhcm.GetCount() < 39) return tError.Set(appErrorMap.Get(8));
	Value tmp = StrDbl(TrimBoth(vhcm[1]));
	if(!tmp.IsNull())
		frequency_GHz = tmp;
	else
		return tError.Set(appErrorMap.Get(3)); // bad frequency

	if(vhcm[2] == "M" || vhcm[2] == "m")
		frequency_GHz = frequency_GHz / 1000.0;
	else
		if(vhcm[2] == "K" || vhcm[2] == "k")
			frequency_GHz = frequency_GHz / 1000000.0;
		else
			if(!(vhcm[2] == "G" || vhcm[2] == "g"))
				return tError.Set(appErrorMap.Get(3)); // bad frequency
	String slong = vhcm[10].Mid(0, 8);
	String slat = vhcm[10].Mid(8, 7);
	int err;
	err = StrToDblCoord(slong, longitude, tError);
	if(err)
		return tError.Set(appErrorMap.Get(2));
	err = StrToDblCoord(slat, latitude, tError);
	if(err)
		return tError.Set(appErrorMap.Get(2));

	// Get Antenna height
	int iah;
	tmp = StrInt(TrimBoth(vhcm[38]));
	if(!tmp.IsNull())
		iah = tmp;
	else
		return tError.Set(appErrorMap.Get(6));
	// Get site height above sea level
	int ish;
	tmp = StrInt(TrimBoth(vhcm[11]));
	if(!tmp.IsNull())
		ish = tmp;
	else
		return tError.Set(appErrorMap.Get(7));
	h_asl = ish + iah;
	return tError.Set(appErrorMap.Get(0));
}
//!---------------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------------
/*
bool operator== (const CoordRec &c1, const CoordRec &c2){
    return (c1.longitude == c2.longitude && c1.latitude == c2.latitude);
}
*/
//!---------------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------------

/*
// Error messages:
// 0 - no error
// 1245 - Parameter file hcmfs.conf not found
// 1246 - Could not open registry key HKEY_LOCAL_MASHINE\Software\HCM
// 1247 - Could not read topo value from registry
long FillTerraPath(const String& path, String& path_topo, String& path_morpho){
	// TODO: through environment variables
	if(path.size() > 0){
		// When drive letter is defined:
		path_topo   = path + disk_separator + "TOPO";
		path_morpho = path + disk_separator + "MORPHO";
		return tError.Set(appErrorMap.Get(0));
	}
	return 1245;//could not find parameter file hcmfs
}
//------------------------------------------------------------------------

// Error messages:
// 0 - no error
// 11 - Parameter file hcmfs.conf not found
// 9 - Could not read registry key HKEY_LOCAL_MASHINE\Software\HCM
// 10 - Could not read border value from registry
long FillBorderPath(const String& path, String& path_border){
	// TODO: through environment variables
	if (path.size() > 0){
		// When drive letter is defined:
		path_border = path + disk_separator + "BORDER";
		return tError.Set(appErrorMap.Get(0));
	}
	return 11;//could not find parameter file hcmfs
}
//-------------------------------------------------------------------------------
*/
//! --------------------------------------------------------------------------------------------




#endif
