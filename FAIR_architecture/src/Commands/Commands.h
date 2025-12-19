/*
 * Commands.h
 *
 *  Created on: 5 February 2024
 *      Author: massimo
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <string>
#include <inttypes.h>

using namespace std;


class GenericCmd {
public:

public:
	GenericCmd(void) { reset(); }
    ~GenericCmd(void) {};

//    GenericCmd & operator = (const GenericCmd &obj);
    bool decode(char *packet,string &command);
    int code_nop(char *str);

private:
	void reset(void) { }
};

#define NO_ACTUATOR "null"

class RigidBodyCmd {
public:
	string actuatorId;
	double X;
	double Y;
	double Z;
	double K;
	double M;
	double N;
public:
	RigidBodyCmd(void) { reset(); }
    ~RigidBodyCmd(void) {};

    RigidBodyCmd & operator = (const RigidBodyCmd &obj);
    bool decode(char *packet,string &command);
    void set_name(string id) {actuatorId=id;}
    void set(double x,double y,double z,double k,double m,double n){X=x;Y=y;Z=z;K=k;M=m;N=n;}
    int code_set_force_moment(char *str);

private:
	void reset(void) { actuatorId=NO_ACTUATOR; X=Y=Z=K=M=N=0.; }
};

struct PositionMotorCmd {
public:
	double angle;
	uint8_t power;
	uint8_t enable;
	double maxSpeed;
public:
	PositionMotorCmd(void) { reset(); }
    ~PositionMotorCmd(void) {};

    PositionMotorCmd & operator = (const PositionMotorCmd &obj);
    bool decode(char *packet,string &command);
    // code commands
    int code_set_angle(char *str);
    int code_go_home(char *str);
    int code_set_home(char *str);
    int code_power(char *str);
    int code_enable(char *str);
    int code_set_max_speed(char *str);

private:
	void reset(void) { angle=maxSpeed=0.; enable=power=0; }
};

struct SpeedMotorCmd {
public:
	double speed;
	uint8_t power;
	uint8_t enable;
public:
	SpeedMotorCmd(void) { reset(); }
    ~SpeedMotorCmd(void) {};

    SpeedMotorCmd & operator = (const SpeedMotorCmd &obj);
    bool decode(char *packet,string &command);
    // code commands
    int code_set_speed(char *str);
    int code_power(char *str);
    int code_enable(char *str);

private:
	void reset(void) { speed=0.; enable=power=0; }
};


struct TcaCmd {
public:
	double X;
	double Y;
	double Z;
	double K;
	double M;
	double N;
	double Xnorm;
	double Ynorm;
	double Nnorm;
	string mode;
	string function;
	string config;
public:
	TcaCmd(void) { reset(); }
    ~TcaCmd(void) {};

    TcaCmd & operator = (const TcaCmd &obj);
    bool decode(char *packet,string &command);
    // code commands
    int code_start(char *str);
    int code_stop(char *str);
    int code_enable(char *str);
    int code_disable(char *str);
    int code_set_2dof(char *str);
    int code_set_3dof(char *str);
    int code_set_2dof_norm(char *str);
    int code_set_3dof_norm(char *str);
    int code_set_4dof(char *str);
    int code_set_6dof(char *str);
    int code_set_mode(char *str);
    int code_set_function(char *str);
    int code_set_config(char *str);

private:
	void reset(void) { X=Y=Z=K=M=N=0.; mode=function=config="null"; }
};



struct SwampTcaCmd {
public:
	//
	// reference rpm & azimuth
	//
	double n;
	double alpha;
	//
	// plant management
	//
	uint8_t power;
	uint8_t enable;
	double alphaOffset[4];
	string motorType;
	string motorId;

public:
	SwampTcaCmd(void) { reset(); }
    ~SwampTcaCmd(void) {};

    SwampTcaCmd & operator = (const SwampTcaCmd &obj);
    bool decode(char *packet,string &command);
    // code commands
    int code_enable_rpm_azimuth(char *str) { int number=sprintf(str,"SWAMP_TCA ENABLE_RPM_AZIMUTH\r\n");return number;}
    int code_disable_rpm_azimuth(char *str) { int number=sprintf(str,"SWAMP_TCA DISABLE_RPM_AZIMUTH\r\n");return number;}
    int code_set_rpm_azimuth(char *str);
    int code_set_power(char *str);
    int code_set_enable(char *str);
    int code_homing(char *str);
    int code_set_azimuth_offset(char *str);
    int code_set_azimuth_home(char *str);

private:
	void reset(void) { n=alpha=0.; power=enable=0; for(int i=0;i<4;i++) alphaOffset[i]=0; motorType=motorId="null";}
};





struct BlueBoatTcaCmd {
public:
	//
	// reference rpm (raw mode)
	//
	double nL; // [%]
	double nR; // [%]
	//
	// reference differential rpm (manual mode)
	//
	double n; // [%]
	double delta_n; // [%]

public:
	BlueBoatTcaCmd(void) { reset(); }
    ~BlueBoatTcaCmd(void) {};

    BlueBoatTcaCmd & operator = (const BlueBoatTcaCmd &obj);
    bool decode(char *packet,string &command);
    // code commands
    int code_set_left_right_rpm(char *str);
    int code_set_differential_rpm(char *str);
    int code_set_manual_mode(char *str);
    int code_set_guided_mode(char *str);
    int code_arm(char *str);
    int code_disarm(char *str);
    int code_map_servo_raw(char *str);
    int code_map_servo_default(char *str);

private:
	void reset(void) { nL=nR=0.; }
};




struct GuidanceCmd {
public:
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
	GuidanceCmd(void) { reset(); }
    ~GuidanceCmd(void) {};

    GuidanceCmd & operator = (const GuidanceCmd &obj);
    bool decode(char *packet,string &command);
    // code commands
    int code_set_keep_point(char *str);
    int code_set_way_point(char *str);
    int code_set_line(char *str);
    int code_set_path(char *str);

private:
	void reset(void) { latL=lonL=gammaL=latWP=lonWP=psiWP=uBar=0.; }
};


struct AutopilotCmd {
public:
	// AUTO-HEADING: reference heading
	double psi; // [deg]
	// AUTO-COURSE: reference course
	double chi; // [deg]
	// desired linear velocity in vehicle-fixed frame
	double uBar; // [m/s]

public:
	AutopilotCmd(void) { reset(); }
    ~AutopilotCmd(void) {};

    AutopilotCmd & operator = (const AutopilotCmd &obj);
    bool decode(char *packet,string &command);
    // code commands
    int code_set_heading(char *str);
    int code_set_course(char *str);

private:
	void reset(void) { psi=chi=uBar=0.; }
};


struct ControlCmd {
public:
	// surge, sway, yawrate
	double u; // [m/s]
	double v; // [m/s]
	double r; // [deg/s]

public:
	ControlCmd(void) { reset(); }
    ~ControlCmd(void) {};

    ControlCmd & operator = (const ControlCmd &obj);
    bool decode(char *packet,string &command);
    // code commands
    int code_set_surge(char *str);
    int code_set_sway(char *str);
    int code_set_yaw(char *str);

private:
	void reset(void) { u=v=r=0.; }
};


struct ExecCntrlCmd {
public:
	string task;
	string algo;

public:
	ExecCntrlCmd(void) { reset(); }
    ~ExecCntrlCmd(void) {};

    ExecCntrlCmd & operator = (const ExecCntrlCmd &obj);
    bool decode(char *packet,string &command);
    // code commands
    int code_enable_guidance(char *str);
    int code_disable_guidance(char *str);
    int code_enable_autopilot(char *str);
    int code_disable_autopilot(char *str);
    int code_enable_control(char *str);
    int code_disable_control(char *str);
    int code_set_guidance_algo(char *str);
    int code_set_autopilot_algo(char *str);
    int code_set_control_algo(char *str);

private:
	void reset(void) { task=algo=""; }
};


struct PlantCmd {
public:
	uint8_t power;
	uint8_t enable;
	uint8_t id;
	double timeout;
public:
	PlantCmd(void) { reset(); }
    ~PlantCmd(void) {};

    PlantCmd & operator = (const PlantCmd &obj);
    bool decode(char *packet,string &command);
    // code commands
    int code_enable_actuator(char *str);
    int code_power_actuator(char *str);
    int code_power_sensor(char *str);
    int code_power_device(char *str);
    int code_init_actuator(char *str);
    int code_init_sensor(char *str);
    int code_init_device(char *str);
private:
	void reset(void) { timeout=0.; enable=power=id=0; }
};


#define BLUE_BOAT_MANUAL "manual"
#define BLUE_BOAT_HOLD "hold"
#define BLUE_BOAT_GUIDED "guided"
#define BLUE_BOAT_ARM "arm"
#define BLUE_BOAT_DISARM "disarm"

struct BlueBoatCmd {
public:
	string mode;
	string arm;
//	double thrust;
//	double torque;
	double leftRpm; // [%]
	double rightRpm; // [%]
	double lat; // [deg]
	double lon; // [deg]
public:
	BlueBoatCmd(void) { reset(); }
    ~BlueBoatCmd(void) {};

    BlueBoatCmd & operator = (const BlueBoatCmd &obj);
    bool decode(char *packet,string &command);
    // code commands
    int code_set_mode(char *str);
    int code_set_arm(char *str);
//    int code_set_thrust_torque(char *str);
    int code_set_manual_ref(char *str);
    int code_map_servo_raw(char *str);
    int code_map_servo_default(char *str);
    int code_set_waypoint(char *str);

private:
	void reset(void) { mode=BLUE_BOAT_MANUAL; arm=BLUE_BOAT_DISARM; leftRpm=rightRpm=0.; }
};

#endif /* COMMANDS_H_ */
