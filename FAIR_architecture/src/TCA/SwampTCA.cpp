/*
 * SwampTCA.cpp
 *
 *  Created on: 5 mar 2024
 *      Author: massimo
 */

#include "SwampTCA.h"
#include "MathAngles.h"
#include "BasicMath.h"

SwampTCA::SwampTCA(void)
:TCA()
{
	mSwampActuationRef=NULL;
	mSwampActuationMeas=NULL;
//	printf("SwampTCA::SwampTCA begin %s\n",mName.c_str());fflush(stdout);
}

SwampTCA::~SwampTCA(void) {
	TCA::~TCA();
	if(mSwampActuationRef!=NULL) {
		delete mSwampActuationRef;
		mSwampActuationRef=NULL;
	}
	if(mSwampActuationMeas!=NULL) {
		delete mSwampActuationMeas;
		mSwampActuationMeas=NULL;
	}
}

void SwampTCA::set_input_output(SwampActuation* swampActuationRef,SwampActuation* swampActuationMeas) {
	mSwampActuationRef=swampActuationRef;
	mSwampActuationMeas=swampActuationMeas;
	SwampTCA::reset();
}

void SwampTCA::set_manual_default(void) {
	//
	// set default MANUAL mode
	//
	mTCAstate->mMode="manual";
	mTCAstate->mFunction="forward_steering";
	mTCAstate->mConfig="all";
}

void SwampTCA::set_auto_default(void) {
	//
	// set default AUTO mode
	//
	mTCAstate->mMode="auto";
	mTCAstate->mFunction="steering";
	mTCAstate->mConfig="all";
}


void SwampTCA::reset(void) {
//printf("SwampTCA::reset %llu\n",(unsigned long long)mSwampActuationRef);fflush(stdout);
	TCA::reset();
	mSwampActuationRef->reset();
}


void SwampTCA::set_mode(string  str) {
//printf("SwampTCA::set_mode\n");fflush(stdout);
	TCA::set_mode(str);
	if(mTCAstate->mMode=="manual") {
		//
		// zeroes force/torque references
		//
		mForceMomentRef->reset();
		//
		// check consistent state
		//
		if((mTCAstate->mFunction=="translation")&&(mTCAstate->mConfig=="parallel"))
			return;
		if((mTCAstate->mFunction=="forward_steering")||(mTCAstate->mFunction=="backward_steering")) {
			if((mTCAstate->mConfig=="all")||(mTCAstate->mConfig=="bow")||(mTCAstate->mConfig=="stern"))
				return;
		}
		//
		// zeroes SWAMP references
		//
		mSwampActuationRef->reset();
		//
		// set default MANUAL mode
		//
		set_manual_default();
//		mTCAstate->mFunction="forward_steering";
//		mTCAstate->mConfig="all";
	}
	else if(mTCAstate->mMode=="auto") {
		//
		// check consistent state
		//
		if(mTCAstate->mFunction=="hovering") {
			if((mTCAstate->mConfig=="diamond")||(mTCAstate->mConfig=="diamond_azimuth"))
				return;
		}
		if(mTCAstate->mFunction=="steering") {
			if((mTCAstate->mConfig=="all")||(mTCAstate->mConfig=="bow")||(mTCAstate->mConfig=="stern"))
				return;
		}
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
		// zeroes SWAMP references
		//
		mSwampActuationRef->reset();
		//
		// set default MANUAL mode
		//
		set_manual_default();
//		mTCAstate->mMode="manual";
//		mTCAstate->mFunction="forward_steering";
//		mTCAstate->mConfig="all";
		//
		// printf error message
		//
		printf("SwampTCA::set_mode - unexpected mode %s\n",str.c_str());
		fflush(stdout);
	}
}


void SwampTCA::set_function(string  str) {
//printf("SwampTCA::set_function\n");fflush(stdout);
	TCA::set_function(str);
	if(mTCAstate->mFunction=="translation") {
		//
		// check consistent state
		//
		if(mTCAstate->mMode=="manual") {
			if(mTCAstate->mConfig=="parallel")
				return;
		}
		//
		// zeroes force/torque references
		//
		mForceMomentRef->reset();
		//
		// zeroes SWAMP references
		//
		mSwampActuationRef->reset();
		//
		// force function
		//
		// MANUAL mode
		mTCAstate->mMode="manual";
		// set config to "parallel"
		mTCAstate->mConfig="parallel";
	}
	else if((mTCAstate->mFunction=="forward_steering")||(mTCAstate->mFunction=="backward_steering")) {
		//
		// check consistent state
		//
		if(mTCAstate->mMode=="manual") {
			if((mTCAstate->mConfig=="all")||(mTCAstate->mConfig=="bow")||(mTCAstate->mConfig=="stern"))
				return;
		}
		//
		// zeroes force/torque references
		//
		mForceMomentRef->reset();
		//
		// zeroes SWAMP references
		//
		mSwampActuationRef->reset();
		//
		// force function
		//
		// MANUAL mode
		mTCAstate->mMode="manual";
		if(!((mTCAstate->mConfig=="all")||(mTCAstate->mConfig=="bow")||(mTCAstate->mConfig=="stern"))) {
			// set config to default
			mTCAstate->mConfig="all";
		}
	}
	else if(mTCAstate->mFunction=="hovering") {
		//
		// check consistent state
		//
		if(mTCAstate->mMode=="auto") {
			if((mTCAstate->mConfig=="diamond")||(mTCAstate->mConfig=="diamond_azimuth"))
				return;
		}
		//
		// zeroes force/torque references
		//
		mForceMomentRef->reset();
		//
		// zeroes SWAMP references
		//
		mSwampActuationRef->reset();
		//
		// force function
		//
		// AUTO mode
		mTCAstate->mMode="auto";
		if(!((mTCAstate->mConfig=="diamond")||(mTCAstate->mConfig=="diamond_azimuth"))) {
			// set config to default
			mTCAstate->mConfig="diamond";
		}
	}
	else if(mTCAstate->mFunction=="steering") {
		//
		// check consistent state
		//
		if(mTCAstate->mMode=="auto") {
			if((mTCAstate->mConfig=="all")||(mTCAstate->mConfig=="bow")||(mTCAstate->mConfig=="stern"))
				return;
		}
		//
		// zeroes force/torque references
		//
		mForceMomentRef->reset();
		//
		// zeroes SWAMP references
		//
		mSwampActuationRef->reset();
		//
		// force function
		//
		// AUTO mode
		mTCAstate->mMode="auto";
		if(!((mTCAstate->mConfig=="all")||(mTCAstate->mConfig=="bow")||(mTCAstate->mConfig=="stern"))) {
			// set config to default
			mTCAstate->mConfig="all";
		}
	}

	else {
		//
		// zeroes force/torque references
		//
		mForceMomentRef->reset();
		//
		// zeroes SWAMP references
		//
		mSwampActuationRef->reset();
		//
		// set default MANUAL mode
		//
		set_manual_default();
//		mTCAstate->mMode="manual";
//		mTCAstate->mFunction="forward_steering";
//		mTCAstate->mConfig="all";
		//
		// printf error message
		//
		printf("SwampTCA::set_function - unexpected function %s\n",str.c_str());
		fflush(stdout);
	}
}


void SwampTCA::set_configuration(string  str) {
//printf("SwampTCA::set_configuration\n");fflush(stdout);
	//
	// check if set is compatible with function
	//
	bool flag=false;
	flag=flag||((str=="parallel")&&(mTCAstate->mFunction=="translation"));
	flag=flag||((str=="all")&&((mTCAstate->mFunction=="forward_steering")&&(mTCAstate->mFunction=="backward_steering")&&(mTCAstate->mFunction=="steering")));
	flag=flag||((str=="bow")&&((mTCAstate->mFunction=="forward_steering")&&(mTCAstate->mFunction=="backward_steering")&&(mTCAstate->mFunction=="steering")));
	flag=flag||((str=="stern")&&((mTCAstate->mFunction=="forward_steering")&&(mTCAstate->mFunction=="backward_steering")&&(mTCAstate->mFunction=="steering")));
	flag=flag||((str=="diamond_azimuth")&&(mTCAstate->mFunction=="hovering"));
	if(flag)
		TCA::set_function(str);
	else {
		//
		// zeroes force/torque references
		//
		mForceMomentRef->reset();
		//
		// zeroes SWAMP references
		//
		mSwampActuationRef->reset();
		//
		// set default MANUAL mode
		//
		set_manual_default();
//		mTCAstate->mMode="manual";
//		mTCAstate->mFunction="forward_steering";
//		mTCAstate->mConfig="all";
		//
		// printf error message
		//
		printf("SwampTCA::set_config - unexpected config %s\n",str.c_str());
		fflush(stdout);
	}

}

void SwampTCA::compute_applied_force_torque(void) {
	double alpha[SWAMP_N];
	double T[SWAMP_N];

	alpha[SWAMP_RL]=mSwampActuationMeas->alphaRL * M_PI/180.;
	alpha[SWAMP_FL]=mSwampActuationMeas->alphaFL * M_PI/180.;
	alpha[SWAMP_RR]=mSwampActuationMeas->alphaRR * M_PI/180.;
	alpha[SWAMP_FR]=mSwampActuationMeas->alphaFR * M_PI/180.;

	T[SWAMP_RL]=SWAMP_A_THRUST * mSwampActuationMeas->nRL * mSwampActuationMeas->nRL;
	T[SWAMP_FL]=SWAMP_A_THRUST * mSwampActuationMeas->nFL * mSwampActuationMeas->nFL;
	T[SWAMP_RR]=SWAMP_A_THRUST * mSwampActuationMeas->nRR * mSwampActuationMeas->nRR;
	T[SWAMP_FR]=SWAMP_A_THRUST * mSwampActuationMeas->nFR * mSwampActuationMeas->nFR;

	double X[SWAMP_N],Y[SWAMP_N];
	for(int i=0;i<SWAMP_N;i++) {
		X[i]=T[i]*cos(alpha[i]);
		Y[i]=T[i]*sin(alpha[i]);
	}
	mForceMomentHat->X=mForceMomentHat->Y=0.;
	for(int i=0;i<SWAMP_N;i++) {
		mForceMomentHat->X += X[i];
		mForceMomentHat->Y += Y[i];
	}
	mForceMomentHat->N=0.;
	mForceMomentHat->N += X[SWAMP_RL]*SWAMP_D - Y[SWAMP_RL]*SWAMP_L;
	mForceMomentHat->N += X[SWAMP_FL]*SWAMP_D + Y[SWAMP_FL]*SWAMP_L;
	mForceMomentHat->N += - X[SWAMP_RR]*SWAMP_D - Y[SWAMP_RR]*SWAMP_L;
	mForceMomentHat->N += - X[SWAMP_FR]*SWAMP_D + Y[SWAMP_FR]*SWAMP_L;
}

void SwampTCA::execute_manual(void) {
//printf("SwampThrustControlAllocation::execute_manual\n");fflush(stdout);
	if(mTCAstate->mFunction=="translation")
		translation_mapping();
	else if(mTCAstate->mFunction=="forward_steering") {
		if(mTCAstate->mConfig=="all")
			forward_XN_all_mapping();
		else if(mTCAstate->mConfig=="bow")
			forward_XN_bow_mapping();
		else if(mTCAstate->mConfig=="stern")
			forward_XN_stern_mapping();
		else {
			printf("SwampTCA::execute_manual mFunction %s mConfig %s not processed\n",
					mTCAstate->mFunction.c_str(),mTCAstate->mConfig.c_str());
			fflush(stdout);
		}
	}
	else if(mTCAstate->mFunction=="backward_steering") {
		if(mTCAstate->mConfig=="all")
			backward_XN_all_mapping();
		else if(mTCAstate->mConfig=="bow")
			backward_XN_bow_mapping();
		else if(mTCAstate->mConfig=="stern")
			backward_XN_stern_mapping();
		else {
			printf("SwampTCA::execute_manual mFunction %s mConfig %s not processed\n",
					mTCAstate->mFunction.c_str(),mTCAstate->mConfig.c_str());
			fflush(stdout);
		}
	}
	else if(mTCAstate->mFunction=="null")
		reset();
	else {
		printf("SwampTCA::execute_manual mFunction %s not processed\n",mTCAstate->mFunction.c_str());
		fflush(stdout);
	}
}

void SwampTCA::translation_mapping(void) {
//printf("SwampTCA::translation_mapping\n");fflush(stdout);
	mSwampActuationRef->nRL=mSwampActuationRef->n;
	mSwampActuationRef->nFL=mSwampActuationRef->n;
	mSwampActuationRef->nRR=mSwampActuationRef->n;
	mSwampActuationRef->nFR=mSwampActuationRef->n;
	mSwampActuationRef->alphaRL=mod180(mSwampActuationRef->alpha);
	mSwampActuationRef->alphaFL=mod180(mSwampActuationRef->alpha);
	mSwampActuationRef->alphaRR=mod180(mSwampActuationRef->alpha);
	mSwampActuationRef->alphaFR=mod180(mSwampActuationRef->alpha);
}

void SwampTCA::forward_XN_all_mapping(void) {
//printf("SwampTCA::forward_XN_all_mapping\n");fflush(stdout);
	mSwampActuationRef->nRL=mSwampActuationRef->n;
	mSwampActuationRef->nFL=mSwampActuationRef->n;
	mSwampActuationRef->nRR=mSwampActuationRef->n;
	mSwampActuationRef->nFR=mSwampActuationRef->n;
	mSwampActuationRef->alphaRL=mod180(-mSwampActuationRef->alpha);
	mSwampActuationRef->alphaFL=mod180(mSwampActuationRef->alpha);
	mSwampActuationRef->alphaRR=mod180(-mSwampActuationRef->alpha);
	mSwampActuationRef->alphaFR=mod180(mSwampActuationRef->alpha);
}


void SwampTCA::forward_XN_bow_mapping(void) {
//printf("SwampTCA::forward_XN_bow_mapping\n");fflush(stdout);
	mSwampActuationRef->nRL=0.;
	mSwampActuationRef->nFL=mSwampActuationRef->n;
	mSwampActuationRef->nRR=0.;
	mSwampActuationRef->nFR=mSwampActuationRef->n;
	mSwampActuationRef->alphaRL=0.;
	mSwampActuationRef->alphaFL=mod180(mSwampActuationRef->alpha);
	mSwampActuationRef->alphaRR=0.;
	mSwampActuationRef->alphaFR=mod180(mSwampActuationRef->alpha);
}

void SwampTCA::forward_XN_stern_mapping(void) {
//printf("SwampTCA::forward_XN_stern_mapping\n");fflush(stdout);
	mSwampActuationRef->nRL=mSwampActuationRef->n;
	mSwampActuationRef->nFL=0.;
	mSwampActuationRef->nRR=mSwampActuationRef->n;
	mSwampActuationRef->nFR=0.;
	mSwampActuationRef->alphaRL=mod180(-mSwampActuationRef->alpha);
	mSwampActuationRef->alphaFL=0.;
	mSwampActuationRef->alphaRR=mod180(-mSwampActuationRef->alpha);
	mSwampActuationRef->alphaFR=0.;
}

void SwampTCA::backward_XN_all_mapping(void) {
//printf("SwampTCA::backward_XN_all_mapping\n");fflush(stdout);
	mSwampActuationRef->nRL=mSwampActuationRef->n;
	mSwampActuationRef->nFL=mSwampActuationRef->n;
	mSwampActuationRef->nRR=mSwampActuationRef->n;
	mSwampActuationRef->nFR=mSwampActuationRef->n;
	mSwampActuationRef->alphaRL=summod180(-180,mSwampActuationRef->alpha);
	mSwampActuationRef->alphaFL=summod180(180,-mSwampActuationRef->alpha);
	mSwampActuationRef->alphaRR=summod180(-180,mSwampActuationRef->alpha);
	mSwampActuationRef->alphaFR=summod180(180,-mSwampActuationRef->alpha);
}

void SwampTCA::backward_XN_bow_mapping(void) {
//printf("SwampTCA::backward_XN_bow_mapping\n");fflush(stdout);
	mSwampActuationRef->nRL=0.;
	mSwampActuationRef->nFL=mSwampActuationRef->n;
	mSwampActuationRef->nRR=0.;
	mSwampActuationRef->nFR=mSwampActuationRef->n;
	mSwampActuationRef->alphaRL=0.;
	mSwampActuationRef->alphaFL=summod180(180,-mSwampActuationRef->alpha);
	mSwampActuationRef->alphaRR=0.;
	mSwampActuationRef->alphaFR=summod180(180,-mSwampActuationRef->alpha);
}

void SwampTCA::backward_XN_stern_mapping(void) {
//printf("SwampTCA::backward_XN_stern_mapping\n");fflush(stdout);
	mSwampActuationRef->nRL=mSwampActuationRef->n;
	mSwampActuationRef->nFL=0.;
	mSwampActuationRef->nRR=mSwampActuationRef->n;
	mSwampActuationRef->nFR=0.;
	mSwampActuationRef->alphaRL=summod180(-180,mSwampActuationRef->alpha);
	mSwampActuationRef->alphaFL=0.;
	mSwampActuationRef->alphaRR=summod180(-180,mSwampActuationRef->alpha);
	mSwampActuationRef->alphaFR=0.;
}

void SwampTCA::execute_auto(void) {
//printf("SwampTCA::execute_auto\n");fflush(stdout);
	if(mTCAstate->mFunction=="hovering") {
		if(mTCAstate->mConfig=="diamond")
			auto_hovering();
		else if(mTCAstate->mConfig=="diamond_azimuth")
			auto_hovering_azimuth_fixed();
		else {
			printf("SwampTCA::execute_auto mFunction %s mConfig %s not processed\n",
					mTCAstate->mFunction.c_str(),mTCAstate->mConfig.c_str());
			fflush(stdout);
		}
	}
	else if(mTCAstate->mFunction=="steering") {
		if(mTCAstate->mConfig=="all")
			auto_XN_all();
		else if(mTCAstate->mConfig=="bow")
			auto_XN_bow();
		else if(mTCAstate->mConfig=="stern")
			auto_XN_stern();
		else {
			printf("SwampTCA::execute_auto mFunction %s mConfig %s not processed\n",
					mTCAstate->mFunction.c_str(),mTCAstate->mConfig.c_str());
			fflush(stdout);
		}
	}
	else if(mTCAstate->mFunction=="null")
		reset();
	else {
		printf("SwampTCA::execute_auto mFunction %s not processed\n",mTCAstate->mFunction.c_str());
		fflush(stdout);
	}
}


void SwampTCA::auto_hovering(void) {
printf("SwampTCA::auto_hovering\n");fflush(stdout);
	double c,T[4];
	double cMin,Tmin[4];

	cMin=999999999999999999999999999999999.;
	Tmin[0]=Tmin[1]=Tmin[2]=Tmin[3]=0.;

	for (int i=0;i<=90;i++) {
		c=optimise_hovering_azimuth_fixed(T,(double)i);
		if(c<cMin) {
			cMin=c;
			mSwampActuationRef->alpha = i;
			Tmin[SWAMP_RL]=T[SWAMP_RL];
			Tmin[SWAMP_FL]=T[SWAMP_FL];
			Tmin[SWAMP_RR]=T[SWAMP_RR];
			Tmin[SWAMP_FR]=T[SWAMP_FR];
		}
	}
	allocate_azimuth_diamond();
	mSwampActuationRef->nRL = sqrt(Tmin[SWAMP_RL]/SWAMP_A_THRUST);
	mSwampActuationRef->nFL = sqrt(Tmin[SWAMP_FL]/SWAMP_A_THRUST);
	mSwampActuationRef->nRR = sqrt(Tmin[SWAMP_RR]/SWAMP_A_THRUST);
	mSwampActuationRef->nFR = sqrt(Tmin[SWAMP_FR]/SWAMP_A_THRUST);
}

void SwampTCA::auto_hovering_azimuth_fixed(void) {
//	printf("SwampTCA::auto_hovering_azimuth_fixed\n");fflush(stdout);
	double T[4];

	optimise_hovering_azimuth_fixed(T,mSwampActuationRef->alpha);

	allocate_azimuth_diamond();
	mSwampActuationRef->nRL = sqrt(T[SWAMP_RL]/SWAMP_A_THRUST);
	mSwampActuationRef->nFL = sqrt(T[SWAMP_FL]/SWAMP_A_THRUST);
	mSwampActuationRef->nRR = sqrt(T[SWAMP_RR]/SWAMP_A_THRUST);
	mSwampActuationRef->nFR = sqrt(T[SWAMP_FR]/SWAMP_A_THRUST);
}

void SwampTCA::auto_XN_all_n_alpha(void) {
//	printf("SwampTCA::auto_XN_all_n_alpha\n");fflush(stdout);
	double X,N;
	X=fabs(mForceMomentRef->X);
	N=mForceMomentRef->N;

	double F,alpha;
	//
	// check saturation
	//
	double deltaN2;
	double Tmax=SWAMP_THRUST_MAX;

	if(SQR(N)>=16.*SQR(SWAMP_L)*SQR(Tmax)) {
		N=4*SWAMP_L*Tmax*sign(N);
		X=0.;
	}
	else
	{
		deltaN2=SQR(Tmax)-SQR(N)/(16.*SQR(SWAMP_L));
		if(SQR(X)>=(16.*deltaN2))
		{
			X=4.*sqrt(deltaN2);
		}
	}

	if((X==0.)&&(N==0.))
	{
		alpha=0.;
		F=0.;
	}
	else
	{
		alpha=atan2(N,SWAMP_L*X);
		if(X==0.)
		{
			F=N/(4*SWAMP_L*sin(alpha));
		}
		else
			F=X/4*cos(alpha);
	}

	alpha=alpha*180./M_PI;

	//
	// compute rpm and azimuth
	//
	mSwampActuationRef->n = sqrt(F/SWAMP_A_THRUST);
	mSwampActuationRef->alpha = alpha;
}

void SwampTCA::auto_XN_all(void) {
//	printf("SwampTCA::auto_XN_all\n");fflush(stdout);
	if(mForceMomentRef->X >= 0.)
		auto_forward_XN_all();
	else
		auto_backward_XN_all();
}

void SwampTCA::auto_forward_XN_all(void) {
//	printf("SwampTCA::auto_forward_XN_all\n");fflush(stdout);
	auto_XN_all_n_alpha();
	forward_XN_all_mapping();
}

void SwampTCA::auto_backward_XN_all(void) {
//	printf("SwampTCA::auto_backward_XN_all\n");fflush(stdout);
	auto_XN_all_n_alpha();
	backward_XN_all_mapping();
}


void SwampTCA::auto_XN_2_n_alpha(void) {
//printf("SwampTCA::auto_XN_2_n_alpha\n");fflush(stdout);
	double X,N;
	X=fabs(mForceMomentRef->X);
	N=mForceMomentRef->N;

	double F,alpha;
	//
	// check saturation
	//
	double deltaN2;
	double Tmax=SWAMP_THRUST_MAX;

	if(SQR(N)>=4.*SQR(SWAMP_L)*SQR(Tmax)) {
		N=2*SWAMP_L*Tmax*sign(N);
		X=0.;
	}
	else
	{
		deltaN2=SQR(Tmax)-SQR(N)/(4.*SQR(SWAMP_L));
		if(SQR(X)>=(4.*deltaN2))
		{
			X=2.*sqrt(deltaN2);
		}
	}

	if((X==0.)&&(N==0.))
	{
		alpha=0.;
		F=0.;
	}
	else
	{
		alpha=atan2(N,SWAMP_L*X);
		if(X==0.)
		{
			F=N/(2*SWAMP_L*sin(alpha));
		}
		else
			F=X/2*cos(alpha);
	}

	alpha=alpha*180./M_PI;

	//
	// compute rpm and azimuth
	//
	mSwampActuationRef->n = sqrt(F/SWAMP_A_THRUST);
	mSwampActuationRef->alpha = alpha;
}


void SwampTCA::auto_XN_bow(void) {
//	printf("SwampTCA::auto_XN_bow\n");fflush(stdout);
	if(mForceMomentRef->X >= 0.)
		auto_forward_XN_bow();
	else
		auto_backward_XN_bow();
}

void SwampTCA::auto_forward_XN_bow(void) {
//	printf("SwampTCA::auto_forward_XN_bow\n");fflush(stdout);
	auto_XN_2_n_alpha();
	forward_XN_bow_mapping();
}

void SwampTCA::auto_backward_XN_bow(void){
//	printf("SwampTCA::auto_backward_XN_bow\n");fflush(stdout);
	auto_XN_2_n_alpha();
	backward_XN_bow_mapping();
}


void SwampTCA::auto_XN_stern(void) {
//	printf("SwampTCA::auto_XN_stern\n");fflush(stdout);
	if(mForceMomentRef->X >= 0.)
		auto_forward_XN_stern();
	else
		auto_backward_XN_stern();
}

void SwampTCA::auto_forward_XN_stern(void) {
//	printf("SwampTCA::auto_forward_XN_stern\n");fflush(stdout);
	auto_XN_2_n_alpha();
	forward_XN_stern_mapping();
}

void SwampTCA::auto_backward_XN_stern(void){
//	printf("SwampTCA::auto_backward_XN_stern\n");fflush(stdout);
	auto_XN_2_n_alpha();
	backward_XN_stern_mapping();
}


double SwampTCA::optimise_hovering_azimuth_fixed(double *T,double alphaDeg) {
    //
    // matrices used by optimisation algorithms
    //
    Matrix Ahov(3,4), x(4,1), b(3,1), Wt(3,3), Wp(4,4), Q(4,4), c(4,1), grad(4,1);
    Matrix D3x3(3,3), D4x4(4,4), D3x4(3,4), D3x1(3,1), D4x1(4,1);
    Matrix xNew(4,1);
    Matrix num(1,1),den(1,1);
    Matrix cost(1,1);
    double thr;
    int nSteps;
    //
	// build A
    //
	double alpha=alphaDeg*M_PI/180.;
	double ca=cos(alpha);
	double sa=sin(alpha);
	double ld=SWAMP_D*ca+SWAMP_L*sa;
	Ahov(0,0) = -ca; Ahov(0,1) = ca; Ahov(0,2) = -ca; Ahov(0,3) = ca;
	Ahov(1,0) = sa; Ahov(1,1) = sa; Ahov(1,2) = -sa; Ahov(1,3) = -sa;
	Ahov(2,0) = -ld; Ahov(2,1) = ld; Ahov(2,2) = ld; Ahov(2,3) = -ld;
	//
	// build b
	//
	b(0,0)=mForceMomentRef->X;
	b(1,0)=mForceMomentRef->Y;
	b(2,0)=mForceMomentRef->N;
	//
	// build Wt
	//
	Wt.zeros(); Wt(0,0)=SWAMP_X_WEIGHT; Wt(1,1)=SWAMP_Y_WEIGHT; Wt(2,2)=SWAMP_N_WEIGHT;
	//
	// build Wp
	//
	Wp.eye(); Wp=SWAMP_WEIGHT_POWER*Wp;
	//
	// build Q
	//
	D3x4=Wt*Ahov;
	D4x4=Ahov.t()*D3x4;
	D4x4=D4x4+Wp;
	Q=2.*D4x4;
	//
	// build c
	//
	D3x1=Wt*b;
	D4x1=Ahov.t()*D3x1;
	c=-2.*D4x1;

	nSteps=100;
	x(0,0)=x(1,0)=x(2,0)=x(3,0)=SWAMP_THRUST_MAX/2.;
	cost=0.5*x.t()*Q*x+c.t()*x+b.t()*b;
	thr=0.000001;

	double step;
	for(int i=0;i<nSteps;i++) {
		if(cost(0,0)>thr) {
			// gradient descent
			grad=Q*x+c;
			num=x.t()*Q*grad+c.t()*grad;
			den=grad.t()*Q*grad;
			step=num(0,0)/den(0,0);
			xNew=x-step*grad;
			// check constraints
			x(0,0)=sat(xNew(0,0),0.,SWAMP_THRUST_MAX);
			x(1,0)=sat(xNew(1,0),0.,SWAMP_THRUST_MAX);
			x(2,0)=sat(xNew(2,0),0.,SWAMP_THRUST_MAX);
			x(3,0)=sat(xNew(3,0),0.,SWAMP_THRUST_MAX);
			// compute cost
			cost=0.5*x.t()*Q*x+c.t()*x+b.t()*b;
		}
	}

	T[0]=x(0,0);
	T[1]=x(1,0);
	T[2]=x(2,0);
	T[3]=x(3,0);

	return cost(0,0);
}

void SwampTCA::allocate_azimuth_diamond(void) {
	mSwampActuationRef->alphaRL = summod180(180.,-mSwampActuationRef->alpha);
	mSwampActuationRef->alphaFL = mSwampActuationRef->alpha;
	mSwampActuationRef->alphaRR = summod180(180.,mSwampActuationRef->alpha);
	mSwampActuationRef->alphaFR = -mSwampActuationRef->alpha;
}


void SwampTCA::printf_results(void) {
	char packet[512];

	mSwampActuationRef->sprint_csv_log_name(packet);
	printf("SwampTCA::printf_results %s %s %s %s %s\n",mName.c_str(),mTCAstate->mMode.c_str(),mTCAstate->mFunction.c_str(),mTCAstate->mConfig.c_str(),packet);
	fflush(stdout);
	mSwampActuationRef->sprint_csv_long_name(packet);
	printf("SwampTCA::printf_results %s %s %s %s %s\n",mName.c_str(),mTCAstate->mMode.c_str(),mTCAstate->mFunction.c_str(),mTCAstate->mConfig.c_str(),packet);
	fflush(stdout);
	mSwampActuationRef->sprint(packet);
	printf("SwampTCA::printf_results %s %s %s %s %s\n",mName.c_str(),mTCAstate->mMode.c_str(),mTCAstate->mFunction.c_str(),mTCAstate->mConfig.c_str(),packet);
	fflush(stdout);

}



