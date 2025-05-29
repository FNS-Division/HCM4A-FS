#ifndef __HCM4A_CalcFS4A_ctrlEditPack_h_
#define __HCM4A_CalcFS4A_ctrlEditPack_h_

#include "dtPack.h"

using namespace HCMFS;

//!---------------------------------------------------------------------------------------------
String ShowEditDlg(){
	TopWindow  dlg;
	dlg.Title("Add to the list...");
	dlg.SetRect(0, 0, Zx(200), Zy(20));
	EditString text;
	dlg.Add(text.TopPosZ(0, 20).HSizePos());
	text.SetData("");
	dlg.Run();
	return (String)text.GetData();
}

//!---------------------------------------------------------------------------------------------

struct HcmEditRec{
	void Clear(){nvMap.Clear();}
	const VectorMap<String, NameVal>& GetMapNV()const{return nvMap;}
	void SetVal(const String& code, const String& val);
	void Load(const HcmData& data);
	void Save(HcmData& data);
	void ToGridNV(GridCtrl& grid, bool isInControl, Array<DropList>& lstDLST, Array<EditString>& lstSTR, Array<EditInt>& lstINT,
				  Array<EditDouble>& lstDBL, Array<DropDate>& lstDATE);
	void ToGridNV(GridCtrl& grid, bool isInControl, Array<DropGrid>& lstDGRID, Array<EditString>& lstSTR, Array<EditInt>& lstINT,
				  Array<EditDouble>& lstDBL, Array<DropDate>& lstDATE);
	void FromGridNV(GridCtrl& grid);
	String ToString()const;
	HcmEditRec(){};
protected:
	bool FillDropList(const String& code, DropList& dl);
	bool FillDropGrid(const String& code, DropGrid& dg);
	VectorMap<String, NameVal> nvMap;
};
String HcmEditRec::ToString()const{
	String str = "";
	for(auto v: nvMap)
		str << v.ToString() << "\n";
	return str;
}
void HcmEditRec::SetVal(const String& code, const String& val){
	int idx = nvMap.Find(code);
	if(idx > -1) nvMap[idx].val = val;
}
void HcmEditRec::Load(const HcmData& data){
	for(int i = 0; i < nvMap.GetCount(); i++){
		String code = nvMap.GetKey(i);
		HcmField hField = data.GetField(code);
		nvMap[i].Set(hField.GetCode(), hField.GetName(), hField.GetStr(), hField.GetDesct());
	}
}
void HcmEditRec::Save(HcmData& data){
	for(int i = 0; i < nvMap.GetCount(); i++){
		String code = nvMap.GetKey(i);
		data.EditVal(code, nvMap[i].val);
	}
}
bool HcmEditRec::FillDropList(const String& code, DropList& dl){
	Single<HcmLstData>().GetLst(code).Fill(dl);
	return dl.GetCount();
}
bool HcmEditRec::FillDropGrid(const String& code, DropGrid& dg){
	Single<HcmLstData>().GetLst(code).Fill(dg);
	return dg.GetCount();
}

void HcmEditRec::ToGridNV(GridCtrl& grid, bool isInControl, Array<DropList>& lstDLST, Array<EditString>& lstSTR, Array<EditInt>& lstINT,
						  Array<EditDouble>& lstDBL, Array<DropDate>& lstDATE){
	grid.Clear();
	for(int i=0; i < GetMapNV().GetCount(); i++){
		grid.Add();
		NameVal nv = GetMapNV()[i];
		grid("T_CODE") = nv.code;
		grid("T_NAME") = nv.name;
		bool isMandat = isInControl && nv.fDescr.IsMandat();
		switch(nv.fDescr.GetFldEdit()){
			case FldEdit::NONE:
				grid("T_VALUE") = nv.val;
				break;
			case FldEdit::STR:{
					if(grid.GetCtrl(2) == NULL){
						grid.SetCtrl(2, lstSTR.Add().NotNull(isMandat).SetFont(StdFont().Height(StdFont().GetHeight()+3).Bold()));
					}
					grid("T_VALUE") = nv.val;
					if(grid.GetCtrl(2) != NULL){
						grid.GetCtrl(2)->SetData(nv.val);
						grid.GetCtrl(2)->Refresh();
					}
				}
				break;
			case FldEdit::INT:
				if(grid.GetCtrl(2) == NULL){
					grid.SetCtrl(2, lstINT.Add().NotNull(isMandat).SetFont(StdFont().Height(StdFont().GetHeight()+3).Bold()));
				}
				grid("T_VALUE") = StrInt(nv.val);
				if(grid.GetCtrl(2) != NULL){
					grid.GetCtrl(2)->SetData(StrInt(nv.val));
					grid.GetCtrl(2)->Refresh();
				}
				break;
			case FldEdit::FLOAT:
				if(grid.GetCtrl(2) == NULL){
					grid.SetCtrl(2, lstDBL.Add().NotNull(isMandat).SetFont(StdFont().Height(StdFont().GetHeight()+3).Bold()));
				}
				grid("T_VALUE") = StrDbl(nv.val);
				if(grid.GetCtrl(2) != NULL){
					grid.GetCtrl(2)->SetData(StrDbl(nv.val));
					grid.GetCtrl(2)->Refresh();
				}
				break;
			case FldEdit::DATE:
				if(grid.GetCtrl(2) == NULL){
					DropDate& drDate = lstDATE.Add();
					drDate.NotNull(isMandat).SetFont(StdFont().Height(StdFont().GetHeight()+3).Bold());
					// drDate.Format("%3:02d.%2:02d.%1:4d");
					grid.SetCtrl(2, drDate);
					// grid.SetCtrl(2, lstDATE.Add().NotNull(isMandat).SetFont(StdFont().Height(StdFont().GetHeight()+3).Bold()));
					//Format("%3:02d%2:02d%1:4d")
				}
				grid("T_VALUE") = nv.val;
				if(grid.GetCtrl(2) != NULL){
					grid.GetCtrl(2)->SetData(nv.val);
					grid.GetCtrl(2)->Refresh();
				}
				break;
			case FldEdit::LIST:{
					if(grid.GetCtrl(2) == NULL){
						DropList& dl = lstDLST.Add(new DropList());
						dl.NotNull(isMandat);
						dl.AddButton().Main(false).SetImage(Images::ADD_24()).WhenClick  << [&dl]{
							String str = ShowEditDlg();
							if(dl.Find(str) == -1) dl.Add(str);
						};
						// dl.DisplayAll(isMandat);
						dl.SetDisplay(Single<ItemDisplay>());
						if(!FillDropList(nv.code, dl)) FillDropList(nv.fDescr.GetLst(), dl);
						if(dl.Find(nv.val) == -1) dl.Add(nv.val);
						grid.SetCtrl(2, dl);
					}
					grid("T_VALUE") = nv.val;
					// grid.SetDisplay(i, 1, Single<ItemGrid>(isMandat));
					if(grid.GetCtrl(2) != NULL){
						grid.GetCtrl(2)->SetData(TrimBoth(nv.val));
						grid.GetCtrl(2)->Refresh();
					}
				}
				break;
		}
		grid.SetRowHeight(grid.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5); // row height = GridCtrl.height * 1.5
	}
}
void HcmEditRec::ToGridNV(GridCtrl& grid, bool isInControl, Array<DropGrid>& lstDGRID, Array<EditString>& lstSTR, Array<EditInt>& lstINT, Array<EditDouble>& lstDBL, Array<DropDate>& lstDATE){
	grid.Clear();
	for(int i=0; i < GetMapNV().GetCount(); i++){
		grid.Add();
		NameVal nv = GetMapNV()[i];
		grid("T_CODE") = nv.code;
		grid("T_NAME") = nv.name;
		bool isMandat = isInControl && nv.fDescr.IsMandat();
		switch(nv.fDescr.GetFldEdit()){
			case FldEdit::NONE:
				grid("T_VALUE") = nv.val;
				break;
			case FldEdit::STR:{
					if(grid.GetCtrl(2) == NULL){
						grid.SetCtrl(2, lstSTR.Add().NotNull(isMandat).SetFont(StdFont().Height(StdFont().GetHeight()+3).Bold()));
					}
					grid("T_VALUE") = nv.val;
					if(grid.GetCtrl(2) != NULL){
						grid.GetCtrl(2)->SetData(nv.val);
						grid.GetCtrl(2)->Refresh();
					}
				}
				break;
			case FldEdit::INT:
				if(grid.GetCtrl(2) == NULL){
					grid.SetCtrl(2, lstINT.Add().NotNull(isMandat).SetFont(StdFont().Height(StdFont().GetHeight()+3).Bold()));
				}
				grid("T_VALUE") = StrInt(nv.val);
				if(grid.GetCtrl(2) != NULL){
					grid.GetCtrl(2)->SetData(StrInt(nv.val));
					grid.GetCtrl(2)->Refresh();
				}
				break;
			case FldEdit::FLOAT:
				if(grid.GetCtrl(2) == NULL){
					grid.SetCtrl(2, lstDBL.Add().NotNull(isMandat).SetFont(StdFont().Height(StdFont().GetHeight()+3).Bold()));
				}
				grid("T_VALUE") = StrDbl(nv.val);
				if(grid.GetCtrl(2) != NULL){
					grid.GetCtrl(2)->SetData(StrDbl(nv.val));
					grid.GetCtrl(2)->Refresh();
				}
				break;
			case FldEdit::DATE:
				if(grid.GetCtrl(2) == NULL){
					grid.SetCtrl(2, lstDATE.Add().NotNull(isMandat).SetFont(StdFont().Height(StdFont().GetHeight()+3).Bold()));
				}
				grid("T_VALUE") = nv.val;
				if(grid.GetCtrl(2) != NULL){
					grid.GetCtrl(2)->SetData(nv.val);
					grid.GetCtrl(2)->Refresh();
				}
				break;
			case FldEdit::LIST:{
					if(grid.GetCtrl(2) == NULL){
						DropGrid& dg = lstDGRID.Add(new DropGrid());
						dg.NotNull(isMandat);
						FillDropGrid(nv.code, dg);

						dg.DisplayAll(0);
						dg.ColorRows();
						dg.SetDropLines(15);
						dg.SetValueColumn(0);
						//dg.AddValueColumn(0);


						grid.SetCtrl(2, dg);
					}
					grid("T_VALUE") = nv.val;
					if(grid.GetCtrl(2) != NULL){
						grid.GetCtrl(2)->SetData(TrimBoth(nv.val));
						grid.GetCtrl(2)->Refresh();
					}
				}
				break;
		}
		grid.SetRowHeight(grid.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5); // row height = GridCtrl.height * 1.5
	}
}
void HcmEditRec::FromGridNV(GridCtrl& grid){
	for(int i = 0; i < grid.GetRowCount(); i++){
		SetVal(grid.GetCell(i, 0).val.ToString(), grid.GetCell(i, 2).val.ToString());
	}
}
//!---------------------------------------------------------------------------------------

struct EditEntryRec: HcmEditRec{
	EditEntryRec(){
		nvMap.Add("4A", NameVal());
		nvMap.Add("0A", NameVal());
		nvMap.Add("1A", NameVal());
		nvMap.Add("1A1", NameVal());
		nvMap.Add("13X", NameVal());
	}
};
//!---------------------------------------------------------------------------------------

struct EditHeadRec: HcmEditRec{
	EditHeadRec(){
		nvMap.Add("1", NameVal());
		nvMap.Add("2", NameVal());
		nvMap.Add("3", NameVal());
		nvMap.Add("4", NameVal());
		nvMap.Add("5", NameVal());
		nvMap.Add("6", NameVal());
		nvMap.Add("7", NameVal());
		nvMap.Add("8", NameVal());
		nvMap.Add("9", NameVal());
		nvMap.Add("10", NameVal());
	}
};
//!---------------------------------------------------------------------------------------

struct EditGeneralRec: HcmEditRec{
	EditGeneralRec(){
			nvMap.Add("4A", NameVal());
			nvMap.Add("1A", NameVal());
			nvMap.Add("1A1", NameVal());
			nvMap.Add("0A", NameVal());
		nvMap.Add("4C", NameVal());
		nvMap.Add("9A", NameVal());
		nvMap.Add("9B", NameVal());
		nvMap.Add("9D", NameVal());
		nvMap.Add("4Z", NameVal());
		nvMap.Add("9L", NameVal());
		nvMap.Add("6A", NameVal());
		nvMap.Add("6B", NameVal());
		nvMap.Add("6Z", NameVal());
		nvMap.Add("1Z", NameVal());
		nvMap.Add("4B", NameVal());
			nvMap.Add("13X", NameVal());
		nvMap.Add("2C", NameVal());
		nvMap.Add("13Y", NameVal());
		nvMap.Add("2W", NameVal());
		nvMap.Add("2Z", NameVal());
		nvMap.Add("13Z", NameVal());
	}
};
//!---------------------------------------------------------------------------------------

struct EditEquipRec: HcmEditRec{
	EditEquipRec(){
	    nvMap.Add("7H", NameVal());
	    nvMap.Add("7I", NameVal());
	    nvMap.Add("8B", NameVal());
	    nvMap.Add("8B3", NameVal());
	    nvMap.Add("7G1", NameVal());
	    nvMap.Add("7G2", NameVal());
	    nvMap.Add("9H", NameVal());
	    nvMap.Add("7K", NameVal());
	    nvMap.Add("7A", NameVal());
	    nvMap.Add("7G3", NameVal());
	}
};
struct EditEqFARec: HcmEditRec{
	EditEqFARec(){
	    nvMap.Add("7Gf1", NameVal());
	    nvMap.Add("7Ga1", NameVal());
	    nvMap.Add("7Gf2", NameVal());
	    nvMap.Add("7Ga2", NameVal());
	    nvMap.Add("7Gf3", NameVal());
	    nvMap.Add("7Ga3", NameVal());
	    nvMap.Add("7Gf4", NameVal());
	    nvMap.Add("7Ga4", NameVal());
	    nvMap.Add("7Gf5", NameVal());
	    nvMap.Add("7Ga5", NameVal());
	    nvMap.Add("7Gf6", NameVal());
	    nvMap.Add("7Ga6", NameVal());
	}
};

struct EditAntRec: HcmEditRec{
	EditAntRec(){
	    nvMap.Add("9XM", NameVal());
	    nvMap.Add("9XT", NameVal());
	    nvMap.Add("9Y", NameVal());
	    nvMap.Add("9XFU", NameVal());
	    nvMap.Add("9XFL", NameVal());
	    nvMap.Add("9X1", NameVal());
	    nvMap.Add("9Xptcp", NameVal());
	    //nvMap.Add("9Xcpsize", NameVal());
	    nvMap.Add("9Xptxp", NameVal());
	    //nvMap.Add("9Xxpsize", NameVal());
	}
};
//!---------------------------------------------------------------------------------------



struct CpXpEditRec{
	void Clear(){tdVal.Clear();}
	const TwoDblVal& Get()const{return tdVal;}
	void Load(EditState state, const HcmData& data, const TwoStrVal& tsVal);
	void Save(HcmData& data);
	void GenGrid(GridCtrl& grid);
	void ToGrid(GridCtrl& grid);
	void FromGrid(GridCtrl& grid);
	String ToString()const;
	CpXpEditRec(){};
protected:
	void Fill(const HcmData& data);
	TwoDblVal ToTwoDbl(const TwoStrVal& val);
	Vector<TwoDblVal> tdLst;
	String anCode;				//	"9XCPan"
	String atCode;				//	"9XCPat"
	String szCode;				//	"9Xcpsize"
	TwoDblVal tdVal;
	EditState edState;
};

TwoDblVal CpXpEditRec::ToTwoDbl(const TwoStrVal& ts){
	TwoDblVal td(0.0, 0.0);
	Value vAn = StrDbl(ts.val_1); if(!vAn.IsNull()) td.val_1 = double(vAn);
	Value vAt = StrDbl(ts.val_2); if(!vAt.IsNull()) td.val_2 = double(vAt);
	return td;
}
void CpXpEditRec::Fill(const HcmData& data){
	tdLst.Clear();
	Vector<String> angles = clone(data.GetField(anCode).GetVec());
	Vector<String> attens  = clone(data.GetField(atCode).GetVec());
	int i=0;
	while(i<angles.GetCount() && i<attens.GetCount()){
		tdLst.Add(TwoDblVal(ToTwoDbl(TwoStrVal(angles[i], attens[i]))));
		i++;
	}
	Sort(tdLst, [](const TwoDblVal& a, const TwoDblVal& b) { return a.val_1 < b.val_1; });
}
void CpXpEditRec::Load(EditState state, const HcmData& data, const TwoStrVal& tsVal){
	Fill(data);
	edState = state;
	tdVal = ToTwoDbl(tsVal);
}
void CpXpEditRec::Save(HcmData& data){
	int idx = -1;
	for(int i=0; i<tdLst.GetCount(); i++){
		if(tdLst[i].val_1 == tdVal.val_1){
			idx = i;
			break;
		}
	}
	if(edState == EditState::Del && idx > -1) tdLst.Remove(idx);
	if(edState == EditState::Add && idx > -1) tdLst[idx].val_2 = tdVal.val_2;
	if(edState == EditState::Add && idx == -1) tdLst.Add(clone(tdVal));
	if(edState == EditState::Edit && idx > -1) tdLst[idx].val_2 = tdVal.val_2;

	Sort(tdLst, [](const TwoDblVal& a, const TwoDblVal& b) { return a.val_1 < b.val_1; });
	Vector<String> angles;
	Vector<String> attens;
	for(auto v: tdLst){
		angles.Add(DblStr(v.val_1));
		attens.Add(DblStr(v.val_2));
	}
	data.EditVal(anCode, angles);
	data.EditVal(atCode, attens);
	data.EditVal(szCode, IntStr(tdLst.GetCount()));
}

String CpXpEditRec::ToString()const{
	return Format("ANGLE: %f, ATTEN: %f", tdVal.val_1, tdVal.val_2);
}
void CpXpEditRec::GenGrid(GridCtrl& grid){
	//grid.Header(false);
	grid.Clear(true);
	grid.ColoringMode(1);
	grid.OddColor().ColoringMode(1);
	grid.EvenColor().ColoringMode(2);
	grid.Indicator(); //---------------
	grid.AddColumn("ANGLE", String(t_("Angle"))).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grid.AddColumn("ATTEN", String(t_("Attenuation"))).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grid.RejectNullRow();
	grid.Navigating();
	grid.SetToolBar();
	grid.Clipboard();
	grid.Absolute();
	grid.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10
}
void CpXpEditRec::ToGrid(GridCtrl& grid){
	grid.Clear();
	grid.Add();
	if(grid.GetCtrl(0) == NULL){
		grid.SetCtrl(0, new EditDouble());
	}
	if(grid.GetCtrl(1) == NULL){
		grid.SetCtrl(1, new EditDouble());
	}
	grid("ANGLE") = (tdVal.val_1);
	grid("ATTEN") = (tdVal.val_2);
	if(grid.GetCtrl(0) != NULL){
		grid.GetCtrl(0)->SetData(tdVal.val_1);
		grid.GetCtrl(0)->Refresh();
	}
	if(grid.GetCtrl(1) != NULL){
		grid.GetCtrl(1)->SetData(tdVal.val_2);
		grid.GetCtrl(1)->Refresh();
	}
	grid.SetRowHeight(grid.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5); // row height = GridCtrl.height * 1.5
}
void CpXpEditRec::FromGrid(GridCtrl& grid){
	double dAn = 0.0; double dAt = 0.0;
	Value vAn = StrDbl(grid.GetCell(0, 0).val.ToString()); if(!vAn.IsNull()) dAn = vAn;
	Value vAt = StrDbl(grid.GetCell(0, 1).val.ToString()); if(!vAt.IsNull()) dAt = vAt;
	tdVal = (TwoDblVal(dAn, dAt));
}
//!---------------------------------------------------------------------------------------

struct EditAntCpRec: CpXpEditRec{
	EditAntCpRec(){
		anCode = "9XCPan";
		atCode = "9XCPat";
		szCode = "9Xcpsize";
	}
};
struct EditAntXpRec: CpXpEditRec{
	EditAntXpRec(){
		anCode = "9XXPan";
		atCode = "9XXPat";
		szCode = "9Xxpsize";
	}
};
//!---------------------------------------------------------------------------------------

struct OneValGridEditDlg: public WithOneValGridEditDlg<TopWindow>{
public:
	void ToCtrl(HcmEditRec& rec);
	void FromCtrl(HcmEditRec& rec);
	void Clear();
	void Serialize(Stream& s);
	void BeforeFrmShow(const String& nameCtrl, VectorMap<String, String>& sMap);
	void BeforeFrmClose(const String& nameCtrl, VectorMap<String, String>& sMap);
	OneValGridEditDlg(const CfgEditRec& cfgEditRec);
protected:
	Array<DropGrid> lstDGRID;
	Array<DropList> lstDLST;
	Array<EditString> lstSTR;
	Array<EditInt> lstINT;
	Array<EditDouble> lstDBL;
	Array<DropDate> lstDATE;
	//Array<EditDate> lstDATE;
	bool isInControl;
	void Setup();
};
OneValGridEditDlg::OneValGridEditDlg(const CfgEditRec& cfgEditRec){
	CtrlLayoutOKCancel(*this, "Edit...");
	Sizeable();
	isInControl = cfgEditRec.isInControl;
	Setup();
}
void OneValGridEditDlg::Serialize(Stream& s){
    int version = 1;
    s / version;
	SerializePlacement(s);
	grid.Serialize(s);
}
void OneValGridEditDlg::BeforeFrmShow(const String& nameCtrl, VectorMap<String, String>& sMap){
	int idx = sMap.Find(nameCtrl);
	if(idx != -1) LoadFromString(*this, sMap[idx]);
}
void OneValGridEditDlg::BeforeFrmClose(const String& nameCtrl, VectorMap<String, String>& sMap){
	int idx = sMap.Find(nameCtrl);
	if(idx == -1)
		sMap.Add(nameCtrl, StoreAsString(*this));
	else
		sMap[idx] = StoreAsString(*this);
}
void OneValGridEditDlg::Setup(){
	grid.Clear(true);
	grid.ColoringMode(1);
	grid.OddColor().ColoringMode(1);
	grid.EvenColor().ColoringMode(2);
	grid.Header(false);
	grid.Indicator(); //---------------
	grid.AddColumn("T_CODE", String("")).Min(50).AlignLeft().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grid.AddColumn("T_NAME", String("")).Min(250).AlignLeft().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grid.AddColumn("T_VALUE", String("")).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grid.RejectNullRow();
	grid.Navigating();
	grid.SetToolBar();
	grid.Clipboard();
	grid.Absolute();
	grid.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10
}
void OneValGridEditDlg::ToCtrl(HcmEditRec& rec){
	rec.ToGridNV(grid, isInControl, lstDLST, lstSTR, lstINT, lstDBL, lstDATE);
}
void OneValGridEditDlg::FromCtrl(HcmEditRec& rec){
	rec.FromGridNV(grid);
}
void OneValGridEditDlg::Clear(){
	grid.Clear();
}
//!---------------------------------------------------------------------------------------------

struct TwoValGridEditDlg: public WithTwoValGridEditDlg<TopWindow>{
public:
	void ToCtrl(const EditEqFARec& rec);
	void FromCtrl(EditEqFARec& rec);
	void Clear();
	void Serialize(Stream& s);
	void BeforeFrmShow(const String& nameCtrl, VectorMap<String, String>& sMap);
	void BeforeFrmClose(const String& nameCtrl, VectorMap<String, String>& sMap);
	Array<EditDouble> lstDBL;
	TwoValGridEditDlg();
protected:
	void Setup();
};
TwoValGridEditDlg::TwoValGridEditDlg(){
	CtrlLayoutOKCancel(*this, "Edit...");
	Sizeable();
	Setup();
}
void TwoValGridEditDlg::Serialize(Stream& s){
    int version = 1;
    s / version;
	SerializePlacement(s);
	grid.Serialize(s);
}
void TwoValGridEditDlg::BeforeFrmShow(const String& nameCtrl, VectorMap<String, String>& sMap){
	int idx = sMap.Find(nameCtrl);
	if(idx != -1) LoadFromString(*this, sMap[idx]);
}
void TwoValGridEditDlg::BeforeFrmClose(const String& nameCtrl, VectorMap<String, String>& sMap){
	int idx = sMap.Find(nameCtrl);
	if(idx == -1)
		sMap.Add(nameCtrl, StoreAsString(*this));
	else
		sMap[idx] = StoreAsString(*this);
}
void TwoValGridEditDlg::Setup(){
	grid.Clear(true);
	//grid.Header(false);
	grid.ColoringMode(1);
	grid.OddColor().ColoringMode(1);
	grid.EvenColor().ColoringMode(2);
	grid.Indicator(); //---------------
	grid.AddColumn("F_CODE", String(t_(""))).Min(50).AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grid.AddColumn("T_FREQ", String(t_("Frequency, MHz"))).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grid.AddColumn("A_CODE", String(t_(""))).Min(50).AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grid.AddColumn("T_ATTEN", String(t_("Attenuation, bB"))).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grid.RejectNullRow();
	grid.Navigating();
	grid.SetToolBar();
	grid.Clipboard();
	grid.Absolute();
	grid.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10
}
void TwoValGridEditDlg::ToCtrl(const EditEqFARec& rec){
	grid.Clear();
	for(int i=0; i < rec.GetMapNV().GetCount(); i++){
		if(i%2 == 0){
			grid.Add();
			NameVal nv = rec.GetMapNV()[i];
			grid("F_CODE") = nv.code;
			EditDouble& ed = lstDBL.Add(new EditDouble());
			grid.SetCtrl(1, ed);
			grid("T_FREQ") = StrDbl(nv.val);
		}else{
			NameVal nv = rec.GetMapNV()[i];
			grid("A_CODE") = nv.code;
			EditDouble& ed = lstDBL.Add(new EditDouble());
			grid.SetCtrl(3, ed);
			grid("T_ATTEN") = StrDbl(nv.val);
		}
		grid.SetRowHeight(grid.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5); // row height = GridCtrl.height * 1.5
	}
}
void TwoValGridEditDlg::FromCtrl(EditEqFARec& rec){
	for(int i = 0; i < grid.GetRowCount(); i++){
		rec.SetVal(grid.GetCell(i, 0).val.ToString(), grid.GetCell(i, 1).val.ToString());
		rec.SetVal(grid.GetCell(i, 2).val.ToString(), grid.GetCell(i, 3).val.ToString());
	}
}
void TwoValGridEditDlg::Clear(){
	grid.Clear();
}
//!---------------------------------------------------------------------------------------


struct AntCpXpEditDlg: public WithAntCpXpEditDlg<TopWindow>{
public:
	void ToCtrl(CpXpEditRec& rec);
	void FromCtrl(CpXpEditRec& rec);
	void Clear();
	void Serialize(Stream& s);
	void BeforeFrmShow(const String& nameCtrl, VectorMap<String, String>& sMap);
	void BeforeFrmClose(const String& nameCtrl, VectorMap<String, String>& sMap);
	bool IsCurSel();

	AntCpXpEditDlg();
protected:
	void Setup();
};
AntCpXpEditDlg::AntCpXpEditDlg(){
	CtrlLayoutOKCancel(*this, "Edit...");
	Sizeable();
	Setup();
}
void AntCpXpEditDlg::Serialize(Stream& s){
    int version = 1;
    s / version;
	SerializePlacement(s);
	grid.Serialize(s);
}
void AntCpXpEditDlg::BeforeFrmShow(const String& nameCtrl, VectorMap<String, String>& sMap){
	int idx = sMap.Find(nameCtrl);
	if(idx != -1) LoadFromString(*this, sMap[idx]);
}
void AntCpXpEditDlg::BeforeFrmClose(const String& nameCtrl, VectorMap<String, String>& sMap){
	int idx = sMap.Find(nameCtrl);
	if(idx == -1)
		sMap.Add(nameCtrl, StoreAsString(*this));
	else
		sMap[idx] = StoreAsString(*this);
}
void AntCpXpEditDlg::Setup(){
	//grid.Header(false);
	grid.Clear(true);
	grid.ColoringMode(1);
	grid.OddColor().ColoringMode(1);
	grid.EvenColor().ColoringMode(2);
	grid.Indicator(); //---------------
	grid.AddColumn("ANGLE", String(t_("Angle"))).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grid.AddColumn("ATTEN", String(t_("Attenuation"))).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grid.RejectNullRow();
	grid.Navigating();
	grid.SetToolBar();
	grid.Clipboard();
	grid.Absolute();
	grid.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10
}
bool AntCpXpEditDlg::IsCurSel(){
	if(!grid.GetRowCount()) return false;
	return true;
}

void AntCpXpEditDlg::ToCtrl(CpXpEditRec& rec){
	rec.ToGrid(grid);
}
void AntCpXpEditDlg::FromCtrl(CpXpEditRec& rec){
	rec.FromGrid(grid);
}
void AntCpXpEditDlg::Clear(){
	grid.Clear();
}
//!---------------------------------------------------------------------------------------



#endif
