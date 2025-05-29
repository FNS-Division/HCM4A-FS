#ifndef __HCM4A_CalcFs4A_dtCalc_h_
#define __HCM4A_CalcFs4A_dtCalc_h_

#include "common.h"
#include "dtHcmDll.h"
#include "dtSingle.h"
#include "dtMult.h"
#include "dtTriger.h"


void TrigerCalc(Vector<TrigerRec>& calcTrigResult, const MultHcmPack& mPack, const AppCFG& appCFG){
	const int maxLen = 200;
	calcTrigResult.Clear();
	if(Hcm4AfsDll()){
		Progress pi(t_("Triger calculation ..."), mPack.txList.GetCount());
		for(auto v: mPack.txList){
			pi.Step();
			int error = 0;
			int lenght = 0;
			char ctryList[maxLen]; // для правильной работы HCM4AFS_ANNEX11(заполнения ctryList), объявить массив char ctryList[200] с max кол-вом элементов
			Hcm4AfsDll().HCM4AFS_ANNEX11(v.longitude, v.latitude, v.frequency, v.h_asl, appCFG.cfgPathRec.GetBorder(), ctryList, lenght, error);
			if(lenght){
				String countres = TrimBoth(ctryList);
				calcTrigResult.Add(TrigerRec(v, countres, error));
			}
			else{
				if(error)
					calcTrigResult.Add(TrigerRec(v, "", error));
			}
		}
		pi.Set(0, mPack.ptrList.GetCount());
		for(auto v: mPack.ptrList){
			pi.Step();
			int error = 0;
			int lenght = 0;
			char ctryList[maxLen]; // для правильной работы HCM4AFS_ANNEX11(заполнения ctryList), объявить массив char ctryList[200] с max кол-вом элементов
			Hcm4AfsDll().HCM4AFS_ANNEX11(v.prx.longitude, v.prx.latitude, v.prx.frequency, v.prx.h_asl, appCFG.cfgPathRec.GetBorder(), ctryList, lenght, error);
			if(lenght){
				String countres = TrimBoth(ctryList);
				calcTrigResult.Add(TrigerRec(v.prx, countres, error));
			}
			else{
				if(error)
					calcTrigResult.Add(TrigerRec(v.prx, "", error));
			}
		}
		pi.Set(0, mPack.ptrList.GetCount());
		for(auto v: mPack.ptrList){
			pi.Step();
			int error = 0;
			int lenght = 0;
			char ctryList[maxLen]; // для правильной работы HCM4AFS_ANNEX11(заполнения ctryList), объявить массив char ctryList[200] с max кол-вом элементов
			Hcm4AfsDll().HCM4AFS_ANNEX11(v.ptx.longitude, v.ptx.latitude, v.ptx.frequency, v.ptx.h_asl, appCFG.cfgPathRec.GetBorder(), ctryList, lenght, error);
			if(lenght){
				String countres = TrimBoth(ctryList);
				calcTrigResult.Add(TrigerRec(v.ptx, countres, error));
			}
			else{
				if(error)
					calcTrigResult.Add(TrigerRec(v.ptx, "", error));
			}
		}
		pi.Set(0, mPack.rxList.GetCount());
		for(auto v: mPack.rxList){
			pi.Step();
			int error = 0;
			int lenght = 0;
			char ctryList[maxLen]; // для правильной работы HCM4AFS_ANNEX11(заполнения ctryList), объявить массив char ctryList[200] с max кол-вом элементов
			Hcm4AfsDll().HCM4AFS_ANNEX11(v.longitude, v.latitude, v.frequency, v.h_asl, appCFG.cfgPathRec.GetBorder(), ctryList, lenght, error);
			if(lenght){
				String countres = TrimBoth(ctryList);
				calcTrigResult.Add(TrigerRec(v, countres, error));
			}
			else{
				if(error)
					calcTrigResult.Add(TrigerRec(v, "", error));
			}
		}
	}
	else{
		ErrorOK(DeQtf("Error load \"hcm4afs.dll\""));
	}
	Hcm4AfsDll().Free();
}
//!---------------------------------------------------------------------------------------------

void SingleCalc(TblSingleRec& calcSingleResult, const HcmPack& hcmPackRec, const CalcParamData& calcParam, const AppCFG& appCFG){
	calcSingleResult.Clear();
	DataCalc dataCalc;
	Terrain terr(appCFG.cfgPathRec.GetTopo(), appCFG.cfgPathRec.GetMorpho());
	if(Hcm4AfsDll()){
		if( hcmPackRec.GetMap().GetCount() == 2 &&
			hcmPackRec.GetMap()[0].GetEntity() == HcmEntity::TX &&
			hcmPackRec.GetMap()[1].GetEntity() == HcmEntity::RX){
			TStdAnt ant;
			HcmRec tx(ant, hcmPackRec.GetMap()[0]); HcmRecDll _tx(tx);
			HcmRec rx(ant, hcmPackRec.GetMap()[1]); HcmRecDll _rx(rx);
			if(tx.tError.IsOk() && rx.tError.IsOk()){
				//----------------
				// PATH TX->RX  |
				//----------------
				// Calculate distance and azimuth
				// Create profile
				terr.Initialise(tx.longitude, tx.latitude, rx.longitude, rx.latitude);
				if(terr.Error() !=0){
					RLOG("terr.Error() = " << terr.Error());
					ErrorOK(DeQtf("Error opening topo data file (no data)"));
					return;
				}
				dataCalc.distance_tx_rx = terr.Distance();	// Distance
				dataCalc.azimuth_tx_rx= terr.TxAzimuth();		// Tx azimuth to victim receiver
				dataCalc.azimuth_rx_tx= terr.RxAzimuth();		// Rx azimuth to interferer
				dataCalc.profile_tx_rx = terr.GetProfile();
				Hcm4AfsDll().HCM4AFS_A(dataCalc, _tx, _rx, calcParam.inCalcRec);
			}
			else{
				if(!tx.tError.IsOk()){
					dataCalc.error = tx.tError.Code();
					dataCalc.errorMsg = tx.tError.Message();
				}
				else{
					dataCalc.error = rx.tError.Code();
					dataCalc.errorMsg = rx.tError.Message();
				}
			}
			calcSingleResult.Set(dataCalc, tx, rx);
		}
		else if( hcmPackRec.GetMap().GetCount() == 4 &&
			hcmPackRec.GetMap()[0].GetEntity() == HcmEntity::TX &&
			hcmPackRec.GetMap()[1].GetEntity() == HcmEntity::PRX &&
			hcmPackRec.GetMap()[2].GetEntity() == HcmEntity::PTX &&
			hcmPackRec.GetMap()[3].GetEntity() == HcmEntity::RX){
			TStdAnt ant;
			HcmRec tx(ant, hcmPackRec.GetMap()[0]); HcmRecDll _tx(tx);
			HcmRec prx(ant, hcmPackRec.GetMap()[1]); HcmRecDll _prx(prx);
			HcmRec ptx(ant, hcmPackRec.GetMap()[2]); HcmRecDll _ptx(ptx);
			HcmRec rx(ant, hcmPackRec.GetMap()[3]); HcmRecDll _rx(rx);
			if(tx.tError.IsOk() && rx.tError.IsOk() && prx.tError.IsOk() && ptx.tError.IsOk()){
				//----------------
				// PATH TX->PRX  |
				//----------------
				// Calculate distances and azimuths
				// Create profile Tx -> PRX
				terr.Initialise(tx.longitude, tx.latitude, prx.longitude, prx.latitude);
				if(terr.Error() !=0){
					RLOG("terr.Error() = " << terr.Error());
					ErrorOK(DeQtf("Error opening topo data file (no data)"));
					return;
				}
				dataCalc.distance_tx_prx = terr.Distance();		// Distance from TX to PRX
				dataCalc.azimuth_tx_prx= terr.TxAzimuth();		// Tx azimuth to victim receiver
				dataCalc.azimuth_prx_tx= terr.RxAzimuth();		// Rx azimuth to interferer
				dataCalc.profile_tx_prx = terr.GetProfile();
				//----------------
				// PATH PTX->RX  |
				//----------------
				// Calculate distances and azimuths
				// Create profile PTX -> RX
				terr.Initialise(ptx.longitude, ptx.latitude, rx.longitude, rx.latitude);
				if(terr.Error() !=0){
					//ProcessErrors("terrainptxrx",terr.Error(),dataCalc.error,dataCalc.errorMsg);
					if (dataCalc.error != 0) return;
				}
				dataCalc.distance_ptx_rx = terr.Distance();		// Distance from PTX to RX
				dataCalc.azimuth_ptx_rx= terr.TxAzimuth();		// PTx azimuth to Rx
				dataCalc.azimuth_rx_ptx= terr.RxAzimuth();		// Rx azimuth to PTx
				dataCalc.profile_ptx_rx  = terr.GetProfile();

				Hcm4AfsDll().HCM4AFS_P(dataCalc, _tx, _prx, _ptx, _rx, calcParam.inCalcRec);
			}
			else{
				if(!tx.tError.IsOk()){
					dataCalc.error = tx.tError.Code();
					dataCalc.errorMsg = tx.tError.Message();
				}
				else if(!rx.tError.IsOk()){
					dataCalc.error = rx.tError.Code();
					dataCalc.errorMsg = rx.tError.Message();
				}
				else if(!prx.tError.IsOk()){
					dataCalc.error = prx.tError.Code();
					dataCalc.errorMsg = prx.tError.Message();
				}
				else{
					dataCalc.error = ptx.tError.Code();
					dataCalc.errorMsg = ptx.tError.Message();
				}
			}
			calcSingleResult.Set(dataCalc, tx, prx, ptx, rx);
		}
	}
	else{
		ErrorOK(DeQtf("Error load \"hcm4afs.dll\""));
	}
	Hcm4AfsDll().Free();
}
//!---------------------------------------------------------------------------------------------

void hcmfs(DataCalc& dataCalc, DuoType duoType, const HcmRec& tx, const HcmRec& rx, const CalcParamData& calcParam, const AppCFG& appCFG){
	HcmRecDll _tx(tx);
	HcmRecDll _rx(rx);
	Terrain terr(appCFG.cfgPathRec.GetTopo(), appCFG.cfgPathRec.GetMorpho());
	terr.Initialise(tx.longitude, tx.latitude, rx.longitude, rx.latitude);
	if(terr.Error() !=0){
		//ProcessErrors("terrainptxrx",terr.Error(),dataCalc.error,dataCalc.errorMsg);
		if (dataCalc.error != 0) return;
	}
	dataCalc.distance_tx_rx = terr.Distance();	// Distance
	dataCalc.azimuth_tx_rx= terr.TxAzimuth();		// Tx azimuth to victim receiver
	dataCalc.azimuth_rx_tx= terr.RxAzimuth();		// Rx azimuth to interferer
	dataCalc.profile_tx_rx  = terr.GetProfile();
	Hcm4AfsDll().HCM4AFS_A(dataCalc, _tx, _rx, calcParam.inCalcRec);
}

void hcmfs(DataCalc& dataCalc, TrioType trioType, const HcmRec& tx, const HcmRec& prx, const HcmRec& ptx, const HcmRec& rx, const CalcParamData& calcParam, const AppCFG& appCFG){
	HcmRecDll _tx(tx);
	HcmRecDll _prx(prx);
	HcmRecDll _ptx(ptx);
	HcmRecDll _rx(rx);
	Terrain terr(appCFG.cfgPathRec.GetTopo(), appCFG.cfgPathRec.GetMorpho());
	//----------------
	// PATH TX->PRX  |
	//----------------
	// Calculate distances and azimuths
	// Create profile Tx -> PRX
	terr.Initialise(tx.longitude, tx.latitude, prx.longitude, prx.latitude);
	if(terr.Error() !=0){
		//ProcessErrors("terraintxprx", terr.Error(), dataCalc.error, dataCalc.errorMsg);
		if(dataCalc.error != 0) return;
	}
	dataCalc.distance_tx_prx = terr.Distance();		// Distance from TX to PRX
	dataCalc.azimuth_tx_prx= terr.TxAzimuth();		// Tx azimuth to victim receiver
	dataCalc.azimuth_prx_tx= terr.RxAzimuth();		// Rx azimuth to interferer
	dataCalc.profile_tx_prx = terr.GetProfile();

	//----------------
	// PATH PTX->RX  |
	//----------------
	// Calculate distances and azimuths
	// Create profile PTX -> RX
	terr.Initialise(ptx.longitude, ptx.latitude, rx.longitude, rx.latitude);
	if(terr.Error() !=0){
		//ProcessErrors("terrainptxrx",terr.Error(),dataCalc.error,dataCalc.errorMsg);
		if (dataCalc.error != 0) return;
	}
	dataCalc.distance_ptx_rx = terr.Distance();		// Distance from PTX to RX
	dataCalc.azimuth_ptx_rx= terr.TxAzimuth();		// PTx azimuth to Rx
	dataCalc.azimuth_rx_ptx= terr.RxAzimuth();		// Rx azimuth to PTx
	dataCalc.profile_ptx_rx = terr.GetProfile();

	Hcm4AfsDll().HCM4AFS_P(dataCalc, _tx, _prx, _ptx, _rx, calcParam.inCalcRec);
}
//!---------------------------------------------------------------------------------------------

void CalcPTR(Vector<TblMultRec>& calcMultResult, const MultHcmPack& test, const MultHcmPack& ref, const CalcParamData& calcParam, const AppCFG& appCFG){
	// : PASSIVE CALCULATIONS - Test
	Progress pi(t_("Passive calculation ..."), test.ptrList.GetCount());
	for(int i = 0; i < test.ptrList.GetCount(); i++ ){
		pi.Step();
		for(int j = 0; j < ref.txList.GetCount(); j++ ){
			// 1: Ref TX -> Test PRx-PTx -> Ref RX
			for(int k = 0; k < ref.rxList.GetCount(); k++ ){
				if(fabs(ref.txList[j].frequency - ref.rxList[k].frequency) < appCFG.cfgCalcRec.maxDeltaFreqTxRx){
					double distPtxRX = GetDistance(test.ptrList[i].ptx.longitude, test.ptrList[i].ptx.latitude, ref.rxList[k].longitude, ref.rxList[k].latitude);
					double distTxRX = GetDistance(ref.txList[j].longitude, ref.txList[j].latitude, ref.rxList[k].longitude, ref.rxList[k].latitude);
				    if(distPtxRX > 0 && distTxRX > 0 && distPtxRX < appCFG.cfgCalcRec.maxDistPtxRX && distTxRX < appCFG.cfgCalcRec.maxDistTxRX){
						DataCalc dataCalc;
						if(test.GetError().IsOk() && ref.GetError().IsOk())
							hcmfs(dataCalc, TrioType::RTR, ref.txList[j], test.ptrList[i].prx, test.ptrList[i].ptx, ref.rxList[k], calcParam, appCFG);
						else{
							if(!test.GetError().IsOk()){
								dataCalc.error = test.GetError().Code();
								dataCalc.errorMsg = test.GetError().Message();
							}
							else{
								dataCalc.error = ref.GetError().Code();
								dataCalc.errorMsg = ref.GetError().Message();
							}
						}
						if(dataCalc.td >= calcParam.minTD || dataCalc.error != 0){
							if(dataCalc.I != HCM_NO_VALUE || calcParam.isShowUnCalc){
								TblMultRec tblMultRec;
								tblMultRec.Set(TrioType::RTR, dataCalc, ref.txList[j], test.ptrList[i].prx, test.ptrList[i].ptx, ref.rxList[k]);
								calcMultResult.Add(tblMultRec);
							}
						}
				    }
				}
			}
			// 2: Ref TX -> Test PTx-PRx -> Test RX
			for(int k = 0; k < test.rxList.GetCount(); k++ ){
				if(fabs(ref.txList[j].frequency - test.rxList[k].frequency) < appCFG.cfgCalcRec.maxDeltaFreqTxRx){
					double distPtxRX = GetDistance(test.ptrList[i].ptx.longitude, test.ptrList[i].ptx.latitude, test.rxList[k].longitude, test.rxList[k].latitude);
					double distTxRX  = GetDistance(ref.txList[j].longitude, ref.txList[j].latitude, test.rxList[k].longitude, test.rxList[k].latitude);
					if(distPtxRX > 0 && distTxRX > 0 && distPtxRX < appCFG.cfgCalcRec.maxDistPtxRX && distTxRX < appCFG.cfgCalcRec.maxDistTxRX){
						DataCalc dataCalc;
						if(test.GetError().IsOk() && ref.GetError().IsOk())
							hcmfs(dataCalc, TrioType::RTT, ref.txList[j], test.ptrList[i].prx, test.ptrList[i].ptx, test.rxList[k], calcParam, appCFG);
						else{
							if(!test.GetError().IsOk()){
								dataCalc.error = test.GetError().Code();
								dataCalc.errorMsg = test.GetError().Message();
							}
							else{
								dataCalc.error = ref.GetError().Code();
								dataCalc.errorMsg = ref.GetError().Message();
							}
						}
						if(dataCalc.td >= calcParam.minTD || dataCalc.error != 0){
							if(dataCalc.I != HCM_NO_VALUE || calcParam.isShowUnCalc){
								TblMultRec tblMultRec;
								tblMultRec.Set(TrioType::RTT, dataCalc, ref.txList[j], test.ptrList[i].prx, test.ptrList[i].ptx, test.rxList[k]);
								calcMultResult.Add(tblMultRec);
							}
						}
					}
				}
			}
		}
		for(int j = 0; j < test.txList.GetCount(); j++ ){
			// 3: Test TX -> Test PTx-PRx -> Ref RX
			for(int k = 0; k < ref.rxList.GetCount(); k++ ){
				if(fabs(test.txList[j].frequency - ref.rxList[k].frequency) < appCFG.cfgCalcRec.maxDeltaFreqTxRx){
					double distPtxRX = GetDistance(test.ptrList[i].ptx.longitude, test.ptrList[i].ptx.latitude, ref.rxList[k].longitude, ref.rxList[k].latitude);
					double distTxRX = GetDistance(test.txList[j].longitude, test.txList[j].latitude, ref.rxList[k].longitude, ref.rxList[k].latitude);
					if(distPtxRX > 0 && distTxRX > 0 && distPtxRX < appCFG.cfgCalcRec.maxDistPtxRX && distTxRX < appCFG.cfgCalcRec.maxDistTxRX){
						DataCalc dataCalc;
						if(test.GetError().IsOk() && ref.GetError().IsOk())
							hcmfs(dataCalc, TrioType::TTR, test.txList[j], test.ptrList[i].prx, test.ptrList[i].ptx, ref.rxList[k], calcParam, appCFG);
						else{
							if(!test.GetError().IsOk()){
								dataCalc.error = test.GetError().Code();
								dataCalc.errorMsg = test.GetError().Message();
							}
							else{
								dataCalc.error = ref.GetError().Code();
								dataCalc.errorMsg = ref.GetError().Message();
							}
						}
						if(dataCalc.td >= calcParam.minTD || dataCalc.error != 0){
							if(dataCalc.I != HCM_NO_VALUE || calcParam.isShowUnCalc){
								TblMultRec tblMultRec;
								tblMultRec.Set(TrioType::TTR, dataCalc, test.txList[j], test.ptrList[i].prx, test.ptrList[i].ptx, ref.rxList[k]);
								calcMultResult.Add(tblMultRec);
							}
						}
					}
				}
			}
		}
	}
}
//!---------------------------------------------------------------------------------------------

void CalcTx(Vector<TblMultRec>& calcMultResult, const MultHcmPack& test, const MultHcmPack& ref, const CalcParamData& calcParam, const AppCFG& appCFG){
	// : TX CALCULATIONS - Test
	Progress pi(t_("TX calculation ..."), test.txList.GetCount());
	for(int i = 0; i < test.txList.GetCount(); i++){
		pi.Step();
		// 1: Test TX -> Ref RX
		for(int j = 0; j < ref.rxList.GetCount(); j++ ){
			if(fabs(test.txList[i].frequency - ref.rxList[j].frequency) < appCFG.cfgCalcRec.maxDeltaFreqTxRx ){
				double distTxRX = GetDistance(test.txList[i].longitude, test.txList[i].latitude, ref.rxList[j].longitude, ref.rxList[j].latitude);
			    if(distTxRX > 0 && distTxRX < appCFG.cfgCalcRec.maxDistTxRX){
					DataCalc dataCalc;
					if(test.GetError().IsOk() && ref.GetError().IsOk())
						hcmfs(dataCalc, DuoType::TR, test.txList[i], ref.rxList[j], calcParam, appCFG);
					else{
						if(!test.GetError().IsOk()){
							dataCalc.error = test.GetError().Code();
							dataCalc.errorMsg = test.GetError().Message();
						}
						else{
							dataCalc.error = ref.GetError().Code();
							dataCalc.errorMsg = ref.GetError().Message();
						}
					}
					if(dataCalc.td >= calcParam.minTD || dataCalc.error != 0){
						if(dataCalc.I != HCM_NO_VALUE || calcParam.isShowUnCalc){
							TblMultRec tblMultRec;
							tblMultRec.Set(DuoType::TR, dataCalc, test.txList[i], ref.rxList[j]);
							calcMultResult.Add(tblMultRec);
						}
					}
			    }
			}
		}
		// 2: Test TX -> Ref PRX-PTX -> Ref RX
		for(int j = 0; j < ref.ptrList.GetCount(); j++ ){
			for(int k = 0; k < ref.rxList.GetCount(); k++ ){
				if(fabs(test.txList[i].frequency - ref.rxList[k].frequency) < appCFG.cfgCalcRec.maxDeltaFreqTxRx){
					double distPtxRX = GetDistance(ref.ptrList[j].ptx.longitude, ref.ptrList[j].ptx.latitude, ref.rxList[k].longitude, ref.rxList[k].latitude);
					double distTxRX = GetDistance(test.txList[i].longitude, test.txList[i].latitude, ref.rxList[k].longitude, ref.rxList[k].latitude);
					if(distPtxRX > 0 && distTxRX > 0 && distPtxRX < appCFG.cfgCalcRec.maxDistPtxRX && distTxRX < appCFG.cfgCalcRec.maxDistTxRX){
						DataCalc dataCalc;
						if(test.GetError().IsOk() && ref.GetError().IsOk())
							hcmfs(dataCalc, TrioType::TRR, test.txList[i], ref.ptrList[j].prx, ref.ptrList[j].ptx, ref.rxList[k], calcParam, appCFG);
						else{
							if(!test.GetError().IsOk()){
								dataCalc.error = test.GetError().Code();
								dataCalc.errorMsg = test.GetError().Message();
							}
							else{
								dataCalc.error = ref.GetError().Code();
								dataCalc.errorMsg = ref.GetError().Message();
							}
						}
						if(dataCalc.td >= calcParam.minTD || dataCalc.error != 0){
							if(dataCalc.I != HCM_NO_VALUE || calcParam.isShowUnCalc){
								TblMultRec tblMultRec;
								tblMultRec.Set(TrioType::TRR, dataCalc, test.txList[i], ref.ptrList[j].prx, ref.ptrList[j].ptx, ref.rxList[k]);
								calcMultResult.Add(tblMultRec);
							}
						}
					}
				}
			}
		}
		// 3: Test Tx - Ref PRx-Ref PTx - Test Rx
		for(int j = 0; j < ref.ptrList.GetCount(); j++ ){
			for(int k = 0; k < test.rxList.GetCount(); k++ ){
				if(fabs(test.txList[i].frequency - test.rxList[k].frequency) < appCFG.cfgCalcRec.maxDeltaFreqTxRx){
					double distPtxRX = GetDistance(ref.ptrList[j].ptx.longitude, ref.ptrList[j].ptx.latitude, test.rxList[k].longitude, test.rxList[k].latitude);
					double distTxRX = GetDistance(test.txList[i].longitude, test.txList[i].latitude, test.rxList[k].longitude, test.rxList[k].latitude);
					if(distPtxRX > 0 && distTxRX > 0 && distPtxRX < appCFG.cfgCalcRec.maxDistPtxRX && distTxRX < appCFG.cfgCalcRec.maxDistTxRX){
						DataCalc dataCalc;
						if(test.GetError().IsOk() && ref.GetError().IsOk())
							hcmfs(dataCalc, TrioType::TRT, test.txList[i], ref.ptrList[j].prx, ref.ptrList[j].ptx, test.rxList[k], calcParam, appCFG);
						else{
							if(!test.GetError().IsOk()){
								dataCalc.error = test.GetError().Code();
								dataCalc.errorMsg = test.GetError().Message();
							}
							else{
								dataCalc.error = ref.GetError().Code();
								dataCalc.errorMsg = ref.GetError().Message();
							}
						}
						if(dataCalc.td >= calcParam.minTD || dataCalc.error != 0){
							if(dataCalc.I != HCM_NO_VALUE || calcParam.isShowUnCalc){
								TblMultRec tblMultRec;
								tblMultRec.Set(TrioType::TRT, dataCalc, test.txList[i], ref.ptrList[j].prx, ref.ptrList[j].ptx, test.rxList[k]);
								calcMultResult.Add(tblMultRec);
							}
						}
					}
				}
			}

		}
	}
}
//!---------------------------------------------------------------------------------------------

void CalcRx(Vector<TblMultRec>& calcMultResult, const MultHcmPack& test, const MultHcmPack& ref, const CalcParamData& calcParam, const AppCFG& appCFG){
	// : RX CALCULATIONS - Test
	Progress pi(t_("RX calculation ..."), test.rxList.GetCount());
	for(int i = 0; i < test.rxList.GetCount(); i++){
		pi.Step();
		// 1: Ref TX -> Test RX
		for(int j = 0; j < ref.txList.GetCount(); j++ ){
			if(fabs(ref.txList[j].frequency - test.rxList[i].frequency) < appCFG.cfgCalcRec.maxDeltaFreqTxRx){
				double distTxRX = GetDistance(ref.txList[j].longitude, ref.txList[j].latitude, test.rxList[i].longitude, test.rxList[i].latitude);
				if(distTxRX > 0 && distTxRX < appCFG.cfgCalcRec.maxDistTxRX ){
					DataCalc dataCalc;
					if(test.GetError().IsOk() && ref.GetError().IsOk())
						hcmfs(dataCalc, DuoType::RT, ref.txList[j], test.rxList[i], calcParam, appCFG);
					else{
						if(!test.GetError().IsOk()){
							dataCalc.error = test.GetError().Code();
							dataCalc.errorMsg = test.GetError().Message();
						}
						else{
							dataCalc.error = ref.GetError().Code();
							dataCalc.errorMsg = ref.GetError().Message();
						}
					}
					if(dataCalc.td >= calcParam.minTD || dataCalc.error != 0){
						if(dataCalc.I != HCM_NO_VALUE || calcParam.isShowUnCalc){
							TblMultRec tblMultRec;
							tblMultRec.Set(DuoType::RT, dataCalc, ref.txList[j], test.rxList[i]);
							calcMultResult.Add(tblMultRec);
						}
					}
				}
			}
		}
		// 2: Ref TX -> Ref PRX-PTX -> Test RX
		for(int j = 0; j < ref.ptrList.GetCount(); j++ ){
			for(int k = 0; k < ref.txList.GetCount(); k++ ){
				if(fabs(ref.txList[k].frequency - test.rxList[i].frequency) < appCFG.cfgCalcRec.maxDeltaFreqTxRx){
					double distPtxRX = GetDistance(ref.ptrList[j].ptx.longitude, ref.ptrList[j].ptx.latitude, test.rxList[i].longitude, test.rxList[i].latitude);
					double distTxRX = GetDistance(ref.txList[k].longitude, ref.txList[k].latitude, test.rxList[i].longitude, test.rxList[i].latitude);
					if(distPtxRX > 0 && distTxRX > 0 && distPtxRX < appCFG.cfgCalcRec.maxDistPtxRX && distTxRX < appCFG.cfgCalcRec.maxDistTxRX){
						DataCalc dataCalc;
						if(test.GetError().IsOk() && ref.GetError().IsOk())
							hcmfs(dataCalc, TrioType::RRT, ref.txList[k], ref.ptrList[j].prx, ref.ptrList[j].ptx, test.rxList[i], calcParam, appCFG);
						else{
							if(!test.GetError().IsOk()){
								dataCalc.error = test.GetError().Code();
								dataCalc.errorMsg = test.GetError().Message();
							}
							else{
								dataCalc.error = ref.GetError().Code();
								dataCalc.errorMsg = ref.GetError().Message();
							}
						}
						if(dataCalc.td >= calcParam.minTD || dataCalc.error != 0){
							if(dataCalc.I != HCM_NO_VALUE || calcParam.isShowUnCalc){
								TblMultRec tblMultRec;
								tblMultRec.Set(TrioType::RRT, dataCalc, ref.txList[k], ref.ptrList[j].prx, ref.ptrList[j].ptx, test.rxList[i]);
								calcMultResult.Add(tblMultRec);
							}
						}
					}
				}
			}
		}
	}
}
//!---------------------------------------------------------------------------------------------

#endif
