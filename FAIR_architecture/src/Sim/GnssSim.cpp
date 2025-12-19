//
//  gnssSim.cpp
//  GNSSsimulator
//
//  Created by Massimo Caccia on 22/02/22.
//

#include <stdio.h>

#include "GnssSim.h"
#include "nVector.h"


GnssSim::GnssSim(void)
{
	mSamplingTime=GNSS_SAMPLING_TIME;
	msOffset=0xffffffffffffffff;

	mNoiseHorMean=GNSS_HOR_NOISE_MEAN;
	mNoiseHorStd=GNSS_HOR_NOISE_STD;
	mNoiseVerMean=GNSS_VER_NOISE_MEAN;
	mNoiseVerStd=GNSS_VER_NOISE_MEAN;
	//
	// instantiate noise generators
	//
	mNormalHorNoiseGenerator = new NormalNoiseGenerator(mNoiseHorMean,mNoiseHorStd);
	mNormalVerNoiseGenerator = new NormalNoiseGenerator(mNoiseVerMean,mNoiseVerStd);

	mLatitudeOld=mLongitudeOld=0.;
	mInitialised=false;

	mTalkerId="GP";
}

GnssSim::~GnssSim(void) {
	if(mNormalHorNoiseGenerator) delete mNormalHorNoiseGenerator;
	if(mNormalVerNoiseGenerator) delete mNormalVerNoiseGenerator;
};

void GnssSim::get_measurement(GnssPositionData &gnssPosData,GnssVelocityData &gnssVelData,
		LinearPosition position,LinearVelocity linVel,uint64_t elapsedTime)
//void GnssSim::get_measurement(GPSdata &gpsData,Kinematics dev,uint64_t elapsedTime)
{
    time_t t;
    struct tm *tmp;
    t = time(NULL);
    tmp = gmtime(&t);
    //
    // determine clock and elapsedTime offset
    //
    static int previousSec=tmp->tm_sec;

//    printf("============================= GpsSim::get_measurement(.) %d %d %ld %lu\n",previousSec,tmp->tm_sec,elapsedTime,msOffset);
//    printf("============================= GpsSim::get_measurement(.) %d %d %ld %lu %lu\n",previousSec,tmp->tm_sec,elapsedTime,msOffset,elapsedTime-msOffset);
	if((elapsedTime%mSamplingTime)!=0) {
//		printf("GpsSim::get_measurement(.) do nothing : elapsedTime %lu mSamplingTime %lu\n",elapsedTime,mSamplingTime);
		return;
	}
    if((previousSec != tmp->tm_sec)&&(msOffset == 0xffffffffffffffff))
    	msOffset=elapsedTime;

	//
	// GPS date and time
	//

    gnssPosData.date = (1900+tmp->tm_year)*10000 + (tmp->tm_mon+1)*100 + tmp->tm_mday; // yyyymmdd : date = 10000*yyyy + 100*mm + dd
//    gpsData.date = (tmp->tm_year % 100) + (tmp->tm_mon+1)*100 + 10000*tmp->tm_mday; // yyyymmdd : date = 10000*yyyy + 100*mm + dd
    gnssPosData.time = tmp->tm_hour*10000.+tmp->tm_min*100.+tmp->tm_sec+((elapsedTime-msOffset)%1000)/1000.; // hhmmss.ss in UTC
	// latitude, longitude
    double xNoise,yNoise;
    xNoise=mNormalHorNoiseGenerator->generate_noise();
    yNoise=mNormalHorNoiseGenerator->generate_noise();
//printf("GnssSim::get_measurement(.) : elapsedTime %lu mSamplingTime %lu noise [m] %.8lf %.8lf\n",elapsedTime,mSamplingTime,xNoise,yNoise);
    lat_lon_plus_x_y(gnssPosData.latitude,gnssPosData.longitude,position.latitude,position.longitude,xNoise,yNoise);
    if(!mInitialised) {
    	mLatitudeOld=gnssPosData.latitude;
    	mLongitudeOld=gnssPosData.longitude;
    	mInitialised=true;
    }
	// GPS height
    gnssPosData.heightAboveEllipsoid=0; // [m]  // to be implemented
    gnssPosData.heightAboveMSL=0; // [m]  // to be implemented
	// NED velocity
	//
	// evaluate if a filtered signal is required
	//
	double dx,dy,xDot,yDot;
	lat_lon_2_x_y(dx,dy,mLatitudeOld,mLongitudeOld,gnssPosData.latitude,gnssPosData.longitude);
	mLatitudeOld=gnssPosData.latitude;
	mLongitudeOld=gnssPosData.longitude;
	xDot=dx*1000./mSamplingTime; // North velocity [m/s]
	yDot=dy*1000./mSamplingTime; // East velocity [m/s]
	// speed and track
	gnssVelData.speed=sqrt(xDot*xDot+yDot*yDot); // [m/s]
	gnssVelData.track=rad2deg(atan2(dy,dx)); // [deg]
}


void GnssSim::set_parameters(double noiseHorMean,double noiseHorStd,double noiseVerMean,double noiseVerStd) {
	mNoiseHorMean=noiseHorMean;
	mNoiseHorStd=noiseHorStd;
	mNoiseVerMean=noiseVerMean;
	mNoiseVerStd=noiseVerStd;
	//
	// reset noise generators with new parameters
	//
	mNormalHorNoiseGenerator->set_noise(mNoiseHorMean,mNoiseHorStd);
	mNormalVerNoiseGenerator->set_noise(mNoiseVerMean,mNoiseVerStd);
}

bool GnssSim::read_parameters(FILE *f) {
	char str[64];
	double noiseHorMean,noiseHorStd,noiseVerMean,noiseVerStd;

	//
	// read samplingTime [ms]
	//
	uint64_t samplingTime;
	fscanf(f,"%s %" SCNu64 "",str,&samplingTime);
    if(strcmp(str,"samplingTime_ms")!=0)
    {
        printf("GnssSim::read_parameters samplingTime_ms expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    mSamplingTime=samplingTime;

	// read noiseHorMean
	fscanf(f,"%s %lf",str,&noiseHorMean);
    if(strcmp(str,"NoiseHorMean")!=0)
    {
        printf("GnssSim::read_parameters(.) NoiseHorMean expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read noiseHorStd
	fscanf(f,"%s %lf",str,&noiseHorStd);
    if(strcmp(str,"NoiseHorStd")!=0)
    {
        printf("GnssSim::read_parameters(.) NoiseHorStd expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read noiseVerMean
	fscanf(f,"%s %lf",str,&noiseVerMean);
    if(strcmp(str,"NoiseVerMean")!=0)
    {
        printf("GnssSim::read_parameters(.) NoiseVerMean expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read noiseVerStd
	fscanf(f,"%s %lf",str,&noiseVerStd);
    if(strcmp(str,"NoiseVerStd")!=0)
    {
        printf("GnssSim::read_parameters(.) NoiseVerStd expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read talketId
    char tId[8];
	fscanf(f,"%s %s",str,tId);
    if(strcmp(str,"TalkerId")!=0)
    {
        printf("GnssSim::read_parameters(.) TalkerId expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }

    mTalkerId=tId;
    //
    // set parameters and reset noise generators
    //
    set_parameters(noiseHorMean,noiseHorStd,noiseVerMean,noiseVerStd);

//    printf("GpsSim::read_parameters(.) %lf %lf %lf %lf\n",noiseHorMean,noiseHorStd,noiseVerMean,noiseVerStd);
	return true;
}



