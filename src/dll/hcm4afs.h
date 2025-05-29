#ifndef _hcm4afs_hcm4afs_h
#define _hcm4afs_hcm4afs_h

#include "hcmfs.h"
#include "annex11.h"

#if defined(_WIN32)
  #define LIBRARY_API __declspec(dllexport)
#else
  #define LIBRARY_API
#endif

#define DllImport extern "C" __declspec(dllimport)
#define DllExport extern "C" __declspec(dllexport)

#ifdef flagDLL
	#define DLLIMPORT __declspec(dllexport)
#else
	#define DLLIMPORT __declspec(dllimport)
#endif


extern "C" void LIBRARY_API  HCM4AFS(double& I_O_AREA, double& I_NFD, double& I_MD,
									 double& I_I, double& I_A_TOT, double& I_TD, int& I_Error,
									 const char* I_Record1,
									 const char* I_Record2,
									 const char* I_Record3,
									 const char* I_Record4,
									 const char* I_DLL_textfile,
									 const char* I_TopoPath,
									 const char* I_MorphoPath,
									 int I_Plz, double I_TimePercentage, double I_Press, double I_Temp);

extern "C" void LIBRARY_API  HCM4AFS_A(DataCalc& dataCalc,
									  const HcmRec& tx,
									  const HcmRec& rx,
									  const InCalcRec& inRec);

extern "C" void LIBRARY_API  HCM4AFS_P(DataCalc& dataCalc,
									  const HcmRec& tx,
									  const HcmRec& prx,
									  const HcmRec& ptx,
									  const HcmRec& rx,
									  const InCalcRec& inRec);

//!---------------------------------------------------------------------------------------------

extern "C" void LIBRARY_API  HCM4AFS_ANNEX11(const double longitude, const double latitude, const double frequency_GHz,
											 const int h_asl, const char* border_path,
											 char* outcome, int& outlen, int& error);

//!---------------------------------------------------------------------------------------------






#endif



