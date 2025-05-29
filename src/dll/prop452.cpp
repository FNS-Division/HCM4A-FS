
#include "prop452.h"


TProp452_18::TProp452_18(const MapDnN0& mapDnN0, const vector<TrPoint>& prf):mapDnN0(mapDnN0),  prf(prf){
}
//! --------------------------------------------------------------------------------------------


bool TProp452_18::Init(double f, double tLong, double tLat, double rLong, double rLat, double htg, double hrg, double DN, double N0){
	if(prf.size() < 2){
		// TODO return ErrorCode
		// error('CalcLoss: path profile requires at least 4 points.');
		// DCout("ProfileAnalysis::error", "ProfileAnalysis: path profile requires at least 2 points.");
		return isCalcEnable = false;
	}

	// Compute the path profile parameters
	// Compute  dtm -   the longest continuous land (inland + coastal) section of the great-circle path (km)
	calcRes.d_tm = LongestContDist(prf);	// for zone = (A1, A2)	 Eq:(3a)
	// Compute  dlm -   the longest continuous inland section of the great-circle path (km)
	calcRes.d_lm = LongestContDist(prf, Zone::A2);	// Eq:(3a)
	// Calculate the longitude and latitude of the mid-point of the path, phimLong, and phimLat for dpnt = 0.5dt
	double Re = 6371;
	calcRes.dtot = prf[prf.size()-1].dist-prf[0].dist;
	double dpnt = 0.5*calcRes.dtot;
	double phimLong, phimLat, bt2r, dgc;
	GreatCirclePath(rLong, tLong, rLat, tLat, Re, dpnt, phimLong, phimLat, bt2r, dgc);
	// Find radio-refractivity lapse rate dN using the digital maps at phim_e (lon), phim_n (lat) - as a bilinear interpolation
	
	// Рассчитать dn и n0, если они не были введены в ручную
	if(DN != 0.0)
		calcRes.DN = DN;
	else
	    calcRes.DN = mapDnN0.GetDN50(phimLong, phimLat);
	if(N0 != 0.0)
		calcRes.N0 = N0;
	else
	   calcRes.N0 = mapDnN0.GetN050(phimLong, phimLat);

	// Compute b0
	calcRes.b0 = CalcBeta0(phimLat, calcRes.d_tm, calcRes.d_lm);
	EarthRadEff(calcRes.DN, calcRes.ae, calcRes.ab);
	// Compute the path fraction over see
	// omega = PathFractionSea(prf);
	GetWaterFraction(prf, calcRes.dtot, calcRes.d_ct, calcRes.d_cr, calcRes.omega);
	SmoothEarthHeights(prf, calcRes.dtot, htg, hrg, calcRes.ae, f,
					   calcRes.h_st, calcRes.h_sr, calcRes.hstd, calcRes.hsrd, calcRes.h_te, calcRes.h_re, calcRes.h_m,
					   calcRes.d_lt, calcRes.d_lr, calcRes.theta_t, calcRes.theta_r, calcRes.theta, calcRes.pathType);
	// Returns TX elevation [mrad]
	if(calcRes.pathType == PathType::los){
		calcRes.elevation_tx  = ((hrg + prf[prf.size()-1].height)-(htg + prf[0].height)) / calcRes.dtot - calcRes.dtot * 1000 / (2 * calcRes.ae);
	}
	else{
		calcRes.elevation_tx = calcRes.theta_t;
	}
	// Returns RX elevation [mrad]
	if(calcRes.pathType == PathType::los)
		calcRes.elevation_rx  = ((htg + prf[0].height)-(hrg + prf[prf.size()-1].height)) / calcRes.dtot - calcRes.dtot * 1000 / (2 * calcRes.ae);
	else
		calcRes.elevation_rx  = calcRes.theta_r;
	return isCalcEnable = true;
}
//! --------------------------------------------------------------------------------------------
double TProp452_18::TxElevation(){
	return calcRes.elevation_tx;
}
double TProp452_18::RxElevation(){
	return calcRes.elevation_rx;
}
//! --------------------------------------------------------------------------------------------

Out452 TProp452_18::CalcLoss(const In452& inRec){
	if(!isCalcEnable){
		// DCout("CalcLoss::error", "CalcLoss: path profile requires at least 2 points.");
		return calcRes;
	}
	// Tx and Rx antenna heights above mean sea level amsl (m)
    calcRes.hts = prf[0].height + inRec.htg;
    calcRes.hrs = prf[prf.size()-1].height + inRec.hrg;
	// Effective Earth curvature Ce (km^-1)
	double Ce = 1/calcRes.ae;
	// Find the intermediate profile point with the highest slope of the line
	// from the transmitter to the point

	// Section 4.5: The overall prediction
    double Stim = ((prf[1].height + 500 * Ce * prf[1].dist * (calcRes.dtot - prf[1].dist) - calcRes.hts) / prf[1].dist);
	for(int i = 2; i < prf.size()-1; i++){
        Stim = std::max(Stim, (prf[i].height + 500 * Ce * prf[i].dist * (calcRes.dtot - prf[i].dist) - calcRes.hts) / prf[i].dist); // Eq:(14)
    }
	// Calculate the slope of the line from transmitter to receiver assuming a LoS path
	double Str = (calcRes.hrs - calcRes.hts)/calcRes.dtot;	// Eq (15)
	// Calculate an interpolation factor Fj to take account of the path angular distance (58)
	double THETA = 0.3;
	double KSI = 0.8;
	double Fj = 1.0 - 0.5*( 1.0 + tanh(3.0 * KSI * (Stim-Str)/THETA) ); // eq (58)
	// Calculate an interpolation factor, Fk, to take account of the great circle path distance:
	double dsw = 20.0;
	double kappa = 0.5;
	double Fk = 1.0 - 0.5*( 1.0 + tanh(3.0 * kappa * (calcRes.dtot-dsw)/dsw) );	// eq (59)
	// modified with 3-D path for free-space computation,
	double d3D = sqrt(calcRes.dtot*calcRes.dtot + pow((calcRes.hts-calcRes.hrs)/1000.0, 2));
	//double Lbfsg, Lb0p, Lb0b, Ag;
	LineOfSingPropg(d3D, inRec.f, inRec.p, calcRes.b0, calcRes.omega, inRec.temp, inRec.press, calcRes.d_lt, calcRes.d_lr,
					calcRes.Lbfsg, calcRes.Lb0p, calcRes.Lb0beta, calcRes.Ag);
	CalcDlp(prf, calcRes.dtot, calcRes.hts, calcRes.hrs, calcRes.hstd, calcRes.hsrd, inRec.f, calcRes.omega, inRec.p,
			calcRes.b0, calcRes.DN, inRec.plz, calcRes.Ldp, calcRes.Ld50);
	// The median basic transmission loss associated with diffraction Eq (43)
	calcRes.Lbd50 = calcRes.Ld50 + calcRes.Lbfsg;
	// The basic tranmission loss associated with diffraction not exceeded for p% time Eq (44)
	calcRes.Lbd = calcRes.Ldp + calcRes.Lb0p;
	// A notional minimum basic transmission loss associated with LoS propagation and over-sea sub-path diffraction
	double Lminb0p = calcRes.Ldp*(1-calcRes.omega) + calcRes.Lb0p;
	double Fi = 0.0;
	if(inRec.p >= calcRes.b0){
		Fi = InvCumNorm(inRec.p/100)/InvCumNorm(calcRes.b0/100);			// eq (41a)
		Lminb0p = calcRes.Lbd50 + (calcRes.Ldp*(1-calcRes.omega) + calcRes.Lb0beta - calcRes.Lbd50)*Fi;	// eq (60)
	}
	// Calculate a notional minimum basic transmission loss associated with LoS and transhorizon signal enhancements
	double eta = 2.5;
	calcRes.Lba = CalcLba(calcRes.dtot, calcRes.d_lt, calcRes.d_lr, calcRes.d_ct, calcRes.d_cr, calcRes.d_lm,
						  calcRes.hts, calcRes.hrs, calcRes.h_te, calcRes.h_re, calcRes.h_m, calcRes.theta_t, calcRes.theta_r,
						  inRec.f, inRec.p, inRec.temp, inRec.press, calcRes.omega, calcRes.ae, calcRes.b0);
	double Lminbap = eta*log(exp(calcRes.Lba/eta) + exp(calcRes.Lb0p/eta));	// eq (61)
	// Calculate a notional basic transmission loss associated with diffraction and LoS or ducting/layer reflection enhancements
	double Lbda = calcRes.Lbd;
	if(Lminbap <= calcRes.Lbd) Lbda = Lminbap + (calcRes.Lbd-Lminbap)*Fk;
	// Calculate a modified basic transmission loss, which takes diffraction and
	// LoS or ducting/layer-reflection enhancements into account
	double Lbam = Lbda + (Lminb0p - Lbda)*Fj;	// eq (63)
	// Calculate the basic transmission loss due to troposcatter not exceeded for any time percantage p
	calcRes.Lbs = CalcLbs(calcRes.dtot, calcRes.theta, inRec.f, inRec.p, inRec.temp, inRec.press, calcRes.N0, inRec.Gt, inRec.Gr);
	// Calculate the final transmission loss not exceeded for p% time
	calcRes.Lb = -5*log10(pow(10, (-0.2*calcRes.Lbs)) + pow(10, (-0.2*Lbam))); // eq (64)
	return calcRes;
}
//! --------------------------------------------------------------------------------------------
