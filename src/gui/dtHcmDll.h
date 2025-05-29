#ifndef __HCM4A_TCalc_FS4A_dtHcmDll_h_
#define __HCM4A_TCalc_FS4A_dtHcmDll_h_

#include "dtHcmData.h"


// --- pathtype:
enum class DllPathType {los=1, thor=2};
// DllDllPathType::los	- (1)[Line of Sight]
// DllPathType::thor	- (2)[Trans-Horizon]
inline DllPathType IntToDllPathType(int t){
	if(t == static_cast<int>(DllPathType::los))  return DllPathType::los;
	if(t == static_cast<int>(DllPathType::thor)) return DllPathType::thor;
	return DllPathType::los;
}
inline int DllPathTypeToInt(DllPathType t){
	return static_cast<int>(t);
}
inline string DllPathTypeToStr(DllPathType t){
	switch(t){
		case DllPathType::los:
			return "los";
		case DllPathType::thor:
			return "thor";
	}
	return "los";
}
//! --------------------------------------------------------------------------------------------

// for parametr DLL
struct HcmRecDll{
	bool  isTx;				// true: Tx=transmitter, false: Rx=receiver
	bool  isPassive;			// Ptx=passive transmitter, Prx=passive receiver
	double frequency;			// Frequency unit G: GHz
	std::string name_of_station;
	std::string country;
	double latitude;
	double longitude;
	short	h_asl;
	std::string eqp_manuf_name;
	std::string eqp_type;
	std::string capacity;
	std::string eqclass;
	double channel_spacing;
	double power;
	double atpc;
	double azimuth;
	double elevation;
	std::string polarization;
	double noise;
	double losses;
	short h_ant;
	std::string ant_manuf_name;
	std::string ant_type;
	double gain;
	std::string cp_pattern;
	std::string xp_pattern;
	int error;
	std::vector<TrMask> mask;
	std::vector<AntPoint> cpData;
	std::vector<AntPoint> xpData;
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
	std::string ToString()const{
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
	HcmRecDll(){Clear();}
	HcmRecDll(const bool isTx, const bool isPassive, const double frequency, const std::string& name_of_station,
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

	HcmRecDll(const HcmRecDll& hcmRec):isTx(hcmRec.isTx), isPassive(hcmRec.isPassive), frequency(hcmRec.frequency),
								 name_of_station(hcmRec.name_of_station), country(hcmRec.country),
								 latitude(hcmRec.latitude), longitude(hcmRec.longitude), h_asl(hcmRec.h_asl),
								 eqp_manuf_name(hcmRec.eqp_manuf_name), eqp_type(hcmRec.eqp_type), capacity(hcmRec.capacity),
								 eqclass(hcmRec.eqclass), channel_spacing(hcmRec.channel_spacing), power(hcmRec.power),
								 atpc(hcmRec.atpc), azimuth(hcmRec.azimuth), elevation(hcmRec.elevation), polarization(hcmRec.polarization),
								 noise(hcmRec.noise), losses(hcmRec.losses), h_ant(hcmRec.h_ant), ant_manuf_name(hcmRec.ant_manuf_name),
								 ant_type(hcmRec.ant_type), gain(hcmRec.gain), cp_pattern(hcmRec.cp_pattern), xp_pattern(hcmRec.xp_pattern),
								 error(hcmRec.error), mask(hcmRec.mask), cpData(hcmRec.cpData), xpData(hcmRec.xpData){}

	HcmRecDll(const HcmRec& hcmRec):isTx(hcmRec.isTx), isPassive(hcmRec.isPassive), frequency(hcmRec.frequency),
								 name_of_station(hcmRec.name_of_station), country(hcmRec.country),
								 latitude(hcmRec.latitude), longitude(hcmRec.longitude), h_asl(hcmRec.h_asl),
								 eqp_manuf_name(hcmRec.eqp_manuf_name), eqp_type(hcmRec.eqp_type), capacity(hcmRec.capacity),
								 eqclass(hcmRec.eqclass), channel_spacing(hcmRec.channel_spacing), power(hcmRec.power),
								 atpc(hcmRec.atpc), azimuth(hcmRec.azimuth), elevation(hcmRec.elevation), polarization(hcmRec.polarization),
								 noise(hcmRec.noise), losses(hcmRec.losses), h_ant(hcmRec.h_ant), ant_manuf_name(hcmRec.ant_manuf_name),
								 ant_type(hcmRec.ant_type), gain(hcmRec.gain), cp_pattern(hcmRec.cp_pattern), xp_pattern(hcmRec.xp_pattern),
								 error(hcmRec.tError.Code()){
					for(auto v: hcmRec.mask)
						mask.push_back(v);
					for(auto v: hcmRec.cpData)
						cpData.push_back(v);
					for(auto v: hcmRec.xpData)
						xpData.push_back(v);
	}
};
//! --------------------------------------------------------------------------------------------

struct InCalcRec{
	int plz;
	double time_percentage;
	double press;
	double temp;
	double dN;
	double n0;
	void Clear(){
		plz = 0;
		time_percentage = 0.0;
		press = 0.0;
		temp = 0.0;
		dN = 0.0;
		n0 = 0.0;
	}
	void OnDefault(){
		plz = 1;	// Plz::hor
		time_percentage = 20.0;
		press = 1013.0;
		temp = 15.0;
		dN = 45.0;
		n0 = 325.0;
	}
	bool IsEmpty(){
		return	time_percentage == 0.0 || press == 0.0 || temp == 0.0;
	}
	String ToString()const{
		return Format("plz: %s, time_percentage: %f, press: %f, temp: %f, dN: %f, n0: %f", PlzToStr(IntToPlz(plz)), time_percentage, press, temp, dN, n0);
	}
	InCalcRec(){Clear();}
	InCalcRec(const int plz, const double time_percentage, const double press, const double temp):
		  plz(plz), time_percentage(time_percentage), press(press), temp(temp), dN(0.0), n0(0.0){}
	InCalcRec(const int plz, const double time_percentage, const double press, const double temp, const double dN, const double n0):
		  plz(plz), time_percentage(time_percentage), press(press), temp(temp), dN(dN), n0(n0){}

};
//! --------------------------------------------------------------------------------------------

struct Out452{
	double Lbd;
	double Ldp;
	double Lbd50;
	double Lb;
	double Lba;
	double Lbfsg;
	double Lb0p;
	double Lb0beta; //	Lb0b
	double Lbs;
	double theta;
	double theta_t;
	double theta_r;
	DllPathType pathType;
	double d_lt; //		dlt
	double d_lr; //		dlr
	double d_tm; //		dtm
	double d_lm; //		dlm
	double d_ct; //		Distance over land from the transmit and receive
	double d_cr; //		antennas to the coast along the great-circle interference path (km). Set to zero for a terminal on a ship or sea platform
	double h_te; //		hte
	double h_re; //		hre
	double h_m;	 //		hm
	double h_st; //
	double h_sr; //
	double omega;
	double Ag;
	double DN;
	double N0;
	double elevation_tx;
	double elevation_rx;

	double ae;
	double ab;
	double dtot;
	double hts;
	double hrs;
	double hstd;
	double hsrd;
	double b0;
	double Ldsph;
	double Ld50;
	int error;

	void Clear(){
		Lbd = 0.0;
		Ldp = 0.0;
		Lbd50 = 0.0;
		Lb = 0.0;
		Lba = 0.0;
		Lbfsg = 0.0;
		Lb0p = 0.0;
		Lb0beta = 0.0;
		Lbs = 0.0;
		theta = 0.0;
		theta_t = 0.0;
		theta_r = 0.0;
		DllPathType pathType = DllPathType::los;
		d_lt = 0.0;
		d_lr = 0.0;
		d_tm = 0.0;
		d_lm = 0.0;
		d_ct = 0.0;
		d_cr = 0.0;
		h_te = 0.0;
		h_re = 0.0;
		h_m = 0.0;
		h_st = 0.0;
		h_sr = 0.0;
		omega = 0.0;
		Ag = 0.0;
		DN = 0.0;
		N0 = 0.0;
		elevation_tx = 0.0;
		elevation_rx = 0.0;
		ae = 0.0;
		ab = 0.0;
		dtot = 0.0;
		hts = 0.0;
		hrs = 0.0;
		hstd = 0.0;
		hsrd = 0.0;
		b0 = 0.0;
		Ldsph = 0.0;
		Ld50 = 0.0;
		error = 0;
	}
    string ToString()const{
		std::stringstream ss;
		ss << to_string("Out452Rec:", "");
	    ss << to_string(" Lbd", Lbd);
	    ss << to_string(" Ldp", Ldp);
	    ss << to_string(" Lbd50", Lbd50);
	    ss << to_string(" Lb", Lb);
	    ss << to_string(" Lba", Lba);
	    ss << to_string(" Lbfsg", Lbfsg);
	    ss << to_string(" Lb0p", Lb0p);
	    ss << to_string(" Lb0beta", Lb0beta);
	    ss << to_string(" Lbs", Lbs);
	    ss << to_string(" theta", theta);
	    ss << to_string(" theta_t", theta_t);
	    ss << to_string(" theta_r", theta_r);
		ss << to_string(" pathType", DllPathTypeToStr(pathType));
	    ss << to_string(" d_lt", d_lt);
	    ss << to_string(" d_lr", d_lr);
	    ss << to_string(" d_tm", d_tm);
	    ss << to_string(" d_lm", d_lm);
	    ss << to_string(" d_ct", d_ct);
	    ss << to_string(" d_cr", d_cr);
	    ss << to_string(" h_te", h_te);
	    ss << to_string(" h_re", h_re);
	    ss << to_string(" h_st", h_st);
	    ss << to_string(" h_sr", h_sr);
	    ss << to_string(" h_m", h_m);
	    ss << to_string(" omega", omega);
	    ss << to_string(" Ag", Ag);
	    ss << to_string(" DN", DN);
	    ss << to_string(" N0", N0);
	    ss << to_string(" elevation_tx", elevation_tx);
	    ss << to_string(" elevation_rx", elevation_rx);
		ss << to_string(" ae", ae);
		ss << to_string(" ab", ab);
		ss << to_string(" dtot", dtot);
		ss << to_string(" hts", hts);
		ss << to_string(" hrs", hrs);
		ss << to_string(" hstd", hstd);
		ss << to_string(" hsrd", hsrd);
		ss << to_string(" b0", b0);
		ss << to_string(" Ldsph", Ldsph);
		ss << to_string(" Ld50", Ld50);
		ss << to_string(" Error", error);
		return ss.str();
    };
	Out452(){Clear();}
};
//!---------------------------------------------------------------------------------------------


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
		distance_tx_rx		= 0.0;
		distance_tx_prx		= 0.0;
		distance_ptx_rx		= 0.0;
		azimuth_tx_rx		= 0.0;
		azimuth_rx_tx		= 0.0;
		azimuth_tx_prx		= 0.0;
		azimuth_rx_ptx		= 0.0;
		azimuth_prx_tx		= 0.0;
		azimuth_ptx_rx		= 0.0;
		elevation_prx_tx	= 0.0;
		elevation_ptx_rx	= 0.0;
		elevation_rx_ptx	= 0.0;
		elevation_rx_tx		= 0.0;
		elevation_tx_prx	= 0.0;
		elevation_tx_rx		= 0.0;
		diffPRxTxAzimuth	= 0.0;
		diffPRxTxElevation	= 0.0;
		diffPTxRxAzimuth	= 0.0;
		diffPTxRxElevation	= 0.0;
		diffRxPTxAzimuth	= 0.0;
		diffRxPTxElevation	= 0.0;
		diffRxTxAzimuth		= 0.0;
		diffRxTxElevation	= 0.0;
		diffTxPRxAzimuth	= 0.0;
		diffTxPRxElevation	= 0.0;
		diffTxRxAzimuth		= 0.0;
		diffTxRxElevation	= 0.0;
		diffFreqTxRx		= 0.0;
		nfd					= 0.0;
		md					= 0.0;
		overlap_area		= 0.0;
		overlap_area_co		= 0.0;
		tx_area				= 0.0;
		txHasl_terr			= 0.0;
		rxHasl_terr			= 0.0;
		prxHasl_terr		= 0.0;
		ptxHasl_terr		= 0.0;
		a_ant_ptx_rx		= 0.0;
		a_ant_tx_prx		= 0.0;
		a_ant_tx_rx			= 0.0;
		a_tot				= 0.0;
		I					= 0.0;
		td					= 0.0;
		profile_tx_rx.clear();
		profile_tx_prx.clear();
		profile_ptx_rx.clear();
	}
	DataCalc(){Clear();}
	String ToReportA()const;
	String ToReportP()const;
};

String DataCalc::ToReportA()const{
	String repRes =	"  \n"
					"Output values:\n";
	char ctmp[50];
	sprintf(ctmp, "%8i", error);
	repRes = repRes +	  "  Error value                	= " + string(ctmp) + ": " + errorMsg + "\n";
	if(error ==0){
		sprintf(ctmp, "%8.3f", distance_tx_rx);
		repRes = repRes + "  Distance                   	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", azimuth_tx_rx);
		repRes = repRes + "  H.direction Tx->Rx         	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", azimuth_rx_tx);
		repRes = repRes + "  H.direction Rx->Tx         	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", elevation_tx_rx);
		repRes = repRes + "  V.direction Tx->Rx         	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", elevation_rx_tx);
		repRes = repRes + "  V.direction Rx->Tx         	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", diffTxRxAzimuth);
		repRes = repRes + "  H. diff.ang. Tx->Rx        	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", diffTxRxElevation);
		repRes = repRes + "  V. diff.ang. Tx->Rx        	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", diffRxTxAzimuth);
		repRes = repRes + "  H. diff.ang. Rx->Tx        	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", diffRxTxElevation);
		repRes = repRes + "  V. diff.ang. Rx->Tx        	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", a_ant_tx_rx);
		repRes = repRes + "  a_ant                      	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", diffFreqTxRx);
		repRes = repRes + "  Frequncy diff. (Tx-Rx)     	= " + string(ctmp) + "\t\tMHz\n";

		sprintf(ctmp, "%8.3f", tx_area);
		repRes = repRes + "  Tx_Area                    	= " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", overlap_area);
		repRes = repRes + "  O_Area                     	= " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", overlap_area_co);
		repRes = repRes + "  O_Area at co-channel       	= " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", md);
		repRes = repRes + "  Mask Discrimination MD     	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", nfd);
		repRes = repRes + "  NetFilterDiscrimin.NFD     	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8i", txHasl_terr);
		repRes = repRes + "  Database height of Tx      	= " + string(ctmp) + "\t\tm\n";

		sprintf(ctmp, "%8i", rxHasl_terr);
		repRes = repRes + "  Database height of Rx      	= " + string(ctmp) + "\t\tm\n";

		string ptype;
		if(p452_tx_rx.pathType == DllPathType::los)
			ptype = "line-of-sight";
		else
			ptype = "transhorizon";
		repRes = repRes + "  Path_type                  	= " + ptype + "\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.Lbd);
		repRes = repRes + "  Lbd                        	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.Ldp);
		repRes = repRes + "  Ldp                        	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.Lbd50);
		repRes = repRes + "  Lbd50                      	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.Lb);
		repRes = repRes + "  Lb                         	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.Lba);
		repRes = repRes + "  Lba                        	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.Lbfsg);
		repRes = repRes + "  Lbfsg                      	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.Lb0p);
		repRes = repRes + "  Lb0p                       	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.Lb0beta);
		repRes = repRes + "  Lb0beta                    	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.Lbs);
		repRes = repRes + "  Lbs                        	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.theta);
		repRes = repRes + "  theta                      	= " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.theta_t);
		repRes = repRes + "  theta_t                    	= " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.theta_r);
		repRes = repRes + "  theta_r                    	= " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.d_lt);
		repRes = repRes + "  d_lt                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.d_lr);
		repRes = repRes + "  d_lr                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.d_tm);
		repRes = repRes + "  d_tm                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.d_lm);
		repRes = repRes + "  d_lm                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.d_ct);
		repRes = repRes + "  d_ct                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.d_cr);
		repRes = repRes + "  d_cr                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.h_te);
		repRes = repRes + "  h_te                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.h_re);
		repRes = repRes + "  h_re                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.h_m);
		repRes = repRes + "  h_m                        	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.omega);
		repRes = repRes + "  omega                      	= " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", p452_tx_rx.Ag);
		repRes = repRes + "  Ag                         	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", a_tot);
		repRes = repRes + "  a_tot                      	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", I);
		repRes = repRes + "  I                          	= " + string(ctmp) + "\t\tdBW\n";

		sprintf(ctmp, "%8.3f", td);
		repRes = repRes + "  TD                         	= " + string(ctmp) + "\t\tdB\n";

		repRes = repRes + "\n\n\n"
			"   Terrain profile (1st value = Tx site, last value = Rx site)\n"
			"   Distance between two points ~ 100m.\n\n";

		sprintf(ctmp, "%8i", profile_tx_rx.size());
		repRes = repRes + "  Number of points           = " + string(ctmp) + "\n\n";
		// print topo profile
		for(int i = 1 ; i <= profile_tx_rx.size() ; i++)		{
			sprintf(ctmp, "%5i", static_cast<int>(profile_tx_rx[i-1].height));
			repRes = repRes + " " + string(ctmp);
			if (i/10.0 -floor(i/10.0) == 0.0 && i!=0) repRes = repRes + "\n";
		}
		repRes = repRes + "\n\n\n"
			"   Morphological profile (1st value = Tx site, last value = Rx site)\n"
			"   0 = inland, 1 = sea, 4 = costal area\n\n";
		// print morpho profile
		for(int i = 1 ; i <= profile_tx_rx.size() ; i++){
			// TODO: profile_tx_rx[i-1].zone = enum class Zone {A2=0, B=1, A1=4};
			sprintf(ctmp, "%5i", ZoneToInt(profile_tx_rx[i-1].zone));
			repRes = repRes + " " + string(ctmp);
			if (i/10.0 -floor(i/10.0) == 0.0 && i!=0) repRes = repRes + "\n";
		}
		repRes = repRes + "\n\n"
						  "  -------------------------------------------------------------------"
						  "\n\n";
	}
	else{
		sprintf(ctmp, "%8i", error);
		repRes = repRes + "  Error value                	= " + string(ctmp) + ": " + errorMsg + "\n";
	}
	return repRes;
}

String DataCalc::ToReportP()const{
	String repRes =	"  \n"
					"Output values:\n";
	char ctmp[50];
	sprintf(ctmp, "%8i", error);
	repRes = repRes + "  Error value                	= " + string(ctmp) + ": " + errorMsg + "\n";
	if(error ==0){
		// PASSIVE CASE
		sprintf(ctmp, "%8.3f", distance_tx_prx);
		repRes = repRes + "  Distance Tx - PRx          	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", distance_ptx_rx);
		repRes = repRes + "  Distance PTx - Rx          	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", azimuth_tx_prx);
		repRes = repRes + "  H.direction Tx->PRx        	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", azimuth_prx_tx);
		repRes = repRes + "  H.direction PRx->Tx        	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", elevation_tx_prx);
		repRes = repRes + "  V.direction Tx->PRx        	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", elevation_prx_tx);
		repRes = repRes + "  V.direction PRx->Tx        	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", diffTxPRxAzimuth);
		repRes = repRes + "  H. diff.ang. Tx->PRx       	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", diffTxPRxElevation);
		repRes = repRes + "  V. diff.ang. Tx->PRx       	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", diffPRxTxAzimuth);
		repRes = repRes + "  H. diff.ang. PRx->Tx       	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", diffPRxTxElevation);
		repRes = repRes + "  V. diff.ang. PRx->Tx       	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", a_ant_tx_prx);
		repRes = repRes + "  a_ant Tx - PRx             	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", azimuth_ptx_rx);
		repRes = repRes + "  H.direction PTx->Rx        	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", azimuth_rx_ptx);
		repRes = repRes + "  H.direction Rx->PTx        	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", elevation_ptx_rx);
		repRes = repRes + "  V.direction PTx->Rx        	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", elevation_rx_ptx);
		repRes = repRes + "  V.direction Rx->PTx        	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", diffPTxRxAzimuth);
		repRes = repRes + "  H. diff.ang. PTx->Rx       	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", diffPTxRxElevation);
		repRes = repRes + "  V. diff.ang. PTx->Rx       	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", diffRxPTxAzimuth);
		repRes = repRes + "  H. diff.ang. Rx->PTx       	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", diffRxPTxElevation);
		repRes = repRes + "  V. diff.ang. Rx->PTx       	= " + string(ctmp) + "\t\tdegrees\n";

		sprintf(ctmp, "%8.3f", a_ant_ptx_rx);
		repRes = repRes + "  a_ant PTx - Rx             	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", fabs(diffFreqTxRx));
		repRes = repRes + "  Frequncy diff. (Tx-Rx)     	= " + string(ctmp) + "\t\tMHz\n";

		sprintf(ctmp, "%8.3f", tx_area);
		repRes = repRes + "  Tx_Area                    	= " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", overlap_area);
		repRes = repRes + "  O_Area                     	= " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", overlap_area_co);
		repRes = repRes + "  O_Area at co-channel       	= " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", md);
		repRes = repRes + "  Mask Discrimination MD     	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", nfd);
		repRes = repRes + "  NetFilterDiscrimin.NFD     	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8i", txHasl_terr);
		repRes = repRes + "  Database height of Tx      	= " + string(ctmp) + "\t\tm\n";

		sprintf(ctmp, "%8i", rxHasl_terr);
		repRes = repRes + "  Database height of Rx      	= " + string(ctmp) + "\t\tm\n";

		sprintf(ctmp, "%8i", prxHasl_terr);
		repRes = repRes + "  Database height of PRx     	= " + string(ctmp) + "\t\tm\n";

		string ptype;
		if(p452_tx_prx.pathType == DllPathType::los)
			ptype = "line-of-sight";
		else
			ptype = "transhorizon";
		repRes = repRes + "  Path_type Tx - PRx         	= " + ptype + "\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.Lbd);
		repRes = repRes + "  Lbd                        	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.Ldp);
		repRes = repRes + "  Ldp                        	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.Lbd50);
		repRes = repRes + "  Lbd50                      	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.Lb);
		repRes = repRes + "  Lb                         	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.Lba);
		repRes = repRes + "  Lba                        	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.Lbfsg);
		repRes = repRes + "  Lbfsg                      	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.Lb0p);
		repRes = repRes + "  Lb0p                       	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.Lb0beta);
		repRes = repRes + "  Lb0beta                    	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.Lbs);
		repRes = repRes + "  Lbs                        	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.theta);
		repRes = repRes + "  theta                      	= " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.theta_t);
		repRes = repRes + "  theta_t                    	= " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.theta_r);
		repRes = repRes + "  theta_r                    	= " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.d_lt);
		repRes = repRes + "  d_lt                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.d_lr);
		repRes = repRes + "  d_lr                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.d_tm);
		repRes = repRes + "  d_tm                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.d_lm);
		repRes = repRes + "  d_lm                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.d_ct);
		repRes = repRes + "  d_ct                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.d_cr);
		repRes = repRes + "  d_cr                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.h_te);
		repRes = repRes + "  h_te                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.h_re);
		repRes = repRes + "  h_re                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.h_m);
		repRes = repRes + "  h_m                        	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.omega);
		repRes = repRes + "  omega                      	= " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", p452_tx_prx.Ag);
		repRes = repRes + "  Ag                         	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8i", ptxHasl_terr);
		repRes = repRes + "  Database height of PTx     	= " + string(ctmp) + "\t\tm\n";

		if(p452_ptx_rx.pathType == DllPathType::los)
			ptype = "line-of-sight";
		else
			ptype = "transhorizon";
		repRes = repRes + "  Path_type PTx - Rx         	= " + ptype + "\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.Lbd);
		repRes = repRes + "  Lbd                        	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.Ldp);
		repRes = repRes + "  Ldp                        	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.Lbd50);
		repRes = repRes + "  Lbd50                      	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.Lb);
		repRes = repRes + "  Lb                         	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.Lba);
		repRes = repRes + "  Lba                        	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.Lbfsg);
		repRes = repRes + "  Lbfsg                      	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.Lb0p);
		repRes = repRes + "  Lb0p                       	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.Lb0beta);
		repRes = repRes + "  Lb0beta                    	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.Lbs);
		repRes = repRes + "  Lbs                        	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.theta);
		repRes = repRes + "  theta                      	= " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.theta_t);
		repRes = repRes + "  theta_t                    	= " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.theta_r);
		repRes = repRes + "  theta_r                    	= " + string(ctmp) + "\t\tmrad\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.d_lt);
		repRes = repRes + "  d_lt                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.d_lr);
		repRes = repRes + "  d_lr                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.d_tm);
		repRes = repRes + "  d_tm                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.d_lm);
		repRes = repRes + "  d_lm                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.d_ct);
		repRes = repRes + "  d_ct                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.d_cr);
		repRes = repRes + "  d_cr                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.h_te);
		repRes = repRes + "  h_te                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.h_re);
		repRes = repRes + "  h_re                       	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.h_m);
		repRes = repRes + "  h_m                        	= " + string(ctmp) + "\t\tkm\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.omega);
		repRes = repRes + "  omega                      	= " + string(ctmp) + "\n";

		sprintf(ctmp, "%8.3f", p452_ptx_rx.Ag);
		repRes = repRes + "  Ag                         	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", a_tot);
		repRes = repRes + "  a_tot                      	= " + string(ctmp) + "\t\tdB\n";

		sprintf(ctmp, "%8.3f", I);
		repRes = repRes + "  I                          	= " + string(ctmp) + "\t\tdBW\n";

		sprintf(ctmp, "%8.3f", td);
		repRes = repRes + "  TD                         	= " + string(ctmp) + "\t\tdB\n";

		repRes = repRes + "\n\n\n"
				"   Terrain profile (1st value = Tx site, last value = PRx site)\n"
				"   Distance between two points ~ 100m \n\n";

		sprintf(ctmp, "%8i", profile_tx_prx.size());
		repRes = repRes + "  Number of points           = " + string(ctmp) + "\n\n";

		// print topo profile
		for(int i = 1; i <= profile_tx_prx.size(); i++){
			sprintf(ctmp, "%5i", static_cast<int>(profile_tx_prx[i-1].height));
			repRes = repRes + " " + string(ctmp);
			if(i/10.0 -floor(i/10.0) == 0.0 && i!=0) repRes = repRes + "\n";
		}
		repRes = repRes + "\n\n\n"
				"   Morphological profile (1st value = Tx site, last value = PRx site)\n"
				"   0 = inland, 1 = sea, 4 = costal area\n\n";

		// print morpho profile
		for(int i = 1; i <= profile_tx_prx.size(); i++){
			// TODO: profile_tx_prx[i-1].zone = enum class Zone {A2=0, B=1, A1=4};
			sprintf(ctmp, "%5i", ZoneToInt(profile_tx_prx[i-1].zone));
			repRes = repRes + " " + string(ctmp);
			if(i/10.0 -floor(i/10.0) == 0.0 && i!=0) repRes = repRes + "\n";
		}
		repRes = repRes + "\n\n\n"
				"   Terrain profile (1st value = PTx site, last value = Rx site)\n"
				"   Distance between two points ~ 100m \n\n";

		sprintf(ctmp, "%8i", profile_ptx_rx.size());
		repRes = repRes + "  Number of points           = " + string(ctmp) + "\n\n";

		// print topo profile
		for(int i = 1; i <= profile_ptx_rx.size(); i++){
			sprintf(ctmp, "%5i", static_cast<int>(profile_ptx_rx[i-1].height));
			repRes = repRes + " " + string(ctmp);
			if(i/10.0 -floor(i/10.0) == 0.0 && i!=0) repRes = repRes + "\n";
		}
		repRes = repRes + "\n\n\n"
				"   Morphological profile (1st value = PTx site, last value = Rx site)\n"
				"   0 = inland, 1 = sea, 4 = costal area\n\n";

		// print morpho profile
		for(int i = 1; i <= profile_ptx_rx.size(); i++){
			// TODO: profile_ptx_rx[i-1].zone = enum class Zone {A2=0, B=1, A1=4};
			sprintf(ctmp, "%5i", ZoneToInt(profile_ptx_rx[i-1].zone));
			repRes = repRes + " " + string(ctmp);
			if(i/10.0 -floor(i/10.0) == 0.0 && i!=0) repRes = repRes + "\n";
		}
		repRes = repRes + "\n\n"
			  "-------------------------------------------------------------------"
						  "\n\n";
	}
	else{
		sprintf(ctmp, "%8i", error);
		repRes = repRes + "  Error value                	= " + string(ctmp) + ": " + errorMsg + "\n";
	}
	return repRes;
}
//!---------------------------------------------------------------------------------------------


#endif
