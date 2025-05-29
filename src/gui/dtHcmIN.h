#ifndef __HCM4A_CalcFs4A_dtHcmIN_h_
#define __HCM4A_CalcFs4A_dtHcmIN_h_

#include <Core/Core.h>
#include <plugin/pcre/Pcre.h> // RegExp
#include <Xlnt/Xlnt.h>

#include "common.h"

using namespace Upp;

namespace HCMFS{

enum class FldMandat {OFF = 0, ON, TX, RX, IF};
inline FldMandat IntToFldMandat(int z){
	if(z == static_cast<int>(FldMandat::OFF)) return FldMandat::OFF;
	if(z == static_cast<int>(FldMandat::ON))  return FldMandat::ON;
	if(z == static_cast<int>(FldMandat::TX))  return FldMandat::TX;
	if(z == static_cast<int>(FldMandat::RX))  return FldMandat::RX;
	if(z == static_cast<int>(FldMandat::IF))  return FldMandat::IF;
	return FldMandat::OFF;
}
inline int FldMandatToInt(FldMandat z){
	return static_cast<int>(z);
}
inline String FldMandatToStr(FldMandat z){
	String rStr = "OFF";
	switch(z){
		case FldMandat::OFF:
			rStr = "OFF";
			break;
		case FldMandat::ON:
			rStr = "ON";
			break;
		case FldMandat::TX:
			rStr = "TX";
			break;
		case FldMandat::RX:
			rStr = "RX";
			break;
		case FldMandat::IF:
			rStr = "IF";
			break;
	}
	return rStr;
}
inline FldMandat StrToFldMandat(const String& str){
	if(ToUpper(str) == "OFF")	return FldMandat::OFF;
	if(ToUpper(str) == "ON")	return FldMandat::ON;
	if(ToUpper(str) == "TX")	return FldMandat::TX;
	if(ToUpper(str) == "RX")	return FldMandat::RX;
	if(ToUpper(str) == "IF")	return FldMandat::IF;
	return FldMandat::OFF;
}
inline bool IsFldMandat(const String& str){
	return
		(StrToFldMandat(ToUpper(str)) == FldMandat::TX)||
		(StrToFldMandat(ToUpper(str)) == FldMandat::RX)||
		(StrToFldMandat(ToUpper(str)) == FldMandat::OFF)||
		(StrToFldMandat(ToUpper(str)) == FldMandat::IF)||
		(StrToFldMandat(ToUpper(str)) == FldMandat::ON);
}
//! --------------------------------------------------------------------------------------------

enum class HcmEntity {None, HEADER, TX, PRX, PTX, RX};
inline HcmEntity IntToHcmEntity(int z){
	if(z == static_cast<int>(HcmEntity::TX))  return HcmEntity::TX;
	if(z == static_cast<int>(HcmEntity::RX))  return HcmEntity::RX;
	if(z == static_cast<int>(HcmEntity::PRX)) return HcmEntity::PRX;
	if(z == static_cast<int>(HcmEntity::PTX)) return HcmEntity::PTX;
	return HcmEntity::None;
}
inline int HcmEntityToInt(HcmEntity z){
	return static_cast<int>(z);
}
inline String HcmEntityToStr(HcmEntity z){
	String rStr = "None";
	switch(z){
		case HcmEntity::HEADER:
			rStr = "HEADER";
			break;
		case HcmEntity::TX:
			rStr = "TX";
			break;
		case HcmEntity::RX:
			rStr = "RX";
			break;
		case HcmEntity::PRX:
			rStr = "PRX";
			break;
		case HcmEntity::PTX:
			rStr = "PTX";
			break;
		case HcmEntity::None:
			rStr = "None";
			break;
	}
	return rStr;
}
inline HcmEntity StrToHcmEntity(const String& str){
	if(ToUpper(str) == "HEADER")return HcmEntity::HEADER;
	if(ToUpper(str) == "TX")	return HcmEntity::TX;
	if(ToUpper(str) == "RX")	return HcmEntity::RX;
	if(ToUpper(str) == "PRX")	return HcmEntity::PRX;
	if(ToUpper(str) == "PTX")	return HcmEntity::PTX;
	return HcmEntity::None;
}
inline bool IsHcmEntity(const String& str){
	return
		(StrToHcmEntity(ToUpper(str)) == HcmEntity::TX)||
		(StrToHcmEntity(ToUpper(str)) == HcmEntity::RX)||
		(StrToHcmEntity(ToUpper(str)) == HcmEntity::PRX)||
		(StrToHcmEntity(ToUpper(str)) == HcmEntity::PTX);
}
inline HcmEntity ToHcmEntity(bool isTx, bool isPassive){
	if(isTx && !isPassive)  return HcmEntity::TX;
	if(isTx && isPassive)   return HcmEntity::PTX;
	if(!isTx && isPassive)  return HcmEntity::PRX;
	if(!isTx && !isPassive) return HcmEntity::RX;
	return HcmEntity::None;
}
//! --------------------------------------------------------------------------------------------

enum class FldType {STR, INT, FLOAT, DATE, VEC};
inline FldType IntToFldType(int z){
	if(z == static_cast<int>(FldType::STR))   return FldType::STR;
	if(z == static_cast<int>(FldType::INT))   return FldType::INT;
	if(z == static_cast<int>(FldType::FLOAT)) return FldType::FLOAT;
	if(z == static_cast<int>(FldType::DATE))  return FldType::DATE;
	if(z == static_cast<int>(FldType::VEC))  return FldType::VEC;
	return FldType::STR;
}
inline int FldTypeToInt(FldType z){
	return static_cast<int>(z);
}
inline String FldTypeToStr(FldType z){
	String rStr = "STR";
	switch(z){
		case FldType::STR:
			rStr = "STR";
			break;
		case FldType::INT:
			rStr = "INT";
			break;
		case FldType::FLOAT:
			rStr = "FLOAT";
			break;
		case FldType::DATE:
			rStr = "DATE";
			break;
		case FldType::VEC:
			rStr = "VEC";
			break;
	}
	return rStr;
}
inline FldType StrToFldType(const String& str){
	if(ToUpper(str) == "STR")	return FldType::STR;
	if(ToUpper(str) == "INT")	return FldType::INT;
	if(ToUpper(str) == "FLOAT")	return FldType::FLOAT;
	if(ToUpper(str) == "DATE")	return FldType::DATE;
	if(ToUpper(str) == "VEC")	return FldType::VEC;
	return FldType::STR;
}
inline bool IsFldType(const String& str){
	return
		(StrToFldType(ToUpper(str)) == FldType::STR)||
		(StrToFldType(ToUpper(str)) == FldType::INT)||
		(StrToFldType(ToUpper(str)) == FldType::FLOAT)||
		(StrToFldType(ToUpper(str)) == FldType::DATE)||
		(StrToFldType(ToUpper(str)) == FldType::VEC);
}
//! --------------------------------------------------------------------------------------------

enum class FldEdit {NONE, STR, INT, FLOAT, DATE, LIST};
inline FldEdit IntToFldEdit(int z){
	if(z == static_cast<int>(FldEdit::NONE))  return FldEdit::NONE;
	if(z == static_cast<int>(FldEdit::STR))   return FldEdit::STR;
	if(z == static_cast<int>(FldEdit::INT))   return FldEdit::INT;
	if(z == static_cast<int>(FldEdit::FLOAT)) return FldEdit::FLOAT;
	if(z == static_cast<int>(FldEdit::DATE))  return FldEdit::DATE;
	if(z == static_cast<int>(FldEdit::LIST))  return FldEdit::LIST;
	return FldEdit::STR;
}
inline int FldEditToInt(FldEdit z){
	return static_cast<int>(z);
}
inline String FldEditToStr(FldEdit z){
	String rStr = "STR";
	switch(z){
		case FldEdit::NONE:
			rStr = "NONE";
			break;
		case FldEdit::STR:
			rStr = "STR";
			break;
		case FldEdit::INT:
			rStr = "INT";
			break;
		case FldEdit::FLOAT:
			rStr = "FLOAT";
			break;
		case FldEdit::DATE:
			rStr = "DATE";
			break;
		case FldEdit::LIST:
			rStr = "LIST";
			break;
	}
	return rStr;
}
inline FldEdit StrToFldEdit(const String& str){
	if(ToUpper(str) == "NONE")	return FldEdit::NONE;
	if(ToUpper(str) == "STR")	return FldEdit::STR;
	if(ToUpper(str) == "INT")	return FldEdit::INT;
	if(ToUpper(str) == "FLOAT")	return FldEdit::FLOAT;
	if(ToUpper(str) == "DATE")	return FldEdit::DATE;
	if(ToUpper(str) == "LIST")	return FldEdit::LIST;
	return FldEdit::STR;
}
//! --------------------------------------------------------------------------------------------

struct HcmLstRec{
	void Clear(){lst.Clear();}
	String Name()const{return name;}
	const VectorMap<String, String>& Lst()const{return lst;}
	int LoadCSV(const String& file);
	void Fill(DropList& drList)const;
	void Fill(DropGrid & drGrid)const;
	HcmLstRec():name(""){Clear();}
	HcmLstRec(const String& name):name(name){Clear();}
	HcmLstRec(const String& name, const Vector<String>& vec);
	HcmLstRec(const HcmLstRec& rec):name(rec.name), lst(clone(rec.lst)){}
	HcmLstRec(const String& name, const String& file):name(name){LoadCSV(file);};
protected:
	String name;
	VectorMap<String, String> lst;
};
HcmLstRec::HcmLstRec(const String& name, const Vector<String>& vec):name(name){
	for(auto v: vec){
		lst.FindAdd(v, v);
	}
}
int HcmLstRec::LoadCSV(const String& file){
	lst.Clear();
	if(!FileExists(file)){
		LOG(Format("Error: File %s, not found", file));
		return 0;
	}
	FileIn f_in(file);
	while(!f_in.IsEof()){
		Vector<String> tmpVec;
        String tmpS = f_in.GetLine();
        if(!TrimBoth(tmpS).IsEmpty()){
			StringStream strm = StringStream() << tmpS;
			tmpVec = GetCsvLine(strm, C_RAZDEL, CHARSET_DEFAULT);
			if(tmpVec.GetCount() > 1)
				lst.Add(tmpVec[0], tmpVec[1]);
			else if(tmpVec.GetCount() > 0)
				lst.Add(tmpVec[0], tmpVec[0]);
        }
    }
    return lst.GetCount();
}
void HcmLstRec::Fill(DropList& drList)const{
	for(int i=0; i<lst.GetCount(); i++){
		// drList.Add(lst.GetKey(i), lst[i]);
		drList.Add(lst.GetKey(i));
	}
}
void HcmLstRec::Fill(DropGrid & drGrid)const{
	drGrid.Clear();
	drGrid.ClearButton();
	drGrid.AddColumn("Code");
	drGrid.AddColumn("Name");
	for(int i=0; i<lst.GetCount(); i++){
		drGrid.Add(lst.GetKey(i), lst[i]);
	}
}
//!---------------------------------------------------------------------------------------

struct HcmLstData{
	const HcmLstRec& GetLst(const String& nameLst)const{
		static HcmLstRec tmpLst(nameLst);
		int idx = lstMap.Find(nameLst);
		if(idx > -1){
			return lstMap[idx];
		}
		return tmpLst;
	}
	bool AddLst(const HcmLstRec& lst){
		return lstMap.FindAdd(lst.Name(), clone(lst));
	}
	bool AddLst(const String& name, const Vector<String>& vec){
		int idx = lstMap.Find(name);
		if(idx > -1){
			lstMap.Add(name, HcmLstRec(name, vec));
		}
		return false;
	}
	bool AddLst(const String& name, const String& file){
		int idx = lstMap.Find(name);
		if(idx > -1){
			lstMap.Add(name, HcmLstRec(name, file));
		}
		return false;
	}
	bool Find(const String& nameLst, const String& code){
		HcmLstRec lst = GetLst(nameLst);
		if(lst.Lst().Find(code) != -1)
			return true;
		else
			return false;
	}
	HcmLstData(){}
protected:
	ArrayMap<String, HcmLstRec> lstMap; // key: hcmCode
};
//!---------------------------------------------------------------------------------------


struct FldDescr :Moveable<FldDescr>{
	void Clear(){
		type = FldType::STR;
		mandat = FldMandat::OFF;
		mask = "";
		lst = "";
		length = 0;
	}
	String ToString()const{
		return Format("%s, mandat: %s, length: %d, mask: %s", FldTypeToStr(type), FldMandatToStr(mandat), length, mask);
	}
	FldType GetType()const{return type;};
	FldMandat GetMandat()const{return mandat;};
	FldEdit GetFldEdit()const{return fldEdit;};
	bool IsMandat()const{return mandat == FldMandat::ON;};
	int GetLength()const{return length;};
	String GetMask()const{return mask;};
	String GetLst()const{return lst;};
	void Set(const FldDescr& desc){
		type = (desc.type);
		mandat = (desc.mandat);
		fldEdit = (desc.fldEdit);
		length = (desc.length);
		mask = (desc.mask);
		lst = desc.lst;
	}
	void Jsonize(JsonIO& json);
	FldDescr(){Clear();}
	FldDescr(FldType type, FldMandat mandat, FldEdit fldEdit, const String& mask, const String& lst, int length):
		type(type), mandat(mandat), fldEdit(fldEdit), mask(mask), lst(lst), length(length){}
	FldDescr(FldType type):
		type(type), mandat(FldMandat::OFF), fldEdit(FldEdit::STR), mask(""), lst(""), length(0){}
	FldDescr(FldType type, FldMandat mandat):
		type(type), mandat(mandat), fldEdit(FldEdit::STR), mask(""), lst(""), length(0){}
	FldDescr(FldType type, FldMandat mandat, int length):
		type(type), mandat(mandat), fldEdit(FldEdit::STR), mask(""), lst(""), length(length){}
	FldDescr(FldType type, FldMandat mandat, const String& mask):
		type(type), mandat(mandat), fldEdit(FldEdit::STR), mask(mask), lst(""), length(0){}
	FldDescr(FldType type, FldMandat mandat, FldEdit fldEdit):
		type(type), mandat(mandat), fldEdit(fldEdit), mask(""), lst(""), length(0){}
	FldDescr(FldType type, FldMandat mandat, FldEdit fldEdit, const String& lst):
		type(type), mandat(mandat), fldEdit(fldEdit), mask(""), lst(lst), length(0){}
	FldDescr(const FldDescr& desc):
		type(desc.type), mandat(desc.mandat), fldEdit(desc.fldEdit), mask(desc.mask), lst(desc.lst), length(desc.length){}
protected:
	FldType type;
	FldMandat mandat;
	FldEdit fldEdit;
	String mask;
	String lst; // name of list
	int length;
	double max;
	double min;
};
void FldDescr::Jsonize(JsonIO& json){
	if(json.IsLoading()){
		String sType;
		String sMandat;
		String sIN;
		json
			("type", sType)
			("mandat", sMandat)
			("fldEdit", sIN)
			("mask", mask)
			("lst", lst)
			("length", length)
		;
		type = StrToFldType(sType);
		mandat = StrToFldMandat(sMandat);
		fldEdit = StrToFldEdit(sIN);
	}
	else{
		String sType = FldTypeToStr(type);
		String sMandat = FldMandatToStr(mandat);
		String sIN = FldEditToStr(fldEdit);
		json
			("type", sType)
			("mandat", sMandat)
			("fldEdit", sIN)
			("mask", mask)
			("lst", lst)
			("length", length)
		;
	}
}
//!---------------------------------------------------------------------------------------

struct NameVal :Moveable<NameVal>{
	String code;
	String name;
	String val;
	FldDescr fDescr;
	String ToString()const{return Format("[%s] %s: %s", code, name, val);};
	void Set(const NameVal& nv){code = nv.code; name = nv.name; val = nv.val; fDescr.Set(nv.fDescr);}
	void Set(const String& _code, const String& _name, const String& _val, const FldDescr& _fDescr)
		{code = _code; name = _name; val = _val; fDescr = clone(_fDescr);}
	NameVal():code(""), name(""), val(""){};

	NameVal(const String& code, const String& name, const String& val):
			code(code), name(name), val(val){fDescr.Clear();};
	NameVal(const String& code, const String& name, const double val):
			code(code), name(name), val(DblStr(val)){fDescr.Clear();};
	NameVal(const String& code, const String& name, const int val):
			code(code), name(name), val(IntStr(val)){fDescr.Clear();};

	NameVal(const String& code, const String& name, const String& val, const FldDescr& fDescr):
			code(code), name(name), val(val), fDescr(clone(fDescr)){};
	NameVal(const NameVal& nv): code(nv.code), name(nv.name), val(nv.val), fDescr(clone(nv.fDescr)){};
};

struct TwoCodeVal :Moveable<TwoCodeVal>{
	String code_1;
	String val_1;
	String code_2;
	String val_2;
	String ToString()const{return Format("[%s] %s; [%s] %s", code_1, val_1, code_2, val_2);};
	void Set(const TwoCodeVal& tcv){code_1 = tcv.code_1; val_1 = tcv.val_1; code_2 = tcv.code_2; val_2 = tcv.val_2;}
	TwoCodeVal(){code_1= ""; val_1= ""; code_2= ""; val_2 = "";};
	TwoCodeVal(const String& code_1, const String& val_1, const String& code_2, const String& val_2):
			code_1(code_1), val_1(val_1), code_2(code_2), val_2(val_2){};
	TwoCodeVal(const String& code_1, const double val_1, const String& code_2, const double val_2):
			code_1(code_1), code_2(code_2), val_1(DblStr(val_1)), val_2(DblStr(val_2)){};
	TwoCodeVal(const String& code_1, const double val_1, const String& code_2, const int val_2):
			code_1(code_1), code_2(code_2), val_1(IntStr(val_1)), val_2(IntStr(val_2)){};
};
struct TwoStrVal: Moveable<TwoStrVal>{
	String val_1;
	String val_2;
	bool IsEmpty()const{return TrimBoth(val_1).IsEmpty() || TrimBoth(val_2).IsEmpty();}
	void Set(const TwoStrVal& tsv){val_1 = tsv.val_1; val_2 = tsv.val_2;}
	String ToString()const{return Format("%s; %s", val_1, val_2);};
	TwoStrVal(){val_1= ""; val_2 = "";};
	TwoStrVal(const String& val_1, const String& val_2): val_1(val_1), val_2(val_2){};
};
struct TwoDblVal: Moveable<TwoDblVal>{
	double val_1;
	double val_2;
	void Clear(){val_1= 0.0; val_2 = 0.0;}
	void Set(const TwoDblVal& tsv){val_1 = tsv.val_1; val_2 = tsv.val_2;}
	String ToString()const{return Format("%f; %f", val_1, val_2);};
	TwoDblVal(){Clear();};
	TwoDblVal(const double val_1, const double val_2): val_1(val_1), val_2(val_2){};
};
//!---------------------------------------------------------------------------------------

struct CodeVec :Moveable<CodeVec>{
	String code;
	Vector<String> vec;
	String ToString()const{return Format("%s: %s", code, vec.ToString());};
	void Jsonize(JsonIO& json){json("code", code)("val", vec);};
	void Set(const CodeVec& nv){code = nv.code; vec = clone(nv.vec);}
	void Set(const String& _code, const Vector<String>& _vec)
		{code = _code; vec = clone(_vec);}
	CodeVec():code(""){vec.Clear();};
	CodeVec(const String& code, const Vector<String>& vec):	code(code), vec(clone(vec)){};
	CodeVec(const CodeVec& cv): code(cv.code), vec(clone(cv.vec)){};
};
//!---------------------------------------------------------------------------------------

struct HcmField :Moveable<HcmField>{
	String ToString()const{
		return Format("[%s]:\t\t%s:\t%s", code, name, vval.ToString());
	}
	const FldDescr& GetDesct()const{return fDescr;}
	String GetCode()const{return code;}
	String GetName()const{return name;}
	String GetErMsg()const{return erMsg;}
	void SetErMsg(const String& str){erMsg << str;}
	String GetStr()const{
		if(vval.GetCount() == 1)
			return vval[0];
		else if(vval.GetCount() > 1)
			return vval.ToString();
		return "";
	}
	NameVal ToNV()const{
		return NameVal(code, name, GetStr(), fDescr);
	}
	String ToCSV()const{
		String str = "";
		if(vval.GetCount() == 1){
			return vval[0];
		}
		for(int i=0; i< vval.GetCount(); i++){
			if(i==0)
				str << vval[i];
			else
				str << S_RAZDEL << vval[i];
		}
		return str;
	}
	Vector<String> ToVec()const{
		Vector<String> vec;
		for(int i=0; i< vval.GetCount(); i++)
			vec.Add(vval[i]);
		return vec;
	}
	const Vector<String>& GetVec()const{return vval;}
	void EditVal(const String& s){vval.Clear(); vval << s;}
	void EditVal(const Vector<String>& v){vval = clone(v);}
	void SetVal(const String& s){vval << s;}
	void SetVal(const Vector<String>& v){vval = clone(v);}
	void Clear(){vval.Clear(); erMsg.Clear();}
	bool IsEmpty()const{
		if(vval.GetCount() == 1)
			return vval[0].IsEmpty();
		else if(vval.GetCount() > 1){
			for(auto v: vval)
				if(!v.IsEmpty()){
					return false;
				}
		}
		return true;
	}
	void Jsonize(JsonIO& json);
	String ToJsonStr();
	bool FromJsonStr(const String &s);
	bool Check();
	~HcmField(){vval.Clear();}
	HcmField(): code(""), name(""){fDescr.Clear(); Clear();}
	HcmField(const String& code, const String& name, const FldDescr& fDescr):
		 code(code), name(name), fDescr(clone(fDescr)){vval.Clear();}
	HcmField(const HcmField& fld):
		 code(fld.code), name(fld.name), fDescr(clone(fld.fDescr)), vval(clone(fld.vval)){}
protected:
	FldDescr fDescr;
	String code;
	String name;
	String erMsg;
	Vector<String> vval;
	bool CheckMandat();
	bool CheckOnType();
	bool CheckInLst();
	bool CheckOnLenghts();
	bool CheckOnMask();
};
void HcmField::Jsonize(JsonIO& json){
	json
		("code", code)
		("val", vval)
	;
}
String HcmField::ToJsonStr(){
	return StoreAsJson(*this, true);
}
bool HcmField::FromJsonStr(const String &s){
	return LoadFromJson(*this, s);
}

bool HcmField::Check(){
	if(!CheckMandat()) return false;
	if(!CheckOnType()) return false;
	if(!CheckInLst()) return false;
	if(!CheckOnLenghts()) return false;
	if(!CheckOnMask()) return false;
	return true;
}

bool HcmField::CheckMandat(){
	String str = TrimBoth(GetStr());
	//if(GetDesct().IsMandat() && str.IsEmpty()){
	FldMandat fmd = GetDesct().GetMandat();
	if(fmd == FldMandat::ON && str.IsEmpty()){
		erMsg = Format(" [%s] %s %s", GetCode(), GetName(), " is missing");
		return false;
	}
	return true;
}
// checking the value for type compliance
bool HcmField::CheckOnType(){
	Value fValue = Null;
	switch(GetDesct().GetType()){
		case FldType::STR:
			fValue = GetStr();
			if(!IsString(fValue)){
				erMsg = Format(" [%s] %s %s", GetCode(), GetName(), " is not a alphanumeric");
				return false;
			}
			return true;
		case FldType::INT:
			fValue = StrInt(GetStr());
			if(!IsNumber(fValue)){
				erMsg = Format(" [%s] %s %s", GetCode(), GetName(), " is not a integer number");
				return false;
			}
			return true;
		case FldType::FLOAT:
			fValue = StrDbl(GetStr());
			if(!IsNumber(fValue)){
				erMsg = Format(" [%s] %s %s", GetCode(), GetName(), " is not a float number");
				return false;
			}
			return true;
		case FldType::DATE:{
				String txt = GetStr();
				if(!IsNull(txt)){
					Date d;
					if(StrToDate(d, txt)) fValue = d;
					if(!IsDateTime(fValue)){
						erMsg = Format(" [%s] %s %s", GetCode(), GetName(), " is not a date");
						return false;
					}
				}
			}
			return true;
		case FldType::VEC:
			return true;
	}
	erMsg = Format("%s %s", GetName(), " is unknown");
	return false;
}
// checking the value for matching the list
bool HcmField::CheckInLst(){
	if(fDescr.GetLst().IsEmpty()) return true;
	HcmLstRec lst = Single<HcmLstData>().GetLst(fDescr.GetLst());
	bool isFind = false;
	for(int i=0; i<lst.Lst().GetCount(); i++){
		if(ToUpper(lst.Lst().GetKey(i)) == ToUpper(GetStr())){
			isFind = true;
			break;
		}
	}
	if(!isFind){
		erMsg = Format(" [%s] %s %s", GetCode(), GetName(), " is unknown");
		return false;
	}
	return true;
}
bool HcmField::CheckOnLenghts(){
	if(fDescr.GetLength()==0) return true;
	if(!fDescr.IsMandat()) return true;
	String str = TrimBoth(GetStr());
	if(str.GetLength() != fDescr.GetLength()){
		erMsg = Format(" [%s] %s %s", GetCode(), GetName(), " has incorrect number of characters");
		return false;
	}
	return true;
}
bool HcmField::CheckOnMask(){
	if(fDescr.GetMask().IsEmpty()) return true;
	String p = "(\\b" + fDescr.GetMask() + "\\b)";
	RegExp r0(p);
	if(!r0.Match(TrimBoth(GetStr()))){
		erMsg = Format(" [%s] %s %s", GetCode(), GetName(), " is incorrect");
		return false;
	}
	return true;
}
//! --------------------------------------------------------------------------------------------
//! --------------------------------------------------------------------------------------------
//! --------------------------------------------------------------------------------------------

struct HcmData: Moveable<HcmData>{
	const VectorMap<int, HcmField>& GetMap()const{return map;}
	const HcmEntity GetEntity()const{return hcmEntity;};
	const HcmField GetField(const String& code)const;
	const HcmField GetField(const int id)const;
	bool EditVal(const String& code, const String& val);
	bool EditVal(const String& code, const Vector<String>& vval);
	bool EditVal(const int id, const String& val);
	bool SetVal(const int id, const String& val);
	bool SetVal(const int id, const Vector<String>& v);
	void ClearVal(const int id);
	
	String ToString()const;
	String ToCSV()const{return "";};
	void FromCSV(const String& hcmString){};
	//virtual void FromCSV(const String& hcmString) = 0;
	const Vector<NameVal> ToNV()const;
	void Clear();
	void Set(const HcmData& hcmData){map = clone(hcmData.map);};
	HcmData(): hcmEntity(HcmEntity::None){};
	HcmData(const HcmData& hcmData){map = clone(hcmData.map); hcmEntity = hcmData.GetEntity();};
	~HcmData(){map.Clear();};
protected:
	HcmEntity hcmEntity;
	VectorMap<int, HcmField> map; // key HcmField::code
};
void HcmData::Clear(){
	for(int i=0; i<map.GetCount(); i++)
		map[i].Clear();
}

const Vector<NameVal> HcmData::ToNV()const{
	Vector<NameVal> nameValList;
	for(int i=0; i < map.GetCount(); i++)
		nameValList.Add(map[i].ToNV());
	return nameValList;
}
const HcmField HcmData::GetField(const int id)const{
	int idx = map.Find(id);
	if(idx != -1)
		return clone(map[idx]);
	else
		return HcmField();
}
const HcmField HcmData::GetField(const String& code)const{
	for(int i=0; i < map.GetCount(); i++){
		if(map[i].GetCode() == code)
			return clone(map[i]);
	}
	return HcmField();
}
bool HcmData::EditVal(const int id, const String& val){
	int idx = map.Find(id);
	if(idx != -1){
		map[idx].EditVal(val);
		return true;
	}
	return false;
}
bool HcmData::EditVal(const String& code, const String& val){
	for(int i=0; i < map.GetCount(); i++){
		if(map[i].GetCode() == code){
			map[i].EditVal(val);
			return true;
		}
	}
	return false;
}
bool HcmData::EditVal(const String& code, const Vector<String>& vval){
	for(int i=0; i < map.GetCount(); i++){
		if(map[i].GetCode() == code){
			map[i].SetVal(vval);
			return true;
		}
	}
	return false;
}
bool HcmData::SetVal(const int id, const String& val){
	int idx = map.Find(id);
	if(idx != -1){
		map[idx].SetVal(val);
		return true;
	}
	return false;
}
bool HcmData::SetVal(const int id, const Vector<String>& v){
	int idx = map.Find(id);
	if(idx != -1){
		map[idx].SetVal(v);
		return true;
	}
	return false;
}
void HcmData::ClearVal(const int id){
	int idx = map.Find(id);
	if(idx != -1){
		map[idx].Clear();
	}
}

String HcmData::ToString()const{
	String str = "HcmData:\n";
	for(int i=0; i < map.GetCount(); i++){
		str << Format("%d:\t%s\n", map.GetKey(i), map[i].ToString());
	}
	return str;
}
//! --------------------------------------------------------------------------------------------




struct HcmHeader: HcmData, Moveable<HcmHeader>{
	void FromCSV(const String& hcmString);
	String ToCSV()const;
	void Clear();
	void Jsonize(JsonIO& json);
	void Set(const Vector<CodeVec>& cvLst);
	void Set(const HcmHeader& header){map = clone(header.map);  hcmEntity = header.GetEntity();};
	void Check(String& vlog);
	HcmHeader();
	HcmHeader(const HcmHeader& header){ map = clone(header.map);  hcmEntity = header.GetEntity(); };
	~HcmHeader(){map.Clear();};
};
HcmHeader::HcmHeader(){
	hcmEntity = HcmEntity::HEADER;
	map.Add(0, HcmField("1",  t_("File number"), FldDescr(FldType::STR, FldMandat::ON)));
	map.Add(1, HcmField("2",  t_("File contents"), FldDescr(FldType::STR, FldMandat::ON)));
	map.Add(2, HcmField("3",  t_("File contents code"), FldDescr(FldType::STR, FldMandat::ON)));
	map.Add(3, HcmField("4",  t_("Country"), FldDescr(FldType::STR, FldMandat::ON, FldEdit::LIST, "4B")));
	map.Add(4, HcmField("5",  t_("Name of the responsible person"), FldDescr(FldType::STR, FldMandat::OFF)));
	map.Add(5, HcmField("6",  t_("Phone"), FldDescr(FldType::STR, FldMandat::OFF)));
	map.Add(6, HcmField("7",  t_("Telefax"), FldDescr(FldType::STR, FldMandat::OFF)));
	map.Add(7, HcmField("8",  t_("E-mail"), FldDescr(FldType::STR, FldMandat::OFF)));
	map.Add(8, HcmField("9",  t_("Number of records"), FldDescr(FldType::STR, FldMandat::OFF)));
	// map.Add(9, HcmField("10", t_("Writing date"), FldDescr(FldType::STR, FldMandat::ON, "(0[1-9]|[12][0-9]|3[01])[.]*(0[1-9]|1[012])[.]*(19|20)\\d\\d")));
	map.Add(9, HcmField("10", t_("Writing date"), FldDescr(FldType::DATE, FldMandat::ON, FldEdit::DATE)));
}
void HcmHeader::Clear(){
	for(int i=0; i<map.GetCount(); i++)
		map[i].Clear();
}
void HcmHeader::FromCSV(const String& hcmString){
	Clear();
	Vector<String> vhcm;
	StringStream strm = StringStream() << hcmString;
	auto v = GetCsvLine(strm, C_RAZDEL, CHARSET_DEFAULT);
	for(auto e : v){
		vhcm.Add(e);
	}
	for(int i=0; i < vhcm.GetCount(); i++){
		EditVal(i, TrimBoth(vhcm[i]));
	}
}
String HcmHeader::ToCSV()const{
	String str = "";
	for(int i=0; i < map.GetCount(); i++){
		if(i==0){
			str << map[i].ToCSV();
			continue;
		}
		str  << S_RAZDEL << map[i].ToCSV();
	}
	return str << S_RAZDEL;
}
void HcmHeader::Jsonize(JsonIO& json){
	Vector<CodeVec> cvLst;
	if(json.IsStoring()){
		for(int i=0; i<map.GetCount(); i++){
			cvLst.Add(CodeVec(map[i].GetCode(), map[i].GetVec()));
		}
	}
	json
		("Data", cvLst)
	;
	if(json.IsLoading()){
		Set(cvLst);
	}
}
void HcmHeader::Set(const Vector<CodeVec>& cvLst){
	for(auto cv: cvLst){
		for(auto m: map){
			if(m.GetCode() == cv.code){
				m.EditVal(cv.vec);
				break;
			}
		}
	}
}
void HcmHeader::Check(String& vlog){
	for(int i=0; i< map.GetCount(); i++){
		if(!map[i].Check())
			vlog << (map[i].GetErMsg()) << "\n";
	}
}
//! --------------------------------------------------------------------------------------------
//! --------------------------------------------------------------------------------------------

struct HcmIN: HcmData, Moveable<HcmIN>{
	typedef HcmIN CLASSNAME;
	void FromCSV(const String& hcmString);
	void FromVec(const Vector<String>& vhcm);
	String ToCSV()const;
	Vector<String> ToVec(Vector<String>& columns)const;

	void Jsonize(JsonIO& json);
	void Set(const Vector<CodeVec>& cvLst);
	void Check(String& vlog);
	HcmIN();
	HcmIN(const HcmIN& hcmIN){map = clone(hcmIN.map); hcmEntity = hcmIN.GetEntity();};
	~HcmIN(){map.Clear();};
protected:
	// bool Check_4C(HcmField& fld);
	bool CheckIfMandat(HcmField& fld);
	bool Check_7A(HcmField& fld);
	bool Check_7G(HcmField& fld);
	bool Check_7G3(HcmField& fld);
	bool Check_8B(HcmField& fld);
	bool Check_9A(HcmField& fld);
	bool Check_13Y(HcmField& fld);
	bool Check_13X(HcmField& fld);
	bool Check_9X(HcmField& fld);
};

HcmIN::HcmIN(){
	hcmEntity = HcmEntity::None;
	map.Add(0,  HcmField("0A",	 t_("Type of entry"), FldDescr(FldType::STR, FldMandat::ON, FldEdit::LIST, "")));
	map.Add(1,  HcmField("1A",	 t_("Frequency"), FldDescr(FldType::FLOAT, FldMandat::ON, FldEdit::FLOAT)));
	map.Add(2,  HcmField("1A1",	 t_("Frequency unit"), FldDescr(FldType::STR, FldMandat::ON, FldEdit::LIST, "")));
	map.Add(3,  HcmField("1Z",	 t_("Frequency category"), FldDescr(FldType::STR, FldMandat::ON, FldEdit::LIST)));
	map.Add(4,  HcmField("6A",	 t_("Class of station"), FldDescr(FldType::STR, FldMandat::OFF, FldEdit::LIST)));
	map.Add(5,  HcmField("6B",	 t_("Nature of service"), FldDescr(FldType::STR, FldMandat::OFF, FldEdit::LIST)));
	map.Add(6,  HcmField("6Z",	 t_("Category of use"), FldDescr(FldType::STR, FldMandat::OFF, FldEdit::LIST)));
	map.Add(7,  HcmField("2C",	 t_("Date of bringing into use"), FldDescr(FldType::DATE, FldMandat::OFF, FldEdit::DATE)));
	map.Add(8,  HcmField("4A",	 t_("Name of station"), FldDescr(FldType::STR, FldMandat::OFF)));
	map.Add(9,  HcmField("4B",	 t_("Country"), FldDescr(FldType::STR, FldMandat::ON, FldEdit::LIST, "")));
	map.Add(10, HcmField("4C",	 t_("Geographical co-ordinates"), FldDescr(FldType::STR, FldMandat::ON, "[0-9]{3}[EeWw][0-9]{6}[NnSs][0-9]{4}")));
	map.Add(11, HcmField("4Z",	 t_("Height of the station site above sea level"), FldDescr(FldType::INT, FldMandat::ON, FldEdit::INT)));
	map.Add(12, HcmField("7A",	 t_("Designation of emission"), FldDescr(FldType::STR, FldMandat::OFF)));
	map.Add(13, HcmField("7H",	 t_("Equipment manufacturer name"), FldDescr(FldType::STR, FldMandat::OFF)));
	map.Add(14, HcmField("7I",	 t_("Equipment type"), FldDescr(FldType::STR, FldMandat::OFF)));
	map.Add(15, HcmField("7K",	 t_("Max. capacity of the link"), FldDescr(FldType::STR, FldMandat::OFF, FldEdit::LIST, "")));
	map.Add(16, HcmField("7Gf1", t_("Tx/Rx mask. Point 1: frequency"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(17, HcmField("7Ga1", t_("Tx/Rx mask. Point 1: attenuation"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(18, HcmField("7Gf2", t_("Tx/Rx mask. Point 2: frequency"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(19, HcmField("7Ga2", t_("Tx/Rx mask. Point 2: attenuation"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(20, HcmField("7Gf3", t_("Tx/Rx mask. Point 3: frequency"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(21, HcmField("7Ga3", t_("Tx/Rx mask. Point 3: attenuation"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(22, HcmField("7Gf4", t_("Tx/Rx mask. Point 4: frequency"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(23, HcmField("7Ga4", t_("Tx/Rx mask. Point 4: attenuation"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(24, HcmField("7Gf5", t_("Tx/Rx mask. Point 5: frequency"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(25, HcmField("7Ga5", t_("Tx/Rx mask. Point 5: attenuation"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(26, HcmField("7Gf6", t_("Tx/Rx mask. Point 6: frequency"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(27, HcmField("7Ga6", t_("Tx/Rx mask. Point 6: attenuation"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(28, HcmField("7G1",	 t_("Equipment class"), FldDescr(FldType::STR, FldMandat::OFF)));
	map.Add(29, HcmField("7G2",	 t_("NFD 2"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(30, HcmField("7G3",	 t_("Channel spacing"), FldDescr(FldType::FLOAT, FldMandat::ON, FldEdit::FLOAT)));
	map.Add(31, HcmField("8B",	 t_("Maximum permitted transmitter power"), FldDescr(FldType::FLOAT, FldMandat::TX, FldEdit::FLOAT)));
	map.Add(32, HcmField("8B3",	 t_("ATPC"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(33, HcmField("9A",	 t_("Azimuth"), FldDescr(FldType::FLOAT, FldMandat::ON, FldEdit::FLOAT)));
	map.Add(34, HcmField("9B",	 t_("Elevation"), FldDescr(FldType::FLOAT, FldMandat::ON, FldEdit::FLOAT)));
	map.Add(35, HcmField("9D",	 t_("Polarization"), FldDescr(FldType::STR, FldMandat::ON, FldEdit::LIST, "")));
	map.Add(36, HcmField("9H",	 t_("Receiver noise power level"), FldDescr(FldType::FLOAT, FldMandat::RX, FldEdit::FLOAT)));
	map.Add(37, HcmField("9L",	 t_("Branches and line losses"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(38, HcmField("9Y",	 t_("Height of antenna above ground"), FldDescr(FldType::INT, FldMandat::ON, FldEdit::INT)));
	map.Add(39, HcmField("13Z",	 t_("Remarks"), FldDescr(FldType::STR, FldMandat::OFF)));
	map.Add(40, HcmField("13Y",	 t_("Status of co-ordination"), FldDescr(FldType::STR, FldMandat::OFF, FldEdit::LIST)));
	map.Add(41, HcmField("2W",	 t_("Date of co-ordination request"), FldDescr(FldType::DATE, FldMandat::OFF, FldEdit::DATE)));
	map.Add(42, HcmField("2Z",	 t_("Final date of achieving co-ordination"), FldDescr(FldType::DATE, FldMandat::OFF, FldEdit::DATE)));
	map.Add(43, HcmField("13X",	 t_("Co-ordination reference"), FldDescr(FldType::STR, FldMandat::OFF, 20)));
	map.Add(44, HcmField("9XM",	 t_("Antenna manufacturer name"), FldDescr(FldType::STR, FldMandat::IF)));
	map.Add(45, HcmField("9XT",	 t_("Antenna type"), FldDescr(FldType::STR, FldMandat::IF)));
	map.Add(46, HcmField("9XFL", t_("Lower antenna frequency"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(47, HcmField("9XFU", t_("Upper antenna frequency"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(48, HcmField("9X1",	 t_("Antenna gain"), FldDescr(FldType::FLOAT, FldMandat::ON, FldEdit::FLOAT)));
	map.Add(49, HcmField("9Xptcp",	 t_("Copolar radiation pattern"), FldDescr(FldType::STR, FldMandat::OFF, FldEdit::LIST, "")));
	map.Add(50, HcmField("9Xcpsize", t_("Number of CP antenna mask data"), FldDescr(FldType::INT, FldMandat::OFF, FldEdit::INT)));
	map.Add(51, HcmField("9XCPan",	 t_("Antenna CP angle"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(52, HcmField("9XCPat",	 t_("Antenna CP attenuation"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(53, HcmField("9Xptxp",	 t_("Cross-polar radiation pattern"), FldDescr(FldType::STR, FldMandat::OFF, FldEdit::LIST, "")));
	map.Add(54, HcmField("9Xxpsize", t_("Number of XP antenna mask data"), FldDescr(FldType::INT, FldMandat::OFF, FldEdit::INT)));
	map.Add(55, HcmField("9XXPan",	 t_("Antenna XP angle"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
	map.Add(56, HcmField("9XXPat",	 t_("Antenna XP attenuation"), FldDescr(FldType::FLOAT, FldMandat::OFF, FldEdit::FLOAT)));
}
// DATA: "(0[1-9]|[12][0-9]|3[01])[.]*(0[1-9]|1[012])[.]*(19|20)\\d\\d" = 23032025 = 23.03.2025

void HcmIN::FromVec(const Vector<String>& vhcm){
	for(int i=0; i < vhcm.GetCount(); i++){
		if(i <= 48)
			SetVal(i, TrimBoth(vhcm[i]));
		else
			break;
	}
	hcmEntity = StrToHcmEntity(map[0].GetStr());
	//------------------------------------------

	if(vhcm.GetCount() > 49){
		//--- ANTENNA COPOLAR RADIATION PATTERN
		SetVal(49, vhcm[49]);
	}
	int cpMaskCount = 0;
	if(vhcm.GetCount() > 50){
		//--- Number of CP antenna mask data
		Value tmp = StrInt(TrimBoth(vhcm[50]));
		if(!tmp.IsNull()){
			cpMaskCount = tmp;
			SetVal(50, tmp.ToString());
		}
	}
	Vector<String> cpAngle;
	Vector<String> cpAttenuation;
	int cpMaskIdxMax = 51+cpMaskCount*2;
	if(cpMaskCount > 0 && vhcm.GetCount() > cpMaskIdxMax){
		for(int i = 51; i < cpMaskIdxMax; i++){
			if(i%2 != 0)
				cpAngle << vhcm[i];
			else
				cpAttenuation << vhcm[i];
		}
	}
	if(!cpAngle.IsEmpty()) SetVal(51, cpAngle);
	if(!cpAttenuation.IsEmpty()) SetVal(52, cpAttenuation);
	//------------------------------------------

	int xpStartIdx = cpMaskIdxMax;
	if(vhcm.GetCount() > xpStartIdx){
		//--- ANTENNA  Cross-polar radiation pattern
		SetVal(53, vhcm[xpStartIdx]);
	}
	int xpMaskCount = 0;
	if(vhcm.GetCount() > xpStartIdx+1){
		//--- Number of CP antenna mask data
		Value tmp = StrInt(TrimBoth(vhcm[xpStartIdx+1]));
		if(!tmp.IsNull()){
			xpMaskCount = tmp;
			SetVal(54, tmp.ToString());
		}
	}
	Vector<String> xpAngle;
	Vector<String> xpAttenuation;
	int xpMaskIdxMax = (xpStartIdx+2)+xpMaskCount*2;
	if(xpMaskCount > 0 && vhcm.GetCount() >= xpMaskIdxMax){
		for(int i = (xpStartIdx+2); i < xpMaskIdxMax; i++){
			if(i%2 != 0)
				xpAngle << vhcm[i];
			else
				xpAttenuation << vhcm[i];
		}
	}
	if(!xpAngle.IsEmpty()) SetVal(55, xpAngle);
	if(!xpAttenuation.IsEmpty()) SetVal(56, xpAttenuation);
	
}

void HcmIN::FromCSV(const String& hcmString){
	Clear();
	Vector<String> vhcm;
	StringStream strm = StringStream() << hcmString;
	auto v = GetCsvLine(strm, C_RAZDEL, CHARSET_DEFAULT);
	for(auto e : v){
		vhcm.Add(e);
	}
	for(int i=0; i < vhcm.GetCount(); i++){
		if(i <= 48)
			SetVal(i, TrimBoth(vhcm[i]));
		else
			break;
	}
	hcmEntity = StrToHcmEntity(map[0].GetStr());
	//------------------------------------------

	if(vhcm.GetCount() > 49){
		//--- ANTENNA COPOLAR RADIATION PATTERN
		SetVal(49, vhcm[49]);
	}
	int cpMaskCount = 0;
	if(vhcm.GetCount() > 50){
		//--- Number of CP antenna mask data
		Value tmp = StrInt(TrimBoth(vhcm[50]));
		if(!tmp.IsNull()){
			cpMaskCount = tmp;
			SetVal(50, tmp.ToString());
		}
	}
	Vector<String> cpAngle;
	Vector<String> cpAttenuation;
	int cpMaskIdxMax = 51+cpMaskCount*2;
	if(cpMaskCount > 0 && vhcm.GetCount() > cpMaskIdxMax){
		for(int i = 51; i < cpMaskIdxMax; i++){
			if(i%2 != 0)
				cpAngle << vhcm[i];
			else
				cpAttenuation << vhcm[i];
		}
	}
	if(!cpAngle.IsEmpty()) SetVal(51, cpAngle);
	if(!cpAttenuation.IsEmpty()) SetVal(52, cpAttenuation);
	//------------------------------------------

	int xpStartIdx = cpMaskIdxMax;
	if(vhcm.GetCount() > xpStartIdx){
		//--- ANTENNA  Cross-polar radiation pattern
		SetVal(53, vhcm[xpStartIdx]);
	}
	int xpMaskCount = 0;
	if(vhcm.GetCount() > xpStartIdx+1){
		//--- Number of CP antenna mask data
		Value tmp = StrInt(TrimBoth(vhcm[xpStartIdx+1]));
		if(!tmp.IsNull()){
			xpMaskCount = tmp;
			SetVal(54, tmp.ToString());
		}
	}
	Vector<String> xpAngle;
	Vector<String> xpAttenuation;
	int xpMaskIdxMax = (xpStartIdx+2)+xpMaskCount*2;
	if(xpMaskCount > 0 && vhcm.GetCount() >= xpMaskIdxMax){
		for(int i = (xpStartIdx+2); i < xpMaskIdxMax; i++){
			if(i%2 != 0)
				xpAngle << vhcm[i];
			else
				xpAttenuation << vhcm[i];
		}
	}
	if(!xpAngle.IsEmpty()) SetVal(55, xpAngle);
	if(!xpAttenuation.IsEmpty()) SetVal(56, xpAttenuation);
}

String HcmIN::ToCSV()const{
	String str = "";
	for(int i=0; i < map.GetCount(); i++){
		if(i==0){
			str << map[i].ToCSV();
			continue;
		}
		if(i<49)
			str  << S_RAZDEL << map[i].ToCSV();
		if(i==49 || i==50){
			str  << S_RAZDEL << map[i].ToCSV();
		}
		if(i==51 && map.GetCount() >=52){
			Vector<String> cpAngles = clone(map[i].GetVec());
			Vector<String> cpAttens = clone(map[i+1].GetVec());
			int idx=0;
			String cpStr = "";
			while(idx<cpAngles.GetCount() && idx<cpAttens.GetCount()){
				cpStr << S_RAZDEL << cpAngles[idx] << S_RAZDEL << cpAttens[idx];
				idx++;
			}
			str << cpStr;
		}
		if(i==53 || i==54){
			str  << S_RAZDEL << map[i].ToCSV();
		}
		if(i==55 && map.GetCount() >=56){
			Vector<String> xpAngles = clone(map[i].GetVec());
			Vector<String> xpAttens = clone(map[i+1].GetVec());
			int idx=0;
			String xpStr = "";
			while(idx<xpAngles.GetCount() && idx<xpAttens.GetCount()){
				xpStr << S_RAZDEL << xpAngles[idx] << S_RAZDEL << xpAttens[idx];
				idx++;
			}
			str << xpStr;
		}
	}
	return str << S_RAZDEL;
}

Vector<String> HcmIN::ToVec(Vector<String>& columns)const{
	Vector<String> vec;
	bool isFillCols = false;
	if(columns.IsEmpty()) isFillCols = true;
	for(int i=0; i < map.GetCount(); i++){
		if(i<=50){
			for(auto v: map[i].ToVec()){
				vec.Add(v);
				if(isFillCols) columns.Add(map[i].GetCode());
			}
		}
		if(i==51 && map.GetCount() >=52){
			Vector<String> cpAngles = clone(map[i].GetVec());
			Vector<String> cpAttens = clone(map[i+1].GetVec());
			int idx=0;
			while(idx<cpAngles.GetCount() && idx<cpAttens.GetCount()){
				if(isFillCols) columns.Add(map[i].GetCode()+"(" + IntStr(idx) + ")");
				vec.Add(cpAngles[idx]);
				if(isFillCols) columns.Add(map[i+1].GetCode()+"(" + IntStr(idx) + ")");
				vec.Add(cpAttens[idx]);
				idx++;
			}
		}
		if(i==53 || i==54){
			for(auto v: map[i].ToVec()){
				vec.Add(v);
				if(isFillCols) columns.Add(map[i].GetCode());
			}
		}
		if(i==55 && map.GetCount() >=56){
			Vector<String> xpAngles = clone(map[i].GetVec());
			Vector<String> xpAttens = clone(map[i+1].GetVec());
			int idx=0;
			String xpStr = "";
			while(idx<xpAngles.GetCount() && idx<xpAttens.GetCount()){
				if(isFillCols) columns.Add(map[i].GetCode()+"(" + IntStr(idx) + ")");
				vec.Add(xpAngles[idx]);
				if(isFillCols) columns.Add(map[i+1].GetCode()+"(" + IntStr(idx) + ")");
				vec.Add(xpAttens[idx]);
				idx++;
			}
		}
	}
	return vec;
}

void HcmIN::Jsonize(JsonIO& json){
	Vector<CodeVec> cvLst;
	if(json.IsStoring()){
		for(int i=0; i<map.GetCount(); i++){
			cvLst.Add(CodeVec(map[i].GetCode(), map[i].GetVec()));
		}
		json
			("Data", cvLst)
		;
	}
	else{
	//if(json.IsLoading()){
		json
			("Data", cvLst)
		;
		Set(cvLst);
		hcmEntity = StrToHcmEntity(map[0].GetStr());
	}
}
void HcmIN::Set(const Vector<CodeVec>& cvLst){
	for(auto cv: cvLst){
		for(int i=0; i< map.GetCount(); i++){
			if(map[i].GetCode() == cv.code){
				map[i].EditVal(cv.vec);
				break;
			}
		}
	}
}


/*
bool HcmIN::Check_4C(HcmField& fld){
	if(fld.GetCode() != "4C") return true;
	LOG("Check_4C: " << fld.GetStr());
	return true;
}
*/

bool HcmIN::CheckIfMandat(HcmField& fld){
	FldMandat fmd = fld.GetDesct().GetMandat();
	if(fmd == FldMandat::OFF || fmd == FldMandat::ON) return true;
	if(fmd == FldMandat::RX && hcmEntity == HcmEntity::RX && fld.IsEmpty()){
		fld.SetErMsg(Format(" [%s] %s %s", fld.GetCode(), fld.GetName(), " is missing"));
		return false;
	}
	if(fmd == FldMandat::TX && hcmEntity == HcmEntity::TX && fld.IsEmpty()){
		fld.SetErMsg(Format(" [%s] %s %s", fld.GetCode(), fld.GetName(), " is missing"));
		return false;
	}
	return true;
}

bool HcmIN::Check_7A(HcmField& fld){
	if(fld.GetCode() != "7A") return true;
	// Designation of emission: 28M0G7W
	HcmEntity entity = StrToHcmEntity(map[0].GetStr());
	String design = TrimBoth(fld.GetStr());
	if(design.IsEmpty() && (entity == HcmEntity::PRX || entity == HcmEntity::PTX))
		return true;
	else if(design.IsEmpty() && fld.GetDesct().IsMandat()){
		fld.SetErMsg(Format(" [%s] %s %s", fld.GetCode(), fld.GetName(), " is missing"));
		return false;
	}
	if(!design.IsEmpty() && (design.GetLength() < 7 || design.GetLength() > 9)){
		fld.SetErMsg(Format(" [%s] %s %s", fld.GetCode(), fld.GetName(), " has incorrect number of characters"));
		return false;
	}
	return true;
}
bool HcmIN::Check_7G(HcmField& fld){
	String code = fld.GetCode().Mid(0, 3);
	if(code != "7Ga" && code != "7Gf" ) return true;
	if(code == "7Gf"){
		
	}
	else{
		
	}
	return true;
}

bool HcmIN::Check_7G3(HcmField& fld){
	if(fld.GetCode() != "7G3") return true;
	LOG("Check_7G3: " << fld.GetStr());
	return true;
}
bool HcmIN::Check_8B(HcmField& fld){
	if(fld.GetCode() != "8B") return true;
	LOG("Check_8B: " << fld.GetStr());
	return true;
}
bool HcmIN::Check_9A(HcmField& fld){
	if(fld.GetCode() != "9A") return true;
	LOG("Check_9A: " << fld.GetStr());
	return true;
}
bool HcmIN::Check_13Y(HcmField& fld){
	if(fld.GetCode() != "13Y") return true;
	LOG("Check_13Y: " << fld.GetStr());
	return true;
}
bool HcmIN::Check_13X(HcmField& fld){
	if(fld.GetCode() != "13X") return true;
	if(fld.IsEmpty() && !fld.GetDesct().IsMandat()) return true;

	// 0-2: CCC		Country requesting co-ordination
	String countryCode = TrimBoth(fld.GetStr().Mid(0, 3));
	if(!Single<HcmLstData>().Find("4B", countryCode)){
		fld.SetErMsg(Format(" [%s] %s %s", fld.GetCode(), fld.GetName(), " code country requesting co-ordination is unknown"));
		return false;
	}
	// 3-6 YYYY	4 digits of the year of initial co-ordination, year (numerical; range: > 1900)
	Value year = StrInt(TrimBoth(fld.GetStr().Mid(3, 4)));
	// TODO the upper bound of the year of initial co-ordination = (GetSysDate().year+1) ?
	if(!IsNumber(year) || int(year) <= 1900 || int(year) > GetSysDate().year+1){
		fld.SetErMsg(Format(" [%s] %s %s", fld.GetCode(), fld.GetName(), " year of initial co-ordination is missing or is not a number"));
		return false;
	}
	// 7-13 PPPPPPP	Process identification
	String process = TrimBoth(fld.GetStr().Mid(7, 7));
	if(process.IsEmpty() || process.GetLength() < 7){
		fld.SetErMsg(Format(" [%s] %s %s", fld.GetCode(), fld.GetName(), " process identification is incorrect"));
		return false;
	}
	// 14-15 FF frequency order number or link order number (numeric)
	Value ff = StrInt(TrimBoth(fld.GetStr().Mid(14, 2)));
	if(!IsNumber(ff)){
		fld.SetErMsg(Format(" [%s] %s %s", fld.GetCode(), fld.GetName(), " frequency order number is not a number"));
		return false;
	}
	// 16-17 RR Number of the associated records (numeric)
	Value rr = StrInt(TrimBoth(fld.GetStr().Mid(16, 2)));
	if(!IsNumber(rr)){
		fld.SetErMsg(Format(" [%s] %s %s", fld.GetCode(), fld.GetName(), " number of the associated records is not a number"));
		return false;
	}
	// 18-19 OO	Order number of the record in the assignment (numeric)
	Value oo = StrInt(TrimBoth(fld.GetStr().Mid(18, 2)));
	if(!IsNumber(oo)){
		fld.SetErMsg(Format(" [%s] %s %s", fld.GetCode(), fld.GetName(), " order number of the record in the assignment is not a number"));
		return false;
	}
	return true;
}
bool HcmIN::Check_9X(HcmField& fld){
	if(fld.GetCode() != "9X") return true;
	LOG("Check_9X: " << fld.GetStr());
	return true;
}


void HcmIN::Check(String& vlog){
	for(int i=0; i< map.GetCount(); i++){
		if(!map[i].Check()){
			vlog << (map[i].GetErMsg()) << "\n";
			continue;
		}
		if(!CheckIfMandat(map[i])){
			vlog << (map[i].GetErMsg()) << "\n";
			continue;
		}
		if(!Check_7A(map[i])){
			vlog << (map[i].GetErMsg()) << "\n";
			continue;
		}
		if(!Check_7G(map[i])){
			vlog << (map[i].GetErMsg()) << "\n";
			continue;
		}
		if(!Check_7G3(map[i])){
			vlog << (map[i].GetErMsg()) << "\n";
			continue;
		}
		if(!Check_8B(map[i])){
			vlog << (map[i].GetErMsg()) << "\n";
			continue;
		}
		if(!Check_9A(map[i])){
			vlog << (map[i].GetErMsg()) << "\n";
			continue;
		}
		if(!Check_13Y(map[i])){
			vlog << (map[i].GetErMsg()) << "\n";
			continue;
		}
		if(!Check_13X(map[i])){
			vlog << (map[i].GetErMsg()) << "\n";
			continue;
		}
		if(!Check_9X(map[i])){
			vlog << (map[i].GetErMsg()) << "\n";
			continue;
		}
	}
}


} // namespace HCMFS

#endif
