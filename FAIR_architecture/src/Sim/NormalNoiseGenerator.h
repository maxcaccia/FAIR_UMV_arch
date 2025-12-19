//
//  normalNoiseGenerator.h
//  SWAMPsimulator
//
//  Created by Massimo Caccia on 22/02/22.
//

#ifndef NormalNoiseGenerator_h
#define NormalNoiseGenerator_h

#include <random>

class NormalNoiseGenerator {
public:
    NormalNoiseGenerator(double noiseMean,double noiseStd);
    ~NormalNoiseGenerator(void) {};
    
    double generate_noise(void);
    void set_noise(double noiseMean,double noiseStd);
    
protected:
    double mNoiseMean;
    double mNoiseStd;

    std::random_device m_rnd{};
    std::mt19937 gen{m_rnd()};

    std::normal_distribution<> *m_d;
};


#endif /* NormalNoiseGenerator_h */
