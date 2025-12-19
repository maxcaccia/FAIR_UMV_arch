//
//  SensorInfo.h
//  Sensor info: handles sensor name and kinematics of a device mounted on a rigid-body
//
//  Created by Massimo Caccia on 31/07/24.
//

#include "SensorInfo.h"

void MeasurementUncertainty::reset(void) {
	latitude_stdev = -1;
	longitude_stdev = -1;
	roll_stdev = -1;
	pitch_stdev = -1;
	yaw_stdev = -1;
	speed_stdev = -1;
	course_std = -1;
	surge_rate_stdev = -1;
	sway_rate_stdev = -1;
	heave_rate_stdev = -1;
	roll_rate_stdev = -1;
	pitch_rate_stdev = -1;
	yaw_rate_stdev = -1;
}


MeasurementUncertainty & MeasurementUncertainty::operator = (const MeasurementUncertainty &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;

	latitude_stdev = obj.latitude_stdev;
	longitude_stdev = obj.longitude_stdev;
	roll_stdev = obj.roll_stdev;
	pitch_stdev = obj.pitch_stdev;
	yaw_stdev = obj.yaw_stdev;
	speed_stdev = obj.speed_stdev;
	course_std = obj.course_std;
	surge_rate_stdev = obj.surge_rate_stdev;
	sway_rate_stdev = obj.sway_rate_stdev;
	heave_rate_stdev = obj.heave_rate_stdev;
	roll_rate_stdev = obj.roll_rate_stdev;
	pitch_rate_stdev = obj.pitch_rate_stdev;
	yaw_rate_stdev = obj.yaw_rate_stdev;

	// return a reference to myself
	return *this;
}


bool MeasurementUncertainty::fread(FILE *f) {
	char str[64];
	double value;

	fscanf(f,"%s %lf",str,&value);
	if(strcmp(str,"latitude_stdev")!=0) {
		printf("ERROR: MeasurementUncertainty::fread - read %s while expected latitude_stdev\n",str);
		return(false);
	}
	else
		latitude_stdev=value;

	fscanf(f,"%s %lf",str,&value);
	if(strcmp(str,"longitude_stdev")!=0) {
		printf("ERROR: MeasurementUncertainty::fread - read %s while expected longitude_stdev\n",str);
		return(false);
	}
	else
		longitude_stdev=value;

	fscanf(f,"%s %lf",str,&value);
	if(strcmp(str,"roll_stdev")!=0) {
		printf("ERROR: MeasurementUncertainty::fread - read %s while expected roll_stdev\n",str);
		return(false);
	}
	else
		roll_stdev=value;

	fscanf(f,"%s %lf",str,&value);
	if(strcmp(str,"pitch_stdev")!=0) {
		printf("ERROR: MeasurementUncertainty::fread - read %s while expected pitch_stdev\n",str);
		return(false);
	}
	else
		pitch_stdev=value;

	fscanf(f,"%s %lf",str,&value);
	if(strcmp(str,"yaw_stdev")!=0) {
		printf("ERROR: MeasurementUncertainty::fread - read %s while expected yaw_stdev\n",str);
		return(false);
	}
	else
		yaw_stdev=value;

	fscanf(f,"%s %lf",str,&value);
	if(strcmp(str,"speed_stdev")!=0) {
		printf("ERROR: MeasurementUncertainty::fread - read %s while expected speed_stdev\n",str);
		return(false);
	}
	else
		speed_stdev=value;

	fscanf(f,"%s %lf",str,&value);
	if(strcmp(str,"course_std")!=0) {
		printf("ERROR: MeasurementUncertainty::fread - read %s while expected course_std\n",str);
		return(false);
	}
	else
		course_std=value;

	fscanf(f,"%s %lf",str,&value);
	if(strcmp(str,"surge_rate_stdev")!=0) {
		printf("ERROR: MeasurementUncertainty::fread - read %s while expected surge_rate_stdev\n",str);
		return(false);
	}
	else
		surge_rate_stdev=value;

	fscanf(f,"%s %lf",str,&value);
	if(strcmp(str,"sway_rate_stdev")!=0) {
		printf("ERROR: MeasurementUncertainty::fread - read %s while expected sway_rate_stdev\n",str);
		return(false);
	}
	else
		sway_rate_stdev=value;

	fscanf(f,"%s %lf",str,&value);
	if(strcmp(str,"heave_rate_stdev")!=0) {
		printf("ERROR: MeasurementUncertainty::fread - read %s while expected heave_rate_stdev\n",str);
		return(false);
	}
	else
		heave_rate_stdev=value;

	fscanf(f,"%s %lf",str,&value);
	if(strcmp(str,"roll_rate_stdev")!=0) {
		printf("ERROR: MeasurementUncertainty::fread - read %s while expected roll_rate_stdev\n",str);
		return(false);
	}
	else
		roll_rate_stdev=value;

	fscanf(f,"%s %lf",str,&value);
	if(strcmp(str,"pitch_rate_stdev")!=0) {
		printf("ERROR: MeasurementUncertainty::fread - read %s while expected pitch_rate_stdev\n",str);
		return(false);
	}
	else
		pitch_rate_stdev=value;

	fscanf(f,"%s %lf",str,&value);
	if(strcmp(str,"yaw_rate_stdev")!=0) {
		printf("ERROR: MeasurementUncertainty::fread - read %s while expected yaw_rate_stdev\n",str);
		return(false);
	}
	else
		yaw_rate_stdev=value;

	return true;
}


void MeasurementUncertainty::print(void) {
	if(latitude_stdev!=-1.)
		printf("\t\tlatitude_stdev %.3lf\n",latitude_stdev);
	if(longitude_stdev!=-1.)
		printf("\t\tlongitude_stdev %.3lf\n",longitude_stdev);
	if(roll_stdev!=-1.)
		printf("\t\troll_stdev %.3lf\n",roll_stdev);
	if(pitch_stdev!=-1.)
		printf("\t\tpitch_stdev %.3lf\n",pitch_stdev);
	if(yaw_stdev!=-1.)
		printf("\t\tyaw_stdev %.3lf\n",yaw_stdev);
	if(speed_stdev!=-1.)
		printf("\t\tspeed_stdev %.3lf\n",speed_stdev);
	if(course_std!=-1.)
		printf("\t\tcourse_std %.3lf\n",course_std);
	if(surge_rate_stdev!=-1.)
		printf("\t\tsurge_rate_stdev %.3lf\n",surge_rate_stdev);
	if(sway_rate_stdev!=-1.)
		printf("\t\tsway_rate_stdev %.3lf\n",sway_rate_stdev);
	if(heave_rate_stdev!=-1.)
		printf("\t\theave_rate_stdev %.3lf\n",heave_rate_stdev);
	if(roll_rate_stdev!=-1.)
		printf("\t\troll_rate_stdev %.3lf\n",roll_rate_stdev);
	if(pitch_rate_stdev!=-1.)
		printf("\t\tpitch_rate_stdev %.3lf\n",pitch_rate_stdev);
	if(yaw_rate_stdev!=-1.)
		printf("\t\tyaw_rate_stdev %.3lf\n",yaw_rate_stdev);
}

bool MeasurementUncertainty::get_value(double &value,string s) {
//	printf("MeasurementUncertainty::get_value %s %lf\n",s.c_str(),value);fflush(stdout);
//	print();
	if((s=="latitude_stdev")&&(latitude_stdev!=-1.)) {
		value=latitude_stdev;
		return true;
	}
	if((s=="longitude_stdev")&&(longitude_stdev!=-1.)) {
		value=longitude_stdev;
		return true;
	}
	if((s=="roll_stdev")&&(roll_stdev!=-1.)) {
		value=roll_stdev;
		return true;
	}
	if((s=="pitch_stdev")&&(pitch_stdev!=-1.)) {
		value=pitch_stdev;
		return true;
	}
	if((s=="yaw_stdev")&&(yaw_stdev!=-1.)) {
		value=yaw_stdev;
		return true;
	}
	if((s=="speed_stdev")&&(speed_stdev!=-1.)) {
		value=speed_stdev;
		return true;
	}
	if((s=="course_std")&&(course_std!=-1.)) {
		value=course_std;
		return true;
	}
	if((s=="surge_rate_stdev")&&(surge_rate_stdev!=-1.)) {
		value=surge_rate_stdev;
		return true;
	}
	if((s=="sway_rate_stdev")&&(sway_rate_stdev!=-1.)) {
		value=sway_rate_stdev;
		return true;
	}
	if((s=="heave_rate_stdev")&&(heave_rate_stdev!=-1.)) {
		value=heave_rate_stdev;
		return true;
	}
	if((s=="roll_rate_stdev")&&(roll_rate_stdev!=-1.)) {
		value=roll_rate_stdev;
		return true;
	}
	if((s=="pitch_rate_stdev")&&(pitch_rate_stdev!=-1.)) {
		value=pitch_rate_stdev;
		return true;
	}
	if((s=="yaw_rate_stdev")&&(yaw_rate_stdev!=-1.)) {
		value=yaw_rate_stdev;
		return true;
	}

//	printf("MeasurementUncertainty::get_value : %s not found!\n",s.c_str()); fflush(stdout);
	return false;
}


void SensorInfo::reset(void) {
	mName="";
	mPose.reset();
//	mModelName="";
}

SensorInfo & SensorInfo::operator = (const SensorInfo &obj) {
	// check for self-assignment
	if(this==&obj)
		return *this;

	mName=obj.mName;
	mPose=obj.mPose;
	mModelName=obj.mModelName;
	mStd=obj.mStd;

	// return a reference to myself
	return *this;
}

bool SensorInfo::fread(FILE *f) {
	char model[256],str[256];
//	fscanf(f,"%s %s",nome,str);
//	if(strcmp(nome,"name")!=0) {
//		printf("ERROR: SensorInfo::fread - read %s while expected name\n",nome);
//		return(false);
//	}
//	mName=str;
	if(!mPose.fread(f)) {
		printf("ERROR: SensorInfo mPose.fread\n");
		return(false);
	}
	fscanf(f,"%s %s",model,str);
	if(strcmp(model,"model")!=0) {
		printf("ERROR: SensorInfo::fread - read %s while expected model\n",model);
		return(false);
	}
	mModelName.push_back(str);
	return true;
}

bool SensorInfo::fread_pose(FILE *f) {
	if(!mPose.fread(f)) {
		printf("ERROR: SensorInfo mPose.fread\n");
		return(false);
	}
	return true;
}

bool SensorInfo::fread_models(FILE *f,string configDir) {
//printf("SensorInfo::fread_models %s\n",configDir.c_str());

	char model[256];

	while(true) {
		fscanf(f,"%s",model);
		if(strcmp(model,"#end")==0)
			return true;
		else  { // process model
			string modelStr=model;
			FILE *fIn;
			string fileName;
		    fileName=configDir+modelStr+".cfg";
//printf("SensorInfo::fread_models open file %s\n",fileName.c_str());
			if((fIn=fopen(fileName.c_str(),"r"))==NULL) {
				printf("ERROR: ,string configDir - opening file %s\n",fileName.c_str());fflush(stdout);
				return(false);
			}
			MeasurementUncertainty measStd;
			if(!measStd.fread(fIn)) {
				printf("ERROR: SensorInfo measStd.fread %s\n",fileName.c_str());fflush(stdout);
				return(false);
			}
			mModelName.push_back(modelStr);
			mStd.push_back(measStd);
			fclose(fIn);
		}
	}
//	if(strcmp(model,"model")!=0) {
//		printf("ERROR: SensorInfo::fread - read %s while expected model\n",model);
//		return(false);
//	}

}


bool SensorInfo::get_value(double &value,string s) {
//	for(int i=0;i<(int)mStd.size();i++) {
//		printf("SensorInfo::get_value i %d %s\n",i,mModelName[i].c_str());
//	}
	for(int i=0;i<(int)mStd.size();i++) {
//		printf("SensorInfo::get_value i %d %s\n",i,mModelName[i].c_str());
		if(mStd[i].get_value(value, s))
			return true;
	}

	return false;
}


void SensorInfo::print(void) {
	printf("%s\n",mName.c_str());
	mPose.print();
	for(int i=0;i<(int)mModelName.size();i++) {
		printf("\t%s\n",mModelName[i].c_str());
		mStd[i].print();
	}

}

