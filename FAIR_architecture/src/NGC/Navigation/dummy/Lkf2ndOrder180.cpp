/*
 * Lkf2ndOrder180.cpp
 *
 *  Created on: 14 February 2025
 *      Author: massimo
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "Lkf2ndOrder180.h"
#include "MathAngles.h"


Lkf2ndOrder180::Lkf2ndOrder180(double dt):
F(2,2),
xState(2,1),P(2,2),
xPred(2,1),Ppred(2,2),
Q(2,2),R(2,2),
z(2,1), zHat(2,1),y(2,1),
H(2,2),
S(2,2),
K(2,2),
I(2,2)
{
	mTimeStamp=NULL;

	// input: measured position, measured velocity
	x = NULL; // measured position
	xDot = NULL; // measured velocity
	valid = NULL; // valid measurement flag
	//
	// measurement noise: position and velocity
	//
	rP = NULL;
	rV = NULL;
	// output: estimates
	xHat = NULL; // estimated position
	xDotHat = NULL; // estimated velocity
	xValidHat = NULL; // valid estimate flag
	xDotValidHat = NULL; // valid estimate flag
	Pxx = NULL; // covariance of estimated position
	PxDotxDot = NULL; // covariance of estimated velocity

	mDt=dt;

	mInitFlag=true;

	F.eye(); F(0,1)=mDt;
	H.eye();
	I.eye();

	//
	// position and velocity uncertainty (init)
	//
	sigma_x0 = 0.;
	sigma_xDot0 = 0.;

	//
	// system noise (acceleration)
	//
	sigma_xDotDot = 0.;
};


void Lkf2ndOrder180::execute(void)
{
//	printf("Lkf2ndOrder180::execute mTimeStamp %.3lf\n",*mTimeStamp);fflush(stdout);
;
	//
	// prediction step
	//
	xPred = F*xState;
	xPred(0,0)=mod180(xPred(0,0));
	Ppred = F*P*F.t() + Q;
	//
	// check available measurements
	//
	bool flag = *valid;
	//
	// init filter
	//
	if(mInitFlag) {
		//printf("Lkf2ndOrder180::execute init mTimeStamp %lld\n",(long long unsigned int)*mTimeStamp);fflush(stdout);
		//
		// init filter
		//
		if(flag) {
			init_filter();
			mInitFlag=false;
		}
		else
			reset();
		return ;
	}

	//
	// update step
	//
	if(flag) { // position measurements are available
		// build measurement vector
		z(0,0) = *x;
		z(1,0) = *xDot;
		// build predicted measurement vector
		zHat = H * xPred;
		// compute innovation
		y = z - zHat;
		y(0,0)=mod180(y(0,0));
		// set measurement noise
		R(0,0) = (*rP) * (*rP);
		R(1,1) = (*rV) * (*rV);
		// compute innovation covariance
		S = H*Ppred*H.t() + R;
		// compute Kalman gain
		K = Ppred*H.t()*S.inv();
		// update state estimate
		xState = xPred + K*y;
		xState(0,0)=mod180(xState(0,0));
		// update estimate covariance
		P = Ppred - K*H*Ppred;
	}
	else { // no measurements are available
		// update state estimate
		xState = xPred;
		// update estimate covariance
		P = Ppred;
	}

	//
	// update estimate
	//
	*xHat = xState(0,0);
	*xDotHat = xState(1,0);
	*xValidHat = true;
	*xDotValidHat = true;
	*Pxx = sqrt(P(0,0));
	*PxDotxDot = sqrt(P(1,1));
}

void Lkf2ndOrder180::init_filter(void) {
	// state
	xState.zeros();
	xState(0,0)=*x;
	xState(1,0)=*xDot;
	// state covariance
	P.zeros();
	P(0,0) = sigma_x0*sigma_x0;
	P(1,1) = sigma_xDot0*sigma_xDot0;
}


void Lkf2ndOrder180::reset(void) {
	mInitFlag=true;

	Q.zeros(); Q(1,1)=sigma_xDotDot*sigma_xDotDot*mDt;

	*xValidHat=false;
	*xDotValidHat=false;

}

bool Lkf2ndOrder180::read_configuration(string filename) {
	FILE *f;

	if((f=fopen(filename.c_str(),"r"))==NULL) {
		printf("Lkf2ndOrder180::read_configuration : ERROR opening file %s\n",filename.c_str());
		fflush(stdout);
		printf("Lkf2ndOrder180::read_configuration before return\n");fflush(stdout);
		return false;
	}

	string str;
	char strChar[64];
	//
	// read init position uncertainty --> P(0,0)
	//
	// read init_position_uncertainty
	fscanf(f,"%s",strChar); str=strChar;
	if(str!="init_position_uncertainty") {
		printf("Lkf2ndOrder180::read_configuration ERROR - init_position_uncertainty expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&sigma_x0);
	//
	// read init velocity uncertainty --> P(1,1)
	//
	// read init_velocity_uncertainty
	fscanf(f,"%s",strChar); str=strChar;
	if(str!="init_velocity_uncertainty") {
		printf("Lkf2ndOrder180::read_configuration ERROR - init_velocity_uncertainty expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&sigma_xDot0);
	//
	// read init system noise --> Q(1,1)
	//
	// read init_system_noise
	fscanf(f,"%s",strChar); str=strChar;
	if(str!="init_system_noise") {
		printf("Lkf2ndOrder180::read_configuration ERROR - init_system_noise expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&sigma_xDotDot);

	fclose(f);
	return true;

}


