#ifndef __HCM4A_hcm4a_fs_hcmfs_h_
#define __HCM4A_hcm4a_fs_hcmfs_h_


#include "hcmData.h"
#include "terrain.h"
#include "nfd.h"
#include "prop452.h"

using namespace std;

struct DataCalc{
	string dataPaths;
	int error;
	string errorMsg;
	Out452 p452_tx_rx;
	Out452 p452_tx_prx;
	Out452 p452_ptx_rx;
	double distance_tx_rx;
	double distance_tx_prx;
	double distance_ptx_rx;
	double azimuth_tx_rx;
	double azimuth_rx_tx;
	double azimuth_tx_prx;
	double azimuth_prx_tx;
	double azimuth_ptx_rx;
	double azimuth_rx_ptx;
	double elevation_tx_rx;
	double elevation_rx_tx;
	double elevation_tx_prx;
	double elevation_prx_tx;
	double elevation_ptx_rx;
	double elevation_rx_ptx;
	double diffTxRxAzimuth;
	double diffPTxRxAzimuth;
	double diffTxPRxAzimuth;
	double diffTxRxElevation;
	double diffPTxRxElevation;
	double diffTxPRxElevation;
	double diffRxTxAzimuth;
	double diffRxPTxAzimuth;
	double diffPRxTxAzimuth;
	double diffRxTxElevation;
	double diffRxPTxElevation;
	double diffPRxTxElevation;
	double diffFreqTxRx;
	double nfd;
	double md;
	double overlap_area;
	double overlap_area_co;
	double tx_area;
	short  txHasl_terr;
	short  rxHasl_terr;
	short  prxHasl_terr;
	short  ptxHasl_terr;
	double a_ant_tx_rx;		// antenna attenuation
	double a_ant_tx_prx;
	double a_ant_ptx_rx;
	double a_tot;
	double I;
	double td;
	std::vector<TrPoint> profile_tx_rx;
	std::vector<TrPoint> profile_tx_prx;
	std::vector<TrPoint> profile_ptx_rx;
	void Clear(){
		dataPaths = "";
		error				= 0;
		errorMsg			= "";
		p452_ptx_rx.Clear();
		p452_tx_rx.Clear();
		p452_tx_prx.Clear();
		distance_tx_rx		= NO_DATA;
		distance_tx_prx		= NO_DATA;
		distance_ptx_rx		= NO_DATA;
		azimuth_tx_rx		= NO_DATA;
		azimuth_rx_tx		= NO_DATA;
		azimuth_tx_prx		= NO_DATA;
		azimuth_rx_ptx		= NO_DATA;
		azimuth_prx_tx		= NO_DATA;
		azimuth_ptx_rx		= NO_DATA;
		elevation_prx_tx	= NO_DATA;
		elevation_ptx_rx	= NO_DATA;
		elevation_rx_ptx	= NO_DATA;
		elevation_rx_tx		= NO_DATA;
		elevation_tx_prx	= NO_DATA;
		elevation_tx_rx		= NO_DATA;
		diffPRxTxAzimuth	= NO_DATA;
		diffPRxTxElevation	= NO_DATA;
		diffPTxRxAzimuth	= NO_DATA;
		diffPTxRxElevation	= NO_DATA;
		diffRxPTxAzimuth	= NO_DATA;
		diffRxPTxElevation	= NO_DATA;
		diffRxTxAzimuth		= NO_DATA;
		diffRxTxElevation	= NO_DATA;
		diffTxPRxAzimuth	= NO_DATA;
		diffTxPRxElevation	= NO_DATA;
		diffTxRxAzimuth		= NO_DATA;
		diffTxRxElevation	= NO_DATA;
		diffFreqTxRx		= NO_DATA;
		nfd					= NO_DATA;
		md					= NO_DATA;
		overlap_area		= NO_DATA;
		overlap_area_co		= NO_DATA;
		tx_area				= NO_DATA;
		txHasl_terr			= NO_DATA;
		rxHasl_terr			= NO_DATA;
		prxHasl_terr		= NO_DATA;
		ptxHasl_terr		= NO_DATA;
		a_ant_ptx_rx		= NO_DATA;
		a_ant_tx_prx		= NO_DATA;
		a_ant_tx_rx			= NO_DATA;
		a_tot				= NO_DATA;
		I					= NO_DATA;
		td					= NO_DATA;
		profile_tx_rx.clear();
		profile_tx_prx.clear();
		profile_ptx_rx.clear();
	}
	DataCalc(){Clear();}
};

struct InCalcRec{
	int plz;
	double time_percentage;
	double press;
	double temp;
	double dN;
	double n0;
	void Clear(){
		plz = 1;				// hor=1, vert=2
		time_percentage = 0.0;	// 20.0
		press = 0.0;
		temp = 0.0;
		dN = 0.0;
		n0 = 0.0;
	}
	void OnCalcFish(){
		plz = 1;				// hor=1, vert=2
		time_percentage = 20.0;
		press = 1013.0;
		temp = 15.0;
		dN = 45.0;
		n0 = 325.0;
	}
	string ToString()const{
	    std::stringstream ss;
		ss << to_string("plz", plz);
		ss << to_string("time_percentage", time_percentage);
		ss << to_string("press", press);
		ss << to_string("temp", temp);
		ss << to_string("dN", dN);
		ss << to_string("n0", n0);
		ss << to_string("\n\n");
		return ss.str();
	};
	
	InCalcRec(){Clear();}
	InCalcRec(const int plz, const double time_percentage, const double press, const double temp):
		  plz(plz), time_percentage(time_percentage), press(press), temp(temp), dN(0.0), n0(0.0){}
	InCalcRec(const int plz, const double time_percentage, const double press, const double temp, const double dN, const double n0):
		  plz(plz), time_percentage(time_percentage), press(press), temp(temp), dN(dN), n0(n0){}
};

//!---------------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------------

void Hcm4AfsCalc(const MapDnN0& maps, const string& rec1, const string& rec2, const string& rec3, const string& rec4,
				 const string& path_topo, const string& path_morpho, const string& file_report,
			     const int plz, const double time_percentage, const double press, const double temp,
			     DataCalc& calcResult);

void Hcm4AfsCalcA(const MapDnN0& maps, const HcmRec& tx, const HcmRec& rx,
				  const InCalcRec& inRec,  DataCalc& calcResult);
				  
void Hcm4AfsCalcP(const MapDnN0& maps, const HcmRec& tx, const HcmRec& prx, const HcmRec& ptx, const HcmRec& rx,
				  const InCalcRec& inRec,  DataCalc& calcResult);

double	AntennaAtt(double angle, vector<AntPoint>& pattern);
double	GetElevation(double antH1, double Hasl1, double antH2, double Hasl2, double distance);

void	InterferencePP(const MapDnN0& maps, Terrain& terr, HcmRec& tx, HcmRec& rx,
					   const InCalcRec& inRec, DataCalc& result);
void	InterferencePP(const MapDnN0& maps, Terrain& terr, HcmRec& tx, HcmRec& prx, HcmRec& ptx, HcmRec& rx,
					   const InCalcRec& inRec, DataCalc& result);
void	InterferencePP(const MapDnN0& maps, HcmRec& tx, HcmRec& rx,
					   const InCalcRec& inRec, DataCalc& result);
void	InterferencePP(const MapDnN0& maps, HcmRec& tx, HcmRec& prx, HcmRec& ptx, HcmRec& rx,
					   const InCalcRec& inRec, DataCalc& result);

void	ProcessErrors(const string& code, int errIn, int& errOut, string& errMsg);
string	ErrorMsg(int err);

int		CalcNfd(HcmRec& tx, HcmRec& rx, double& nfd, double& md, double& overlap_area, double& overlap_area_co, double& tx_area);
void	CalcA_ant(const HcmRec& tx, const HcmRec& rx, DataCalc& result, double& txAntatt, double& rxAntatt);
double	CombineAngles(double angle1, double angle2);

string  GetLibraryVersion();
int		WriteResults(const string& filename, string& report, DataCalc& result, bool isPassive);
string	PrintResultA(const DataCalc& result);
string	PrintResultP(const DataCalc& result);

#endif
