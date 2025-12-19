/*
 * Plant.h
 *
 *  Created on: 27 feb 2025
 *      Author: massimo
 */

#ifndef PLANT_H_
#define PLANT_H_

#include <stdio.h>

#include <inttypes.h>
#include "PlantState.h"


class Plant {
protected:
	string mName;
	// state
	PlantState *mPlantState;

public:
	// constructor/destructor
	Plant();
    virtual ~Plant(void);
    virtual void reset(void) {mPlantState->reset();}
    void set_name(string name) { mName=name; }

    // configure
    void set_state(PlantState *plantState) { mPlantState=plantState; }

    // execute
	virtual void execute(void);

};


#endif /* PLANT_H_ */
