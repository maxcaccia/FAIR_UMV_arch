/*
 * AttitudeLKf2ndOrder180.cpp
 *
 *  Created on: 25 February 2025
 *      Author: massimo
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "AttitudeLKf2ndOrder180.h"
#include "MathAngles.h"


AttitudeLKf2ndOrder180::AttitudeLKf2ndOrder180(double dt):
F(6,6),
xState(6,1),P(6,6),
xPred(6,1),Ppred(6,6),
Q(6,6),R(6,6),
z(6,1), zHat(6,1),y(6,1),
H(6,6),
S(6,6),
K(6,6),
I(6,6)
{
	mTimeStamp=NULL;

	// input: measured attitude and angular rate
	mPhiThetaPsi = NULL; // measured roll, pitch, yaw
	mpqr = NULL; // measured roll, pitch, yaw rate
	//
	// measurement noise: position and velocity
	//
	// output: estimated attitude and angular rate
	mPhiThetaPsiHat = NULL; // estimated roll, pitch, yaw
	mpqrHat = NULL; // estimated roll, pitch, yaw rate

	mDt=dt;

	mInitFlag=true;

	F.eye(); F(0,1)=F(2,3)=F(4,5)=mDt;
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


void AttitudeLKf2ndOrder180::execute(void)
{
	printf("AttitudeLKf2ndOrder180::execute mTimeStamp %.3lf\n",*mTimeStamp);fflush(stdout);

	//
	// prediction step
	//
	xPred = F*xState;
	xPred(0,0)=mod180(xPred(0,0));
	xPred(2,0)=mod180(xPred(2,0));
	xPred(4,0)=mod180(xPred(4,0));
	Ppred = F*P*F.t() + Q;
	//
	// check available measurements
	//
	bool flag = mPhiThetaPsi->mValid;
	//
	// init filter
	//
	if(mInitFlag) {
		//printf("AttitudeLKf2ndOrder180::execute init mTimeStamp %lld\n",(long long unsigned int)*mTimeStamp);fflush(stdout);
		//
		// init filter
		//
		if(flag) {
			init_filter();
			mInitFlag=false;
		}
		else
			reset();
//		printf("AttitudeLKf2ndOrder180::execute return\n");fflush(stdout);
		return ;
	}

	//
	// update step
	//
	if(flag) { // position measurements are available
		// build measurement vector
		z(0,0) = mPhiThetaPsi->phi;
		z(1,0) = mpqr->p;
		z(2,0) = mPhiThetaPsi->theta;
		z(3,0) = mpqr->q;
		z(4,0) = mPhiThetaPsi->psi;
		z(5,0) = mpqr->r;
		// build predicted measurement vector
		zHat = H * xPred;
		// compute innovation
		y = z - zHat;
		y(0,0)=mod180(y(0,0));
		y(2,0)=mod180(y(2,0));
		y(4,0)=mod180(y(4,0));
		// set measurement noise
		R(0,0) = SQR(mPhiThetaPsi->phiStd);
		R(1,1) = SQR(mpqr->pStd);
		R(2,2) = SQR(mPhiThetaPsi->thetaStd);
		R(3,3) = SQR(mpqr->qStd);
		R(4,4) = SQR(mPhiThetaPsi->psiStd);
		R(5,5) = SQR(mpqr->rStd);
		// compute innovation covariance
		S = H*Ppred*H.t() + R;
		// compute Kalman gain
		K = Ppred*H.t()*S.inv();
		// update state estimate
		xState = xPred + K*y;
		xState(0,0)=mod180(xState(0,0));
		xState(2,0)=mod180(xState(2,0));
		xState(4,0)=mod180(xState(4,0));
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
	mPhiThetaPsiHat->phi = xState(0,0);
	mpqrHat->p = xState(1,0);
	mPhiThetaPsiHat->theta = xState(2,0);
	mpqrHat->q = xState(3,0);
	mPhiThetaPsiHat->psi = xState(4,0);
	mpqrHat->r = xState(5,0);

	mPhiThetaPsiHat->phiStd = sqrt(P(0,0));
	mpqrHat->pStd = sqrt(P(1,1));
	mPhiThetaPsiHat->thetaStd = sqrt(P(2,2));
	mpqrHat->qStd = sqrt(P(3,3));
	mPhiThetaPsiHat->psiStd = sqrt(P(4,4));
	mpqrHat->rStd = sqrt(P(5,5));

	mPhiThetaPsiHat->mValid = mpqrHat->mValid = !mInitFlag;

//	printf("AttitudeLKf2ndOrder180::execute end\n");fflush(stdout);

}

void AttitudeLKf2ndOrder180::init_filter(void) {
//	printf("AttitudeLKf2ndOrder180::init_filter begin\n");fflush(stdout);
	// state
	xState.zeros();
	xState(0,0) = mPhiThetaPsi->phi;
	xState(1,0) = mpqr->p;
	xState(2,0) = mPhiThetaPsi->theta;
	xState(3,0) = mpqr->q;
	xState(4,0) = mPhiThetaPsi->psi;
	xState(5,0) = mpqr->r;
	// state covariance
	P.zeros();
	P(0,0) = P(2,2) = P(4,4) = sigma_x0*sigma_x0;
	P(1,1) = P(3,3) = P(5,5) = sigma_xDot0*sigma_xDot0;
//	printf("AttitudeLKf2ndOrder180::init_filter end\n");fflush(stdout);
}


void AttitudeLKf2ndOrder180::reset(void) {
//	printf("AttitudeLKf2ndOrder180::reset begin\n");fflush(stdout);
	mInitFlag=true;

	Q.zeros();
//	Q.print();fflush(stdout);
	Q(1,1)=Q(3,3)=Q(5,5)=sigma_xDotDot*sigma_xDotDot*mDt;

	mPhiThetaPsiHat->mValid = mpqrHat->mValid = !mInitFlag;
//	printf("AttitudeLKf2ndOrder180::reset end\n");fflush(stdout);
}


bool AttitudeLKf2ndOrder180::read_configuration(string filename) {
	FILE *f;

	if((f=fopen(filename.c_str(),"r"))==NULL) {
		printf("AttitudeLKf2ndOrder180::read_configuration : ERROR opening file %s\n",filename.c_str());
		fflush(stdout);
		return false;
	}

	string str;
	char strChar[64];
	//
	// read init position uncertainty --> P(0,0),P(2,2),P(4,4)
	//
	// read init_position_uncertainty
	fscanf(f,"%s",strChar); str=strChar;
	if(str!="init_position_uncertainty") {
		printf("AttitudeLKf2ndOrder180::read_configuration ERROR - init_position_uncertainty expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&sigma_x0);
	//
	// read init velocity uncertainty --> P(1,1),P(3,3),P(5,5)
	//
	// read init_velocity_uncertainty
	fscanf(f,"%s",strChar); str=strChar;
	if(str!="init_velocity_uncertainty") {
		printf("AttitudeLKf2ndOrder180::read_configuration ERROR - init_velocity_uncertainty expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&sigma_xDot0);
	//
	// read init system noise --> Q(1,1),Q(3,3),Q(5,5)
	//
	// read init_system_noise
	fscanf(f,"%s",strChar); str=strChar;
	if(str!="init_system_noise") {
		printf("AttitudeLKf2ndOrder180::read_configuration ERROR - init_system_noise expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&sigma_xDotDot);

	fclose(f);
	return true;
}


