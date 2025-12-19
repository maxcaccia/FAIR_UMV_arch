/*
 * BlueBoatVesselThread.h
 *
 *  Created on: Mar 26, 2025
 *      Author: mc
 */

#ifndef BLUEBOATVESSELTHREAD_H_
#define BLUEBOATVESSELTHREAD_H_

#include "RigidBodyVesselThread.h"

class BlueBoatVesselThread: public RigidBodyVesselThread {
public:
	BlueBoatVesselThread();
	virtual ~BlueBoatVesselThread();

	void make_thread(void);

	void dynamics(void);

protected:
    void process_force_moment_command(); // function to be overwritten
};



#endif /* BLUEBOATVESSELTHREAD_H_ */
