#ifndef __HCM4A_TCalc_FS4A_dtMult_h_
#define __HCM4A_TCalc_FS4A_dtMult_h_

#include "sqLite.h"

enum class DuoType{TR, RT};
enum class TrioType{RTR, RTT, TTR, TRR, RRT, TRT};

//	Rtx_Tprxptx_Rrx - RTR
//	Rtx_Tprxptx_Trx - RTT
//	Ttx_Tprxptx_Rrx - TTR
//	Ttx_Rrx			- TR
//	Ttx_Rprxptx_Rrx	- TRR
//	Rtx_Trx			- RT
//	Rtx_Rprxptx_Trx	- RRT

struct TblMultRec :Moveable<TblMultRec>{
	int tl_id;
	Time tl_date;
	String tl_txtestname;
	String tl_prxtestname;
	String tl_ptxtestname;
	String tl_rxtestname;
	String tl_txcountry;
	String tl_rxcountry;
	String tl_pxcountry;
	String tl_txrefname;
	String tl_prxrefname;
	String tl_ptxrefname;
	String tl_rxrefname;
	double tl_i_td;
	double tl_i_i;
	double tl_i_o_area;
	double tl_i_nfd;
	double tl_i_md;
	double tl_i_a_tot;
	int tl_i_error;
	String tl_msgerror;
	String tl_report;
	void Clear(){
		tl_id = 0;
		tl_date = Time();
		tl_txtestname = "";
		tl_prxtestname = "";
		tl_ptxtestname = "";
		tl_rxtestname = "";
		tl_txcountry = "";
		tl_rxcountry = "";
		tl_pxcountry = "";
		tl_txrefname = "";
		tl_prxrefname = "";
		tl_ptxrefname = "";
		tl_rxrefname = "";
		tl_i_td = HCM_NO_VALUE;
		tl_i_i = HCM_NO_VALUE;
		tl_i_o_area = HCM_NO_VALUE;
		tl_i_nfd = HCM_NO_VALUE;
		tl_i_md = HCM_NO_VALUE;
		tl_i_a_tot = HCM_NO_VALUE;
		tl_i_error = 0;
		tl_msgerror = "";
		tl_report = "";
	}
	void Jsonize(JsonIO& json){
		json
		("id", tl_id)
		("date", tl_date)
		("txTestName", tl_txtestname)
		("prxTestName", tl_prxtestname)
		("ptxTestName", tl_ptxtestname)
		("rxTestName", tl_rxtestname)
		("txCountry", tl_txcountry)
		("rxCountry", tl_rxcountry)
		("pxCountry", tl_pxcountry)
		("txRefName", tl_txrefname)
		("prxRefName", tl_prxrefname)
		("ptxRefName", tl_ptxrefname)
		("rxRefname", tl_rxrefname)
		("td", tl_i_td)
		("i", tl_i_i)
		("o_area", tl_i_o_area)
		("nfd", tl_i_nfd)
		("md", tl_i_md)
		("a_tot", tl_i_a_tot)
		("error", tl_i_error)
		("msgerror", tl_msgerror)
		//("Tl_report", tl_report)
		;
	}
	String ToJsonStr()const{return StoreAsJson(*this);};
	void FromJsonStr(const String& s){LoadFromJson(*this, s);};
	String ToString()const{
		return String()
			<< "	id:\t\t " <<  tl_id << "\n"
			<< "	date:\t " <<  tl_date << "\n"
			<< "	txTestName:\t " <<  tl_txtestname << "\n"
			<< "	prxTestName:\t " <<  tl_prxtestname << "\n"
			<< "	ptxTestName:\t " <<  tl_ptxtestname << "\n"
			<< "	rxTestName:\t " <<  tl_rxtestname << "\n"
			<< "	txCountry:\t " <<  tl_txcountry << "\n"
			<< "	rxCountry:\t " <<  tl_rxcountry << "\n"
			<< "	prCountry:\t " <<  tl_pxcountry << "\n"
			<< "	txRefName:\t " <<  tl_txrefname << "\n"
			<< "	prxRefName:\t " <<  tl_prxrefname << "\n"
			<< "	ptxRefName:\t " <<  tl_ptxrefname << "\n"
			<< "	rxRefName:\t " <<  tl_rxrefname << "\n"
			<< "	td:\t\t " <<  tl_i_td << "\n"
			<< "	i:\t\t " <<  tl_i_i << "\n"
			<< "	o_area:\t\t " <<  tl_i_o_area << "\n"
			<< "	nfd:\t\t " <<  tl_i_nfd << "\n"
			<< "	md:\t\t " <<  tl_i_md << "\n"
			<< "	a_tot:\t\t " <<  tl_i_a_tot << "\n"
			<< "	error:\t\t " <<  tl_i_error << "\n"
			<< "	msgError:\t " <<  tl_msgerror << "\n"
			//<< "	Tl_report: " <<  tl_report << "\n"
		;
	}
	void FillTitle(Vector<String>& title){
		title.Clear();
		title.Add("id");
		title.Add("date");
		title.Add("txTestName");
		title.Add("prxTestName");
		title.Add("ptxTestName");
		title.Add("rxTestName");
		title.Add("txCountry");
		title.Add("rxCountry");
		title.Add("prCountry");
		title.Add("txRefName");
		title.Add("prxRefName");
		title.Add("ptxRefName");
		title.Add("rxRefName");
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
		data.Add(String() << IntStr(tl_id));
		data.Add(String() << (tl_date));
		data.Add(String() << (tl_txtestname));
		data.Add(String() << (tl_prxtestname));
		data.Add(String() << (tl_ptxtestname));
		data.Add(String() << (tl_rxtestname));
		data.Add(String() << (tl_txcountry));
		data.Add(String() << (tl_rxcountry));
		data.Add(String() << (tl_pxcountry));
		data.Add(String() << (tl_txrefname));
		data.Add(String() << (tl_prxrefname));
		data.Add(String() << (tl_ptxrefname));
		data.Add(String() << (tl_rxrefname));
		data.Add(String() << DblStr(tl_i_td));
		data.Add(String() << DblStr(tl_i_i));
		data.Add(String() << DblStr(tl_i_o_area));
		data.Add(String() << DblStr(tl_i_nfd));
		data.Add(String() << DblStr(tl_i_md));
		data.Add(String() << DblStr(tl_i_a_tot));
		data.Add(String() << IntStr(tl_i_error));
		data.Add(String() << (tl_msgerror));
	}
	
	void Set(const TblMultRec& rec){
		Clear();
		tl_id = rec.tl_id;
		tl_date = rec.tl_date;
		tl_txtestname = rec.tl_txtestname;
		tl_prxtestname = rec.tl_prxtestname;
		tl_ptxtestname = rec.tl_ptxtestname;
		tl_rxtestname = rec.tl_rxtestname;
		tl_txcountry = rec.tl_txcountry;
		tl_rxcountry = rec.tl_rxcountry;
		tl_pxcountry = rec.tl_pxcountry;
		tl_txrefname = rec.tl_txrefname;
		tl_prxrefname = rec.tl_prxrefname;
		tl_ptxrefname = rec.tl_ptxrefname;
		tl_rxrefname = rec.tl_rxrefname;
		tl_i_td = rec.tl_i_td;
		tl_i_i = rec.tl_i_i;
		tl_i_o_area = rec.tl_i_o_area;
		tl_i_nfd = rec.tl_i_nfd;
		tl_i_md = rec.tl_i_md;
		tl_i_a_tot = rec.tl_i_a_tot;
		tl_i_error = rec.tl_i_error;
		tl_msgerror = rec.tl_msgerror;
		tl_report = rec.tl_report;
	}
	void Set(DuoType duo, const DataCalc& dataCalc, const HcmRec& txRec, const HcmRec& rxRec){
		Clear();
		tl_id = 0;
		tl_date = GetSysTime();
		switch(duo){
			case DuoType::TR:
				tl_txtestname = txRec.name_of_station;
				tl_prxtestname = "";
				tl_ptxtestname = "";
				tl_rxtestname = "";
				tl_txcountry = txRec.country;
				tl_rxcountry = rxRec.country;
				tl_pxcountry = "";
				tl_txrefname = "";
				tl_prxrefname = "";
				tl_ptxrefname = "";
				tl_rxrefname = rxRec.name_of_station;
				break;
			case DuoType::RT:
				tl_txtestname = "";
				tl_prxtestname = "";
				tl_ptxtestname = "";
				tl_rxtestname = rxRec.name_of_station;
				tl_txcountry = txRec.country;
				tl_rxcountry = rxRec.country;
				tl_pxcountry = "";
				tl_txrefname = txRec.name_of_station;
				tl_prxrefname = "";
				tl_ptxrefname = "";
				tl_rxrefname = "";
				break;
		}
		tl_i_td = dataCalc.td;
		tl_i_i = dataCalc.I;
		tl_i_o_area = dataCalc.overlap_area;
		tl_i_nfd = dataCalc.nfd;
		tl_i_md = dataCalc.md;
		tl_i_a_tot = dataCalc.a_tot;
		tl_i_error = dataCalc.error;
		tl_msgerror = dataCalc.errorMsg;
		tl_report = String()
				 << txRec.ToPrint()  << "\n"
				 << rxRec.ToPrint()  << "\n"
				 << dataCalc.ToReportA();
	}
	void Set(TrioType trio, const DataCalc& dataCalc, const HcmRec& txRec, const HcmRec& prxRec, const HcmRec& ptxRec, const HcmRec& rxRec){
		Clear();
		tl_id = 0;
		tl_date = GetSysTime();
		switch(trio){
			case TrioType::RTR:
				tl_txtestname = "";
				tl_prxtestname = prxRec.name_of_station;
				tl_ptxtestname = ptxRec.name_of_station;
				tl_rxtestname = "";
				tl_txcountry = txRec.country;
				tl_rxcountry = rxRec.country;
				tl_pxcountry = prxRec.country;
				tl_txrefname = txRec.name_of_station;
				tl_prxrefname = "";
				tl_ptxrefname = "";
				tl_rxrefname = rxRec.name_of_station;
				break;
			case TrioType::RTT:
				tl_txtestname = "";
				tl_prxtestname = prxRec.name_of_station;
				tl_ptxtestname = ptxRec.name_of_station;
				tl_rxtestname = rxRec.name_of_station;
				tl_txcountry = txRec.country;
				tl_rxcountry = rxRec.country;
				tl_pxcountry = prxRec.country;
				tl_txrefname = txRec.name_of_station;
				tl_prxrefname = "";
				tl_ptxrefname = "";
				tl_rxrefname = "";
				break;
			case TrioType::TTR:
				tl_txtestname = txRec.name_of_station;
				tl_prxtestname = prxRec.name_of_station;
				tl_ptxtestname = ptxRec.name_of_station;
				tl_rxtestname = "";
				tl_txcountry = txRec.country;
				tl_rxcountry = rxRec.country;
				tl_pxcountry = prxRec.country;
				tl_txrefname = "";
				tl_prxrefname = "";
				tl_ptxrefname = "";
				tl_rxrefname = rxRec.name_of_station;
				break;
			case TrioType::TRR:
				tl_txtestname = txRec.name_of_station;
				tl_prxtestname = "";
				tl_ptxtestname = "";
				tl_rxtestname = "";
				tl_txcountry = txRec.country;
				tl_rxcountry = rxRec.country;
				tl_pxcountry = prxRec.country;
				tl_txrefname = "";
				tl_prxrefname = prxRec.name_of_station;
				tl_ptxrefname = ptxRec.name_of_station;
				tl_rxrefname = rxRec.name_of_station;
				break;
			case TrioType::RRT:
				tl_txtestname = "";
				tl_prxtestname = "";
				tl_ptxtestname = "";
				tl_rxtestname = rxRec.name_of_station;
				tl_txcountry = txRec.country;
				tl_rxcountry = rxRec.country;
				tl_pxcountry = prxRec.country;
				tl_txrefname = txRec.name_of_station;
				tl_prxrefname = prxRec.name_of_station;
				tl_ptxrefname = ptxRec.name_of_station;
				tl_rxrefname = "";
				break;
				
			case TrioType::TRT:
				tl_txtestname = txRec.name_of_station;
				tl_prxtestname = "";
				tl_ptxtestname = "";
				tl_rxtestname = rxRec.name_of_station;
				tl_txcountry = txRec.country;
				tl_rxcountry = rxRec.country;
				tl_pxcountry = prxRec.country;
				tl_txrefname = "";
				tl_prxrefname = prxRec.name_of_station;
				tl_ptxrefname = ptxRec.name_of_station;
				tl_rxrefname = "";
				break;
		}
		tl_i_td = dataCalc.td;
		tl_i_i = dataCalc.I;
		tl_i_o_area = dataCalc.overlap_area;
		tl_i_nfd = dataCalc.nfd;
		tl_i_md = dataCalc.md;
		tl_i_a_tot = dataCalc.a_tot;
		tl_i_error = dataCalc.error;
		tl_msgerror = dataCalc.errorMsg;
		tl_report = String()
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
		return tl_id == 0;
	}
	TblMultRec(){Clear();};
	TblMultRec(const String& jstr){Clear();FromJsonStr(jstr);};
	TblMultRec(
		const int tl_id,
		const Time& tl_date,
		const String& tl_txtestname,
		const String& tl_prxtestname,
		const String& tl_ptxtestname,
		const String& tl_rxtestname,
		const String& tl_txcountry,
		const String& tl_rxcountry,
		const String& tl_pxcountry,
		const String& tl_txrefname,
		const String& tl_prxrefname,
		const String& tl_ptxrefname,
		const String& tl_rxrefname,
		const double tl_i_td,
		const double tl_i_i,
		const double tl_i_o_area,
		const double tl_i_nfd,
		const double tl_i_md,
		const double tl_i_a_tot,
		const int tl_i_error,
		const String& tl_msgerror,
		const String& tl_report
	):
		tl_id(tl_id),
		tl_date(tl_date),
		tl_txtestname(tl_txtestname),
		tl_prxtestname(tl_prxtestname),
		tl_ptxtestname(tl_ptxtestname),
		tl_rxtestname(tl_rxtestname),
		tl_txcountry(tl_txcountry),
		tl_rxcountry(tl_rxcountry),
		tl_pxcountry(tl_pxcountry),
		tl_txrefname(tl_txrefname),
		tl_prxrefname(tl_prxrefname),
		tl_ptxrefname(tl_ptxrefname),
		tl_rxrefname(tl_rxrefname),
		tl_i_td(tl_i_td),
		tl_i_i(tl_i_i),
		tl_i_o_area(tl_i_o_area),
		tl_i_nfd(tl_i_nfd),
		tl_i_md(tl_i_md),
		tl_i_a_tot(tl_i_a_tot),
		tl_i_error(tl_i_error),
		tl_msgerror(tl_msgerror),
		tl_report(tl_report)
	{}
	TblMultRec(const TblMultRec& rec):
		tl_id(rec.tl_id),
		tl_date(rec.tl_date),
		tl_txtestname(rec.tl_txtestname),
		tl_prxtestname(rec.tl_prxtestname),
		tl_ptxtestname(rec.tl_ptxtestname),
		tl_rxtestname(rec.tl_rxtestname),
		tl_txcountry(rec.tl_txcountry),
		tl_rxcountry(rec.tl_rxcountry),
		tl_pxcountry(rec.tl_pxcountry),
		tl_txrefname(rec.tl_txrefname),
		tl_prxrefname(rec.tl_prxrefname),
		tl_ptxrefname(rec.tl_ptxrefname),
		tl_rxrefname(rec.tl_rxrefname),
		tl_i_td(rec.tl_i_td),
		tl_i_i(rec.tl_i_i),
		tl_i_o_area(rec.tl_i_o_area),
		tl_i_nfd(rec.tl_i_nfd),
		tl_i_md(rec.tl_i_md),
		tl_i_a_tot(rec.tl_i_a_tot),
		tl_i_error(rec.tl_i_error),
		tl_msgerror(rec.tl_msgerror),
		tl_report(rec.tl_report)
	{}
	~TblMultRec(){};
};

struct TblMultData :Moveable<TblMultData>{
	Vector<TblMultRec> multCalcLst;
	TblMultData(){multCalcLst.Clear();}
	TblMultData(const Vector<TblMultRec>& multCalcLst):multCalcLst(clone(multCalcLst)){}
	TblMultData(const TblMultData& multData):multCalcLst(clone(multData.multCalcLst)){}
};

#endif
