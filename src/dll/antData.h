#ifndef __HCM4A__DLL_hcm4afs_antData_h_
#define __HCM4A__DLL_hcm4afs_antData_h_

#include "common.h"

using namespace std;


struct AntPoint{
	double angle;
	double attenuation;
	void Clear(){
		angle = 0.0;
		attenuation = 0.0;
	}
	string ToPrint()const{
		std::stringstream ss;
		ss << "\t angle: " << to_string(angle) << "\t attenuation: " << to_string(attenuation) << "\n";
		return ss.str();
	}
	string ToString()const{
		std::stringstream ss;
		ss << "\t\t\t\t=\t" << to_string(angle) << "\t" << to_string(attenuation) << "\n";
		return ss.str();
	}
	AntPoint(){Clear();}
	AntPoint(const double angle, const double attenuation):angle(angle), attenuation(attenuation){}
	AntPoint(const AntPoint& point): angle(point.angle), attenuation(point.attenuation){}
};
//! --------------------------------------------------------------------------------------------

// Class of standard antenna data
class TStdAnt{
private:
	int err;
	void Initialise();
	static const double freq_table[301][13];
	static const double att_table[301][13];
	struct AntRec{
		int Gain;
		vector<AntPoint> CPpoint;
		vector<AntPoint> XPpoint;
	};
	vector<AntRec> data;
public:
	int Error(){return err;};
	void CpPattern(double gain, vector<AntPoint>& pattern);
	void XpPattern(double gain, vector<AntPoint>& pattern);
	TStdAnt();
};
//!---------------------------------------------------------------------------------------------


#endif
