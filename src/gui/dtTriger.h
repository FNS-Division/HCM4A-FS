#ifndef __HCM4A_CalcFS4A_dtTriger_h_
#define __HCM4A_CalcFS4A_dtTriger_h_

struct TrigerRec :Moveable<TrigerRec>{
	int id;
	String name;
	String reference;
	String countries;
	int error;
	void Clear(){
		id = 0;
		name = "";
		reference = "";
		countries = "";
		error = 0;
	}
	void Jsonize(JsonIO& json){
		json
		("id", id)
		("name", name)
		("reference", reference)
		("countries", countries)
		("error", error)
		;
	}
	String ToJsonStr()const{return StoreAsJson(*this);};
	void FromJsonStr(const String& s){LoadFromJson(*this, s);};
	String ToString()const{
		return String()
			<< "	id: " <<  id << "\n"
			<< "	name: " <<  name << "\n"
			<< "	reference: " <<  reference << "\n"
			<< "	countries: " <<  countries << "\n"
			<< "	error: " <<  error << "\n"
		;
	}
	void Set(const TrigerRec& rec){
		Clear();
		id = rec.id;
		name = rec.name;
		reference = rec.reference;
		countries = rec.countries;
		error = rec.error;
	}
	void Set(const HcmRec& hrec, const String& clist, int er){
		Clear();
		name = hrec.name_of_station;
		reference = hrec.coord_ref;
		countries = clist;
		error = er;
	}
	void Set(const String& jStr){
		Clear();FromJsonStr(jStr);
	}
	bool IsEmpty(){
		return id == 0;
	}
	TrigerRec(){Clear();};
	TrigerRec(const String& jstr){Clear();FromJsonStr(jstr);};

	TrigerRec(
		const int id,
		const String& name,
		const String& reference,
		const String& countries,
		const int error
	):
		id(id),
		name(name),
		reference(reference),
		countries(countries),
		error(error)
	{}
	TrigerRec(const TrigerRec& rec):
		id(rec.id),
		name(rec.name),
		reference(rec.reference),
		countries(rec.countries),
		error(rec.error)
	{}
	TrigerRec(const HcmRec& hrec, const String& clist, int er){
		Clear();
		name = hrec.name_of_station;
		reference = hrec.coord_ref;
		countries = clist;
		error = er;
	}
};
//!---------------------------------------------------------------------------------------------


#endif
