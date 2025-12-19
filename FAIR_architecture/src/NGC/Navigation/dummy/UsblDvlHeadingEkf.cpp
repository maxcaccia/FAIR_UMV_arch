/*
 * PositionVelocityHeadingEkf.cpp
 *
 *  Created on: 7 mag 2022
 *      Author: massimo
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "PositionVelocityHeadingEkf.h"
#include "GuidanceControlAlgo.h"


PositionVelocityHeadingEkf::PositionVelocityHeadingEkf(double dt):
F(2,2),
B(2,2),
u(2,1),
x(2,1),	P(2,2),
xPred(2,1),	Ppred(2,2),
Q(2,2), R(2,2),
z(2,1), zHat(2,1), y(2,1),
H(2,2),
S(2,2),
K(2,2),
{
	mTimeStamp=NULL;

	mAngPosHat=NULL;
	mLinPosMeas=NULL;
	mLinVelHat=NULL;
	mLinPosHat=NULL;

	mDt=dt;

	mInitFlag=true;

	F.zeros(); F(0,0)=F(1,1)=1.;
	B.zeros(); B(0,0)=B(1,1)=mDt;

	dx0=0.;

	sigmaXdot=0.;
	roX=0.;
};


void PositionVelocityHeadingEkf::execute(void)
{
//	printf("PositionVelocityHeadingEkf::execute mTimeStamp %.3lf\n",*mTimeStamp);fflush(stdout);

	//
	// set input
	//
	double xDot = mLinVelHat->u * cos(mAngPosHat->psi*M_PI/180.) - mLinVelHat->v * sin(mAngPosHat->psi*M_PI/180.);
	double yDot = mLinVelHat->u * sin(mAngPosHat->psi*M_PI/180.) + mLinVelHat->v * cos(mAngPosHat->psi*M_PI/180.);
	u(0,0) = xDot;
	u(1,0) = yDot;
	//
	// prediction step
	//
	xPred = F*x + B*u;
	Ppred = F*P*F.t() + Q;
	//
	// check available measurements
	//
	bool flag = mLinPosMeas->mTimeStamp == *mTimeStamp;
	//
	// init filter
	//
	if(mInitFlag) {
		//printf("PositionVelocityHeadingEkf::execute init mTimeStamp %lld\n",(long long unsigned int)*mTimeStamp);fflush(stdout);
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
		z(0,0) = mLinPosMeas->x;
		z(1,0) = mLinPosMeas->y.;
		// build predicted measurement vector
		zHat(0,0) = x(0,0); // x
		zHat(1,0) = x(1,0); // y
		// compute innovation
		y = z - zHat;
		// compute innovation covariance
		S = H*Ppred*H.t() + R;
		// compute Kalman gain
		K = Ppred*H.t()*S.inv();
		// update state estimate
		x = xPred + K*y;
		// update estimate covariance
		P = Ppred - K*H*Ppred;
	}
	else { // no measurements are available
		// update state estimate
		x = xPred;
		// update estimate covariance
		P = Ppred;
	}
}


void GnssEkf::set_input_output(AngularPosition *angPosHat,LinearVelocity *linVelHat,
		GnssPositionData *gnssGGA,GnssVelocityData *gnssVTG,LinearPosition *linPosHat) {
	mAngPosHat=angPosHat;
	mLinVelHat=linVelHat;
	mGnssGGA=gnssGGA;
	mGnssVTG=gnssVTG;
	mLinPosHat=linPosHat;

	dx0=0.;
	dxDotC0=0.;

	sigmaXdot=0.;
	sigmaCurrentDot=0.;
	roX=0.;
	roU=0.;
	roChi=0.;
}

bool GnssEkf::read_configuration(string filename) {
	FILE *f;

	if((f=fopen(filename.c_str(),"r"))==NULL) {
		printf("GnssEkf::read_configuration : ERROR opening file %s\n",filename.c_str());
		fflush(stdout);
		fclose(f);
		return false;
	}

	string str;
	//
	// read init position uncertainty --> P00(0:1,0:1)
	//
	// read init_position_uncertainty
	fread_string(f,str);
	if(str!="init_position_uncertainty") {
		printf("GnssEkf::read_configuration ERROR - init_position_uncertainty expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&dx0);
	//
	// read init sea current uncertainty --> P00(2:3,2:3)
	//
	// read init_sea_current_uncertainty
	fread_string(f,str);
	if(str!="init_sea_current_uncertainty") {
		printf("GnssEkf::read_configuration ERROR - init_sea_current_uncertainty expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&dxDotC0);

	double value;
	//
	// read system noise speed --> Q(0:1,0:1)
	//
	// read system_noise_speed
	fread_string(f,str);
	if(str!="system_noise_speed") {
		printf("GnssEkf::read_configuration ERROR - system_noise_speed expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&value);
	sigmaXdot = value * mDt;
	//
	// read system noise sea current acceleration --> Q(2:3,2:3)
	//
	// read system_noise_sea_current_acceleration
	fread_string(f,str);
	if(str!="system_noise_sea_current_acceleration") {
		printf("GnssEkf::read_configuration ERROR - system_noise_sea_current_acceleration expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&value);
	sigmaCurrentDot = value * mDt;

	//
	// read GNSS position measurement noise --> R
	//
	// read measurement_noise_gnss_position
	fread_string(f,str);
	if(str!="measurement_noise_gnss_position") {
		printf("GnssEkf::read_configuration ERROR - measurement_noise_gnss_position expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&roX);
	//
	// read GNSS speed measurement noise --> R
	//
	// read measurement_noise_gnss_speed
	fread_string(f,str);
	if(str!="measurement_noise_gnss_speed") {
		printf("GnssEkf::read_configuration ERROR - measurement_noise_gnss_speed expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&roU);
	//
	// read GNSS course measurement noise --> R
	//
	// read measurement_noise_gnss_course
	fread_string(f,str);
	if(str!="measurement_noise_gnss_course") {
		printf("GnssEkf::read_configuration ERROR - measurement_noise_gnss_course expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&roChi);

	fclose(f);
	return true;
}


void GnssEkf::init_filter(void) {
	x.zeros(); compute_NED_position(x(0,0),x(1,0));

	P.zeros(); P(0,0)=P(1,1)=dx0*dx0; P(2,2)=P(3,3)=dxDotC0*dxDotC0;
}



void UsblDvlHeadingEkfEkf::execute(void)
{
	//
	// set time-variant entries of the state transition matrix
	//
	F(0,2) = mDt * cos(mAngPosHat->psi*M_PI/180.);
	F(0,3) = - mDt * sin(mAngPosHat->psi*M_PI/180.);
	F(1,2) = mDt * sin(mAngPosHat->psi*M_PI/180.);
	F(1,3) = mDt * cos(mAngPosHat->psi*M_PI/180.);
	//
	// prediction step
	//
	xPred = F*x;
	Ppred = F*P*F.t() + Q;
	//
	// check available measurements
	//
	bool flagUsbl = mUsblMeas->mTimeStamp == *mTimeStamp;
	bool flagDvl = mDvlMeas->mTimeStamp == *mTimeStamp;
	//
	// init filter
	//
	if(mInitFlag) {
		//
		// init filter
		//
		if(flagUsbl) {
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
	if(flagUsbl && flagDvl) { // position and speed measurements are available
		// build measurement vector
		z4(0,0) = mUsblMeas->x;
		z4(1,0) = mUsblMeas->y;
		z4(2,0) = mDvlMeas->u;
		z4(3,0) = mDvlMeas->v;
		// observation matrix is H4: 4x4 identity matrix
		// build predicted measurement vector
		zHat4 = H4 * xPred;
		// compute innovation
		y4 = z4 - zHat4;
		// compute innovation covariance
		S4 = H4*Ppred*H4.t() + R4;
		// compute Kalman gain
		K4 = Ppred*H4.t()*S4.inv();
		// update state estimate
		x = xPred + K4*y4;
		// update estimate covariance
		P = Ppred - K4*H4*Ppred;
	}
	else if(flagUsbl && (!flagDvl)) { // only position measurements are available
		// build measurement vector
		z2(0,0) = mUsblMeas->x;
		z2(1,0) = mUsblMeas->y;
		// observation matrix is H: 2x4 USBL matrix
		H2.zeros();
		H2(0,0)=1.; H2(1,1)=1.;		
		// build predicted measurement vector
		zHat2 = H2 * xPred;
		// compute innovation
		y2 = z2 - zHat2;
		// compute innovation covariance
		S2 = H2*Ppred*H2.t() + R2p;
		// compute Kalman gain
		K2 = Ppred*H2.t()*S2.inv();
		// update state estimate
		x = xPred + K2*y2;
		// update estimate covariance
		P = Ppred - K2*H2*Ppred;
	}
	else if((!flagUsbl) && flagDvl) { // only speed measurements are available
		// build measurement vector
		z2(0,0) = mDvlMeas->u;
		z2(1,0) = mDvlMeas->v;
		// observation matrix is H: 2x4 DVL matrix
		H2.zeros();
		H2(0,2)=1.; H2(1,3)=1.;	
		// build predicted measurement vector
		zHat2 = H2 * xPred;
		// compute innovation
		y2 = z2 - zHat2;
		// compute innovation covariance
		S2 = H2*Ppred*H2.t() + R2v;
		// compute Kalman gain
		K2 = Ppred*H2.t()*S2.inv();
		// update state estimate
		x = xPred + K2*y2;
		// update estimate covariance
		P = Ppred - K2*H2*Ppred;
	}
	else { // no measurements are available
		// update state estimate
		x = xPred;
		// update estimate covariance
		P = Ppred;
	}
}






void GnssEkf::reset(void)
{

	mInitFlag=true;

//	init_filter();

	Q.zeros(); Q(0,0)=Q(1,1)=sigmaXdot*sigmaXdot; Q(2,2)=Q(3,3)=sigmaCurrentDot*sigmaCurrentDot;
	R2xy.zeros(); R2xy(0,0)=R2xy(1,1)=roX*roX;
	R2Uchi.zeros(); R2Uchi(0,0)=roU*roU; R2Uchi(1,1)=roChi*roChi;
	R4.zeros(); R4(0,0)=R4(1,1)=roX*roX; R4(2,2)=roU*roU; R4(3,3)=roChi*roChi;

}

void GnssEkf::print(void) {
	printf("GnssEkf::print %s\n",mName.c_str()); fflush(stdout);

	printf("filter parameters:\n");
	printf("   init_position_uncertainty : %.3lf\n   init_sea_current_uncertainty : %.3lf\n",dx0,dxDotC0);
	printf("   system_noise_speed : %.3lf\n   system_noise_sea_current_acceleration : %.3lf\n",sigmaXdot/mDt,sigmaCurrentDot/mDt);
	printf("   measurement_noise_gnss_position : %.3lf\n   measurement_noise_gnss_speed : %.3lf\n   measurement_noise_gnss_course : %.3lf\n",
			roX,roU,roChi);

}


