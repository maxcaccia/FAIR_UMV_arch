/*
 * TCAalgorithms.cpp
 *
 *  Created on: 22 July 2025
 *      Author: massimo
 */

#include "TCAalgorithms.h"

TCAalgorithms::TCAalgorithms(void) {
	//
	// create optimizers
	//
	// surge-yaw differential propulsion
	surge_yaw_differential_opt = nlopt_create(NLOPT_LN_BOBYQA,2);
	// surge-yaw azimuth jets
	surge_yaw_azimuth_opt = nlopt_create(NLOPT_LN_BOBYQA,2);
	// surge-sway-yaw diamond
	surge_sway_yaw_diamond_opt = nlopt_create(NLOPT_LN_BOBYQA,5);
	// surge-sway-yaw free
	surge_sway_yaw_free_opt = nlopt_create(NLOPT_LN_BOBYQA,8);

	//
	// set parameters
	//
	surge_yaw_ref_params.lambdaN = 0.2;
	surge_yaw_ref_params.kDiff = kDiff;
	surge_sway_yaw_ref_params.lambdaY = 1.0;
	surge_sway_yaw_ref_params.lambdaN = 5.0;

	//
	// set the objective function with parameters
	//
	// surge-yaw differential propulsion
	nlopt_set_min_objective(surge_yaw_differential_opt, cost_surge_yaw_differential, &surge_yaw_ref_params);
	// surge-yaw azimuth jets
	nlopt_set_min_objective(surge_yaw_azimuth_opt, cost_surge_yaw_thrust_azimuth, &surge_yaw_ref_params);
	// surge-sway-yaw diamond
	nlopt_set_min_objective(surge_sway_yaw_diamond_opt, cost_surge_sway_yaw_thrust_azimuth_diamond, &surge_sway_yaw_ref_params);
	// surge-sway-yaw free
	nlopt_set_min_objective(surge_sway_yaw_free_opt, cost_surge_sway_yaw_thrust_azimuth_free, &surge_sway_yaw_ref_params);

	//
	// set relative tolerance on optimisation parameters
	//
	double tolerance = 1e-6;
	nlopt_set_xtol_rel(surge_yaw_differential_opt, tolerance);
	nlopt_set_xtol_rel(surge_yaw_azimuth_opt, tolerance);
	nlopt_set_xtol_rel(surge_sway_yaw_diamond_opt, tolerance);
	nlopt_set_xtol_rel(surge_sway_yaw_free_opt, tolerance);

	// set the max optimisation time in seconds
	double maxtime = 1.0;
	nlopt_set_maxtime(surge_yaw_differential_opt, maxtime);
	nlopt_set_maxtime(surge_yaw_azimuth_opt, maxtime);
	nlopt_set_maxtime(surge_sway_yaw_diamond_opt, maxtime);
	nlopt_set_maxtime(surge_sway_yaw_free_opt, maxtime);

	kDiff=d=l=0.;
}


TCAalgorithms::~TCAalgorithms() {

}


void TCAalgorithms::set_parameters(double k,double D,double L,double kY,double kN,double kT,double kAlpha) {
	kDiff=k;
	d=D;
	l=L;
	lambdaY=kY;
	lambdaN=kN;
	lambdaT=kT;
	lambdaAlpha=kAlpha;

	surge_yaw_ref_params.kDiff=kDiff;
	surge_yaw_ref_params.l=d;
	surge_yaw_ref_params.l=l;
	surge_yaw_ref_params.lambdaN=lambdaN;

	surge_sway_yaw_ref_params.d=d;
	surge_sway_yaw_ref_params.l=l;
	surge_sway_yaw_ref_params.lambdaY=lambdaY;
	surge_sway_yaw_ref_params.lambdaN=lambdaN;
	surge_sway_yaw_ref_params.lambdaT=lambdaT;
	surge_sway_yaw_ref_params.lambdaAlpha=lambdaAlpha;

	set_constraints(kDiff,d,l);
}

void TCAalgorithms::set_constraints(double k,double D,double L) {

	//
	// set optimiser constraints
	//
	// surge-yaw differential propulsion
	surge_yaw_differential_lb[0] = -kDiff;
	surge_yaw_differential_lb[1] = -kDiff;
	surge_yaw_differential_ub[0] = 1.;
	surge_yaw_differential_ub[1] = 1.;
	nlopt_set_lower_bounds(surge_yaw_differential_opt, surge_yaw_differential_lb);
	nlopt_set_upper_bounds(surge_yaw_differential_opt, surge_yaw_differential_ub);
	// surge-yaw azimuth jets
	surge_yaw_azimuth_lb[0] = 0.;
	surge_yaw_azimuth_lb[1] = -M_PI;
	surge_yaw_azimuth_ub[0] = 1.;
	surge_yaw_azimuth_ub[1] = M_PI;
	nlopt_set_lower_bounds(surge_yaw_azimuth_opt, surge_yaw_azimuth_lb);
	nlopt_set_upper_bounds(surge_yaw_azimuth_opt, surge_yaw_azimuth_ub);
	// surge-sway-yaw diamond
	surge_sway_yaw_diamond_lb[0] = 0;
	surge_sway_yaw_diamond_lb[1] = 0;
	surge_sway_yaw_diamond_lb[2] = 0;
	surge_sway_yaw_diamond_lb[3] = 0;
	surge_sway_yaw_diamond_lb[4] = 0;
	surge_sway_yaw_diamond_ub[0] = 1;
	surge_sway_yaw_diamond_ub[1] = 1;
	surge_sway_yaw_diamond_ub[2] = 1;
	surge_sway_yaw_diamond_ub[3] = 1;
	surge_sway_yaw_diamond_ub[4] = M_PI/2;
	nlopt_set_lower_bounds(surge_sway_yaw_diamond_opt, surge_sway_yaw_diamond_lb);
	nlopt_set_upper_bounds(surge_sway_yaw_diamond_opt, surge_sway_yaw_diamond_ub);
	// surge-sway-yaw free
	surge_sway_yaw_free_lb[0] = 0;
	surge_sway_yaw_free_lb[1] = 0;
	surge_sway_yaw_free_lb[2] = 0;
	surge_sway_yaw_free_lb[3] = 0;
	surge_sway_yaw_free_lb[4] = -M_PI;
	surge_sway_yaw_free_lb[5] = -M_PI;
	surge_sway_yaw_free_lb[6] = -M_PI;
	surge_sway_yaw_free_lb[7] = -M_PI;
	surge_sway_yaw_free_ub[0] = 1;
	surge_sway_yaw_free_ub[1] = 1;
	surge_sway_yaw_free_ub[2] = 1;
	surge_sway_yaw_free_ub[3] = 1;
	surge_sway_yaw_free_ub[4] = M_PI;
	surge_sway_yaw_free_ub[5] = M_PI;
	surge_sway_yaw_free_ub[6] = M_PI;
	surge_sway_yaw_free_ub[7] = M_PI;
	nlopt_set_lower_bounds(surge_sway_yaw_free_opt, surge_sway_yaw_free_lb);
	nlopt_set_upper_bounds(surge_sway_yaw_free_opt, surge_sway_yaw_free_ub);
}


double TCAalgorithms::TCA_differential_steering(double Xtilde,double Ntilde,double &T1tilde,double &T2tilde) {

	surge_yaw_ref_params.XtildeRef = Xtilde;
	surge_yaw_ref_params.NtildeRef = Ntilde;

	double x[2];
	double c=0.;
	x[0] = T1tilde = Xtilde + ((kDiff+1)/2)*Ntilde;
	x[1] = T2tilde = Xtilde - ((kDiff+1)/2)*Ntilde;

	if((T1tilde<-kDiff)||(T1tilde>1)||(T2tilde<-kDiff)||(T2tilde>1)) {
//		printf("TCAalgorithms::TCA_differential_steering here T1 %.2lf T2 %.2lf k %.2lf\n",
//				T1tilde,T2tilde,kDiff);
		x[0] = x[1] = 0.;
		nlopt_optimize(surge_yaw_differential_opt, x, &c);

		T1tilde = x[0];
		T2tilde = x[1];
	}
	c = cost_surge_yaw_differential((unsigned)NULL,x,NULL,&surge_yaw_ref_params);

	//printf("TCAalgorithms::TCA_differential_steering c %.3lf X %.2lf N %.2lf T1 %.2lf T2 %.2lf\n",
			//c,Xtilde,Ntilde,T1tilde,T2tilde);
	//fflush(stdout);

	return c;
}


double TCAalgorithms::TCA_thrust_azimuth_steering(double Xtilde,double Ntilde,double &Ttilde,double &alpha) {

	surge_yaw_ref_params.XtildeRef = Xtilde;
	surge_yaw_ref_params.NtildeRef = Ntilde;

	double x[2];
	double c=0.;
	alpha = atan2(Ntilde*sqrt(l*l+d*d),Xtilde*l);
	Ttilde = sqrt(Xtilde*Xtilde+Ntilde*Ntilde*(l*l+d*d)/(l*l));
	Ttilde = sat(Ttilde,-1,1);

	x[0] = Ttilde;
	x[1] = alpha;

	c = cost_surge_yaw_thrust_azimuth((unsigned)NULL,x,NULL,&surge_yaw_ref_params);

	printf("TCAalgorithms::TCA_thrust_azimuth_steering c %.3lf X %.2lf N %.2lf T %.2lf alpha %.2lf\n",
			c,Xtilde,Ntilde,Ttilde,alpha*180./M_PI);
	fflush(stdout);

	return c;
}


double TCAalgorithms::TCA_thrust_azimuth_diamond(double Xtilde,double Ytilde,double Ntilde,double *Ttilde,double &alpha) {
	surge_sway_yaw_ref_params.XtildeRef = Xtilde;
	surge_sway_yaw_ref_params.YtildeRef = Ytilde;
	surge_sway_yaw_ref_params.NtildeRef = Ntilde;

	double x[5];
	double c=0.;

	x[0] = x[1] = x[2] = x[3] = 0.5;
	double m=sqrt(Xtilde*Xtilde+Ytilde*Ytilde+Ntilde*Ntilde);
	double alphaBar=atan2(l,d);
	x[4] = ((Ytilde*sign(Ytilde)*M_PI/2)+Ntilde*sign(Ntilde)*alphaBar)/m;
	//x[4] = 0.;

	nlopt_optimize(surge_sway_yaw_diamond_opt, x, &c);

	c = raw_cost_surge_sway_yaw_thrust_azimuth_diamond((unsigned)NULL,x,NULL,&surge_sway_yaw_ref_params);

	Ttilde[0] = x[0];
	Ttilde[1] = x[1];
	Ttilde[2] = x[2];
	Ttilde[3] = x[3];
	alpha = x[4];

	printf("TCAalgorithms::TCA_thrust_azimuth_diamond c %.3lf X %.2lf Y %.2lf N %.2lf T0 %.2lf T1 %.2lf T2 %.2lf T3 %.2lf alpha %.2lf\n",
			c,Xtilde,Ytilde,Ntilde,
			Ttilde[0],Ttilde[1],Ttilde[2],Ttilde[3],alpha*180./M_PI);
	fflush(stdout);

	return c;
}

double TCAalgorithms::cost_surge_yaw_differential(unsigned n,const double *x,double *grad,void *data) {
	(void)n;
	(void)grad;

	cost_surge_yaw_ref_params_t *params = (cost_surge_yaw_ref_params_t*)data;

	double T1 = x[0]; // left thrust
	double T2 = x[1]; // right thrust
	double c;
//	printf("T1 %.2lf T2 %.2lf X %.2lf N %.2lf k %.2lf\n",
//			T1,T2,params->XtildeRef,params->NtildeRef,params->kDiff);
	c = pow((T1+T2)/2 - params->XtildeRef, 2)
			+ params->lambdaN * pow((T1-T2)/(1+params->kDiff) - params->NtildeRef, 2);

//	printf("TCAalgorithms::cost_surge_yaw_differential c %.2lf T1 %.2lf T2 %.2lf X %.2lf N %.2lf k %.2lf\n",
//			c,T1,T2,params->XtildeRef,params->NtildeRef,params->kDiff);

	return c;
}

double TCAalgorithms::cost_surge_yaw_thrust_azimuth(unsigned n,const double *x,double *grad,void *data) {
	(void)n;
	(void)grad;

	cost_surge_yaw_ref_params_t *params = (cost_surge_yaw_ref_params_t*)data;

	double T = x[0];
	double alpha = x[1];
	double c;

	double XtildeHat = T*cos(alpha);
	double NtildeHat = T*sin(alpha)*params->l / sqrt(pow(params->l,2)+pow(params->d,2));
	c = pow(XtildeHat - params->XtildeRef, 2)
			+ params->lambdaN * pow(NtildeHat - params->NtildeRef, 2);

	return c;
}

double TCAalgorithms::cost_surge_sway_yaw_thrust_azimuth_diamond(unsigned n,const double *x,double *grad,void *data) {
	(void)n;
	(void)grad;

	cost_surge_sway_yaw_ref_params_t *params = (cost_surge_sway_yaw_ref_params_t*)data;

	double T1 = x[0]; // rear left thrust
	double T2 = x[1]; // front left thrust
	double T3 = x[2]; // rear right thrust
	double T4 = x[3]; // front right thrust
	double alpha = x[4]; // azimuth angle [rad]

	double alphaBar=atan2(params->l,params->d);
	double Xtilde,Ytilde,Ntilde;
	Xtilde=((-T1+T2-T3+T4)/4)*cos(alpha);
	Ytilde=((T1+T2-T3-T4)/4)*sin(alpha);
	Ntilde=((-T1+T2+T3-T4)/4)*(cos(alphaBar)*cos(alpha)+sin(alphaBar)*sin(alpha));

	double cE = pow(Xtilde - params->XtildeRef, 2)
								+ params->lambdaY * pow(Ytilde - params->YtildeRef, 2)
	+ params->lambdaN * pow(Ntilde - params->NtildeRef, 2);

	//double wT=0.001;
	double T0=0.;
	double cT=params->lambdaT*((T1-T0)*(T1-T0)+(T2-T0)*(T2-T0)+(T3-T0)*(T3-T0)+(T4-T0)*(T4-T0));

	double alpha0=0;
	double cAlpha=params->lambdaAlpha*(alpha-alpha0)*(alpha-alpha0);

	double c=cE+cT+cAlpha;

	return c;
}


double TCAalgorithms::raw_cost_surge_sway_yaw_thrust_azimuth_diamond(unsigned n,const double *x,double *grad,void *data) {
	(void)n;
	(void)grad;

	cost_surge_sway_yaw_ref_params_t *params = (cost_surge_sway_yaw_ref_params_t*)data;

	double T1 = x[0]; // rear left thrust
	double T2 = x[1]; // front left thrust
	double T3 = x[2]; // rear right thrust
	double T4 = x[3]; // front right thrust
	double alpha = x[4]; // azimuth angle [rad]

	double alphaBar=atan2(params->l,params->d);
	double Xtilde,Ytilde,Ntilde;
	Xtilde=((-T1+T2-T3+T4)/4)*cos(alpha);
	Ytilde=((T1+T2-T3-T4)/4)*sin(alpha);
	Ntilde=((-T1+T2+T3-T4)/4)*(cos(alphaBar)*cos(alpha)+sin(alphaBar)*sin(alpha));

	double c = pow(Xtilde - params->XtildeRef, 2)
				+ params->lambdaY * pow(Ytilde - params->YtildeRef, 2)
				+ params->lambdaN * pow(Ntilde - params->NtildeRef, 2);

	return c;
}

double TCAalgorithms::cost_surge_sway_yaw_thrust_azimuth_free(unsigned n,const double *x,double *grad,void *data) {
	return 0;
}

double TCAalgorithms::raw_cost_surge_sway_yaw_thrust_azimuth_free(unsigned n,const double *x,double *grad,void *data) {
	return 0;
}


