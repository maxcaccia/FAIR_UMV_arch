/*
 * SwampVesselThread.h
 *
 *  Created on: Jan 12, 2024
 *      Author: mc
 */

#ifndef SWAMPVESSELTHREAD_H_
#define SWAMPVESSELTHREAD_H_

#include "RigidBodyVesselThread.h"

class SwampVesselThread: public RigidBodyVesselThread {
public:
	SwampVesselThread();
	virtual ~SwampVesselThread();

	void make_thread(void);

protected:
    void process_force_moment_command(); // function to be overwritten
};



#endif /* SWAMPVESSELTHREAD_H_ */
