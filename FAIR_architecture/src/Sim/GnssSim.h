//
//  GnssSim.h
//  GNSS simulator
//
//  Created by Massimo Caccia on 22/02/22.
//

#ifndef GNSSSIM_H
#define GNSSSIM_H

#include "GnssPositionData.h"
#include "GnssVelocityData.h"
#include "LinearPosition.h"
#include "AngularPosition.h"
#include "LinearVelocity.h"
#include "AngularVelocity.h"
#include "NormalNoiseGenerator.h"

//
// define standard GPS measurement errors
//
#define GNSS_HOR_NOISE_MEAN 0.
#define GNSS_HOR_NOISE_STD 0.5
#define GNSS_VER_NOISE_MEAN 0.
#define GNSS_VER_NOISE_STD 1.5

//
// define standard GNSS sampling time [ms]
//
#define GNSS_SAMPLING_TIME 1000

class GnssSim {
public:
	GnssSim(void);
    ~GnssSim(void);

    void get_measurement(GnssPositionData &gnssPosData,GnssVelocityData &gnssVelData,
    		LinearPosition position,LinearVelocity linVel,uint64_t elapsedTime);
    bool read_parameters(FILE *f);

private:
    void set_parameters(double noiseHorMean,double noiseHorStd,double noiseVerMean,double noiseVerStd);

private:
    uint64_t mSamplingTime;
    uint64_t msOffset;
//    uint64_t m_elapsed_time;
    double mNoiseHorMean,mNoiseHorStd; // GPS horizontal noise in [m]
    double mNoiseVerMean,mNoiseVerStd; // GPS vertical noise in [m]
    //
    // normal noise generators
    //
    NormalNoiseGenerator *mNormalHorNoiseGenerator,*mNormalVerNoiseGenerator;
    //
    // previous position, used for speed computation
    //
    double mLatitudeOld, mLongitudeOld;
    bool mInitialised;

    GnssPositionData mGnssPosData;
    GnssVelocityData mGnssVelData;

public:
    string mTalkerId; // GP : GPS ; GL : GLONASS ; GN : GPS&GLONASS
};


#endif /* GNSSIM_H */
