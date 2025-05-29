#ifndef __HCM4A_TCalc_FS4A_dtSingle_h_
#define __HCM4A_TCalc_FS4A_dtSingle_h_

#include "sqLite.h"

struct TblSingleRec :Moveable<TblSingleRec>{
	//typedef TblSingleRec CLASSNAME;
	int ts_id;
	Time ts_date;
	String ts_txname;
	String ts_prxname;
	String ts_ptxname;
	String ts_rxname;
	String ts_txcountry;
	String ts_rxcountry;
	double ts_i_td;
	double ts_i_i;
	double ts_i_o_area;
	double ts_i_nfd;
	double ts_i_md;
	double ts_i_a_tot;
	int ts_i_error;
	String ts_msgerror;
	String ts_report;
	void Clear(){
		ts_id = 0;
		ts_date = Time();
		ts_txname = "";
		ts_prxname = "";
		ts_ptxname = "";
		ts_rxname = "";
		ts_txcountry = "";
		ts_rxcountry = "";
		ts_i_td = HCM_NO_VALUE;
		ts_i_i = HCM_NO_VALUE;
		ts_i_o_area = HCM_NO_VALUE;
		ts_i_nfd = HCM_NO_VALUE;
		ts_i_md = HCM_NO_VALUE;
		ts_i_a_tot = HCM_NO_VALUE;
		ts_i_error = 0;
		ts_msgerror = "";
		ts_report = "";
	}
	void Jsonize(JsonIO& json){
		json
		("id", ts_id)
		("date", ts_date)
		("txName", ts_txname)
		("prxName", ts_prxname)
		("ptxName", ts_ptxname)
		("rxName", ts_rxname)
		("txCountry", ts_txcountry)
		("rxCountry", ts_rxcountry)
		("td", ts_i_td)
		("i", ts_i_i)
		("o_area", ts_i_o_area)
		("nfd", ts_i_nfd)
		("md", ts_i_md)
		("a_tot", ts_i_a_tot)
		("error", ts_i_error)
		("msgerror", ts_msgerror)
		//("report", ts_report)
		;
	}
	String ToJsonStr()const{return StoreAsJson(*this);};
	void FromJsonStr(const String& s){LoadFromJson(*this, s);};
	String ToString()const{
		return String()
			<< "	id:\t\t " <<  ts_id << "\n"
			<< "	date:\t\t " <<  ts_date << "\n"
			<< "	txName:\t\t " <<  ts_txname << "\n"
			<< "	prxName:\t\t " <<  ts_prxname << "\n"
			<< "	ptxName:\t\t " <<  ts_ptxname << "\n"
			<< "	rxName:\t\t " <<  ts_rxname << "\n"
			<< "	txCountry:\t\t " <<  ts_txcountry << "\n"
			<< "	rxCountry:\t\t " <<  ts_rxcountry << "\n"
			<< "	td:\t\t " <<  ts_i_td << "\n"
			<< "	i:\t\t " <<  ts_i_i << "\n"
			<< "	o_area:\t\t " <<  ts_i_o_area << "\n"
			<< "	nfd:\t\t " <<  ts_i_nfd << "\n"
			<< "	md:\t\t " <<  ts_i_md << "\n"
			<< "	a_tot:\t\t " <<  ts_i_a_tot << "\n"
			<< "	error:\t\t " <<  ts_i_error << "\n"
			<< "	msgError:\t\t " <<  ts_msgerror << "\n"
			//<< "	Ts_report: " <<  ts_report << "\n"
		;
	}
	void FillTitle(Vector<String>& title){
		title.Clear();
		title.Add("id");
		title.Add("date");
		title.Add("txName");
		title.Add("prxName");
		title.Add("ptxName");
		title.Add("rxName");
		title.Add("txCountry");
		title.Add("rxCountry");
		title.Add("td");
		title.Add("i");
		title.Add("o_area");
		title.Add("nfd");
		title.Add("md");
		title.Add("a_tot");
		title.Add("error");
		title.Add("msgError");
	}
	void FillData(Vector<String>& data){
		data.Clear();
		data.Add(String() << IntStr(ts_id));
		data.Add(String() << (ts_date));
		data.Add(String() << (ts_txname));
		data.Add(String() << (ts_prxname));
		data.Add(String() << (ts_ptxname));
		data.Add(String() << (ts_rxname));
		data.Add(String() << (ts_txcountry));
		data.Add(String() << (ts_rxcountry));
		data.Add(String() << DblStr(ts_i_td));
		data.Add(String() << DblStr(ts_i_i));
		data.Add(String() << DblStr(ts_i_o_area));
		data.Add(String() << DblStr(ts_i_nfd));
		data.Add(String() << DblStr(ts_i_md));
		data.Add(String() << DblStr(ts_i_a_tot));
		data.Add(String() << IntStr(ts_i_error));
		data.Add(String() << (ts_msgerror));
	}
	void Set(const TblSingleRec& rec){
		Clear();
		ts_id = rec.ts_id;
		ts_date = rec.ts_date;
		ts_txname = rec.ts_txname;
		ts_prxname = rec.ts_prxname;
		ts_ptxname = rec.ts_ptxname;
		ts_rxname = rec.ts_rxname;
		ts_txcountry = rec.ts_txcountry;
		ts_rxcountry = rec.ts_rxcountry;
		ts_i_td = rec.ts_i_td;
		ts_i_i = rec.ts_i_i;
		ts_i_o_area = rec.ts_i_o_area;
		ts_i_nfd = rec.ts_i_nfd;
		ts_i_md = rec.ts_i_md;
		ts_i_a_tot = rec.ts_i_a_tot;
		ts_i_error = rec.ts_i_error;
		ts_msgerror = rec.ts_msgerror;
		ts_report = rec.ts_report;
	}
	void Set(const DataCalc& dataCalc, const HcmRec& txRec, const HcmRec& rxRec){
		Clear();
		ts_date = GetSysTime();
		ts_txname = txRec.name_of_station;
		ts_prxname = "";
		ts_ptxname = "";
		ts_rxname = rxRec.name_of_station;
		ts_txcountry = txRec.country;
		ts_rxcountry = rxRec.country;
		ts_i_td = dataCalc.td;
		ts_i_i = dataCalc.I;
		ts_i_o_area = dataCalc.overlap_area;
		ts_i_nfd = dataCalc.nfd;
		ts_i_md = dataCalc.md;
		ts_i_a_tot = dataCalc.a_tot;
		ts_i_error = dataCalc.error;
		ts_msgerror = dataCalc.errorMsg;
		ts_report = String()
				 << txRec.ToPrint()  << "\n"
				 << rxRec.ToPrint()  << "\n"
				 << dataCalc.ToReportA();
	}
	void Set(const DataCalc& dataCalc, const HcmRec& txRec, const HcmRec& prxRec, const HcmRec& ptxRec, const HcmRec& rxRec){
		Clear();
		ts_date = GetSysTime();
		ts_txname = txRec.name_of_station;
		ts_prxname = prxRec.name_of_station;
		ts_ptxname = ptxRec.name_of_station;
		ts_rxname = rxRec.name_of_station;
		ts_txcountry = txRec.country;
		ts_rxcountry = rxRec.country;
		ts_i_td = dataCalc.td;
		ts_i_i = dataCalc.I;
		ts_i_o_area = dataCalc.overlap_area;
		ts_i_nfd = dataCalc.nfd;
		ts_i_md = dataCalc.md;
		ts_i_a_tot = dataCalc.a_tot;
		ts_i_error = dataCalc.error;
		ts_msgerror = dataCalc.errorMsg;
		ts_report = String()
				 << txRec.ToPrint()  << "\n"
				 << prxRec.ToPrint() << "\n"
				 << ptxRec.ToPrint() << "\n"
				 << rxRec.ToPrint()  << "\n"
				 << dataCalc.ToReportP();
	}
	void Set(const String& jStr){
		Clear();FromJsonStr(jStr);
	}
	bool IsEmpty(){
		return ts_id == 0;
	}
	TblSingleRec(){Clear();};
	TblSingleRec(const String& jstr){Clear();FromJsonStr(jstr);};
	TblSingleRec(
		const int ts_id,
		const Time& ts_date,
		const String& ts_txname,
		const String& ts_prxname,
		const String& ts_ptxname,
		const String& ts_rxname,
		const String& ts_txcountry,
		const String& ts_rxcountry,
		const double ts_i_td,
		const double ts_i_i,
		const double ts_i_o_area,
		const double ts_i_nfd,
		const double ts_i_md,
		const double ts_i_a_tot,
		const int ts_i_error,
		const String& ts_msgerror,
		const String& ts_report
	):
		ts_id(ts_id),
		ts_date(ts_date),
		ts_txname(ts_txname),
		ts_prxname(ts_prxname),
		ts_ptxname(ts_ptxname),
		ts_rxname(ts_rxname),
		ts_txcountry(ts_txcountry),
		ts_rxcountry(ts_rxcountry),
		ts_i_td(ts_i_td),
		ts_i_i(ts_i_i),
		ts_i_o_area(ts_i_o_area),
		ts_i_nfd(ts_i_nfd),
		ts_i_md(ts_i_md),
		ts_i_a_tot(ts_i_a_tot),
		ts_i_error(ts_i_error),
		ts_msgerror(ts_msgerror),
		ts_report(ts_report)
	{}
	TblSingleRec(const TblSingleRec& rec):
		ts_id(rec.ts_id),
		ts_date(rec.ts_date),
		ts_txname(rec.ts_txname),
		ts_prxname(rec.ts_prxname),
		ts_ptxname(rec.ts_ptxname),
		ts_rxname(rec.ts_rxname),
		ts_txcountry(rec.ts_txcountry),
		ts_rxcountry(rec.ts_rxcountry),
		ts_i_td(rec.ts_i_td),
		ts_i_i(rec.ts_i_i),
		ts_i_o_area(rec.ts_i_o_area),
		ts_i_nfd(rec.ts_i_nfd),
		ts_i_md(rec.ts_i_md),
		ts_i_a_tot(rec.ts_i_a_tot),
		ts_i_error(rec.ts_i_error),
		ts_msgerror(rec.ts_msgerror),
		ts_report(rec.ts_report)
	{}
	TblSingleRec(const DataCalc& dataCalc, const HcmRec& txRec, const HcmRec& rxRec){
		ts_date = GetSysTime();
		ts_txname = txRec.name_of_station;
		ts_prxname = "";
		ts_ptxname = "";
		ts_rxname = rxRec.name_of_station;
		ts_txcountry = txRec.country;
		ts_rxcountry = rxRec.country;
		ts_i_td = dataCalc.td;
		ts_i_i = dataCalc.I;
		ts_i_o_area = dataCalc.overlap_area;
		ts_i_nfd = dataCalc.nfd;
		ts_i_md = dataCalc.md;
		ts_i_a_tot = dataCalc.a_tot;
		ts_i_error = dataCalc.error;
		ts_msgerror = dataCalc.errorMsg;
		ts_report = String()
				 << txRec.ToPrint() << "\n"
				 << rxRec.ToPrint() << "\n"
				 << dataCalc.ToReportA();
	}
	TblSingleRec(const DataCalc& dataCalc, const HcmRec& txRec, const HcmRec& prxRec, const HcmRec& ptxRec, const HcmRec& rxRec){
		ts_date = GetSysTime();
		ts_txname = txRec.name_of_station;
		ts_prxname = prxRec.name_of_station;
		ts_ptxname = ptxRec.name_of_station;
		ts_rxname = rxRec.name_of_station;
		ts_txcountry = txRec.country;
		ts_rxcountry = rxRec.country;
		ts_i_td = dataCalc.td;
		ts_i_i = dataCalc.I;
		ts_i_o_area = dataCalc.overlap_area;
		ts_i_nfd = dataCalc.nfd;
		ts_i_md = dataCalc.md;
		ts_i_a_tot = dataCalc.a_tot;
		ts_i_error = dataCalc.error;
		ts_msgerror = dataCalc.errorMsg;
		//ts_report = dataCalc.ToReportP();
		ts_report = String()
				 << txRec.ToPrint()  << "\n"
				 << prxRec.ToPrint() << "\n"
				 << ptxRec.ToPrint() << "\n"
				 << rxRec.ToPrint()  << "\n"
				 << dataCalc.ToReportP();
	}
	const Vector<NameVal> GetNameValList()const{
		Vector<NameVal> nameValList;
		nameValList.Add(NameVal("", "№", 1));
		nameValList.Add(NameVal("", "txName", ts_txname));
		nameValList.Add(NameVal("", "prxName", ts_prxname));
		nameValList.Add(NameVal("", "ptxName", ts_ptxname));
		nameValList.Add(NameVal("", "rxName", ts_rxname));
		nameValList.Add(NameVal("", "TD", Format("%.3f", ts_i_td)));
		nameValList.Add(NameVal("", "txCountry", ts_txcountry));
		nameValList.Add(NameVal("", "rxCountry", ts_rxcountry));
		nameValList.Add(NameVal("", "I", Format("%.3f", ts_i_i)));
		nameValList.Add(NameVal("", "O_AREA", Format("%.3f", ts_i_o_area)));
		nameValList.Add(NameVal("", "NFD", Format("%.3f", ts_i_nfd)));
		nameValList.Add(NameVal("", "MD", Format("%.3f", ts_i_md)));
		nameValList.Add(NameVal("", "A_TOT", Format("%.3f", ts_i_a_tot)));
		if(!ts_msgerror.IsEmpty())
			nameValList.Add(NameVal("", "Error", Format("%d: %s", ts_i_error, ts_msgerror)));
		else{
			nameValList.Add(NameVal("", "Error", appErrorMap.Get(ts_i_error).ToString()));
		}
		return nameValList;
	}
};
//!---------------------------------------------------------------------------------------------

/*
struct TblSingleData :Moveable<TblSingleData>{
	TblSingleRec Get(Sqlite3Session& sqlSes, const int ts_id);
	TblSingleRec Get(Sqlite3Session& sqlSes);
	Vector<TblSingleRec> GetAll(Sqlite3Session& sqlSes);
	int Fill(Sqlite3Session& sqlSes, Vector<TblSingleRec>& list);
	int Fill(Sqlite3Session& sqlSes, GridCtrl& grd, Font& hFont, Font& rFont);
	bool Ins(Sqlite3Session& sqlSes, const TblSingleRec& rec);
	bool Upd(Sqlite3Session& sqlSes, const TblSingleRec& rec);
	bool Del(Sqlite3Session& sqlSes, const int ts_id);
	bool Del(Sqlite3Session& sqlSes);
	TblSingleData(){}
};

TblSingleRec TblSingleData::Get(Sqlite3Session& sqlSes, const int ts_id){
	TblSingleRec tblSingleRec;
	if(!sqlSes.IsOpen()) return tblSingleRec;
	Sql sql(sqlSes);
	sql.ClearError();
	sql * Select(
		TS_ID,
		TS_Date,
		TS_txName,
		TS_prxName,
		TS_ptxName,
		TS_rxName,
		TS_txCountry,
		TS_rxCountry,
		TS_I_TD,
		TS_I_I,
		TS_I_O_AREA,
		TS_I_NFD,
		TS_I_MD,
		TS_I_A_TOT,
		TS_I_Error,
		TS_msgError,
		TS_report
	).From(TMP_SINGLE).Where(TS_ID == ts_id);
	if(sql.Fetch()){
		tblSingleRec.ts_id = sql[TS_ID];
		tblSingleRec.ts_date = sql[TS_Date];
		tblSingleRec.ts_txname = sql[TS_txName];
		tblSingleRec.ts_prxname = sql[TS_prxName];
		tblSingleRec.ts_ptxname = sql[TS_ptxName];
		tblSingleRec.ts_rxname = sql[TS_rxName];
		tblSingleRec.ts_txcountry = sql[TS_txCountry];
		tblSingleRec.ts_rxcountry = sql[TS_rxCountry];
		tblSingleRec.ts_i_td = sql[TS_I_TD];
		tblSingleRec.ts_i_i = sql[TS_I_I];
		tblSingleRec.ts_i_o_area = sql[TS_I_O_AREA];
		tblSingleRec.ts_i_nfd = sql[TS_I_NFD];
		tblSingleRec.ts_i_md = sql[TS_I_MD];
		tblSingleRec.ts_i_a_tot = sql[TS_I_A_TOT];
		tblSingleRec.ts_i_error = sql[TS_I_Error];
		tblSingleRec.ts_msgerror = sql[TS_msgError];
		tblSingleRec.ts_report = sql[TS_report];
	}
	return tblSingleRec;
}

TblSingleRec TblSingleData::Get(Sqlite3Session& sqlSes){
	TblSingleRec tblSingleRec;
	if(!sqlSes.IsOpen()) return tblSingleRec;
	Sql sql(sqlSes);
	sql.ClearError();
	sql * Select(
		TS_ID,
		TS_Date,
		TS_txName,
		TS_prxName,
		TS_ptxName,
		TS_rxName,
		TS_txCountry,
		TS_rxCountry,
		TS_I_TD,
		TS_I_I,
		TS_I_O_AREA,
		TS_I_NFD,
		TS_I_MD,
		TS_I_A_TOT,
		TS_I_Error,
		TS_msgError,
		TS_report
	).From(TMP_SINGLE).Where(TS_ID == Select(SqlMax(TS_ID)).From(TMP_SINGLE));
	// SELECT qc_id FROM qclaim WHERE qc_id = (select max(qc_id) from qclaim where QA_ID = (?))";
	// Select(SqlMax(COLUMN)).From(TABLE)
	if(sql.Fetch()){
		tblSingleRec.ts_id = sql[TS_ID];
		tblSingleRec.ts_date = sql[TS_Date];
		tblSingleRec.ts_txname = sql[TS_txName];
		tblSingleRec.ts_prxname = sql[TS_prxName];
		tblSingleRec.ts_ptxname = sql[TS_ptxName];
		tblSingleRec.ts_rxname = sql[TS_rxName];
		tblSingleRec.ts_txcountry = sql[TS_txCountry];
		tblSingleRec.ts_rxcountry = sql[TS_rxCountry];
		tblSingleRec.ts_i_td = sql[TS_I_TD];
		tblSingleRec.ts_i_i = sql[TS_I_I];
		tblSingleRec.ts_i_o_area = sql[TS_I_O_AREA];
		tblSingleRec.ts_i_nfd = sql[TS_I_NFD];
		tblSingleRec.ts_i_md = sql[TS_I_MD];
		tblSingleRec.ts_i_a_tot = sql[TS_I_A_TOT];
		tblSingleRec.ts_i_error = sql[TS_I_Error];
		tblSingleRec.ts_msgerror = sql[TS_msgError];
		tblSingleRec.ts_report = sql[TS_report];
	}
	return tblSingleRec;
}

Vector<TblSingleRec> TblSingleData::GetAll(Sqlite3Session& sqlSes){
	Vector<TblSingleRec> tmp_singleList;	if(!sqlSes.IsOpen()) return tmp_singleList;
	Sql sql(sqlSes);
	sql.ClearError();
	sql * Select(
		TS_ID,
		TS_Date,
		TS_txName,
		TS_prxName,
		TS_ptxName,
		TS_rxName,
		TS_txCountry,
		TS_rxCountry,
		TS_I_TD,
		TS_I_I,
		TS_I_O_AREA,
		TS_I_NFD,
		TS_I_MD,
		TS_I_A_TOT,
		TS_I_Error,
		TS_msgError,
		TS_report
	).From(TMP_SINGLE);
	while(sql.Fetch()){
	TblSingleRec& tblSingleRec = tmp_singleList.Add();
		tblSingleRec.ts_id = sql[TS_ID];
		tblSingleRec.ts_date = sql[TS_Date];
		tblSingleRec.ts_txname = sql[TS_txName];
		tblSingleRec.ts_prxname = sql[TS_prxName];
		tblSingleRec.ts_ptxname = sql[TS_ptxName];
		tblSingleRec.ts_rxname = sql[TS_rxName];
		tblSingleRec.ts_txcountry = sql[TS_txCountry];
		tblSingleRec.ts_rxcountry = sql[TS_rxCountry];
		tblSingleRec.ts_i_td = sql[TS_I_TD];
		tblSingleRec.ts_i_i = sql[TS_I_I];
		tblSingleRec.ts_i_o_area = sql[TS_I_O_AREA];
		tblSingleRec.ts_i_nfd = sql[TS_I_NFD];
		tblSingleRec.ts_i_md = sql[TS_I_MD];
		tblSingleRec.ts_i_a_tot = sql[TS_I_A_TOT];
		tblSingleRec.ts_i_error = sql[TS_I_Error];
		tblSingleRec.ts_msgerror = sql[TS_msgError];
		tblSingleRec.ts_report = sql[TS_report];
	}
	return tmp_singleList;
}

int TblSingleData::Fill(Sqlite3Session& sqlSes, Vector<TblSingleRec>& list){
	if(!sqlSes.IsOpen()) return 0;
	Sql sql(sqlSes);
	sql.ClearError();
	sql * Select(
		TS_ID,
		TS_Date,
		TS_txName,
		TS_prxName,
		TS_ptxName,
		TS_rxName,
		TS_txCountry,
		TS_rxCountry,
		TS_I_TD,
		TS_I_I,
		TS_I_O_AREA,
		TS_I_NFD,
		TS_I_MD,
		TS_I_A_TOT,
		TS_I_Error,
		TS_msgError,
		TS_report
	).From(TMP_SINGLE);
	while(sql.Fetch()){
	TblSingleRec& tblSingleRec = list.Add();
		tblSingleRec.ts_id = sql[TS_ID];
		tblSingleRec.ts_date = sql[TS_Date];
		tblSingleRec.ts_txname = sql[TS_txName];
		tblSingleRec.ts_prxname = sql[TS_prxName];
		tblSingleRec.ts_ptxname = sql[TS_ptxName];
		tblSingleRec.ts_rxname = sql[TS_rxName];
		tblSingleRec.ts_txcountry = sql[TS_txCountry];
		tblSingleRec.ts_rxcountry = sql[TS_rxCountry];
		tblSingleRec.ts_i_td = sql[TS_I_TD];
		tblSingleRec.ts_i_i = sql[TS_I_I];
		tblSingleRec.ts_i_o_area = sql[TS_I_O_AREA];
		tblSingleRec.ts_i_nfd = sql[TS_I_NFD];
		tblSingleRec.ts_i_md = sql[TS_I_MD];
		tblSingleRec.ts_i_a_tot = sql[TS_I_A_TOT];
		tblSingleRec.ts_i_error = sql[TS_I_Error];
		tblSingleRec.ts_msgerror = sql[TS_msgError];
		tblSingleRec.ts_report = sql[TS_report];
	}
	return list.GetCount();
}
int TblSingleData::Fill(Sqlite3Session& sqlSes, GridCtrl& grd, Font& hFont, Font& rFont){
	grd.ColoringMode(1);
	grd.OddColor().ColoringMode(1);
	grd.EvenColor().ColoringMode(2);
	grd.Indicator();//

	grd.AddColumn(TS_ID, String(t_("№"))).Min(50).AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grd.AddColumn(TS_txName, String(t_("txName"))).Min(150).AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grd.AddColumn(TS_prxName, String(t_("prxName"))).Min(150).AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grd.AddColumn(TS_ptxName, String(t_("ptxName"))).Min(150).AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grd.AddColumn(TS_rxName, String(t_("rxName"))).Min(150).AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grd.AddColumn(TS_I_TD, String(t_("TD"))).Min(150).AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grd.AddColumn(TS_txCountry, String(t_("txCountry"))).Min(50).AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grd.AddColumn(TS_rxCountry, String(t_("rxCountry"))).Min(50).AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grd.AddColumn(TS_I_I, String(t_("I"))).Min(150).AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grd.AddColumn(TS_I_O_AREA, String(t_("O_AREA"))).Min(150).AlignLeft().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grd.AddColumn(TS_I_NFD, String(t_("NFD"))).Min(50).AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grd.AddColumn(TS_I_MD, String(t_("MD"))).Min(100).AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grd.AddColumn(TS_I_A_TOT, String(t_("A_TOT"))).Min(100).AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grd.AddColumn(TS_I_Error, String(t_("Error"))).Min(150).AlignCenter().SetDisplay(Single<ItemGrid>()).SetHeaderFont(Single<ItemGrid>().headFont);
	grd.RejectNullRow();
	grd.SetToolBar();
	grd.SetRowHeight(0, hFont.GetHeight()+10); // высота заголовка
	grd.Proportional();
	grd.Clear();

	if(!sqlSes.IsOpen()) return 0;
	Sql sql(sqlSes);
	sql.ClearError();
	sql * Select(
		TS_ID,
		TS_Date,
		TS_txName,
		TS_prxName,
		TS_ptxName,
		TS_rxName,
		TS_txCountry,
		TS_rxCountry,
		TS_I_TD,
		TS_I_I,
		TS_I_O_AREA,
		TS_I_NFD,
		TS_I_MD,
		TS_I_A_TOT,
		TS_I_Error,
		TS_msgError,
		TS_report
	).From(TMP_SINGLE);
	int count = 0;
	while(sql.Fetch()){
		count++;
		grd.Add();
		grd(TS_ID) = count;//sql[TS_ID];
		grd(TS_Date) = sql[TS_Date];
		grd(TS_txName) = sql[TS_txName];
		grd(TS_prxName) = sql[TS_prxName];
		grd(TS_ptxName) = sql[TS_ptxName];
		grd(TS_rxName) = sql[TS_rxName];
		grd(TS_txCountry) = sql[TS_txCountry];
		grd(TS_rxCountry) = sql[TS_rxCountry];
		grd(TS_I_TD) = sql[TS_I_TD];
		grd(TS_I_I) = sql[TS_I_I];
		grd(TS_I_O_AREA) = sql[TS_I_O_AREA];
		grd(TS_I_NFD) = sql[TS_I_NFD];
		grd(TS_I_MD) = sql[TS_I_MD];
		grd(TS_I_A_TOT) = sql[TS_I_A_TOT];
		grd(TS_I_Error) = sql[TS_I_Error];
		grd(TS_msgError) = sql[TS_msgError];
		//grd(TS_report) = sql[TS_report];
		grd.SetRowHeight(grd.GetRowCount(), rFont.GetHeight()+int(rFont.GetHeight()/2));
	}
	return grd.GetRowCount();
}

bool TblSingleData::Ins(Sqlite3Session& sqlSes, const TblSingleRec& rec){
	if(!sqlSes.IsOpen()) return false;
	Sql sql(sqlSes);
	sql.ClearError();
	try{
		sql * Insert(TMP_SINGLE)
		//(TS_ID, rec.ts_id)
		(TS_Date, rec.ts_date)
		(TS_txName, rec.ts_txname)
		(TS_prxName, rec.ts_prxname)
		(TS_ptxName, rec.ts_ptxname)
		(TS_rxName, rec.ts_rxname)
		(TS_txCountry, rec.ts_txcountry)
		(TS_rxCountry, rec.ts_rxcountry)
		(TS_I_TD, rec.ts_i_td)
		(TS_I_I, rec.ts_i_i)
		(TS_I_O_AREA, rec.ts_i_o_area)
		(TS_I_NFD, rec.ts_i_nfd)
		(TS_I_MD, rec.ts_i_md)
		(TS_I_A_TOT, rec.ts_i_a_tot)
		(TS_I_Error, rec.ts_i_error)
		(TS_msgError, rec.ts_msgerror)
		(TS_report, rec.ts_report)
		;
	}
	catch(SqlExc &e){
		LOG(Format("ERROR to insert into %s: %s ", TMP_SINGLE.ToString(), e));
		return false;
	}
	return true;
}

bool TblSingleData::Upd(Sqlite3Session& sqlSes, const TblSingleRec& rec){
	if(!sqlSes.IsOpen()) return false;
	Sql sql(sqlSes);
	sql.ClearError();
	try{
		sql * Update(TMP_SINGLE)
		//(TS_ID, rec.ts_id)
		(TS_Date, rec.ts_date)
		(TS_txName, rec.ts_txname)
		(TS_prxName, rec.ts_prxname)
		(TS_ptxName, rec.ts_ptxname)
		(TS_rxName, rec.ts_rxname)
		(TS_txCountry, rec.ts_txcountry)
		(TS_rxCountry, rec.ts_rxcountry)
		(TS_I_TD, rec.ts_i_td)
		(TS_I_I, rec.ts_i_i)
		(TS_I_O_AREA, rec.ts_i_o_area)
		(TS_I_NFD, rec.ts_i_nfd)
		(TS_I_MD, rec.ts_i_md)
		(TS_I_A_TOT, rec.ts_i_a_tot)
		(TS_I_Error, rec.ts_i_error)
		(TS_msgError, rec.ts_msgerror)
		(TS_report, rec.ts_report)
		.Where(TS_ID == rec.ts_id);
	}
	catch(SqlExc &e){
		LOG(Format("ERROR to update into %s: %s ", TMP_SINGLE.ToString(), e));
		return false;
	}
	return true;
}

bool TblSingleData::Del(Sqlite3Session& sqlSes, const int ts_id){
	if(!sqlSes.IsOpen()) return false;
	Sql sql(sqlSes);
	sql.ClearError();
	try{
		sql * Delete(TMP_SINGLE)
		.Where(TS_ID == ts_id);
	}
	catch(SqlExc &e){
		LOG(Format("ERROR to delete %s: %s ", TMP_SINGLE.ToString(), e));
		return false;
	}
	return true;
}
bool TblSingleData::Del(Sqlite3Session& sqlSes){
	if(!sqlSes.IsOpen()) return false;
	Sql sql(sqlSes);
	sql.ClearError();
	try{
		sql * Delete(TMP_SINGLE);
	}
	catch(SqlExc &e){
		LOG(Format("ERROR to delete %s: %s ", TMP_SINGLE.ToString(), e));
		return false;
	}
	return true;
}
*/


#endif
