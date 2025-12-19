/*
 * HydroDynamics.h
 *
 *  Created on: 15 November 2022
 *      Author: massimo
 */
#ifndef HYDRODYNAMICS_H_
#define HYDRODYNAMICS_H_

#include "TimeStampedData.h"

//
// drag coefficients are assumed to be negative
// angular rates are in [rad/s]
//
// mu*urDot = + ku*ur + kuu*ur|ur| + kvr*vr*r + X
// mv*vrDot = + kv*vr + kvv*vr|vr| + kur*ur*r + Y
// mw*wrDot = + kw*wr + kww*wr|wr| + Z
//
// Ir*rDot = + kr*r + krr*r|r| + kru*r*|u| + N
// or
// Ir*rDot = + kr*r + krr*r|r| + krX*r*sqrt(|X|) + N

class HydroDynamics : public TimeStampedData {
public:
	// mass, included added mass
	double mu; // [Kg]
	double mv; // [Kg]
	double mw; // [Kg]
	// inertia, included added inertia
	double Ip; // [Kg m]
	double Iq; // [Kg m]
	double Ir; // [Kg m]
	// uncoupled drag coefficients
	double ku, kuu;
	double kv, kvv;
	double kw, kww;
	double kp, kpp;
	double kq, kqq;
	double kr, krr;
	// coupled drag coefficients
	double kvr;
	double kur;
	double kru;
	// coupled force angular rate coefficients
	double krX;

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
