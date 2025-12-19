/*
 * nVector.cpp
 *
 *  Created on: Jan 3, 2023
 *      Author: Massimo Caccia
 *
 *      This code is based on the following:
 *      The *n*-vector library is based on the following article, and hence it should be cited in publications using the library:
 *      > Kenneth Gade (2010): A Non-singular Horizontal Position Representation, *The Journal of Navigation*,
 *      Volume 63, Issue 03, pp 395-417, July 2010,
 *      [DOI: 10.1017/S0373463309990415](https://doi.org/10.1017/S0373463309990415).<br/>
 *      *<https://www.navlab.net/Publications/A_Nonsingular_Horizontal_Position_Representation.pdf>*
 */

#include "nVector.h"


double deg2rad(double alpha) { return alpha*M_PI/180.; }
double rad2deg(double alpha) { return alpha*180./M_PI; }

//
// compute n-vector nE corresponding to (lat,lon) coordinates in degrees
//
void lat_lon_2_nE(Vector &nE,double lat,double lon) {
	double lambda=deg2rad(lat);
	double mu=deg2rad(lon);

	nE[0]=sin(lambda);
	nE[1]=sin(mu)*cos(lambda);
	nE[2]=-cos(mu)*cos(lambda);
}

//
// computes (lat,lon) coordinates in degrees corresponding to n-vector nE
//
void nE_2_lat_lon(double &lat,double &lon,Vector nE) {
	double lambda=atan2(nE[0],sqrt(nE[1]*nE[1]+nE[2]*nE[2]));
	double mu=atan2(nE[1],-nE[2]);

	lat=rad2deg(lambda);
	lon=rad2deg(mu);
}

//
// compute horizontal North and East directions
//
void nE_2_North_East(Vector &kNorth,Vector &kEast,Vector nE) {
	Vector one(1.,0.,0.);

	kEast=one%nE;
	kNorth=nE%kEast;
}

//
// From n-vector nE,h to position vector pEB - B.1 (22)
//
void nE_h_2_pEB_WGS84(Vector &pEB,Vector nE,double h) {
	double bWGS84=aWGS84*(1-fWGS84);
	double a2b2=(aWGS84*aWGS84)/(bWGS84*bWGS84);
	double den=sqrt(nE[0]*nE[0]+a2b2*nE[1]*nE[1]+a2b2*nE[2]*nE[2]);
	double c=bWGS84/den;

	Vector v(nE[0],a2b2*nE[1],a2b2*nE[2]);
	pEB=v*c+nE*h;
}

//
// From position vector pEB to n-vector nE,h - B.2 (23)
//
void pEB_2_nE_h_WGS84(Vector &nE,double &h,Vector pEB) {
	double bWGS84=aWGS84*(1-fWGS84);
	double a2=aWGS84*aWGS84;
	double b2a2=(bWGS84*bWGS84)/a2;
	double e=sqrt(1-b2a2);
	double q=((1-e*e)/a2)*pEB[0]*pEB[0];
	double p=(pEB[1]*pEB[1]+pEB[2]*pEB[2])/a2;
	double e4=e*e*e*e;
	double r=(p+q-e4)/6;
	double s=(e4*p*q)/(4*r*r*r);
	double t0=1+s+sqrt(s*(2+s));
	double t=pow(t0,1./3);
	double u=r*(1+t+1/t);
	double v=sqrt(u*u+e4*q);
	double w=e*e*(u+v-q)/(2*v);
	double k=sqrt(u+v+w*w)-w;
	double d=k*sqrt(pEB[1]*pEB[1]+pEB[2]*pEB[2])/(k+e*e);
	double factor=sqrt(d*d+pEB[0]*pEB[0]);

	h=((k+e*e-1)/k)*factor;
	nE[0]=pEB[0]/factor;
	nE[1]=(k*pEB[1]/(k+e*e))/factor;
	nE[2]=(k*pEB[2]/(k+e*e))/factor;
}

//
// rotation matrix relating the N and E frames
//
void nE_2_REN(Matrix3x3 &REN,Vector nE) {
	Vector kNorth;
	Vector kEast;
	nE_2_North_East(kNorth,kEast,nE);
	double kN=kNorth.norm();
	double kE=kEast.norm();
	REN(0,0)=kNorth[0]/kN; REN(0,1)=kEast[0]/kE; REN(0,2)=-nE[0];
	REN(1,0)=kNorth[1]/kN; REN(1,1)=kEast[1]/kE; REN(1,2)=-nE[1];
	REN(2,0)=kNorth[2]/kN; REN(2,1)=kEast[2]/kE; REN(2,2)=-nE[2];
}

//
// traslation of (x,y,0.)
//
void lat_lon_plus_x_y(double &lat,double &lon,double lat0,double lon0,double x,double y) {
    Vector nE;
    lat_lon_2_nE(nE,lat0,lon0);
    Vector pEN; //
    nE_h_2_pEB_WGS84(pEN,nE,0.);
    Matrix3x3 REN;
    nE_2_REN(REN,nE);
    Vector delta(x,y,0.);
    Vector pEB=pEN+REN*delta;
    Vector nB;
    double h;
    pEB_2_nE_h_WGS84(nB,h,pEB);
    nE_2_lat_lon(lat,lon,nB);
}

//
// compute (x,y) of (lat1,lon1) with respect a NED frame in (lat0,lon0)
//
void lat_lon_2_x_y(double &x,double &y,double lat0,double lon0,double lat1,double lon1) {
	Vector nE0,nE1;
	Vector pE0,pE1;
	Vector dp;
	Matrix3x3 REN;

	lat_lon_2_nE(nE0,lat0,lon0);
	lat_lon_2_nE(nE1,lat1,lon1);
	nE_h_2_pEB_WGS84(pE0,nE0,0.);
	nE_h_2_pEB_WGS84(pE1,nE1,0.);
	dp=pE1-pE0;
	nE_2_REN(REN,nE0);
	Vector dpE0;
	dpE0=REN.t()*dp;
	x=dpE0[0];
	y=dpE0[1];
//	printf("lat_lon_2_x_y(double &x,double &y,double lat0,double lon0,double lat1,double lon1)\n");
//	dpE0.print();
}

//
// compute yaw-pitch-roll transformation matrix
//
void phi_theta_psi_2_Rzyx(Matrix3x3 &Rzyx,double phiDeg,double thetaDeg,double psiDeg) {
	double phi=deg2rad(phiDeg);
	double theta=deg2rad(thetaDeg);
	double psi=deg2rad(psiDeg);
	double cphi=cos(phi);
	double sphi=sin(phi);
	double ctheta=cos(theta);
	double stheta=sin(theta);
	double cpsi=cos(psi);
	double spsi=sin(psi);

	Rzyx(0,0)=cpsi*ctheta; Rzyx(0,1)=-spsi*cphi+cpsi*stheta*sphi; Rzyx(0,2)=spsi*sphi+cpsi*cphi*stheta;
	Rzyx(1,0)=spsi*ctheta; Rzyx(1,1)=cpsi*cphi+sphi*stheta*spsi;  Rzyx(1,2)=-cpsi*sphi+stheta*spsi*cphi;
	Rzyx(2,0)=-stheta;     Rzyx(2,1)=ctheta*sphi;                 Rzyx(2,2)=ctheta*cphi;
}

//
// compute yaw-pitch-roll from rotation matrix
//
void Rzyx_2_phi_theta_psi(double &phi,double &theta,double &psi,Matrix3x3 Rzyx) {
	// *this is assumed to be a Rotation matrix
	//
	// this procedure implements the pseudo-code given in
	// Computing Euler angles from a rotation matrix by Gregory G. Slabaugh
	//
	if(fabs(Rzyx(2,0))!=1.) {
		theta = -asin(Rzyx(2,0));
		phi=atan2(Rzyx(2,1)/cos(theta),Rzyx(2,2)/cos(theta));
		psi=atan2(Rzyx(1,0)/cos(theta),Rzyx(0,0)/cos(theta));
	}
	else {
		psi=0.;
		if(Rzyx(2,0)==-1.) {
			theta=M_PI/2.;
			phi=psi+atan2(Rzyx(0,1),Rzyx(0,2));
		}
		else {
			theta=-M_PI/2.;
			phi=-psi+atan2(-Rzyx(0,1),-Rzyx(0,2));
		}
	}
}

