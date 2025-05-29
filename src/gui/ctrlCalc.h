#ifndef __HCM4A_CalcFS4A_ctrlCalc_h_
#define __HCM4A_CalcFS4A_ctrlCalc_h_

#include <Core/Hash.h>
#include <GridCtrl/GridCtrl.h>

#include "dtCalc.h"

using namespace Upp;




struct HSinglCalcCtrl{
	typedef HSinglCalcCtrl CLASSNAME;
	WithLayCalcResH<ParentCtrl> layCalcRes;	// layout to place the control
	GridCtrl grCalcRes;
	void Clear();
	void Setup();
	void Load(const TblSingleRec& tblSingleRec);
	int GetCurSelID();
	HSinglCalcCtrl();
};
HSinglCalcCtrl::HSinglCalcCtrl(){
	CtrlLayout(layCalcRes);
	layCalcRes.Add(grCalcRes.SizePos());
	Setup();
}

void HSinglCalcCtrl::Setup(){
	grCalcRes.Clear(true);
	grCalcRes.ColoringMode(1);
	grCalcRes.OddColor().ColoringMode(1);
	grCalcRes.EvenColor().ColoringMode(2);
	grCalcRes.Indicator(); //---------------
	grCalcRes.AddColumn("T_No", String(t_("№"))).Min(50).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("TX_NAME", String(t_("txName"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("PRX_NAME", String(t_("prxName"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("PTX_NAME", String(t_("ptxName"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("RX_NAME", String(t_("rxName"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("T_TD", String(t_("TD"))).Min(150).HeaderAlignCenter().AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("TX_COUNTRY", String(t_("txCountry"))).Min(50).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("RX_COUNTRY", String(t_("rxCountry"))).Min(50).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("I_I", String(t_("I"))).Min(150).HeaderAlignCenter().AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("T_O_AREA", String(t_("O_AREA"))).Min(150).HeaderAlignCenter().AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("T_NFD", String(t_("NFD"))).Min(50).HeaderAlignCenter().AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("T_MD", String(t_("MD"))).Min(100).HeaderAlignCenter().AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("T_A_TOT", String(t_("A_TOT"))).Min(100).HeaderAlignCenter().AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("T_Error", String(t_("Error"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.RejectNullRow();
	grCalcRes.Navigating();
	grCalcRes.SetToolBar();
	grCalcRes.Clipboard();
	grCalcRes.Absolute();
	grCalcRes.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10
}

void HSinglCalcCtrl::Load(const TblSingleRec& tblSingleRec){
	grCalcRes.Clear();
		grCalcRes.Add();
		grCalcRes("T_No") = 1;
	    grCalcRes("TX_NAME") = tblSingleRec.ts_txname;
	    grCalcRes("PRX_NAME") = tblSingleRec.ts_prxname;
	    grCalcRes("PTX_NAME") = tblSingleRec.ts_ptxname;
	    grCalcRes("RX_NAME") = tblSingleRec.ts_rxname;
	    grCalcRes("TX_COUNTRY") = tblSingleRec.ts_txcountry;
	    grCalcRes("RX_COUNTRY") = tblSingleRec.ts_rxcountry;
		grCalcRes("I_I") = Format("%.4f", tblSingleRec.ts_i_i);
		grCalcRes("T_O_AREA") = Format("%.4f", tblSingleRec.ts_i_o_area);
		grCalcRes("T_NFD") = Format("%.4f", tblSingleRec.ts_i_nfd);
		grCalcRes("T_MD") = Format("%.4f", tblSingleRec.ts_i_md);
		grCalcRes("T_A_TOT") = Format("%.4f", tblSingleRec.ts_i_a_tot);
		grCalcRes("T_TD") = Format("%.4f", tblSingleRec.ts_i_td);
		grCalcRes("T_Error") = appErrorMap.Get(tblSingleRec.ts_i_error).ToString(); // String() << tblSingleRec.ts_i_error << ": " << tblSingleRec.ts_msgerror;
		grCalcRes.SetRowHeight(grCalcRes.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5);// row height = GridCtrl.height * 1.5
}
int HSinglCalcCtrl::GetCurSelID(){
	Value npp = grCalcRes("T_No");
	if(!npp.IsNull()){
		return int(npp);
	}
	return -1;
}
void HSinglCalcCtrl::Clear(){
	grCalcRes.Clear();
}
//!---------------------------------------------------------------------------------------

struct VSinglCalcCtrl{
	typedef VSinglCalcCtrl CLASSNAME;
	WithLayCalcResV<ParentCtrl> layCalcRes;	// layout to place the control
	GridCtrl grCalcRes;
	void Clear();
	void Setup();
	void Load(const TblSingleRec& tblSingleRec);
	int GetCurSelID();
	VSinglCalcCtrl();
};
VSinglCalcCtrl::VSinglCalcCtrl(){
	CtrlLayout(layCalcRes);
	layCalcRes.Add(grCalcRes.SizePos());
	Setup();
}

void VSinglCalcCtrl::Setup(){
	grCalcRes.Clear(true);
	grCalcRes.Header(false);
	grCalcRes.ColoringMode(1);
	grCalcRes.OddColor().ColoringMode(1);
	grCalcRes.EvenColor().ColoringMode(2);
	grCalcRes.Indicator(); //---------------
	grCalcRes.AddColumn("T_NAME", String("")).Min(250).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("T_VALUE", String("")).Min(250).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.RejectNullRow();
	grCalcRes.Navigating();
	grCalcRes.SetToolBar();
	grCalcRes.Clipboard();
	grCalcRes.Absolute();
	grCalcRes.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10
	// grCalcRes.WhenLeftDouble
}

void VSinglCalcCtrl::Load(const TblSingleRec& tblSingleRec){
	grCalcRes.Clear();
	for(int i=0; i<tblSingleRec.GetNameValList().GetCount(); i++){
		grCalcRes.Add();
		grCalcRes("T_NAME") = tblSingleRec.GetNameValList()[i].name;
		grCalcRes("T_VALUE") = tblSingleRec.GetNameValList()[i].val;
		grCalcRes.SetRowHeight(grCalcRes.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5);// row height = GridCtrl.height * 1.5
	}
}
int VSinglCalcCtrl::GetCurSelID(){
	Value npp = grCalcRes("T_No");
	if(!npp.IsNull()){
		return int(npp);
	}
	return -1;
}
void VSinglCalcCtrl::Clear(){
	grCalcRes.Clear();
}
//!---------------------------------------------------------------------------------------



struct HMultCalcCtrl{
	typedef HMultCalcCtrl CLASSNAME;
	WithLayCalcResH<ParentCtrl> layCalcRes;	// layout to place the control
	GridCtrl grCalcRes;
	void Clear();
	void Setup();
	void Load(const Vector<TblMultRec>& multList);
	void ShowRecNo();
	int curRecNum;
	int GetCurSelID();
	HMultCalcCtrl();
};
HMultCalcCtrl::HMultCalcCtrl(){
	curRecNum = 0;
	CtrlLayout(layCalcRes);
	layCalcRes.Add(grCalcRes.SizePos());
	Setup();
}

void HMultCalcCtrl::Setup(){
	grCalcRes.Clear(true);
	grCalcRes.ColoringMode(1);
	grCalcRes.OddColor().ColoringMode(1);
	grCalcRes.EvenColor().ColoringMode(2);
	grCalcRes.Indicator(); //---------------
	grCalcRes.AddColumn("T_No", String(t_("№"))).Min(50).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("TX_TEST_NAME", String(t_("TX_TEST"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("PRX_TEST_NAME", String(t_("PRX_TEST"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("PTX_TEST_NAME", String(t_("PTX_TEST"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("RX_TEST_NAME", String(t_("RX_TEST"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("TX_REF_NAME", String(t_("TX_REF"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("PRX_REF_NAME", String(t_("PRX_REF"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("PTX_REF_NAME", String(t_("PTX_REF"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("RX_REF_NAME", String(t_("RX_REF"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("TX_COUNTRY", String(t_("TX_COUNTRY"))).Min(50).HeaderAlignCenter().AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("PX_COUNTRY", String(t_("PX_COUNTRY"))).Min(50).HeaderAlignCenter().AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("RX_COUNTRY", String(t_("RX_COUNTRY"))).Min(50).HeaderAlignCenter().AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("T_TD", String(t_("TD"))).Min(150).HeaderAlignCenter().AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("I_I", String(t_("I"))).Min(150).HeaderAlignCenter().AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("T_O_AREA", String(t_("O_AREA"))).Min(150).HeaderAlignCenter().AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("T_NFD", String(t_("NFD"))).Min(50).HeaderAlignCenter().AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("T_MD", String(t_("MD"))).Min(100).HeaderAlignCenter().AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("T_A_TOT", String(t_("A_TOT"))).Min(100).HeaderAlignCenter().AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("T_Error", String(t_("Error"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.RejectNullRow();
	grCalcRes.Navigating();
	grCalcRes.SetToolBar();
	grCalcRes.Clipboard();
	grCalcRes.Absolute();
	grCalcRes.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10
	//grCalcRes.WhenAction = THISBACK(ShowRecNo);
}

void HMultCalcCtrl::Load(const Vector<TblMultRec>& multList){
	curRecNum = 0;
	grCalcRes.Clear();
	int cnt = 0;
	for(auto v: multList){
		cnt++;
		grCalcRes.Add();
		grCalcRes("T_No") = cnt;
	    grCalcRes("TX_TEST_NAME") = v.tl_txtestname;
	    grCalcRes("PRX_TEST_NAME") = v.tl_prxtestname;
	    grCalcRes("PTX_TEST_NAME") = v.tl_ptxtestname;
	    grCalcRes("RX_TEST_NAME") = v.tl_rxtestname;
	    grCalcRes("TX_COUNTRY") = v.tl_txcountry;
	    grCalcRes("PX_COUNTRY") = v.tl_pxcountry;
	    grCalcRes("RX_COUNTRY") = v.tl_rxcountry;
	    grCalcRes("TX_REF_NAME") = v.tl_txrefname;
	    grCalcRes("PRX_REF_NAME") = v.tl_prxrefname;
	    grCalcRes("PTX_REF_NAME") = v.tl_ptxrefname;
	    grCalcRes("RX_REF_NAME") = v.tl_rxrefname;
		grCalcRes("I_I") = Format("%.4f", v.tl_i_i);
		grCalcRes("T_O_AREA") = Format("%.4f", v.tl_i_o_area);
		grCalcRes("T_NFD") = Format("%.4f", v.tl_i_nfd);
		grCalcRes("T_MD") = Format("%.4f", v.tl_i_md);
		grCalcRes("T_A_TOT") = Format("%.4f", v.tl_i_a_tot);
		grCalcRes("T_TD") = Format("%.4f", v.tl_i_td);
		grCalcRes("T_Error") = appErrorMap.Get(v.tl_i_error).ToString();
		grCalcRes.SetRowHeight(grCalcRes.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5);// row height = GridCtrl.height * 1.5
	}
}

void HMultCalcCtrl::ShowRecNo(){
	PromptOK(String() << GetCurSelID());
}

int HMultCalcCtrl::GetCurSelID(){
	Value npp = grCalcRes("T_No");
	if(!npp.IsNull()){
		curRecNum = npp; curRecNum--;
		return curRecNum;
	}
	return -1;
}
void HMultCalcCtrl::Clear(){
	grCalcRes.Clear();
}
//!---------------------------------------------------------------------------------------



struct HTrigCalcCtrl{
	typedef HTrigCalcCtrl CLASSNAME;
	WithLayCalcResH<ParentCtrl> layCalcRes;	// layout to place the control
	GridCtrl grCalcRes;
	void Clear();
	void Setup();
	void Load(const Vector<TrigerRec>& multList);
	int GetCurSelID();
	HTrigCalcCtrl();
};
HTrigCalcCtrl::HTrigCalcCtrl(){
	CtrlLayout(layCalcRes);
	layCalcRes.Add(grCalcRes.SizePos());
	Setup();
}

void HTrigCalcCtrl::Setup(){
	grCalcRes.Clear(true);
	grCalcRes.ColoringMode(1);
	grCalcRes.OddColor().ColoringMode(1);
	grCalcRes.EvenColor().ColoringMode(2);
	grCalcRes.Indicator(); //---------------
	grCalcRes.AddColumn("T_No", String(t_("№"))).Min(50).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("NAME", String(t_("NAME"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("REF", String(t_("REFERENCE"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("COUNTRIES", String(t_("COUNTRIES"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.AddColumn("Error", String(t_("Error"))).Min(150).HeaderAlignCenter().AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grCalcRes.RejectNullRow();
	grCalcRes.Navigating();
	grCalcRes.SetToolBar();
	grCalcRes.Clipboard();
	grCalcRes.Absolute();
	grCalcRes.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10
}

void HTrigCalcCtrl::Load(const Vector<TrigerRec>& multList){
	grCalcRes.Clear();
	int cnt = 0;
	for(auto v: multList){
		cnt++;
		grCalcRes.Add();
		grCalcRes("T_No") = cnt;
	    grCalcRes("NAME") = v.name;
	    grCalcRes("REF") = v.reference;
	    grCalcRes("COUNTRIES") = v.countries;
	    grCalcRes("Error") = appErrorMap.Get(v.error).ToString();
		grCalcRes.SetRowHeight(grCalcRes.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5);// row height = GridCtrl.height * 1.5
	}
}

int HTrigCalcCtrl::GetCurSelID(){
	Value npp = grCalcRes("T_No");
	if(!npp.IsNull()){
		return int(npp);
	}
	return -1;
}
void HTrigCalcCtrl::Clear(){
	grCalcRes.Clear();
}
//!---------------------------------------------------------------------------------------



class FormHTrigCalc : public WithLayCalcResH<TopWindow> {
public:
	typedef FormHTrigCalc CLASSNAME;
	HTrigCalcCtrl frmCtrl;
	bool isShow;
	void OnFrmClose();
	void Serialize(Stream& s);
	FormHTrigCalc();
};
FormHTrigCalc::FormHTrigCalc(){
	CtrlLayout(*this, String(t_("")));
	Sizeable().NoZoomable();
	Add(frmCtrl.layCalcRes.SizePos());
	WhenClose = THISBACK(OnFrmClose);
	isShow = false;
}
void FormHTrigCalc::Serialize(Stream& s){
	int version = 4;
	s / version;
	SerializePlacement(s);
	frmCtrl.grCalcRes.Serialize(s);
}
void FormHTrigCalc::OnFrmClose(){
	isShow = false;
	Close();
}
//!---------------------------------------------------------------------------------------



class FormHMultCalc : public WithLayCalcResH<TopWindow> {
public:
	typedef FormHMultCalc CLASSNAME;
	HMultCalcCtrl frmCtrl;
	MenuBar mainMenu;
	void MainMenu(Bar& bar) {
		bar.Sub(t_("File"), [=](Bar& bar) { FileMenu(bar); });
	}
	void FileMenu(Bar& bar) {
		bar.Add(t_("Save As"), [=] { SaveAs(); });
		bar.Separator();
		bar.Add(t_("Exit"), [=] { OnFrmClose(); });
	}
	Callback fcbSave;
	void SaveAs();
	bool isShow;
	void OnFrmClose();
	void Serialize(Stream& s);
	FormHMultCalc();
};
FormHMultCalc::FormHMultCalc(){
	CtrlLayout(*this, String(t_("")));
	Sizeable().NoZoomable();
	Add(frmCtrl.layCalcRes.SizePos());
	AddFrame(mainMenu);
	mainMenu.Set([=](Bar& bar) { MainMenu(bar); }); // or  mainMenu.Set(THISFN(MainMenu));
	WhenClose = THISBACK(OnFrmClose);
	isShow = false;
}
void FormHMultCalc::Serialize(Stream& s){
	int version = 2;
	s / version;
	SerializePlacement(s);
	frmCtrl.grCalcRes.Serialize(s);
}
void FormHMultCalc::OnFrmClose(){
	isShow = false;
	Close();
}
void FormHMultCalc::SaveAs(){
	if(fcbSave != Null){
		fcbSave();
	}
}

//!---------------------------------------------------------------------------------------

class FormVSinglCalc : public WithLayCalcResV<TopWindow> {
public:
	typedef FormVSinglCalc CLASSNAME;
	VSinglCalcCtrl frmCtrl;
	bool isShow;
	void OnFrmClose();
	void Serialize(Stream& s);
	FormVSinglCalc();
};
FormVSinglCalc::FormVSinglCalc(){
	CtrlLayout(*this, String(t_("")));
	Sizeable().MinimizeBox(false).MaximizeBox(false);
	Add(frmCtrl.layCalcRes.SizePos());
	WhenClose = THISBACK(OnFrmClose);
	isShow = false;
}
void FormVSinglCalc::Serialize(Stream& s){
	int version = 3;
	s / version;
	SerializePlacement(s);
	frmCtrl.grCalcRes.Serialize(s);
}
void FormVSinglCalc::OnFrmClose(){
	isShow = false;
	Close();
}
//!---------------------------------------------------------------------------------------

class FormHSinglCalc : public WithLayCalcResH<TopWindow> {
public:
	typedef FormHSinglCalc CLASSNAME;
	HSinglCalcCtrl frmCtrl;
	MenuBar mainMenu;
	void MainMenu(Bar& bar) {
		bar.Sub(t_("File"), [=](Bar& bar) { FileMenu(bar); });
	}
	void FileMenu(Bar& bar) {
		bar.Add(t_("Save As"), [=] { SaveAs(); });
		bar.Separator();
		bar.Add(t_("Exit"), [=] { OnFrmClose(); });
	}
	Callback fcbSave;
	void SaveAs();
	bool isShow;
	void OnFrmClose();
	void Serialize(Stream& s);
	FormHSinglCalc();
};
FormHSinglCalc::FormHSinglCalc(){
	CtrlLayout(*this, String(t_("")));
	Sizeable().MinimizeBox(false).MaximizeBox(false);
	Add(frmCtrl.layCalcRes.SizePos());
	AddFrame(mainMenu);
	mainMenu.Set([=](Bar& bar) { MainMenu(bar); }); // or  mainMenu.Set(THISFN(MainMenu));
	WhenClose = THISBACK(OnFrmClose);
	isShow = false;
}
void FormHSinglCalc::Serialize(Stream& s){
	int version = 3;
	s / version;
	SerializePlacement(s);
	frmCtrl.grCalcRes.Serialize(s);
}
void FormHSinglCalc::OnFrmClose(){
	isShow = false;
	Close();
}
void FormHSinglCalc::SaveAs(){
	if(fcbSave != Null){
		fcbSave();
	}
}

//----------------------------------------------------------------------------------------------

struct CalcParamDlg: public WithFrmParamCalc<TopWindow> {
	typedef CalcParamDlg CLASSNAME;
	void Serialize(Stream& s);
	void ToCtrl();
	void FromCtrl();
	void SetCheck();
	void Init();
	void Save();
	void Load();
	CalcParamDlg(CalcParamData& calcParam);
protected:
	CalcParamData& calcParam;
};
CalcParamDlg::CalcParamDlg(CalcParamData& calcParam):calcParam(calcParam){
	CtrlLayoutOKCancel(*this, t_("Set the calculation parameters"));
	Sizeable().Zoomable();
	//opb_DN.WhenAction = THISBACK(SetCheck);
	Init();
	Load();
}
void CalcParamDlg::Serialize(Stream& s){
	int version = 1;
	s / version;
	SerializePlacement(s);
}
void CalcParamDlg::Init(){
	dl_Plz.SetDisplay(Single<ItemDisplay>());
	ed_MinTD.SetFont(Single<ItemGrid>().headFont);
}
void CalcParamDlg::SetCheck(){
	bool isCheck = opb_DN;
	if(isCheck){
		opb_DN.Set(int(isCheck));
		ed_DN.Enable(isCheck);
		ed_N0.Enable(isCheck);
	}
	else{
		opb_DN.Set(int(!isCheck));
		ed_DN.Enable(!isCheck);
		ed_N0.Enable(!isCheck);
	}
}
void CalcParamDlg::ToCtrl(){
	PlzToDropList(dl_Plz);
	dl_Plz.SetIndex(0);
	ed_TimePercent.SetData(calcParam.inCalcRec.time_percentage);
	ed_Press.SetData(calcParam.inCalcRec.press);
	ed_Temp.SetData(calcParam.inCalcRec.temp);

	ed_MinTD.SetData(calcParam.minTD);
	opt_ShowUnCalc.Set(calcParam.isShowUnCalc);

	ed_DN.SetData(calcParam.inCalcRec.dN);
	ed_N0.SetData(calcParam.inCalcRec.n0);
	opb_DN.Set(int(false));
	ed_DN.Enable(false);
	ed_N0.Enable(false);
}
void CalcParamDlg::FromCtrl(){
	calcParam.inCalcRec.plz = PlzToInt(StrToPlz(dl_Plz.GetData().ToString()));
	calcParam.inCalcRec.time_percentage = ed_TimePercent.GetData();
	calcParam.inCalcRec.press = ed_Press.GetData();
	calcParam.inCalcRec.temp = ed_Temp.GetData();
	calcParam.minTD = ed_MinTD.GetData();
	calcParam.isShowUnCalc = opt_ShowUnCalc.Get();
	if(opb_DN){
		calcParam.inCalcRec.dN = ed_DN.GetData();
		calcParam.inCalcRec.n0 = ed_N0.GetData();
	}
	else{
		calcParam.inCalcRec.dN = 0.0;
		calcParam.inCalcRec.n0 = 0.0;
	}
}
void CalcParamDlg::Load(){
	calcParam.Load();
	if(calcParam.IsEmpty()) calcParam.inCalcRec.OnDefault();
	ToCtrl();
}
void CalcParamDlg::Save(){
	FromCtrl();
	calcParam.Save();
}
//!---------------------------------------------------------------------------------------------

#endif
