#ifndef __HCM4A_hcm4a_fs_common_h_
#define __HCM4A_hcm4a_fs_common_h_

#define HCM_NO_VALUE -2222
#define NO_COORD -900.0
#define C_PI 3.1415926
#define EARTH_RADIUS 6371
#define S_RAZDEL ";"
#define C_RAZDEL ';'
#define S_PATH_SEPAR "/"
#define C_PATH_SEPAR '/'

//#include <Core/Core.h>

#include "dtHcmError.h"


// for GridCtrl Fields Display
struct ItemGrid : GridDisplay{
 public:
	Font headFont;
	Font itemFont;
	ItemGrid(bool isMandat = false):isMandat(isMandat){
		headFont = StdFont().Height(StdFont().GetHeight()+3).Bold(); // itemFont = StdFont().Bold().Height(20);
		itemFont = StdFont().Height(StdFont().GetHeight()+1).Bold(); // itemFont = StdFont().Bold().Height(20);
	}
    virtual void Paint(Draw &w, int x, int y, int cx, int cy, const Value &val, dword style,
                       Color &fg, Color &bg, Font &fnt, bool found, int fs, int fe){
        if(!isMandat)
            GridDisplay::Paint(w, x, y, cx, cy, val, style, fg, bg, itemFont, found, fs, fe);
        else
            GridDisplay::Paint(w, x, y, cx, cy, val, style, fg, _bg, itemFont, found, fs, fe);
    }
private:
	bool isMandat;
	Color _bg = Mistyrose(); //(140, 140, 140);
};
//!---------------------------------------------------------------------------------------

class ItemDisplay: public Display {
public:
	Font itemFont;
	Font headFont;
	int DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const;
	virtual Size GetStdSize(const Value& q) const {
		NilDraw w;
		return Size(DoPaint(w, Rect(0, 0, INT_MAX, INT_MAX), q, Null, Null, 0), itemFont.GetCy());
	}
	virtual void Paint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const {
		if(isMandat)
			DoPaint(w, r, q, ink, Mistyrose(), style);
		else
			DoPaint(w, r, q, ink, paper, style);
	}
	ItemDisplay():isMandat(false){ itemFont = StdFont().Height(StdFont().GetHeight()+2).Bold(); headFont = StdFont().Height(StdFont().GetHeight()+4).Bold(); }
	ItemDisplay(bool isMandat):isMandat(isMandat){ itemFont = StdFont().Height(StdFont().GetHeight()+2).Bold(); headFont = StdFont().Height(StdFont().GetHeight()+4).Bold(); }
private:
	bool isMandat;
};
int ItemDisplay::DoPaint(Draw& w, const Rect& r, const Value& q, Color ink, Color paper, dword style) const
{
	String s = AsString(q);
	w.DrawRect(r, paper);
	Font wFont = StdFont().Height(StdFont().GetHeight()).Bold();
	w.DrawText(r.left, r.top, s , wFont, ink);
	return GetTextSize(s, wFont).cx;
}



//! --------------------------------------------------------------------------------------------
void ExecFile(const String& file){
	#ifdef PLATFORM_WIN32 // !PATH_CASE
		//system(file);
		ShellExecuteW(NULL, L"open", ToSystemCharsetW(file), NULL, L".", SW_SHOWDEFAULT);
		//Launch(file);
	#else
		system("./" + file);
	#endif
}
//!---------------------------------------------------------------------------------------------


enum class EditState {None, Edit, Add, Del};
//! --------------------------------------------------------------------------------------------

enum enumTypeOfEntry {tpTX = 1, tpRX = 2, tpPTX = 3, tpPRX = 4, tpUNKNOWN = 5};
//! --------------------------------------------------------------------------------------------

enum enumEqClass {clsNOT_DEFINED, cls2, cls4L, cls4H, cls5A, cls5B, cls6A, cls6B, clsUNKNOWN};
//! --------------------------------------------------------------------------------------------

enum enumAntPattern {aptNOT_DEFINED, aptUNKNOWN, aptCP, aptXP, aptHH, aptVV, aptVH, aptHV};
//! --------------------------------------------------------------------------------------------

enum enFreqUnit {unNOT_DEFINED, K, M, G};
//! --------------------------------------------------------------------------------------------



inline double GetDistance(double longitude1, double latitude1, double longitude2, double latitude2){
  return EARTH_RADIUS * acos(sin(C_PI * latitude1 / 180) * sin(C_PI * latitude2 / 180) + cos(C_PI * latitude1 / 180) *
		 cos(C_PI * latitude2 / 180) * cos((C_PI * longitude1 / 180 - C_PI * longitude2 / 180)));
}
//! --------------------------------------------------------------------------------------------

template<typename T>
String ToStr(const T& t){
    return String() << t;
}
template<typename T>
String ToStr(const String& str, const T& t){
	return String()  << str << ": " << t << "\n";
}
template<typename T>
String ToStr(const String& str, const std::vector<T>& vec){
    String ss = String() << str << ": " << "\n";;
    for(auto t: vec)
		ss  << t.ToString();
    return ss;
}
//! --------------------------------------------------------------------------------------------

template <class T>
inline void DCout(const String& str, T& val){
	#ifdef _DEBUG
		Cout() << str << ": " << val << "\n";
		LOG(String() << str << ": " << val);
	#endif
};
//! --------------------------------------------------------------------------------------------

template <class T>
bool PrintToFile(const String& filename, const T& t){
	FileOut f_out(filename);
	if(!f_out) return false;
	f_out.PutLine(String() << "\n" << t.ToString());
    f_out.Close();
    return true;
}
template <class T>
bool PrintToFile(const String& filename, const String& str, const T& t){
	FileOut f_out(filename);
	if(!f_out) return false;
	f_out.PutLine(String() << "\n" << str << ": " << t);
    f_out.Close();
    return true;
}
//! --------------------------------------------------------------------------------------------

template <class T>
bool VecToFile(const String& filename, const Vector<T>& vec){
	FileOut f_out(filename);
	if(!f_out) return false;
	String str = "\n";
	for(T t: vec){
		str << t.ToString() << "\n";
	}
	f_out.PutLine(str);
    f_out.Close();
    return true;
}
//! --------------------------------------------------------------------------------------------

template <class T>
bool VecToFile2(const String& filename, const Vector<T>& vec){
	FileOut f_out(filename);
	if(!f_out) return false;
	String str = "\n";
	for(T t: vec){
		str << t << "\n";
	}
	f_out.PutLine(str);
    f_out.Close();
    return true;
}
//! --------------------------------------------------------------------------------------------

template<typename T>
std::string to_string(const T &t){
    std::stringstream ss;
    ss << t;
    return ss.str();
}
template<typename T>
std::string to_string(const std::vector<T>& vec){
    std::stringstream ss;
    ss  << "\n";
    for(auto t: vec)
		ss << t.ToStdString();
    return ss.str();
}
template<typename T>
std::string to_string(const std::string& str, const T &t){
    std::stringstream ss;
    ss  << str << ": " << t << "\n";
    return ss.str();
}
template<typename T>
std::string to_string(const std::string& str, const std::vector<T>& vec){
    std::stringstream ss;
    ss  << str << ": " << "\n";
    for(auto t: vec)
		ss << t.ToStdString();
    return ss.str();
}
//! --------------------------------------------------------------------------------------------


// --- polarization: 1-horizontal, 2-vertical
enum class Plz {hor=1, vert=2};
// Plz::hor - horizontal
// Plz::vert - vertical
inline Plz IntToPlz(int p){
	if(p == static_cast<int>(Plz::hor))  return Plz::hor;
	if(p == static_cast<int>(Plz::vert)) return Plz::vert;
	return Plz::hor;
}
inline int PlzToInt(Plz p){
	return static_cast<int>(p);
}
inline String PlzToStr(Plz t){
	String rStr = "?";
	switch(t){
		case Plz::hor:
			rStr = "Hor";
			break;
		case Plz::vert:
			rStr = "Vert";
			break;
	}
	return rStr;
}
inline Plz StrToPlz(const String& str){
	if(str == "Hor")	return Plz::hor;
	if(str == "Vert")	return Plz::vert;
	return Plz::vert;
}
void PlzToDropList(DropList& dList){
	dList.Clear();
	dList.Add(PlzToStr(Plz::hor));
	dList.Add(PlzToStr(Plz::vert));
}
//! --------------------------------------------------------------------------------------------


//String OpenFileTXT(const String& workDir);

String OpenFileTXT(const String& workDir){
	FileSel fs;
	fs.ReadOnlyOption(true);
	fs.MkDirOption(false);
	if(workDir.IsEmpty())
		fs.ActiveDir(GetExeFolder());
	else
		fs.ActiveDir(workDir);
	//fs.Type("file TXT|XML|JSON: [*.txt;*.xml;*.json]", "*.txt *.xml *.json");
	fs.Type("file TXT: [*.txt]", "*.txt");
	//fs.Type("file TXT|XML: [*.txt;*.xml]", "*.txt *.xml");
	if(fs.ExecuteOpen()){
		return fs;
	}
	else
		return "";
}

//----------------------------------------------------------------------------------------------
//	PrintToFile(reportFileName, calcMultResult[recNo].tl_report);
//	ExecFile(reportFileName);
//----------------------------------------------------------------------------------------------

#endif
