//
//  AhrsSim.h
//  SWAMPsimulator
//
//  Created by Massimo Caccia on 22/02/22.
//

#ifndef AHRSSIM_H
#define AHRSSIM_H


#include "AhrsData.h"
#include "LinearPosition.h"
#include "AngularPosition.h"
#include "LinearVelocity.h"
#include "AngularVelocity.h"
#include "NormalNoiseGenerator.h"
#include "nVector.h"
#include "MathAngles.h"

#define AHRS_PSI_BIAS 0.0
#define AHRS_ANGLE_NOISE_STD 0.1
#define AHRS_ANGULAR_RATE_NOISE_STD 0.1
#define AHRS_VELOCITY_NOISE_STD 0.03
#define AHRS_ACCELERATION_NOISE_STD 0.01

//
// define standard AHRS sampling time [ms]
//
#define AHRS_SAMPLING_TIME 100

class AhrsSim {
public:
	AhrsSim(void);
    ~AhrsSim(void);

    bool get_measurement(AhrsData &ahrsData,
    		AngularPosition attitude,LinearVelocity linVel,AngularVelocity angVel,uint64_t elapsedTime);
    bool read_parameters(FILE *f);

private:
    void set_parameters(double psiBias,double angleNoiseStd,double angularRateNoiseStd,
    		double velocityNoiseStd,double accelerationNoiseStd);

private:
    uint64_t mSamplingTime;
    double mPsiBias; // heading bias in [deg]
    double mAngleNoiseStd; // yaw,pitch,roll angle noise std [deg]
    double mAngularRateNoiseStd;// yaw,pitch,roll rate noise std [deg/s]
    double mVelocityNoiseStd; // surge,sway,heave velocity noise std [m/s]
    double mAccelerationNoiseStd; // surge,sway,heave acceleration noise std [m/s^2]
    //
    // normal noise generators
    //
    NormalNoiseGenerator *mNormalAngleNoiseGenerator,*mNormalAngularRateNoiseGenerator;
    NormalNoiseGenerator *mNormalVelocityNoiseGenerator,*mNormalAccelerationNoiseGenerator;
    //
    // previous velocity, used for acceleration computation
    //
    double m_uOld, m_vOld, m_wOld;
    bool mInitialised;
};


#endif /* AHRSSIM_H */
