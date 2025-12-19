//
//  SpeedMotorSim.cpp
//  Speed Motor simulator
//
//  Created by Massimo Caccia on 02/03/23.
//

#include <stdio.h>

#include "SpeedMotorSim.h"

SpeedMotorSim::SpeedMotorSim(void)
{
	mSamplingTime=SPEED_MOTOR_SAMPLING_TIME;

    mPower=false;
    mEnable=false;
    mSpeedRef=0.;
    mSpeed=0.;
    mTemperature=SPEED_MOTOR_TEMPERATURE;

    mRpm2thrust2=SPEED_MOTOR_RPM2THRUST2;
    mRpm2thrust0=SPEED_MOTOR_RPM2THRUST0;
    mRpm2current2=SPEED_MOTOR_RPM2CURRENT2;
    mRpm2current0=SPEED_MOTOR_RPM2CURRENT0;

    mNoiseRpmMean=SPEED_MOTOR_RPM_NOISE_MEAN;
    mNoiseRpmStd=SPEED_MOTOR_RPM_NOISE_STD;
    mNoiseCurrentMean=SPEED_MOTOR_CURRENT_NOISE_MEAN;
    mNoiseCurrentStd=SPEED_MOTOR_CURRENT_NOISE_STD;
    mNoiseTemperatureMean=SPEED_MOTOR_TEMPERATURE_NOISE_MEAN;
    mNoiseTemperatureStd=SPEED_MOTOR_TEMPERATURE_NOISE_STD;

	//
	// instantiate noise generators
	//
    mNormalRpmNoiseGenerator = new NormalNoiseGenerator(mNoiseRpmMean,mNoiseRpmStd);
    mNormalCurrentNoiseGenerator = new NormalNoiseGenerator(mNoiseCurrentMean,mNoiseCurrentStd);
    mNormalTemperatureNoiseGenerator = new NormalNoiseGenerator(mNoiseTemperatureMean,mNoiseTemperatureStd);

    //
    // initialise memory
    //
    for(int i=0;i<N_SPEED_MOTOR_ARMA_PAR;i++)
    	mSpeedARMA[i]=mSpeedRefARMA[i]=0.;
}

SpeedMotorSim::~SpeedMotorSim(void) {
	if(mNormalRpmNoiseGenerator) delete mNormalRpmNoiseGenerator;
	if(mNormalCurrentNoiseGenerator) delete mNormalCurrentNoiseGenerator;
	if(mNormalTemperatureNoiseGenerator) delete mNormalTemperatureNoiseGenerator;
};

void SpeedMotorSim::set_power(bool value)
{
    mPower=value;
    if(!mPower) {
    	mEnable=false;
    	mSpeed=0.;
    	mSpeedRef=0.;
    }
}

void SpeedMotorSim::set_enable(bool value)
{
    if(mPower)
    	mEnable=value;
    if(!mEnable)
    	mSpeedRef=mSpeed=0.;
}

void SpeedMotorSim::set_speed(double speedRef)
{
	if(mEnable)
		mSpeedRef=speedRef;
	//printf("SpeedMotorSim::set_speed %lf\n",mSpeedRef);
}

void SpeedMotorSim::get_measurement(SpeedMotorData &speedMotorData,uint64_t elapsedTime)
{
	if((elapsedTime%mSamplingTime)!=0) {
//		printf("SpeedMotorSim::get_measurement(.) do nothing : elapsedTime %lu mSamplingTime %lu\n",elapsedTime,mSamplingTime);
		return;
	}
	if(!(mPower&&mEnable))
		return;
//	printf("SpeedMotorSim::get_measurement(.) : elapsedTime %lu mSamplingTime %lu\n",elapsedTime,mSamplingTime);
	//
	// speed, current and temperature data
	//
    double speedNoise,currentNoise,temperatureNoise;
    speedNoise=mNormalRpmNoiseGenerator->generate_noise();
    currentNoise=mNormalCurrentNoiseGenerator->generate_noise();
    temperatureNoise=mNormalTemperatureNoiseGenerator->generate_noise();
//printf("SpeedMotorSim::get_measurement(.) : elapsedTime %lu mSamplingTime %lu speedNoise [rpm] %.2lf currentNoise [V] %.2lf temperatureNoise [C] %.2lf\n",
//		elapsedTime,mSamplingTime,speedNoise,currentNoise,temperatureNoise);
 	speedMotorData.speed=mSpeed+speedNoise;
 	double current=mRpm2current2*mSpeed*mSpeed+mRpm2current0;
 	//
 	// WARNING: Minion current is acquired diviuded by a factor 2
 	//
 	speedMotorData.current=(current+currentNoise)/2.;
 	speedMotorData.temperature=mTemperature+temperatureNoise;

// 	printf("SpeedMotorSim::get_measurement(.) %.2lf\n",speedMotorData.speed);
}

double SpeedMotorSim::get_thrust(void) {
	return (mRpm2thrust2*mSpeed*mSpeed+mRpm2thrust0);
}


void SpeedMotorSim::set_parameters(double rpm2thrust2,double rpm2thrust0,double rpm2current2,double rpm2current0,
		double noiseRpmMean,double noiseRpmStd,
		double noiseCurrentMean,double noiseCurrentStd,
		double noiseTemperatureMean,double noiseTemperatureStd)
{
	mRpm2thrust2=rpm2thrust2;
	mRpm2thrust0=rpm2thrust0;
	mRpm2current2=rpm2current0;
	mRpm2current2=rpm2current0;

    mNoiseRpmMean=noiseRpmMean;
    mNoiseRpmStd=noiseRpmStd;
    mNoiseCurrentMean=noiseCurrentMean;
    mNoiseCurrentStd=noiseCurrentStd;
    mNoiseTemperatureMean=noiseTemperatureMean;
    mNoiseTemperatureStd=noiseTemperatureStd;

	//
	// reset noise generators with new parameters
	//
    mNormalRpmNoiseGenerator->set_noise(mNoiseRpmMean,mNoiseRpmStd);
    mNormalCurrentNoiseGenerator->set_noise(mNoiseCurrentMean,mNoiseCurrentStd);
    mNormalTemperatureNoiseGenerator->set_noise(mNoiseTemperatureMean,mNoiseTemperatureStd);
}

bool SpeedMotorSim::read_parameters(FILE *f) {
	char str[64];
	double rpm2thrust2,rpm2thrust0,rpm2current2,rpm2current0;
	double noiseRpmMean,noiseRpmStd;
	double noiseCurrentMean,noiseCurrentStd;
	double noiseTemperatureMean,noiseTemperatureStd;
	double a[N_SPEED_MOTOR_ARMA_PAR],b[N_SPEED_MOTOR_ARMA_PAR];

	//
	// read samplingTime [ms]
	//
	uint64_t samplingTime;
	fscanf(f,"%s %" SCNu64 "",str,&samplingTime);
    if(strcmp(str,"samplingTime_ms")!=0)
    {
        printf("SpeedMotorSim::read_parameters samplingTime_ms expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    mSamplingTime=samplingTime;

	// read rpm2thrust2
	fscanf(f,"%s %lf",str,&rpm2thrust2);
    if(strcmp(str,"rpm2thrust2")!=0)
    {
        printf("SpeedMotorSim::read_parameters(.) rpm2thrust2 expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read rpm2thrust0
	fscanf(f,"%s %lf",str,&rpm2thrust0);
    if(strcmp(str,"rpm2thrust0")!=0)
    {
        printf("SpeedMotorSim::read_parameters(.) rpm2thrust0 expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // read rpm2current2
	fscanf(f,"%s %lf",str,&rpm2current2);
    if(strcmp(str,"rpm2current2")!=0)
    {
        printf("SpeedMotorSim::read_parameters(.) rpm2current2 expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // read rpm2current0
	fscanf(f,"%s %lf",str,&rpm2current0);
    if(strcmp(str,"rpm2current0")!=0)
    {
        printf("SpeedMotorSim::read_parameters(.) rpm2current0 expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    // read noiseRpmMean
	fscanf(f,"%s %lf",str,&noiseRpmMean);
    if(strcmp(str,"noiseRpmMean")!=0)
    {
        printf("SpeedMotorSim::read_parameters(.) noiseRpmMean expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read noiseRpmStd
	fscanf(f,"%s %lf",str,&noiseRpmStd);
    if(strcmp(str,"noiseRpmStd")!=0)
    {
        printf("SpeedMotorSim::read_parameters(.) noiseRpmStd expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read noiseCurrentMean
	fscanf(f,"%s %lf",str,&noiseCurrentMean);
    if(strcmp(str,"noiseCurrentMean")!=0)
    {
        printf("SpeedMotorSim::read_parameters(.) noiseCurrentMean expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read noiseCurrentStd
	fscanf(f,"%s %lf",str,&noiseCurrentStd);
    if(strcmp(str,"noiseCurrentStd")!=0)
    {
        printf("SpeedMotorSim::read_parameters(.) noiseCurrentStd expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read noiseTemperatureMean
	fscanf(f,"%s %lf",str,&noiseTemperatureMean);
    if(strcmp(str,"noiseTemperatureMean")!=0)
    {
        printf("SpeedMotorSim::read_parameters(.) noiseTemperatureMean expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read noiseTemperatureStd
	fscanf(f,"%s %lf",str,&noiseTemperatureStd);
    if(strcmp(str,"noiseTemperatureStd")!=0)
    {
        printf("SpeedMotorSim::read_parameters(.) noiseTemperatureStd expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read ARMA model
	fscanf(f,"%s",str);
    if(strcmp(str,"ARMAmodel")!=0)
    {
        printf("SpeedMotorSim::read_parameters(.) ARMAmodel expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    for(int i=0;i<N_SPEED_MOTOR_ARMA_PAR;i++)
    	fscanf(f,"%lf",&a[i]);
    for(int i=0;i<N_SPEED_MOTOR_ARMA_PAR;i++)
    	fscanf(f,"%lf",&b[i]);
    //
    // set parameters and reset noise generators
    //
    set_parameters(rpm2thrust2,rpm2thrust0,rpm2current2,rpm2current0,
    		noiseRpmMean,noiseRpmStd,
			noiseCurrentMean,noiseCurrentStd,
			noiseTemperatureMean,noiseTemperatureStd);
    set_ARMA_model_parameters(a,b);

//    printf("SpeedMotorSim::read_parameters(.) %lf %lf %lf\n",rpm2thrust,rpm2current,tau);
	return true;
}

void SpeedMotorSim::execute_step(uint64_t elapsedTime) {
	if((elapsedTime%mSamplingTime)!=0) {
	//		printf("SpeedMotorSim::execute_step(.) do nothing : elapsedTime %lu mSamplingTime %lu\n",elapsedTime,mSamplingTime);
			return;
		}
	// shift memory
	for(int i=1;i<(N_SPEED_MOTOR_ARMA_PAR);i++) {
		mSpeedARMA[N_SPEED_MOTOR_ARMA_PAR-i]=mSpeedARMA[N_SPEED_MOTOR_ARMA_PAR-i-1];
		mSpeedRefARMA[N_SPEED_MOTOR_ARMA_PAR-i]=mSpeedRefARMA[N_SPEED_MOTOR_ARMA_PAR-i-1];
	}
	mSpeedRefARMA[0]=mSpeedRef;
	// compute actual speed
	mSpeed=0.;
	for(int i=0;i<N_SPEED_MOTOR_ARMA_PAR;i++) {
		mSpeed+=(mARMA_b[i]*mSpeedRefARMA[i]+mARMA_a[i]*mSpeedARMA[i]);
	}
	mSpeedARMA[0]=mSpeed;
//printf("SpeedMotorSim::execute_step mSpeedRef %.2lf mSpeed %.2lf\n",mSpeedRef,mSpeed); fflush(stdout);
}
