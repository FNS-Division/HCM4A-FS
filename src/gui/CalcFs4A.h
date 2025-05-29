#ifndef _CalcFs4A_CalcFs4A_h
#define _CalcFs4A_CalcFs4A_h

#include <Core/Core.h>
#include <CtrlLib/CtrlLib.h>
#include <DropGrid/DropGrid.h>
#include <SqlCtrl/SqlCtrl.h>
#include <GridCtrl/GridCtrl.h>
#include <Functions4U/Functions4U.h>

using namespace Upp;

#ifdef C_UMK
	// to compile from umk with flag C_UMK
	#define APP_PATH CalcFS4A
#else
	// **** !!! to compile from IDE **** !!!
	#define APP_PATH _HCM4A/CalcFS4A
#endif
//!--------------------------------------------

#define LAYOUTFILE <APP_PATH/app.lay>
#include <CtrlCore/lay.h>
#define IMAGEFILE <APP_PATH/app.iml>
#define IMAGECLASS Images
#include <Draw/iml.h>
//!--------------------------------------------

#ifdef PLATFORM_WIN32
	#define DLLFILENAME "hcm4afs.dll"
#else
	#define DLLFILENAME "hcm4afs.so"
#endif
//!--------------------------------------------

#include "dtHcmData.h"
#include "dtHcmDll.h"

HcmErrorMap appErrorMap;

#define DLIHEADER   <APP_PATH/hcm4afs.dli>
#define DLIMODULE   Hcm4AfsDll
#include <Core/dli.h>
//!--------------------------------------------

#include "appCfg.h"
#include "ctrlViewPack.h"
#include "common.h"

#include "ctrlCalc.h"
#include "ctrlReport.h"

#include "dtCalc.h"
#include "sqLite.h"
#include "dtSingle.h"
#include "dtMult.h"
#include "dtTriger.h"
//!--------------------------------------------
 
const String VERSION = "1.5.11";
const String APP_LST_DIR = "Data";

//!--------------------------------------------

enum class AppMode {NONE, SINGL, MULT};

//!--------------------------------------------

class CalcFs4A : public WithFrmMain<TopWindow>{
public:
	typedef CalcFs4A CLASSNAME;
	TabCtrl tab;
	void BeforeFrmShow();
	void BeforeFrmClose();
	void Serialize(Stream& s);
	void MainMenu(Bar& bar) {
		bar.Sub(t_("File"), [=](Bar& bar) { FileMenu(bar); });
		bar.Sub(t_("Calculation"), [=](Bar& bar) { CalcMenu(bar); });
		bar.Sub(t_("Tools"), [=](Bar& bar) { ToolsMenu(bar); });
	}
	void FileMenu(Bar& bar) {
		bar.Add(t_("New"), Images::NEW(), [=] { NewPack(); });
		bar.Add(t_("Open"), Images::OPEN(), [=] { OpenPack(); });
		bar.Add(t_("Save"), Images::SAVE(), [=] { SavePack(); });
		bar.Add(t_("Save As"), Images::SAVE_AS(), [=] { SavePackAs(); });
		bar.Separator();
		bar.Add(t_("Exit"), Images::EXIT(), [=] { OnClose(); });
	}
	void CalcMenu(Bar& bar) {
		bar.Add(t_("(Single | List-to-List) Entry Calculation"), Images::CALC(), [=] { RunCalc(); });
		bar.Add(t_("Trigger for Co-ordination "), Images::MAP(), [=] { RunCalcTriger(); });
	}
	void ToolsMenu(Bar& bar) {
		bar.Add(t_("Options"), Images::CONFIG(), [=] { ShowCfgDlg(); });
		bar.Add(t_("Validate HMC FS file"), Images::CHECKIT(), [=] { RunCheck(); });
	}
	CalcFs4A();
protected:
	MenuBar mainMenu;
	LayHcmPack layTest;
	LayHcmPack layRef;
	AppMode appMode;
	// --------------------
	// for non-modal form calls, declare as a class member
	FormHMultCalc frmHMultCalc;
	FormHSinglCalc frmHSinglCalc;
	FormHTrigCalc frmHTrigCalc;
	FormReport formReport;
	// --------------------
	CalcParamData inCalcParam;
	Vector<TrigerRec> trigCalcLst;
	// --------------------
	void Setup();
	void OnClose();
	void ShowCfgDlg();
	void CloseNotModalFrm();
	void AboutApp();
	// --------------------
	void FillAppLst();
	void OpenPack();
	void NewPack();
	void SavePack();
	void SavePackAs();
	void RunCheck();
	void RunCalc();
	void RunCalcSingle();
	void RunCalcMulti();
	void RunCalcTriger();
	void SaveSingleCalcToXLS();
	void SaveMultCalcToXLS();
	TblSingleRec calcSingleResult;
	TblMultData multHcmData;
	// --------------------
	String GetVersion()const{return VERSION;}
};

CalcFs4A::CalcFs4A(){
	CtrlLayout(*this, Format(t_(" %s: HCM4A-FS ver. %s"), GetExeTitle(), GetVersion()));
	Sizeable().Zoomable();
	tab.Add(layTest.SizePos(), t_("\1[ [ [3*!Arial!  HCM data file]]]"));
	AddFrame(mainMenu);
	mainMenu.Set([=](Bar& bar) { MainMenu(bar); }); // or  mainMenu.Set(THISFN(MainMenu));
	// layTest.cbSinglCalc = THISBACK(RunCalcSingle);
	Setup();
}
void CalcFs4A::Setup(){
	WhenClose = THISBACK(OnClose);
	btn_New.SetImage(Images::NEW());
	btn_New.Tip(t_("New HMC FS file"));
	btn_New <<= THISBACK(NewPack);
	btn_Open.SetImage(Images::OPEN());
	btn_Open.Tip(t_("Open HMC FS file"));
	btn_Open <<= THISBACK(OpenPack);
	btn_Save.SetImage(Images::SAVE());
	btn_Save.Tip(t_("Save HMC FS file"));
	btn_Save <<= THISBACK(SavePack);
	btn_SaveAs.SetImage(Images::SAVE_AS());
	btn_SaveAs.Tip(t_("Save HMC FS file as"));
	btn_SaveAs <<= THISBACK(SavePackAs);
	btn_Check.SetImage(Images::CHECKIT());
	btn_Check.Tip(t_("Validate HMC FS file"));
	btn_Check <<= THISBACK(RunCheck);
	btn_CalcSingle.SetImage(Images::CALC());
	btn_CalcSingle.Tip(t_("(Single | List-to-List) Entry Calculation"));
	btn_CalcSingle <<= THISBACK(RunCalc);
	btn_Trigger.SetImage(Images::MAP());
	btn_Trigger.Tip("Trigger for Co-ordination");
	btn_Trigger <<= THISBACK(RunCalcTriger);
	btn_Config.SetImage(Images::CONFIG());
	btn_Config.Tip("Options");
	btn_Config <<= THISBACK(ShowCfgDlg);
	btn_About.SetImage(Images::ABOUT());
	btn_About.Tip("About CalcFS4A");
	btn_About <<= THISBACK(AboutApp);
	btn_Exit.SetImage(Images::EXIT());
	btn_Exit.Tip("Exit");
	btn_Exit <<= THISBACK(OnClose);
}
//!---------------------------------------------------------------------------------------------

void CalcFs4A::AboutApp(){
	WithAboutDlg<TopWindow> dlg;
	CtrlLayoutCancel(dlg, t_("About CalcFS4A"));
	dlg.image.SetImage(Images::ABOUT_96());
	Font fnt = Single<ItemGrid>().headFont;
	String aboutStr = String() << "HCM software is used for co-ordination of fixed service stations according to HCM4A-Agreement";
	aboutStr << "\n\nVersion: " << VERSION;
	aboutStr << "\nCopyrite © \"International Telecommunication Union (ITU)\"";
	dlg.richText.SetQTF(DeQtf(aboutStr));
	dlg.Execute();
}
//!---------------------------------------------------------------------------------------------

void CalcFs4A::Serialize(Stream& s){
	int version = 1;
	s / version;
	SerializePlacement(s);
	layTest.Serialize(s);
	layRef.Serialize(s);
	frmHMultCalc.Serialize(s);
	frmHSinglCalc.Serialize(s);
	frmHTrigCalc.Serialize(s);
	formReport.Serialize(s);
}
//!---------------------------------------------------------------------------------------------

void CalcFs4A::FillAppLst(){
	Single<HcmLstData>().AddLst(HcmLstRec("0A", Vector<String>{"Tx", "Rx", "Ptx", "Prx"}));
	Single<HcmLstData>().AddLst(HcmLstRec("1A1", Vector<String>{"k", "M", "G"}));
	Single<HcmLstData>().AddLst(HcmLstRec("1Z", String() << APP_LST_DIR << "/" << Single<AppCFG>().cfgEditRec.GetLstFreqCategoryName()));
	Single<HcmLstData>().AddLst(HcmLstRec("6A", Vector<String>{"FX"}));
	Single<HcmLstData>().AddLst(HcmLstRec("6B", Vector<String>{"CO", "CP", "CR", "CV", "OT"}));
	Single<HcmLstData>().AddLst(HcmLstRec("6Z", String() << APP_LST_DIR << "/" << Single<AppCFG>().cfgEditRec.GetLstCategoryOfUseName()));
	Single<HcmLstData>().AddLst(HcmLstRec("4B", String() << APP_LST_DIR << "/" << Single<AppCFG>().cfgEditRec.GetLstCountriesName()));
	Single<HcmLstData>().AddLst(HcmLstRec("9D", Vector<String>{"H", "V"}));
	Single<HcmLstData>().AddLst(HcmLstRec("9Xptcp", Vector<String>{"", "CP", "VV", "HH"}));
	Single<HcmLstData>().AddLst(HcmLstRec("9Xptxp", Vector<String>{"", "XP", "VH", "HV"}));
	Single<HcmLstData>().AddLst(HcmLstRec("7K", String() << APP_LST_DIR << "/" << Single<AppCFG>().cfgEditRec.GetLstMaxCapacitLinkName()));
	Single<HcmLstData>().AddLst(HcmLstRec("13Y", String() << APP_LST_DIR << "/" << Single<AppCFG>().cfgEditRec.GetLstStatusOfCooName()));
}
//!---------------------------------------------------------------------------------------------

void CalcFs4A::BeforeFrmShow(){
	Single<AppCFG>().Load();
	FillAppLst();
	inCalcParam.Load();
	LoadFromFile(*this);
}
void CalcFs4A::BeforeFrmClose(){
	inCalcParam.Save();
	Single<AppCFG>().Save();
	StoreToFile(*this);
}
void CalcFs4A::OnClose(){
	layTest.CloseHcmPack();
	layRef.CloseHcmPack();
    Close();
}
//!---------------------------------------------------------------------------------------------

void CalcFs4A::CloseNotModalFrm(){
	if(frmHSinglCalc.isShow){frmHSinglCalc.isShow = false; frmHSinglCalc.Close();};
	if(frmHMultCalc.isShow)	{frmHMultCalc.isShow = false; frmHMultCalc.Close();};
	if(frmHTrigCalc.isShow)	{frmHTrigCalc.isShow = false; frmHTrigCalc.Close();};
	if(formReport.isShow)	{formReport.isShow = false; formReport.Close();};
}
//!---------------------------------------------------------------------------------------------

void CalcFs4A::ShowCfgDlg(){
	CfgDlg dlg(Single<AppCFG>());
	if(dlg.Run() == IDOK) dlg.Save();
}
//!---------------------------------------------------------------------------------------------

void CalcFs4A::OpenPack(){
	appMode = AppMode::NONE;
	OpenHcmDlg openDlg(Single<AppCFG>());
	if(openDlg.Run() == IDOK){
		String testFile = openDlg.ed_FileTest.GetData().ToString();
		String refFile = openDlg.ed_FileRef.GetData().ToString();
		if(openDlg.opb_Ref && FileExists(testFile) && FileExists(refFile)){
			appMode = AppMode::MULT;
			if(tab.GetCount()==1)
				tab.Add(layRef.SizePos(), t_("\1[ [ [3*!Arial!  HCM reference data file]]]"));
		}
		else{
			if(FileExists(testFile)){
				appMode = AppMode::SINGL;
				if(tab.GetCount() > 1 )
					tab.Remove(1);
			}
		}
		layTest.CloseHcmPack();
		layRef.CloseHcmPack();
		switch(appMode){
			case AppMode::NONE:
				return;
			case AppMode::SINGL:
				layTest.OpenHcmPack(testFile);
				break;
			case AppMode::MULT:
				layTest.OpenHcmPack(testFile);
				layRef.OpenHcmPack(refFile);
				break;
		}
		CloseNotModalFrm();
	}
}
void CalcFs4A::SavePack(){
	layTest.SaveHcmPack();
	layRef.SaveHcmPack();
}
void CalcFs4A::SavePackAs(){
	layTest.SaveHcmPackAs();
	layRef.SaveHcmPackAs();
}
void CalcFs4A::NewPack(){
	layTest.NewHcmPack();
	layRef.NewHcmPack();
	layTest.AddEntry();
}
//!---------------------------------------------------------------------------------------------

void CalcFs4A::RunCheck(){
	if(layTest.GetHcmPack().IsEmpty()){
		OpenPack();
	}
	bool isCheck = false;
	String vlog;
	if(layTest.GetHcmPack().IsValid()){
		isCheck = true;
		vlog << layTest.GetHcmPack().GetPackFile() << ":\n";
		layTest.CheckHcmPack(vlog);
	}
	if(layRef.GetHcmPack().IsValid()){
		isCheck = true;
		vlog << "\n\n" << layRef.GetHcmPack().GetPackFile() << ":\n";
		layRef.CheckHcmPack(vlog);
	}
	if(isCheck){
		formReport.Title("HMC FS file validation results");
		formReport.de_Report.SetData(vlog);
		if(formReport.isShow){
			ActiveFocus(formReport);
		}
		else{
			formReport.OpenMain();	// for non-modal form calls
			formReport.isShow = true;
		}
	}
}
//!---------------------------------------------------------------------------------------------

void CalcFs4A::SaveSingleCalcToXLS(){
	FileSel fs;
	fs.ReadOnlyOption(true);
	fs.MkDirOption(false);
	fs.ActiveDir(GetExeFolder());
	fs.EditFileName(true);
	fs.Types("Excel files\t*.xlsx");
	if(fs.ExecuteSaveAs()){
		const String file = fs;
		xlnt::workbook wbOut;
	    xlnt::worksheet wsOut = wbOut.active_sheet();
	    wsOut.title("single calc");
	    Vector<String> title;
		calcSingleResult.FillTitle(title);
	    for(int i=0; i<title.GetCount(); i++){
			wsOut.cell(xlnt::cell_reference(i+1, 1)).value(title[i].ToStd());
		}
	    Vector<String> data;
		calcSingleResult.FillData(data);
	    for(int i=0; i<data.GetCount(); i++){
	        wsOut.cell(xlnt::cell_reference(i+1, 2)).value(data[i].ToStd());
	    }
		wbOut.save(file.ToStd());
	}
}
void CalcFs4A::SaveMultCalcToXLS(){
	FileSel fs;
	fs.ReadOnlyOption(true);
	fs.MkDirOption(false);
	fs.ActiveDir(GetExeFolder());
	fs.EditFileName(true);
	fs.Types("Excel files\t*.xlsx");
	if(fs.ExecuteSaveAs()){
		const String file = fs;
		xlnt::workbook wbOut;
	    xlnt::worksheet wsOut = wbOut.active_sheet();
	    wsOut.title("list to list calc");
	    Vector<String> title;
		Vector<Vector<String>> data;
		if(multHcmData.multCalcLst.GetCount()){
			multHcmData.multCalcLst[0].FillTitle(title);
			for(auto v: multHcmData.multCalcLst){
				v.FillData(data.Add());
			}
		}
	    for(int i=0; i<title.GetCount(); i++){
			wsOut.cell(xlnt::cell_reference(i+1, 1)).value(title[i].ToStd());
		}
		for(int i=0; i<data.GetCount(); i++){
			for(int j=0; j<data[i].GetCount(); j++)
				wsOut.cell(xlnt::cell_reference(j+1, i+2)).value(data[i][j].ToStd());
		}
		wbOut.save(file.ToStd());
	}
}
//!---------------------------------------------------------------------------------------------

void CalcFs4A::RunCalc(){
	if(layTest.GetHcmPack().IsEmpty()){
		OpenPack();
	}
	if(!layTest.GetHcmPack().IsValid()){
		Exclamation(DeQtf(Format("There is no way to perform calculation, file: \"%s\"\n is not an hcm data file", layTest.GetHcmPack().GetPackFile())));
		return;
	}
	switch(appMode){
		case AppMode::NONE:
			return;
		case AppMode::SINGL:
			RunCalcSingle();
			break;
		case AppMode::MULT:
			RunCalcMulti();
			break;
	}
}

void CalcFs4A::RunCalcSingle(){
	if(!layTest.GetHcmPack().IsValid()) return;
	CalcParamDlg dlg(inCalcParam);
	if(dlg.Run() != IDOK) return;
		dlg.Save();
		dlg.FromCtrl();
		calcSingleResult.Clear();
		SingleCalc(calcSingleResult, layTest.GetHcmPack(), inCalcParam, Single<AppCFG>());
		frmHSinglCalc.frmCtrl.Clear();
		frmHSinglCalc.frmCtrl.Load(calcSingleResult);
		frmHSinglCalc.fcbSave << [=]{SaveSingleCalcToXLS();};
		frmHSinglCalc.Title(GetFileName(layTest.GetHcmPack().GetPackFile()));
		// Show Calc Report ----------------------------------------------
		frmHSinglCalc.frmCtrl.grCalcRes.WhenLeftDouble << [=] {
			formReport.fileName = GetFileName(layTest.GetHcmPack().GetPackFile());
			formReport.de_Report.SetData(calcSingleResult.ts_report);
			if(formReport.isShow){
				ActiveFocus(formReport);
			}
			else{
				formReport.OpenMain();	// for non-modal form calls
				formReport.isShow = true;
			}
		};
		//----------------------------------------------------
		if(frmHSinglCalc.isShow){
			ActiveFocus(frmHSinglCalc);
		}
		else{
			frmHSinglCalc.OpenMain();	// for non-modal form calls
			frmHSinglCalc.isShow = true;
		}
}
//!---------------------------------------------------------------------------------------------

void CalcFs4A::RunCalcMulti(){
	CalcParamDlg dlg(inCalcParam);
	if(dlg.Run() != IDOK) return;
	dlg.Save();
	dlg.FromCtrl();
	if(Hcm4AfsDll()){
		multHcmData.multCalcLst.Clear();
		MultHcmPack mTestPack; mTestPack.FromPack(layTest.GetHcmPack());
		MultHcmPack mRefPack; mRefPack.FromPack(layRef.GetHcmPack());
		CalcPTR(multHcmData.multCalcLst, mTestPack, mRefPack, inCalcParam, Single<AppCFG>());
		CalcTx(multHcmData.multCalcLst, mTestPack, mRefPack, inCalcParam, Single<AppCFG>());
		CalcRx(multHcmData.multCalcLst, mTestPack, mRefPack, inCalcParam, Single<AppCFG>());
	}
	Hcm4AfsDll().Free();
	frmHMultCalc.frmCtrl.Load(multHcmData.multCalcLst);
	frmHMultCalc.fcbSave << [=]{SaveMultCalcToXLS();};
	frmHMultCalc.Title(GetFileName(Single<AppCFG>().cfgCalcRec.fPackTest) + " / " + GetFileName(Single<AppCFG>().cfgCalcRec.fPackRef));
	// Show Calc Report ----------------------------------------------
	frmHMultCalc.frmCtrl.grCalcRes.WhenLeftDouble << [=] {
		int recNo = frmHMultCalc.frmCtrl.GetCurSelID();
		if(recNo>=0 && recNo < multHcmData.multCalcLst.GetCount()){
			formReport.fileName = String() << Format("%s_%s.txt", GetFileTitle(Single<AppCFG>().cfgCalcRec.fPackTest), recNo);
			formReport.de_Report.SetData(multHcmData.multCalcLst[recNo].tl_report);
			if(formReport.isShow){
				ActiveFocus(formReport);
			}
			else{
				formReport.OpenMain();	// for non-modal form calls
				formReport.isShow = true;
			}
		}
	};
	//---------------------------------------------------------------
	if(frmHMultCalc.isShow){
		ActiveFocus(frmHMultCalc);
	}
	else{
		frmHMultCalc.OpenMain();	// for non-modal form calls
		frmHMultCalc.isShow = true;
	}
}
//!---------------------------------------------------------------------------------------------

void CalcFs4A::RunCalcTriger(){
	trigCalcLst.Clear();
	String fileName = OpenFileTXT(Single<AppCFG>().cfgPathRec.GetWorkDir());
	if(fileName.IsEmpty()) return;
	MultHcmPack mPack;
	if(mPack.FromCSV(fileName)){
		TrigerCalc(trigCalcLst, mPack, Single<AppCFG>());
		frmHTrigCalc.frmCtrl.Load(trigCalcLst);
		frmHTrigCalc.Title(GetFileName(fileName));
		if(frmHTrigCalc.isShow){
			ActiveFocus(frmHTrigCalc);
		}
		else{
			frmHTrigCalc.OpenMain();	// for non-modal form calls
			frmHTrigCalc.isShow = true;
		}
	}
	else{
		Exclamation(DeQtf(Format("Error opening file: \"%s\"\nThe selected file is not an hcm data file", fileName)));
	}
}
//!---------------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------------

#endif
