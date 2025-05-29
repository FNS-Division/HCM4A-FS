#ifndef __HCM4A_hcm4a_fs_common_h_
#define __HCM4A_hcm4a_fs_common_h_

#include <iostream>
#include <sstream>
#include <fstream>

#define NO_DATA -2222
#define NO_COORD -900.0
#define C_PI 3.1415926
#define EARTH_RADIUS 6371
#define S_RAZDEL ";"
#define C_RAZDEL ';'

#ifdef _WIN32
	#define path_separator "\\"
	#define disk_separator ":\\"
#else
	#define path_separator "/"
	#define disk_separator "/"
#endif
//! --------------------------------------------------------------------------------------------


enum class TEntryToEntry {TX_RX, TX_PRX, PTX_RX};

template<typename T>
std::string to_string(const T &t){
    std::stringstream ss;
    ss << t;
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
    //ss  << str << ": " << "\n";
    for(auto t: vec)
		ss  << t.ToString();
    return ss.str();
}
template<typename T>
std::string to_string(const std::vector<T>& vec){
    std::stringstream ss;
    ss  << "\n";
    for(auto t: vec)
		ss  << t << "\n";
    return ss.str();
}
//! --------------------------------------------------------------------------------------------

template <class T>
inline void DCout(const std::string& str, T val){
	#ifdef _DEBUG
		std::cout << str << ": " << val << "\n";
	#endif
};
//! --------------------------------------------------------------------------------------------

template <class T>
bool TypeToFile(const std::string& filename, const T& t){
#ifdef _DEBUG
	std::ofstream outFile(filename);
	std::stringstream ss;
	ss << "\n";
	ss << t.ToString();
    outFile << ss.rdbuf();
    outFile.close();
#endif
    return true;
}
//! --------------------------------------------------------------------------------------------

template <class T>
bool VecToFile(const std::string& filename, const std::vector<T>& vec){
#ifdef _DEBUG
	std::ofstream outFile(filename);
	std::stringstream ss;
	ss << "\n";
	for(T t: vec){
		ss << t.ToString();
	}
    outFile << ss.rdbuf();
    outFile.close();
#endif
    return true;
}
//! --------------------------------------------------------------------------------------------

template <class T>
bool StrVecToFile(const std::string& filename, const std::vector<T>& vec){
#ifdef _DEBUG
	std::ofstream outFile(filename);
	std::stringstream ss;
	ss << "\n";
	for(T t: vec){
		ss << t << "\n";
	}
    outFile << ss.rdbuf();
    outFile.close();
#endif
    return true;
}
//! --------------------------------------------------------------------------------------------

template <class T>
bool LogToFile(const std::string& filename, const std::string& str, const T& t){
#ifdef _DEBUG
	std::fstream outFile;
	outFile.open(filename, std::ios::app);  // открываем файл для записи в конец
	std::stringstream ss;
	ss <<  str << ": " << t << "\n";
    outFile << ss.rdbuf();
    outFile.close();
#endif
    return true;
}
//! --------------------------------------------------------------------------------------------

#endif
