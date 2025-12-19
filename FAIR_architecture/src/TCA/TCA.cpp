/*
 * TCA.cpp
 *
 *  Created on: 5 mar 2024
 *      Author: massimo
 */

#include "TCA.h"

TCA::TCA(void) {
	mNopCounter=0;
	mNopHandlerEnable=true;
	mName="";
	mConfigDir="";
	mXYNref=NULL;
	mXYNnormRef=NULL;
	mXYNhat=NULL;
	mXYNnormHat=NULL;
	mTCAstate=NULL;
//	mMode=TCA_MANUAL;
//	mFunction="null";
//	mConfig="null";

	d=l=kDiff=Tmax=0.;
	lambdaY = lambdaN = 1.0;
	lambdaT = 0.001;
	lambdaAlpha = 0.;

}

TCA::~TCA(void) {
	if(mXYNref!=NULL) {
		mXYNref=NULL;
	}
	if(mXYNnormRef!=NULL) {
		mXYNnormRef=NULL;
	}
	if(mXYNhat!=NULL) {
		mXYNhat=NULL;
	}
	if(mXYNnormHat!=NULL) {
		mXYNnormHat=NULL;
	}
	if(mTCAstate!=NULL) {
		mTCAstate=NULL;
	}
}


void TCA::process_command(char *cmd,ssize_t n) {
//	printf("******************** %s ---- TCA::process_command : %s\r\n",mName.c_str(),cmd);fflush(stdout);

	char chrType[64],chrCmd[64];
	string strType,strCmd;
	sscanf(cmd,"%s %s",chrType,chrCmd);
	strType=chrType;
	strCmd=chrCmd;

	if(strType!="TCA") {
//		printf("TCA::process_command - wrong type %s\n",strType.c_str());
//		fflush(stdout);
		return ;
	}

	if(mTcaCmd.decode(cmd, strCmd)) {
		if(strCmd=="START") {
			start();
		}
		if(strCmd=="STOP") {
			stop();
		}
		if(strCmd=="ENABLE") {
			mTCAstate->mEnable=true;
			set_manual_default();
		}
		else if(strCmd=="DISABLE") {
			mTCAstate->mEnable=false;
			reset();
		}
		else if(strCmd=="SET_2DOF") {
			set_2_dof(mTcaCmd.X,mTcaCmd.N);
		}
		else if(strCmd=="SET_3DOF") {
			set_3_dof(mTcaCmd.X,mTcaCmd.Y,mTcaCmd.N);
		}
		else if(strCmd=="SET_2DOF_NORM") {
			set_2_dof_norm(mTcaCmd.Xnorm,mTcaCmd.Nnorm);
		}
		else if(strCmd=="SET_3DOF_NORM") {
			set_3_dof_norm(mTcaCmd.Xnorm,mTcaCmd.Ynorm,mTcaCmd.Nnorm);
		}
		else if(strCmd=="SET_MODE") {
			set_mode(mTcaCmd.mode);
		}
		else if(strCmd=="SET_FUNCTION") {
			set_function(mTcaCmd.function);
		}
		else if(strCmd=="SET_CONFIG") {
			set_config(mTcaCmd.config);
		}
		else  {
			//				printf("TCA::process_command ERROR read - TCA cmd is %s\n",strCmd.c_str());
			//				fflush(stdout);
			return ;
		}
	}


}



void TCA::set_3_dof(double X,double Y,double N) {
	if(mTCAstate->mMode == "raw")
		return ;

	mXYNref->X=X;
	if(mTCAstate->mFunction=="steering")
		// Note: in steering function sway force is set to zero
		mXYNref->Y=0.;
	else
		mXYNref->Y=Y;
	mXYNref->N=N;

	standard_to_normalised_ref();
}

void TCA::set_2_dof(double X,double N) {
	if(mTCAstate->mMode == "raw")
		return ;

	mXYNref->X=X;
	mXYNref->Y=0.;
	mXYNref->N=N;

	standard_to_normalised_ref();
}




void TCA::set_3_dof_norm(double Xnorm,double Ynorm,double Nnorm) {
	if(mTCAstate->mMode == "raw")
		return ;

	mXYNnormRef->X=Xnorm;
	if(mTCAstate->mFunction=="steering")
		// Note: in steering function sway force is set to zero
		mXYNnormRef->Y=0.;
	else
		mXYNnormRef->Y=Ynorm;
	mXYNnormRef->N=Nnorm;

	normalised_to_standard_ref();
}

void TCA::set_2_dof_norm(double Xnorm,double Nnorm) {
	if(mTCAstate->mMode == "raw")
		return ;

	mXYNnormRef->X=Xnorm;
	mXYNnormRef->Y=0.;
	mXYNnormRef->N=Nnorm;

	normalised_to_standard_ref();
}


void TCA::set_propulsion_class(string str) {
	if(str=="differential_propellers")
		mTCAstate->mPropulsionClass=DIFFERENTIAL_PROPELLERS;
	else if(str=="azimuth_jets_4")
		mTCAstate->mPropulsionClass=AZIMUTH_JETS_4;
	else {
		printf("TCA::set_propulsion_class invalid propulsion class %s found!\n",str.c_str());
		fflush(stdout);
	}
}


void TCA::set_config(string str) {
	if(mTCAstate->mPropulsionClass==UNDEFINED) {
		printf("TCA::set_config ERROR : UNDEFINED propulsion class\n");
		fflush(stdout);
		return ;
	}
	if(str=="surge_yaw") {
		if(mTCAstate->mFunction=="hovering") {
			printf("TCA::set_config ERROR : %s\n",str.c_str());
			fflush(stdout);
		}
		else
			mTCAstate->mConfig = str;
	}
	else if((str=="diamond")||(str=="free")) {
		if(mTCAstate->mPropulsionClass==DIFFERENTIAL_PROPELLERS) {
			printf("TCA::set_config ERROR : %s\n",str.c_str());
			fflush(stdout);
		}
		else
			mTCAstate->mConfig = str;
	}
	else {
		printf("TCA::set_config ERROR : %s\n",str.c_str());
		fflush(stdout);
	}
}


void TCA::set_function(string str) {
	if(mTCAstate->mPropulsionClass==UNDEFINED) {
		printf("TCA::set_function ERROR : UNDEFINED propulsion class\n");
		fflush(stdout);
		return ;
	}
	if(str=="steering")
		mTCAstate->mFunction = str;
	else if(str=="hovering") {
		if(mTCAstate->mPropulsionClass==DIFFERENTIAL_PROPELLERS) {
			printf("TCA::set_function ERROR : %s invalid with differential_propellers\n",str.c_str());
			fflush(stdout);
		}
		else if(mTCAstate->mConfig=="surge_yaw") {
			printf("TCA::set_function ERROR : %s invalid with %s\n",str.c_str(),mTCAstate->mConfig.c_str());
			fflush(stdout);
		}
		else
			mTCAstate->mFunction = str;
	}
	else {
		printf("TCA::set_function ERROR : %s\n",str.c_str());
		fflush(stdout);
	}
}


void TCA::set_mode(string  str) {
	if((str==TCA_AUTO)||(str==TCA_MANUAL)||(str==TCA_RAW)) {
		mTCAstate->mMode=str;
	}
	else {
		printf("TCA::set_mode unexpected mode %s\n",str.c_str());
		fflush(stdout);
	}
}

void TCA::set_input_output(XYN *XYNref,XYNnorm *XYNnormRef,XYN *XYNhat,XYNnorm *XYNnormHat) {
	mXYNref=XYNref;
	mXYNnormRef=XYNnormRef;
	mXYNhat=XYNhat;
	mXYNnormHat=XYNnormHat;
//	TCA::reset();
}


void TCA::execute(void) {
	mNopCounter++;

	if((mNopCounter > NOP_THRESHOLD) && mNopHandlerEnable)
		handle_nop_emergency();

	//
	// compute applied force and torque according to actuator data
	//
	compute_applied_force_torque();

	//printf("TCA::execute TCAstate %d %d %s %s %s\n",
			//mTCAstate->mPropulsionClass,(int)mTCAstate->mEnable,
			//mTCAstate->mMode.c_str(),mTCAstate->mFunction.c_str(),mTCAstate->mConfig.c_str());
	//fflush(stdout);

	//
	// allocate thrust on actuators
	//
	if(mTCAstate->mMode==TCA_AUTO)
		execute_auto();
	else if(mTCAstate->mMode==TCA_MANUAL)
		execute_manual();
	else
		execute_raw();
//	printf_results();
}


void TCA::standard_to_normalised_ref(void) {
	switch(mTCAstate->mPropulsionClass) {
	case DIFFERENTIAL_PROPELLERS:
		mXYNnormRef->X=mXYNref->X/(2*Tmax);
		mXYNnormRef->Y=mXYNref->Y/(2*Tmax);
		mXYNnormRef->N=mXYNref->N/((kDiff+1)*d*Tmax);
		break;
	case AZIMUTH_JETS_4:
		mXYNnormRef->X=mXYNref->X/(4*Tmax);
		mXYNnormRef->Y=mXYNref->Y/(4*Tmax);
		mXYNnormRef->N=mXYNref->N/(4*sqrt(l*l+d*d)*Tmax);
		break;
	default:
		printf("TCA::standard_to_normalised_ref Error mTCAstate->mPropulsionClass %d\n",mTCAstate->mPropulsionClass);
		fflush(stdout);
		break;
	}
	//
	// normalised reference are in percentage
	//
	mXYNnormRef->X*=100.;
	mXYNnormRef->Y*=100.;
	mXYNnormRef->N*=100.;
}

void TCA::normalised_to_standard_ref(void) {
	switch(mTCAstate->mPropulsionClass) {
	case DIFFERENTIAL_PROPELLERS:
		mXYNref->X=2*Tmax*mXYNnormRef->X;
		mXYNref->Y=2*Tmax*mXYNnormRef->Y;
		mXYNref->N=(kDiff+1)*d*Tmax*mXYNnormRef->N;
		break;
	case AZIMUTH_JETS_4:
		mXYNref->X=4*Tmax*mXYNnormRef->X;
		mXYNref->Y=4*Tmax*mXYNnormRef->Y;
		mXYNref->N=4*sqrt(l*l+d*d)*Tmax*mXYNnormRef->N;
		break;
	default:
		printf("TCA::normalised_to_standard_ref Error mTCAstate->mPropulsionClass %d\n",mTCAstate->mPropulsionClass);
		fflush(stdout);
		break;
	}
}

void TCA::normalised_to_standard_hat(void) {
	switch(mTCAstate->mPropulsionClass) {
	case DIFFERENTIAL_PROPELLERS:
		mXYNhat->X=2*Tmax*mXYNnormHat->X;
		mXYNhat->Y=2*Tmax*mXYNnormHat->Y;
		mXYNhat->N=(kDiff+1)*d*Tmax*mXYNnormHat->N;
		break;
	case AZIMUTH_JETS_4:
		mXYNhat->X=4*Tmax*mXYNnormHat->X;
		mXYNhat->Y=4*Tmax*mXYNnormHat->Y;
		mXYNhat->N=4*sqrt(l*l+d*d)*Tmax*mXYNnormHat->N;
		break;
	default:
		printf("TCA::normalised_to_standard_hat Error mTCAstate->mPropulsionClass %d\n",mTCAstate->mPropulsionClass);
		fflush(stdout);
		break;
	}
}

bool TCA::read_configuration(void) {
//bool TCA::fread(string filename) {

	string fileName;
	fileName=mConfigDir+mName+"-TCAparam.cfg";
	FILE *f;
	if((f=fopen(fileName.c_str(),"r"))==NULL) {
		printf("ERROR: TCA::read_configuration - opening file %s\n",fileName.c_str());
		return(false);
	}
//	if((f=fopen(filename.c_str(),"r"))==NULL) {
//		printf("ERROR: bool TCA::fread(string fileName) - %s not found!\n",filename.c_str());
//		return false;
//	}

	char str[256];

	// maximum thrust
	fscanf(f,"%s %lf",str,&Tmax);
    if(strcmp(str,"Tmax")!=0)
    {
        printf("TCA::read_configuration Tmax expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// thrust reduction for counter rotation in propeller differential mode
	fscanf(f,"%s %lf",str,&kDiff);
    if(strcmp(str,"kDiff")!=0)
    {
        printf("TCA::read_configuration kDiff expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// sway semi-distance between actuators
	fscanf(f,"%s %lf",str,&d);
    if(strcmp(str,"d")!=0)
    {
        printf("TCA::read_configuration d expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// surge semi-distance between actuators
	fscanf(f,"%s %lf",str,&l);
    if(strcmp(str,"l")!=0)
    {
        printf("TCA::read_configuration l expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// sway force error relative weight
	fscanf(f,"%s %lf",str,&lambdaY);
    if(strcmp(str,"lambdaY")!=0)
    {
        printf("TCA::read_configuration lambdaY expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// yaw moment error relative weight
	fscanf(f,"%s %lf",str,&lambdaN);
    if(strcmp(str,"lambdaN")!=0)
    {
        printf("TCA::read_configuration lambdaN expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// thrust displacement with respect default working point relative weight
	fscanf(f,"%s %lf",str,&lambdaT);
    if(strcmp(str,"lambdaT")!=0)
    {
        printf("TCA::read_configuration lambdaT expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
	// azimuth displacement with respect default working point relative weight
	fscanf(f,"%s %lf",str,&lambdaAlpha);
    if(strcmp(str,"lambdaAlpha")!=0)
    {
        printf("TCA::read_configuration lambdaAlpha expected %s found!\n",str);fflush(stdout);
        fclose(f);
        return false;
    }
    fclose(f);

    //
    // set TCA algo parameters
    //
    mTCAalgo.set_parameters(kDiff, d, l, lambdaY, lambdaN, lambdaT, lambdaAlpha);

	return true;
}


void TCA::execute_auto(void) {
//	printf("TCA::execute_auto\n");fflush(stdout);
    //
    // normalised reference in [-1,1]
    //
	double XtildeRef = mXYNnormRef->X/100.;
	double YtildeRef = mXYNnormRef->Y/100.;
	double NtildeRef = mXYNnormRef->N/100.;

	switch(mTCAstate->mPropulsionClass) {
	case DIFFERENTIAL_PROPELLERS:
		if((mTCAstate->mConfig=="surge_yaw")&&(mTCAstate->mFunction=="steering")) {
			mTCAalgo.TCA_differential_steering(XtildeRef, NtildeRef, mThrustNorm[0], mThrustNorm[1]);
			mThrust[0]=Tmax*mThrustNorm[0];
			mThrust[1]=Tmax*mThrustNorm[1];
		}
		else {
			printf("TCA::execute_auto Error - invalid state: %d %s %s\n",
					mTCAstate->mPropulsionClass,mTCAstate->mConfig.c_str(),mTCAstate->mFunction.c_str());
			fflush(stdout);
		}
		break;
	case AZIMUTH_JETS_4:
		if((mTCAstate->mConfig=="surge_yaw")&&(mTCAstate->mFunction=="steering")) {
			double Ttilde,alpha;
			mTCAalgo.TCA_thrust_azimuth_steering(XtildeRef, NtildeRef, Ttilde, alpha);
			mThrust[0]=mThrust[1]=mThrust[2]=mThrust[3]=Tmax*Ttilde;
			mAzimuth[0] = mAzimuth[2] = -alpha*180./M_PI;
			mAzimuth[1] = mAzimuth[3] = alpha*180./M_PI;
		}
//		else if((mTCAstate->mConfig=="diamond")&&(mTCAstate->mFunction=="steering")) {
//			printf("TCA::execute_auto diamond-steering to be implemented!\n");fflush(stdout);
//		}
//		else if((mTCAstate->mConfig=="diamond")&&(mTCAstate->mFunction=="hovering")) {
		else if((mTCAstate->mConfig=="diamond")&&((mTCAstate->mFunction=="hovering")||(mTCAstate->mFunction=="steering"))) {
			double alpha;
			mTCAalgo.TCA_thrust_azimuth_diamond(XtildeRef, YtildeRef, NtildeRef, mThrustNorm, alpha);
			mThrust[0] = Tmax*mThrustNorm[0];
			mThrust[1] = Tmax*mThrustNorm[1];
			mThrust[2] = Tmax*mThrustNorm[2];
			mThrust[3] = Tmax*mThrustNorm[3];
			mAzimuth[0] = summodpi(M_PI,-alpha)*180./M_PI;
			mAzimuth[1] = alpha*180./M_PI;
			mAzimuth[2] = summodpi(alpha,-M_PI)*180./M_PI;
			mAzimuth[3] = -alpha*180./M_PI;
		}
//		else if((mTCAstate->mConfig=="free")&&(mTCAstate->mFunction=="steering")) {
//			printf("TCA::execute_auto free-steering to be implemented!\n");fflush(stdout);
//		}
//		else if((mTCAstate->mConfig=="free")&&(mTCAstate->mFunction=="hovering")) {
		else if((mTCAstate->mConfig=="free")&&((mTCAstate->mFunction=="hovering")||(mTCAstate->mFunction=="steering"))) {
			printf("TCA::execute_auto free-hovering/steering to be implemented!\n");fflush(stdout);
		}
		else {
			printf("TCA::execute_auto Error - invalid state: %d %s %s\n",
					mTCAstate->mPropulsionClass,mTCAstate->mConfig.c_str(),mTCAstate->mFunction.c_str());
			fflush(stdout);
		}
		break;
	default:
		printf("TCA::execute_auto Error - invalid propulsion class: %d\n",
				mTCAstate->mPropulsionClass);
		fflush(stdout);
		break;
	}
}


