/*
 * HydroDynamicsCntrl.h
 *
 *  Created on: 2 August 2024
 *      Author: massimo
 */
#ifndef HYDRODYNAMICSCNTRL_H_
#define HYDRODYNAMICSCNTRL_H_

#include "TimeStampedData.h"

#define HYDRODYNAMICSCNTRL_CODE "HYDROCNTRL"

//
// drag coefficients are assumed to be negative
// angular rates are in [rad/s]
//
// parameters:
//   mu mv Ir mxg Xu Xuu Yv Yvv Nr Nrr
//
// mu*   urDot               = + Xu* ur + Xuu* ur|ur| + X
// mv*   vrDot = + Yv* vr + Yvv* vr|vr|               + Y
// Ir*   rDot  = + Nr* r  + Nrr* r|r|   - mxg* ur*r   + N
//

class HydroDynamicsCntrl : public TimeStampedData {
public:
	// mass/inertia, included added mass/inertia
	double mu;
	double mv;
	double Ir;
	// uncoupled drag coefficients
	double Xu, Xuu;
	double Yv, Yvv;
	double Nr, Nrr;
	// effect of center of mass longitudinal offset
	double mxg;

public:
	HydroDynamicsCntrl(string name);
	HydroDynamicsCntrl(void);
    ~HydroDynamicsCntrl(void) {};

    HydroDynamicsCntrl & operator = (const HydroDynamicsCntrl &obj);

    bool fread(string fileName);

    //void fprint(FILE *f);
    int sprint(char *s);
    void sread(char *s);
private:
	void reset(void);
 };

#endif /* HYDRODYNAMICSCNTRL_H_ */
