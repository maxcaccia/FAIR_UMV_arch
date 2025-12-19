/*
 * GuidanceControlReference.h
 *
 *  Created on: 8 May 2024
 *      Author: massimo
 */
#ifndef GUIDANCECONTROLREFERENCE_H_
#define GUIDANCECONTROLREFERENCE_H_

#include "TimeStampedData.h"

#define GUIDANCE_REFERENCE_CODE "GDNC_REF"
#define AUTOPILOT_REFERENCE_CODE "APLT_REF"
#define CONTROL_REFERENCE_CODE "CNTRL_REF"

class GuidanceReference : public TimeStampedData {
public:
	// local Earth-fixed NED frame coordinates
	double lat0; // [deg.dddddddd]
	double lon0; // [deg.dddddddd]
	// Line-Following: latitude, longitude, and orientation of a reference line point
	double latL; // [deg.dddddddd]
	double lonL; // [deg.dddddddd]
	double gammaL; // [deg]
	// GO-TO , KEEP-POINT: latitude and longitude of the reference way-point
	double latWP; // [deg.dddddddd]
	double lonWP; // [deg.dddddddd]
	// KEEP-POINT: reference heading at the desired way-point
	double psiWP; // [deg]
	// desired linear velocity in vehicle-fixed frame
	double uBar; // [m/s]
public:
	GuidanceReference(string name);
	GuidanceReference(void);
    ~GuidanceReference(void) {};

    GuidanceReference & operator = (const GuidanceReference &obj);

//    bool fread(string fileName);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
	int sprint(char *s);
	void sprint_csv(char *s);
	// read data: decode a packet created by sprint(char *s)
	void sread(char *s);

private:
	void reset(void);
};


class AutopilotReference : public TimeStampedData {
public:
	// AUTO-HEADING: reference heading
	double psi; // [deg]
	// AUTO-COURSE: reference course
	double chi; // [deg]
	// desired linear velocity in vehicle-fixed frame
	double uBar; // [m/s]
public:
	AutopilotReference(string name);
	AutopilotReference(void);
    ~AutopilotReference(void) {};

    AutopilotReference & operator = (const AutopilotReference &obj);

//    bool fread(string fileName);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
	int sprint(char *s);
	void sprint_csv(char *s);
	// read data: decode a packet created by sprint(char *s)
	void sread(char *s);

private:
	void reset(void);
};


class ControlReference : public TimeStampedData {
public:
	// surge, sway, yawrate
	double u; // [m/s]
	double v; // [m/s]
	double r; // [deg/s]
public:
	ControlReference(string name);
	ControlReference(void);
    ~ControlReference(void) {};

    ControlReference & operator = (const ControlReference &obj);

//    bool fread(string fileName);

	// print headers
	void sprint_csv_log_name(char *s);
	void sprint_csv_long_name(char *s);
	// print data
	int sprint(char *s);
	void sprint_csv(char *s);
	// read data: decode a packet created by sprint(char *s)
	void sread(char *s);

private:
	void reset(void);
};


#endif /* GUIDANCECONTROLREFERENCE_H_ */
