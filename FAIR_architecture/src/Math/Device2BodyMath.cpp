//
//  Device2BodyMath.cpp
//  handles transformations between device data to data from a device mounted in the vehicle center of mass
//
//  Created by Massimo Caccia on 8/01/25.
//

#include "Device2BodyMath.h"

	//
	// reference frames
	//	E: NED frame
	//	B: surge-sway-heave rigid body -fixed frame
	//	D: device -fixed frame
	//


//
// compute rigid-body center of mass position given GNSS position measurement (Earth-fixed NED frame)
//
void compute_GNSS2RB_position(GnssPositionData &rbPos,
							GnssPositionData devPos,
							DevicePose mPose,AngularPosition rbAttitude) {
	//
	// an Eart-fixed NED frame <E> is positioned at the GNSS (device) measuured latitude and longitude
	//

	//	delta_d_B : device pose in <B>
	Vector delta_d_B(mPose.x,mPose.y,mPose.z);
	//	R_EB : rotation matrix from <E> to <B>
	Matrix3x3 R_EB;
	phi_theta_psi_2_Rzyx(R_EB,rbAttitude.phi,rbAttitude.theta,rbAttitude.psi);

	//	dp_E : Center of mass displacement in <E>
	Vector dp_E;
	dp_E = -1* (R_EB * delta_d_B);

	// compute latitude and longitude of the vehicle center of mass
	rbPos=devPos;
	lat_lon_plus_x_y(rbPos.latitude,rbPos.longitude,devPos.latitude,devPos.longitude,dp_E[0],dp_E[1]);
}


//
// compute rigid-body attitude given AHRS measurement
//
void compute_AHRS2RB_attitude(AhrsData &rbAttitude,
								AhrsData devAttitude,
								DevicePose mPose) {
	//
	// roll-pitch-yaw angles of <B> in <E> are computed
	//

	//	R_BD : rotation matrix from <B> to <D>
	Matrix3x3 R_BD;
	phi_theta_psi_2_Rzyx(R_BD,mPose.phi,mPose.theta,mPose.psi);
	//	R_ED : rotation matrix from <E> to <D>
	Matrix3x3 R_ED;
	phi_theta_psi_2_Rzyx(R_ED,devAttitude.phi,devAttitude.theta,devAttitude.psi);

	//	R_EB : rotation matrix from <E> to <B>
	Matrix3x3 R_EB;
	R_EB = R_ED * R_BD.t();
	double phi,theta,psi;
	Rzyx_2_phi_theta_psi(phi,theta,psi,R_EB);
	rbAttitude.phi=phi*180./M_PI;
	rbAttitude.theta=theta*180./M_PI;
	rbAttitude.psi=psi*180./M_PI;

	//
	// roll-pitch-yaw rates around <B> axis are computed
	//

	//	omega_D : omega in <D>
	Vector omega_D(devAttitude.p*M_PI/180.,devAttitude.q*M_PI/180.,devAttitude.r*M_PI/180.);
	//	omega_B : omega in <B>
	Vector omega_B;
	omega_B = R_BD * omega_D;

	rbAttitude.p = omega_B[0]*180./M_PI;
	rbAttitude.q = omega_B[1]*180./M_PI;
	rbAttitude.r = omega_B[2]*180./M_PI;

}


//
// compute rigid-body center of mass linear velocity given GNSS speed measurement (Earth-fixed NED frame)
//
void compute_GNSS2RB_speed(GnssVelocityData &rbVel,
						GnssVelocityData devVel,
						DevicePose mPose,AngularPosition rbAttitude,AngularVelocity rbAngVel) {

	//	delta_d_B : device pose in <B>
	Vector delta_d_B(mPose.x,mPose.y,mPose.z);
	//	R_EB : rotation matrix from <E> to <B>
	Matrix3x3 R_EB;
	phi_theta_psi_2_Rzyx(R_EB,rbAttitude.phi,rbAttitude.theta,rbAttitude.psi);
	//	omega_B : omega in <B>
	Vector omega_B(rbAngVel.p*M_PI/180.,rbAngVel.q*M_PI/180.,rbAngVel.r*M_PI/180.);

	//	dDot_E : device speed in <E>, computed from VTG sentence
	Vector dDot_E(devVel.speed*cos(devVel.track*M_PI/180.),devVel.speed*sin(devVel.track*M_PI/180.),0);

	// pDot_E : center of mass speed in <E>
	Vector pDot_E;
	pDot_E = dDot_E - R_EB * ( omega_B % delta_d_B);
	double speed = sqrt(pDot_E[0]*pDot_E[0]+pDot_E[1]*pDot_E[1]);
	double track = atan2(pDot_E[1],pDot_E[0])*180./M_PI;

	rbVel.speed = speed;
	rbVel.track = track;

}


//
// compute rigid-body center of mass linear velocity given DVL speed measurement (body-fixed frame)
//
void compute_DVL2RB_speed(DvlData &rbVel,
						DvlData devVel,
						DevicePose mPose,AngularVelocity rbAngVel) {

	//	delta_d_B : device pose in <B>
	Vector delta_d_B(mPose.x,mPose.y,mPose.z);
	//	R_BD : rotation matrix from <B> to <D>
	Matrix3x3 R_BD;
	phi_theta_psi_2_Rzyx(R_BD,mPose.phi,mPose.theta,mPose.psi);
	//	omega_B : omega in <B>
	Vector omega_B(rbAngVel.p*M_PI/180.,rbAngVel.q*M_PI/180.,rbAngVel.r*M_PI/180.);

	//	dDot_D : device speed in <D>
	Vector dDot_D(devVel.u,devVel.v,0);

	// pDot_B : center of mass speed in <B>
	Vector pDot_B;
	pDot_B = R_BD * dDot_D + omega_B % delta_d_B;

	rbVel=devVel;
	rbVel.u = pDot_B[0];
	rbVel.v = pDot_B[1];

}


