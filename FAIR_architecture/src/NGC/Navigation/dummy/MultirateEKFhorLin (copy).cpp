/*
 * MultirateEKFhorLin.cpp
 *
 *  Created on: 24 feb 2025
 *      Author: massimo
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "MultirateEKFhorLin.h"
#include "MathAngles.h"
#include "nVector.h"


MultirateEKFhorLin::MultirateEKFhorLin(double dt):
F(6,6),
B(6,2),
uCntrl(2,1),
x(6,1),	P(6,6),
xPred(6,1),	Ppred(6,6),
Q(6,6), R(6,6),
z(6,1),zHat(6,1), y(6,1),
H(6,6), S(6,6), K(6,6), I(6,6)
{
	mTimeStamp=NULL;

	mDt=dt;

	mInitFlag=true;

	F.zeros(); F(0,0)=F(1,1)=F(4,4)=F(5,5)=1.;  F(0,4)=F(1,5)=mDt;
	B.zeros(); B(2,0)=mDt/mu; B(3,1)=mDt/mv;
	I.eye();

    //
    // NGC: local NED frame latitude and longitude
    //
    lat0 = NULL;
    lon0 = NULL;
	//
	// input: measured latitude-longitude, NED speed and velocity in the body-fixed reference frame
	//
	lat = NULL; // measured latitude
	lon = NULL; // measured longitude
	validLatLon = NULL; // valid latitude-longitude measurement flag
	U = NULL; // measured speed
	chi = NULL; // measured course
	validSpeed = NULL; // valid NED speed measurement flag
	u = NULL; // measured surge rate
	v = NULL; // measured sway rate
	validSurgeSway = NULL; // valid surge-sway rate measurement flag
	//
	// measurement noise: position, NED speed and velocity
	//
	rXY = NULL;
	rU = NULL;
	rChi = NULL;
	rSurgeSway = NULL;
	//
	// input: estimated heading and yaw rate
	//
	psiHat = NULL; // heading
	rHat = NULL; // yaw rate
	//
	// output: estimates
	//
	latHat = NULL; // estimated latitude
	lonHat = NULL; // estimated longitude
	validLatLonHat = NULL; // valid latitude-longitude estimate flag
	urHat = NULL; // estimated surge rate with respect to the water
	vrHat = NULL; // estimated sway rate with respect to the water
	validurvrHat = NULL; // valid surge-sway rate with respect to the water estimate flag
	xCdotHat = NULL; // estimated North sea current
	yCdotHat = NULL; // estimated East sea current
	validSeaCurrentHat = NULL; // valid sea current estimate flag
	// covariance of the estimates
	Phat = NULL;

	//
	// latitude-longitude, NED speed and velocity uncertainty (init)
	//
	sigma_lat_lon0 = 0.;
	sigma_ur_vr0 = 0.;
	sigma_sea_current0 = 0.;

	//
	// system noise (acceleration)
	//
	sigma_urvrDot = 0.;
	sigma_seaCurrentDot = 0.;

	mu=mv=Xu=Xuu=Xv=Xvv=mr2=dx=dy=0.;
};


void MultirateEKFhorLin::execute(void)
{
	printf("MultirateEKFhorLin::execute mTimeStamp %.3lf\n",*mTimeStamp);fflush(stdout);

	double psiRad = *psiHat*M_PI/180.;
	double cpsi=cos(psiRad);
	double spsi=sin(psiRad);
	double rRad = *rHat*M_PI/180.;
	//
	// nonlinear prediction step : state
	//
	xPred(0,0) = x(0,0) + (x(2,0)*cpsi-x(3,0)*spsi+x(4,0))*mDt;
	xPred(1,0) = x(1,0) + (x(2,0)*spsi+x(3,0)*cpsi+x(5,0))*mDt;
	xPred(2,0) = x(2,0) + (Xu*x(2,0)+Xuu*x(2,0)*fabs(x(2,0))+mv*rRad*x(3,0)/mu+mr2*rRad*rRad+uCntrl(0,0)/mu)*mDt;
	xPred(3,0) = x(3,0) + (Xv*x(3,0)+Xvv*x(3,0)*fabs(x(3,0))-mu*rRad*x(2,0)/mv+uCntrl(1,0)/mv)*mDt;
	xPred(4,0) = x(4,0);
	xPred(5,0) = x(5,0);
	//
	// nonlinear prediction step : observation
	//
	// latitude-longitude
	double xLat=xPred(0,0);
	double yLon=xPred(1,0);
	// NED velocity prediction
	double xDotPred = xPred(2,0)*cpsi-xPred(3,0)*spsi+xPred(4,0)-rRad*(dx*spsi+dy*cpsi);
	double yDotPred = xPred(2,0)*spsi+xPred(3,0)*cpsi+xPred(5,0)+rRad*(dx*cpsi-dy*spsi);
	double Upred2=xDotPred*xDotPred+yDotPred*yDotPred;
	double Upred=sqrt(Upred2);
	double chiPred=atan2(yDotPred , xDotPred);
	//  surge-sway rate
	double uPred = xPred(2,0)+xPred(4,0)*cpsi+xPred(5,0)*spsi-rRad*dy;
	double vPred = xPred(3,0)-xPred(4,0)*spsi+xPred(5,0)*cpsi+rRad*dx;
	// predicted measurement vector
	zHat(0,0) = xLat;
	zHat(1,0) = yLon;
	zHat(2,0) = Upred;
	zHat(3,0) = chiPred;
	zHat(4,0) = uPred;
	zHat(5,0) = vPred;
	//
	//
	// linearised state transition matrix
	//
	F(0,2)=cpsi*mDt; F(0,3)=-spsi*mDt;
	F(1,2)=spsi*mDt; F(1,3)=cpsi*mDt;
	F(2,2)=1+(Xu+2*Xuu*fabs(xPred(2,0)))*mDt; F(2,3)=mv*rRad*mDt/mu;
	F(3,2)=-mu*rRad*mDt/mv; F(3,3)=1+(Xv+2*Xvv*fabs(xPred(3,0)))*mDt;
	//
	// prediction of estimate covariance
	//
	Ppred = F*P*F.t() + Q;
	//
	// linearised observation matrix
	//
	H.eye();
	// latitude-longitude measurement
	H(0,0)=H(1,1)=1.;
	// NED speed and course
	H(2,2) = (xPred(2,0)+xPred(4,0)*cpsi+xPred(5,0)*spsi-rRad*dy)/Upred;
	H(2,3) = (xPred(3,0)-xPred(4,0)*spsi+xPred(5,0)*cpsi+rRad*dx)/Upred;
	H(2,4) = xDotPred/Upred;
	H(2,5) = yDotPred/Upred;
	H(3,2) = -(xPred(3,0)-xPred(4,0)*spsi+xPred(5,0)*cpsi+rRad*dx)/Upred2;
	H(3,3) = (xPred(2,0)+xPred(4,0)*cpsi+xPred(5,0)*spsi-rRad*dy)/Upred2;
	H(3,4) = -yDotPred/Upred2;
	H(3,5) = xDotPred/Upred2;
	// surge and sway rate measurement
	H(4,2) = 1; H(4,4) = cpsi; H(4,5) = spsi;
	H(5,3) = 1; H(5,4) = -spsi; H(5,5) = cpsi;

	//
	// check available measurements
	//
	bool flagPosition = *validLatLon;
	bool flagNEDspeed = *validSpeed;
	bool flagVelocity = *validSurgeSway;
//	//
//	// compute available measurement devices
//	//
//	int flag = (int)flagPosition + (int)flagNEDspeed + (int)flagVelocity;

	//
	// init filter
	//
	if(mInitFlag) {
		printf("MultirateEKFhorLin::execute init mTimeStamp %lld\n",(long long unsigned int)*mTimeStamp);fflush(stdout);
		//
		// init filter
		//
		if(flagPosition) {
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
	// build measurement vector
	double xV,yV;
	lat_lon_2_x_y(xV,yV,*lat0,*lon0,*lat,*lon);
	z(0,0) = xV;
	z(1,0) = yV;
	z(2,0) = *U;
	z(3,0) = *chi*M_PI/180.;
	z(4,0) = *u;
	z(5,0) = *v;
	// check available/valid measurements
	if(!flagPosition) {
		z(0,0) = zHat(0,0) = 0.;
		z(1,0) = zHat(1,0) = 0.;
		H(0,0)=H(0,1)=H(0,2)=H(0,3)=H(0,4)=H(0,5)=0.;
		H(1,0)=H(1,1)=H(1,2)=H(1,3)=H(1,4)=H(1,5)=0.;
	}
	if(!flagNEDspeed) {
		z(2,0) = zHat(2,0) = 0.;
		z(3,0) = zHat(3,0) = 0.;
		H(2,0)=H(2,1)=H(2,2)=H(2,3)=H(2,4)=H(2,5)=0.;
		H(3,0)=H(3,1)=H(3,2)=H(3,3)=H(3,4)=H(3,5)=0.;
	}
	if(!flagVelocity) {
		z(4,0) = zHat(4,0) = 0.;
		z(5,0) = zHat(5,0) = 0.;
		H(4,0)=H(4,1)=H(4,2)=H(4,3)=H(4,4)=H(4,5)=0.;
		H(5,0)=H(5,1)=H(5,2)=H(5,3)=H(5,4)=H(5,5)=0.;
	}
	// compute innovation
	y = z - zHat;
	y(3,0) = modpi(y(3,0));
	// set measurement noise
	R(0,0) = R(1,1) = (*rXY) * (*rXY);
	R(2,2) = (*rU) * (*rU);
	R(3,3) = (*rChi) * (*rChi);
	R(2,2) = (*rSurgeSway) * (*rSurgeSway);
	R(2,2) = (*rSurgeSway) * (*rSurgeSway);
	// compute innovation covariance
	S = H*Ppred*H.t() + R;
	// compute Kalman gain
	K = Ppred*H.t()*S.inv();
	// update state estimate
	x = xPred + K*y;
	// update estimate covariance
	P = Ppred - K*H*Ppred;

	//
	// update estimate
	//
	lat_lon_plus_x_y(*latHat,*lonHat,*lat0,*lon0,x(0,0),x(1,0));
	*validLatLonHat = true;
	*urHat = x(2,0);
	*vrHat = x(3,0);
	*validurvrHat = true;
	*xCdotHat = x(4,0);
	*yCdotHat = x(5,0);
	*validSeaCurrentHat = true;
	*Phat = P;
}

bool MultirateEKFhorLin::read_configuration(string filename) {
	FILE *f;

	if((f=fopen(filename.c_str(),"r"))==NULL) {
		printf("MultirateEKFhorLin::read_configuration : ERROR opening file %s\n",filename.c_str());
		fflush(stdout);
		fclose(f);
		return false;
	}

	string str;
	char strChar[64];
	//
	// read init position uncertainty --> P(0,0),P(1,1)
	//
	// read init_position_uncertainty
	fscanf(f,"%s",strChar); str=strChar;
	if(str!="init_position_uncertainty") {
		printf("MultirateEKFhorLin::read_configuration ERROR - init_position_uncertainty expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&sigma_lat_lon0);
	//
	// read init surge-sway rate uncertainty --> P(2,2),P(3,3)
	//
	// read init_velocity_uncertainty
	fscanf(f,"%s",strChar); str=strChar;
	if(str!="init_velocity_uncertainty") {
		printf("MultirateEKFhorLin::read_configuration ERROR - init_velocity_uncertainty expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&sigma_ur_vr0);
	//
	// read init sea current uncertainty --> P(2,2),P(3,3)
	//
	// read init_sea_current_uncertainty
	fscanf(f,"%s",strChar); str=strChar;
	if(str!="init_sea_current_uncertainty") {
		printf("MultirateEKFhorLin::read_configuration ERROR - init_sea_current_uncertainty expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&sigma_sea_current0);

	//
	// read init surge sway acceleration system noise --> Q(2,2),Q(3,3)
	//
	// read init_surge_sway_acceleration_noise
	fscanf(f,"%s",strChar); str=strChar;
	if(str!="init_surge_sway_acceleration_noise") {
		printf("MultirateEKFhorLin::read_configuration ERROR - init_surge_sway_acceleration_noise expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&sigma_urvrDot);

	//
	// read init sea current acceleration system noise --> Q(2,2),Q(3,3)
	//
	// read init_sea_current_acceleration_noise
	fscanf(f,"%s",strChar); str=strChar;
	if(str!="init_sea_current_acceleration_noise") {
		printf("MultirateEKFhorLin::read_configuration ERROR - init_sea_current_acceleration_noise expected %s found\n",str.c_str());
		fclose(f);
		return false;
	}
	fscanf(f,"%lf",&sigma_seaCurrentDot);

	fclose(f);
	return true;
}


void MultirateEKFhorLin::init_filter(void) {
	x.zeros();
	double xV,yV;
	lat_lon_2_x_y(xV,yV,*lat0,*lon0,*lat,*lon);
	x(0,0) = xV;
	x(1,0) = yV;

	P.zeros();
	P(0,0)=P(1,1)=sigma_lat_lon0*sigma_lat_lon0;
	P(2,2)=P(3,3)=sigma_ur_vr0*sigma_ur_vr0;
	P(4,4)=P(5,5)=sigma_sea_current0*sigma_sea_current0;
}

void MultirateEKFhorLin::reset(void)
{

	mInitFlag=true;

//	init_filter();

	Q.zeros();
	Q(2,2)=Q(3,3)=sigma_urvrDot*sigma_urvrDot;
	Q(4,4)=Q(5,5)=sigma_seaCurrentDot*sigma_seaCurrentDot;

	*validLatLonHat=false;
	*validurvrHat=false;
	*validSeaCurrentHat=false;
}

void MultirateEKFhorLin::print(void) {
	printf("MultirateEKFhorLin::print %s\n",mName.c_str()); fflush(stdout);

//	printf("filter parameters:\n");
//	printf("   init_position_uncertainty : %.3lf\n   init_sea_current_uncertainty : %.3lf\n",dx0,dxDotC0);
//	printf("   system_noise_speed : %.3lf\n   system_noise_sea_current_acceleration : %.3lf\n",sigmaXdot/mDt,sigmaCurrentDot/mDt);
//	printf("   measurement_noise_gnss_position : %.3lf\n   measurement_noise_gnss_speed : %.3lf\n   measurement_noise_gnss_course : %.3lf\n",
//			roX,roU,roChi);

}


