#ifndef __HCM4A_r452_18_lib_452_18_h_
#define __HCM4A_r452_18_lib_452_18_h_

#include <iostream>
#include <fstream>
#include <sstream>
#include "terrain.h"
#include "common.h"

using namespace std;

namespace R452_18
{
	
#ifndef M_PI
# define M_PI   3.141592653589793238462643383279502884
#endif 
//! --------------------------------------------------------------------------------------------


// --- polarization:
enum class Plz {hor=1, vert=2};
// Plz::hor  - (1)horizontal
// Plz::vert - (2)vertical
inline Plz IntToPlz(int p){
	if(p == static_cast<int>(Plz::hor))  return Plz::hor;
	if(p == static_cast<int>(Plz::vert)) return Plz::vert;
	return Plz::hor;
}
inline int PlzToInt(Plz p){
	return static_cast<int>(p);
}
inline string PlzToStr(Plz p){
	switch(p){
		case Plz::hor:
			return "hor";
		case Plz::vert:
			return "vert";
	}
	return "hor";
}
//! --------------------------------------------------------------------------------------------

// --- pathtype:
enum class PathType {los=1, thor=2};
// PathType::los	- (1)[Line of Sight]
// PathType::thor	- (2)[Trans-Horizon]
inline PathType IntToPathType(int t){
	if(t == static_cast<int>(PathType::los))  return PathType::los;
	if(t == static_cast<int>(PathType::thor)) return PathType::thor;
	return PathType::los;
}
inline int PathTypeToInt(PathType t){
	return static_cast<int>(t);
}
inline string PathTypeToStr(PathType t){
	switch(t){
		case PathType::los:
			return "los";
		case PathType::thor:
			return "thor";
	}
	return "los";
}
//! --------------------------------------------------------------------------------------------

// the sin with arguments in degrees
inline double SinD(double deg){
    return sin(deg * M_PI / 180.0);
}
// the cos with arguments in degrees
inline double CosD(double deg){
    return cos(deg * M_PI / 180.0);
}
// the arctan with arguments in degrees
inline double Atan2D(double y, double x){
    return atan2(y, x) * 180.0 / M_PI;
}
// the arcsin with arguments in degrees
inline double AsinD(double y){
    return asin(y) * 180.0 / M_PI;
}
//! --------------------------------------------------------------------------------------------

// --- For DLL only ---
class DigitalMap{
	std::vector<std::vector<double> > _map;
	size_t _sizeX, _sizeY;
private:
	double _spacing;
public:
	DigitalMap(char* pSt, char* pEnd, bool isLastAndFirstColEqual = true){
	    std::string record;
	    for(char* p = pSt; p != pEnd; ++p){
			record+= *p;
			if(*p == '\n'){
				std::istringstream is(record);
				std::vector<double> row((std::istream_iterator<double>(is)), std::istream_iterator<double>());
				_map.push_back(row);
				record = "";
			}
		}
		_sizeX = _map[0].size();  // num cols
		_sizeY = _map.size();     // num rows
	    if(isLastAndFirstColEqual){
			_spacing = 360.0 / (_map[1].size() - 1);
		}
		else{
			_spacing = 360.0 / (_map[1].size());
		}
	}
public:
	DigitalMap(void){
		_sizeX = 0;  // num cols
		_sizeY = 0;  // num rows
		_spacing = 0;
	}
public:
	double GetInterpolatedValue(double longitude, double latitude)const{
	    double longitudeOffset = longitude;
	    if (longitude < 0.0){
			longitudeOffset = longitude + 360.0;
		}
	    double latitudeOffset = 90.0 - latitude;
	    int latitudeIndex = (int)(latitudeOffset/_spacing);
	    int longitudeIndex = (int)(longitudeOffset/_spacing);
	    latitudeIndex %= _sizeY;
	    longitudeIndex %= _sizeX;

	    double latitudeFraction = (latitudeOffset/_spacing) - latitudeIndex;
	    double longitudeFraction = (longitudeOffset/_spacing) - longitudeIndex;

		double value_ul = _map[latitudeIndex][longitudeIndex];
	    double value_ur = _map[latitudeIndex][(longitudeIndex + 1) % _sizeX];
		double value_ll = _map[(latitudeIndex + 1) % _sizeY][longitudeIndex];
	    double value_lr =
        _map[(latitudeIndex + 1) % _sizeY][(longitudeIndex + 1) % _sizeX];

	    double interpolatedHeight1 = (longitudeFraction * (value_ur - value_ul)) + value_ul;
	    double interpolatedHeight2 = (longitudeFraction * (value_lr - value_ll)) + value_ll;
	    double interpolatedHeight3 = latitudeFraction*(interpolatedHeight2-interpolatedHeight1)+interpolatedHeight1;
		return interpolatedHeight3;
	}
};
class MapDnN0{
private:
	DigitalMap _mapDN50;
	DigitalMap _mapN050;
public:
	MapDnN0(char* dnSt, char* dnEnd, char* n0St, char* n0End){
		_mapDN50 = DigitalMap(dnSt, dnEnd);
		_mapN050 = DigitalMap(n0St, n0End);
	}
	double GetDN50(double lon, double lat)const{
		return _mapDN50.GetInterpolatedValue(lon, lat);
	}
	double GetN050(double lon, double lat)const{
		return _mapN050.GetInterpolatedValue(lon, lat);
	}
};
//! --------------------------------------------------------------------------------------------


//!	Function: double CalcBeta0(double phi, double dtm, double dlm);
//	This function computes β0(%), the time percentage for which refractivity lapse-rates exceeding 100 N units/km
//	can be expected in the first 100 m of the lower atmosphere
//	as defined in ITU-R P.452-18. Eq:(2)
//
//	Input arguments:
//		double phi     -   path centre latitude (deg)
//		double dtm     -   the longest continuous land (inland + coastal) section of the great-circle path (km)
//		double dlm     -   the longest continuous inland section of the great-circle path (km)
//
//	Output arguments:
//		double b0      -   β0(%) the time percentage that the refractivity gradient (DELTA-N) exceeds 100 N-units/km
//							 in the first 100m of the lower atmosphere
//	Example:
//    double b0 = CalcBeta0(phi, dtm, dlm);
//
double CalcBeta0(double phi, double dtm, double dlm);
//! --------------------------------------------------------------------------------------------


//!	Function: void EarthRadEff(double dn, double& ae, double& ab);
//	This function computes the median value of the effective earth
//	radius, and the effective Earth radius exceeded for beta0
//	as defined in ITU-R P.452-18.
//
//	Input arguments:
//		DN      -   the average radiorefractive index lapse-rate through the
//                 lowest 1 km of the atmosphere (N-units/km)
//
//  Output arguments:
//      ae      -   the median effective Earth radius (km)
//      ab      -   the effective Earth radius exceeded for beta0 of time
//
//  Example:
//      EarthRadEff(dn, ae, ab);
//
void EarthRadEff(double dn, double& ae, double& ab);
//! --------------------------------------------------------------------------------------------


//!	Function: void LineOfSingPropg(...);
//	This function computes: Line-of-sight propagation (including short-term effects)
//  as defined in ITU-R P.452-18. p.4.1
//
//	Input parameters:
//		dfs		-	distance between the transmit and receive antennas (km):
//		f		-   Frequency (GHz)
//		p		-	Required time percentage(s) for which the calculated basic
//					transmission loss is not exceeded
//		b0		-   Point incidence of anomalous propagation for the path
//					central location
//		w		-   Fraction of the total path over water
//		temp    -   Temperature (degrees C)
//		press   -   Dry air pressure (hPa)
//		dlt     -   For a transhorizon path, distance from the transmit antenna to
//					its horizon (km). For a LoS path, each is set to the distance
//					from the terminal to the profile point identified as the Bullington
//					point in the diffraction method for 50% time
//		dlr     -   For a transhorizon path, distance from the receive antenna to
//					its horizon (km). The same note as for dlt applies here.
//
//	Output parameters:
//     Lbfsg   -   Basic transmission loss due to free-space propagation and
//                 attenuation by atmospheric gases
//     Lb0p    -   Basic transmission loss not exceeded for time percentage, p, due to LoS propagation
//     Lb0b    -   Basic transmission loss not exceedd for time percentage, b0, due to LoS propagation
//
//  Example:
//     LineOfSingPropg(dfs, f, p, b0, w, temp, press, dlt, dlr, Lbfsg, Lb0p, Lb0b, Ag);
//
void LineOfSingPropg(double dfs, double f, double p, double b0, double w, double temp, double press, double dlt, double dlr,
					 double& lbfsg, double& lb0p, double& lb0b, double& Ag);
//! --------------------------------------------------------------------------------------------


//!	Function: double CalcLbull(const vector<TrPoint>& prf, const double dtot, double hts, double hrs, double ap, double f);
//	This function computes the Bullington part of the diffraction loss
//	as defined in ITU-R P.452-18 p.4.2.1
//
//	Input parameters:
//		vector<TrPoint> prf	-   vector of TrPoint of the i-th profile point
//								vector contain n+1 profile points
//		double dtot			-   Great-circle path distance (km)
//		double hts			-   transmitter antenna height in meters above sea level (i=0)
//		double hrs			-   receiver antenna height in meters above sea level (i=n)
//		double ap			-   the effective earth radius in kilometers
//		double f			-   frequency expressed in GHz
//
//	Output parameters:
//		double Lbull   -   Bullington diffraction loss for a given path
//
//  Example:
//		double Lbull = CalcLbull(prf, hts, hrs, ap, f);
//
double CalcLbull(const vector<TrPoint>& prf, const double dtot, double hts, double hrs, double ap, double f);
//! --------------------------------------------------------------------------------------------


//!	Function: double CalcLdft_tmp(double epsr, double sigma, double d, double hte, double hre, double adft, double f, Plz plz);
//  This function computes the first-term part of Spherical-Earth diffraction
//  loss exceeded for p time for antenna heights
//  as defined in ITU-R P.452-18, Sec. 4.2.2.1, eq: 30-37
//
//	Input parameters:
//		double epsr		-   Relative permittivity
//		double sigma	-   Conductivity (S/m)
//		double d		-   Great-circle path distance (km)
//		double hte		-   Effective height of interfering antenna (m)
//		double hre		-   Effective height of interfered-with antenna (m)
//		double adft		-   effective Earth radius (km)
//		double f		-   frequency (GHz)
//		Plz plz		    -   horizontal polarization | vertical polarization
//
//	Output parameters:
//		double Ldft_tmp	-	The first-term spherical-Earth diffraction loss not exceeded for p time
//								implementing equations (30-37),
//
//  Example:
//		double Ldft_tmp = CalcLdft_tmp(epsr, sigma, d, hte, hre, adft, f, plz);
//
double CalcLdft_tmp(double epsr, double sigma, double d, double hte, double hre, double adft, double f, Plz plz);
//! --------------------------------------------------------------------------------------------


//!	Function:  double CalcLdft(double d, double hte, double hre, double adft, double f, double omega, Plz plz);
//	This function computes the first-term part of Spherical-Earth diffraction
//	loss exceeded for p time for antenna heights
//	as defined in ITU-R P.452-18, Sec. 4.2.2.1, eq: 29
//
//	Input parameters:
//		double d       -   Great-circle path distance (km)
//		double hte     -   Effective height of interfering antenna (m)
//		double hre     -   Effective height of interfered-with antenna (m)
//		double adft    -   effective Earth radius (km)
//		double f       -   frequency (GHz)
//		double omega   -   fraction of the path over sea
//		Plz plz		   -   horizontal polarization | vertical polarization
//
//	Output parameters:
//		double Ldft   -	The first-term spherical-Earth diffraction loss not exceeded for p time
//
//	Example:
//		double Ldft = CalcLdft(d, hte, hre, adft, f, omega, plz);
//
double CalcLdft(double d, double hte, double hre, double adft, double f, double omega, Plz plz);
//! --------------------------------------------------------------------------------------------

//!	Function:  double CalcLdsph(double d, double hte, double hre, double ap, double f, double omega, Plz plz);
//	This function computes the Spherical-Earth diffraction loss exceeded
//	for p time for antenna heights hte and hre (m)
//	as defined in ITU-R P.452-18, Sec. 4.2.2, eq: 28
//
//	Input parameters:
//		double d       -   Great-circle path distance (km)
//		double hte     -   Effective height of interfering antenna (m)
//		double hre     -   Effective height of interfered-with antenna (m)
//		double ap      -   the effective Earth radius in kilometers
//		double f       -   frequency expressed in GHz
//		double omega   -   the fraction of the path over sea
//		Plz plz		   -   horizontal polarization | vertical polarization
//
//	Output parameters:
//		double Ldsph	-	The spherical-Earth diffraction loss not exceeded for p//!  time
//
//	Example:
//		double Ldsph = CalcLdsph(d, hte, hre, ap, f, omega, plz);
//
double CalcLdsph(double d, double hte, double hre, double ap, double f, double omega, Plz plz);
//! --------------------------------------------------------------------------------------------

	
//!	Function:  double CalcLd(const vector<TrPoint>& prf, const double dtot, double hts, double hrs, double hstd, double hsrd,
//							 double ap, double f, double omega, Plz plz);
//	This function computes the complete 'delta-Bullington' diffraction loss
//	as defined in ITU-R P.452-18, Sec. 4.2.3
//
//	Input parameters:
//		vector<TrPoint> prf	-   vector of TrPoint of the i-th profile point
//						vector contain n+1 profile points
//		double dtot -   Great-circle path distance (km)
//		double hts	-   transmitter antenna height in meters above sea level (i=0)
//		double hrs	-   receiver antenna height in meters above sea level (i=n)
//		double hstd	-   Effective height of interfering antenna (m amsl) c.f. 5.1.6.3
//		double hsrd	-	Effective height of interfered-with antenna (m amsl) c.f. 5.1.6.3
//		double ap	-   the effective Earth radius in kilometers
//		double f	-   frequency expressed in GHz
//		double omega-   the fraction of the path over sea
//		Plz plz		-   horizontal polarization | vertical polarization
//
//	Output parameters:
//		double Ld	-	diffraction loss for the general patha according to
//
//	Example:
//		double Ld = CalcLd(prf, dtot, hts, hrs, hstd, hsrd, ap, f, omega, plz, Ldsph);
//
double CalcLd(const vector<TrPoint>& prf, const double dtot, double hts, double hrs, double hstd, double hsrd,
			  double ap, double f, double omega, Plz plz);
//! --------------------------------------------------------------------------------------------


//!	Function: void GreatCirclePath(double rLong, double tLong, double rLat, double tLat, double re, double dpnt,
//									double& pntLong, double& pntLat, double& bt2r, double& dgc);
//	This function computes the great-circle intermediate points on the
//	radio path as defined in ITU-R P.2001-2 Attachment H
//
//	Input parameters:
//		double rLong   -   Receiver longitude, positive to east (deg)
//		double tLong   -   Transmitter longitude, positive to east (deg)
//		double rLat    -   Receiver latitude, positive to north (deg)
//		double tLat   -   Transmitter latitude, positive to north (deg)
//		double re      -   Average Earth radius (km)
//		double dpnt    -   Distance from the transmitter to the intermediate point (km)
//
//	Output parameters:
//		double pntLong -   Longitude of the intermediate point (deg)
//		double pntLat  -   Latitude of the intermediate point (deg)
//		double bt2r    -   Bearing of the great-circle path from Tx towards the Rx (deg)
//		double dgc     -   Great-circle path length (km)
//
//	Example:
//		GreatCirclePath(rLong, tLong, rLat, tLat, re, dpnt, pntLong, pntLat, bt2r, dgc);
//
void GreatCirclePath(double rLong, double tLong, double rLat, double tLat, double re, double dpnt,
					 double& pntLong, double& pntLat, double& bt2r, double& dgc);
//! --------------------------------------------------------------------------------------------


//!	Function: double InvCumNorm(double x);
//	This function implements an approximation to the inverse cummulative
//	normal distribution function for x <= 0.5 as defined in Attachment 3 to
//	Annex 1 of the ITU-R P.452-18
//
//	Input parameters:
//		double x   -
//	Output parameters:
//		double  -
//
//	Example:
//		Fi = InvCumNorm(p/100);
//
double InvCumNorm(double x);
//! --------------------------------------------------------------------------------------------


//!	Function: double LongestContDist(const vector<TrPoint>& prf);
//	This function computes the longest continuous section of the
//	great-circle path (km) for a given Zone::A1 or Zone::A2
//
//	Input arguments:
//		vector<TrPoint> prf	-   vector of TrPoint of the i-th profile point
//						vector contain n+1 profile points
//
//	Output arguments:
//		dm	-	the longest continuous section of the great-circle path (km) for a given Zone::A1 or Zone::A2
//
//	Example:
//		double dm = LongestContDist(prf);
//
double LongestContDist(const vector<TrPoint>& prf);
//! --------------------------------------------------------------------------------------------

//!	Function: double LongestContDist(const vector<TrPoint>& prf, Zone zone_r);
//	This function computes the longest continuous section of the
//	great-circle path (km) for a given zone_r
//
//	Input arguments:
//		vector<TrPoint> prf	-   vector of TrPoint of the i-th profile point
//						vector contain n+1 profile points
//
//	Output arguments:
//		dm	-	the longest continuous section of the great-circle path (km) for a given zone_r
//
//	Example:
//		double dm = LongestContDist(prf, zone_r);
//
double LongestContDist(const vector<TrPoint>& prf, Zone zone_r);
//! --------------------------------------------------------------------------------------------


//!	Function: double PathFractionSea(const vector<TrPoint>& prf);
//	This function computes the path fraction belonging to a given zone Sea
//	of the great-circle path (km)
//
//	Input arguments:
//		vector<TrPoint> prf	-   vector of TrPoint of the i-th profile point
//						vector contain n+1 profile points
//
//	Output arguments:
//		double omega   -   path fraction belonging to the given zone Sea
//
//	Example:
//		double omega = PathFractionSea(prf);
//
double PathFractionSea(const vector<TrPoint>& prf);
//! --------------------------------------------------------------------------------------------

//!	Function: void SmoothEarthHeights(const vector<TrPoint>& prf, const double dtot, double htg, double hrg, double ae, double f,
//						double& hst, double& hsr, double& hstd, double& hsrd, double& hte, double& hre, double& hm, double& dlt,
//						double& dlr, double& theta_t, double& theta_r, double& theta_tot, PathType& pathtype);
//	This function derives smooth-Earth effective antenna heights according to
//	as defined in ITU-R P.452-16, Sections 4 and 5 of the Annex 2
//
//	Input parameters:
//		vector<TrPoint> prf	-   vector of TrPoint of the i-th profile point
//							vector contain n+1 profile points
//		double dtot		-   Great-circle path distance (km)
//		double htg, hrg	-   Tx and Rx antenna heights above ground level (m)
//		double ae		-   median effective Earth's radius (c.f. Eq (6a))
//		double f		-	frequency (GHz)
//
//	Output parameters:
//
//		double hst, hsr     -   Tx and Rx antenna heights of the smooth-Earth surface amsl (m)
//		double hstd, hsrd   -   Tx and Rx effective antenna heigts for the diffraction model (m)
//		double hte, hre     -   Tx and Rx terminal effective heights for the ducting/layer reflection model (m)
//		double hm           -   The terrain roughness parameter (m)
//		double dlt          -   interfering antenna horizon distance (km)
//		double dlr          -   Interfered-with antenna horizon distance (km)
//		double theta_t      -   Interfering antenna horizon elevation angle (mrad)
//		double theta_r      -   Interfered-with antenna horizon elevation angle (mrad)
//		double theta_tot    -   Angular distance (mrad)
//		PathType pathtype	-   los |transhorizon
//
//	Example:
//		SmoothEarthHeights(prf, dtot, htg, hrg, ae, f, hst, hsr, hstd, hsrd, hte, hre, hm, dlt, dlr, theta_t, theta_r, theta_tot, pathtype);
//
void SmoothEarthHeights(const vector<TrPoint>& prf, const double dtot, double htg, double hrg, double ae, double f,
						double& hst, double& hsr, double& hstd, double& hsrd, double& hte, double& hre, double& hm, double& dlt,
						double& dlr, double& theta_t, double& theta_r, double& theta_tot, PathType& pathtype);
//! --------------------------------------------------------------------------------------------

void GetWaterFraction(const vector<TrPoint>& prf, double distance, double& dct, double& dcr, double& omega);

//! --------------------------------------------------------------------------------------------


//!	Function: double CalcLba(double dtot, double dlt, double dlr, double dct, double dcr, double dlm, double hts, double hrs, double hte, double hre, double hm,
//						 double theta_t, double theta_r, double f, double p, double temp, double press, double omega, double ae, double b0);
//
//	This function computes the basic transmission loss occuring during
//	periods of anomalous propagation (ducting and layer reflection)
//	as defined in ITU-R P.452-18, Sec 4.4, Eq 46
//
//	Input parameters:
//		double dtot         -   Great-circle path distance (km)
//		double dlt          -   interfering antenna horizon distance (km)
//		double dlr          -   Interfered-with antenna horizon distance (km)
//		double dct, dcr     -   Distance over land from the transmit and receive
//								antennas tothe coast along the great-circle interference path (km).
//								Set to zero for a terminal on a ship or sea platform
//		double dlm          -   the longest continuous inland section of the great-circle path (km)
//		double hts, hrs     -   Tx and Rx antenna heights aobe mean sea level amsl (m)
//		double hte, hre     -   Tx and Rx terminal effective heights for the ducting/layer reflection model (m)
//		double hm           -   The terrain roughness parameter (m)
//		double theta_t      -   Interfering antenna horizon elevation angle (mrad)
//		double theta_r      -   Interfered-with antenna horizon elevation angle (mrad)
//		double f            -   frequency expressed in GHz
//		double p            -   percentage of time
//		double temp         -   Temperature (deg C)
//		double press        -   Dry air pressure (hPa)
//		double omega        -   fraction of the total path over water
//		double ae           -   the median effective Earth radius (km)
//		double b0           -   the time percentage that the refractivity gradient (DELTA-N) exceeds 100 N-units/km in the first 100m of the lower atmosphere
//
//	Output parameters:
//		Lba    -   the basic transmission loss due to anomalous propagation
//					(ducting and layer reflection)
//
//     Example:
//     double Lba = CalcLba(dtot, dlt, dlr, dct, dcr, dlm, hts, hrs, hte, hre, hm, theta_t, theta_r, f, p, temp, press, omega, b0)
//
double CalcLba(double dtot, double dlt, double dlr, double dct, double dcr, double dlm, double hts, double hrs, double hte, double hre, double hm,
		   double theta_t, double theta_r, double f, double p, double temp, double press, double omega, double ae, double b0);
//! --------------------------------------------------------------------------------------------
					  

//!	Function: double CalcLbs(double dtot, double theta, double f, double p, double temp, double press, double n0, double gt, double gr)
//	This function computes the basic transmission loss due to troposcatterer
//	not exceeded for p% of time
//	as defined in ITU-R P.452-18, Sec 4.3
//
//	Input parameters:
//		dtot    -   Great-circle path distance (km)
//		theta   -   Path angular distance (mrad)
//		f       -   frequency expressed in GHz
//		p       -   percentage of time
//		temp    -   Temperature (deg C)
//		press   -   Dry air pressure (hPa)
//		n0      -   path centre sea-level surface refractivity derived from Fig. 6
//		gt,gr   -   Antenna gain in the direction of the horizon along the
//					great-circle interference path (dBi)
//
//	Output parameters:
//		Lbs    -	the basic transmission loss due to troposcatterer
//					not exceeded for p// of time
//
//	Example:
//		double Lbs = CalcLbs(dtot, theta, f, p, temp, press, n0, gt, gr);
//
double CalcLbs(double dtot, double theta, double f, double p, double temp, double press, double n0, double gt, double gr);
//! --------------------------------------------------------------------------------------------


//!	Function: void CalcLbs_2(double f, double dt, double hts, double hrs, double ae, double the, double thetat, double thetar,
//					     double phicvn, double phicve, double Gt, double Gr, double p, double hs, double& lbs, double& theta);
//	This function computes the troposcatter basic transmission loss
//	as defined in Section 4.3
//
//	Input parameters:
//		double f       -   Frequency GHz
//		double dt      -   Total distance (km)
//		double ht, hr  -   Altitudes of transmitting antenna and receiving antennas in km
//		double thetat  -   Tx horizon elevation angle relative to the local horizontal (mrad)
//		double thetar  -   Rx horizon elevation angle relative to the local horizontal (mrad)
//		double phicvn  -   Troposcatter common volume latitude (deg)
//		double phicve  -   Troposcatter common volume longitude (deg)
//		double Gt, Gr  -   Gain of transmitting and receiving antenna in the azimuthal direction
//							of the path towards the other antenna and at the elevation angle
//							above the local horizontal of the other antenna in the case of a LoS
//							path, otherwise of the antenna's radio horizon, for median effective
//							Earth radius.
//		double p       -   Percentage of average year for which predicted basic loss
//							is not exceeded (//)
//		double hs      -   height of the earth's surface above sea level (km)
//
//	Output parameters:
//		double Lbs    -   Troposcatter basic transmission loss (dB)
//		double theta  -   Scatter angle (mrad)
//
//	Example:
//     CalcLbs_2(f, dt, ht, hr, thetat, thetar, phicvn, phicve,  Gt, Gr, p, hs, lbs, theta)
//
//void CalcLbs_2(const MapDnN0& maps, double f, double dt, double hts, double hrs, double ae, double the, double thetat, double thetar,
//		   double phicvn, double phicve, double Gt, double Gr, double p, double hs, double& lbs, double& theta);
//! --------------------------------------------------------------------------------------------


//!	Function: CalcDlp(const vector<double>& d, const vector<double>& h, double hts, double hrs, double hstd, double hsrd,
//				  double f, double omega, double p, double b0, double DN, double& Ldp, double& Ld50);
//	This function computes the diffraction loss not exceeded for p// of time
//	as defined in ITU-R P.452-18 (Section 4.5.4)
//
//	Input parameters:
//		vector<TrPoint> prf	-   vector of TrPoint of the i-th profile point
//					vector contain n+1 profile points
//		hts     -   transmitter antenna height in meters above sea level (i=0)
//		hrs     -   receiver antenna height in meters above sea level (i=n)
//		hstd    -   Effective height of interfering antenna (m amsl) c.f. 5.1.6.3
//		hsrd    -   Effective height of interfered-with antenna (m amsl) c.f. 5.1.6.3
//		f       -   frequency expressed in GHz
//		omega   -   the fraction of the path over sea
//		p       -   percentage of time
//		b0      -   the time percentage that the refractivity gradient (DELTA-N) exceeds 100 N-units/km in the first 100m of the lower atmosphere
//		DN      -   the average radio-refractive index lapse-rate through the
//					lowest 1 km of the atmosphere. Note that DN is positive
//					quantity in this procedure
//
//	Output parameters:
//		Ldp		-	diffraction loss for the general path not exceeded for p // of the time
//					according to Section 4.2.4 of ITU-R P.452-16.
//		Ld50	-   diffraction loss for p = 50//
//
//	Example:
//		CalcDlp(prf, hts, hrs, hstd, hsrd, ap, f, omega, p, b0, DN, Ldp, Ld50)
//
void CalcDlp(const vector<TrPoint>& prf, const double dtot, double hts, double hrs, double hstd, double hsrd,
			 double f, double omega, double p, double b0, double DN, Plz plz,
			 double& Ldp, double& Ld50);
//! --------------------------------------------------------------------------------------------

//!	Function: void SpecAtten_p676_11(double f, double p, double rho, double t, double& g_0, double& g_w);
//	This function computes the specific attenuation due to dry air and water vapour,
//	at frequencies up to 1 000 GHz for different values of of pressure, temperature
//	and humidity by means of a summation of the individual resonance lines from
//	oxygen and water vapour according to ITU-R P.676-11
//
//	Input parameters:
//		f       -   Frequency (GHz)
//		p       -   Dry air pressure (hPa)
//		rho     -   Water vapor density (g/m^3)
//		t       -   Temperature (K)
//
//	Output parameters:
//		g_o, g_w   -   specific attenuation due to dry air and water vapour
//
//
//	Example:
//     SpecAtten_p676_11(f, p, rho, t, g_0, g_w);
//
void SpecAtten_p676_11(double f, double p, double rho, double t, double& g_0, double& g_w);
//! --------------------------------------------------------------------------------------------




struct In452{
	double f;		// (GHz)		Frequency (GHz)
	double p;		// (%)			Required time percentage for which the calculated basic transmission loss is not exceeded
	double htg;		// (m)			Tx Antenna center heigth above ground level (m)
	double hrg;		// (m)			Rx Antenna center heigth above ground level (m)
	double tLong;	// (deg)		Tx Longitude (degrees)
	double tLat;	// (deg)		Tx Latitude  (degrees)
	double rLong;	// (deg)		Rx Longitude (degrees)
	double rLat;	// (deg)		Rx Latitude  (degrees)
	double Gt;		// (dBi)		Antenna Tx gain in the direction of the horizon along the great-circle interference path (dBi)
	double Gr;		// (dBi)		Antenna Rx gain in the direction of the horizon along the great-circle interference path (dBi)
	Plz plz;		// (1-h/2-v)	polarization of the signal Plz: horizontal | vertical
	double press;	// (hPa)		Dry air pressure (hPa)
	double temp;	// (deg C)		Air temperature (degrees C)
	double DN;
	double N0;
	void Clear(){
		f = 0.0;
		p = 0.0;
		htg = 0.0;
		hrg = 0.0;
		tLong = 0.0;
		tLat = 0.0;
		rLong = 0.0;
		rLat = 0.0;
		Gt = 0.0;
		Gr = 0.0;
		plz = Plz::vert;
		press = 0.0;
		temp = 0.0;
		DN = 0.0;
		N0 = 0.0;
	}
    string ToString()const{
		std::stringstream ss;
		ss << to_string("In452:", "");
		ss << to_string(" f", f);
		ss << to_string(" p", p);
		ss << to_string(" htg", htg);
		ss << to_string(" hrg", hrg);
		ss << to_string(" txLong", tLong);
		ss << to_string(" txLat", tLat);
		ss << to_string(" rxLong", rLong);
		ss << to_string(" rxLat", rLat);
		ss << to_string(" Gt", Gt);
		ss << to_string(" Gr", Gr);
		ss << to_string(" plz", PlzToStr(plz));
		ss << to_string(" press", press);
		ss << to_string(" temp", temp);
		ss << to_string(" DN", DN);
		ss << to_string(" N0", N0);
		return ss.str();
    };
	In452(){Clear();}
	In452(const double f, const double p, const double htg, const double hrg, const double tLong,
			 const double tLat, const double rLong, const double rLat, const double Gt, const double Gr,
			 Plz plz, const double press, const double temp, const double DN, const double N0):
			 f(f), p(p), htg(htg), hrg(hrg), tLong(tLong), tLat(tLat), rLong(rLong), rLat(rLat),
			 Gt(Gt), Gr(Gr), plz(plz), press(press), temp(temp), DN(DN), N0(N0){}
};

struct Out452{
	double Lbd;
	double Ldp;
	double Lbd50;
	double Lb;
	double Lba;
	double Lbfsg;
	double Lb0p;
	double Lb0beta; //	Lb0b
	double Lbs;
	double theta;
	double theta_t;
	double theta_r;
	PathType pathType;
	double d_lt; //		dlt
	double d_lr; //		dlr
	double d_tm; //		dtm
	double d_lm; //		dlm
	double d_ct; //		Distance over land from the transmit and receive
	double d_cr; //		antennas to the coast along the great-circle interference path (km). Set to zero for a terminal on a ship or sea platform
	double h_te; //		hte
	double h_re; //		hre
	double h_m;	 //		hm
	double h_st; //
	double h_sr; //
	double omega;
	double Ag;
	double DN;
	double N0;
	double elevation_tx;
	double elevation_rx;

	double ae;
	double ab;
	double dtot;
	double hts;
	double hrs;
	double hstd;
	double hsrd;
	double b0;
	double Ldsph;
	double Ld50;
	int error;

	void Clear(){
		Lbd = NO_DATA;
		Ldp = NO_DATA;
		Lbd50 = NO_DATA;
		Lb = NO_DATA;
		Lba = NO_DATA;
		Lbfsg = NO_DATA;
		Lb0p = NO_DATA;
		Lb0beta = NO_DATA;
		Lbs = NO_DATA;
		theta = NO_DATA;
		theta_t = NO_DATA;
		theta_r = NO_DATA;
		PathType pathType = PathType::los;
		d_lt = NO_DATA;
		d_lr = NO_DATA;
		d_tm = NO_DATA;
		d_lm = NO_DATA;
		d_ct = NO_DATA;
		d_cr = NO_DATA;
		h_te = NO_DATA;
		h_re = NO_DATA;
		h_m = NO_DATA;
		h_st = NO_DATA;
		h_sr = NO_DATA;
		omega = NO_DATA;
		Ag = NO_DATA;
		DN = NO_DATA;
		N0 = NO_DATA;
		elevation_tx = NO_DATA;
		elevation_rx = NO_DATA;
		ae = NO_DATA;
		ab = NO_DATA;
		dtot = NO_DATA;
		hts = NO_DATA;
		hrs = NO_DATA;
		hstd = NO_DATA;
		hsrd = NO_DATA;
		b0 = NO_DATA;
		Ldsph = NO_DATA;
		Ld50 = NO_DATA;
		error = 0;
	}
    string ToString()const{
		std::stringstream ss;
		ss << to_string("Out452:", "");
	    ss << to_string(" Lbd", Lbd);
	    ss << to_string(" Ldp", Ldp);
	    ss << to_string(" Lbd50", Lbd50);
	    ss << to_string(" Lb", Lb);
	    ss << to_string(" Lba", Lba);
	    ss << to_string(" Lbfsg", Lbfsg);
	    ss << to_string(" Lb0p", Lb0p);
	    ss << to_string(" Lb0beta", Lb0beta);
	    ss << to_string(" Lbs", Lbs);
	    ss << to_string(" theta", theta);
	    ss << to_string(" theta_t", theta_t);
	    ss << to_string(" theta_r", theta_r);
		ss << to_string(" pathType", PathTypeToStr(pathType));
	    ss << to_string(" d_lt", d_lt);
	    ss << to_string(" d_lr", d_lr);
	    ss << to_string(" d_tm", d_tm);
	    ss << to_string(" d_lm", d_lm);
	    ss << to_string(" d_ct", d_ct);
	    ss << to_string(" d_cr", d_cr);
	    ss << to_string(" h_te", h_te);
	    ss << to_string(" h_re", h_re);
	    ss << to_string(" h_st", h_st);
	    ss << to_string(" h_sr", h_sr);
	    ss << to_string(" h_m", h_m);
	    ss << to_string(" omega", omega);
	    ss << to_string(" Ag", Ag);
	    ss << to_string(" DN", DN);
	    ss << to_string(" N0", N0);
	    ss << to_string(" elevation_tx", elevation_tx);
	    ss << to_string(" elevation_rx", elevation_rx);
		ss << to_string(" ae", ae);
		ss << to_string(" ab", ab);
		ss << to_string(" dtot", dtot);
		ss << to_string(" hts", hts);
		ss << to_string(" hrs", hrs);
		ss << to_string(" hstd", hstd);
		ss << to_string(" hsrd", hsrd);
		ss << to_string(" b0", b0);
		ss << to_string(" Ldsph", Ldsph);
		ss << to_string(" Ld50", Ld50);
		ss << to_string(" Error", error);
		return ss.str();
    };
	Out452(){Clear();}
};


//!	Function: Out452 = CalcLoss(const MapDnN0& maps, const vector<TrPoint>& prf, In452& in);
//
//   This is the MAIN function that computes the basic transmission loss not exceeded for p% of time
//   as defined in ITU-R P.452-18 (Section 4.5) for clear-air conditions. Other functions called from
//   this function.
//
//     Input parameters In452:
//     f       -   Frequency (GHz)
//     p       -   Required time percentage for which the calculated basic
//                 transmission loss is not exceeded
//     d       -   vector of distances di of the i-th profile point (km)
//     h       -   vector of heights hi of the i-th profile point (meters
//                 above mean sea level. Both vectors contain n+1 profile points
//     g       -   vector of clutter + terrain profile heights gi along the path gi = hi + Ri (masl)
//                 where Ri is the (representative) clutter height
//     zone    -   vector of zone types: Coastal land (1), Inland (2) or Sea (3)
//     htg     -   Tx Antenna center heigth above ground level (m)
//     hrg     -   Rx Antenna center heigth above ground level (m)
//     tLong   -   Tx Longitude (degrees)
//     tLat    -   Tx Latitude  (degrees)
//     rLong   -   Rx Longitude (degrees)
//     rLat    -   Rx Latitude  (degrees)
//     Gt, Gr  -   Antenna gain in the direction of the horizon along the
//                 great-circle interference path (dBi)
//     Plz plz -   polarization of the signal Plz: horizontal | vertical
//     press   -   Dry air pressure (hPa)
//     temp    -   Air temperature (degrees C)
//
//     Output parameters Out452:
//		Lb     -   basic  transmission loss according to ITU-R P.452-18
//
//     Example:
//     Out452 outRec = CalcLoss(maps, prf, in);
//
// Out452 CalcLoss(const MapDnN0& maps, const vector<TrPoint>& prf, const In452& in);
//! --------------------------------------------------------------------------------------------

}  // namespace R452_18

#endif
