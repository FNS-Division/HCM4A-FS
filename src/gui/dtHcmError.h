#ifndef __HCM4A_CalcFs4A_dtHcmError_h_
#define __HCM4A_CalcFs4A_dtHcmError_h_

#include <Core/Core.h>

using namespace Upp;

class HcmError: Moveable<HcmError>{
public:
	bool IsOk(){return _code == 0;};
	int Code()const{return _code;};
	const String& Message()const{return _message;};
	void Clear(){_code = 0; _message = "";};
	String ToString()const{
		return String()  << _code << ": " << _message;
	}
	int Set(const int code, const String& message){
		_code = code;
		_message = message;
		return _code;
	}
	int Set(const HcmError& er){
		_code = er.Code();
		_message = er.Message();
		return _code;
	}
	HcmError(){Clear();};
	HcmError(const int code, const String& message):_code(code), _message(message){};
private:
	int _code;
	String _message;
};


class HcmErrorMap: Moveable<HcmErrorMap>{
public:
	void Add(const int code, const String& msg){
		// erList.FindAdd(er.Code(), er.Message());
		int idx = erList.Find(code);
		if(idx == -1)
			erList.Add(code, msg);
		else{
			erList[idx] = erList[idx] << "\n" << msg;
		}
	}
	void Add(const HcmError& er){
		// erList.FindAdd(er.Code(), er.Message());
		int idx = erList.Find(er.Code());
		if(idx == -1)
			erList.Add(er.Code(), er.Message());
		else{
			erList[idx] = erList[idx] << "\n" << er.Message();
		}
	}
	HcmError Get(const int code)const{
		int idx = erList.Find(code);
		if(idx != -1)
			return HcmError(erList.GetKey(idx), erList[idx]);
		return HcmError(-1, String() << t_("Unknown error code: ") << code);
	}
	void Init();
	HcmErrorMap(){Init();};
private:
	VectorMap<int, String>erList;
};
//! --------------------------------------------------------------------------------------------

void HcmErrorMap::Init(){
	// if code < 0 is not break Error
	// Add(-1, t_("Morphological data not found, assume we have inland"));
	Add(-1, t_("Unknown error"));
	Add(0,  t_("There are no errors"));

    Add(1,  t_("Invalid Type of entry"));
    Add(2,  t_("Invalid frequency"));
    Add(3,  t_("Invalid Frequency unit"));
    Add(6,  t_("Missing Geographical co-ordinates"));
    Add(7,  t_("Invalid Height of the station site above sea level"));
    Add(8,  t_("Missing frequency of transmitter spectrum mask or receiver selectivity mask"));
    Add(9,  t_("Invalid NFD1"));
    Add(10, t_("Invalid NFD2"));
    Add(11, t_("Invalid Channel spacing"));
    Add(12, t_("Invalid Maximum permitted transmitter power"));
    Add(13, t_("Invalid ATPC"));
    Add(14, t_("Invalid Azimuth"));
    Add(15, t_("Invalid Elevation"));
    Add(16, t_("Invalid Polarization"));
    Add(17, t_("Invalid Receiver noise level"));
    Add(18, t_("Invalid Branches and line losses"));
    Add(19, t_("Invalid Height of antenna above ground"));
    Add(20, t_("Invalid antenna gain"));
    Add(21, t_("Invalid Copolar radiation pattern"));
    Add(22, t_("Invalid Number of copolar radiation pattern"));
    Add(23, t_("Invalid Copolar antenna data or missing"));
    Add(24, t_("Invalid Crosspolar radiation pattern"));
    Add(25, t_("Invalid Number of crosspolar radiation pattern data"));
    Add(26, t_("Invalid Crosspolar antenna data or missing"));
    Add(27, t_("Missing attenuation of transmitter spectrum mask or receiver selectivity mask"));
    Add(28, t_("Frequency is not present"));
    Add(29, t_("Frequency unit is not present"));
    Add(30, t_("Error in frequency range (only 1 - 39.5 GHz are permissible)"));
    Add(31, t_("No height of the station site above sea level available"));
    Add(32, t_("Error in transmitter spectrum mask or receiver selectivity mask"));
    Add(33, t_("Transmitter power not present"));
    Add(34, t_("Azimuth not present"));
    Add(35, t_("Elevation not present"));
	Add(36, t_("Error opening topo data file (no data)"));
    Add(37, t_("Receiver noise power level not present"));
    Add(38, t_("No antenna height above ground"));
    Add(39, t_("Antenna gain not present"));
    Add(40, t_("Antenna data (standard) not present"));
    Add(42, t_("Equipment manufacturer name not present"));
    Add(43, t_("Equipment type not present"));

	Add(101,  t_("Invalid geographical coordinates"));
	Add(102,  t_("Latitude is not in range of 0.0 - 90.0"));
	Add(103,  t_("Longitude is not in range of 0.0 - 360.0"));

	// terrain.h -----------------------------------------
	Add(220,  t_("Error reading terrain record"));
    Add(361,  t_("Polarization not available"));
	Add(400,  t_("Height is missing (-9999)"));
	Add(401,  t_("Error opening file (no data)"));
	Add(402,  t_("Error reading record"));
	Add(406,  t_("Distance = 0"));

	// nfd.h ---------------------------------------------
	Add(501,  t_("Tx and Rx masks do not overlap"));
	Add(502,  t_("Incorrect class name"));
	Add(503,  t_("Cannot find default mask"));

	// annex11.h -----------------------------------------------------------------------------------
	Add(601,  t_("No borderline data found"));
	Add(602,  t_("Bad coordinates"));
	Add(603,  t_("Bad frequency"));
	Add(604,  t_("Error opening file"));
	Add(605,  t_("Error reading file"));

	Add(999,  t_("HCM record is empty"));
	Add(1000, t_("Tx record is not a Tx"));
	Add(1001, t_("Rx record is not a Rx"));
	Add(1002, t_("Tx frequency (1A) not present"));
	Add(1003, t_("Tx frequency unit (1A1) not present"));
	Add(1004, t_("Tx co-ordinates (4C) not present"));
	Add(1005, t_("Tx height of site above sea level (4Z) not present"));
	Add(1006, t_("Transmitter spectrum mask  (7G) (frequency) is not present"));
	Add(1007, t_("Transmitter spectrum mask  (7G) (attenuation) is not present"));
	Add(1008, t_("Tx power (8A) is not present"));
	Add(1010, t_("Tx azimuth (9A) is not present"));
	Add(1011, t_("Tx elevation (9B) is not present"));
	Add(1012, t_("Tx polarization (9D) is not present"));
	Add(1014, t_("Tx antenna height above ground (9Y) is not present"));
	Add(1015, t_("Tx antenna gain (part of 9X) is not present"));
	Add(1016, t_("Default Tx antenna radiation pattern (9X) is not present"));
	Add(1020, t_("Rx frequency (1A) not present"));
	Add(1021, t_("Rx frequency unit (1A1) not present"));
	Add(1022, t_("Rx co-ordinates (4C) not present"));
	Add(1023, t_("Rx height of site above sea level (4Z) not present"));
	Add(1024, t_("Error in Tx channel spacing (7G3)"));
	Add(1026, t_("Rx azimuth (9A) is not present"));
	Add(1027, t_("Rx elevation (9B) is not present"));
	Add(1028, t_("Rx polarization (9D) is not present"));
	Add(1029, t_("Rx noise power level FKTB (9H) is not present"));
	Add(1031, t_("Rx antenna height above ground (9Y) is not present"));
	Add(1032, t_("Rx antenna gain (part of 9X) is not present"));
	Add(1033, t_("Default Rx antenna radiation pattern (9X) is not present"));
	Add(1037, t_("Error in Tx frequency (1A)"));
	Add(1038, t_("Error in Tx frequency unit (1A1)"));
	Add(1039, t_("Error in Tx frequency range (only 1 - 43.5 GHz are permissible)"));
	Add(1040, t_("Error in Rx frequency (1A)"));
	Add(1041, t_("Error in Rx frequency unit (1A1)"));
	Add(1042, t_("Error in Rx frequency range (only 1 - 43.5 GHz are permissible)"));
	Add(1043, t_("Error in Tx co-ordinates (4C)"));
	Add(1044, t_("Error in Rx co-ordinates (4C)"));
	Add(1045, t_("Error in Tx site height (4Z)"));
	Add(1046, t_("Error in Rx site height (4Z)"));
	Add(1047, t_("Error in Tx spectrum mask data (7G)"));
	Add(1048, t_("Error in Rx selectivity mask data (7G)"));
	Add(1049, t_("Error in Tx power (8A)"));
	Add(1050, t_("Error in Tx ATPC (8B3)"));
	Add(1051, t_("Error in Tx azimuth (9A)"));
	Add(1052, t_("Error in Rx azimuth (9A)"));
	Add(1053, t_("Error in Tx elevation (9B)"));
	Add(1054, t_("Error in Rx elevation (9B)"));
	Add(1055, t_("Error in (Rx) FkTB (9H)"));
	Add(1056, t_("Error in Tx line losses (9L)"));
	Add(1057, t_("Error in Rx line losses (9L)"));
	Add(1058, t_("Error in Tx antenna height (9Y)"));
	Add(1059, t_("Error in Rx antenna height (9Y)"));
	Add(1060, t_("Error in Tx antenna gain (in 9X)"));
	Add(1061, t_("Error in Rx antenna gain (in 9X)"));
	Add(1062, t_("Tx position = Rx position; calculations not possible"));
	Add(1067, t_("Antenna co-polar data for the respective polarisation is not available"));
	Add(1068, t_("Antenna cross-polar data for the respective polarisation is not available"));
	Add(1070, t_("Receiver selectivity mask (7G)(frequency) is not present"));
	Add(1071, t_("Receiver selectivity mask (7G)(attenuation) is not present"));
	Add(1075, t_("Rx equipment manufacturer name (7H) not present"));
	Add(1077, t_("PTx record is not a PTx"));
	Add(1079, t_("PTx co-ordinates (4C) not present"));
	Add(1080, t_("PTx height of site above sea level (4Z) not present"));
	Add(1081, t_("PTx azimuth (9A) is not present"));
	Add(1082, t_("PTx elevation (9B) is not present"));
	Add(1083, t_("PTx polarization (9D) is not present"));
	Add(1084, t_("PTx antenna height above ground (9Y) is not present"));
	Add(1085, t_("PTx antenna gain (part of 9X) is not present"));
	Add(1086, t_("Default PTx antenna radiation pattern (9X) is not present"));
	Add(1090, t_("PRx record is not a PRx"));
	Add(1091, t_("PRx co-ordinates (4C) not present"));
	Add(1092, t_("PRx height of site above sea level (4Z) not present"));
	Add(1093, t_("PRx azimuth (9A) is not present"));
	Add(1094, t_("PRx elevation (9B) is not present"));
	Add(1095, t_("PRx polarization (9D) is not present"));
	Add(1096, t_("PRx antenna height above ground (9Y) is not present"));
	Add(1097, t_("PRx antenna gain (part of 9X) is not present"));
	Add(1098, t_("Default PRx antenna radiation pattern (9X) is not present"));
	Add(1102, t_("Error in PTx co-ordinates (4C)"));
	Add(1103, t_("Error in PRx co-ordinates (4C)"));
	Add(1104, t_("Error in PTx site height (4Z)"));
	Add(1105, t_("Error in PRx site height (4Z)"));
	Add(1106, t_("Error in PTx azimuth (9A)"));
	Add(1107, t_("Error in PRx azimuth (9A)"));
	Add(1108, t_("Error in PTx elevation (9B)"));
	Add(1109, t_("Error in PRx elevation (9B)"));
	Add(1110, t_("Error in PTx line losses (9L)"));
	Add(1111, t_("Error in PRx line losses (9L)"));
	Add(1112, t_("Error in PTx antenna height (9Y)"));
	Add(1113, t_("Error in PRx antenna height (9Y)"));
	Add(1114, t_("Error in PTx antenna gain (in 9X)"));
	Add(1115, t_("Error in PRx antenna gain (in 9X)"));
	Add(1116, t_("Tx position = PRx position)); calculations not possible"));
	Add(1118, t_("PTx position = Rx position)); calculations not possible"));
	Add(1203, t_("Invalid Tx copolar radiation pattern (part of 9X)"));
	Add(1204, t_("Invalid number of Tx copolar radiation pattern data (part of 9X)"));
	Add(1205, t_("Invalid or missing Tx copolar antenna data (part of 9X)"));
	Add(1206, t_("Invalid or missing Tx crosspolar radiation pattern (part of 9X)"));
	Add(1207, t_("Invalid number of Tx crosspolar radiation pattern data (part of 9X)"));
	Add(1208, t_("Invalid or missing Tx crosspolar antenna data (part of 9X)"));
	Add(1209, t_("Invalid Rx copolar radiation pattern (part of 9X)"));
	Add(1210, t_("Invalid number of Rx copolar radiation pattern data (part of 9X)"));
	Add(1211, t_("Invalid or missing Rx copolar antenna data (part of 9X)"));
	Add(1212, t_("Invalid or missing Rx crosspolar radiation pattern (part of 9X)"));
	Add(1213, t_("Invalid number of Rx crosspolar radiation pattern data (part of 9X)"));
	Add(1214, t_("Invalid or missing Rx crosspolar antenna data (part of 9X)"));
	Add(1215, t_("Invalid PTx polarization (9D)"));
	Add(1216, t_("Invalid PRx polarization (9D)"));
	Add(1217, t_("Invalid Tx polarization (9D)"));
	Add(1218, t_("Invalid Rx polarization (9D)"));
	Add(1219, t_("Invalid PTx copolar radiation pattern (part of 9X)"));
	Add(1220, t_("Invalid PRx copolar radiation pattern (part of 9X)"));
	Add(1221, t_("Invalid number of PTx copolar radiation pattern data (part of 9X)"));
	Add(1222, t_("Invalid number of PRx copolar radiation pattern data (part of 9X)"));
	Add(1223, t_("Invalid or missing PTx copolar antenna data (part of 9X)"));
	Add(1224, t_("Invalid or missing PRx copolar antenna data (part of 9X)"));
	Add(1225, t_("Invalid or missing PTx crosspolar radiation pattern (part of 9X)"));
	Add(1226, t_("Invalid or missing PRx crosspolar radiation pattern (part of 9X)"));
	Add(1227, t_("Invalid number of PTx crosspolar radiation pattern data (part of 9X)"));
	Add(1228, t_("Invalid number of PRx crosspolar radiation pattern data (part of 9X)"));
	Add(1229, t_("Invalid or missing PTx crosspolar antenna data (part of 9X)"));
	Add(1230, t_("Invalid or missing PRx crosspolar antenna data (part of 9X)"));
	Add(1233, t_("Tx spectrum mask has less than 2 pairs of frequency and attenuation"));
	Add(1234, t_("Rx selectivity mask has less than 2 pairs of frequency and attenuation"));
	Add(1235, t_("Last attenuation of Tx spectrum mask < 40 dB"));
	Add(1236, t_("Last attenuation of Rx selectivity mask < 40 dB"));
	Add(1237, t_("Error opening the result file"));
	Add(1238, t_("Error writing data to the result file"));
	Add(1241, t_("Invalid name of transmitter equipment class (7G1)"));
	Add(1242, t_("Invalid name of receiver equipment class (7G1)"));
	Add(1243, t_("No default transmitter mask data (7G) available"));
	Add(1244, t_("No default receiver mask data (7G) available"));
	Add(1245, t_("Could not find parameter file hcmrc"));
	Add(1246, t_("Could not open registry key HKEY_LOCAL_MASHINE\\Software\\HCM"));
	Add(1247, t_("Could not read topo path value from registry"));
	Add(1248, t_("Error in Rx channel spacing (7G3)"));
}


#endif
