
#include <math.h>
#include "common.h"
#include "hcmfs.h"

//! --------------------------------------------------------------------------------------------

string GetLibraryVersion(){
  string qsver = "ver. 1.0";
  return qsver;
}
//! --------------------------------------------------------------------------------------------

double CombineAngles(double angle1, double angle2){
	double tmp = cos(angle1/180 * C_PI) * cos (angle2/180 * C_PI);
	if(tmp > 1.0) tmp = 1.0;
	else
		if(tmp < -1.0) tmp = -1.0;
	return acos(tmp) * 180 / C_PI;
}
//! --------------------------------------------------------------------------------------------

double AntennaAtt(double angle, const vector<AntPoint>& pattern){
	if(pattern[pattern.size()-1].angle <= 180 && angle > 180) angle = 360 - angle;
	double gn = 0.0;
	bool isFound = false;
	for(int i = 0; i < pattern.size(); i++){
		if(pattern[i].angle > angle){
			isFound = true;
			if(i == 0){
				// if first point (0;0) is missing
				gn = angle / pattern[i].angle * pattern[i].attenuation;
			}
			else{
				gn = (angle - pattern[i-1].angle) /
					 (pattern[i].angle - pattern[i-1].angle) *
					 (pattern[i].attenuation - pattern[i-1].attenuation) +
					 pattern[i-1].attenuation;
			}
			break;
		}
	}
	if(!isFound){
		// this case occurs when (last angle of pattern) < angle < 360
		gn = (angle - pattern[pattern.size()-1].angle) /
			 (360.0 - pattern[pattern.size()-1].angle) *
			 (pattern[0].attenuation - pattern[pattern.size()-1].attenuation) +
			 pattern[pattern.size()-1].attenuation;
	}
	return gn;
}
//! --------------------------------------------------------------------------------------------

double GetElevation(double antH1, double Hasl1, double antH2, double Hasl2, double distance){
	return 180 / C_PI * atan(((Hasl2 + antH2) - (Hasl1 + antH1)) / (1000.0 * distance));
}
//! --------------------------------------------------------------------------------------------

string	ErrorMsg(int error){
	switch(error){
	case 999: return "HCM record is empty";
	case 1000: return "Tx record is not a Tx";
	case 1001: return "Rx record is not a Rx";
	case 1002: return "Tx frequency (1A) not present";
	case 1003: return "Tx frequency unit (1A1) not present";
	case 1004: return "Tx co-ordinates (4C) not present";
	case 1005: return "Tx height of site above sea level (4Z) not present";
	case 1006: return "Transmitter spectrum mask  (7G) (frequency) is not present";
	case 1007: return "Transmitter spectrum mask  (7G) (attenuation) is not present";
	case 1008: return "Tx power (8A) is not present";
	case 1010: return "Tx azimuth (9A) is not present";
	case 1011: return "Tx elevation (9B) is not present";
	case 1012: return "Tx polarization (9D) is not present";
	case 1014: return "Tx antenna height above ground (9Y) is not present";
	case 1015: return "Tx antenna gain (part of 9X) is not present";
	case 1016: return "Default Tx antenna radiation pattern (9X) is not present";
	case 1020: return "Rx frequency (1A) not present";
	case 1021: return "Rx frequency unit (1A1) not present";
	case 1022: return "Rx co-ordinates (4C) not present";
	case 1023: return "Rx height of site above sea level (4Z) not present";
	case 1024: return "Error in Tx channel spacing (7G3)";
	case 1026: return "Rx azimuth (9A) is not present";
	case 1027: return "Rx elevation (9B) is not present";
	case 1028: return "Rx polarization (9D) is not present";
	case 1029: return "Rx noise power level FKTB (9H) is not present";
	case 1031: return "Rx antenna height above ground (9Y) is not present";
	case 1032: return "Rx antenna gain (part of 9X) is not present";
	case 1033: return "Default Rx antenna radiation pattern (9X) is not present";
	case 1037: return "Error in Tx frequency (1A)";
	case 1038: return "Error in Tx frequency unit (1A1)";
	case 1039: return "Error in Tx frequency range (only 1 - 43.5 GHz are permissible)";
	case 1040: return "Error in Rx frequency (1A)";
	case 1041: return "Error in Rx frequency unit (1A1)";
	case 1042: return "Error in Rx frequency range (only 1 - 43.5 GHz are permissible)";
	case 1043: return "Error in Tx co-ordinates (4C)";
	case 1044: return "Error in Rx co-ordinates (4C)";
	case 1045: return "Error in Tx site height (4Z)";
	case 1046: return "Error in Rx site height (4Z)";
	case 1047: return "Error in Tx spectrum mask data (7G)";
	case 1048: return "Error in Rx selectivity mask data (7G)";
	case 1049: return "Error in Tx power (8A)";
	case 1050: return "Error in Tx ATPC (8B3)";
	case 1051: return "Error in Tx azimuth (9A)";
	case 1052: return "Error in Rx azimuth (9A)";
	case 1053: return "Error in Tx elevation (9B)";
	case 1054: return "Error in Rx elevation (9B)";
	case 1055: return "Error in (Rx) FkTB (9H)";
	case 1056: return "Error in Tx line losses (9L)";
	case 1057: return "Error in Rx line losses (9L)";
	case 1058: return "Error in Tx antenna height (9Y)";
	case 1059: return "Error in Rx antenna height (9Y)";
	case 1060: return "Error in Tx antenna gain (in 9X)";
	case 1061: return "Error in Rx antenna gain (in 9X)";
	case 1067: return "Antenna co-polar data for the respective polarisation is not available";
	case 1068: return "Antenna cross-polar data for the respective polarisation is not available";
	case 1070: return "Receiver selectivity mask  (7G) (frequency) is not present";
	case 1071: return "Receiver selectivity mask  (7G) (attenuation) is not present";
	case 1075: return "Rx equipment manufacturer name (7H) not present";
	case 1077: return "PTx record is not a PTx";
	case 1079: return "PTx co-ordinates (4C) not present";
	case 1080: return "PTx height of site above sea level (4Z) not present";
	case 1081: return "PTx azimuth (9A) is not present";
	case 1082: return "PTx elevation (9B) is not present";
	case 1083: return "PTx polarization (9D) is not present";
	case 1084: return "PTx antenna height above ground (9Y) is not present";
	case 1085: return "PTx antenna gain (part of 9X) is not present";
	case 1086: return "Default PTx antenna radiation pattern (9X) is not present";
	case 1090: return "PRx record is not a PRx";
	case 1091: return "PRx co-ordinates (4C) not present";
	case 1092: return "PRx height of site above sea level (4Z) not present";
	case 1093: return "PRx azimuth (9A) is not present";
	case 1094: return "PRx elevation (9B) is not present";
	case 1095: return "PRx polarization (9D) is not present";
	case 1096: return "PRx antenna height above ground (9Y) is not present";
	case 1097: return "PRx antenna gain (part of 9X) is not present";
	case 1098: return "Default PRx antenna radiation pattern (9X) is not present";
	case 1102: return "Error in PTx co-ordinates (4C)";
	case 1103: return "Error in PRx co-ordinates (4C)";
	case 1104: return "Error in PTx site height (4Z)";
	case 1105: return "Error in PRx site height (4Z)";
	case 1106: return "Error in PTx azimuth (9A)";
	case 1107: return "Error in PRx azimuth (9A)";
	case 1108: return "Error in PTx elevation (9B)";
	case 1109: return "Error in PRx elevation (9B)";
	case 1110: return "Error in PTx line losses (9L)";
	case 1111: return "Error in PRx line losses (9L)";
	case 1112: return "Error in PTx antenna height (9Y)";
	case 1113: return "Error in PRx antenna height (9Y)";
	case 1114: return "Error in PTx antenna gain (in 9X)";
	case 1115: return "Error in PRx antenna gain (in 9X)";
	case 1116: return "Tx position = PRx position; calculations not possible";
	case 1118: return "PTx position = Rx position; calculations not possible";
	case 1203: return "Invalid Tx copolar radiation pattern (part of 9X)";
	case 1204: return "Invalid number of Tx copolar radiation pattern data (part of 9X)";
	case 1205: return "Invalid or missing Tx copolar antenna data (part of 9X)";
	case 1206: return "Invalid or missing Tx crosspolar radiation pattern (part of 9X)";
	case 1207: return "Invalid number of Tx crosspolar radiation pattern data (part of 9X)";
	case 1208: return "Invalid or missing Tx crosspolar antenna data (part of 9X)";
	case 1209: return "Invalid Rx copolar radiation pattern (part of 9X)";
	case 1210: return "Invalid number of Rx copolar radiation pattern data (part of 9X)";
	case 1211: return "Invalid or missing Rx copolar antenna data (part of 9X)";
	case 1212: return "Invalid or missing Rx crosspolar radiation pattern (part of 9X)";
	case 1213: return "Invalid number of Rx crosspolar radiation pattern data (part of 9X)";
	case 1214: return "Invalid or missing Rx crosspolar antenna data (part of 9X)";
	case 1215: return "Invalid PTx polarization (9D)";
	case 1216: return "Invalid PRx polarization (9D)";
	case 1217: return "Invalid Tx polarization (9D)";
	case 1218: return "Invalid Rx polarization (9D)";
	case 1219: return "Invalid PTx copolar radiation pattern (part of 9X)";
	case 1220: return "Invalid PRx copolar radiation pattern (part of 9X)";
	case 1221: return "Invalid number of PTx copolar radiation pattern data (part of 9X)";
	case 1222: return "Invalid number of PRx copolar radiation pattern data (part of 9X)";
	case 1223: return "Invalid or missing PTx copolar antenna data (part of 9X)";
	case 1224: return "Invalid or missing PRx copolar antenna data (part of 9X)";
	case 1225: return "Invalid or missing PTx crosspolar radiation pattern (part of 9X)";
	case 1226: return "Invalid or missing PRx crosspolar radiation pattern (part of 9X)";
	case 1227: return "Invalid number of PTx crosspolar radiation pattern data (part of 9X)";
	case 1228: return "Invalid number of PRx crosspolar radiation pattern data (part of 9X)";
	case 1229: return "Invalid or missing PTx crosspolar antenna data (part of 9X)";
	case 1230: return "Invalid or missing PRx crosspolar antenna data (part of 9X)";
	case 1233: return "Tx spectrum mask has less than 2 pairs of frequency and attenuation";
	case 1234: return "Rx selectivity mask has less than 2 pairs of frequency and attenuation";
	case 1235: return "Last attenuation of Tx spectrum mask < 40 dB";
	case 1236: return "Last attenuation of Rx selectivity mask < 40 dB";
	case 1237: return "Error opening the result file";
	case 1238: return "Error writing data to the result file";
	case 1241: return "Invalid name of transmitter equipment class (7G1)";
	case 1242: return "Invalid name of receiver equipment class (7G1)";
	case 1243: return "No default transmitter mask data (7G) available";
	case 1244: return "No default receiver mask data (7G) available";
	case 1245: return "Could not find parameter file hcmrc";
	case 1246: return "Could not open registry key HKEY_LOCAL_MASHINE\\Software\\HCM";
	case 1247: return "Could not read topo path value from registry";
	case 1248: return "Error in Rx channel spacing (7G3)";
	}
	return "";
}
//! --------------------------------------------------------------------------------------------

// Error code processing for compatibility with the old HCM_FS version
void ProcessErrors(const string& code, int errIn, int& errOut, string& errMsg){
	switch(errIn){
	case 1: errOut = 36;	errMsg = "Error opening topo data file (no data)"; break;
	case 2: errOut = 220;	errMsg = "Error reading terrain record"; break;
	case 4: errOut = 0;		errMsg = "Morphological data not found, assume we have inland"; break;
	case 5: errOut = 400;	errMsg = "Height is missing (-9999)"; break;
	case 6:
		if(code == "terraintxrx"){
			errOut = 1062;
			errMsg = "Tx position = Rx position; calculations not possible";
			break;
		}
		if(code == "terraintxprx"){
			errOut = 1116;
			errMsg = "Tx position = PRx position; calculations not possible";
			break;
		}
		if(code == "terrainptxrx"){
			errOut = 1118;
			errMsg = "PTx position = Rx position; calculations not possible";
			break;
		}
	}
}
//! --------------------------------------------------------------------------------------------


// Errors:
//  0 - no error
//  1237 - Can not open file for writing
//  1238 - Error writing data
int WriteResults(const string& filename, string& reportTX, string& reportPRX, string& reportPTX, string& reportRX, DataCalc& dataCalc, bool passive){
	string report;
	if(filename.size() > 0){
		if (passive)
			report = report + reportTX + "\n" + reportPRX + "\n" + reportPTX + "\n" + reportRX;
		else
			report = reportTX + "\n" + reportRX;
	}else return 0;

	FILE* f = fopen(filename.c_str(), "w");
	if (f == NULL) return 1237;
	report = "\n"
			 "Version = " + GetLibraryVersion() + "\n"
			 "\n"
			 "Drive name or paths	        = " + dataCalc.dataPaths + "\n"
			 "Input values:\n"
			 +  report;

	string repRes;
	if(!passive)
		repRes = PrintResultA(dataCalc);
	else
		repRes = PrintResultP(dataCalc);

	report = report + repRes;
	try{
		size_t i = fwrite(report.c_str(),report.size(),1,f);
		if(i !=1){
			fclose(f);
			return 1238;
		}
	}
	catch(...){
		fclose(f);
		return 1238;
	}
	fclose(f);
	return 0;
}
//! --------------------------------------------------------------------------------------------

string PrintResultA(const DataCalc& dataCalc){
	string repRes =	"  \n"
					"Output values:\n";
	char ctmp[50];
	sprintf(ctmp, "%8i", dataCalc.error);
	repRes = repRes + "  Error value                = " + string(ctmp) + "\n";
	if(dataCalc.error ==0){
		sprintf(ctmp, "%8.3f", dataCalc.distance_tx_rx);
		repRes = repRes + "  Distance                   = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.azimuth_tx_rx);
		repRes = repRes + "  H.direction Tx->Rx         = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.azimuth_rx_tx);
		repRes = repRes + "  H.direction Rx->Tx         = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.elevation_tx_rx);
		repRes = repRes + "  V.direction Tx->Rx         = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.elevation_rx_tx);
		repRes = repRes + "  V.direction Rx->Tx         = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.diffTxRxAzimuth);
		repRes = repRes + "  H. diff.ang. Tx->Rx        = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.diffTxRxElevation);
		repRes = repRes + "  V. diff.ang. Tx->Rx        = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.diffRxTxAzimuth);
		repRes = repRes + "  H. diff.ang. Rx->Tx        = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.diffRxTxElevation);
		repRes = repRes + "  V. diff.ang. Rx->Tx        = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.a_ant_tx_rx);
		repRes = repRes + "  a_ant                      = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.diffFreqTxRx);
		repRes = repRes + "  Frequncy diff. (Tx-Rx)     = " + string(ctmp) + "\t\tMHz\n";

		sprintf(ctmp, "%8.3f", dataCalc.tx_area);
		repRes = repRes + "  Tx_Area                    = " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", dataCalc.overlap_area);
		repRes = repRes + "  O_Area                     = " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", dataCalc.overlap_area_co);
		repRes = repRes + "  O_Area at co-channel       = " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", dataCalc.md);
		repRes = repRes + "  Mask Discrimination MD     = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.nfd);
		repRes = repRes + "  NetFilterDiscrimin.NFD     = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8i", dataCalc.txHasl_terr);
		repRes = repRes + "  Database height of Tx      = " + string(ctmp) + "\t\tm\n";

		sprintf(ctmp, "%8i", dataCalc.rxHasl_terr);
		repRes = repRes + "  Database height of Rx      = " + string(ctmp) + "\t\tm\n";

		string ptype;
		if(dataCalc.p452_tx_rx.pathType == PathType::los)
			ptype = "line-of-sight";
		else
			ptype = "transhorizon";
		repRes = repRes + "  Path_type                  = " + ptype + "\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.Lbd);
		repRes = repRes + "  Lbd                        = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.Ldp);
		repRes = repRes + "  Ldp                        = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.Lbd50);
		repRes = repRes + "  Lbd50                      = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.Lb);
		repRes = repRes + "  Lb                         = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.Lba);
		repRes = repRes + "  Lba                        = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.Lbfsg);
		repRes = repRes + "  Lbfsg                      = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.Lb0p);
		repRes = repRes + "  Lb0p                       = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.Lb0beta);
		repRes = repRes + "  Lb0beta                    = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.Lbs);
		repRes = repRes + "  Lbs                        = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.theta);
		repRes = repRes + "  theta                      = " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.theta_t);
		repRes = repRes + "  theta_t                    = " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.theta_r);
		repRes = repRes + "  theta_r                    = " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.d_lt);
		repRes = repRes + "  d_lt                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.d_lr);
		repRes = repRes + "  d_lr                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.d_tm);
		repRes = repRes + "  d_tm                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.d_lm);
		repRes = repRes + "  d_lm                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.d_ct);
		repRes = repRes + "  d_ct                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.d_cr);
		repRes = repRes + "  d_cr                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.h_te);
		repRes = repRes + "  h_te                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.h_re);
		repRes = repRes + "  h_re                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.h_m);
		repRes = repRes + "  h_m                        = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.omega);
		repRes = repRes + "  omega                      = " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_rx.Ag);
		repRes = repRes + "  Ag                         = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.a_tot);
		repRes = repRes + "  a_tot                      = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.I);
		repRes = repRes + "  I                          = " + string(ctmp) + "\t\tdBW\n";

		sprintf(ctmp, "%8.3f", dataCalc.td);
		repRes = repRes + "  TD                         = " + string(ctmp) + "\t\tdB\n";

		repRes = repRes + "\n\n\n"
			"   Terrain profile (1st value = Tx site, last value = Rx site)\n"
			"   Distance between two points ~ 100m.\n\n";

		sprintf(ctmp, "%8i", dataCalc.profile_tx_rx.size());
		repRes = repRes + "  Number of points           = " + string(ctmp) + "\n\n";
		// print topo profile
		for(int i = 1 ; i <= dataCalc.profile_tx_rx.size() ; i++)		{
			sprintf(ctmp, "%5i", static_cast<int>(dataCalc.profile_tx_rx[i-1].height));
			repRes = repRes + " " + string(ctmp);
			if (i/10.0 -floor(i/10.0) == 0.0 && i!=0) repRes = repRes + "\n";
		}
		repRes = repRes + "\n\n\n"
			"   Morphological profile (1st value = Tx site, last value = Rx site)\n"
			"   0 = inland, 1 = sea, 4 = costal area\n\n";
		// print morpho profile
		for(int i = 1 ; i <= dataCalc.profile_tx_rx.size() ; i++){
			// TODO: dataCalc.profile_tx_rx[i-1].zone = enum class Zone {A2=0, B=1, A1=4};
			sprintf(ctmp, "%5i", ZoneToInt(dataCalc.profile_tx_rx[i-1].zone));
			repRes = repRes + " " + string(ctmp);
			if (i/10.0 -floor(i/10.0) == 0.0 && i!=0) repRes = repRes + "\n";
		}
		repRes = repRes + "\n\n"
						  "  -------------------------------------------------------------------"
						  "\n\n";
	}
	else
		repRes = repRes + "  Error message              = " + dataCalc.errorMsg + "\n";
	return repRes;
}

string PrintResultP(const DataCalc& dataCalc){
	string repRes =	"  \n"
					"Output values:\n";
	char ctmp[50];
	sprintf(ctmp, "%8i", dataCalc.error);
	repRes = repRes + "  Error value                = " + string(ctmp) + "\n";
	if(dataCalc.error ==0){
		// PASSIVE CASE
		sprintf(ctmp, "%8.3f", dataCalc.distance_tx_prx);
		repRes = repRes + "  Distance Tx - PRx          = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.distance_ptx_rx);
		repRes = repRes + "  Distance PTx - Rx          = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.azimuth_tx_prx);
		repRes = repRes + "  H.direction Tx->PRx        = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.azimuth_prx_tx);
		repRes = repRes + "  H.direction PRx->Tx        = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.elevation_tx_prx);
		repRes = repRes + "  V.direction Tx->PRx        = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.elevation_prx_tx);
		repRes = repRes + "  V.direction PRx->Tx        = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.diffTxPRxAzimuth);
		repRes = repRes + "  H. diff.ang. Tx->PRx       = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.diffTxPRxElevation);
		repRes = repRes + "  V. diff.ang. Tx->PRx       = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.diffPRxTxAzimuth);
		repRes = repRes + "  H. diff.ang. PRx->Tx       = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.diffPRxTxElevation);
		repRes = repRes + "  V. diff.ang. PRx->Tx       = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.a_ant_tx_prx);
		repRes = repRes + "  a_ant Tx - PRx             = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.azimuth_ptx_rx);
		repRes = repRes + "  H.direction PTx->Rx        = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.azimuth_rx_ptx);
		repRes = repRes + "  H.direction Rx->PTx        = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.elevation_ptx_rx);
		repRes = repRes + "  V.direction PTx->Rx        = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.elevation_rx_ptx);
		repRes = repRes + "  V.direction Rx->PTx        = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.diffPTxRxAzimuth);
		repRes = repRes + "  H. diff.ang. PTx->Rx       = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.diffPTxRxElevation);
		repRes = repRes + "  V. diff.ang. PTx->Rx       = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.diffRxPTxAzimuth);
		repRes = repRes + "  H. diff.ang. Rx->PTx       = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.diffRxPTxElevation);
		repRes = repRes + "  V. diff.ang. Rx->PTx       = " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", dataCalc.a_ant_ptx_rx);
		repRes = repRes + "  a_ant PTx - Rx             = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", fabs(dataCalc.diffFreqTxRx));
		repRes = repRes + "  Frequncy diff. (Tx-Rx)     = " + string(ctmp) + "\t\tMHz\n";

		sprintf(ctmp, "%8.3f", dataCalc.tx_area);
		repRes = repRes + "  Tx_Area                    = " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", dataCalc.overlap_area);
		repRes = repRes + "  O_Area                     = " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", dataCalc.overlap_area_co);
		repRes = repRes + "  O_Area at co-channel       = " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", dataCalc.md);
		repRes = repRes + "  Mask Discrimination MD     = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.nfd);
		repRes = repRes + "  NetFilterDiscrimin.NFD     = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8i", dataCalc.txHasl_terr);
		repRes = repRes + "  Database height of Tx      = " + string(ctmp) + "\t\tm\n";

		sprintf(ctmp, "%8i", dataCalc.rxHasl_terr);
		repRes = repRes + "  Database height of Rx      = " + string(ctmp) + "\t\tm\n";

		sprintf(ctmp, "%8i", dataCalc.prxHasl_terr);
		repRes = repRes + "  Database height of PRx     = " + string(ctmp) + "\t\tm\n";

		string ptype;
		if(dataCalc.p452_tx_prx.pathType == PathType::los)
			ptype = "line-of-sight";
		else
			ptype = "transhorizon";
		repRes = repRes + "  Path_type Tx - PRx         = " + ptype + "\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.Lbd);
		repRes = repRes + "  Lbd                        = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.Ldp);
		repRes = repRes + "  Ldp                        = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.Lbd50);
		repRes = repRes + "  Lbd50                      = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.Lb);
		repRes = repRes + "  Lb                         = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.Lba);
		repRes = repRes + "  Lba                        = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.Lbfsg);
		repRes = repRes + "  Lbfsg                      = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.Lb0p);
		repRes = repRes + "  Lb0p                       = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.Lb0beta);
		repRes = repRes + "  Lb0beta                    = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.Lbs);
		repRes = repRes + "  Lbs                        = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.theta);
		repRes = repRes + "  theta                      = " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.theta_t);
		repRes = repRes + "  theta_t                    = " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.theta_r);
		repRes = repRes + "  theta_r                    = " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.d_lt);
		repRes = repRes + "  d_lt                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.d_lr);
		repRes = repRes + "  d_lr                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.d_tm);
		repRes = repRes + "  d_tm                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.d_lm);
		repRes = repRes + "  d_lm                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.d_ct);
		repRes = repRes + "  d_ct                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.d_cr);
		repRes = repRes + "  d_cr                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.h_te);
		repRes = repRes + "  h_te                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.h_re);
		repRes = repRes + "  h_re                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.h_m);
		repRes = repRes + "  h_m                        = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.omega);
		repRes = repRes + "  omega                      = " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_tx_prx.Ag);
		repRes = repRes + "  Ag                         = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8i", dataCalc.ptxHasl_terr);
		repRes = repRes + "  Database height of PTx     = " + string(ctmp) + "\t\tm\n";

		if(dataCalc.p452_ptx_rx.pathType == PathType::los)
			ptype = "line-of-sight";
		else
			ptype = "transhorizon";
		repRes = repRes + "  Path_type PTx - Rx         = " + ptype + "\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.Lbd);
		repRes = repRes + "  Lbd                        = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.Ldp);
		repRes = repRes + "  Ldp                        = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.Lbd50);
		repRes = repRes + "  Lbd50                      = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.Lb);
		repRes = repRes + "  Lb                         = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.Lba);
		repRes = repRes + "  Lba                        = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.Lbfsg);
		repRes = repRes + "  Lbfsg                      = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.Lb0p);
		repRes = repRes + "  Lb0p                       = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.Lb0beta);
		repRes = repRes + "  Lb0beta                    = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.Lbs);
		repRes = repRes + "  Lbs                        = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.theta);
		repRes = repRes + "  theta                      = " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.theta_t);
		repRes = repRes + "  theta_t                    = " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.theta_r);
		repRes = repRes + "  theta_r                    = " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.d_lt);
		repRes = repRes + "  d_lt                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.d_lr);
		repRes = repRes + "  d_lr                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.d_tm);
		repRes = repRes + "  d_tm                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.d_lm);
		repRes = repRes + "  d_lm                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.d_ct);
		repRes = repRes + "  d_ct                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.d_cr);
		repRes = repRes + "  d_cr                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.h_te);
		repRes = repRes + "  h_te                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.h_re);
		repRes = repRes + "  h_re                       = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.h_m);
		repRes = repRes + "  h_m                        = " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.omega);
		repRes = repRes + "  omega                      = " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", dataCalc.p452_ptx_rx.Ag);
		repRes = repRes + "  Ag                         = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.a_tot);
		repRes = repRes + "  a_tot                      = " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", dataCalc.I);
		repRes = repRes + "  I                          = " + string(ctmp) + "\t\tdBW\n";

		sprintf(ctmp, "%8.3f", dataCalc.td);
		repRes = repRes + "  TD                         = " + string(ctmp) + "\t\tdB\n";

		repRes = repRes + "\n\n\n"
				"   Terrain profile (1st value = Tx site, last value = PRx site)\n"
				"   Distance between two points ~ 100m \n\n";

		sprintf(ctmp, "%8i", dataCalc.profile_tx_prx.size());
		repRes = repRes + "  Number of points           = " + string(ctmp) + "\n\n";

		// print topo profile
		for(int i = 1; i <= dataCalc.profile_tx_prx.size(); i++){
			sprintf(ctmp, "%5i", static_cast<int>(dataCalc.profile_tx_prx[i-1].height));
			repRes = repRes + " " + string(ctmp);
			if(i/10.0 -floor(i/10.0) == 0.0 && i!=0) repRes = repRes + "\n";
		}
		repRes = repRes + "\n\n\n"
				"   Morphological profile (1st value = Tx site, last value = PRx site)\n"
				"   0 = inland, 1 = sea, 4 = costal area\n\n";

		// print morpho profile
		for(int i = 1; i <= dataCalc.profile_tx_prx.size(); i++){
			// TODO: dataCalc.profile_tx_prx[i-1].zone = enum class Zone {A2=0, B=1, A1=4};
			sprintf(ctmp, "%5i", ZoneToInt(dataCalc.profile_tx_prx[i-1].zone));
			repRes = repRes + " " + string(ctmp);
			if(i/10.0 -floor(i/10.0) == 0.0 && i!=0) repRes = repRes + "\n";
		}
		repRes = repRes + "\n\n\n"
				"   Terrain profile (1st value = PTx site, last value = Rx site)\n"
				"   Distance between two points ~ 100m \n\n";

		sprintf(ctmp, "%8i", dataCalc.profile_ptx_rx.size());
		repRes = repRes + "  Number of points           = " + string(ctmp) + "\n\n";

		// print topo profile
		for(int i = 1; i <= dataCalc.profile_ptx_rx.size(); i++){
			sprintf(ctmp, "%5i", static_cast<int>(dataCalc.profile_ptx_rx[i-1].height));
			repRes = repRes + " " + string(ctmp);
			if(i/10.0 -floor(i/10.0) == 0.0 && i!=0) repRes = repRes + "\n";
		}
		repRes = repRes + "\n\n\n"
				"   Morphological profile (1st value = PTx site, last value = Rx site)\n"
				"   0 = inland, 1 = sea, 4 = costal area\n\n";

		// print morpho profile
		for(int i = 1; i <= dataCalc.profile_ptx_rx.size(); i++){
			// TODO: dataCalc.profile_ptx_rx[i-1].zone = enum class Zone {A2=0, B=1, A1=4};
			sprintf(ctmp, "%5i", ZoneToInt(dataCalc.profile_ptx_rx[i-1].zone));
			repRes = repRes + " " + string(ctmp);
			if(i/10.0 -floor(i/10.0) == 0.0 && i!=0) repRes = repRes + "\n";
		}
		repRes = repRes + "\n\n"
			  "-------------------------------------------------------------------"
						  "\n\n";
	}
	else
		repRes = repRes + "  Error message              = " + dataCalc.errorMsg + "\n";
	return repRes;
}


int CalcNfd(HcmRec& tx, HcmRec& rx, double& nfd, double& md, double& overlap_area, double& overlap_area_co, double& tx_area){
	nfd = 0.0;
	TNfd nf;
	// Set TX mask
	if(tx.mask.size() > 0)
		nf.SetTxMask(tx.mask);
	else
		nf.SetTxMask(tx.frequency, tx.eqclass, tx.channel_spacing);
	switch(nf.error){
		case 2:
			return 1241;
		case 3:
			return 1243;
		default:;
	}
	// Set RX mask
	if(rx.mask.size() > 0)
		nf.SetRxMask(rx.mask);
	else
		nf.SetRxMask(rx.frequency, rx.eqclass, rx.channel_spacing);
	switch(nf.error){
		case 2:
			return 1241;
		case 3:
			return 1244;
		default:;
	}
	double dF = fabs(((long)(tx.frequency * 1000000) - (long)(rx.frequency * 1000000))/1000.0);
	nfd = nf.NFD(dF, overlap_area, overlap_area_co);
	md = nf.MD(tx_area);
	return 0;
}
//! --------------------------------------------------------------------------------------------

void CalcA_ant(const HcmRec& tx, const HcmRec& rx, DataCalc& dataCalc, double& txAntatt, double& rxAntatt){
	double tda_t = 0, tda_r = 0;
	//Test if antenna patterns are correct
	if(!(tx.cp_pattern == (tx.polarization + tx.polarization) || tx.cp_pattern == "CP")){
		// Antenna copolar data for the respective polarisation is not available
		dataCalc.error = 1067;
		return;
	}
	if(!((tx.polarization == "H" && tx.xp_pattern == "HV") ||
		 (tx.polarization == "V" && tx.xp_pattern == "VH") || tx.xp_pattern == "XP")){
		// Antenna crosspolar data for the respective polarisation is not available
		dataCalc.error = 1068;
		return;
	}
	if(!(rx.cp_pattern == (rx.polarization + rx.polarization) || rx.cp_pattern == "CP")){
		// Antenna copolar data for the respective polarisation is not available
		dataCalc.error = 1067;
		return;
	}
	if(!((rx.polarization == "H" && rx.xp_pattern == "HV") ||
		 (rx.polarization == "V" && rx.xp_pattern == "VH") || rx.xp_pattern == "XP")){
		// Antenna crosspolar data for the respective polarisation is not available
		dataCalc.error = 1068;
		return;
	}
	if(tx.isPassive){
		// Link PTx -> Rx
		dataCalc.diffPTxRxAzimuth = dataCalc.azimuth_ptx_rx - tx.azimuth;
		if(dataCalc.diffPTxRxAzimuth < 0) dataCalc.diffPTxRxAzimuth = 360.0 + dataCalc.diffPTxRxAzimuth;
		dataCalc.diffPTxRxElevation = tx.elevation - dataCalc.elevation_ptx_rx;
		if(dataCalc.diffPTxRxElevation < 0) dataCalc.diffPTxRxElevation = 360 + dataCalc.diffPTxRxElevation;
		dataCalc.diffRxPTxAzimuth = dataCalc.azimuth_rx_ptx - rx.azimuth;
		if(dataCalc.diffRxPTxAzimuth < 0) dataCalc.diffRxPTxAzimuth = 360.0 + dataCalc.diffRxPTxAzimuth;
		dataCalc.diffRxPTxElevation = rx.elevation - dataCalc.elevation_rx_ptx;
		if(dataCalc.diffRxPTxElevation < 0) dataCalc.diffRxPTxElevation = 360 + dataCalc.diffRxPTxElevation;
		tda_t = CombineAngles(dataCalc.diffPTxRxAzimuth, dataCalc.diffPTxRxElevation);
		tda_r = CombineAngles(dataCalc.diffRxPTxAzimuth, dataCalc.diffRxPTxElevation);
	}
	else if(rx.isPassive){
		// Link Tx -> PRx
		dataCalc.diffTxPRxAzimuth = dataCalc.azimuth_tx_prx - tx.azimuth;
		if(dataCalc.diffTxPRxAzimuth < 0) dataCalc.diffTxPRxAzimuth = 360.0 + dataCalc.diffTxPRxAzimuth;
		dataCalc.diffTxPRxElevation = tx.elevation - dataCalc.elevation_tx_prx;
		if(dataCalc.diffTxPRxElevation < 0) dataCalc.diffTxPRxElevation = 360 + dataCalc.diffTxPRxElevation;
		dataCalc.diffPRxTxAzimuth = dataCalc.azimuth_prx_tx - rx.azimuth;
		if(dataCalc.diffPRxTxAzimuth < 0) dataCalc.diffPRxTxAzimuth = 360.0 + dataCalc.diffPRxTxAzimuth;
		dataCalc.diffPRxTxElevation = rx.elevation - dataCalc.elevation_prx_tx;
		if(dataCalc.diffPRxTxElevation < 0) dataCalc.diffPRxTxElevation = 360 + dataCalc.diffPRxTxElevation;
		tda_t = CombineAngles(dataCalc.diffTxPRxAzimuth, dataCalc.diffTxPRxElevation);
		tda_r = CombineAngles(dataCalc.diffPRxTxAzimuth, dataCalc.diffPRxTxElevation);
	}
	else{
		// Link Tx -> Rx
		// Calculate the horizontal difference angle between
		// Tx antenna azimuth and azimuth to receiver (= dataCalc.diffTxRxAzimuth)
		dataCalc.diffTxRxAzimuth = dataCalc.azimuth_tx_rx - tx.azimuth;
		if(dataCalc.diffTxRxAzimuth < 0) dataCalc.diffTxRxAzimuth = 360.0 + dataCalc.diffTxRxAzimuth;
		// Calculate the vertical difference angle between
		// Tx antenna elevation and elevation to receiver (=dataCalc.diffTxRxElevation)
		dataCalc.diffTxRxElevation = tx.elevation - dataCalc.elevation_tx_rx;
		if(dataCalc.diffTxRxElevation < 0) dataCalc.diffTxRxElevation = 360 + dataCalc.diffTxRxElevation;
		// Calculate the horizontal difference angle between
		// Rx antenna azimuth and azimuth to transmitter (= dataCalc.diffRxTxAzimuth)
		dataCalc.diffRxTxAzimuth = dataCalc.azimuth_rx_tx - rx.azimuth;
		if(dataCalc.diffRxTxAzimuth < 0) dataCalc.diffRxTxAzimuth = 360.0 + dataCalc.diffRxTxAzimuth;
		// Calculate the vertical difference angle between
		// Rx antenna elevation and elevation to transmitter (=dataCalc.diffRxTxElevation)
		dataCalc.diffRxTxElevation = rx.elevation - dataCalc.elevation_rx_tx;
		if(dataCalc.diffRxTxElevation < 0) dataCalc.diffRxTxElevation = 360 + dataCalc.diffRxTxElevation;
		tda_t = CombineAngles(dataCalc.diffTxRxAzimuth, dataCalc.diffTxRxElevation);
		tda_r = CombineAngles(dataCalc.diffRxTxAzimuth, dataCalc.diffRxTxElevation);
	}
	// Antenna attenuaion for signals of V and H polarizations
	double a_Tc = AntennaAtt(tda_t, tx.cpData);
		double a_Tx = AntennaAtt(tda_t, tx.xpData);
	txAntatt = a_Tc;
	double a_Rc = AntennaAtt(tda_r, rx.cpData);
		double a_Rx = AntennaAtt(tda_r, rx.xpData);
	rxAntatt = a_Rc;

	double co, xo;
	// TX polarisation H
	if(tx.polarization == "h" || tx.polarization == "H"){
		// RX polarisation H
		if(rx.polarization == "h" || rx.polarization == "H"){
			co = a_Tc + a_Rc;
			xo = a_Tx + a_Rx;
		}
		// RX polarisation V
		else{
			co = a_Tc + a_Rx;
			xo = a_Tx + a_Rc;
		}
	}
	// TX polarisation V
	else{
		// RX polarisation H
		if(rx.polarization == "h" || rx.polarization == "H"){
			co = a_Tx + a_Rc;
			xo = a_Tc + a_Rx;
		}
		// RX polarisation V
		else{
			co = a_Tx + a_Rx;
			xo = a_Tc + a_Rc;
		}
	}
	// Total antenna attenuation
	if(tx.isPassive)
		dataCalc.a_ant_ptx_rx = co - 20 * log10( 1 + pow(10,(co-xo)/20) );
	else if (rx.isPassive)
		dataCalc.a_ant_tx_prx = co - 20 * log10( 1 + pow(10,(co-xo)/20) );
	else
		dataCalc.a_ant_tx_rx = co - 20 * log10( 1 + pow(10,(co-xo)/20) );
}
//! --------------------------------------------------------------------------------------------



//---------------------------------------------------------------------------------------------------
// The basic function for interference calculation according to HCM-Agreement
// This function is internal and is not exported to library (*.dll or *.so)
//  Tx = transmitter
//  Rx = receiver
void InterferencePP(const MapDnN0& maps, Terrain& terr, HcmRec& tx, HcmRec& rx,
					const InCalcRec& inRec, DataCalc& dataCalc){
	// Calculate NFD and overlapping area;
	dataCalc.error = CalcNfd(tx, rx, dataCalc.nfd, dataCalc.md, dataCalc.overlap_area, dataCalc.overlap_area_co, dataCalc.tx_area);
	if(dataCalc.error != 0){
		dataCalc.errorMsg = ErrorMsg(dataCalc.error);
		return;
	}
	if(dataCalc.overlap_area == 0.0){
		dataCalc.error = 0;
		dataCalc.td = 0;
		dataCalc.I = NO_DATA;
		return;
	}
	dataCalc.diffFreqTxRx = fabs(tx.frequency - rx.frequency) *1000; // MHz

	// NO PASSIVE REPEATER
	// Calculate distance and azimuth
	// Create profile
	terr.Initialise(tx.longitude, tx.latitude, rx.longitude, rx.latitude);
	if(terr.Error() !=0){
		ProcessErrors("terraintxrx", terr.Error(), dataCalc.error, dataCalc.errorMsg);
		if(dataCalc.error != 0) return;
	}
	dataCalc.distance_tx_rx = terr.Distance();	// Distance
	dataCalc.azimuth_tx_rx= terr.TxAzimuth();		// Tx azimuth to victim receiver
	dataCalc.azimuth_rx_tx= terr.RxAzimuth();		// Rx azimuth to interferer
	dataCalc.profile_tx_rx = *terr.Profile();
	dataCalc.txHasl_terr = dataCalc.profile_tx_rx[0].height;	// Database height of Tx
	dataCalc.rxHasl_terr = dataCalc.profile_tx_rx[dataCalc.profile_tx_rx.size()-1].height; // Database height of Rx

	dataCalc.profile_tx_rx[0].height = tx.h_asl;
	dataCalc.profile_tx_rx[dataCalc.profile_tx_rx.size()-1].height = rx.h_asl;

	//---------------------------------------//
	//-Calculate the transmission loss a_tot-//
	//---------------------------------------//

	TProp452_18 propTxRx(maps, dataCalc.profile_tx_rx);
	propTxRx.Init(tx.frequency, tx.longitude, tx.latitude, rx.longitude, rx.latitude, tx.h_ant, rx.h_ant, inRec.dN, inRec.n0);
	// Calculate vertical angles between Tx and Rx
	dataCalc.elevation_tx_rx = propTxRx.TxElevation()*0.18 / C_PI; // Convert from mrad to deg
	dataCalc.elevation_rx_tx = propTxRx.RxElevation()*0.18 / C_PI; // Convert from mrad to deg
	// Calculate the antenna attenuation a_ant
	double txAntatt, rxAntatt;
	CalcA_ant(tx, rx, dataCalc, txAntatt, rxAntatt);
	if(dataCalc.error !=0){
		dataCalc.errorMsg = ErrorMsg(dataCalc.error);
		return;
	}
	
	In452 inTxRx(tx.frequency, inRec.time_percentage, tx.h_ant, rx.h_ant,
				tx.longitude, tx.latitude, rx.longitude, rx.latitude,
				tx.gain - txAntatt, rx.gain - rxAntatt, IntToPlz(inRec.plz), inRec.press, inRec.temp, inRec.dN, inRec.n0);

	//*** Calculate propagation loss a_prop for Tx->Rx
	dataCalc.p452_tx_rx = propTxRx.CalcLoss(inTxRx);

	// Transmission loss a_tot
	dataCalc.a_tot = dataCalc.md + dataCalc.nfd + tx.atpc + dataCalc.a_ant_tx_rx + tx.losses + rx.losses + dataCalc.p452_tx_rx.Lb - tx.gain - rx.gain;
	// END OF CALCULATIONS
	
	// Interference
	dataCalc.I = tx.power - dataCalc.a_tot;
	// Threshold degradation
	dataCalc.td = 10 * log10(1 + pow(10, (dataCalc.I - rx.noise)/10));
	// END
}
//--------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
// The basic function for interference calculation according to HCM-Agreement
// This function is internal and is not exported to library (*.dll or *.so)
//  Tx = transmitter
//  Rx = receiver
//  Ptx = passive transmitter
//  Prx = passive receiver
void InterferencePP(const MapDnN0& maps, Terrain& terr, HcmRec& tx, HcmRec& prx, HcmRec& ptx, HcmRec& rx,
					const InCalcRec& inRec, DataCalc& dataCalc){
	// Calculate NFD and overlapping area;
	dataCalc.error = CalcNfd(tx, rx, dataCalc.nfd, dataCalc.md, dataCalc.overlap_area, dataCalc.overlap_area_co, dataCalc.tx_area);
	if(dataCalc.error != 0){
		dataCalc.errorMsg = ErrorMsg(dataCalc.error);
		return;
	}
	if(dataCalc.overlap_area == 0.0){
		dataCalc.error = 0;
		dataCalc.td = 0;
		dataCalc.I = NO_DATA;
		return;
	}
	dataCalc.diffFreqTxRx = fabs(tx.frequency - rx.frequency) *1000; // MHz
	// PASSIVE REPEATER EXISTS
	//----------------
	// PATH TX ->PRX  |
	//----------------
	// Calculate distances and azimuths
	// Create profile Tx -> PRX
	terr.Initialise(tx.longitude, tx.latitude, prx.longitude, prx.latitude);

	if(terr.Error() !=0){
		ProcessErrors("terraintxprx", terr.Error(), dataCalc.error, dataCalc.errorMsg);
		if(dataCalc.error != 0) return;
	}
	dataCalc.distance_tx_prx = terr.Distance();		// Distance from TX to PRX
	dataCalc.azimuth_tx_prx= terr.TxAzimuth();		// Tx azimuth to victim receiver
	dataCalc.azimuth_prx_tx= terr.RxAzimuth();		// Rx azimuth to interferer
	dataCalc.profile_tx_prx = *terr.Profile();
	dataCalc.txHasl_terr = dataCalc.profile_tx_prx[0].height;	// Database height of Tx
	dataCalc.prxHasl_terr = dataCalc.profile_tx_prx[dataCalc.profile_tx_prx.size()-1].height; // Database height of PRx
	dataCalc.profile_tx_prx[0].height = tx.h_asl;
	dataCalc.profile_tx_prx[dataCalc.profile_tx_prx.size()-1].height = prx.h_asl;

	TProp452_18 propTxPrx(maps, dataCalc.profile_tx_prx);
	propTxPrx.Init(tx.frequency, tx.longitude, tx.latitude, prx.longitude, prx.latitude, tx.h_ant, prx.h_ant, inRec.dN, inRec.n0);
	// Calculate vertical angles between Tx and Rx
	dataCalc.elevation_tx_prx = propTxPrx.TxElevation()*0.18 / C_PI; // Convert from mrad to deg
	dataCalc.elevation_prx_tx = propTxPrx.RxElevation()*0.18 / C_PI; // Convert from mrad to deg
	// Calculate the antenna attenuation a_ant for Tx->PRx
	double txAntatt, prxAntatt;
	CalcA_ant(tx, prx, dataCalc, txAntatt, prxAntatt);
	if(dataCalc.error !=0) return;
	//prop->Loss(20, tx->gain - txAntatt, prx->gain - prxAntatt, tx->latitude, rx->latitude, dataCalc.p452_tx_prx);
	In452 inTxPrx(tx.frequency, inRec.time_percentage, tx.h_ant, prx.h_ant,
				tx.longitude, tx.latitude, prx.longitude, prx.latitude,
				tx.gain - txAntatt, prx.gain - prxAntatt, IntToPlz(inRec.plz), inRec.press, inRec.temp, inRec.dN, inRec.n0);

	//*** Calculate propagation loss a_prop for Tx->PRx
	dataCalc.p452_tx_prx = propTxPrx.CalcLoss(inTxPrx);

	//----------------
	// PATH PTX ->RX  |
	//----------------
	// Calculate distances and azimuths
	// Create profile PTX -> RX
	terr.Initialise(ptx.longitude, ptx.latitude, rx.longitude, rx.latitude);
	if(terr.Error() !=0){
		ProcessErrors("terrainptxrx",terr.Error(),dataCalc.error,dataCalc.errorMsg);
		if (dataCalc.error != 0) return;
	}
	dataCalc.distance_ptx_rx = terr.Distance();		// Distance from PTX to RX
	dataCalc.azimuth_ptx_rx= terr.TxAzimuth();		// PTx azimuth to Rx
	dataCalc.azimuth_rx_ptx= terr.RxAzimuth();		// Rx azimuth to PTx
	dataCalc.profile_ptx_rx = *terr.Profile();
	dataCalc.ptxHasl_terr = dataCalc.profile_ptx_rx[0].height;	// Database height of PTx
	dataCalc.rxHasl_terr = dataCalc.profile_ptx_rx[dataCalc.profile_ptx_rx.size()-1].height; // Database height of Rx
	dataCalc.profile_ptx_rx[0].height = ptx.h_asl;
	dataCalc.profile_ptx_rx[dataCalc.profile_ptx_rx.size()-1].height = rx.h_asl;

	TProp452_18 propPtxRx(maps, dataCalc.profile_ptx_rx);
	propPtxRx.Init(tx.frequency, ptx.longitude, ptx.latitude, rx.longitude, rx.latitude, ptx.h_ant, rx.h_ant, inRec.dN, inRec.n0);
	// Calculate vertical angles between PTx and Rx
	dataCalc.elevation_ptx_rx = propPtxRx.TxElevation()*0.18 / C_PI; // Convert from mrad to deg
	dataCalc.elevation_rx_ptx = propPtxRx.RxElevation()*0.18 / C_PI; // Convert from mrad to deg
	// Calculate the antenna attenuation a_ant for PTx->Rx
	double ptxAntatt, rxAntatt;
	CalcA_ant(ptx, rx, dataCalc, ptxAntatt, rxAntatt);
	if(dataCalc.error !=0) return;
	In452 inPtxRx(tx.frequency, inRec.time_percentage, ptx.h_ant, rx.h_ant,
				ptx.longitude, ptx.latitude, rx.longitude, rx.latitude,
				ptx.gain - ptxAntatt, rx.gain - rxAntatt, IntToPlz(inRec.plz), inRec.press, inRec.temp, inRec.dN, inRec.n0);

	//*** Calculate propagation loss a_prop for Ptx->Rx
	dataCalc.p452_ptx_rx = propPtxRx.CalcLoss(inPtxRx);

	// Transmission loss a_tot
	dataCalc.a_tot = tx.losses - tx.gain + dataCalc.p452_tx_prx.Lb - prx.gain +
				   dataCalc.a_ant_tx_prx + prx.losses + ptx.losses - ptx.gain +
				   dataCalc.p452_ptx_rx.Lb - rx.gain + dataCalc.a_ant_ptx_rx +
				   rx.losses + dataCalc.md + dataCalc.nfd + tx.atpc;

	// END OF CALCULATIONS FOR PASSIVE

	// Interference
	dataCalc.I = tx.power - dataCalc.a_tot;
	// Threshold degradation
	dataCalc.td = 10 * log10 ( 1 + pow(10,(dataCalc.I - rx.noise)/10) );
	// END
}
//--------------------------------------------------------------------------------

void InterferencePP(const MapDnN0& maps, HcmRec& tx, HcmRec& rx,
					const InCalcRec& inRec, DataCalc &dataCalc){
	// NO PASSIVE REPEATER
	// Calculate NFD and overlapping area;
	dataCalc.error = CalcNfd(tx, rx, dataCalc.nfd, dataCalc.md, dataCalc.overlap_area, dataCalc.overlap_area_co, dataCalc.tx_area);
	if(dataCalc.error != 0){
		dataCalc.errorMsg = ErrorMsg(dataCalc.error);
		return;
	}
	if(dataCalc.overlap_area == 0.0){
		dataCalc.error = 0;
		dataCalc.td = 0;
		dataCalc.I = NO_DATA;
		return;
	}
	dataCalc.diffFreqTxRx = fabs(tx.frequency - rx.frequency) *1000; // MHz
	dataCalc.txHasl_terr = dataCalc.profile_tx_rx[0].height;	// Database height of Tx
	dataCalc.rxHasl_terr = dataCalc.profile_tx_rx[dataCalc.profile_tx_rx.size()-1].height; // Database height of Rx
	dataCalc.profile_tx_rx[0].height = tx.h_asl;
	dataCalc.profile_tx_rx[dataCalc.profile_tx_rx.size()-1].height = rx.h_asl;

	//---------------------------------------//
	//-Calculate the transmission loss a_tot-//
	//---------------------------------------//
	TProp452_18 propTxRx(maps, dataCalc.profile_tx_rx);
	propTxRx.Init(tx.frequency, tx.longitude, tx.latitude, rx.longitude, rx.latitude, tx.h_ant, rx.h_ant, inRec.dN, inRec.n0);
	// Calculate vertical angles between Tx and Rx
	dataCalc.elevation_tx_rx = propTxRx.TxElevation()*0.18 / C_PI; // Convert from mrad to deg
	dataCalc.elevation_rx_tx = propTxRx.RxElevation()*0.18 / C_PI; // Convert from mrad to deg
	// Calculate the antenna attenuation a_ant
	double txAntatt, rxAntatt;
	CalcA_ant(tx, rx, dataCalc, txAntatt, rxAntatt);
	if(dataCalc.error !=0){
		dataCalc.errorMsg = ErrorMsg(dataCalc.error);
		return;
	}
	
	In452 inTxRx(tx.frequency, inRec.time_percentage, tx.h_ant, rx.h_ant,
				tx.longitude, tx.latitude, rx.longitude, rx.latitude,
				tx.gain - txAntatt, rx.gain - rxAntatt, IntToPlz(inRec.plz), inRec.press, inRec.temp, inRec.dN, inRec.n0);

	//*** Calculate propagation loss a_prop for Tx->Rx
	dataCalc.p452_tx_rx = propTxRx.CalcLoss(inTxRx);

	// Transmission loss a_tot
	dataCalc.a_tot = dataCalc.md + dataCalc.nfd + tx.atpc + dataCalc.a_ant_tx_rx + tx.losses + rx.losses + dataCalc.p452_tx_rx.Lb - tx.gain - rx.gain;
	// END OF CALCULATIONS
	
	// Interference
	dataCalc.I = tx.power - dataCalc.a_tot;
	// Threshold degradation
	dataCalc.td = 10 * log10(1 + pow(10, (dataCalc.I - rx.noise)/10));
	// END
}
void InterferencePP(const MapDnN0& maps, HcmRec& tx, HcmRec& prx, HcmRec& ptx, HcmRec& rx,
					const InCalcRec& inRec, DataCalc &dataCalc){
	// PASSIVE REPEATER EXISTS
	// Calculate NFD and overlapping area;
	dataCalc.error = CalcNfd(tx, rx, dataCalc.nfd, dataCalc.md, dataCalc.overlap_area, dataCalc.overlap_area_co, dataCalc.tx_area);
	if(dataCalc.error != 0){
		dataCalc.errorMsg = ErrorMsg(dataCalc.error);
		return;
	}
	if(dataCalc.overlap_area == 0.0){
		dataCalc.error = 0;
		dataCalc.td = 0;
		dataCalc.I = NO_DATA;
		return;
	}
	dataCalc.diffFreqTxRx = fabs(tx.frequency - rx.frequency) *1000; // MHz
	//----------------
	// PATH TX ->PRX  |
	//----------------
	dataCalc.txHasl_terr = dataCalc.profile_tx_prx[0].height;	// Database height of Tx
	dataCalc.prxHasl_terr = dataCalc.profile_tx_prx[dataCalc.profile_tx_prx.size()-1].height; // Database height of PRx
	dataCalc.profile_tx_prx[0].height = tx.h_asl;
	dataCalc.profile_tx_prx[dataCalc.profile_tx_prx.size()-1].height = prx.h_asl;

	TProp452_18 propTxPrx(maps, dataCalc.profile_tx_prx);
	propTxPrx.Init(tx.frequency, tx.longitude, tx.latitude, prx.longitude, prx.latitude, tx.h_ant, prx.h_ant, inRec.dN, inRec.n0);
	// Calculate vertical angles between Tx and Rx
	dataCalc.elevation_tx_prx = propTxPrx.TxElevation()*0.18 / C_PI; // Convert from mrad to deg
	dataCalc.elevation_prx_tx = propTxPrx.RxElevation()*0.18 / C_PI; // Convert from mrad to deg
	// Calculate the antenna attenuation a_ant for Tx->PRx
	double txAntatt, prxAntatt;
	CalcA_ant(tx, prx, dataCalc, txAntatt, prxAntatt);
	if(dataCalc.error !=0) return;
	//prop->Loss(20, tx->gain - txAntatt, prx->gain - prxAntatt, tx->latitude, rx->latitude, dataCalc.p452_tx_prx);
	In452 inTxPrx(tx.frequency, inRec.time_percentage, tx.h_ant, prx.h_ant,
				tx.longitude, tx.latitude, prx.longitude, prx.latitude,
				tx.gain - txAntatt, prx.gain - prxAntatt, IntToPlz(inRec.plz), inRec.press, inRec.temp, inRec.dN, inRec.n0);

	//*** Calculate propagation loss a_prop for Tx->PRx
	dataCalc.p452_tx_prx = propTxPrx.CalcLoss(inTxPrx);

	//----------------
	// PATH PTX ->RX  |
	//----------------
	// Calculate distances and azimuths
	// Create profile PTX -> RX
	dataCalc.ptxHasl_terr = dataCalc.profile_ptx_rx[0].height;	// Database height of PTx
	dataCalc.rxHasl_terr = dataCalc.profile_ptx_rx[dataCalc.profile_ptx_rx.size()-1].height; // Database height of Rx
	dataCalc.profile_ptx_rx[0].height = ptx.h_asl;
	dataCalc.profile_ptx_rx[dataCalc.profile_ptx_rx.size()-1].height = rx.h_asl;

	TProp452_18 propPtxRx(maps, dataCalc.profile_ptx_rx);
	propPtxRx.Init(tx.frequency, ptx.longitude, ptx.latitude, rx.longitude, rx.latitude, ptx.h_ant, rx.h_ant, inRec.dN, inRec.n0);
	// Calculate vertical angles between PTx and Rx
	dataCalc.elevation_ptx_rx = propPtxRx.TxElevation()*0.18 / C_PI; // Convert from mrad to deg
	dataCalc.elevation_rx_ptx = propPtxRx.RxElevation()*0.18 / C_PI; // Convert from mrad to deg
	// Calculate the antenna attenuation a_ant for PTx->Rx
	double ptxAntatt, rxAntatt;
	CalcA_ant(ptx, rx, dataCalc, ptxAntatt, rxAntatt);
	if(dataCalc.error !=0) return;
	In452 inPtxRx(tx.frequency, inRec.time_percentage, ptx.h_ant, rx.h_ant,
				ptx.longitude, ptx.latitude, rx.longitude, rx.latitude,
				ptx.gain - ptxAntatt, rx.gain - rxAntatt, IntToPlz(inRec.plz), inRec.press, inRec.temp, inRec.dN, inRec.n0);

	//*** Calculate propagation loss a_prop for Ptx->Rx
	dataCalc.p452_ptx_rx = propPtxRx.CalcLoss(inPtxRx);

	// Transmission loss a_tot
	dataCalc.a_tot = tx.losses - tx.gain + dataCalc.p452_tx_prx.Lb - prx.gain +
				   dataCalc.a_ant_tx_prx + prx.losses + ptx.losses - ptx.gain +
				   dataCalc.p452_ptx_rx.Lb - rx.gain + dataCalc.a_ant_ptx_rx +
				   rx.losses + dataCalc.md + dataCalc.nfd + tx.atpc;
	// END OF CALCULATIONS FOR PASSIVE
	// Interference
	dataCalc.I = tx.power - dataCalc.a_tot;
	// Threshold degradation
	dataCalc.td = 10 * log10 ( 1 + pow(10,(dataCalc.I - rx.noise)/10) );
	// END
}
//!---------------------------------------------------------------------------------------------



void Hcm4AfsCalc(const MapDnN0& maps, const string& rec1, const string& rec2, const string& rec3, const string& rec4,
				 const string& path_topo, const string& path_morpho, const string& file_report,
			     const int plz, const double time_percentage, const double press, const double temp,
			     DataCalc& calcResult){
	// Start Init variables ---

	string fileReport = "";
	bool isMakeReport = false;
	if(!file_report.empty()){
		isMakeReport = true;
		fileReport = file_report;
	}
	// Check if isPassive repeater is included
	bool isPassive = false;
	if(rec2.size() > 2){
		if((rec2[0] == 'R' || rec2[0] == 'r') && (rec2[1] == 'X' || rec2[1] == 'x'))
			isPassive = false;
		else
			isPassive = true;
	}
	else
		if (rec3.size()!=0) isPassive = true;

	string reportTx, reportPrx, reportPtx, reportRx;
	if(path_topo == ""){
		calcResult.error = 36; //	Add(36, t_("Error opening topo data file (no data)"));
		calcResult.errorMsg = ErrorMsg(calcResult.error);
		WriteResults(fileReport, reportTx, reportPrx, reportPtx, reportRx, calcResult, isPassive);
		return;
	}
	calcResult.dataPaths = "TOPO = " + path_topo + "; MORPHO= " + path_morpho;

	Terrain curTerra(path_topo, path_morpho);
	// Fin Init variables ---
	
	TStdAnt antTx, antRx, antPtx, antPrx;
	HcmRec tx, rx, ptx, prx;
	calcResult.error = FillHcmData(antTx, rec1, tx, reportTx, isMakeReport);

	// Convert Tx
	if(!tx.isTx || calcResult.error == 1 || tx.isPassive){
		// If tx is not Tx
		calcResult.error = 1000;
		calcResult.errorMsg = ErrorMsg(calcResult.error);
		WriteResults(fileReport, reportTx, reportPrx, reportPtx, reportRx, calcResult, isPassive);
		return;
	}
	if(calcResult.error != 0){
		calcResult.errorMsg = ErrorMsg(calcResult.error);
		WriteResults(fileReport, reportTx, reportPrx, reportPtx, reportRx, calcResult, isPassive);
		return;
	}
	if(isPassive){
		// If link contains isPassive repeater
		// Convert PRx
		calcResult.error = FillHcmData(antPrx, rec2, prx, reportPrx, isMakeReport);
		if(!(!prx.isTx && prx.isPassive) || calcResult.error == 1){
			// If prx is not PRx
			calcResult.error = 1090;
			calcResult.errorMsg = ErrorMsg(calcResult.error);
			WriteResults(fileReport, reportTx, reportPrx, reportPtx, reportRx, calcResult, isPassive);
			return;
		}
		if(calcResult.error != 0){
			calcResult.errorMsg = ErrorMsg(calcResult.error);
			WriteResults(fileReport, reportTx, reportPrx, reportPtx, reportRx, calcResult, isPassive);
			return;
		}
		// Convert PTx
		calcResult.error = FillHcmData(antPtx, rec3, ptx, reportPtx, isMakeReport);
		if(!(ptx.isTx && ptx.isPassive) || calcResult.error == 1){
			// If ptx is not PTx
			calcResult.error = 1077;
			calcResult.errorMsg = ErrorMsg(calcResult.error);
			WriteResults(fileReport, reportTx, reportPrx, reportPtx, reportRx, calcResult, isPassive);
			return;
		}
		if(calcResult.error != 0){
			calcResult.errorMsg = ErrorMsg(calcResult.error);
			WriteResults(fileReport, reportTx, reportPrx, reportPtx, reportRx, calcResult, isPassive);
			return;
		}
		// Convert Rx
		calcResult.error = FillHcmData(antRx, rec4, rx, reportRx, isMakeReport);
		if(!(!rx.isTx && !rx.isPassive) || calcResult.error == 1){
			// If rx is not Rx
			calcResult.error = 1001;
			calcResult.errorMsg = ErrorMsg(calcResult.error);
			WriteResults(fileReport, reportTx, reportPrx, reportPtx, reportRx, calcResult, isPassive);
			return;
		}
		if(calcResult.error != 0){
			calcResult.errorMsg = ErrorMsg(calcResult.error);
			WriteResults(fileReport, reportTx, reportPrx, reportPtx, reportRx, calcResult, isPassive);
			return;
		}
	}else{
		// Convert Rx
		calcResult.error = FillHcmData(antRx, rec2, rx, reportRx, isMakeReport);
		if(!(!rx.isTx && !rx.isPassive) || calcResult.error == 1){
			// If rx is not Rx
			calcResult.error = 1001;
			calcResult.errorMsg = ErrorMsg(calcResult.error);
			WriteResults(fileReport, reportTx, reportPrx, reportPtx, reportRx, calcResult, isPassive);
			calcResult.errorMsg = ErrorMsg(calcResult.error);
			return;
		}
		if(calcResult.error != 0){
			calcResult.errorMsg = ErrorMsg(calcResult.error);
			WriteResults(fileReport, reportTx, reportPrx, reportPtx, reportRx, calcResult, isPassive);
			return;
		}
	}
	// end of converting data ---
	// Calculate interference && Set output values
	if(!isPassive){
		InCalcRec inRec(plz, time_percentage, press, temp);
		InterferencePP(maps, curTerra, tx, rx, inRec, calcResult);
	}
	else{
		InCalcRec inRec(plz, time_percentage, press, temp);
		InterferencePP(maps, curTerra, tx, prx, ptx, rx, inRec, calcResult);
	}
	// Write report file
	if(calcResult.error != 0) calcResult.errorMsg = ErrorMsg(calcResult.error);
	int err = WriteResults(fileReport, reportTx, reportPrx, reportPtx, reportRx, calcResult, isPassive);
	if(err != 0) calcResult.error = err;
}
//!---------------------------------------------------------------------------------------------

void Hcm4AfsCalcA(const MapDnN0& maps, const HcmRec& txRec, const HcmRec& rxRec,
				  const InCalcRec& inRec, DataCalc& calcResult){
	// isActive: Start Init variables ---
	// Check if isPassive repeater is included
	bool isPassive = false;
	calcResult.error = txRec.error;

	// Convert Tx
	if(! txRec.isTx || calcResult.error == 1 || txRec.isPassive){
		// If tx is not Tx
		calcResult.error = 1000;
		calcResult.errorMsg = ErrorMsg(calcResult.error);
		return;
	}
	if(calcResult.error != 0){
		calcResult.errorMsg = ErrorMsg(calcResult.error);
		return;
	}
	// Convert Rx
	calcResult.error =  rxRec.error;
	if(!(!rxRec.isTx && !rxRec.isPassive) || calcResult.error == 1){
		// If rx is not Rx
		calcResult.error = 1001;
		calcResult.errorMsg = ErrorMsg(calcResult.error);
		return;
	}
	if(calcResult.error != 0){
		calcResult.errorMsg = ErrorMsg(calcResult.error);
		return;
	}
	// end of converting data

	// Calculate interference

	HcmRec tx(txRec);
	HcmRec rx(rxRec);
	InterferencePP(maps, tx, rx, inRec, calcResult);
	if(calcResult.error != 0) calcResult.errorMsg = ErrorMsg(calcResult.error);
}
//!---------------------------------------------------------------------------------------------

void Hcm4AfsCalcP(const MapDnN0& maps, const HcmRec& txRec, const HcmRec& prxRec, const HcmRec& ptxRec, const HcmRec& rxRec,
				  const InCalcRec& inRec, DataCalc& calcResult){
	// Start Init variables ---
	// Check if isPassive repeater is included
	bool isPassive = true;

	calcResult.error = txRec.error;
	// Convert Tx
	if(! txRec.isTx || calcResult.error == 1 ||  txRec.isPassive){
		// If tx is not Tx
		calcResult.error = 1000;
		calcResult.errorMsg = ErrorMsg(calcResult.error);
		return;
	}
	if(calcResult.error != 0){
		calcResult.errorMsg = ErrorMsg(calcResult.error);
		return;
	}
	if(isPassive){
		// If link contains isPassive repeater
		// Convert PRx
		calcResult.error =  prxRec.error;
		if(!(! prxRec.isTx &&  prxRec.isPassive) || calcResult.error == 1){
			// If prx is not PRx
			calcResult.error = 1090;
			calcResult.errorMsg = ErrorMsg(calcResult.error);
			return;
		}
		if(calcResult.error != 0){
			calcResult.errorMsg = ErrorMsg(calcResult.error);
			return;
		}
		// Convert PTx
		calcResult.error =  ptxRec.error;
		if(!( ptxRec.isTx &&  ptxRec.isPassive) || calcResult.error == 1){
			// If ptx is not PTx
			calcResult.error = 1077;
			calcResult.errorMsg = ErrorMsg(calcResult.error);
			return;
		}
		if(calcResult.error != 0){
			calcResult.errorMsg = ErrorMsg(calcResult.error);
			return;
		}
		// Convert Rx
		calcResult.error =  rxRec.error;
		if(!(!rxRec.isTx && !rxRec.isPassive) || calcResult.error == 1){
			// If rx is not Rx
			calcResult.error = 1001;
			calcResult.errorMsg = ErrorMsg(calcResult.error);
			return;
		}
		if(calcResult.error != 0){
			calcResult.errorMsg = ErrorMsg(calcResult.error);
			return;
		}
	}// end of converting data

	// Calculate interference
	HcmRec tx(txRec);
	HcmRec prx(prxRec);
	HcmRec ptx(ptxRec);
	HcmRec rx(rxRec);
	InterferencePP(maps, tx, prx, ptx, rx, inRec, calcResult);
	if(calcResult.error != 0) calcResult.errorMsg = ErrorMsg(calcResult.error);
}


