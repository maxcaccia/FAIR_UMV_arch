/*
 * Plant.cpp
 *
 *  Created on: 27 feb 2025
 *      Author: massimo
 */

#include "Plant.h"

Plant::Plant(void) {
	mName="";
	mPlantState=NULL;
}

Plant::~Plant(void) {
	if(mPlantState!=NULL) {
		mPlantState=NULL;
	}
}


void Plant::execute(void) {
	printf("virtual void Plant::execute(void)\n"); fflush(stdout);
}



