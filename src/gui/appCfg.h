#ifndef __HCM4A_CalcFS4A_appCfg_h_
#define __HCM4A_CalcFS4A_appCfg_h_

#include "iniCfg.h"

struct CalcParamData: Moveable<CalcParamData>{
	InCalcRec inCalcRec;
	double minTD;			// tdThreshold = 1.0;
	bool isShowUnCalc;
	bool isInputDN;
	void Init(){
		inCalcRec.OnDefault();
		minTD = 0.0;
		isShowUnCalc = false;
		isInputDN = false;
	}
	void ToINI(IniCfg& ini){
		ini.SetValue("plz", IntStr(inCalcRec.plz));
		ini.SetValue("time_percentage", DblStr(inCalcRec.time_percentage));
		ini.SetValue("press", DblStr(inCalcRec.press));
		ini.SetValue("temp", DblStr(inCalcRec.temp));
		ini.SetValue("dN", DblStr(inCalcRec.dN));
		ini.SetValue("n0", DblStr(inCalcRec.n0));
		ini.SetValue("isInputDN", IntStr(isInputDN));
		ini.SetValue("minTD", DblStr(minTD));
		ini.SetValue("isShowUnCalc", IntStr(isShowUnCalc));
	}
	void FromINI(const IniCfg& ini){
		Init();
		Value v;
		v = StrInt(ini.GetValue("plz"));
		if(!v.IsNull()) inCalcRec.plz = v;
		v = StrDbl(ini.GetValue("time_percentage"));
		if(!v.IsNull()) inCalcRec.time_percentage = v;
		v = StrDbl(ini.GetValue("press"));
		if(!v.IsNull()) inCalcRec.press = v;
		v = StrDbl(ini.GetValue("temp"));
		if(!v.IsNull()) inCalcRec.temp = v;
		v = StrDbl(ini.GetValue("dN"));
		if(!v.IsNull()) inCalcRec.dN = v;
		v = StrDbl(ini.GetValue("n0"));
		if(!v.IsNull()) inCalcRec.n0 = v;
		v = StrInt(ini.GetValue("isInputDN"));
		if(!v.IsNull()) isInputDN = v;
		v = StrDbl(ini.GetValue("minTD"));
		if(!v.IsNull()) minTD = v;
		v = StrInt(ini.GetValue("isShowUnCalc"));
		if(!v.IsNull()) isShowUnCalc = v;
	}
	bool IsEmpty(){
		return inCalcRec.IsEmpty();
	}
	void Load(){
		ini.Load();
		FromINI(ini);
	}
	void Save(){
		ToINI(ini);
		ini.Save();
	}
	CalcParamData(){Init();}
protected:
	IniCfg ini;
};
//!---------------------------------------------------------------------------------------------

struct CfgCalcRec: Moveable<CfgCalcRec>{
	double maxDeltaFreqTxRx;// Frequency_TXRX = 300.0
	double maxDistTxRX;		//
	double maxDistPtxRX;	// Distance_PTXRX = 2.0
	String fPackTest;
	String fPackRef;
	void Init(){
		maxDeltaFreqTxRx = 300.0;
		maxDistTxRX = 1000.0;
		maxDistPtxRX = 2.0;
		fPackTest = "";
		fPackRef = "";
	}
	void ToINI(IniCfg& ini){
		ini.SetValue("maxDeltaFreqTxRx", DblStr(maxDeltaFreqTxRx));
		ini.SetValue("maxDistTxRX", DblStr(maxDistTxRX));
		ini.SetValue("maxDistPtxRX", DblStr(maxDistPtxRX));
		ini.SetValue("fPackTest", fPackTest);
		ini.SetValue("fPackRef", fPackRef);
	}
	void FromINI(const IniCfg& ini){
		Init();
		Value v;
		v = StrDbl(ini.GetValue("maxDeltaFreqTxRx"));
		if(!v.IsNull()) maxDeltaFreqTxRx = v;
		v = StrDbl(ini.GetValue("maxDistTxRX"));
		if(!v.IsNull()) maxDistTxRX = v;
		v = StrDbl(ini.GetValue("maxDistPtxRX"));
		if(!v.IsNull()) maxDistPtxRX = v;
		fPackTest = ini.GetValue("fPackTest");
		fPackRef = ini.GetValue("fPackRef");
	}
	CfgCalcRec(){Init();}
};
//!---------------------------------------------------------------------------------------------

struct CfgPathRec: Moveable<CfgPathRec>{
	String GetTopo()const{return topo_path;}
	String GetMorpho()const{return morpho_path;}
	String GetBorder()const{return border_path;}
	String GetWorkDir()const{return work_dir;}
	void SetTopo(const String& str){ topo_path = str;}
	void SetMorpho(const String& str){ morpho_path = str;}
	void SetBorder(const String& str){ border_path = str;}
	void SetWorkDir(const String& str){ work_dir = str;}
	void Clear(){
		topo_path = "";
		morpho_path = "";
		border_path = "";
		work_dir = "";
	}
	void Init(){
		topo_path   = NativePath(GetHomeDirectory() << "/_MAP/TOPO");
		morpho_path = NativePath(GetHomeDirectory() << "/_MAP/MORPHO");
		border_path = NativePath(GetHomeDirectory() << "/_MAP/BORDER");
		work_dir	= GetHomeDirectory();
	}
	void ToINI(IniCfg& ini){
		ini.SetValue("topo_path", topo_path);
		ini.SetValue("morpho_path", morpho_path);
		ini.SetValue("border_path", border_path);
		ini.SetValue("work_dir", work_dir);
	}
	void FromINI(const IniCfg& ini){
		topo_path = ini.GetValue("topo_path");
		morpho_path = ini.GetValue("morpho_path");
		border_path = ini.GetValue("border_path");
		work_dir = ini.GetValue("work_dir");
	}
	String ToString()const{
		return Format("\nTOPO: %s\nMORPHO: %s\n BORDER: %s\n", topo_path, morpho_path, border_path);
	}
	CfgPathRec(){Clear();}
	CfgPathRec(const CfgPathRec& сfgPath):topo_path(сfgPath.topo_path), morpho_path(сfgPath.morpho_path), border_path(сfgPath.border_path){}
protected:
	String topo_path;
	String morpho_path;
	String border_path;
	String work_dir;
};
//!---------------------------------------------------------------------------------------------

struct CfgEditRec: Moveable<CfgEditRec>{
	String GetLstCountriesName()const{return lstCountriesName.IsEmpty() ? "Countries" : lstCountriesName;}
	void SetLstCountriesName(const String& str){ lstCountriesName = str;}
	String GetLstCategoryOfUseName()const{return lstCategoryOfUseName.IsEmpty() ? "CategoryOfUse" : lstCategoryOfUseName;}
	void SetLstCategoryOfUseName(const String& str){ lstCategoryOfUseName = str;}
	String GetLstFreqCategoryName()const{return lstFreqCategoryName.IsEmpty() ? "FreqCategory" : lstFreqCategoryName;}
	void SetLstFreqCategoryName(const String& str){ lstFreqCategoryName = str;}
	String GetLstMaxCapacitLinkName()const{return lstMaxCapacitLinkName.IsEmpty() ? "MaxCapacitLink" : lstMaxCapacitLinkName;}
	void SetLstMaxCapacitLinkName(const String& str){ lstMaxCapacitLinkName = str;}
	String GetLstStatusOfCooName()const{return lstStatusOfCooName.IsEmpty() ? "StatusOfCoo" : lstStatusOfCooName;}
	void SetLstStatusOfCooName(const String& str){ lstStatusOfCooName = str;}

	bool isInControl;
	void Init(){
		isInControl = true;
		lstCountriesName = "Countries";
		lstCategoryOfUseName = "CategoryOfUse";
		lstFreqCategoryName = "FreqCategory";
		lstMaxCapacitLinkName = "MaxCapacitLink";
		lstStatusOfCooName = "StatusOfCoo";
	}
	void ToINI(IniCfg& ini){
		ini.SetValue("isInControl", IntStr(isInControl));
		ini.SetValue("Countries", lstCountriesName);
		ini.SetValue("CategoryOfUse", lstCategoryOfUseName);
		ini.SetValue("FreqCategory", lstFreqCategoryName);
		ini.SetValue("MaxCapacitLink", lstMaxCapacitLinkName);
		ini.SetValue("StatusOfCoo", lstStatusOfCooName);
	}
	void FromINI(const IniCfg& ini){
		Init();
		Value v;
		v = StrInt(ini.GetValue("isInControl"));
		if(!v.IsNull()) isInControl = v;
		lstCountriesName = ini.GetValue("Countries");
		lstCategoryOfUseName = ini.GetValue("CategoryOfUse");
		lstFreqCategoryName = ini.GetValue("FreqCategory");
		lstMaxCapacitLinkName = ini.GetValue("MaxCapacitLink");
		lstStatusOfCooName = ini.GetValue("StatusOfCoo");
	}
	CfgEditRec(){Init();}
protected:
	String lstCountriesName;
	String lstCategoryOfUseName;
	String lstFreqCategoryName;
	String lstMaxCapacitLinkName;
	String lstStatusOfCooName;
};
//!---------------------------------------------------------------------------------------------


struct AppCFG{
	CfgPathRec cfgPathRec;
	CfgCalcRec cfgCalcRec;
	CfgEditRec cfgEditRec;
	void Load();
	void Save();
	AppCFG();
protected:
	IniCfg ini;
};
AppCFG::AppCFG(){
	cfgPathRec.Init();
	cfgCalcRec.Init();
	cfgEditRec.Init();
}
void AppCFG::Load(){
	ini.Load();
	cfgPathRec.FromINI(ini);
	cfgCalcRec.FromINI(ini);
	cfgEditRec.FromINI(ini);
}
void AppCFG::Save(){
	cfgPathRec.ToINI(ini);
	cfgCalcRec.ToINI(ini);
	cfgEditRec.ToINI(ini);
	ini.Save();
}
//!---------------------------------------------------------------------------------------------

struct CfgPathCtrl{
	typedef CfgPathCtrl CLASSNAME;
	WithLayCfgPath<ParentCtrl> layCfgPath;	// layout to place the control
	void ToCtrl(const CfgPathRec& cfgPathRec);
	void FromCtrl(CfgPathRec& cfgPathRec);
	void SetTopo();
	void SetMorpho();
	void SetBorder();
	CfgPathCtrl();
protected:
	String lastDir;
	String SelDir(const String& name);
};
CfgPathCtrl::CfgPathCtrl(){
	CtrlLayout(layCfgPath);
	lastDir = "";
	layCfgPath.btn_Topo <<= THISBACK(SetTopo);
	layCfgPath.btn_Morpho <<= THISBACK(SetMorpho);
	layCfgPath.btn_Border <<= THISBACK(SetBorder);
}
void CfgPathCtrl::ToCtrl(const CfgPathRec& cfgPathRec){
	layCfgPath.ed_Topo.SetData(cfgPathRec.GetTopo());
	layCfgPath.ed_Morpho.SetData(cfgPathRec.GetMorpho());
	layCfgPath.ed_Border.SetData(cfgPathRec.GetBorder());
}
void CfgPathCtrl::FromCtrl(CfgPathRec& cfgPathRec){
	cfgPathRec.SetTopo(layCfgPath.ed_Topo.GetData());
	cfgPathRec.SetMorpho(layCfgPath.ed_Morpho.GetData());
	cfgPathRec.SetBorder(layCfgPath.ed_Border.GetData());
}
String CfgPathCtrl::SelDir(const String& name){
	String dir = "";
	if(!lastDir.IsEmpty())
		dir = lastDir;
	else
		dir = GetExeFolder();
	FileSel fs;
	fs.ReadOnlyOption(true);
	fs.MkDirOption(false);
	fs.ActiveDir(dir);
	if(fs.ExecuteSelectDir(Format("Select %s directory", name))){
		lastDir = fs;
		return fs;
	}
	return dir;
}
void CfgPathCtrl::SetTopo(){
	layCfgPath.ed_Topo.SetData(SelDir("Topo"));
}
void CfgPathCtrl::SetMorpho(){
	layCfgPath.ed_Morpho.SetData(SelDir("Morpho"));
}
void CfgPathCtrl::SetBorder(){
	layCfgPath.ed_Border.SetData(SelDir("Border"));
}
//!---------------------------------------------------------------------------------------------

struct CfgCalcCtrl{
	typedef CfgCalcCtrl CLASSNAME;
	WithLayCfgCalc<ParentCtrl> layCfgCalc;	// layout to place the control
	void ToCtrl(const CfgCalcRec& cfgCalcRec);
	void FromCtrl(CfgCalcRec& cfgCalcRec);
	CfgCalcCtrl();
};
CfgCalcCtrl::CfgCalcCtrl(){
	CtrlLayout(layCfgCalc);
}
void CfgCalcCtrl::ToCtrl(const CfgCalcRec& cfgCalcRec){
	layCfgCalc.ed_FreqDif.SetData(cfgCalcRec.maxDeltaFreqTxRx);
	layCfgCalc.ed_DistTxRx.SetData(cfgCalcRec.maxDistTxRX);
	layCfgCalc.ed_DistPTxRx.SetData(cfgCalcRec.maxDistPtxRX);
}
void CfgCalcCtrl::FromCtrl(CfgCalcRec& cfgCalcRec){
	cfgCalcRec.maxDeltaFreqTxRx = layCfgCalc.ed_FreqDif.GetData();
	cfgCalcRec.maxDistTxRX = layCfgCalc.ed_DistTxRx.GetData();
	cfgCalcRec.maxDistPtxRX = layCfgCalc.ed_DistPTxRx.GetData();
}
//!---------------------------------------------------------------------------------------------

struct CfgEditCtrl{
	typedef CfgEditCtrl CLASSNAME;
	WithLayCfgEdit<ParentCtrl> layCfgEdit;	// layout to place the control
	void ToCtrl(const CfgEditRec& cfgEditRec);
	void FromCtrl(CfgEditRec& cfgEditRec);
	CfgEditCtrl();
};
CfgEditCtrl::CfgEditCtrl(){
	CtrlLayout(layCfgEdit);
}
void CfgEditCtrl::ToCtrl(const CfgEditRec& cfgEditRec){
	layCfgEdit.op_InControl = cfgEditRec.isInControl;
}
void CfgEditCtrl::FromCtrl(CfgEditRec& cfgEditRec){
	cfgEditRec.isInControl = layCfgEdit.op_InControl;
}
//!---------------------------------------------------------------------------------------------


class CfgDlg : public WithFrmCfg<TopWindow> {
public:
	typedef CfgDlg CLASSNAME;
	void Serialize(Stream& s);
	void ToCtrl();
	void FromCtrl();
	void Save();
	void Load();
	CfgDlg(AppCFG& appCFG);
protected:
	CfgPathCtrl ctrlCfgPath;
	CfgCalcCtrl ctrlCfgCalc;
	CfgEditCtrl ctrlCfgEdit;
	AppCFG& appCFG;
};
CfgDlg::CfgDlg(AppCFG& appCFG):appCFG(appCFG){
	CtrlLayoutOKCancel(*this, "Setup");
	Sizeable().Zoomable();
	tab.Add(ctrlCfgPath.layCfgPath.SizePos(), t_("\1[ [ [3*!Arial! Paths]]]"));
	tab.Add(ctrlCfgCalc.layCfgCalc.SizePos(), t_("\1[ [ [3*!Arial! Calculation]]]"));
	tab.Add(ctrlCfgEdit.layCfgEdit.SizePos(), t_("\1[ [ [3*!Arial! Edit]]]"));
	Load();
}
void CfgDlg::Serialize(Stream& s){
	int version = 1;
	s / version;
	SerializePlacement(s);
}
void CfgDlg::ToCtrl(){
	ctrlCfgPath.ToCtrl(appCFG.cfgPathRec);
	ctrlCfgCalc.ToCtrl(appCFG.cfgCalcRec);
	ctrlCfgEdit.ToCtrl(appCFG.cfgEditRec);
}
void CfgDlg::FromCtrl(){
	ctrlCfgPath.FromCtrl(appCFG.cfgPathRec);
	ctrlCfgCalc.FromCtrl(appCFG.cfgCalcRec);
	ctrlCfgEdit.FromCtrl(appCFG.cfgEditRec);
}
void CfgDlg::Load(){
	appCFG.Load();
	ToCtrl();
}
void CfgDlg::Save(){
	FromCtrl();
	appCFG.Save();
}

#endif
