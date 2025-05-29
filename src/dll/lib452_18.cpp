#include "lib452_18.h"

namespace R452_18
{

double CalcBeta0(double phi, double dtm, double dlm){
    double tau = 1 - exp(-(4.12 * 1e-4 * pow(dlm, 2.41)));											// Eq:(3a)
    double mu1 = pow(pow(10, -dtm / (16 - 6.6 * tau)) + pow(10, -5 * (0.496 + 0.354 * tau)), 0.2);	// Eq:(3)
    mu1 = std::min(mu1, 1.0);
    double b0 = 0.0;
    if(abs(phi) <= 70){
        double mu4 = pow(10, ((-0.935 + 0.0176 * abs(phi)) * log10(mu1)) );	// Eq:(4)
        b0 = pow(10, (-0.015 * abs(phi) + 1.67)) * mu1 * mu4;				// Eq:(2)
    }
    else{
        double mu4 = pow(10, (0.3 * log10(mu1)));							// Eq:(4)
			        b0 = 4.17 * mu1 * mu4;                                  // Eq:(2)
    }
	return b0;
}
//! --------------------------------------------------------------------------------------------

void EarthRadEff(double dn, double& ae, double& ab){
	double k50 = 157/(157-dn);	// Eq:(5)
	ae = 6371*k50;				// Eq:(6a)
	ab = 6371*3.0;				// Eq:(6b)
}
//! --------------------------------------------------------------------------------------------

void LineOfSingPropg(double dfs, double f, double p, double b0, double w, double temp, double press, double dlt, double dlr,
					 double& Lbfsg, double& Lb0p, double& Lb0b, double& Ag){
	// water vapor density
	double rho = 7.5 + 2.5 * w;	// Eq:(9a)
	// compute specific attenuation due to dry air and water vapor:
	double g_0 = 0.0;
	double g_w = 0.0;
	SpecAtten_p676_11(f, press, rho, temp + 273.15, g_0, g_w);
	Ag = (g_0+g_w)*dfs;	// Eq:(9)
	// Basic transmission loss due to free-space propagation and attenuation by atmospheric gases
	Lbfsg = 92.4+20.0*log10(f)+20.0*log10(dfs)+Ag;	// Eq:(8)
	// Corrections for multipath and focusing effects at p and b0
	double Esp = 2.6*(1-exp(-0.1*(dlt+dlr)))*log10(p/50);	// Eq:(10a)
	double Esb = 2.6*(1-exp(-0.1*(dlt+dlr)))*log10(b0/50);	// Eq:(10b)
	// Basic transmission loss not exceeded for time percentage p due to LoS propagation
	Lb0p = Lbfsg + Esp;	//	Eq:(11)
	// Basic transmission loss not exceeded for time percentage b0 due to LoS propagation
	Lb0b = Lbfsg + Esb;	//	Eq:(12)
}
//! --------------------------------------------------------------------------------------------

double CalcLbull(const vector<TrPoint>& prf, const double dtot, double hts, double hrs, double ap, double f){
	// Effective Earth curvature Ce (km^-1)
	double Ce = 1/ap;
	// Wavelength in meters speed of light as per ITU-R P.2001
	double lambda = 0.2998/f;
	// Complete path length
	// intermediate profile point with the highest slope of the line from the transmitter to the point
    double Stim = (prf[1].height + 500.0*Ce*prf[1].dist*(dtot - prf[1].dist) - hts)/prf[1].dist;			// Eq:(14)
    for(int i = 2; i < prf.size()-1; i++){
        double Stim_i = ((prf[i].height + (500.0*Ce*prf[i].dist*(dtot - prf[i].dist))) - hts)/prf[i].dist; // Eq:(14)
        Stim = max(Stim, Stim_i);
    }
	// Calculate the slope of the line from transmitter to receiver assuming a LoS path
	double Str = (hrs-hts)/dtot;	// Eq:(15)
    double Luc = 0.0;
	if(Stim < Str){	// Case 1, Path is LoS
	    // Find the intermediate profile point with the highest diffraction parameter nu:
        double Nu_max = (prf[1].height + 500*Ce*prf[1].dist*(dtot - prf[1].dist) - (hts*(dtot - prf[1].dist) + hrs*prf[1].dist)/dtot) * sqrt(0.002 * dtot/(lambda*prf[1].dist*(dtot - prf[1].dist)));
        for(int i = 2; i < prf.size()-1; i++){
            double Nu_max_i = (prf[i].height + 500*Ce*prf[i].dist*(dtot - prf[i].dist) - (hts*(dtot - prf[i].dist) + hrs*prf[i].dist)/dtot) * sqrt(0.002*dtot/(lambda*prf[i].dist*(dtot-prf[i].dist)));
            Nu_max = max(Nu_max_i, Nu_max);
        }
		if(Nu_max > -0.78)
			Luc = 6.9 + 20*log10(sqrt(pow(Nu_max-0.1, 2)+1)+Nu_max-0.1);	// Eq:(13), (17)
    }
	else{
		// Path is transhorizon Find the intermediate profile point with the highest slope of the line from the receiver to the point
        double Srim = (prf[1].height + 500.0*Ce*prf[1].dist*(dtot - prf[1].dist) - hrs)/(dtot - prf[1].dist);	// Eq:(18)
        for(int i = 2; i < prf.size()-1; i++){
            double Srim_i = (prf[i].height + 500.0*Ce*prf[i].dist*(dtot - prf[i].dist) - hrs)/(dtot - prf[i].dist);  // Eq:(18)
            Srim = max(Srim, Srim_i);
        }
	    // Calculate the distance of the Bullington point from the transmitter:
	    double Dbp = (hrs - hts + Srim*dtot)/(Stim + Srim);	// Eq:(19)
	    // Calculate the diffraction parameter, nub, for the Bullington point
	    double Nub = (hts + Stim*Dbp - (hts*(dtot - Dbp) + hrs*Dbp)/dtot )*sqrt(0.002*dtot/(lambda*Dbp*(dtot-Dbp)));// Eq:(20)
	    // The knife-edge loss for the Bullington point is given by
		if(Nub > -0.78)
			Luc = 6.9 + 20*log10(sqrt(pow(Nub-0.1, 2)+1)+Nub-0.1);	// Eq:(13), (21)
	}
	// For Luc calculated using either (17) or (21), Bullington diffraction loss for the path is given by
	return Luc+(1-exp(-Luc/6.0))*(10+0.02*dtot);	// Eq:(22)
}
//! --------------------------------------------------------------------------------------------

double CalcLdft_tmp(double epsr, double sigma, double d, double hte, double hre, double adft, double f, Plz plz){
	// Normalized factor for surface admittance for horizontal (1) and vertical (2) polarizations
    double Khv = 0.036*pow((adft*f), (-1.0/3.0))*pow(pow((epsr - 1), 2)+pow((18*sigma/f), 2), (-1.0/4.0));   // Eq:(30a)
	if(plz == Plz::vert)
        Khv = Khv*sqrt(pow(epsr, 2)+pow(18*sigma/f, 2));    // Eq:(30b)
	// Earth ground/polarization parameter
    double Beta_dft = (1+1.6*pow(Khv, 2)+0.67*pow(Khv, 4))/(1+4.5*pow(Khv, 2)+1.53*pow(Khv, 4)); // Eq:(31)
	// Normalized distance
    double X = 21.88*Beta_dft*pow(f/(adft*adft), (1.0/3.0))*d;	// Eq:(32)
	// Normalized transmitter and receiver heights
    double Yt = 0.9575*Beta_dft*pow((f*f/adft), (1.0/3.0))*hte;     // Eq:(33a)
    double Yr = 0.9575*Beta_dft*pow((f*f/adft), (1.0/3.0))*hre;     // Eq:(33b)
	// Calculate the distance term given by:
	double Fx = 0.0;
	if(X >= 1.6)
        Fx = 11+10*log10(X)-17.6*X;
    else
        Fx = -20*log10(X)-5.6488* pow(X, 1.425);	// Eq:(34)
	double Bt = Beta_dft*Yt;	// Eq:(36b)
	double Br = Beta_dft*Yr;	// Eq:(36b)
	double GYt = 0.0;	// Eq:(35)
    if(Bt>2)
        GYt = 17.6*pow(Bt-1.1, 0.5)-5*log10(Bt-1.1)-8;
    else
        GYt = 20*log10(Bt+0.1*pow(Bt, 3));
    
	double GYr = 0.0;	// Eq:(35)
    if(Br>2)
        GYr = 17.6*pow(Br-1.1, 0.5)-5*log10(Br-1.1)-8;
    else
        GYr = 20*log10(Br+0.1*pow(Br, 3));

    double lim = 2 + 20 * log10(Khv);
    GYt = std::max(GYt, lim);
    GYr = std::max(GYr, lim);
	return -Fx-GYt-GYr;	// Eq:(37)
}
//! --------------------------------------------------------------------------------------------

double CalcLdft(double d, double hte, double hre, double adft, double f, double omega, Plz plz){
	// First-term part of the spherical-Earth diffraction loss over land
	double epsr = 22.0;
	double sigma = 0.003;
	double Ldft_land = CalcLdft_tmp(epsr, sigma, d, hte, hre, adft, f, plz);
	// First-term part of the spherical-Earth diffraction loss over sea
	epsr = 80.0;
	sigma = 5.0;
	double Ldft_sea = CalcLdft_tmp(epsr, sigma, d, hte, hre, adft, f, plz);
	// First-term spherical diffraction loss
	return Ldft_sea*omega+Ldft_land*(1-omega); // Eq:(29)
}
//! --------------------------------------------------------------------------------------------

double CalcLdsph(double d, double hte, double hre, double ap, double f, double omega, Plz plz){
	double Ldsph = 0.0;
	// Wavelength in meters speed of light as per ITU-R P.2001
	double lambda = 0.2998/f;
	// Calculate the marginal LoS distance for a smooth path
	double Dlos = sqrt(2*ap)*(sqrt(0.001*hte)+sqrt(0.001*hre));	// Eq:(23)
	if(d >= Dlos){
		// calculate diffraction loss Ldft using the method in Sec. 4.2.2.1 for adft = ap and set Ldsph to Ldft
		Ldsph = CalcLdft(d, hte, hre, ap, f, omega, plz);
	}
	else{
		// calculate the smallest clearance between the curved-Earth path and the ray between the antennas, hse
		double c = (hte - hre)/(hte + hre);											// Eq:(25d)
		double m = 250*d*d/(ap*(hte +hre));											// Eq:(25e)
		double b = 2.0*sqrt((m+1)/(3.0*m))*cos(M_PI/3.0+1.0/3.0*acos(3.0*c/2.0*sqrt(3.0*m/pow(m+1, 3))));	// Eq:(25c)
		double dse1 = d/2*(1+b);													// Eq:(25a)
		double dse2 = d - dse1;														// Eq:(25b)
		double hse = ((hte-500*dse1*dse1/ap)*dse2+(hre-500*dse2*dse2/ap)*dse1)/d;	// Eq:(24)
		// Calculate the required clearance for zero diffraction loss
		double hreq = 17.456*sqrt(dse1*dse2*lambda/d);								// Eq:(26)
	    if(hse > hreq){
	        // If hse > hreq the spherical-Earth diffraction loss Ldsph is zero
	        Ldsph = 0.0;
        }
		else{
			// calculate the modified effective Earth radius aem, which gives marginal LoS at distance d
			double aem = 500*pow(d/(sqrt(hte)+sqrt(hre)), 2);						// Eq:(27)
			// Use the method in Sec. 4.2.2.1 for adft ) aem to obtain Ldft
			double Ldft = CalcLdft(d, hte, hre, aem, f, omega, plz);				// Eq:(29)
			if(Ldft < 0)
				Ldsph = 0;
			else
				Ldsph = Ldft*(1-hse/hreq);											// Eq:(28)
		}
	}
	return Ldsph;
}
//! --------------------------------------------------------------------------------------------

double CalcLd(const vector<TrPoint>& prf, const double dtot,
			  double hts, double hrs, double hstd, double hsrd, double ap, double f, double omega, Plz plz){

	double Lbulla = CalcLbull(prf, dtot, hts, hrs, ap, f);	// Eq:(22)
	// Use the method in 4.2.1 for a second time, with all profile heights hi set to zero and modified antenna heights given by
	double hts1 = hts - hstd;	// Eq:(38a)
	double hrs1 = hrs - hsrd;	// Eq:(38b)
	vector<TrPoint> prf_g0(prf.begin(), prf.end());
	for(int i = 0; i < prf_g0.size(); i++)
		prf_g0[i].height = 0.0;
	// where hstd and hsrd are given in 5.1.6.3 of Attachment 2. Set the resulting Bullington diffraction loss for this smooth path to Lbulls
	double Lbulls = CalcLbull(prf_g0, dtot, hts1, hrs1, ap, f);	// Eq:(22)
	// Use the method in 4.2.2 to calculate the spherical-Earth diffraction loss for the actual path length (dtot) with
	double hte = hts1;	// Eq:(39a)
	double hre = hrs1;	// Eq:(39b)
	double Ldsph = CalcLdsph(dtot, hte, hre, ap, f, omega, plz);
	// Diffraction loss for the general paht is now given by
	return Lbulla + std::max(Ldsph - Lbulls, 0.0);	// Eq:(40)
}
//! --------------------------------------------------------------------------------------------

void GreatCirclePath(double phire, double phite, double phirn, double phitn, double re, double dpnt,
					 double& phipnte, double& phipntn, double& bt2r, double& dgc){
    double Dlon = phire - phite;
    // Calculate quantity r (H.2.2)
    double r = SinD(phitn) * SinD(phirn) + CosD(phitn) * CosD(phirn) * CosD(Dlon);
    // Calculate the path length as the angle subtended at the center of
    // average-radius Earth (H.2.3)
    double Phid = acos(r);  // radians
    // Calculate the great-circle path length (H.2.4)
    dgc = Phid * re;  // km
    // Calculate the quantity x1 (H.2.5a)
    double x1 = SinD(phirn) - r * SinD(phitn);
    // Calculate the quantity y1 (H.2.5b)
    double y1 = CosD(phitn) * CosD(phirn) * SinD(Dlon);
    // Calculate the bearing of the great-circle path for Tx to Rx (H.2.6)
    bt2r = phire;
    if(abs(x1) < 1e-9 && abs(y1) < 1e-9){
        bt2r = phire;
    }else{
        bt2r = Atan2D(y1, x1);
    }
    //// H.3 Calculation of intermediate path point
    // Calculate the distance to the point as the angle subtended at the center
    // of average-radius Earth (H.3.1)
    double Phipnt = dpnt / re;  //radians
    // Calculate quantity s (H.3.2)
    double s = SinD(phitn) * cos(Phipnt) + CosD(phitn) * sin(Phipnt) * CosD(bt2r);
    // The latitude of the intermediate point is now given by (H.3.3)
    phipntn = AsinD(s); // degs
    // Calculate the quantity x2 (H.3.4a)
    double x2 = cos(Phipnt) - s * SinD(phitn);
    // Calculate the quantity y2 (H.3.4b)
    double y2 = CosD(phitn) * sin(Phipnt) * SinD(bt2r);
    // Calculate the longitude of the intermediate point Phipnte (H.3.5)
    phipnte = bt2r;
    if(x2 < 1e-9 && y2 < 1e-9){
        phipnte = bt2r;
    }else{
        phipnte = phite + Atan2D(y2, x2);
    }
}
//! --------------------------------------------------------------------------------------------

double InvCumNorm(double x){
	if(x < 0.000001) x = 0.000001;
	if(x > 0.5) return 0.0; // function is valid for 0.000001 <= x <= 0.5
	double tx = sqrt(-2*log(x));								// Eq:(158a)
	double C0 = 2.515516698;									// Eq:(158c)
	double C1 = 0.802853;										// Eq:(158d)
	double C2 = 0.010328;										// Eq:(158e)
	double D1 = 1.432788;										// Eq:(158f)
	double D2 = 0.189269;										// Eq:(158g)
	double D3 = 0.001308;										// Eq:(158h)
	double ksi = ((C2*tx+C1)*tx+C0)/(((D3*tx+D2)*tx+D1)*tx+1);	// Eq:(158b)
	return ksi - tx;											// Eq:(158)
}
//! --------------------------------------------------------------------------------------------

double LongestContDist(const vector<TrPoint>& prf){
    double dm = 0.0;
    double dmc = 0.0;
    double delta;
    size_t n = prf.size();
	for(int i = 0; i < n; i++){
		if((prf[i].zone == Zone::A1) || (prf[i].zone == Zone::A2)){
			if(i == 0)
				delta = (prf[1].dist - prf[0].dist) / 2.0;
			else{
				if(i == n - 1)
					delta = (prf[n-1].dist-prf[n-2].dist) / 2.0;
				else
					delta = (prf[i+1].dist-prf[i-1].dist) / 2.0;
            }
            dmc = dmc + delta;
            dm = std::max(dm, dmc);
        }
        else
			dmc = 0;
    }
    return dm;
}
double LongestContDist(const vector<TrPoint>& prf, Zone zone_r){
    double dm = 0.0;
    double dmc = 0.0;
    double delta;
    size_t n = prf.size();
    for(int i = 0; i < n; i++){
        if(prf[i].zone == zone_r){
			if(i == 0)
				delta = (prf[1].dist - prf[0].dist) / 2.0;
            else{
				if(i == n - 1)
					delta = (prf[n - 1].dist - prf[n - 2].dist) / 2.0;
				else
					delta = (prf[i + 1].dist - prf[i - 1].dist) / 2.0;
			}
			dmc = dmc + delta;
			dm = std::max(dm, dmc);
		}
		else
			dmc = 0;
	}
    return dm;
}
//! --------------------------------------------------------------------------------------------

double PathFractionSea(const vector<TrPoint>& prf){
    double dm = 0.0;
    size_t n = prf.size();
    double delta;
    for(int i = 0; i < n; i++){
        if(prf[i].zone == Zone::B){
            if(i == 0)
                delta = (prf[1].dist - prf[0].dist) / 2.0;
            else{
				if(i == n - 1)
                    delta = (prf[n-1].dist - prf[n-2].dist) / 2.0;
                else
                    delta = (prf[i+1].dist - prf[i-1].dist) / 2.0;
            }
			dm = dm + delta;
        }
    }
    double omega = dm / (prf[n-1].dist - prf[0].dist);
    if(omega > 1.0) omega = 1.0;
    return omega;
}
//! --------------------------------------------------------------------------------------------

//Gets
//d_tm land
//d_lm inland
//omega water fraction
void GetWaterFraction(const vector<TrPoint>& prf, double distance, double& dct, double& dcr, double& omega ){
	bool found = false;
	size_t n = prf.size();
	int wfrac = 0, wstart = 0, wstop = n - 1;
	for(int i = 1; i < n; i++){
		if(prf[i].zone == Zone::B){
			// Calculation of fraction of total path over water (24a)
			wfrac++;
			// Calculation of hct, hcr
			if(!found){
				found = true;
				wstart = i;
			}
			wstop = i;
		}
	}
	if(found){
		dct = wstart * distance / (n - 1);
		dcr = (n - 1 - wstop) * distance / (n - 1);
	}
	else{
		// set to 500 km. This parameter is important only if it is < 5 km.
		// Therefore if path does not contain water, we set it to high value
		dct = 500;
		dcr = 500;
	}
	// WATER
	omega = wfrac /(double)(n);
	if(omega > 1) omega = 1;
}
//! --------------------------------------------------------------------------------------------

void SmoothEarthHeights(const vector<TrPoint>& prf, const double dtot, double htg, double hrg, double ae, double f,
						double& hst, double& hsr, double& hstd, double& hsrd, double& hte, double& hre, double& hm, double& dlt,
						double& dlr, double& theta_t, double& theta_r, double& theta_tot, PathType& pathtype){
	// Tx and Rx antenna heights above mean sea level amsl (m)
	double hts = prf[0].height + htg;
	double hrs = prf[prf.size()-1].height + hrg;
	// --- Section 5.1.6.2
	double v1 = 0.0;
	double v2 = 0.0;
	for(int i=1; i<prf.size();i++){
		v1 = v1 + (prf[i].dist-prf[i-1].dist)*(prf[i].height+prf[i-1].height);	// Eq:(147)
	    v2 = v2 + (prf[i].dist-prf[i-1].dist)*(prf[i].height*(2*prf[i].dist + prf[i-1].dist)+prf[i-1].height*(prf[i].dist+2*prf[i-1].dist));// Eq:(148)
	}
	hst = (2*v1*dtot-v2)/pow(dtot, 2);	// Eq:(149)
	hsr = (v2-v1*dtot)/pow(dtot, 2);	// Eq:(150)
	// --- Section 5.1.6.3
    double Hobs = prf[1].height - (hts*(dtot - prf[1].dist) + hrs*prf[1].dist)/dtot;  // Eq:(151d)
    double Alpha_obt = Hobs/prf[1].dist;
    double Alpha_obr = Hobs/(dtot - prf[1].dist);
    for(int i = 2; i < prf.size()-1; i++){
        double HH = prf[i].height - (hts*(dtot - prf[i].dist) + hrs*prf[i].dist)/dtot;	// Eq:(151d)
        Hobs = std::max(HH, Hobs);								// Eq:(151a)
        Alpha_obt = std::max(Alpha_obt, HH/prf[i].dist);				// Eq:(151b)
        Alpha_obr = std::max(Alpha_obr, HH/(dtot - prf[i].dist));		// Eq:(151c)
    }
	// Calculate provisional values for the Tx and Rx smooth surface heights
	double gt = Alpha_obt/(Alpha_obt + Alpha_obr);	// Eq:(152e)
	double gr = Alpha_obr/(Alpha_obt + Alpha_obr);	// Eq:(152f)
	double hstp = 0.0;
	double hsrp = 0.0;
	if(Hobs <= 0){
		hstp = hst;	// Eq:(152a)
		hsrp = hsr;	// Eq:(152b)
	}
	else{
		hstp = hst - Hobs*gt;	// Eq:(152c)
		hsrp = hsr - Hobs*gr;	// Eq:(152d)
	}
	// calculate the final values as required by the diffraction model
	if(hstp >= prf[0].height)
		hstd = prf[0].height;			// Eq:(153a)
	else
		hstd = hstp;					// Eq:(153b)
	if(hsrp > prf[prf.size()-1].height)
		hsrd = prf[prf.size()-1].height;// Eq:(153c)
	else
		hsrd = hsrp;			// Eq:(153d)
	// --- Section 4
	// T: Interfering antenna horizon elevation angle and distance
    theta_t = 1000 * atan((prf[1].height - hts)/(1000*prf[1].dist) - prf[1].dist/(2 * ae));	// Eq:(138)
    int lt = 0;
    double theta = 0.0;
    for(int i = 2; i < prf.size()-1; i++){
        theta = 1000*atan((prf[i].height - hts)/(1000*prf[i].dist) - prf[i].dist/(2 * ae));	// Eq:(138)
        if(theta > theta_t){											// Eq:(137)
            theta_t = theta;
            lt = i;
        }
    }
    double theta_td = 1000*atan((hrs - hts)/(1000*dtot) - dtot/(2*ae));  // Eq:(139)
    double theta_rd = 1000*atan((hts - hrs)/(1000*dtot) - dtot/(2*ae));  // Eq:(139)
    pathtype = PathType::los;
    if(theta_t > theta_td) pathtype = PathType::thor; // Eq:(140): test for the trans-horizon path

	// --- Section 5.1.2 Interfering antenna horizon distance, dlt
    dlt = prf[lt].dist;	// Eq:(141)
	// R: Interfered-with antenna horizon elevation angle and distance
    theta_r = 1000*atan((prf[1].height - hrs)/(1000*(dtot - prf[1].dist)) - (dtot - prf[1].dist)/(2*ae)); // Eq:(143)
    int lr = 0;
    for(int i = 2; i < prf.size()-1; i++){
        theta = 1000*atan((prf[i].height - hrs)/(1000*(dtot - prf[i].dist)) - (dtot - prf[i].dist)/(2*ae)); // Eq:(143)
        if(theta > theta_r){   // Eq:(142b)
            theta_r = theta;
            lr = i;
        }
    }
    dlr = dtot - prf[lr].dist; // Eq:(144)
    if(pathtype == PathType::los){
		theta_t = theta_td;		// Eq:(140)
		theta_r = theta_rd;		// Eq:(142a)
		double lambda = 0.2998 / f;
		double Ce = 1 / ae;
		lt = 0;
		double Nu_max = (prf[1].height+500*Ce*prf[1].dist*(dtot-prf[1].dist)-(hts*(dtot-prf[1].dist)+hrs*prf[1].dist)/dtot)*sqrt(0.002*dtot/(lambda*prf[1].dist*(dtot-prf[1].dist))); // Eq:(141a)
	    for(int i = 2; i < prf.size()-1; i++){
            double Nu = (prf[i].height+500*Ce*prf[i].dist*(dtot-prf[i].dist)-(hts*(dtot-prf[i].dist)+hrs*prf[i].dist)/dtot)*sqrt(0.002*dtot/(lambda*prf[i].dist*(dtot-prf[i].dist))); // Eq:(141a)
            if(Nu > Nu_max){
                Nu_max =Nu;
                lt = i;
            }
		}
		dlt = prf[lt].dist;	// Eq:(141)
		dlr = dtot - dlt;	// Eq:(144a)
		lr = 0;
		for(int i = 2; i < prf.size()-1; i++){
            if(dlr < dtot - prf[i].dist) lr = i;
		}
	}

	// --- Section 5.1.5
	// Angular distance
	theta_tot = 1e3 * dtot/ae + theta_t + theta_r;	// Eq:(145)
	// --- Section 5.1.6.4
	// Ducting/layer-reflection model
	// Calculate the smooth-Earth heights at transmitter and receiver as
	// required for the roughness factor
	hst = std::min(hst, prf[0].height);				// Eq:(154a)
	hsr = std::min(hsr, prf[prf.size()-1].height);	// Eq:(154b)
	// Slope of the smooth-Earth surface
	double m = (hsr - hst)/dtot;		// Eq:(155)
	// The terminal effective heigts for the ducting/layer-reflection model
	hte = htg + prf[0].height - hst;				// Eq:(156)
	hre = hrg + prf[prf.size()-1].height - hsr;	// Eq:(156)

    hm = prf[lt].height - (hst + m * prf[lt].dist);
    for(int i = lt + 1; i <= lr; i++){
		hm = std::max(hm, prf[i].height - (hst + m * prf[i].dist));	// Eq:(157)
    }
}
//! --------------------------------------------------------------------------------------------

double CalcLba(double dtot, double dlt, double dlr, double dct, double dcr, double dlm, double hts, double hrs, double hte, double hre, double hm,
		   double theta_t, double theta_r, double f, double p, double temp, double press, double omega, double ae, double b0){
	double alf = 0;
	if(f < 0.5)	alf = 45.375 - 137.0*f + 92.5*f*f;	// Eq:(47a)
	double theta_t1 = theta_t - 0.1*dlt;			// Eq:(48a)
	double theta_r1 = theta_r - 0.1*dlr;			// Eq:(48a)
	double ast = 0;
	double asr = 0;
	if(theta_t1 > 0)
		ast = 20*log10(1 + 0.361*theta_t1*sqrt(f*dlt)) + 0.264*theta_t1*pow(f, 1/3.0);	// Eq:(48)
	if(theta_r1 > 0)
		asr = 20*log10(1 + 0.361*theta_r1*sqrt(f*dlr)) + 0.264*theta_r1*pow(f, 1/3.0);	// Eq:(48)
	double act = 0;
	double acr = 0;
	if(dct <= 5 && dct <= dlt && omega >= 0.75)
	    act = -3*exp(-0.25*dct*dct)*(1+tanh(0.07*(50-hts)));				// Eq:(49)
	if(dcr <= 5 && dcr <= dlr && omega >= 0.75)
        acr = -3*exp(-0.25*dcr*dcr)*(1+tanh(0.07*(50-hrs)));				// Eq:(49)
	double gamma_d = 5e-5 * ae * pow(f, 1/3.0);								// Eq:(51)
	theta_t1 = theta_t;
	theta_r1 = theta_r;
	if(theta_t > 0.1*dlt) theta_t1 = 0.1*dlt;								// Eq:(52a)
	if(theta_r > 0.1*dlr) theta_r1 = 0.1*dlr;								// Eq:(52a)
	double theta1 = 1e3*dtot/ae + theta_t1 + theta_r1;						// Eq:(52)
	double dI = std::min(dtot-dlt-dlr, 40.0);								// Eq:(57)
	double mu3 = 1.0;
	if(hm > 10) mu3 = exp( -4.6e-5 * (hm-10)*(43+6*dI) );					// Eq:(56)
	double tau = 1- exp(-(4.12e-4*pow(dlm, 2.41)));							// Eq:(3a)
	double epsilon = 3.5;
	double alpha = -0.6 - epsilon*1e-9*pow(dtot, 3.1)*tau;					// Eq:(55a)
	if(alpha < -3.4) alpha = -3.4;
	double mu2 = pow(500/ae * pow(dtot, 2)/pow(sqrt(hte) + sqrt(hre), 2), alpha);// Eq:(55)
	if(mu2 > 1) mu2 = 1;
	double beta = b0 * mu2 * mu3;											// Eq:(54)
	double Gamma = (1.076/pow(2.0058-log10(beta), 1.012))*exp(-(9.51-4.8*log10(beta)+0.198*pow(log10(beta),2))*1e-6*pow(dtot, 1.13));// Eq:(53a)
	double ap = -12+(1.2+3.7e-3*dtot)*log10(p/beta)+12*pow(p/beta, Gamma);	// Eq:(53)
	double adp = gamma_d*theta1 + ap;										// Eq:(50)
	double rho = 7.5 + 2.5*omega;											// Eq:(9a)
	double t = temp + 273.15;
	double g_0 = 0.0; double g_w = 0.0;
	SpecAtten_p676_11(f, press, rho, t, g_0, g_w);
	double ag = (g_0 + g_w) * dtot;
	double af = 102.45+20*log10(f)+20*log10(dlt+dlr)+alf+ast+asr+act+acr;	// Eq:(47)
	return af+adp+ag;														// Eq:(46)
}
//! --------------------------------------------------------------------------------------------

double CalcLbs(double dtot, double theta, double f, double p, double temp, double press, double n0, double gt, double gr){
	// Body of function
	double t = temp + 273.15;
	// Frequency dependent loss
	double lf = 25*log10(f)-2.5*pow(log10(f/2), 2);	// Eq:(45a)
	// aperture to medium coupling loss (dB)
	double lc = 0.051*exp(0.055*(gt+gr));			// Eq:(45b)
	// gaseous absorbtion derived from equation Eq:(9) using rho = 3 g/m^3 for the whole path length
	double rho = 3.0;
	// compute specific attenuation due to dry air and water vapor:
	double g_0 = 0.0;
	double g_w = 0.0;
	SpecAtten_p676_11(f, press, rho, t, g_0, g_w);
	double ag = (g_0 + g_w) * dtot;	// Eq:(9)
	// the basic transmission loss due to troposcatter not exceeded for any time
	// percentage p, below 50% is given
	return 190+lf+20*log10(dtot)+0.573*theta-0.15*n0+lc+ag-10.1*pow(-log10(p/50), 0.7);	// Eq:(45)
}
//! --------------------------------------------------------------------------------------------

/*
void CalcLbs_2(const MapDnN0& maps, double f, double dt, double hts, double hrs, double ae, double the, double thetat, double thetar,
		   double phicvn, double phicve, double Gt, double Gr, double p, double hs, double& lbs, double& theta){
	double fMHz = f*1000;	// from GHz in MHz
	// Climatic classification
	// Find average annual sea-level surface refractivity N0 and radio-refractivity lapse rate dN
	// for the common volume of the link in question using the digital maps at phicve (lon),
	// phicvn (lat) - as a bilinear interpolation

    double dn = maps.GetDN50(phicve, phicvn);
    double n0 = maps.GetN050(phicve, phicvn);

	// Calculation of tropocscatter basic transmission loss
	// Step2: Calculate the scatter angle theta
	theta = 1000*the + thetat + thetar;	// mrad Eq:(145)
	// Step 3: Estimate the aperture-to-median coupling loss Lc Eq:(11)
	double lc = 0.07 * exp(0.055* (Gt + Gr));	// dB Eq:(45a)
	// Step 4: Estimate the average annual transmission loss associated with
	// troposcatter not exceeded for p% of time Eq:(45):
	double hb = 7.35;	// km  scale height set to the global mean
	double beta = dt/(2*ae) + thetar/1000 + (hrs-hts)/(1000*dt);		// Eq:(45e)
	double h0 = hts/1000 + dt*sin(beta)/(sin(theta/1000)) *(0.5* dt * sin(beta)/(ae*sin(theta/1000))+sin(thetat/1000));	// Eq:(45d)
	double yp = 0.035*n0*exp(-h0/hb)*pow(-log10(p/50), 0.67);
	if(p >= 50)
		yp = -0.035*n0*exp(-h0/hb)*pow(-log10((100-p)/50), 0.67);
	double ff = 0.18*n0*exp(-hs/hb)-0.23*dn;							// Eq:(45b)
	lbs = ff + 22.0*log10(fMHz)+35.0*log10(theta)+17.0*log10(dt)+lc-yp;	// Eq:(45)
}
*/
//! --------------------------------------------------------------------------------------------

void SpecAtten_p676_11(double f, double p, double rho, double t, double& g_0, double& g_w){
    // T1: spectroscopic data for oxigen
    //				f0        a1    a2     a3   a4     a5     a6
        double  oxigen[44][7] = {
                {50.474214, 0.975, 9.651, 6.690, 0.0, 2.566, 6.850},
                {50.987745, 2.529, 8.653, 7.170, 0.0, 2.246, 6.800},
                {51.503360, 6.193, 7.709, 7.640, 0.0, 1.947, 6.729},
                {52.021429, 14.320, 6.819, 8.110, 0.0, 1.667, 6.640},
                {52.542418, 31.240, 5.983, 8.580, 0.0, 1.388, 6.526},
                {53.066934, 64.290, 5.201, 9.060, 0.0, 1.349, 6.206},
                {53.595775, 124.600, 4.474, 9.550, 0.0, 2.227, 5.085},
                {54.130025, 227.300, 3.800, 9.960, 0.0, 3.170, 3.750},
                {54.671180, 389.700, 3.182, 10.370, 0.0, 3.558, 2.654},
                {55.221384, 627.100, 2.618, 10.890, 0.0, 2.560, 2.952},
                {55.783815, 945.300, 2.109, 11.340, 0.0, -1.172, 6.135},
                {56.264774, 543.400, 0.014, 17.030, 0.0, 3.525, -0.978},
                {56.363399, 1331.800, 1.654, 11.890, 0.0, -2.378, 6.547},
                {56.968211, 1746.600, 1.255, 12.230, 0.0, -3.545, 6.451},
                {57.612486, 2120.100, 0.910, 12.620, 0.0, -5.416, 6.056},
                {58.323877, 2363.700, 0.621, 12.950, 0.0, -1.932, 0.436},
                {58.446588, 1442.100, 0.083, 14.910, 0.0, 6.768, -1.273},
                {59.164204, 2379.900, 0.387, 13.530, 0.0, -6.561, 2.309},
                {59.590983, 2090.700, 0.207, 14.080, 0.0, 6.957, -0.776},
                {60.306056, 2103.400, 0.207, 14.150, 0.0, -6.395, 0.699},
                {60.434778, 2438.000, 0.386, 13.390, 0.0, 6.342, -2.825},
                {61.150562, 2479.500, 0.621, 12.920, 0.0, 1.014, -0.584},
                {61.800158, 2275.900, 0.910, 12.630, 0.0, 5.014, -6.619},
                {62.411220, 1915.400, 1.255, 12.170, 0.0, 3.029, -6.759},
                {62.486253, 1503.000, 0.083, 15.130, 0.0, -4.499, 0.844},
                {62.997984, 1490.200, 1.654, 11.740, 0.0, 1.856, -6.675},
                {63.568526, 1078.000, 2.108, 11.340, 0.0, 0.658, -6.139},
                {64.127775, 728.700, 2.617, 10.880, 0.0, -3.036, -2.895},
                {64.678910, 461.300, 3.181, 10.380, 0.0, -3.968, -2.590},
                {65.224078, 274.000, 3.800, 9.960, 0.0, -3.528, -3.680},
                {65.764779, 153.000, 4.473, 9.550, 0.0, -2.548, -5.002},
                {66.302096, 80.400, 5.200, 9.060, 0.0, -1.660, -6.091},
                {66.836834, 39.800, 5.982, 8.580, 0.0, -1.680, -6.393},
                {67.369601, 18.560, 6.818, 8.110, 0.0, -1.956, -6.475},
                {67.900868, 8.172, 7.708, 7.640, 0.0, -2.216, -6.545},
                {68.431006, 3.397, 8.652, 7.170, 0.0, -2.492, -6.600},
                {68.960312, 1.334, 9.650, 6.690, 0.0, -2.773, -6.650},
                {118.750334, 940.300, 0.010, 16.640, 0.0, -0.439, 0.079},
                {368.498246, 67.400, 0.048, 16.400, 0.0, 0.000, 0.000},
                {424.763020, 637.700, 0.044, 16.400, 0.0, 0.000, 0.000},
                {487.249273, 237.400, 0.049, 16.000, 0.0, 0.000, 0.000},
                {715.392902, 98.100, 0.145, 16.000, 0.0, 0.000, 0.000},
                {773.839490, 572.300, 0.141, 16.200, 0.0, 0.000, 0.000},
                {834.145546, 183.100, 0.145, 14.700, 0.0, 0.000, 0.000}
        };

    // T2: spectroscopic data for water-vapor
    //			   f0		b1	    b2    b3   b4	  b5	b6
        double vapor[35][7] = {
                {22.235080, .1079, 2.144, 26.38, .76, 5.087, 1.00},
                {67.803960, .0011, 8.732, 28.58, .69, 4.930, .82},
                {119.995940, .0007, 8.353, 29.48, .70, 4.780, .79},
                {183.310087, 2.273, .668,  29.06, .77, 5.022, .85},
                {321.225630, .0470, 6.179, 24.04, .67, 4.398, .54},
                {325.152888, 1.514, 1.541, 28.23, .64, 4.893, .74},
                {336.227764, .0010, 9.825, 26.93, .69, 4.740, .61},
                {380.197353, 11.67, 1.048, 28.11, .54, 5.063, .89},
                {390.134508, .0045, 7.347, 21.52, .63, 4.810, .55},
                {437.346667, .0632, 5.048, 18.45, .60, 4.230, .48},
                {439.150807, .9098, 3.595, 20.07, .63, 4.483, .52},
                {443.018343, .1920, 5.048, 15.55, .60, 5.083, .50},
                {448.001085, 10.41, 1.405, 25.64, .66, 5.028, .67},
                {470.888999, .3254, 3.597, 21.34, .66, 4.506, .65},
                {474.689092, 1.260, 2.379, 23.20, .65, 4.804, .64},
                {488.490108, .2529, 2.852, 25.86, .69, 5.201, .72},
                {503.568532, .0372, 6.731, 16.12, .61, 3.980, .43},
                {504.482692, .0124, 6.731, 16.12, .61, 4.010, .45},
                {547.676440, .9785, 0.158, 26.00, .70, 4.500, 1.00},
                {552.020960, .1840, 0.158, 26.00, .70, 4.500, 1.00},
                {556.935985, 497.0, 0.159, 30.86, .69, 4.552, 1.00},
                {620.700807, 5.015, 2.391, 24.38, .71, 4.856, .68},
                {645.766085, .0067, 8.633, 18.00, .60, 4.000, .50},
                {658.005280, .2732, 7.816, 32.10, .69, 4.140, 1.00},
                {752.033113, 243.4, 0.396, 30.86, .68, 4.352, .84},
                {841.051732, .0134, 8.177, 15.90, .33, 5.760, .45},
                {859.965698, .1325, 8.055, 30.60, .68, 4.090, .84},
                {899.303175, .0547, 7.914, 29.85, .68, 4.530, .90},
                {902.611085, .0386, 8.429, 28.65, .70, 5.100, .95},
                {906.205957, .1836, 5.110, 24.08, .70, 4.700, .53},
                {916.171582, 8.400, 1.441, 26.73, .70, 5.150, .78},
                {923.112692, .0079, 10.293, 29.0, .70, 5.000, .80},
                {970.315022, 9.009, 1.919, 25.50, .64, 4.940, .67},
                {987.926764, 134.6, 0.257, 29.85, .68, 4.550, .90},
                {1780.000000, 17506., .952, 196.3, 2.00, 24.15, 5.00}
        };
        
	double theta = 300.0/t;	// Eq:(3)
	double e = rho*t/216.7;	// Eq:(4)
    g_0 = 0;
    double sum = 0.0;
    double d = 5.6e-4 * (p + e) * pow(theta, 0.8);      // Eq:(9)
    double Ndf = f*p*theta*theta*(6.14e-5/(d*(1+pow(f/d, 2)))+ 1.4e-12*p*pow(theta, 1.5)/(1+1.9e-5*pow(f, 1.5))); // Eq:(8)

	// Oxigen computation
    for(int i = 0; i < 44; i++){
        double fi = oxigen[i][0];
        double Si = oxigen[i][1] * 1e-7 * p * pow(theta, 3) * exp(oxigen[i][2] * (1.0 - theta));     // Eq:(3)
        double df = oxigen[i][3] * 1e-4 * (p * pow(theta, (0.8 - oxigen[i][4])) + 1.1 * e * theta);  // Eq:(6a)
        // Doppler broadening
        df = sqrt(df * df + 2.25e-6); // Eq:(6b)
        double delta = (oxigen[i][5] + oxigen[i][6]*theta)*1e-4*(p + e)*pow(theta, 0.8);     // Eq:(7)
        double Fi = f/fi*((df - delta*(fi - f))/(pow(fi - f, 2.0) + df*df) + (df - delta*(fi + f))/(pow(fi + f, 2.0) + df * df)); // Eq:(5)
        // specific attenuation due to dry air (oxygen, pressure induced nitrogen
        // and non-resonant Debye attenuation), Eq:(1-2)
        sum = sum + Si*Fi;
    }
	// specific attenuation due to dry air (oxygen, pressure induced nitrogen and non-resonant Debye attenuation), Eq:(1-2)
    g_0 = 0.182 * f * (sum + Ndf);	// Eq:(22)

	// vapor computation
	sum = 0.0;
    for(int i = 0; i < 35; i++){
        double fi = vapor[i][0];
        double Si = vapor[i][1] * 1e-1 * e * pow(theta, 3.5) * exp(vapor[i][2] * (1.0 - theta));					// Eq:(3)
        double df = vapor[i][3] * 1e-4 * (p * pow(theta, vapor[i][4]) + vapor[i][5] * e * pow(theta, vapor[i][6])); // Eq:(6a)
        // doppler broadening
        df = 0.535 * df + sqrt(0.217 * df * df + 2.1316e-12 * fi * fi / theta); // Eq:(6b)
        double delta = 0;                                                           // equation (7)
        double Fi = f/fi*((df-delta*(fi-f))/(pow(fi-f, 2.0)+df*df)+(df-delta*(fi+f))/(pow(fi+f, 2.0)+df*df));       // Eq:(5)
        sum = sum + Si * Fi;
    }
	// specific attenuation due to water vapour, Eq:(1-2)
    g_w = 0.182 * f * sum;	// Eq:(23)
}
//! --------------------------------------------------------------------------------------------


// Use the method in 4.2.3 to calculate diffractino loss Ld for effective
// Earth radius ap = ae as given by equation (6a). Set median diffractino loss to Ldp50
void CalcDlp(const vector<TrPoint>& prf, const double dtot, double hts, double hrs, double hstd, double hsrd,
			 double f, double omega, double p, double b0, double DN, Plz plz,
			 double& Ldp, double& Ld50){
			     
	Ldp = 0.0;
	Ld50 = 0.0;
	double ae = 0.0;
	double ab = 0.0;
	EarthRadEff(DN, ae, ab);
	Ld50 = CalcLd(prf, dtot, hts, hrs, hstd, hsrd, ae, f, omega, plz);
	if(p == 50.0){
		Ldp = Ld50;
		return;
	}
	if(p < 50.0){
	    // Use the method in 4.2.3 to calculate diffraction loss Ld for effective
		// Earth radius ap = abeta, as given in equation (6b). Set diffraction loss
		// not exceeded for beta0% time Ldb = Ld
        double Ldb = CalcLd(prf, dtot, hts, hrs, hstd, hsrd, ab, f, omega, plz);
		// Compute the interpolation factor Fi
		double Fi;
	    if(p > b0)
	        Fi = InvCumNorm(p/100)/InvCumNorm(b0/100);	// eq (41a)
	    else
	        Fi = 1;
		// The diffraction loss Ldp not exceeded for p% of time is now given by
	    Ldp = Ld50 + (Ldb-Ld50)*Fi;	// eq (42)
	}
}
//! --------------------------------------------------------------------------------------------

/*
Out452 CalcLoss(const MapDnN0& maps, const vector<TrPoint>& prf, const In452& in){
	Out452 out;
	if(prf.size() < 2){
		out.error = 1011; // "Tx elevation (9B) is not present";
		return out;
	}
	// Compute the path profile parameters
	// Compute  dtm     -   the longest continuous land (inland + coastal) section of the great-circle path (km)
	double dtm = LongestContDist(prf);	// for zone = (A1, A2)	 Eq:(3a)
	out.d_tm = dtm;
	// Compute  dlm     -   the longest continuous inland section of the great-circle path (km)
	double dlm = LongestContDist(prf, Zone::A2);	// Eq:(3a)
	out.d_lm = dlm;
	// Calculate the longitude and latitude of the mid-point of the path, phimLong, and phimLat for dpnt = 0.5dt
	double Re = 6371;
	double dtot = prf[prf.size()-1].dist-prf[0].dist;
	double dpnt = 0.5*dtot;
	double phimLong, phimLat, bt2r, dgc;
	GreatCirclePath(in.rLong, in.tLong, in.rLat, in.tLat, Re, dpnt, phimLong, phimLat, bt2r, dgc);
	// Find radio-refractivity lapse rate dN using the digital maps at phim_e (lon), phim_n (lat) - as a bilinear interpolation

	double dn = in.DN;
    double n0 = in.N0;
	// Рассчитать dn и n0, если они не были введены в ручную
	if(dn == 0.0) dn = maps.GetDN50(phimLong, phimLat);
    if(n0 == 0.0) n0 = maps.GetN050(phimLong, phimLat);
	out.DN = dn;
	out.N0 = n0;

	double b0 = CalcBeta0(phimLat, dtm, dlm);
	out.b0 = b0;
	double ae = 0.0;
	double ab = 0.0;
	EarthRadEff(dn, ae, ab);
	// Compute the path fraction over see
	double omega, dct, dcr; // omega = PathFractionSea(prf);
	GetWaterFraction(prf, dtot, dct, dcr, omega);
	out.omega = omega;
	out.d_cr = dcr;
	out.d_ct = dct;
	double hst, hsr, hstd, hsrd, hte, hre, hm, dlt, dlr, theta_t, theta_r, theta;
	PathType pathtype;
	SmoothEarthHeights(prf, dtot, in.htg, in.hrg, ae, in.f, hst, hsr, hstd, hsrd, hte, hre, hm, dlt, dlr, theta_t, theta_r, theta, pathtype);
	out.h_te = hte;
	out.h_re = hre;
	out.h_m = hm;
	out.d_lt = dlt;
	out.d_lr = dlr;
	out.theta_t = theta_t;
	out.theta_r = theta_r;
	out.theta = theta;
	out.pathType = pathtype;
	out.ae = ae;
	out.dtot = dtot;
	out.hstd = hstd;
	out.hsrd = hsrd;
	// Returns TX elevation [mrad]
	if(pathtype == PathType::los)
		out.elevation_tx  = (in.hrg + prf[prf.size()-1].height) - (in.htg + prf[0].height) / dtot - dtot * 1000 / (2 * ae);
	else
		out.elevation_tx = theta_t;
	// Returns RX elevation [mrad]
	if(pathtype == PathType::los)
		out.elevation_rx  = (in.htg + prf[0].height) - (in.hrg + prf[prf.size()-1].height) / dtot - dtot * 1000 / (2 * ae);
	else
		out.elevation_rx  = theta_r;
	// Tx and Rx antenna heights above mean sea level amsl (m)
    double hts = prf[0].height + in.htg;
    double hrs = prf[prf.size()-1].height + in.hrg;
	out.hts = hts;
	out.hrs = hrs;
	// Effective Earth curvature Ce (km^-1)
	double Ce = 1/ae;
	// Find the intermediate profile point with the highest slope of the line
	// from the transmitter to the point

	// Section 4.5: The overall prediction
    double Stim = ((prf[1].height + 500 * Ce * prf[1].dist * (dtot - prf[1].dist) - hts) / prf[1].dist);
	for(int i = 2; i < prf.size()-1; i++){
        Stim = std::max(Stim, (prf[i].height + 500 * Ce * prf[i].dist * (dtot - prf[i].dist) - hts) / prf[i].dist); // Eq:(14)
    }
	// Calculate the slope of the line from transmitter to receiver assuming a LoS path
	double Str = (hrs - hts)/dtot;	// Eq (15)
	// Calculate an interpolation factor Fj to take account of the path angular distance (58)
	double THETA = 0.3;
	double KSI = 0.8;
	double Fj = 1.0 - 0.5*( 1.0 + tanh(3.0 * KSI * (Stim-Str)/THETA) ); // eq (58)
	// Calculate an interpolation factor, Fk, to take account of the great circle path distance:
	double dsw = 20.0;
	double kappa = 0.5;
	double Fk = 1.0 - 0.5*( 1.0 + tanh(3.0 * kappa * (dtot-dsw)/dsw) );	// eq (59)
	// modified with 3-D path for free-space computation,
	double d3D = sqrt(dtot*dtot + pow((hts-hrs)/1000.0, 2));
	double Lbfsg, Lb0p, Lb0b, Ag;
	LineOfSingPropg(d3D, in.f, in.p, b0, omega, in.temp, in.press, dlt, dlr, Lbfsg, Lb0p, Lb0b, Ag);
	out.Ag = Ag;
	out.Lbfsg = Lbfsg;
	out.Lb0p = Lb0p;
	out.Lb0beta = Lb0b;
	double Ldp = 0.0;
	double Ld50 = 0.0;
	CalcDlp(prf, dtot, hts, hrs, hstd, hsrd, in.f, omega, in.p, b0, dn, in.plz, Ldp, Ld50);
	out.Ld50 = Ld50;
	out.Ldsph = NO_DATA;

	out.Ldp = Ldp;
	// The median basic transmission loss associated with diffraction Eq (43)
	double Lbd50 = Ld50 + Lbfsg;
	out.Lbd50 = Lbd50;
	// The basic tranmission loss associated with diffraction not exceeded for p% time Eq (44)
	double Lbd = Ldp + Lb0p;
	out.Lbd = Lbd;

	// A notional minimum basic transmission loss associated with LoS propagation and over-sea sub-path diffraction
	double Lminb0p = Ldp*(1-omega) + Lb0p;
	double Fi = 0.0;
	if(in.p >= b0){
		Fi = InvCumNorm(in.p/100)/InvCumNorm(b0/100);			// eq (41a)
		Lminb0p = Lbd50 + (Ldp*(1-omega) + Lb0b - Lbd50)*Fi;	// eq (60)
	}
	// Calculate a notional minimum basic transmission loss associated with LoS and transhorizon signal enhancements
	double eta = 2.5;
	double Lba = CalcLba(dtot, dlt, dlr, dct, dcr, dlm, hts, hrs, hte, hre, hm, theta_t, theta_r, in.f, in.p, in.temp, in.press, omega, ae, b0);
	out.Lba = Lba;
	double Lminbap = eta*log(exp(Lba/eta) + exp(Lb0p/eta));	// eq (61)
	// Calculate a notional basic transmission loss associated with diffraction and LoS or ducting/layer reflection enhancements
	double Lbda = Lbd;
	if(Lminbap <= Lbd) Lbda = Lminbap + (Lbd-Lminbap)*Fk;
	// Calculate a modified basic transmission loss, which takes diffraction and
	// LoS or ducting/layer-reflection enhancements into account
	double Lbam = Lbda + (Lminb0p - Lbda)*Fj;	// eq (63)
	// Calculate the basic transmission loss due to troposcatter not exceeded for any time percantage p
	double Lbs = CalcLbs(dtot, theta, in.f, in.p, in.temp, in.press, n0, in.Gt, in.Gr);
	out.Lbs = Lbs;
	// Calculate the final transmission loss not exceeded for p% time
	out.Lb = -5*log10(pow(10, (-0.2*Lbs)) + pow(10, (-0.2*Lbam))); // eq (64)
	return out;
}
*/
//! --------------------------------------------------------------------------------------------



}	// namespace R452_18