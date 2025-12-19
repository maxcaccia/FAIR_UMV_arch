//
//  PositionMotorSim.cpp
//  Position Mogtor simulator
//
//  Created by Massimo Caccia on 15/03/23.
//

#include <stdio.h>

#include "PositionMotorSim.h"

PositionMotorSim::PositionMotorSim(void)
{
	mSamplingTime=POSITION_MOTOR_SAMPLING_TIME;

    mPower=false;
    mEnable=false;
    mPosition=0; // to be modified introducing random value
    mTemperature=POSITION_MOTOR_TEMPERATURE;

    mPositionRef=0;
    mConfigurationStatus=0;
    mMode=POSITION_MOTOR_GOTO;

    mMaxSpeedRpm=POSITION_MOTOR_MAX_RPM;
    mMaxSpeed=mMaxSpeedRpm*POSITION_MOTOR_TICKS_PER_MOTOR_REV/60; // [tick/s]
    mRpm2current=POSITION_RPM2CURRENT;

    mNoisePositionTickMean=POSITION_MOTOR_TICK_NOISE_MEAN;
    mNoisePositionTickStd=POSITION_MOTOR_TICK_NOISE_STD;
    mNoiseCurrentMean=POSITION_MOTOR_CURRENT_NOISE_MEAN;
    mNoiseCurrentStd=POSITION_MOTOR_CURRENT_NOISE_STD;
    mNoiseTemperatureMean=POSITION_MOTOR_TEMPERATURE_NOISE_MEAN;
    mNoiseTemperatureStd=POSITION_MOTOR_TEMPERATURE_NOISE_STD;

	//
	// instantiate noise generators
	//
    mNormalPositionNoiseGenerator = new NormalNoiseGenerator(mNoisePositionTickMean,mNoisePositionTickStd);
    mNormalCurrentNoiseGenerator = new NormalNoiseGenerator(mNoiseCurrentMean,mNoiseCurrentStd);
    mNormalTemperatureNoiseGenerator = new NormalNoiseGenerator(mNoiseTemperatureMean,mNoiseTemperatureStd);

    //
    // initialise memory
    //
    for(int i=0;i<N_POS_MOTOR_ARMA_PAR;i++)
    	mPosARMA[i]=mPosRefARMA[i]=0.;
}

PositionMotorSim::~PositionMotorSim(void) {
	if(mNormalPositionNoiseGenerator) delete mNormalPositionNoiseGenerator;
	if(mNormalCurrentNoiseGenerator) delete mNormalCurrentNoiseGenerator;
	if(mNormalTemperatureNoiseGenerator) delete mNormalTemperatureNoiseGenerator;
};

void PositionMotorSim::set_power(bool value)
{
    mPower=value;
    if(!mPower) {
    	mEnable=false;
    }
}

void PositionMotorSim::set_enable(bool value)
{
    if(mPower) {
    	mEnable=value;

    	if(mEnable)
    		mConfigurationStatus = mConfigurationStatus | 0x0400;
    	else
    		mConfigurationStatus = mConfigurationStatus & (~0x0400);
    }
//    if(!mEnable)
//    	mSpeed=0.;
}

void PositionMotorSim::set_speed_rpm(double speedRpmRef)
{
    if (!mEnable)
        return;
    mMaxSpeedRpm=speedRpmRef;
    mMaxSpeed=mMaxSpeedRpm*POSITION_MOTOR_TICKS_PER_MOTOR_REV/60; // [tick/s]
}

void PositionMotorSim::set_position(int32_t positionRef) {
	if(mEnable)
		mPositionRef=positionRef;
}

void PositionMotorSim::set_home(void)
{
    if (!mEnable)
        return;
    mPosition=0;
    mPositionRef=0;
    mMode=POSITION_MOTOR_GOTO;
}

void PositionMotorSim::homing(void)
{
    if (!mEnable)
        return;
    mMode=POSITION_MOTOR_HOMING;
    int32_t nRevolutions;
    if(mPosition>0)
    	nRevolutions=int32_t(mPosition/TICKS_PER_360DEG_MOTOR_REV);
    else
    	nRevolutions=int32_t(mPosition/TICKS_PER_360DEG_MOTOR_REV)-1;
    mPositionRef=int32_t((nRevolutions+1)*TICKS_PER_360DEG_MOTOR_REV);
}

void PositionMotorSim::get_measurement(PositionMotorData &positionMotorData,uint64_t elapsedTime)
{
	if((elapsedTime%mSamplingTime)!=0) {
//		printf("PositionMotorSim::get_measurement(.) do nothing : elapsedTime %lu mSamplingTime %lu\n",elapsedTime,mSamplingTime);
		return;
	}
	if(!(mPower&&mEnable))
		return;
//	printf("PositionMotorSim::get_measurement(.) : elapsedTime %lu mSamplingTime %lu\n",elapsedTime,mSamplingTime);
	//
	// position, current and temperature data
	//
    double positionNoise,currentNoise,temperatureNoise;
    positionNoise=mNormalPositionNoiseGenerator->generate_noise();
    currentNoise=mNormalCurrentNoiseGenerator->generate_noise();
    temperatureNoise=mNormalTemperatureNoiseGenerator->generate_noise();
//printf("PositionMotorSim::get_measurement(.) : elapsedTime %lu mSamplingTime %lu positionNoise [ticks] %.2lf currentNoise [V] %.2lf temperatureNoise [C] %.2lf\n",
//		elapsedTime,mSamplingTime,positionNoise,currentNoise,temperatureNoise);
	positionMotorData.position=mPosition+int32_t(positionNoise);
 	double current=0.; //mRpm2current*mMaxSpeedRpm*mMaxSpeedRpm; // WRONG! current is proportional to the square of actual speed not maximum speed
 	positionMotorData.current=current+currentNoise;
 	positionMotorData.temperature=mTemperature+temperatureNoise;

// 	printf("PositionMotorSim::get_measurement(.) %" PRId32 " %" PRId32 "\n",mPositionRef,mPosition);fflush(stdout);
}

double PositionMotorSim::get_angle(void) {
    double angle;
    angle=(mPosition % int(TICKS_PER_360DEG_MOTOR_REV))*(360./TICKS_PER_360DEG_MOTOR_REV);
    angle=mod360(angle);
    return angle;
}


void PositionMotorSim::set_parameters(double maxSpeedRpm,double rpm2current,
		double noisePositionTickMean,double noisePositionTickStd,
		double noiseCurrentMean,double noiseCurrentStd,
		double noiseTemperatureMean,double noiseTemperatureStd)
{
	mMaxSpeedRpm=maxSpeedRpm;
    mMaxSpeed=mMaxSpeedRpm*POSITION_MOTOR_TICKS_PER_MOTOR_REV/60; // [tick/s]

	mRpm2current=rpm2current;

	mNoisePositionTickMean=noisePositionTickMean;
	mNoisePositionTickStd=noisePositionTickStd;
    mNoiseCurrentMean=noiseCurrentMean;
    mNoiseCurrentStd=noiseCurrentStd;
    mNoiseTemperatureMean=noiseTemperatureMean;
    mNoiseTemperatureStd=noiseTemperatureStd;

	//
	// reset noise generators with new parameters
	//
    mNormalPositionNoiseGenerator->set_noise(mNoisePositionTickMean,mNoisePositionTickStd);
    mNormalCurrentNoiseGenerator->set_noise(mNoiseCurrentMean,mNoiseCurrentStd);
    mNormalTemperatureNoiseGenerator->set_noise(mNoiseTemperatureMean,mNoiseTemperatureStd);
}

bool PositionMotorSim::read_parameters(FILE *f) {
	char str[64];
	double rpm2current,maxSpeedRpm;
	double noisePositionTickMean,noisePositionTickStd;
	double noiseCurrentMean,noiseCurrentStd;
	double noiseTemperatureMean,noiseTemperatureStd;

	//
	// read samplingTime [ms]
	//
	uint64_t samplingTime;
	fscanf(f,"%s %" SCNu64 "",str,&samplingTime);
    if(strcmp(str,"samplingTime_ms")!=0)
    {
        printf("PositionMotorSim::read_parameters samplingTime_ms expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    mSamplingTime=samplingTime;

	// read maxSpeedRpm
	fscanf(f,"%s %lf",str,&maxSpeedRpm);
    if(strcmp(str,"maxSpeedRpm")!=0)
    {
        printf("PositionMotorSim::read_parameters(.) maxSpeedRpm expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read rpm2current
	fscanf(f,"%s %lf",str,&rpm2current);
    if(strcmp(str,"rpm2current")!=0)
    {
        printf("PositionMotorSim::read_parameters(.) rpm2current expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read noisePositionTickMean
	fscanf(f,"%s %lf",str,&noisePositionTickMean);
    if(strcmp(str,"noisePositionTickMean")!=0)
    {
        printf("PositionMotorSim::read_parameters(.) noisePositionTickMean expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read noisePositionTickStd
	fscanf(f,"%s %lf",str,&noisePositionTickStd);
    if(strcmp(str,"noisePositionTickStd")!=0)
    {
        printf("PositionMotorSim::read_parameters(.) noisePositionTickStd expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read noiseCurrentMean
	fscanf(f,"%s %lf",str,&noiseCurrentMean);
    if(strcmp(str,"noiseCurrentMean")!=0)
    {
        printf("PositionMotorSim::read_parameters(.) noiseCurrentMean expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read noiseCurrentStd
	fscanf(f,"%s %lf",str,&noiseCurrentStd);
    if(strcmp(str,"noiseCurrentStd")!=0)
    {
        printf("PositionMotorSim::read_parameters(.) noiseCurrentStd expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read noiseTemperatureMean
	fscanf(f,"%s %lf",str,&noiseTemperatureMean);
    if(strcmp(str,"noiseTemperatureMean")!=0)
    {
        printf("PositionMotorSim::read_parameters(.) noiseTemperatureMean expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// read noiseTemperatureStd
	fscanf(f,"%s %lf",str,&noiseTemperatureStd);
    if(strcmp(str,"noiseTemperatureStd")!=0)
    {
        printf("PositionMotorSim::read_parameters(.) noiseTemperatureStd expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    //
    // set parameters and reset noise generators
    //
    set_parameters(maxSpeedRpm,rpm2current,
    		noisePositionTickMean,noisePositionTickStd,
			noiseCurrentMean,noiseCurrentStd,
			noiseTemperatureMean,noiseTemperatureStd);

//    printf("PositionMotorSim::read_parameters(.) %lf %lf %lf\n",rpm2thrust,rpm2current,tau);
	return true;
}

void PositionMotorSim::execute_step(uint64_t elapsedTime) {
	//
	// Note: commented trace printf has already been successfully checked!
	//
//	if((elapsedTime%mSamplingTime)!=0) {
//	//		printf("PositionMotorSim::get_measurement(.) do nothing : elapsedTime %lu mSamplingTime %lu\n",elapsedTime,mSamplingTime);
//			return;
//		}
//	// shift memory
//	for(int i=1;i<(N_POS_MOTOR_ARMA_PAR-1);i++) {
//		mPosARMA[N_POS_MOTOR_ARMA_PAR-i]=mPosARMA[N_POS_MOTOR_ARMA_PAR-i-1];
//		mPosRefARMA[N_POS_MOTOR_ARMA_PAR-i]=mPosRefARMA[N_POS_MOTOR_ARMA_PAR-i-1];
//	}
//	mPosRefARMA[0]=mPositionRef;
//	// compute actual position
//	mPosition=0.;
//	for(int i=0;i<N_POS_MOTOR_ARMA_PAR;i++)
//		mPosition+=(mARMA_b[i]*mPosRefARMA[i]-mARMA_a[i]*mPosARMA[i]);
//	mPosARMA[0]=mPosition;

	double dt=mSamplingTime/1000.;

	if(mEnable) {
	    int32_t delta=int32_t(mMaxSpeed*dt);
//		printf("mMaxSpeed %.2lf mMaxSpeedRpm %.2lf delta %" PRId32 " mPositionRef %" PRId32 " mPosition %" PRId32 "\n",
//				mMaxSpeed,mMaxSpeedRpm,delta,mPositionRef,mPosition);

	    if(abs(mPositionRef-mPosition)<=delta)
	    // azimuth reaches target value
	    {
	        mPosition=mPositionRef;
	        if(mMode==POSITION_MOTOR_HOMING)
	        {
	            mPosition=0;
	            mPositionRef=0;
	            mMode=POSITION_MOTOR_GOTO;
	        }
	    }
	    else if (mPositionRef>mPosition)
	        mPosition+=delta;
	    else
	        mPosition-=delta;
	}
//	printf("PositionMotorSim::execute_step dt %lf power %hu enable %hu position[tick] %" PRId32 " position[deg] %.2lf\n",
//			dt,mPower,mEnable,mPosition,get_angle());
}
