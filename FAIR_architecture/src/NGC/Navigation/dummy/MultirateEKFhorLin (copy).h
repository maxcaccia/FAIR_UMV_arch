/*
 * MultirateEKFhorLin.h
 *
 *  Created on: 24 feb 2025
 *      Author: massimo
 */

#ifndef MULTIRATEEKFHORLIN_H_
#define MULTIRATEEKFHORLIN_H_

#include <stdio.h>

#include <inttypes.h>

#include "Matrix.h"

#include <string>
using namespace std;

class MultirateEKFhorLin
{
private:
	double *mTimeStamp;

	string mName;

    //
    // NGC: local NED frame latitude and longitude
    //
    double *lat0;
    double *lon0;

	//
	// input: measured latitude-longitude, NED speed and velocity in the body-fixed reference frame
	//
	double *lat; // measured latitude
	double *lon; // measured longitude
	bool *validLatLon; // valid latitude-longitude measurement flag
	double *U; // measured speed
	double *chi; // measured course
	bool *validSpeed; // valid NED speed measurement flag
	double *u; // measured surge rate
	double *v; // measured sway rate
	bool *validSurgeSway; // valid surge-sway rate measurement flag
	//
	// measurement noise: position, NED speed and velocity
	//
	double *rXY;
	double *rU,*rChi;
	double *rSurgeSway;
	//
	// input: estimated heading and yaw rate
	//
	double *psiHat; // heading
	double *rHat; // yaw rate
	//
	// output: estimates
	//
	double *latHat; // estimated latitude
	double *lonHat; // estimated longitude
	bool *validLatLonHat; // valid latitude-longitude estimate flag
	double *urHat; // estimated surge rate with respect to the water
	double *vrHat; // estimated sway rate with respect to the water
	bool *validurvrHat; // valid surge-sway rate with respect to the water estimate flag
	double *xCdotHat; // estimated North sea current
	double *yCdotHat; // estimated East sea current
	bool *validSeaCurrentHat; // valid sea current estimate flag
	// covariance of the estimates
	Matrix *Phat;

	double mDt;

	bool mInitFlag;

public:
	// task constructor
	MultirateEKFhorLin(double dt);
	~MultirateEKFhorLin(void) {};

	void set_time_stamp(double *timeStamp) { mTimeStamp=timeStamp; }
	void set_name(string name) { mName=name; }

	void set_local_NED_frame(double *lat0,double *lon0)
		{ this->lat0=lat0; this->lon0=lon0; }
	void MultirateEKFhorLin::set_input_measurements(double *lat,double *lon,bool *validLatLon,
			double *U,double *chi,bool *validSpeed,
			double *u,double *v,bool *validSurgeSway,
			double *rXY,double *rU,double *rChi,double *rSurgeSway) {
		this->lat=lat; this->lon=lon; this->validLatLon=validLatLon;
		this->U=U; this->chi=chi; this->validSpeed=validSpeed;
		this->u=u; this->v=v; this->validSurgeSway=validSurgeSway;
		this->rXY=rXY; this->rU=rU; this->rChi=rChi; this->rSurgeSway=rSurgeSway;
	}
	void MultirateEKFhorLin::set_input_estimates(double *psiHat,double *rHat) {
		this->psiHat=psiHat; this->rHat=rHat;
	}
	void MultirateEKFhorLin::set_output_estimates(double *latHat,double *lonHat,bool *validLatLonHat,
			double *urHat,double *vrHat,bool *validurvrHat,
			double *xCdotHat,double *yCdotHat,bool *validSeaCurrentHat,) {
		this->latHat=latHat; this->lonHat=lonHat; this->validLatLonHat=validLatLonHat;
		this->urHat=urHat; this->vrHat=vrHat; this->validurvrHat=validurvrHat;
		this->xCdotHat=xCdotHat; this->lonHat=lonHat; this->validLatLonHat=validLatLonHat;
	}
	// Matrix *Phat;

	bool read_configuration(string filename);

	void execute(void);
	void reset(void);

	void print(void);

private:
	//
	// latitude-longitude, NED speed and velocity uncertainty (init)
	//
	double sigma_lat_lon0;
	double sigma_ur_vr0;
	double sigma_sea_current0;

	//
	// system noise (acceleration)
	//
	double sigma_urvrDot;
	double sigma_seaCurrentDot;

private:
	// state transition matrix: 6x6
	Matrix F;
	// control-input matrix: 6x2
	Matrix B;
	// control vector: 2x1
	Matrix uCntrl;
	// estimated state and covariance: 6x1 , 6x6
	Matrix x,P;
	// predicted  state and covariance: 6x1 , 6x6
	Matrix xPred,Ppred;
	// system noise covariance: 6x6
	Matrix Q;
	// measurement noise covariance: 6x6
	Matrix R;
	// measurement, expected measurement, and innovation vector: 6x1 ; 6x1 ; 6x1
	Matrix z,zHat,y;
	// observation matrix: 6x6
	Matrix H;
	// innovation covariance: 6x6
	Matrix S;
	// Kalman gain: 6x6
	Matrix K;
	// identity matrix: 6x6
	Matrix I;


	double mu,mv,Xu,Xuu,Xv,Xvv,mr2,dx,dy;

private:
	void init_filter(void);

};


#endif /* MULTIRATEEKFHORLIN_H_ */
