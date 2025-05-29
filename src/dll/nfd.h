#ifndef __HCM4A_hcm4a_fs_nfd_h_
#define __HCM4A_hcm4a_fs_nfd_h_

#include "hcmData.h"

// Interference calculations
class TNfd{
private:
	struct Element{
      double frequency;
      double txAtt;
	  double rxAtt;
	};
	static const double txmask_table[165][17];
	static const double rxmask_table[165][17];
	vector<TrMask> txmask;
	vector<TrMask> rxmask;
	double FlatElement(double TxdB, double RxdB, double dF);
	double SlopeElement(double Tx1dB, double Rx1dB, double Tx2dB, double Rx2dB, double dF);
	double TxArea();
	double OverlappingArea(vector<Element>& mask_items);
	void CorrectMask(vector<TrMask>& maskIn, vector<TrMask>& maskOut);
	void CreateElements(double offset, vector<Element>& mask_items);
public:
	int  error;
	void SetTxMask(vector<TrMask>& mask);
	void SetTxMask(double frequency_GHz, string eqclass, double channelsp);
	void SetRxMask(double frequency_GHz, string eqclass, double channelsp);
	void SetRxMask(vector<TrMask>& mask);
	double MD(double& tx_area);
	double NFD(double offset, double& overlap_area, double& overlap_area_co);
	TNfd();
};



#endif
