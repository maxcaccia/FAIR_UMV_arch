/*
 * MinionDriverInterface.h
 *
 *  Created on: 15 apr 2022
 *      Author: massimo
 */

#ifndef MINIONDRIVERINTERFACE_H_
#define MINIONDRIVERINTERFACE_H_

typedef enum
   {
   COMMAND_NOP_OPCODE                                      = 999   ,
   COMMAND_SET_DIGITAL_CHANNEL_OPCODE                      =1000+ 0,
   COMMAND_SET_ANALOG_CHANNEL_OPCODE                       =1000+ 1,
   COMMAND_MOTOR_POWER_OPCODE                              =1000+ 2,
   COMMAND_MOTOR_ENABLE_OPCODE                             =1000+ 3,
   COMMAND_MOTOR_SET_REFERENCE_OPCODE                      =1000+ 4,
   COMMAND_AZIMUTHAL_MOTOR_SET_MAX_SPEED_OPCODE            =1000+ 5,
   COMMAND_AZIMUTHAL_MOTOR_SET_HOME_OPCODE                 =1000+ 6,
   COMMAND_AZIMUTHAL_MOTOR_GO_HOME_OPCODE                  =1000+ 7,
   COMMAND_SHUTDOWN_OPCODE                                 =10000+ 0,
   COMMAND_REBOOT_OPCODE                                   =10000+ 1,
   COMMAND_CHANGE_TELEMETRY_IP_ADDRESS_AND_UDP_PORT_OPCODE =10000+ 2,
   COMMAND_ENABLE_DEBUG_LOGGING_OPCODE                     =10000+ 3
   }CommandMinionOpCodeEnum;

struct MinionMessage {
		int nodeId;
		uint32_t dateAndTime;
		uint32_t timeMs; //
		uint16_t digitalInput;
		uint16_t digitalOutput;
	    double batteryVoltage;
	    double thrustMotorTemperature;
	    double thrustMotorSpeed;
		double thrustMotorCurrent;
	    int32_t azimuthMotorPosition; // ticks
	    uint16_t azimuthMotorConfigurationStatus;
	    uint32_t azimuthMotorOperationStatus;
	    int16_t azimuthMotorTemperature;
	    int32_t azimuthMotorCurrent;
	    double imuYaw;
	    double imuPitch;
	    double imuRoll;
	    double imuXGyro;
	    double imuYGyro;
	    double imuZGyro;
	    uint8_t imuTemperature;
	    uint8_t imuCalibrationStatus;
	    double gpsTime; // hhmmss.ss in UTC
	    double gpsLatitude;
	    double gpsLongitude; // tenths of degree
	    double gpsAltitude;
	    uint16_t gpsYear;
	    uint8_t gpsMonth;
	    uint8_t gpsDay;
	    uint8_t gpsFixQuality;
	    double gpsHDOP;
	    double gpsHeightGeoid;
};

typedef enum
   {
   THRUST_MOTOR        =0,
   AZIMUTHAL_MOTOR     =1
   }MotorEnum;

typedef enum
	{
      // DI_SHUTDOWN_BUTTON (GPIO4) is used by the shutdown daemon
      DI_THRUST_MOTOR_FAULT          	= 0, // GPIO20
      DI_AZIMUTHAL_MOTOR_FAULT       	= 1  // GPIO21
	}DigitalInputEnum;

typedef enum
	   {
	// DO_SHUTDOWN_LED (GPIO5) is used by the shutdown daemon
	   DO_CPU_ALIVE_LED                	= 0, // GPIO6
	   DO_ETHERNET_RX_LED              	= 1, // GPIO12
	   DO_THRUST_MOTOR_POWER_ENABLE_LED	= 2, // GPIO13
	   DO_AZIMUTHAL_MOTOR_POWER_ENABLE_LED = 3, // GPIO16
	   DO_THRUST_MOTOR_POWER           	= 4, // GPIO17
	   DO_THRUST_MOTOR_ENABLE          	= 5, // GPIO18
	   DO_AZIMUTHAL_MOTOR_POWER        	= 6  // GPIO19
	   }DigitalOutputEnum;

#define THRUST_RPM_MAX 3240.
#define THRUST_SPEED_PERC_FACTOR (THRUST_RPM_MAX/4.0)

#define TICKS_PER_ROUND 939804.444

#define MAX_POS_MOT_SPEED 4500


#endif /* MINIONDRIVERINTERFACE_H_ */
