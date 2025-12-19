/*
 * SwampPlantThread.h
 *
 *  Created on: Mar 3, 2025
 *      Author: mc
 *
 *  SwampPlantThread is the child class for PlantThread
 */

#ifndef SWAMPPLANTTHREAD_H_
#define SWAMPPLANTTHREAD_H_

#include "PlantThread.h"

#define AZIMUTH 0
#define PUMP 1

//#include <list>

class SwampPlantThread: public PlantThread {
public:
	SwampPlantThread();
	virtual ~SwampPlantThread();

	void make_thread(void);

	bool init_configuration(void);

protected:
    void enable_actuator(uint8_t id,uint8_t enable);
    void power_actuator(uint8_t id,uint8_t power);
    void power_sensor(uint8_t id,uint8_t power)
		{ printf("SwampPlantThread::power_sensor\n"); fflush(stdout); }
    void power_device(uint8_t id,uint8_t power)
    	{ printf("SwampPlantThread::power_device\n"); fflush(stdout); }
    void init_actuator(uint8_t id,double timeout);
    void init_sensor(uint8_t id,double timeout)
    	{ printf("SwampPlantThread::init_sensor\n"); fflush(stdout); }
    void init_device(uint8_t id,double timeout)
    	{ printf("SwampPlantThread::init_device\n"); fflush(stdout); }

    void process_data(char *data,ssize_t nRecv);
    void execute_thread_semantics(void);
    void build_data(void);

};



#endif /* PLANTTHREAD_H_ */
