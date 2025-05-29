#ifndef __HCM4A_hcm4a_fs_prop452_h_
#define __HCM4A_hcm4a_fs_prop452_h_

#include "lib452_18.h"

using namespace R452_18;

class TProp452_18{
public:
	bool Init(double f, double tLong, double tLat, double rLong, double rLat, double htg, double hrg, double DN, double N0);
	Out452 CalcLoss(const In452& inRec);
	const Out452& GetCalRec(){return calcRes;};
	double TxElevation();
	double RxElevation();
	TProp452_18(const MapDnN0& mapDnN0, const vector<TrPoint>& prf);
protected:
	const vector<TrPoint> prf;
	const MapDnN0& mapDnN0;
	Out452 calcRes;
	bool isCalcEnable;
};
//! --------------------------------------------------------------------------------------------




#endif
