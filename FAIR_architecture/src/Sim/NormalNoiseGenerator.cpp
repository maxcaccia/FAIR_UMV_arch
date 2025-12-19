//
//  NormalNoiseGenerator.cpp
//  GPSsimulator
//
//  Created by Massimo Caccia on 22/02/22.
//

#include <stdio.h>

#include "NormalNoiseGenerator.h"


NormalNoiseGenerator::NormalNoiseGenerator(double noiseMean,double noiseStd)
{
    mNoiseMean=noiseMean;
    mNoiseStd=noiseStd;
    m_d=new std::normal_distribution<>(mNoiseMean,mNoiseStd);
}


double NormalNoiseGenerator::generate_noise(void)
{
    double noise;
    noise=m_d->operator ()(gen);
    
    return noise;
}

void NormalNoiseGenerator::set_noise(double noiseMean,double noiseStd)
{
    mNoiseMean=noiseMean;
    mNoiseStd=noiseStd;
    if(m_d!=NULL) {
    	delete m_d;
    	m_d=new std::normal_distribution<>(mNoiseMean,mNoiseStd);
    }
}
