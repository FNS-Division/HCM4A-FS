#include <iostream>
#include "hcm4afs.h"

//! --------------------------------------------------------------------------------------------
#define STR2(x) #x
#define STR(x) STR2(x)
#ifdef _WIN32
#define INCBIN_SECTION ".rdata, \"dr\""
#else
#define INCBIN_SECTION ".rodata"
#endif
#define INCBIN(name, file) \
    __asm__(".section " INCBIN_SECTION "\n" \
            ".global incbin_" STR(name) "_start\n" \
            ".balign 16\n" \
            "incbin_" STR(name) "_start:\n" \
            ".incbin \"" file "\"\n" \
            \
            ".global incbin_" STR(name) "_end\n" \
            ".balign 1\n" \
            "incbin_" STR(name) "_end:\n" \
            ".byte 0\n" \
    ); \
    extern __attribute__((aligned(16))) const char incbin_ ## name ## _start[]; \
    extern                              const char incbin_ ## name ## _end[]
 INCBIN(DN50, "DN50.TXT");
 INCBIN(N050, "N050.TXT");
//! --------------------------------------------------------------------------------------------

MapDnN0 mapDnN0 = MapDnN0((char*)incbin_DN50_start, (char*)incbin_DN50_end,
					   (char*)incbin_N050_start, (char*)incbin_N050_end);
//! --------------------------------------------------------------------------------------------

void HCM4AFS(double& I_O_AREA, double& I_NFD, double& I_MD,
			 double& I_I, double& I_A_TOT, double& I_TD, int& I_Error,
			 const char* I_Record1,
			 const char* I_Record2,
			 const char* I_Record3,
			 const char* I_Record4,
			 const char* I_DLL_textfile,
			 const char* I_TopoPath,
			 const char* I_MorphoPath,
			 int I_Plz, double I_TimePercentage, double I_Press, double I_Temp){

	string path_topo = "";
	if(I_TopoPath != NULL) path_topo = I_TopoPath;
	string path_morpho = "";
	if(I_MorphoPath != NULL) path_morpho = I_MorphoPath;
	string rec1 = "";
	if(I_Record1 != NULL) rec1 = I_Record1;
	string rec2 = "";
	if(I_Record2 != NULL) rec2 = I_Record2;
	string rec3 = "";
	if(I_Record3 != NULL) rec3 = I_Record3;
	string rec4 = "";
	if(I_Record4 != NULL) rec4 = I_Record4;
	string fileReport = "";
	if(I_DLL_textfile != NULL) fileReport = I_DLL_textfile;

	DataCalc dataCalc;
	Hcm4AfsCalc(mapDnN0, rec1, rec2, rec3, rec4, path_topo, path_morpho, fileReport, I_Plz, I_TimePercentage, I_Press, I_Temp, dataCalc);

	I_O_AREA = dataCalc.overlap_area;
	I_NFD = dataCalc.nfd;
	I_MD = dataCalc.md;
	I_I = dataCalc.I;
	I_A_TOT = dataCalc.a_tot;
	I_TD  = dataCalc.td;
	I_Error = dataCalc.error;
}
//!---------------------------------------------------------------------------------------

void HCM4AFS_A(DataCalc& dataCalc,
			   const HcmRec& tx,
			   const HcmRec& rx,
			   const InCalcRec& inRec){
	Hcm4AfsCalcA(mapDnN0, tx, rx, inRec, dataCalc);
}
//!---------------------------------------------------------------------------------------

void HCM4AFS_P(DataCalc& dataCalc,
			   const HcmRec& tx,
			   const HcmRec& prx,
			   const HcmRec& ptx,
			   const HcmRec& rx,
			   const InCalcRec& inRec){
	Hcm4AfsCalcP(mapDnN0, tx, prx, ptx, rx, inRec, dataCalc);
}
//!---------------------------------------------------------------------------------------

void HCM4AFS_ANNEX11(const double longitude, const double latitude, const double frequency_GHz,
					 const int h_asl, const char* border_path,
					 char* outcome, int& outlen, int& error){
	// for HCM4AFS_ANNEX 11 to work correctly (filling in outcome),
	// it is enough to declare an array char outcome[MAX] with MAX element.
	InTrigRec inRec(longitude, latitude, frequency_GHz, h_asl, border_path);
	OutTrigRec outRec = GetTriggerCoutry(inRec);
	error = outRec.error;
	memset(outcome, ' ', outRec.ctryList.length());
	strncpy(outcome, outRec.ctryList.c_str(), outRec.ctryList.length());
	outlen = outRec.ctryList.length();
}
//!---------------------------------------------------------------------------------------





