#ifndef __HCM4A_CalcFs4A_ctrlViewPack_h_
#define __HCM4A_CalcFs4A_ctrlViewPack_h_

#include "dtPack.h"
#include "ctrlEditPack.h"
#include "ctrlReport.h"

struct ViewGeneralRec{
	void Clear(){nvList.Clear();}
	const Vector<NameVal>& GetListNV()const{return nvList;}
	NameVal GetField(const int id);
	void Fill(const HcmIN& hcmIN);
	ViewGeneralRec();
protected:
	Vector<int> idList;
	Vector<NameVal> nvList;
};
ViewGeneralRec::ViewGeneralRec(){
	idList.Add(8);	// "4A");
	idList.Add(1);	// "1A");
	idList.Add(2);	// "1A1");
	idList.Add(0);	// "0A");
	idList.Add(10);	// "4C");
	// idList.Add(110);// "4Clo");
	// idList.Add(111);// "4Cla");
	idList.Add(33);	// "9A");
	idList.Add(34);	// "9B");
	idList.Add(35);	// "9D");
	idList.Add(11);	// "4Z");
	idList.Add(37);	// "9L");
	idList.Add(4);	// "6A");
	idList.Add(5);	// "6B");
	idList.Add(6);	// "6Z");
	idList.Add(3);	// "1Z");
	idList.Add(9);	// "4B");
	idList.Add(43);	// "13X");
	idList.Add(7);	// "2C");
	idList.Add(40);	// "13Y");
	idList.Add(41);	// "2W");
	idList.Add(42);	// "2Z");
	idList.Add(39);	// "13Z");
}
NameVal ViewGeneralRec::GetField(const int id){
	NameVal nv;
	if(id >= 0 && id < nvList.GetCount()){
		return clone(nvList[id]);
	}
	return nv;
}
void ViewGeneralRec::Fill(const HcmIN& hcmIN){
	nvList.Clear();
	for(int i = 0; i < idList.GetCount(); i++){
		HcmField hField = hcmIN.GetField(idList[i]);
		if(idList[i] == 10){
			String sLong = hField.GetStr().Mid(0, 8);
			String sLat  = hField.GetStr().Mid(8, 7);
			nvList.Add(NameVal(hField.GetCode(), t_("Longitude"), sLong));
			nvList.Add(NameVal(hField.GetCode(), t_("Latitude"), sLat));
		}
		else{
			nvList.Add(NameVal(hField.GetCode(), hField.GetName(), hField.GetStr()));
		}
	}
}
//!---------------------------------------------------------------------------------------

struct ViewEquipRec{
	void Clear(){nvList.Clear(); vvList.Clear();}
	const Vector<NameVal>& GetListNV()const{return nvList;}
	const Vector<TwoCodeVal>& GetListVV()const{return vvList;}
	NameVal GetField(const int id);
	void Fill(const HcmIN& fmap);
	ViewEquipRec();
	struct TwoVal: Moveable<TwoVal>{
		int frecID;
		int attenID;
		TwoVal():frecID(0), attenID(0){};
		TwoVal(int frecID, int attenID): frecID(frecID), attenID(attenID){};
	};
protected:
	Vector<int> idListF;
	Vector<TwoVal> idListA;
	Vector<NameVal> nvList;
	Vector<TwoCodeVal> vvList;
};
ViewEquipRec::ViewEquipRec(){
	idListF.Add(13);	// "7H");
	idListF.Add(14);	// "7I");
	idListF.Add(31);	// "8B");
	idListF.Add(32);	// "8B3");
	idListF.Add(28);	// "7G1");
	idListF.Add(29);	// "7G2");
	idListF.Add(36);	// "9H");
	idListF.Add(15);	// "7K");
	idListF.Add(12);	// "7A");
	idListF.Add(30);	// "7G3");
	idListA.Add(TwoVal(16, 17)); // 7Gf1, 7Ga1
	idListA.Add(TwoVal(18, 19)); // 7Gf2, 7Ga2
	idListA.Add(TwoVal(20, 21)); // 7Gf3, 7Ga3
	idListA.Add(TwoVal(22, 23)); // 7Gf4, 7Ga4
	idListA.Add(TwoVal(24, 25)); // 7Gf5, 7Ga5
	idListA.Add(TwoVal(26, 27)); // 7Gf6, 7Ga6
}
NameVal ViewEquipRec::GetField(const int id){
	NameVal nv;
	if(id >= 0 && id < nvList.GetCount()){
		return clone(nvList[id]);
	}
	return nv;
}
void ViewEquipRec::Fill(const HcmIN& hcmIN){
	nvList.Clear();
	vvList.Clear();
	for(int i = 0; i < idListF.GetCount(); i++){
		HcmField hField = hcmIN.GetField(idListF[i]);
		nvList.Add(NameVal(hField.GetCode(), hField.GetName(), hField.GetStr()));
	}
	for(int i = 0; i < idListA.GetCount(); i++){
		HcmField hField_f = hcmIN.GetField(idListA[i].frecID);
		HcmField hField_a = hcmIN.GetField(idListA[i].attenID);
		vvList.Add(TwoCodeVal(hField_f.GetCode(), hField_f.GetStr(), hField_a.GetCode(), hField_a.GetStr()));
	}
}
//!---------------------------------------------------------------------------------------

struct ViewAntRec{
	void Clear(){nvListF.Clear(); cpList.Clear(); xpList.Clear();}
	const Vector<NameVal>& GetListNV()const{return nvListF;}
	const Vector<TwoStrVal>& GetListCP()const{return cpList;}
	const Vector<TwoStrVal>& GetListXP()const{return xpList;}
	NameVal GetField(const int id);
	void Fill(const HcmIN& fmap);
	ViewAntRec();
protected:
	Vector<int> idListF;
	Vector<NameVal> nvListF;
	Vector<TwoStrVal> cpList;
	Vector<TwoStrVal> xpList;
};
ViewAntRec::ViewAntRec(){
	idListF.Add(44);	// "9XM");
	idListF.Add(45);	// "9XT");
	idListF.Add(38);	// "9Y");
	idListF.Add(47);	// "9XFU");
	idListF.Add(46);	// "9XFL");
	idListF.Add(48);	// "9X1");
	//-------------
	idListF.Add(49);	// "9Xptcp");	// Copolar radiation pattern
	idListF.Add(50);	// "9Xcpsize");	// Number of CP antenna mask data
	//idListF.Add(51);	// "9XCPan");	// Antenna CP angle
	//idListF.Add(52);	// "9XCPat");	// Antenna CP attenuation
	//-------------
	idListF.Add(53);	// "9Xptxp");	// Cross-polar radiation pattern
	idListF.Add(54);	// "9Xxpsize");	// Number of XP antenna mask data
	//idListF.Add(55);	// "9XXPan");	// Antenna XP angle
	//idListF.Add(56);	// "9XXPat");	// Antenna XP attenuation
	//-------------
}
NameVal ViewAntRec::GetField(const int id){
	NameVal nv;
	if(id >= 0 && id < nvListF.GetCount()){
		return clone(nvListF[id]);
	}
	return nv;
}
void ViewAntRec::Fill(const HcmIN& hcmIN){
	nvListF.Clear();
	cpList.Clear();
	xpList.Clear();
	for(int i = 0; i < idListF.GetCount(); i++){
		HcmField hField = hcmIN.GetField(idListF[i]);
		nvListF.Add(NameVal(hField.GetCode(), hField.GetName(), hField.GetStr()));
	}
	Vector<String> cpAngles = clone(hcmIN.GetField(51).GetVec());
	Vector<String> cpAtten  = clone(hcmIN.GetField(52).GetVec());
	int i=0;
	while(i<cpAngles.GetCount() && i<cpAtten.GetCount()){
		cpList.Add(TwoStrVal(cpAngles[i], cpAtten[i]));
		i++;
	}
	Vector<String> xpAngles = clone(hcmIN.GetField(55).GetVec());
	Vector<String> xpAtten  = clone(hcmIN.GetField(56).GetVec());
	i=0;
	while(i<xpAngles.GetCount() && i<xpAtten.GetCount()){
		xpList.Add(TwoStrVal(xpAngles[i], xpAtten[i]));
		i++;
	}
}
//!---------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------


class LayHcmHead: public WithLayHcmHead<TopWindow> {
public:
	typedef LayHcmHead CLASSNAME;
	WithLayHcmHead<ParentCtrl> layHcmHead;	// layout to place the control
	GridCtrl grHead;
	void ToCtrl(const HcmHeader& header);
	void FromCtrl(HcmHeader& header);
	void Clear();
	void Serialize(Stream& s);
	LayHcmHead();
protected:
	void Setup();
};
LayHcmHead::LayHcmHead(){
	CtrlLayout(*this, String(t_("")));
	Sizeable().Zoomable();
	layHcmHead.Add(grHead.SizePos());
	Add(layHcmHead.SizePos());
	Setup();
}
void LayHcmHead::Serialize(Stream& s){
	grHead.Serialize(s);
}
void LayHcmHead::Setup(){
	grHead.Clear(true);
	grHead.ColoringMode(1);
	grHead.OddColor().ColoringMode(1);
	grHead.EvenColor().ColoringMode(2);
	grHead.Indicator(); //---------------
	grHead.AddColumn("T_NAME", String("")).Min(250).AlignLeft().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grHead.AddColumn("T_VALUE", String("")).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grHead.RejectNullRow();
	grHead.Navigating();
	grHead.SetToolBar();
	grHead.Clipboard();
	grHead.Absolute();
	grHead.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10
}
void LayHcmHead::ToCtrl(const HcmHeader& header){
	grHead.Clear();
	for(int i=0; i < header.GetMap().GetCount(); i++){
		grHead.Add();
		NameVal nv = header.GetMap()[i].ToNV();
		grHead("T_NAME") = nv.name;
		grHead("T_VALUE") = nv.val;
		grHead.SetRowHeight(grHead.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5); // row height = GridCtrl.height * 1.5
	}
}
void LayHcmHead::FromCtrl(HcmHeader& header){
	for(int i = 0; i < grHead.GetRowCount(); i++){
		String str = grHead.GetCell(i, 1).val.ToString();
		header.EditVal(i, str);
	}
}
void LayHcmHead::Clear(){
	//TODO grHead.Clear();
}
//!---------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------

class LayHcmEntris : public WithLayHcmEntris<TopWindow> {
public:
	typedef LayHcmEntris CLASSNAME;
	WithLayHcmEntris<ParentCtrl> layHcmEntris;	// layout to place the control
	WithLayGridCaption<ParentCtrl> layCaption;
	FrameTop<WithLayGridCaption<ParentCtrl>> frameCaption;
	GridCtrl grHcmEntris;
	Label  lbCaption;
	void ToCtrl(const HcmPack& pack);
	void Clear();
	int GetCurSelID();
	void SetFirstRec();
	void Serialize(Stream& s);
	LayHcmEntris();
protected:
	void Setup();
};
LayHcmEntris::LayHcmEntris(){
	CtrlLayout(*this, String(t_("")));
	Sizeable().Zoomable();
	Add(layHcmEntris.SizePos());
	layHcmEntris.Add(grHcmEntris.SizePos());
	lbCaption.SetFrame(BlackFrame());
	layCaption.Add(lbCaption.SizePos());
	frameCaption.Add(layCaption.SizePos());
	frameCaption.Height(Single<ItemGrid>().headFont.GetHeight()*2); // !!! Высота фрейма заголовка = высота шрифта + 3 !!!
	grHcmEntris.AddFrame(frameCaption);
	lbCaption.SetFont(Single<ItemGrid>().headFont);
	lbCaption.SetText("");
	Setup();
}
void LayHcmEntris::Serialize(Stream& s){
	grHcmEntris.Serialize(s);
}
void LayHcmEntris::Setup(){
	grHcmEntris.Clear(true);
	grHcmEntris.ColoringMode(1);
	grHcmEntris.OddColor().ColoringMode(1);
	grHcmEntris.EvenColor().ColoringMode(2);
	grHcmEntris.Indicator(); //---------------
	grHcmEntris.AddColumn("T_No", String(t_("№"))).Min(50).AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grHcmEntris.AddColumn("T_NAME", String(t_("Station name"))).Min(150).AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grHcmEntris.AddColumn("T_Entry", String(t_("Entry"))).Min(50).AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grHcmEntris.AddColumn("T_Freq", String(t_("Frequency"))).Min(100).AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grHcmEntris.AddColumn("T_FUnit", String(t_("F. Unit"))).Min(100).AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grHcmEntris.AddColumn("T_CoordRef", String(t_("Coord. reference"))).Min(150).AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grHcmEntris.RejectNullRow();
	grHcmEntris.Navigating();
	grHcmEntris.SetToolBar();
	grHcmEntris.Clipboard();
	grHcmEntris.Absolute();
	grHcmEntris.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10
}
void LayHcmEntris::ToCtrl(const HcmPack& hcmPack){
	lbCaption.SetText(hcmPack.GetPackFile());
	grHcmEntris.Clear();
	for(int i=0; i<hcmPack.GetMap().GetCount(); i++){
		grHcmEntris.Add();
		grHcmEntris("T_NAME") = hcmPack.GetField(i, 8).GetStr();
		grHcmEntris("T_Entry") = hcmPack.GetField(i, 0).GetStr();
		grHcmEntris("T_Freq") = hcmPack.GetField(i, 1).GetStr();
		grHcmEntris("T_FUnit") = hcmPack.GetField(i, 2).GetStr();
		grHcmEntris("T_CoordRef") = hcmPack.GetField(i, 43).GetStr();
		grHcmEntris("T_No") = i;
		grHcmEntris.SetRowHeight(grHcmEntris.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5);// row height = GridCtrl.height * 1.5
	}
}
int LayHcmEntris::GetCurSelID(){
	if(!grHcmEntris.GetRowCount()) return -1;
	Value npp = grHcmEntris("T_No");
	if(!npp.IsNull()){
		return int(npp);
	}
	return -1;
}
void LayHcmEntris::SetFirstRec(){
	if(grHcmEntris.GetRowCount())
		grHcmEntris.SetCursor(0);
}
void LayHcmEntris::Clear(){
	grHcmEntris.Clear();
}
//!---------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------

class LayEntryGeneral: public WithLayEntryGeneral<TopWindow> {
public:
	typedef LayEntryGeneral CLASSNAME;
	WithLayEntryGeneral<ParentCtrl> layEntryGeneral;		// layout to place the control
	GridCtrl grEntryGeneral;
	void ToCtrl(const HcmIN& hcmIN);
	void Clear();
	void Serialize(Stream& s);
	LayEntryGeneral();
protected:
	ViewGeneralRec generalRec;
	void Setup();
};
LayEntryGeneral::LayEntryGeneral(){
	CtrlLayout(*this, String(t_("")));
	Sizeable().Zoomable();
	Add(layEntryGeneral.SizePos());
	layEntryGeneral.Add(grEntryGeneral.SizePos());
	Setup();
}
void LayEntryGeneral::Serialize(Stream& s){
	grEntryGeneral.Serialize(s);
}
void LayEntryGeneral::Setup(){
	grEntryGeneral.Clear(true);
	grEntryGeneral.ColoringMode(1);
	grEntryGeneral.OddColor().ColoringMode(1);
	grEntryGeneral.EvenColor().ColoringMode(2);
	grEntryGeneral.Indicator(); //---------------
	grEntryGeneral.AddColumn("T_CODE", String("")).Min(50).AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grEntryGeneral.AddColumn("T_NAME", String("")).Min(250).AlignLeft().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grEntryGeneral.AddColumn("T_VALUE", String("")).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grEntryGeneral.RejectNullRow();
	grEntryGeneral.Navigating();
	grEntryGeneral.SetToolBar();
	grEntryGeneral.Clipboard();
	grEntryGeneral.Absolute();
	grEntryGeneral.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10
}
void LayEntryGeneral::ToCtrl(const HcmIN& hcmIN){
	grEntryGeneral.Clear();
	generalRec.Fill(hcmIN);
	const Vector<NameVal>& nv = generalRec.GetListNV();
	for(int i=0; i < nv.GetCount(); i++){
		grEntryGeneral.Add();
		grEntryGeneral("T_CODE") = nv[i].code;
		grEntryGeneral("T_NAME") = nv[i].name;
		grEntryGeneral("T_VALUE") = nv[i].val;
		grEntryGeneral.SetRowHeight(grEntryGeneral.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5);// row height = GridCtrl.height * 1.5
	}
}
void LayEntryGeneral::Clear(){
	grEntryGeneral.Clear();
	generalRec.Clear();
}
//!---------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------

class LayEntryEquip : public WithLayEntryEquip<TopWindow> {
public:
	typedef LayEntryEquip CLASSNAME;
	WithLayEntryEquip<ParentCtrl> layEntryEquip;				// layout to place the control
	WithLayGridCaption<ParentCtrl> layCaption;				// layout для размещения grd_Title
	FrameTop<WithLayGridCaption<ParentCtrl>> frameCaption;
	GridCtrl grEquip;
	GridCtrl grFrecAtten;
	Splitter spl;
	Label  lbCaption;
	void ToCtrl(const HcmIN& hcmIN);
	void Clear();
	void Serialize(Stream& s);
	LayEntryEquip();
protected:
	ViewEquipRec equpRec;
	void Setup();
};
LayEntryEquip::LayEntryEquip(){
	CtrlLayout(*this, String(t_("")));
	Sizeable().Zoomable();
	Add(layEntryEquip.SizePos());
	layEntryEquip.Add(spl.Vert(grEquip.SizePos(), grFrecAtten.SizePos()));
	lbCaption.SetFrame(BlackFrame());
	layCaption.Add(lbCaption.SizePos());
	frameCaption.Add(layCaption.SizePos());
	frameCaption.Height(Single<ItemGrid>().headFont.GetHeight()*2);
	grFrecAtten.AddFrame(frameCaption);
	lbCaption.SetFont(Single<ItemGrid>().headFont);
	lbCaption.SetText(t_(" Transmitter spectrum mask or receiver selectivity mask:"));
	Setup();
}
void LayEntryEquip::Serialize(Stream& s){
	grEquip.Serialize(s);
	grFrecAtten.Serialize(s);
	spl.Serialize(s);
}
void LayEntryEquip::Setup(){
	grEquip.Clear(true);
	grEquip.ColoringMode(1);
	grEquip.OddColor().ColoringMode(1);
	grEquip.EvenColor().ColoringMode(2);
	grEquip.Indicator(); //---------------
	grEquip.AddColumn("T_CODE", String("")).Min(50).AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grEquip.AddColumn("T_NAME", String("")).Min(250).AlignLeft().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grEquip.AddColumn("T_VALUE", String("")).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grEquip.RejectNullRow();
	grEquip.Navigating();
	grEquip.SetToolBar();
	grEquip.Clipboard();
	grEquip.Absolute();
	grEquip.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10
	// --------------------
	grFrecAtten.Clear(true);
	grFrecAtten.ColoringMode(1);
	grFrecAtten.OddColor().ColoringMode(1);
	grFrecAtten.EvenColor().ColoringMode(2);
	grFrecAtten.Indicator(); //---------------
	grFrecAtten.AddColumn("T_FREQ_CODE", String(t_("F_CODE"))).Hidden().Min(50).AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grFrecAtten.AddColumn("T_FREQ", String(t_("Frequency, MHz"))).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grFrecAtten.AddColumn("T_ATTEN_CODE", String(t_("A_CODE"))).Hidden().Min(50).AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grFrecAtten.AddColumn("T_ATTEN", String(t_("Attenuation, bB"))).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grFrecAtten.RejectNullRow();
	grFrecAtten.Navigating();
	grFrecAtten.SetToolBar();
	grFrecAtten.Clipboard();
	grFrecAtten.Absolute();
	grFrecAtten.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10
}
void LayEntryEquip::ToCtrl(const HcmIN& hcmIN){
	grEquip.Clear();
	grFrecAtten.Clear();
	equpRec.Fill(hcmIN);
	const Vector<NameVal>& nv = equpRec.GetListNV();
	for(int i=0; i < nv.GetCount(); i++){
		grEquip.Add();
		grEquip("T_CODE") = nv[i].code;
		grEquip("T_NAME") = nv[i].name;
		grEquip("T_VALUE") = nv[i].val;
		grEquip.SetRowHeight(grEquip.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5);// row height = GridCtrl.height * 1.5
	}
	const Vector<TwoCodeVal>& vv = equpRec.GetListVV();
	for(int i=0; i < vv.GetCount(); i++){
		grFrecAtten.Add();
		grFrecAtten("T_FREQ_CODE") = vv[i].code_1;
		grFrecAtten("T_FREQ") = vv[i].val_1;
		grFrecAtten("T_ATTEN_CODE") = vv[i].code_2;
		grFrecAtten("T_ATTEN") = vv[i].val_2;
		grFrecAtten.SetRowHeight(grFrecAtten.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5);// row height = GridCtrl.height * 1.5
	}
}
void LayEntryEquip::Clear(){
	grEquip.Clear();
	grFrecAtten.Clear();
	equpRec.Clear();
}
//!---------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------

class LayEntryAnt : public WithLayEntryAnt<TopWindow> {
public:
	typedef LayEntryAnt CLASSNAME;
	WithLayEntryAnt<ParentCtrl> layEntryAnt;				// layout to place the control
	GridCtrl grAnt;
	Splitter spl;
	TabCtrl tab;
	GridCtrl grCP;
	GridCtrl grXP;
	void ToCtrl(const HcmIN& hcmIN);
	void Clear();
	void Serialize(Stream& s);
	LayEntryAnt();
protected:
	ViewAntRec antRec ;
	void Setup();
};
LayEntryAnt::LayEntryAnt(){
	CtrlLayout(*this, String(t_("")));
	Sizeable().Zoomable();
	Add(layEntryAnt.SizePos());
	layEntryAnt.Add(spl.Vert(grAnt.SizePos(), tab.SizePos()));
	tab.Add(grCP.SizePos(), t_("\1[ [ [3*!Arial! Co-polar]]]"));
	tab.Add(grXP.SizePos(), t_("\1[ [ [3*!Arial! Cross-polar]]]"));
	Setup();
}
void LayEntryAnt::Serialize(Stream& s){
	spl.Serialize(s);
	grAnt.Serialize(s);
	spl.Serialize(s);
	tab.Serialize(s);
	grCP.Serialize(s);
	grXP.Serialize(s);
}
void LayEntryAnt::Setup(){
	grAnt.Clear(true);
	grAnt.ColoringMode(1);
	grAnt.OddColor().ColoringMode(1);
	grAnt.EvenColor().ColoringMode(2);
	grAnt.Indicator(); //---------------
	grAnt.AddColumn("T_CODE", String("")).Min(50).AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grAnt.AddColumn("T_NAME", String("")).Min(250).AlignLeft().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grAnt.AddColumn("T_VALUE", String("")).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grAnt.RejectNullRow();
	grAnt.Navigating();
	grAnt.SetToolBar();
	grAnt.Clipboard();
	grAnt.Absolute();
	grAnt.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10

	grCP.Clear(true);
	grCP.ColoringMode(1);
	grCP.OddColor().ColoringMode(1);
	grCP.EvenColor().ColoringMode(2);
	grCP.Indicator(); //---------------
	grCP.AddColumn("CP_ANGLE", String(t_("Angle"))).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grCP.AddColumn("CP_ATTEN", String(t_("Attenuation"))).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grCP.RejectNullRow();
	grCP.Navigating();
	grCP.SetToolBar();
	grCP.Clipboard();
	grCP.Absolute();
	grCP.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10

	grXP.Clear(true);
	grXP.ColoringMode(1);
	grXP.OddColor().ColoringMode(1);
	grXP.EvenColor().ColoringMode(2);
	grXP.Indicator(); //---------------
	grXP.AddColumn("XP_ANGLE", String(t_("Angle"))).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grXP.AddColumn("XP_ATTEN", String(t_("Attenuation"))).Min(250).AlignCenter().SetDisplay(Single<ItemGrid>()).HeaderAlignCenter().SetHeaderFont(Single<ItemGrid>().headFont);
	grXP.RejectNullRow();
	grXP.Navigating();
	grXP.SetToolBar();
	grXP.Clipboard();
	grXP.Absolute();
	grXP.SetRowHeight(0, Single<ItemGrid>().headFont.GetHeight()+10); // header height = GridCtrl.height + 10
}

void LayEntryAnt::ToCtrl(const HcmIN& hcmIN){
	grAnt.Clear();
	antRec.Fill(hcmIN);
	const Vector<NameVal>& nv = antRec.GetListNV();
	for(int i=0; i < nv.GetCount(); i++){
		grAnt.Add();
		grAnt("T_CODE") = nv[i].code;
		grAnt("T_NAME") = nv[i].name;
		grAnt("T_VALUE") = nv[i].val;
		grAnt.SetRowHeight(grAnt.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5);// row height = GridCtrl.height * 1.5
	}
	grCP.Clear();
	const Vector<TwoStrVal>& cp = antRec.GetListCP();
	for(int i=0; i < cp.GetCount(); i++){
		grCP.Add();
		grCP("CP_ANGLE") = cp[i].val_1;
		grCP("CP_ATTEN") = cp[i].val_2;
		grCP.SetRowHeight(grCP.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5);// row height = GridCtrl.height * 1.5
	}
	grXP.Clear();
	const Vector<TwoStrVal>& xp = antRec.GetListXP();
	for(int i=0; i < xp.GetCount(); i++){
		grXP.Add();
		grXP("XP_ANGLE") = xp[i].val_1;
		grXP("XP_ATTEN") = xp[i].val_2;
		grXP.SetRowHeight(grXP.GetRowCount(), Single<ItemGrid>().itemFont.GetHeight()*1.5);// row height = GridCtrl.height * 1.5
	}
}
void LayEntryAnt::Clear(){
	grAnt.Clear();
	grCP.Clear();
	grXP.Clear();
	antRec.Clear();
}
//!---------------------------------------------------------------------------------------
//!---------------------------------------------------------------------------------------

class LayHcmPack : public WithLayHcmPack<TopWindow> {
public:
	typedef LayHcmPack CLASSNAME;
	WithLayHcmPack<ParentCtrl> layHcmPack;
	LayHcmEntris layHcmEntris;
	LayHcmHead layHcmHead;
	LayEntryGeneral layEntryGeneral;
	LayEntryEquip layEntryEquip;
	LayEntryAnt layEntryAnt;
	Splitter spl;
	TabCtrl tab;
	Callback cbSinglCalc;
	
	void GrEntrisPopMenu(Bar& bar);
	void GrGeneralPopMenu(Bar& bar);
	void GrHeaderPopMenu(Bar& bar);
	void GrEquipPopMenu(Bar& bar);
	void GrEquipFAPopMenu(Bar& bar);
	void GrAntPopMenu(Bar& bar);
	void GrAntCPPopMenu(Bar& bar);
	void GrAntXPPopMenu(Bar& bar);

	void ToCtrl();
	void EditEntry();
	void AddEntry();
	void CloneEntry();
	void ViewEntry();
	void DelEntry();
	void EditGeneral();
	void EditHeader();
	void EditEquip();
	void EditEquipFA();
	void EditAnt();
	void AddAntCP();
	void AddAntXP();
	void DelAntCP();
	void DelAntXP();

	const HcmPack& GetHcmPack()const{return hcmPack;};
	bool CheckHcmPack(String& slog);

	void OpenHcmPack();
	bool OpenHcmPack(const String& fileName);

	void CloseHcmPack();
	void SaveHcmPack();
	void SaveHcmPackAs();
	void NewHcmPack();

	void Clear();
	void Serialize(Stream& s);
	LayHcmPack();
protected:
	bool isOpen;
	HcmPack hcmPack;
	VectorMap<String, String> seriaMap;
	void Setup();
	void OnChangeEntry();
};
LayHcmPack::LayHcmPack(){
	CtrlLayout(*this, String(t_("")));
	Sizeable().Zoomable();
	Add(layHcmPack.SizePos());
	layHcmPack.Add(spl.Horz(layHcmEntris.SizePos(), tab.SizePos()));
	tab.Add(layHcmHead.SizePos(), t_("\1[ [ [3*!Arial! HCM-FS file header]]]"));
	tab.Add(layEntryGeneral.SizePos(), t_("\1[ [ [3*!Arial! General]]]"));
	tab.Add(layEntryEquip.SizePos(), t_("\1[ [ [3*!Arial! Equipment]]]"));
	tab.Add(layEntryAnt.SizePos(), t_("\1[ [ [3*!Arial! Antenna]]]"));
	isOpen = false;
	Setup();
}
void LayHcmPack::Setup(){
	layHcmEntris.grHcmEntris.WhenChangeRow = [=] { OnChangeEntry(); };
	layHcmEntris.grHcmEntris.WhenMenuBar = THISBACK(GrEntrisPopMenu);
	layEntryGeneral.grEntryGeneral.WhenMenuBar = THISBACK(GrGeneralPopMenu);
	layHcmHead.grHead.WhenMenuBar = THISBACK(GrHeaderPopMenu);
	layEntryEquip.grEquip.WhenMenuBar = THISBACK(GrEquipPopMenu);
	layEntryEquip.grFrecAtten.WhenMenuBar = THISBACK(GrEquipFAPopMenu);
	layEntryAnt.grAnt.WhenMenuBar = THISBACK(GrAntPopMenu);
	layEntryAnt.grCP.WhenMenuBar = THISBACK(GrAntCPPopMenu);
	layEntryAnt.grXP.WhenMenuBar = THISBACK(GrAntXPPopMenu);
}
void LayHcmPack::Serialize(Stream& s){
	spl.Serialize(s);
	// tab.Serialize(s);
	layHcmEntris.Serialize(s);
	layHcmHead.Serialize(s);
	layEntryGeneral.Serialize(s);
	layEntryEquip.Serialize(s);
	layEntryAnt.Serialize(s);
	seriaMap.Serialize(s);
}
void LayHcmPack::GrEntrisPopMenu(Bar& bar){
	bar.Add(t_("Edit .."), Images::EDIT(), THISBACK(EditEntry));
	bar.Add(t_("Add .."), Images::ADD(), THISBACK(AddEntry));
	bar.Add(t_("Copy .."), Images::CLONE(), THISBACK(CloneEntry));
	bar.Separator();
	bar.Add(t_("Del .."), Images::DEL(), THISBACK(DelEntry));
}
void LayHcmPack::GrGeneralPopMenu(Bar& bar){
	bar.Add(t_("Edit .."), Images::EDIT(), THISBACK(EditGeneral));
}
void LayHcmPack::GrHeaderPopMenu(Bar& bar){
	bar.Add(t_("Edit .."), Images::EDIT(), THISBACK(EditHeader));
}
void LayHcmPack::GrEquipPopMenu(Bar& bar){
	bar.Add(t_("Edit .."), Images::EDIT(), THISBACK(EditEquip));
}
void LayHcmPack::GrEquipFAPopMenu(Bar& bar){
	bar.Add(t_("Edit .."), Images::EDIT(), THISBACK(EditEquipFA));
}
void LayHcmPack::GrAntPopMenu(Bar& bar){
	bar.Add(t_("Edit .."), Images::EDIT(), THISBACK(EditAnt));
}
void LayHcmPack::GrAntCPPopMenu(Bar& bar){
	bar.Add(t_("Add/Edit .."), Images::ADD(), THISBACK(AddAntCP));
	bar.Separator();
	bar.Add(t_("Del .."), Images::DEL(), THISBACK(DelAntCP));
}
void LayHcmPack::GrAntXPPopMenu(Bar& bar){
	bar.Add(t_("Add/Edit .."), Images::ADD(), THISBACK(AddAntXP));
	bar.Separator();
	bar.Add(t_("Del .."), Images::DEL(), THISBACK(DelAntXP));
}
void LayHcmPack::Clear(){
	layHcmEntris.Clear();
	layHcmHead.Clear();
	layEntryGeneral.Clear();
	layEntryEquip.Clear();
	layEntryAnt.Clear();
}
void LayHcmPack::ToCtrl(){
	layHcmEntris.ToCtrl(hcmPack);
	layHcmHead.ToCtrl(hcmPack.GetHeader());
	layHcmEntris.SetFirstRec();
}
void LayHcmPack::OnChangeEntry(){
	int idx =  layHcmEntris.GetCurSelID();
	if(idx != -1){
		layEntryGeneral.ToCtrl(hcmPack.GetMap()[idx]);
		layEntryEquip.ToCtrl(hcmPack.GetMap()[idx]);
		layEntryAnt.ToCtrl(hcmPack.GetMap()[idx]);
	}
	else{
		layEntryGeneral.Clear();
		layEntryEquip.Clear();
		layEntryAnt.Clear();
	}
}
//!---------------------------------------------------------------------------------------------

bool LayHcmPack::OpenHcmPack(const String& fileName){
	// if(!hcmPack.GetPackFile().IsEmpty() && hcmPack.GetPackFile() == fileName) return isOpen;
	CloseHcmPack();
	String ext = GetFileExt(fileName);
	isOpen = false;
	if(ext == ".json"){
		isOpen = hcmPack.FromJson(fileName);
	}else if(ext == ".xlsx"){
		isOpen = hcmPack.FromXLS(fileName);
	}else if(ext == ".txt"){
		isOpen = hcmPack.FromCSV(fileName);
	}
	if(isOpen) ToCtrl();
	return isOpen;
}
void LayHcmPack::OpenHcmPack(){
	FileSel fs;
	fs.ReadOnlyOption(true);
	fs.MkDirOption(false);
	String wDir = Single<AppCFG>().cfgPathRec.GetWorkDir();
	if(wDir.IsEmpty() || !DirectoryExists(wDir))
		wDir = GetExeFolder();
	fs.ActiveDir(wDir);
	fs.Types("Text files\t*.txt\nExcel files\t*.xlsx");
	String fileName = "";
	if(fs.ExecuteOpen()){
		fileName = fs;
		if(!fileName.IsEmpty()){
			// if(!hcmPack.GetPackFile().IsEmpty() && hcmPack.GetPackFile() == fileName) return;
			CloseHcmPack();
			Single<AppCFG>().cfgPathRec.SetWorkDir(GetFileFolder(fileName));
			isOpen = false;
			if(fs.GetActiveType()==0){
				isOpen = hcmPack.FromCSV(fileName);
			}
			if(fs.GetActiveType()==1){
				isOpen = hcmPack.FromXLS(fileName);
			}
			if(fs.GetActiveType()==2){
				isOpen = hcmPack.FromJson(fileName);
			}
			if(!isOpen){
				Clear();
				Exclamation(DeQtf(Format("Error opening file: \"%s\"\nThe selected file is not an hcm data file", fileName)));
				return;
			}
			ToCtrl();
		}
	}
}
//!---------------------------------------------------------------------------------------------

void LayHcmPack::SaveHcmPack(){
	if(hcmPack.IsEmpty()) return;
	if(hcmPack.GetPackFile().IsEmpty())
		SaveHcmPackAs();
	else{
		switch(hcmPack.GetFormat()){
			case HcmPackFormat::TXT:
				hcmPack.ToCSV();
				hcmPack.SetModified(false);
				break;
			case HcmPackFormat::JSON:
				hcmPack.ToJson();
				hcmPack.SetModified(false);
				break;
			case HcmPackFormat::XLS:
				hcmPack.ToXLS();
				hcmPack.SetModified(false);
				break;
		}
	}
}
void LayHcmPack::SaveHcmPackAs(){
	if(hcmPack.IsEmpty()) return;
	FileSel fs;
	fs.ReadOnlyOption(true);
	fs.MkDirOption(false);
	fs.ActiveDir(GetExeFolder());
	fs.EditFileName(true);
	fs.Types("Text files\t*.txt\nExcel files\t*.xlsx");
	if(fs.ExecuteSaveAs()){
		const String fileName = fs;
		if(fs.GetActiveType()==0){
			hcmPack.ToCSV(fileName);
			hcmPack.SetModified(false);
		}
		if(fs.GetActiveType()==1){
			hcmPack.ToXLS(fileName);
			hcmPack.SetModified(false);
		}
		if(fs.GetActiveType()==2){
			hcmPack.ToJson(fileName);
			hcmPack.SetModified(false);
		}
	}
}
bool LayHcmPack::CheckHcmPack(String& slog){
	slog << "Header" << ":\n";
	hcmPack.GetHeader().Check(slog);
	for(auto v: hcmPack.GetMap()){
		slog << Format("%s \"%s\":\n", HcmEntityToStr(v.GetEntity()), v.GetField("4A").GetStr());
		v.Check(slog);
	}
	return true;
}

void LayHcmPack::CloseHcmPack(){
	if(hcmPack.IsEmpty()) return;
	if(hcmPack.IsModified()){
		if(PromptYesNo(t_("Do you want to save the changes to the HCM file?"))){
	        SaveHcmPack();
        }
    }
    hcmPack.Clear();
    hcmPack.GetHeader().Clear();
    ToCtrl();
	OnChangeEntry();
}
void LayHcmPack::NewHcmPack(){
	CloseHcmPack();
	layHcmHead.ToCtrl(hcmPack.GetHeader());
}
//!---------------------------------------------------------------------------------------------

void LayHcmPack::EditEntry(){
	EditEntryRec rec;
	OneValGridEditDlg dlg(Single<AppCFG>().cfgEditRec);
	dlg.BeforeFrmShow("EditEntry", seriaMap);
	int idx =  layHcmEntris.GetCurSelID();
	if(idx != -1){
		rec.Load(hcmPack.GetHcmIN(idx));
		dlg.ToCtrl(rec);
		// ActiveFocus(dlg);
		if(dlg.Execute() == IDOK){
			dlg.FromCtrl(rec);
			rec.Save(hcmPack.GetHcmIN(idx));
			hcmPack.SetModified(true);
			layHcmEntris.ToCtrl(hcmPack);
			layEntryGeneral.ToCtrl(hcmPack.GetMap()[idx]);
		}
	}
	dlg.BeforeFrmClose("EditEntry", seriaMap);
}
void LayHcmPack::AddEntry(){
	hcmPack.Add();
	layHcmEntris.ToCtrl(hcmPack);
	layEntryGeneral.Clear();
	layEntryEquip.Clear();
	layEntryAnt.Clear();
}
void LayHcmPack::CloneEntry(){
	int idx =  layHcmEntris.GetCurSelID();
	if(idx != -1){
		hcmPack.Add(hcmPack.GetHcmIN(idx));
		layHcmEntris.ToCtrl(hcmPack);
	}
}

void LayHcmPack::ViewEntry(){
	int idx =  layHcmEntris.GetCurSelID();
	if(idx != -1){
		FormReport formReport;
		formReport.de_Report.SetData(hcmPack.GetHcmIN(idx).ToString());
		formReport.Run();
	}
}

void LayHcmPack::DelEntry(){
	int idx =  layHcmEntris.GetCurSelID();
	if(idx != -1){
		if(PromptYesNo(t_("Delete an entry?"))){
			bool isDel = hcmPack.Delete(idx);
			if(!isDel){
				PromptOK(t_("Сan't delete an entry!"));
				return;
			}
			layHcmEntris.Clear();
			layHcmEntris.ToCtrl(hcmPack);
			layEntryGeneral.Clear();
			layEntryEquip.Clear();
			layEntryAnt.Clear();
		}
	}
}

void LayHcmPack::EditHeader(){
	EditHeadRec rec;
	OneValGridEditDlg dlg(Single<AppCFG>().cfgEditRec);
	dlg.BeforeFrmShow("EditHeader", seriaMap);
	rec.Load(hcmPack.GetHeader());
	dlg.ToCtrl(rec);
	if(dlg.Execute() == IDOK){
		dlg.FromCtrl(rec);
		rec.Save(hcmPack.GetHeader());
		hcmPack.SetModified(true);
		layHcmHead.ToCtrl(hcmPack.GetHeader());
	}
	dlg.BeforeFrmClose("EditHeader", seriaMap);
}

void LayHcmPack::EditGeneral(){
	EditGeneralRec rec;
	OneValGridEditDlg dlg(Single<AppCFG>().cfgEditRec);
	dlg.BeforeFrmShow("EditGeneral", seriaMap);
	int idx =  layHcmEntris.GetCurSelID();
	if(idx != -1){
		rec.Load(hcmPack.GetHcmIN(idx));
		dlg.ToCtrl(rec);
		if(dlg.Execute() == IDOK){
			dlg.FromCtrl(rec);
			rec.Save(hcmPack.GetHcmIN(idx));
			hcmPack.SetModified(true);
			layEntryGeneral.ToCtrl(hcmPack.GetMap()[idx]);
			layHcmEntris.ToCtrl(hcmPack);
		}
	}
	dlg.BeforeFrmClose("EditGeneral", seriaMap);
}

void LayHcmPack::EditEquip(){
	EditEquipRec rec;
	OneValGridEditDlg dlg(Single<AppCFG>().cfgEditRec);
	dlg.BeforeFrmShow("EditEquip", seriaMap);
	int idx =  layHcmEntris.GetCurSelID();
	if(idx != -1){
		rec.Load(hcmPack.GetHcmIN(idx));
		dlg.ToCtrl(rec);
		if(dlg.Execute() == IDOK){
			dlg.FromCtrl(rec);
			rec.Save(hcmPack.GetHcmIN(idx));
			hcmPack.SetModified(true);
			layEntryEquip.ToCtrl(hcmPack.GetMap()[idx]);
		}
	}
	dlg.BeforeFrmClose("EditEquip", seriaMap);
}

void LayHcmPack::EditEquipFA(){
	EditEqFARec rec;
	TwoValGridEditDlg dlg;
	dlg.BeforeFrmShow("EditEquipFA", seriaMap);
	int idx =  layHcmEntris.GetCurSelID();
	if(idx != -1){
		rec.Load(hcmPack.GetHcmIN(idx));
		dlg.ToCtrl(rec);
		if(dlg.Execute() == IDOK){
			dlg.FromCtrl(rec);
			rec.Save(hcmPack.GetHcmIN(idx));
			hcmPack.SetModified(true);
			layEntryEquip.ToCtrl(hcmPack.GetMap()[idx]);
		}
	}
	dlg.BeforeFrmClose("EditEquipFA", seriaMap);
}

void LayHcmPack::EditAnt(){
	EditAntRec rec;
	OneValGridEditDlg dlg(Single<AppCFG>().cfgEditRec);
	dlg.BeforeFrmShow("EditAnt", seriaMap);
	int idx =  layHcmEntris.GetCurSelID();
	if(idx != -1){
		rec.Load(hcmPack.GetHcmIN(idx));
		dlg.ToCtrl(rec);
		if(dlg.Execute() == IDOK){
			dlg.FromCtrl(rec);
			rec.Save(hcmPack.GetHcmIN(idx));
			hcmPack.SetModified(true);
			layEntryAnt.ToCtrl(hcmPack.GetMap()[idx]);
		}
	}
	dlg.BeforeFrmClose("EditAnt", seriaMap);
}

void LayHcmPack::AddAntCP(){
	TwoDblVal tdVal;
	EditAntCpRec rec;
	AntCpXpEditDlg dlg;
	dlg.BeforeFrmShow("EditAntCP", seriaMap);
	int idx =  layHcmEntris.GetCurSelID();
	if(idx != -1){
		if(dlg.IsCurSel())
			rec.Load(EditState::Add, hcmPack.GetHcmIN(idx), TwoStrVal(layEntryAnt.grCP("CP_ANGLE").ToString(), layEntryAnt.grCP("CP_ATTEN").ToString()));
		else
			rec.Load(EditState::Add, hcmPack.GetHcmIN(idx), TwoStrVal("0", "0"));
		dlg.ToCtrl(rec);
		if(dlg.Execute() == IDOK){
			dlg.FromCtrl(rec);
			rec.Save(hcmPack.GetHcmIN(idx));
			hcmPack.SetModified(true);
			layEntryAnt.ToCtrl(hcmPack.GetMap()[idx]);
		}
	}
	dlg.BeforeFrmClose("EditAntCP", seriaMap);
}
void LayHcmPack::DelAntCP(){
	int idx =  layHcmEntris.GetCurSelID();
	if(idx != -1 && layEntryAnt.grCP.GetRowCount()){
		if(PromptYesNo(t_("Delete?"))){
			EditAntCpRec rec;
			rec.Load(EditState::Del, hcmPack.GetHcmIN(idx), TwoStrVal(layEntryAnt.grCP("CP_ANGLE").ToString(), layEntryAnt.grCP("CP_ATTEN").ToString()));
			rec.Save(hcmPack.GetHcmIN(idx));
			hcmPack.SetModified(true);
			layEntryAnt.ToCtrl(hcmPack.GetMap()[idx]);
		}
	}
}

void LayHcmPack::AddAntXP(){
	TwoDblVal tdVal;
	EditAntXpRec rec;
	AntCpXpEditDlg dlg;
	dlg.BeforeFrmShow("EditAntXP", seriaMap);
	int idx =  layHcmEntris.GetCurSelID();
	if(idx != -1){
		if(dlg.IsCurSel())
			rec.Load(EditState::Add, hcmPack.GetHcmIN(idx), TwoStrVal(layEntryAnt.grXP("XP_ANGLE").ToString(), layEntryAnt.grXP("XP_ATTEN").ToString()));
		else
			rec.Load(EditState::Add, hcmPack.GetHcmIN(idx), TwoStrVal("0", "0"));
		dlg.ToCtrl(rec);
		if(dlg.Execute() == IDOK){
			dlg.FromCtrl(rec);
			rec.Save(hcmPack.GetHcmIN(idx));
			hcmPack.SetModified(true);
			layEntryAnt.ToCtrl(hcmPack.GetMap()[idx]);
		}
	}
	dlg.BeforeFrmClose("EditAntXP", seriaMap);
}
void LayHcmPack::DelAntXP(){
	int idx =  layHcmEntris.GetCurSelID();
	if(idx != -1 && layEntryAnt.grXP.GetRowCount()){
		if(PromptYesNo(t_("Delete?"))){
			EditAntXpRec rec;
			rec.Load(EditState::Del, hcmPack.GetHcmIN(idx), TwoStrVal(layEntryAnt.grXP("XP_ANGLE").ToString(), layEntryAnt.grXP("XP_ATTEN").ToString()));
			rec.Save(hcmPack.GetHcmIN(idx));
			hcmPack.SetModified(true);
			layEntryAnt.ToCtrl(hcmPack.GetMap()[idx]);
		}
	}
}
//!---------------------------------------------------------------------------------------------

// форма, редактирования
struct OpenHcmDlg : public WithFrmOpenHcm<TopWindow> {
	void InitCtrl(){
		ed_FileTest.SetFont(Single<ItemGrid>().headFont);
		ed_FileRef.SetFont(Single<ItemGrid>().headFont);
	}
	void ToCtrl(){
		ed_FileTest.SetData(appCFG.cfgCalcRec.fPackTest);
		ed_FileRef.SetData(appCFG.cfgCalcRec.fPackRef);
	}
	void FromCtrl(){
		appCFG.cfgCalcRec.fPackTest = ed_FileTest.GetData();
		appCFG.cfgCalcRec.fPackRef =  ed_FileRef.GetData();
	}
	String SelFile(const String& dir){
		FileSel fs;
		fs.ReadOnlyOption(true);
		fs.MkDirOption(false);
		if(DirectoryExists(dir))
			fs.ActiveDir(dir);
		else if(DirectoryExists(appCFG.cfgPathRec.GetWorkDir()))
			fs.ActiveDir(appCFG.cfgPathRec.GetWorkDir());
		else
			fs.ActiveDir(GetExeFolder());
		fs.Types("Text files\t*.txt\nExcel files\t*.xlsx");
		if(fs.ExecuteOpen()){
			appCFG.cfgPathRec.SetWorkDir(GetFileFolder(fs.Get()));
			return fs;
		}
		else
			return "";
	}
	void SetPackTest(){
		appCFG.cfgCalcRec.fPackTest = SelFile(testDir);
		testDir = GetFileFolder(appCFG.cfgCalcRec.fPackTest);
		ed_FileTest.SetData(appCFG.cfgCalcRec.fPackTest);
	}
	void SetPackRef(){
		appCFG.cfgCalcRec.fPackRef = SelFile(refDir);
		refDir = GetFileFolder(appCFG.cfgCalcRec.fPackRef);
		ed_FileRef.SetData(appCFG.cfgCalcRec.fPackRef);
	}
	OpenHcmDlg(AppCFG& appCFG):appCFG(appCFG){
		CtrlLayoutOKCancel(*this, t_("Chose an HCM data file"));
		//--------------------------------------------------------------------------------------
		InitCtrl();
		ToCtrl();
		refDir = GetFileFolder(appCFG.cfgCalcRec.fPackRef);
		testDir = GetFileFolder(appCFG.cfgCalcRec.fPackTest);
		//--------------------------------------------------------------------------------------
		btn_SelFileTest.SetImage(Images::SEL());
		btn_SelFileRef.SetImage(Images::SEL());
		btn_SelFileTest.WhenAction = [=] {SetPackTest();};
		btn_SelFileRef.WhenAction = [=] {SetPackRef();};
	}
protected:
	AppCFG& appCFG;
	String testDir;
	String refDir;
};
//!---------------------------------------------------------------------------------------




#endif
