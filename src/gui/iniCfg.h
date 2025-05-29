#ifndef _LSA_iniCfg_h_
#define _LSA_iniCfg_h_

/// для работы с файлами типа *.ini

struct IniCfg{
	String GetIniFile()const{return _iniPath;}
	String GetValue(const String& key)const;
	void SetValue(const String& key, const String& val);
	bool Load();
	void Save();
	String ToString()const;
	IniCfg();
	IniCfg(const String& path);
	IniCfg(const VectorMap<String, String>& map);
	IniCfg(const String& path, const VectorMap<String, String>& map);
protected:
	String _iniPath;
	VectorMap<String, String> _iniMap;
};
IniCfg::IniCfg(){
	_iniPath = String() << GetExeTitle() << ".ini";
	Load();
}
IniCfg::IniCfg(const String& path){
	if(TrimBoth(_iniPath).IsEmpty()) _iniPath = String() << GetExeTitle() << ".ini";
	Load();
}
IniCfg::IniCfg(const VectorMap<String, String>& map){
	_iniPath = String() << GetExeTitle() << ".ini";
	if(!Load()){
		for(const auto& m : ~map){
			_iniMap.GetPut(m.key) = m.value;
		}
	}
	Save();
}
IniCfg::IniCfg(const String& path, const VectorMap<String, String>& map):_iniPath(path){
	if(TrimBoth(_iniPath).IsEmpty()) _iniPath = String() << GetExeTitle() << ".ini";
	if(!Load()){
		for(const auto& m : ~map){
			_iniMap.GetPut(m.key) = m.value;
		}
	}
	Save();
}
bool IniCfg::Load(){
	SetIniFile(_iniPath);
	_iniMap = GetIniKeys();
	_iniMap = LoadIniFile(_iniPath);
	return _iniMap.GetCount();
}
void IniCfg::Save(){
	FileOut out(_iniPath);
	for(KeyValueRef<String, String> pair : ~_iniMap)
		out << Format("%s=%s\n", pair.key, pair.value);
}
void IniCfg::SetValue(const String& key, const String& val){
	int idx = _iniMap.Find(key);
	if(idx > -1)
		_iniMap[idx] = val;
	else
		_iniMap.Add(key, val);
}
String IniCfg::GetValue(const String& key)const{
	int idx = _iniMap.Find(key);
	if(idx > -1)
		return _iniMap[idx];
	else
		return "";
};
String IniCfg::ToString()const{
	String str = "IniCfg:\n";
	for(const auto& m : ~_iniMap)
		str << Format(" %s=%s\n", m.key, m.value);
	return str;
}
//----------------------------------------------------------------------------------------------


#endif
