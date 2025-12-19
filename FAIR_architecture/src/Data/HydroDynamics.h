/*
 * HydroDynamics.h
 *
 *  Created on: 15 November 2022
 *      Author: massimo
 */
#ifndef HYDRODYNAMICS_H_
#define HYDRODYNAMICS_H_

#include "TimeStampedData.h"
#include "Matrix.h"

#define HYDRODYNAMICS_CODE "HYDRO"

//
// drag coefficients are assumed to be negative
// angular rates are in [rad/s]
//
// parameters:
//   m Ir XuDot YvDot NrDot xg Xu Xuu Yv Yvv Nr Nrr
//
// mu = m - XuDot
// mv = m - YvDot
// Ir = Iz - NrDot
//
// mu*   urDot               = + Xu* ur + Xuu* ur|ur| + mv*   vr*r + m*xg*    r^2   + X
// mv*   vrDot  + m*xg* rDot = + Yv* vr + Yvv* vr|vr| - mu*   ur*r                  + Y
// m*xg* vrDot  + Ir*   rDot = + Nr* r  + Nrr* r|r|   - m*xg* ur*r + (mu-mv)* ur*vr + N
//
//
//

class HydroDynamics : public TimeStampedData {
public:
	// mass and added mass
	double m;     // [Kg]
	double XuDot; // [Kg]
	double YvDot; // [Kg]
	// inertia and added inertia
	double Iz;    // [Kg m^2]
	double NrDot;    // [Kg m^2]
	// uncoupled drag coefficients
	double Xu, Xuu;
	double Yv, Yvv;
	double Nr, Nrr;
	// xg : center of mass longitudinal offset
	double xg;
	//
	// Coefficients for normalised models
	//
	// kr2 : square(yawrate) induced surge force coefficient
	double kr2;
	// kur : surge-yawrate interaction coefficient
	double kur;
	// kXu : surge force reduction induced by speed coefficient
	double kXu;

public:
	// mass/inertia, included added mass/inertia
	double mu;
	double mv;
	double Ir;

public:
	HydroDynamics(string name);
	HydroDynamics(void);
    ~HydroDynamics(void) {};

    HydroDynamics & operator = (const HydroDynamics &obj);

    bool fread(string fileName);

    //void fprint(FILE *f);
    int sprint(char *s);
    void sread(char *s);
private:
	void reset(void);
 };

#endif /* HYDRODYNAMICS_H_ */
