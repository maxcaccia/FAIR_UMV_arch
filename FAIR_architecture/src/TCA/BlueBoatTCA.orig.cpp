/*
 * BlueBoatTCA.cpp
 *
 *  Created on: 18 June 2025
 *      Author: massimo
 */

#include "BlueBoatTCA.h"
#include "MathAngles.h"
#include "BasicMath.h"

BlueBoatTCA::BlueBoatTCA(void)
:TCA()
{
	mBlueBoatRpmRef=NULL;
	mBlueBoatRpmMeas=NULL;

//	mTCAstate->mMode = TCA_MANUAL;
//	printf("BlueBoatTCA::SwampTCA mMode %s\n",mTCAstate->mMode.c_str());fflush(stdout);

//	mTCAstate->mMode = TCA_MANUAL;
//	//
//	// zeroes force/torque references
//	//
//	mForceMomentRef->reset();
//	//
//	// set default AUTO mode
//	//
//	set_auto_default();

}

BlueBoatTCA::~BlueBoatTCA(void) {
	TCA::~TCA();
	if(mBlueBoatRpmRef!=NULL) {
		delete mBlueBoatRpmRef;
		mBlueBoatRpmRef=NULL;
	}
	if(mBlueBoatRpmMeas!=NULL) {
		delete mBlueBoatRpmMeas;
		mBlueBoatRpmMeas=NULL;
	}
}

void BlueBoatTCA::set_input_output(NormalisedRpm* blueBoatRpmRef,NormalisedRpm* blueBoatRpmMeas) {
	mBlueBoatRpmRef=blueBoatRpmRef;
	mBlueBoatRpmMeas=blueBoatRpmMeas;
	BlueBoatTCA::reset();
}

void BlueBoatTCA::set_manual_default(void) {
	//
	// set default MANUAL mode
	//
	mTCAstate->mMode="manual";
	mTCAstate->mFunction="null";
	mTCAstate->mConfig="null";
}

void BlueBoatTCA::set_auto_default(void) {
	//
	// set default AUTO mode
	//
	mTCAstate->mMode="auto";
	mTCAstate->mFunction="null";
	mTCAstate->mConfig="null";
}


void BlueBoatTCA::reset(void) {
//printf("BlueBoatTCA::reset %llu\n",(unsigned long long)mSwampActuationRef);fflush(stdout);
	TCA::reset();
	mBlueBoatRpmRef->reset();
}


void BlueBoatTCA::set_mode(string  str) {
//printf("BlueBoatTCA::set_mode\n");fflush(stdout);
	TCA::set_mode(str);
	if(mTCAstate->mMode=="manual") {
		//
		// zeroes force/torque references
		//
		mForceMomentRef->reset();
		//
		// zeroes BlueBoat references
		//
		mBlueBoatRpmRef->reset();
		//
		// set default MANUAL mode
		//
		set_manual_default();
//		mTCAstate->mFunction="forward_steering";
//		mTCAstate->mConfig="all";
	}
	else if(mTCAstate->mMode=="auto") {
		//
		// zeroes force/torque references
		//
		mForceMomentRef->reset();
		//
		// set default AUTO mode
		//
		set_auto_default();
//		mTCAstate->mFunction="steering";
//		mTCAstate->mConfig="all";
	}
	else {
		//
		// zeroes force/torque references
		//
		mForceMomentRef->reset();
		//
		// zeroes BlueBoat references
		//
		mBlueBoatRpmRef->reset();
		//
		// set default MANUAL mode
		//
		set_manual_default();
		//
		// printf error message
		//
		printf("BlueBoatTCA::set_mode - unexpected mode %s\n",str.c_str());
		fflush(stdout);
	}
}


void BlueBoatTCA::set_left_right_rpm(double nL,double nR) {
	mBlueBoatRpmRef->mLeftRpm = nL;
	mBlueBoatRpmRef->mRightRpm = nR;
}


void BlueBoatTCA::compute_applied_force_torque(void) {
//	printf("BlueBoatTCA::compute_applied_force_torque : to be implemented!\n");
	fflush(stdout);
	double TL,TR;

	//
	// compute TL,TR
	//
	TL = compute_propeller_thrust(K_THRUST_BLUEBOAT,mBlueBoatRpmMeas->mLeftRpm/100.);
	TR = compute_propeller_thrust(K_THRUST_BLUEBOAT,mBlueBoatRpmMeas->mRightRpm/100.);

	double f=TL+TR;
	if(f>=0.)
		mForceMomentHat->X=100.*f/2.;
	else
		mForceMomentHat->X=100.*f/(2.*K_THRUST_BLUEBOAT);

	mForceMomentHat->N=100*(TL-TR)/(2.*K_THRUST_BLUEBOAT);
}

void BlueBoatTCA::execute_manual(void) {

}


void BlueBoatTCA::execute_auto(void) {
	TCA_BlueBoat();
}


bool BlueBoatTCA::read_lookup_tables(void) {
	string filename;
	filename=mName+"LeftLookupTable.txt";
	bool flag;
	flag=load_lookup_table(filename.c_str(), mLeftLookupTable);
	if(!flag) {
		printf("BlueBoatTCA::read_lookup_tables Error loading file %s\n",filename.c_str());fflush(stdout);
		return false;
	}
	filename=mName+"RightLookupTable.txt";
	flag=load_lookup_table(filename.c_str(), mRightLookupTable);
	if(!flag) {
		printf("BlueBoatTCA::read_lookup_tables Error loading file %s\n",filename.c_str());fflush(stdout);
		return false;
	}
	return true;
}


bool BlueBoatTCA::load_lookup_table(const char *filename, double table[TABLE_SIZE][TABLE_SIZE]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("BlueBoatTCA::load_lookup_table Error opening file\n");fflush(stdout);
        return false;
    }

    for (int i = 0; i < TABLE_SIZE; ++i) {
        for (int j = 0; j < TABLE_SIZE; ++j) {
            if (fscanf(file, "%lf", &table[i][j]) != 1) {
            	printf("BlueBoatTCA::load_lookup_table Error reading value at [%d][%d]\n", i, j);
                fflush(stdout);
                fclose(file);
                return false;
            }
        }
    }

    fclose(file);
    return true;
}


void BlueBoatTCA::TCA_BlueBoat(void) {
	double Xref = sat(mForceMomentRef->X,-100.,100.);
	double NtildeRef = sat(mForceMomentRef->N,-100.,100.);

	// Compute base indices and interpolation weights
	int Xref0 = (int)floor(Xref);
	int NtildeRef0 = (int)floor(NtildeRef);
	double dXref = Xref - Xref0;
	double dNtildeRef = NtildeRef - NtildeRef0;

	int i = (int)floor(Xref + 100);
	int j = (int)floor(NtildeRef + 100);
	int iNext = (i + 1 < TABLE_SIZE) ? i + 1 : TABLE_SIZE - 1;
	int jNext = (j + 1 < TABLE_SIZE) ? j + 1 : TABLE_SIZE - 1;

	// Interpolate nL
	double z00 = mLeftLookupTable[i][j];
	double z01 = mLeftLookupTable[i][jNext];
	double z10 = mLeftLookupTable[iNext][j];
	double z11 = mLeftLookupTable[iNext][jNext];
	mBlueBoatRpmRef->mLeftRpm = bilinear_interpolation(z00, z01, z10, z11, dXref, dNtildeRef);

	// Interpolate nR
	z00 = mRightLookupTable[i][j];
	z01 = mRightLookupTable[i][jNext];
	z10 = mRightLookupTable[iNext][j];
	z11 = mRightLookupTable[iNext][jNext];
	mBlueBoatRpmRef->mRightRpm = bilinear_interpolation(z00, z01, z10, z11, dXref, dNtildeRef);
}


