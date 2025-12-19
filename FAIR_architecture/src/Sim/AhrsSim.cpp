//
//  AhrsSim.cpp
//  AHRS simulator
//
//  Created by Massimo Caccia on 02/03/23.
//

#include <stdio.h>

#include "AhrsSim.h"

AhrsSim::AhrsSim(void)
{
	mSamplingTime=AHRS_SAMPLING_TIME;

	mPsiBias=AHRS_PSI_BIAS;
	mAngleNoiseStd=AHRS_ANGLE_NOISE_STD;
	mAngularRateNoiseStd=AHRS_ANGULAR_RATE_NOISE_STD;
	mVelocityNoiseStd=AHRS_VELOCITY_NOISE_STD;
	mAccelerationNoiseStd=AHRS_ACCELERATION_NOISE_STD;

	//
	// instantiate noise generators
	//
	mNormalAngleNoiseGenerator = new NormalNoiseGenerator(0.,mAngleNoiseStd);
	mNormalAngularRateNoiseGenerator = new NormalNoiseGenerator(0.,mAngularRateNoiseStd);
	mNormalVelocityNoiseGenerator = new NormalNoiseGenerator(0.,mVelocityNoiseStd);
	mNormalAccelerationNoiseGenerator = new NormalNoiseGenerator(0.,mAccelerationNoiseStd);

	m_uOld=m_vOld=m_wOld=0.;
	mInitialised=false;
}

AhrsSim::~AhrsSim(void) {
	if(mNormalAngleNoiseGenerator) delete mNormalAngleNoiseGenerator;
	if(mNormalAngularRateNoiseGenerator) delete mNormalAngularRateNoiseGenerator;
	if(mNormalVelocityNoiseGenerator) delete mNormalVelocityNoiseGenerator;
	if(mNormalAccelerationNoiseGenerator) delete mNormalAccelerationNoiseGenerator;
};


bool AhrsSim::get_measurement(AhrsData &ahrsData,
		AngularPosition attitude,LinearVelocity linVel,AngularVelocity angVel,uint64_t elapsedTime)
{
	if((elapsedTime%mSamplingTime)!=0) {
//		printf("AhrsSim::get_measurement(.) do nothing : elapsedTime %lu mSamplingTime %lu\n",elapsedTime,mSamplingTime);
		return false;
	}

	ahrsData.phi=attitude.phi+mNormalAngleNoiseGenerator->generate_noise(); // roll [deg]
	ahrsData.theta=attitude.theta+mNormalAngleNoiseGenerator->generate_noise(); // pitch [deg]
	ahrsData.psi=summod360(attitude.psi,mNormalAngleNoiseGenerator->generate_noise()); // yaw [deg]
	ahrsData.psi=summod360(ahrsData.psi,mPsiBias); // yaw [deg] - magnetic bias
	// angular velocity
	ahrsData.p=angVel.p+mNormalAngularRateNoiseGenerator->generate_noise(); // roll rate [deg/s]
	ahrsData.q=angVel.q+mNormalAngularRateNoiseGenerator->generate_noise(); // pitch rate [deg/s]
	ahrsData.r=angVel.r+mNormalAngularRateNoiseGenerator->generate_noise(); // yaw rate [deg/s]
	// magnetic components
	ahrsData.magnX=0.; // [Gauss] to be implemented
	ahrsData.magnY=0.; // [Gauss] to be implemented
	ahrsData.magnZ=0.; // [Gauss] to be implemented
	// linear velocity
	ahrsData.u=linVel.u+mNormalVelocityNoiseGenerator->generate_noise(); // [m/s]
	ahrsData.v=linVel.v+mNormalVelocityNoiseGenerator->generate_noise(); // [m/s]
	ahrsData.w=linVel.w+mNormalVelocityNoiseGenerator->generate_noise(); // [m/s]
	// linear accelerations
	if(!mInitialised) {
		m_uOld=ahrsData.u;
		m_vOld=ahrsData.v;
		mInitialised=true;
	}
	double du,dv;
	du=ahrsData.u-m_uOld;
	dv=ahrsData.v-m_vOld;
	ahrsData.uDot=du*1000./mSamplingTime; // [m/s^2]
	ahrsData.vDot=dv*1000./mSamplingTime; // [m/s^2]
	ahrsData.wDot=0.; // [m/s^2]
	m_uOld=ahrsData.u;m_vOld=ahrsData.v;
//	// rotation matrix
//	// coordinate transformation matrix: V_{IMU}_i = R_{i,j} · V_E_j
//	Matrix3x3 R;
//	phi_theta_psi_2_Rzyx(R,ahrsData.phi,ahrsData.theta,ahrsData.psi);
//	ahrsData.R00=R(0,0);
//	ahrsData.R01=R(0,1);
//	ahrsData.R02=R(0,2);
//	ahrsData.R10=R(1,0);
//	ahrsData.R11=R(1,1);
//	ahrsData.R12=R(1,2);
//	ahrsData.R20=R(2,0);
//	ahrsData.R21=R(2,1);
//	ahrsData.R22=R(2,2);
	return true;
}

void AhrsSim::set_parameters(double psiBias,double angleNoiseStd,double angularRateNoiseStd,double velocityNoiseStd,double accelerationNoiseStd)
{
    mPsiBias=psiBias;
    mAngleNoiseStd=angleNoiseStd;
    mAngularRateNoiseStd=angularRateNoiseStd;
    mVelocityNoiseStd=velocityNoiseStd;
    mAccelerationNoiseStd=accelerationNoiseStd;
	//
	// reset noise generators with new parameters
	//
	mNormalAngleNoiseGenerator->set_noise(0.,mAngleNoiseStd);
	mNormalAngularRateNoiseGenerator->set_noise(0.,mAngularRateNoiseStd);
	mNormalVelocityNoiseGenerator->set_noise(0.,mVelocityNoiseStd);
	mNormalAccelerationNoiseGenerator->set_noise(0.,mAccelerationNoiseStd);
}

bool AhrsSim::read_parameters(FILE *f) {
	char str[64];
	double psiBias,angleNoiseStd,angularRateNoiseStd,velocityNoiseStd,accelerationNoiseStd;

	//
	// read samplingTime [ms]
	//
	uint64_t samplingTime;
	fscanf(f,"%s %" SCNu64 "",str,&samplingTime);
    if(strcmp(str,"samplingTime_ms")!=0)
    {
        printf("AhrsSim::read_parameters samplingTime_ms expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    mSamplingTime=samplingTime;

	// read psiBias
	fscanf(f,"%s %lf",str,&psiBias);
    if(strcmp(str,"psiBias")!=0)
    {
        printf("AhrsSim::read_parameters(.) psiBias expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read angleNoiseStd
	fscanf(f,"%s %lf",str,&angleNoiseStd);
    if(strcmp(str,"angleNoiseStd")!=0)
    {
        printf("AhrsSim::read_parameters(.) angleNoiseStd expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read angularRateNoiseStd
	fscanf(f,"%s %lf",str,&angularRateNoiseStd);
    if(strcmp(str,"angularRateNoiseStd")!=0)
    {
        printf("AhrsSim::read_parameters(.) angularRateNoiseStd expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read velocityNoiseStd
	fscanf(f,"%s %lf",str,&velocityNoiseStd);
    if(strcmp(str,"velocityNoiseStd")!=0)
    {
        printf("AhrsSim::read_parameters(.) velocityNoiseStd expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read accelerationNoiseStd
	fscanf(f,"%s %lf",str,&accelerationNoiseStd);
    if(strcmp(str,"accelerationNoiseStd")!=0)
    {
        printf("AhrsSim::read_parameters(.) accelerationNoiseStd expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    //
    // set parameters and reset noise generators
    //
    set_parameters(psiBias,angleNoiseStd,angularRateNoiseStd,velocityNoiseStd,accelerationNoiseStd);

//    printf("AhrsSim::read_parameters(.) %lf %lf %lf %lf %lf\n",psiBias,angleNoiseStd,angularRateNoiseStd,velocityNoiseStd,accelerationNoiseStd);
	return true;
}



