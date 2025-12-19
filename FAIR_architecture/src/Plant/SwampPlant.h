/*
 * SwampPlant.h
 *
 *  Created on: 27 feb 2025
 *      Author: massimo
 */

#ifndef SWAMPTCA_H_
#define SWAMPTCA_H_

#include <stdio.h>

#include <inttypes.h>
#include "Plant.h"


class SwampPlant : public Plant {
public:
	// constructor/destructor
	SwampPlant(void);
    virtual ~SwampPlant(void);
    void reset(void);
    // configure

private:


private:



};



#endif /* SWAMPTCA_H_ */
