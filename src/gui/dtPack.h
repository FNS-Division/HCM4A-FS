#ifndef __HCM4A_CalcFs4A_dtPack_h_
#define __HCM4A_CalcFs4A_dtPack_h_

#include <Xlnt/Xlnt.h>

#include "dtTerrain.h"
#include "dtHcmIN.h"

//const VectorMap<int, HcmIN>& GetMap(HcmEntity en)const;
/*
const VectorMap<int, HcmIN>& HcmPack::GetMap(HcmEntity en)const{
	static VectorMap<int, HcmIN> tmpMap;
	for(int i=0; i< hcmMap.GetCount(); i++){
		if(hcmMap[i].GetEntity() == en)
			tmpMap.Add(hcmMap.GetKey(i), clone(hcmMap[i]));
	}
	return tmpMap;
}
*/

enum class HcmPackFormat {TXT, JSON, XLS};

struct HcmPack{
	bool IsEmpty()const{return hcmMap.GetCount()==0;};
	bool IsValid()const;
	const String GetPackFile()const{return packfile;}
	const HcmField GetField(const int recNpp, const int fieldID)const;
	bool FromXLS(const String& file);
	bool FromCSV(const String& file);
	bool FromJson(const String& file);
	bool ToXLS(const String& file);
	bool ToXLS();
	bool ToCSV()const;
	bool ToCSV(const String& file)const;
	void Jsonize(JsonIO& json);
	bool ToJson(const String& file);
	bool ToJson();
	String ToString()const;

	const HcmHeader& GetHeader()const{return header;}
	HcmHeader& GetHeader(){return header;}

	const VectorMap<int, HcmIN>& GetMap()const;
	HcmIN& GetHcmIN(int id);
	HcmPackFormat GetFormat()const{return format;}

	void SetHeader(const HcmHeader& head){header.Set(head);}
	void Clear();
	void SetModified(bool b){isModified = b;}
	bool IsModified()const{return isModified;}
	bool Delete(int id);
	void Add();
	void Add(const HcmIN& hcmIN);
	HcmPack(){Clear();};
	~HcmPack(){Clear();};
protected:
	bool isModified = false;
	HcmPackFormat format;
	String packfile;
	HcmHeader header;
	Vector<String> columns;
	VectorMap<int, HcmIN> hcmMap;	// key = npp
};
void HcmPack::Clear(){
		packfile = "";
		columns.Clear();
		format = HcmPackFormat::TXT;
		header.Clear();
		hcmMap.Clear();
}
bool HcmPack::IsValid()const{
	if(hcmMap.GetCount()){
		for(auto v: hcmMap)
			if(v.GetEntity()==HcmEntity::None) return false;
		return true;
	}
	return false;
}
String HcmPack::ToString()const{
	return Format("\nFile: %s\nHeader: %s, HcmMap: %s\n", packfile, header.ToString(), hcmMap.ToString());
}

const VectorMap<int, HcmIN>& HcmPack::GetMap()const{
	return hcmMap;
}
HcmIN& HcmPack::GetHcmIN(int id){
	static HcmIN hcmIN;
	int idx = hcmMap.Find(id);
	if(idx > -1) return hcmMap[idx];
	return hcmIN;
}
bool HcmPack::Delete(int id){
	int idx = hcmMap.Find(id);
	if(idx > -1){
		hcmMap.Remove(idx);
		isModified = true;
		return true;
	}
	return false;
}
void HcmPack::Add(){
	int newKey = -1;
	while(true){
		newKey++;
		if(hcmMap.Find(newKey)==-1){
			break;
		}
	}
	hcmMap.Add(newKey, HcmIN());
}
void HcmPack::Add(const HcmIN& hcmIN){
	int newKey = -1;
	while(true){
		newKey++;
		if(hcmMap.Find(newKey)==-1){
			break;
		}
	}
	hcmMap.Add(newKey, clone(hcmIN));
}

bool HcmPack::FromXLS(const String& file){
	hcmMap.Clear();
	columns.Clear();
	if(!FileExists(file)){
		LOG(Format("Error: File %s, not found", file));
		return false;
	}
	isModified = false;
	header.Clear();
	FileIn f_in(file);

    xlnt::workbook wb;
	wb.load(~file);
	xlnt::worksheet ws = wb.active_sheet();
	int nrow = 0;
	for(auto row: ws.rows(false)){
		nrow++;
		Vector<String> vhcm;
		for(auto cell: row){
			if(nrow == 1)
				columns.Add(cell.to_string());
			else
				vhcm.Add(cell.to_string());
		}
		if(nrow > 1){
			HcmIN hcmIN; hcmIN.FromVec(vhcm);
			if(hcmIN.GetEntity() != HcmEntity::None)
				hcmMap.Add(hcmMap.GetCount(), clone(hcmIN));
		}
	}
    packfile = file;
    format = HcmPackFormat::XLS;
    return nrow > 0;
}
bool HcmPack::ToXLS(const String& file){
    xlnt::workbook wbOut;
    xlnt::worksheet wsOut = wbOut.active_sheet();
    wsOut.title("hcmfs data");
    Vector<Vector<String>> vec;
	for(auto v: hcmMap){
		vec.Add(v.ToVec(columns));
	}
	// шапка таблицы
    for(int i=0; i<columns.GetCount(); i++){
		wsOut.cell(xlnt::cell_reference(i+1, 1)).value(columns[i].ToStd());
    }
    for(int i=0; i<vec.GetCount(); i++){
	    for(int j=0; j<vec[i].GetCount(); j++)
	        wsOut.cell(xlnt::cell_reference(j+1, i+2)).value(vec[i][j].ToStd());
    }
    wbOut.save(file.ToStd());
    return true;
}
bool HcmPack::ToXLS(){
    return ToXLS(packfile);
}

bool HcmPack::FromCSV(const String& file){
	hcmMap.Clear();
	if(!FileExists(file)){
		LOG(Format("Error: File %s, not found", file));
		return false;
	}
	isModified = false;
	header.Clear();
	FileIn f_in(file);
	int lcnt = 0;
	while(!f_in.IsEof()){
        String tmpS = f_in.GetLine();
		HcmIN hcmIN; hcmIN.FromCSV(tmpS);
        if(IsHcmEntity(hcmIN.GetField(0).GetStr())){
            hcmMap.Add(lcnt, clone(hcmIN));
			lcnt++;
        }
        else{
            if(lcnt==0)
                header.FromCSV(tmpS);
        }
    }
    packfile = file;
    format = HcmPackFormat::TXT;
    return lcnt > 0;
}
bool HcmPack::ToCSV(const String& file)const{
	FileOut f_out(file);
	if(!f_out) return false;
	f_out.PutLine(header.ToCSV());
	for(auto v: hcmMap){
		f_out.PutLine(v.ToCSV());
	}
    f_out.Close();
    return true;
}
bool HcmPack::ToCSV()const{
    return ToCSV(packfile);
}
const HcmField HcmPack::GetField(const int recNpp, const int fieldID)const{
	HcmField hcmField;
	if(recNpp < hcmMap.GetCount()){
		return hcmMap[recNpp].GetField(fieldID);
	}
	return hcmField;
}
void HcmPack::Jsonize(JsonIO& json){
	Vector<HcmIN> vecIN;
	if(json.IsStoring()){
		for(auto v: hcmMap)
			vecIN.Add(clone(v));
		json
			("Header", header)
			("Entry", vecIN)
			;
	}
	else{
	//if(json.IsLoading()){
		header.Clear();
		hcmMap.Clear();
		json
			("Header", header)
			("Entry", vecIN)
			;
		for(int i=0; i< vecIN.GetCount(); i++)
			hcmMap.Add(i, clone(vecIN[i]));
	}
}
bool HcmPack::ToJson(const String& file){
	return StoreAsJsonFile(*this, file);
}
bool HcmPack::ToJson(){
    return ToJson(packfile);
}
bool HcmPack::FromJson(const String& file){
	if(LoadFromJsonFile(*this, file)){
	    format = HcmPackFormat::JSON;
	    packfile = file;
		return true;
	}
	return false;
}
//!---------------------------------------------------------------------------------------

struct PtrRec: Moveable<PtrRec>{
	HcmRec ptx;
	HcmRec prx;
	void Clear(){ptx.Clear(); prx.Clear();}
	void SetTx(const HcmRec& tx){ptx.Set(tx);};
	void SetRx(const HcmRec& rx){prx.Set(rx);};
	PtrRec(){Clear();}
	PtrRec(const PtrRec& rec): ptx(clone(rec.ptx)), prx(clone(rec.prx)){}
	PtrRec(const HcmRec& ptx, const HcmRec& prx): ptx(clone(ptx)), prx(clone(prx)){}
};
//!---------------------------------------------------------------------------------------------

struct MultHcmPack: Moveable<MultHcmPack>{
	Vector<HcmRec> txList;
	Vector<HcmRec> rxList;
	Vector<PtrRec> ptrList;
	void Clear(){txList.Clear(); rxList.Clear(); ptrList.Clear();}
	bool FromCSV(const String& fileRef);
	bool FromPack(const HcmPack& pack);
	String ToString()const;
	HcmError GetError()const;
	MultHcmPack(){Clear();}
};

// HcmEntity {None, HEADER, TX, PRX, PTX, RX};
HcmError MultHcmPack::GetError()const{
	HcmError error;
	for(auto v: txList){
		if(!v.tError.IsOk()) return v.tError;
	}
	for(auto v: rxList){
		if(!v.tError.IsOk()) return v.tError;
	}
	for(auto v: ptrList){
		if(!v.prx.tError.IsOk()) return v.prx.tError;
		if(!v.ptx.tError.IsOk()) return v.ptx.tError;
	}
	return error;
}


bool MultHcmPack::FromPack(const HcmPack& pack){
	const VectorMap<int, HcmIN>& map = pack.GetMap();
	TStdAnt ant;
	Vector<HcmRec> prxList;
	Vector<HcmRec> ptxList;
	for(int i=0; i<map.GetCount(); i++){
		HcmEntity en = map[i].GetEntity();
		if(en == HcmEntity::TX || en == HcmEntity::PRX || en == HcmEntity::PTX || en == HcmEntity::RX){
			HcmIN hcmIN = clone(map[i]);
			HcmRec hcmRec(ant, hcmIN);
			if(en == HcmEntity::TX)
				txList.Add(hcmRec);
			else if(en == HcmEntity::PRX)
				prxList.Add(hcmRec);
			else if(en == HcmEntity::PTX)
				ptxList.Add(hcmRec);
			else if(en == HcmEntity::RX)
				rxList.Add(hcmRec);
		}
	}
	if(ptxList.GetCount() && ptxList.GetCount() == prxList.GetCount()){
		for(int i=0; i< ptxList.GetCount(); i++){
			ptrList.Add(PtrRec(ptxList[i], prxList[i]));
		}
	}
	return txList.GetCount() && rxList.GetCount();
}
bool MultHcmPack::FromCSV(const String& fileRef){
	FileIn file(fileRef);
	if(file.IsError()||file.IsEof()) return false;
	Vector<HcmRec> ptxList;
	Vector<HcmRec> prxList;
	TStdAnt ant;
	while(!file.IsEof()){
		String hsmString = file.GetLine();
		HcmIN hcmIN; hcmIN.FromCSV(hsmString);
        if(IsHcmEntity(hcmIN.GetField(0).GetStr())){
			HcmRec hcmRec(ant, hcmIN);
			if(hcmRec.IsEmpty()) continue;
			if(hcmRec.isTx && !hcmRec.isPassive)
				txList.Add(hcmRec);
			else if(!hcmRec.isTx && !hcmRec.isPassive)
				rxList.Add(hcmRec);
			else if(hcmRec.isTx && hcmRec.isPassive)
				ptxList.Add(hcmRec);
			else if(!hcmRec.isTx && hcmRec.isPassive)
				prxList.Add(hcmRec);
        }
	}
	// it is considered that the number of PTx and RTx must match
	if(ptxList.GetCount() && ptxList.GetCount() == prxList.GetCount()){
		for(int i=0; i< ptxList.GetCount(); i++){
			ptrList.Add(PtrRec(ptxList[i], prxList[i]));
		}
	}
	return txList.GetCount() && rxList.GetCount();
}
String MultHcmPack::ToString()const{
	String rStr = "";
	rStr << "-txList: \n";
	for(auto v: txList)
		rStr << v.ToPrint() << "\n";
	rStr << "-rxList: \n";
	for(auto v: rxList)
		rStr << v.ToPrint() << "\n";
	rStr << "-ptrList: \n";
	for(auto v: ptrList){
	rStr << "-- prx: \n";
		rStr << v.prx.ToPrint() << "\n";
	rStr << "-- ptx: \n";
		rStr << v.ptx.ToPrint() << "\n";
	}
	return rStr;
}
//!---------------------------------------------------------------------------------------------



#endif
