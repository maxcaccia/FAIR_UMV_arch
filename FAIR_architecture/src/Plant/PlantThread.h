/*
 * PlantThread.h
 *
 *  Created on: Feb 27, 2025
 *      Author: mc
 *
 *  PlantThread is the parent class for <Robot>P2LSMthread
 */

#ifndef PLANTTHREAD_H_
#define PLANTTHREAD_H_

#include "CntrlSimThread.h"
#include "PlantState.h"
#include "Plant.h"
#include "Commands.h"

//#include <list>

class PlantThread: public CntrlSimThread {
public:
	PlantThread();
	virtual ~PlantThread();

	virtual void make_thread(void);

	virtual bool init_configuration(void);

protected:
    void process_command(char *cmd);
    void process_enable_actuator(uint8_t id,uint8_t enable);
    void process_power_actuator(uint8_t id,uint8_t power);
    void process_power_sensor(uint8_t id,uint8_t power);
    void process_power_device(uint8_t id,uint8_t power);
    void process_init_actuator(uint8_t id,double timeout);
    void process_init_sensor(uint8_t id,double timeout);
    void process_init_device(uint8_t id,double timeout);

    virtual void enable_actuator(uint8_t id,uint8_t enable)
    	{ printf("virtual PlantThread::enable_actuator\n"); fflush(stdout); }
    virtual void power_actuator(uint8_t id,uint8_t power)
		{ printf("virtual PlantThread::power_actuator\n"); fflush(stdout); }
    virtual void power_sensor(uint8_t id,uint8_t power)
		{ printf("virtual PlantThread::power_sensor\n"); fflush(stdout); }
    virtual void power_device(uint8_t id,uint8_t power)
		{ printf("virtual PlantThread::power_device\n"); fflush(stdout); }
    virtual void init_actuator(uint8_t id,double timeout);
    virtual void init_sensor(uint8_t id,double timeout)
		{ printf("virtual PlantThread::init_sensor\n"); fflush(stdout); }
    virtual void init_device(uint8_t id,double timeout)
		{ printf("virtual PlantThread::init_device\n"); fflush(stdout); }

    virtual void process_data(char *data,ssize_t nRecv);
    virtual void execute_thread_semantics(void);
    virtual void build_data(void);

private:
    //
    // Plant state : active logical sensors
    //
    PlantState mPlantState;

    ////////////
    //
    // Plant module
    //
    Plant* mPlant;

protected:
    PlantCmd mPlantCmd;

};



#endif /* PLANTTHREAD_H_ */
