
// Error codes
//  TStdAnt
//    3 - No standard antenna data found

#include <algorithm>
#include <cctype>
#include <fstream>
#include <errno.h>
#include <math.h>

#include "hcmData.h"

//!---------------------------------------------------------------------------------------------

string Trim(const string& s){
	string str = s;
	string::size_type pos = str.find_last_not_of(" \r\n\t");
	if(pos != string::npos){
		str.erase(pos + 1);
		pos = str.find_first_not_of(" \r\n\t");
	    if(pos != string::npos) str.erase(0, pos);
	}
	else
		str.erase(str.begin(), str.end());
	return str;
}
//!---------------------------------------------------------------------------------------------

// Converts std::string to double
// returns 0 if OK, 1 if error
double StrToDouble(const string& s, int& error){
	error = 0;
	if(s.size() > 0){
		char* endPtr;
		string str = Trim(s);
		if(str.size() == 0){
			error = 1;
			return 0.0;
		}
		errno = 0;
		endPtr = &str[str.size()-1];
		double result = strtod(str.c_str(), &endPtr);
		if(errno != 0) return 1;
		if(str.size() != static_cast<size_t>(endPtr - &str[0])){
			error = 1;
			return 0.0;
		}
		return result;
	}
	else{
		error = 1;
		return 0.0;
	}
}
//!---------------------------------------------------------------------------------------------

int StrToLong(const string& s, int& error){
	error = 0;
	if (s.size() > 0){
		char * endPtr;
		string str = Trim(s);
		if(str.size() == 0){
			error = 1;
			return 0;
		}
		endPtr = &str[str.size()-1];
		int result = strtoul(str.c_str(), &endPtr,10);
		if (errno == ERANGE) return 1;
		if(str.size() != static_cast<size_t>(endPtr - &str[0])){
			error = 1;
			return 0;
		}
		return result;
	}
	else{
		error = 1;
		return 0;
	}
}
//!---------------------------------------------------------------------------------------------


// Converts format of coordinates from string to double
// Example: 025E3000 -> 25.5
// Error codes:
//	0 = OK
//	101 = coordinate is wrong,
//	102 = latitude is not in range of 0.0 - 90.0,
//	103 = longitude is not in range of 0.0 - 360.0.
double ToCoordinate(const string& coordinate, int& error){
	size_t i = coordinate.find_first_of("NSWEnswe");
	if(i == string::npos){
		error = 101;
		return  0.0;
	}
	int err;
	int degree = (int)StrToLong(coordinate.substr(0, i),err);
	if(err !=0) {error = 101; return 0.0;}
	int minute = (int)StrToLong(coordinate.substr(i+1,2),err);
	if(err !=0) {error = 101; return 0.0;}
	int second = (int)StrToLong(coordinate.substr(i+3,2),err);
	if(err !=0) {error = 101; return 0.0;}
	double d = (second / 60. + minute) / 60. + degree;
	if(coordinate.find_first_of("EWew") != string::npos){
		if(degree > 360){
			error = 103;
			return 0.0;
		}
		if(degree > 180) d = d - 360;
	}
	else{
		if(degree > 90){
			error = 102;
			return 0.0;
		}
	}
	if(coordinate.find_first_of("SWsw") != string::npos) d = -d;
	error = 0;
	return d;
}
//!---------------------------------------------------------------------------------------------

// Converts HCM data string to HcmRec structure
// Data not available = NO_DATA
// Error codes:
//	0 - OK
//	1  Invalid Type of entry
//	2  Invalid frequency
//	3  Invalid Frequency unit
//	6  Missing Geographical co-ordinates
//	7  Invalid Height of the station site above sea level
//	8  Missing frequency of transmitter spectrum mask or receiver selectivity mask
//	9  Invalid NFD1
//	10 Invalid NFD2
//	11 Invalid Channel spacing
//	12 Invalid Maximum permitted transmitter power
//	13 Invalid ATPC
//	14 Invalid Azimuth
//	15 Invalid Elevation
//	16 Invalid Polarization
//	17 Invalid Receiver noise level
//	18 Invalid Branches and line losses
//	19 Invalid Height of antenna above ground
//	20 Invalid antenna gain
//	21 Invalid Copolar radiation pattern
//	22 Invalid Number of copolar radiation pattern
//	23 Invalid Copolar antenna data or missing
//	24 Invalid Crosspolar radiation pattern
//	25 Invalid Number of crosspolar radiation pattern data
//	26 Invalid Crosspolar antenna data or missing
//	27 Missing attenuation of transmitter spectrum mask or receiver selectivity mask
//	28  Frequency is not present
//	29  Frequency unit is not present
//	30  Error in frequency range (only 1 - 39.5 GHz are permissible)
//	101 Invalid geographical coordinates
//	102 Latitude is not in range of 0.0 - 90.0
//	103 Longitude is not in range of 0.0 - 360.0
//	31  No height of the station site above sea level available
//	32  Error in transmitter spectrum mask or receiver selectivity mask
//	33  Transmitter power not present
//	34  Azimuth not present
//	35  Elevation not present
//	36  Polarization not available
//	37  Receiver noise power level not present
//	38  No antenna height above ground
//	39  Antenna gain not present
//	40  Antenna data (standard) not present
//	42  Equipment manufacturer name not present
//	43  Equipment type not present


void AddReport(const string& caption, const string& entry, const string& vstr, string& repStr){
	repStr = repStr + "  " + entry + " " + caption + "\t= " + vstr + "\n";
}
void AddReport(const string& str, string& repStr){
	repStr = repStr + str + "\n";
}
//!---------------------------------------------------------------------------------------------

string HcmRec::ToPrint()const{
	string repStr = "";
	string un = "";
	string tmp = "";
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
	AddReport("Type of entry         ", un, tmp, repStr);
	AddReport("Frequency             ", un, to_string(frequency), repStr);
	AddReport("Frequency unit        ", un, "G", repStr);
	AddReport("Longitude             ", un, to_string(longitude), repStr);
	AddReport("Latitude              ", un, to_string(latitude), repStr);
	AddReport("Height above sea      ", un, to_string(h_asl), repStr);
	AddReport("Equip. manufacture    ", un, eqp_manuf_name, repStr);
	AddReport("Equipment type        ", un, eqp_type, repStr);
	AddReport("Maximum capacity      ", un, capacity, repStr);
	AddReport("  " + un + " " + "Spectrum mask:",  repStr);
	AddReport(to_string("", mask), repStr);
	AddReport("Channel spacing       ", un, to_string(channel_spacing), repStr);
	AddReport("Maximum power         ", un, to_string(power), repStr);
	AddReport("ATPC                  ", un, to_string(atpc), repStr);
	AddReport("Azimuth               ", un, to_string(azimuth), repStr);
	AddReport("Elevation             ", un, to_string(elevation), repStr);
	AddReport("Polarization          ", un, (polarization), repStr);
	AddReport("Branches + line losses", un, to_string(losses), repStr);
	AddReport("Antenna height        ", un, to_string(h_ant), repStr);
	AddReport("Antenna manufacture   ", un, (ant_manuf_name), repStr);
	AddReport("Antenna type          ", un, (ant_type), repStr);
	AddReport("Antenna gain          ", un, to_string(gain), repStr);
	AddReport("Antenna cop. pattern  ", un, (cp_pattern), repStr);
	AddReport("  " + un + " " + "Antenna cop. data:", repStr);
	AddReport(to_string("", cpData), repStr);
	AddReport("Antenna cro. pattern  ", un, (xp_pattern), repStr);
	AddReport("  " + un + " " + "Antenna cro. data:", repStr);
	AddReport(to_string("", xpData), repStr);
	
	return repStr;
}

//!---------------------------------------------------------------------------------------------

int SetTypeOfEntry(int num, const vector<string>& vhcm, HcmRec& data, bool doReport, string& un, string& repStr){
	//--- TYPE OF ENTRY
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		// make type of entry uppercase
		std::transform(vstr.begin(), vstr.end(), vstr.begin(), (int(*)(int)) std::toupper);
		if(!(vstr == "TX" || vstr == "RX" || vstr == "PTX" || vstr == "PRX")) return 1;
		if(vstr.find("TX") != vstr.npos)
			data.isTx = true;
		else
			data.isTx = false;
		if(vstr.find("P") != vstr.npos)
			data.isPassive = true;
		else
			data.isPassive = false;
		if(data.isTx && data.isPassive)
			un = "(PTx)";
		else
			if(data.isTx && !data.isPassive)
				un = "(Tx)";
			else
				if(!data.isTx && data.isPassive)
					un = "(PRx)";
				else
					un = "(Rx)";
		if(doReport) AddReport("Type of entry", un, vstr, repStr);
		// ToLogFile("hcm4afs_dll.log", "SetTypeOfEntry", un);
		// ToLogFile("hcm4afs_dll.log", "data.isPassive", data.isPassive);

		return 0;
	}
	return 999;
}
//!---------------------------------------------------------------------------------------------
int SetFrequency(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- FREQUENCY
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		if(!data.isPassive){
			// Not passive
			if(doReport) AddReport("Frequency", un, vstr, repStr);
			if(vstr.size() == 0){
				if(data.isTx)
					return 1002; // no Tx frequency
				else
					return 1020; // no Rx frequency
			}
			int err;
			data.frequency = StrToDouble(vstr, err);
			if(err != 0){
				if(data.isTx)
					return 1037; // invalid Tx frequency
				else
					return 1040; // invalid Rx frequency
			}
		}
		else{
			// If passive, frequency is not required
			data.frequency = NO_DATA;
		}
		return 0;
	}
	return 2;
}
//!---------------------------------------------------------------------------------------------
int SetFreqUnit(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- FREQUENCY UNIT
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		if(!data.isPassive){
			if(vstr.size() == 0){
				if(data.isTx)
					return 1003; // no Tx frequency unit
				else
					return 1021; // no Rx frequency unit
			}
			if(doReport) AddReport("Frequency unit", un, vstr, repStr);
			// make frequency unit uppercase
			std::transform(vstr.begin(), vstr.end(), vstr.begin(), (int(*)(int))std::toupper);
			if(vstr == "K")
				data.frequency = data.frequency / 1E6;
			else{
				if(vstr == "M")
					data.frequency = data.frequency / 1000;
				else
					if(vstr != "G"){
						if(data.isTx)
							return 1038; // for Tx
						else
							return 1041; // for Rx
					}
			}
			if(data.frequency > 43.5 || data.frequency < 1){
				if(data.isTx)
					return 1039; // Tx frequency outside the range 1 - 43.5
				else
					return 1042; // Rx frequency outside the range 1 - 43.5
			}
		}
		return 0;
	}
	return 3;
}
//!---------------------------------------------------------------------------------------------
int SetCoord(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- GEOGRAPHICAL CO-ORDINATES
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		if(doReport) AddReport("Geo. co-ordinates", un, vstr, repStr);
		if(!vstr.empty()){
			int err = 0;
			data.longitude = ToCoordinate(vstr.substr(0, 8), err);
			if(err != 0 || vstr.size() != 15){
				if(data.isPassive){
					// For Passive repeater
					if(data.isTx)
						return 1102; // Error in PTx coordinates
					else
						return 1103; // Error in PRx coordinates
				}else{
					// For active
					if(data.isTx)
						return 1043; // Error in Tx coordinates
					else
						return 1044; // Error in Rx coordinates
				}
			}
			data.latitude = ToCoordinate(vstr.substr(8,7),err);
			if(err != 0){
				if(data.isPassive){
					// For Passive repeater
					if(data.isTx)
						return 1102; // Error in PTx coordinates
					else
						return 1103; // Error in PRx coordinates
				}else{
					// For active
					if(data.isTx)
						return 1043; // Error in Tx coordinates
					else
						return 1044; // Error in Rx coordinates
				}
			}
		}else{
			// coordinates are missing
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1079;
				else
					return 1091;
			}else{
				// for active case
				if(data.isTx)
					return 1004;
				else
					return 1022;
			}
		}
		return 0;
	}
	return 6;
}
//!---------------------------------------------------------------------------------------------
int SetStationHeight(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- HEIGHT OF THE STATION SITE ABOVE SEA LEVEL
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		if(doReport) AddReport("Height above sea", un, vstr, repStr);
		if(vstr.size() == 0){
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1080; // for PTx
				else
					return 1092; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1005; // for Tx
				else
					return 1023; // for Rx
			}
		}
		int err;
		data.h_asl = (short)StrToLong(vstr,err);
		if(err != 0){
			// invalid height
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1104; // for PTx
				else
					return 1105; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1045; // for Tx
				else
					return 1046; // for Rx
			}
		}
		return 0;
	}
	return 7;
}
//!---------------------------------------------------------------------------------------------
int SetTransSpectrumMask(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- TRANSMITTER SPECTRUM MASK OR RECEIVER SELECTIVITY MASK
	if(vhcm.size()>num){
		int err;
		if(!data.isPassive){
			if(doReport) AddReport("Spectrum mask", un, "", repStr);
			int j = num; //16;
			while(!((vhcm[j].empty() && vhcm[j+1].empty()) || j >= 28)){
				TrMask msk;
				if(vhcm[j].empty() && vhcm[j+1].empty()) break; // no mask data left, if frequency and attenuation is empty
				if(doReport) repStr = repStr + "                             =    " + vhcm[j] + "\t\t" + vhcm[j+1] + "\n";
				if(vhcm[j].empty() && !vhcm[j+1].empty()){
					if(data.isTx)
						return 1006; // for Tx
					else
						return 1070; // for Rx
				}
				if(!vhcm[j].empty() && vhcm[j+1].empty()){
					if(data.isTx)
						return 1007; // for Tx
					else
						return 1071; // for Rx
				}
				msk.frequency = StrToDouble(vhcm[j], err);
				if(err !=0){
					if(data.isTx)
						return 1047; // for Tx
					else
						return 1048; // for Rx
				}
				j++;
				msk.attenuation = StrToDouble(vhcm[j],err);
				if(err !=0){
					if(data.isTx)
						return 1047; // for Tx
					else
						return 1048; // for Rx
				}
				if(!(msk.frequency == 0 && msk.attenuation == 0 && data.mask.size() > 0)){
					// if we get mask point (0,0) not in the first position, omit it.
					data.mask.push_back(msk);
				}
				j++;
			}
		}
		return 0;
	}
	return 8;
}
//!---------------------------------------------------------------------------------------------
int SetEqupmentClass(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		//--- EQUIPMENT CLASS
		if(!data.isPassive){
			if(!vstr.empty()){
				data.eqclass = vstr;
				if(data.mask.size() <=0){
					// look at equipment class only if we have no mask data
					if (!(data.eqclass == "1"  || data.eqclass == "2"  || data.eqclass == "3"  ||
						  data.eqclass == "4H" || data.eqclass == "4L" || data.eqclass == "5A" ||
						  data.eqclass == "5B" || data.eqclass == "6A" || data.eqclass == "6B" ||
						  data.eqclass == "4h" || data.eqclass == "4l" || data.eqclass == "5a" ||
						  data.eqclass == "5b" || data.eqclass == "6a" || data.eqclass == "6b")){
						if(data.isTx)
							return 1241; // invalid transmitter equipment class
						else
							return 1242; // invalid receiver equipment class
					}
				}
			}
			else
				data.eqclass = string(); // data not available
		}
		else
			data.eqclass = string();	 // data not available
		return 0;
	}
	return 1241;
}
//!---------------------------------------------------------------------------------------------
int SetChannelSpacing(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- CHANNEL SPACING
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		if(!data.isPassive){
			int err;
			data.channel_spacing = StrToDouble(vstr, err);
			if(err !=0){
				if(data.isTx)
					return 1024; // invalid channel spacing
				else
					return 1248;
			}
			if(data.channel_spacing <=0){
				if(data.isTx)
					return 1024; // invalid channel spacing
				else
					return 1248;
			}
		}
		else
			data.channel_spacing = NO_DATA;
		return 0;
	}
	return 11;
}
//!---------------------------------------------------------------------------------------------
int Set78MaskElement(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- SET 7-th and 8-th MASK ELEMENTS (2.5 * CHANNEL_SPACING and 3.5 * CHANNEL_SPACING)
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		if(!data.isPassive){
			if(data.mask.size() > 0){
				if(data.mask.size() < 2){
					// At least 2 pairs have to be present
					if(data.isTx)
						return 1233;
					else
						return 1234;
				}
				if(data.mask[data.mask.size()-1].attenuation < 40){
					// Last point must have attenuation >= 40 dB
					if(data.isTx)
						return 1235;
					else
						return 1236;
				}
				if(data.mask[data.mask.size() - 1].frequency < data.channel_spacing * 2.5){
					// Add 7th point which = 2.5 x channel spacing
					TrMask msk;
					msk.frequency = data.channel_spacing * 2.5;
					msk.attenuation = data.mask[data.mask.size() - 1].attenuation;
					data.mask.push_back(msk);
				}
				if(data.mask[data.mask.size() - 1].frequency < data.channel_spacing * 3.5){
					// Add 8th point which = 3.5 x channel spacing
					TrMask msk;
					msk.frequency = data.channel_spacing * 3.5;
					msk.attenuation = data.mask[data.mask.size() - 1].attenuation + 5;
					data.mask.push_back(msk);
				}
			}
			if(doReport) AddReport("Channel spacing", un, vstr, repStr);
		}
		return 0;
	}
	return 11;
}
//!---------------------------------------------------------------------------------------------
int SetMaxPermTrPower(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- MAXIMUM PERMITTED TRANSMITTER POWER
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		if(data.isTx && !data.isPassive){
			int err;
			if(doReport) AddReport("Maximum power", un, vstr, repStr);
			if(vstr.size() == 0) return 1008; // power not present
			data.power = StrToDouble(vstr, err);
			if(err !=0) return 1049; // invalid maximum permitted transmitter power
		}
		return 0;
	}
	return 12;
}
//!---------------------------------------------------------------------------------------------
int SetAtrc(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- ATPC
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		if(!data.isPassive && data.isTx){
			if(doReport) AddReport("ATPC", un, vstr, repStr);
			int err;
			if(!vstr.empty()){
				data.atpc = StrToDouble(vstr, err);
				if(err!=0 || data.atpc < 0) return 1050; // invalid ATPC
			}
			else
				data.atpc = 0;
		}
		else
			data.atpc = NO_DATA;
		return 0;
	}
	return 13;
}
//!---------------------------------------------------------------------------------------------
int SetAzimuth(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- AZIMUTH
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		if(doReport) AddReport("Azimuth", un, vstr, repStr);
		if(vstr.size() == 0){
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1081; // for PTx
				else
					return 1093; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1010; // for Tx
				else
					return 1026; // for Rx
			}
		}
		int err;
		data.azimuth = StrToDouble(vstr,err);
		if(err!=0 || data.azimuth > 360 || data.azimuth < 0){
			// invalid azimuth
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1106; // for PTx
				else
					return 1107; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1051; // for Tx
				else
					return 1052; // for Rx
			}
		}
		return 0;
	}
	return 14;
}
//!---------------------------------------------------------------------------------------------
int SetElevation(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- ELEVATION
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		if(doReport) AddReport("Elevation", un, vstr, repStr);
		if(vstr.size() == 0){
			// elevation not present
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1082; // for PTx
				else
					return 1094; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1011; // for Tx
				else
					return 1027; // for Rx
			}
		}
		int err;
		data.elevation = StrToDouble(vstr, err);
		if(err !=0 || fabs(data.elevation) > 90){
			// invalid elevation
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1108; // for PTx
				else
					return 1109; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1053; // for Tx
				else
					return 1054; // for Rx
			}
		}
		return 0;
	}
	return 15;
}
//!---------------------------------------------------------------------------------------------
int SetPolarization(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- POLARIZATION
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		if(doReport) AddReport("Polarization", un, vstr, repStr);
		string tmpStr = Trim(vstr);
		if(tmpStr.size() == 0){
			// polarization not present
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1083; // for PTx
				else
					return 1095; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1012; // for Tx
				else
					return 1028; // for Rx
			}
		}
		// make polarization uppercase
		std::transform(tmpStr.begin(), tmpStr.end(), tmpStr.begin(), (int(*)(int))std::toupper);
		if(tmpStr == "H" || tmpStr == "V")
			data.polarization = tmpStr;
		else{
			if (data.isPassive){
				// for passive case
				if(data.isTx)
					return 1215; // for PTx
				else
					return 1216; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1217; // for Tx
				else
					return 1218; // for Rx
			}
		}
		return 0;
	}
	return 16;
}
//!---------------------------------------------------------------------------------------------
int SetRxPowerLevel(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- RECEIVER NOISE POWER LEVEL
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		if(!data.isTx && !data.isPassive){
			if(doReport) AddReport("Noise power level FkTB", un, vstr, repStr);
			if(vstr.size() == 0) return 1029; // noise power level not present
			int err;
			data.noise = StrToDouble(vstr, err);
			if(err!=0) return 1055; // invalid receiver noise
		}
		return 0;
	}
	return 17;
}
//!---------------------------------------------------------------------------------------------
int SetBranchAndLineLoss(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- BRANCHES AND LINE LOSSES
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		if(doReport) AddReport("Branches + line losses", un, vstr, repStr);
		if(!vstr.empty()){
			int err;
			data.losses = StrToDouble(vstr, err);
			if(err !=0 || data.losses < 0){
				// invalid branches and line losses
				if(data.isPassive){
					// for passive case
					if(data.isTx)
						return 1110; // for PTx
					else
						return 1111; // for PRx
				}else{
					// for active case
					if(data.isTx)
						return 1056; // for Tx
					else
						return 1057; // for Rx
				}
			}
		}
		else
			data.losses = 0;
		return 0;
	}
	return 18;
}
//!---------------------------------------------------------------------------------------------
int SetAntGroundHeight(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- HEIGHT OF ANTENNA ABOVE GROUND
	if(vhcm.size()>num){
		string vstr = vhcm[num];
		if(doReport) AddReport("Antenna height", un, vstr, repStr);
		if(vstr.size() == 0){
			// antenna height not present
			if (data.isPassive){
				// for passive case
				if(data.isTx)
					return 1084; // for PTx
				else
					return 1096; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1014; // for Tx
				else
					return 1031; // for Rx
			}
		}
		int err;
		data.h_ant = (short)StrToDouble(vstr, err);
		if(err !=0 || data.h_ant < 0){
			// invalid height of antenna above ground
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1112; // for PTx
				else
					return 1113; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1058; // for Tx
				else
					return 1059; // for Rx
			}
		}
		return 0;
	}
	return 19;
}
//!---------------------------------------------------------------------------------------------
int SetAntGain(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- ANTENNA GAIN
	if(vhcm.size() < num){
		// antenna gain not present
		if(data.isPassive){
			// for passive case
			if(data.isTx)
				return 1085; // for PTx
			else
				return 1097; // for PRx
		}else{
			// for active case
			if(data.isTx)
				return 1015; // for Tx
			else
				return 1032; // for Rx
		}
	}
	else{
		string vstr = vhcm[num];
		if(doReport) AddReport("Antenna gain", un, vstr, repStr);
		if(vstr.size() == 0){
			// antenna gain not present
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1085; // for PTx
				else
					return 1097; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1015; // for Tx
				else
					return 1032; // for Rx
			}
		}
		int err;
		data.gain = StrToDouble(vstr, err);
		if(err !=0){
			// invalid antenna gain
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1114; // for PTx
				else
					return 1115;// for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1060; // for Tx
				else
					return 1061; // for Rx
			}
		}
		return 0;
	}
	return 20;
}
//!---------------------------------------------------------------------------------------------
int SetAntPattern(TStdAnt& stdAnt, int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr){
	//--- ANTENNA COPOLAR RADIATION PATTERN
	if(vhcm.size() > num){
		string vstr = vhcm[num];
		if(doReport) AddReport("Antenna cop. pattern", un, vstr, repStr);
		// make antenna pattern uppercase
		std::transform(vstr.begin(), vstr.end(), vstr.begin(), (int(*)(int))std::toupper);
		if(vstr == "CP" || vstr == "VV" || vstr == "HH")
			data.cp_pattern = vstr;
		else{
			// invalid copolar radiation pattern
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1219; // for PTx
				else
					return 1220; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1203; // for Tx
				else
					return 1209; // for Rx
			}
		}
	}else{
		// Put standard antenna CP data, if missing
		if(doReport){
			AddReport("Antenna cop. pattern", un, "  = CP", repStr);
			AddReport("Antenna cop. data", un, ":", repStr);
		}
		int err = SetCP(data, stdAnt, repStr);
		if(err == 0){
			if(doReport){
				AddReport("Antenna cro. pattern", un, "  = XP", repStr);
				AddReport("Antenna cro. data", un, ":", repStr);
			}
			return SetXP(data, stdAnt, repStr);
		}
		else return err; // ALL data is set
	}
	return 0;
}
//!---------------------------------------------------------------------------------------------

int SetNumOfMaskData(int num, const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr, int& nr){
	//--- Number of  mask data
	if(vhcm.size() > num){
		string vstr = vhcm[num];
		int err;
		nr = StrToLong(vstr, err);
		if(err !=0){
			// invalid number of copolar radiation pattern data
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1221; // for PTx
				else
					return 1222; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1204; // for Tx
				else
					return 1210; // for Rx
			}
		}
	}
	else{
		if(data.isPassive){
			// for passive case
			if(data.isTx)
				return 1221; // for PTx
			else
				return 1222; // for PRx
		}else{
			// for active case
			if(data.isTx)
				return 1204; // for Tx
			else
				return 1210; // for Rx
		}
	}
	return 0;
}
//!---------------------------------------------------------------------------------------------
int SetPatternData(const vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr, int nr){
	//--- Pattern data
	if(vhcm.size() >= 51 + nr*2){
		if(doReport) AddReport("Antenna cop. data", un, ":", repStr);
		for(int i = 51 ; i < nr*2 + 51 ; i++){
			AntPoint ant;
			if(doReport){
				AddReport("                             =    ", un, ":", repStr);
				AddReport("                             =    " + vhcm[i] + "\t\t" + vhcm[i+1], repStr);
			}
			int err;
			ant.angle = StrToDouble(vhcm[i++], err);
			if(err !=0){
				// invalid copolar antenna data or missing
				if(data.isPassive){
					// for passive case
					if(data.isTx)
						return 1223; // for PTx
					else
						return 1224; // for PRx
				}else{
					// for active case
					if(data.isTx)
						return 1205; // for Tx
					else
						return 1211; // for Rx
				}
			}
			// convert negative value to positive
			if(ant.angle < 0) ant.angle = 360 + ant.angle;
			ant.attenuation = StrToDouble(vhcm[i], err);
			if(err !=0){
				// invalid copolar antenna data or missing
				if(data.isPassive){
					// for passive case
					if(data.isTx)
						return 1223; // for PTx
					else
						return 1224; // for PRx
				}else{
					// for active case
					if(data.isTx)
						return 1205; // for Tx
					else
						return 1211; // for Rx
				}
			}
			data.cpData.push_back(ant);
		}
		// Sort CP antenna data
		std::sort(data.cpData.begin(), data.cpData.end(), (bool(*)(AntPoint,AntPoint))AnCompare);
		// std::sort(data.cpData.begin(), data.cpData.end(), ancompare);
		// If 1st point is missing, add it
		if(data.cpData[0].angle !=0.0){
			AntPoint ant;
			ant.angle = 0.0;
			ant.attenuation = 0.0;
			data.cpData.push_back(ant);
			std::sort(data.cpData.begin(), data.cpData.end(), (bool(*)(AntPoint,AntPoint))AnCompare);
		}
	}
	else{
		// invalid copolar antenna data or missing
		if(data.isPassive){
			// for passive case
			if(data.isTx)
				return 1223; // for PTx
			else
				return 1224; // for PRx
		}else{
			// for active case
			if(data.isTx)
				return 1205; // for Tx
			else
				return 1211; // for Rx
		}
	}
	return 0;
}
//!---------------------------------------------------------------------------------------------
int SetAntRadPattern(TStdAnt& stdAnt, vector<string>& vhcm, const string& un, HcmRec& data, bool doReport, string& repStr, int nr){
	//--- ANTENNA CROSSPOLAR RADIATION PATTERN
	// index of pattern e.g. VH, CP, HV
	int nrxp = 51 + nr*2;
	if(vhcm.size() - 1 > nrxp){
		if(doReport) AddReport("Antenna cro. pattern", un, vhcm[nrxp], repStr);
		// make crosspolar antenna pattern uppercase
		std::transform(vhcm[nrxp].begin(), vhcm[nrxp].end(), vhcm[nrxp].begin(), (int(*)(int))std::toupper);
		if(vhcm[nrxp] == "XP" || vhcm[nrxp] == "HV" || vhcm[nrxp] == "VH")
			data.xp_pattern = vhcm[nrxp];
		else{
			// invalid crosspolar radiation pattern
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1225; // for PTx
				else
					return 1226; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1206; // for Tx
				else
					return 1212; // for Rx
			}
		}
		// Number of  mask data
		int nr1; // number of XP pattern data
		int err;
		if(vhcm.size() >= nrxp+1){
			nr1 = (short)StrToDouble(vhcm[nrxp+1], err);
			if(err !=0){
				// invalid number of crosspolar radiation pattern data
				if(data.isPassive){
					// for passive case
					if(data.isTx)
						return 1227; // for PTx
					else
						return 1228; // for PRx
				}else{
					// for active case
					if(data.isTx)
						return 1207; // for Tx
					else
						return 1213; // for Rx
				}
			}
		}
		else{
			// missing number of crosspolar radiation pattern data
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1227; // for PTx
				else
					return 1228; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1207; // for Tx
				else
					return 1213; // for Rx
			}
		}

		// Crosspolar mask data
		if(vhcm.size() >= 53 + nr*2 + nr1*2){
			if(doReport) AddReport("Antenna cro. data", un, ":", repStr);
			for(int i = 53 + nr*2 ; i < nr*2 + 52 + nr1*2 ; i++){
				AntPoint ant;
				if(doReport) AddReport("                             =    " + vhcm[i] + "\t\t" + vhcm[i+1], repStr);
				ant.angle = StrToDouble(vhcm[i++],err);
				if(err!=0){
					// invalid crosspolar antenna data or missing
					if(data.isPassive){
						// for passive case
						if(data.isTx)
							return 1229; // for PTx
						else
							return 1230; // for PRx
					}else{
						// for active case
						if(data.isTx)
							return 1208; // for Tx
						else
							return 1214; // for Rx
					}
				}
				// convert negative value to positive
				if(ant.angle < 0) ant.angle = 360 + ant.angle;
				ant.attenuation = StrToDouble(vhcm[i], err);
				if(err !=0){
					// invalid crosspolar antenna data or missing
					if(data.isPassive){
						// for passive case
						if(data.isTx)
							return 1229; // for PTx
						else
							return 1230; // for PRx
					}else{
						// for active case
						if(data.isTx)
							return 1208; // for Tx
						else
							return 1214; // for Rx
					}
				}
				data.xpData.push_back(ant);
			}
			// Sort XP antenna data
			std::sort(data.xpData.begin(), data.xpData.end(), (bool(*)(AntPoint,AntPoint))AnCompare);
			// If 1st point is missing, add it
			if(data.cpData[0].angle !=0.0){
				AntPoint ant;
				ant.angle = 0.0;
				ant.attenuation = 0.0;
				data.cpData.push_back(ant);
				std::sort(data.cpData.begin(), data.cpData.end(), (bool(*)(AntPoint,AntPoint))AnCompare);
			}
		}else{
			// invalid crosspolar antenna data or missing
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1229; // for PTx
				else
					return 1230; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1208; // for Tx
				else
					return 1214; // for Rx
			}
		}
	}
	if(data.xpData.size() == 0){
		if(doReport) AddReport("Antenna cro. pattern", un, "  = XP", repStr);
		int err = SetXP(data, stdAnt, repStr);
		return err;
	}
	return 0;
}
//!---------------------------------------------------------------------------------------------

int FillHcmData(TStdAnt& stdAnt, const string& hsmString, HcmRec& data, string& report, bool doReport){
	// HCM string to vector

	int pr = 0;
	vector<string> vhcm;
	for(int i=0; i<= hsmString.length(); i++){
		if(hsmString[i] == ';'){
			vhcm.push_back(hsmString.substr(pr, i-pr));
			pr = i+1;
		}
	}
	if(pr < hsmString.length()+1){
		vhcm.push_back(hsmString.substr(pr, hsmString.length()-pr));
	}
	// it removes the last vector element if it is empty. It happens if ; is at the end of the line
	if(vhcm[vhcm.size() - 1].empty()) vhcm.erase(vhcm.end() - 1);
	// Take vector values and put them to HcmRec structure

	// ToLogFile("hcm4afs_dll.log", "vhcm:", to_string(vhcm));

	string un = "";
	int res = 0;

	//--- TYPE OF ENTRY
	res = SetTypeOfEntry(0, vhcm, data, doReport, un, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetTypeOfEntry", res);
	//--- FREQUENCY
	res = SetFrequency(1, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetFrequency", res);
	//--- FREQUENCY UNIT
	res = SetFreqUnit(2, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetFreqUnit", res);

	//--- NAME OF STATION
	//--- COUNTRY

	//--- GEOGRAPHICAL CO-ORDINATES
	res = SetCoord(10, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetCoord", res);
	//--- HEIGHT OF THE STATION SITE ABOVE SEA LEVEL
	res = SetStationHeight(11, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetStationHeight", res);
	//--------------------------------------
	if(vhcm.size()>13){
		//--- EQUIPMENT MANUFACTURER NAME
		if(!data.isPassive && doReport)
			AddReport("Equip. manufacture", un, vhcm[13], report);
		if(!vhcm[13].empty()){
			data.eqp_manuf_name = vhcm[13];
		}
	}
	// ToLogFile("hcm4afs_dll.log", "Equip. manufacture", data.eqp_manuf_name);
	if(vhcm.size()>14){
		//--- EQUIPMENT TYPE
		if(!data.isPassive && doReport){
			AddReport("Equipment type", un, vhcm[14], report);
		}
		if(!vhcm[14].empty()){
			data.eqp_type = vhcm[14];
		}
	}
	// ToLogFile("hcm4afs_dll.log", "data.eqp_type", data.eqp_type);
	if(vhcm.size()>15){
		//--- MAX. CAPACITY OF THE LINK
		if(!data.isPassive && doReport)
			AddReport("Maximum capacity", un, vhcm[15], report);
		if(!vhcm[15].empty())
			data.capacity = vhcm[15];
		else
			data.capacity = "";
	}
	// ToLogFile("hcm4afs_dll.log", "data.capacity", data.capacity);
	//--------------------------------------
	
	//--- TRANSMITTER SPECTRUM MASK OR RECEIVER SELECTIVITY MASK
	res = SetTransSpectrumMask(16, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetTransSpectrumMask", res);
	//--- EQUIPMENT CLASS
	res = SetEqupmentClass(28, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetEqupmentClass", res);

	//--- NFD2 UNUSED

	//--- CHANNEL SPACING
	res = SetChannelSpacing(30, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetChannelSpacing", res);
	//--- SET 7-th and 8-th MASK ELEMENTS (2.5 * CHANNEL_SPACING and 3.5 * CHANNEL_SPACING)
	res = Set78MaskElement(30, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "Set78MaskElement", res);
	//--- MAXIMUM PERMITTED TRANSMITTER POWER
	res = SetMaxPermTrPower(31, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetMaxPermTrPower", res);
	//--- ATPC
	res = SetAtrc(32, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetAtrc", res);
	//--- AZIMUTH
	res = SetAzimuth(33, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetAzimuth", res);
	//--- ELEVATION
	res = SetElevation(34, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetElevation", res);
	//--- POLARIZATION
	res = SetPolarization(35, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetPolarization", res);
	//--- RECEIVER NOISE POWER LEVEL
	res = SetRxPowerLevel(36, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetRxPowerLevel", res);
	//--- BRANCHES AND LINE LOSSES
	res = SetBranchAndLineLoss(37, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetBranchAndLineLoss", res);
	//--- HEIGHT OF ANTENNA ABOVE GROUND
	res = SetAntGroundHeight(38, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetAntGroundHeight", res);
	//--------------------------------------
	if(vhcm.size()>44){
		//--- ANTENNA MANUFACTURER NAME
		if(doReport) AddReport("Antenna manufacture", un, vhcm[44], report);
		if(!vhcm[44].empty())
			data.ant_manuf_name = vhcm[44];
		else
			data.ant_manuf_name = "";
	}
	// ToLogFile("hcm4afs_dll.log", "data.ant_manuf_name", data.ant_manuf_name);
	if(vhcm.size()>45){
		//--- ANTENNA TYPE
		if(doReport) AddReport("Antenna type", un, vhcm[45], report);
		if(!vhcm[45].empty())
			data.ant_type = vhcm[45];
		else
			data.ant_type = "";
	}
	// ToLogFile("hcm4afs_dll.log", "data.ant_type", data.ant_type);
	//--------------------------------------
	
	//--- ANTENNA GAIN
	res = SetAntGain(48, vhcm, un, data, doReport, report);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetAntGain", res);

	//--- ANTENNA COPOLAR RADIATION PATTERN
	if(vhcm.size() > 49){
		string vstr = vhcm[49];
		if(doReport) AddReport("Antenna cop. pattern", un, vstr, report);
		// make antenna pattern uppercase
		std::transform(vstr.begin(), vstr.end(), vstr.begin(), (int(*)(int))std::toupper);
		if(vstr == "CP" || vstr == "VV" || vstr == "HH")
			data.cp_pattern = vstr;
		else{
			// invalid copolar radiation pattern
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1219; // for PTx
				else
					return 1220; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1203; // for Tx
				else
					return 1209; // for Rx
			}
		}
	}else{
		// Put standard antenna CP data, if missing
		if(doReport){
			AddReport("Antenna cop. pattern", un, "  = CP", report);
			AddReport("Antenna cop. data", un, ":", report);
		}
		long err = SetCP(data, stdAnt, report);
		if(err == 0){
			if(doReport){
				AddReport("Antenna cro. pattern", un, "  = XP", report);
				AddReport("Antenna cro. data", un, ":", report);
			}
			return SetXP(data, stdAnt, report);
		}
		else return err; // ALL data is set
	}

	int nr = 0;
	//--- Number of  mask data
	res = SetNumOfMaskData(50, vhcm, un, data, doReport, report, nr);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetNumOfMaskData", res);
	//--- Pattern data
	res = SetPatternData(vhcm, un, data, doReport, report, nr);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetPatternData", res);
	//--- ANTENNA CROSSPOLAR RADIATION PATTERN
	res = SetAntRadPattern(stdAnt, vhcm, un, data, doReport, report, nr);
	if(res > 0){
		data.error = res;
		return res;
	}
	// ToLogFile("hcm4afs_dll.log", "SetAntRadPattern", res);
	// TypeToFile("") report
	return 0;
}
//!---------------------------------------------------------------------------------------------


int SetCP(HcmRec& data, TStdAnt& stdAnt, string& report){
	if(data.cpData.size() == 0){
		vector<AntPoint> patt;
		stdAnt.CpPattern(data.gain, patt);
		if(stdAnt.Error()==3){
			// standard antenna not found
			if(data.isPassive){
				// for passive case
				if(data.isTx)
					return 1086; // for PTx
				else
					return 1098; // for PRx
			}else{
				// for active case
				if(data.isTx)
					return 1016; // for Tx
				else
					return 1033; // for Rx
			}
		}
		data.cpData = patt;
		data.cp_pattern = "CP";
		// Create report of standard antenna data
		for(int i = 0 ; i < data.cpData.size() ; i++){
			char an[20]; char at[20];
			sprintf(an,"%8.3f",data.cpData[i].angle);
			sprintf(at,"%8.3f",data.cpData[i].attenuation);
			report = report + "                             =    " + an + "\t\t" + at + "\n";
		}
	}
	return 0;
}
//----------------------------------------------------------------------------------

int SetXP(HcmRec& data, TStdAnt& stdAnt, string& report){
	vector<AntPoint> patt;
	stdAnt.XpPattern(data.gain, patt);
	if(stdAnt.Error() ==3){
		// standard antenna not found
		if(data.isPassive){
			// for passive case
			if(data.isTx)
				return 1086; // for PTx
			else
				return 1098; // for PRx
		}else{
			// for active case
			if(data.isTx)
				return 1016; // for Tx
			else
				return 1033; // for Rx
		}
	}
	data.xpData = patt;
	data.xp_pattern = "XP";
	// Create report of standard antenna data
	for(int i = 0; i < data.xpData.size(); i++){
		char an[20]; char at[20];
		sprintf(an,"%8.3f",data.xpData[i].angle);
		sprintf(at,"%8.3f",data.xpData[i].attenuation);
		report = report + "                             =    " + an + "\t\t" + at + "\n";
	}
	return 0;
}
//!---------------------------------------------------------------------------------------------

int	HcmToTrigger(const string& hsmString, double& frequency_GHz, double& longitude, double& latitude, int& h_asl){
	// Read HCM string to vector
	int pr = 0;
	vector<string> vhcm;
	for(int i=0 ; i<= hsmString.length() ; i++){
		if(hsmString[i]==';'){
			vhcm.push_back(hsmString.substr(pr, i-pr));
			pr = i+1;
		}
	}
	if(pr < hsmString.length()+1) vhcm.push_back( hsmString.substr(pr,hsmString.length()-pr));
	if(vhcm.size() < 39) return 8;
	// frequency = vhcm[1] + vhcm[2];
	try{
		frequency_GHz = atof(vhcm[1].c_str());
	}
	catch(...){
		return 3;
	} // bad frequency
	if(vhcm[2] == "M" || vhcm[2] == "m")
		frequency_GHz = frequency_GHz / 1000.0;
	else
		if(vhcm[2] == "K" || vhcm[2] == "k")
			frequency_GHz = frequency_GHz / 1000000.0;
		else
			if(!(vhcm[2] == "G" || vhcm[2] == "g"))
				return 3; // bad frequency
	try{
		string slong = vhcm[10].substr(0, 8);
		string slat = vhcm[10].substr(8, 7);
		int err;
		longitude = ToCoordinate(slong, err);
		if(err)	return 2;
		latitude = ToCoordinate(slat, err);
		if(err) return 2;
	}
	catch(...){
		return 2;
	} // bad coordinates

	// Get Antenna height
	int iah;
	try{
		iah = atoi(vhcm[38].c_str());
	}catch(...){
		return 6;
	}
	// Get site height above sea level
	int ish;
	try{
		ish = atoi(vhcm[11].c_str());
	}catch(...){
		return 7;
	}
	h_asl = ish + iah;
	return 0;
}
//!---------------------------------------------------------------------------------------------

