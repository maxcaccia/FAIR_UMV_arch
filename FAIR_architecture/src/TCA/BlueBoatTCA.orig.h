/*
 * BlueBoatTCA.h
 *
 *  Created on:18 June 2024
 *      Author: massimo
 */

#ifndef BLUEBOATTCA_H_
#define BLUEBOATTCA_H_

#include <stdio.h>

#include <inttypes.h>
#include "TCA.h"
#include "NormalisedRpm.h"
#include "Matrix.h"

//
// BlueBoat TCA functions in TCA_AUTO mode and corresponding configurations:
//		"null"
//

//
// BlueBoat TCA functions in TCA_MANUAL mode and corresponding configurations:
//		"null"
//

//
// BlueBoat look-up table size
#define TABLE_SIZE 201

#define K_THRUST_BLUEBOAT 0.3898

class BlueBoatTCA : public TCA {
private:
	// Input
	NormalisedRpm* mBlueBoatRpmMeas; // measured robot actuation

	// Output
	NormalisedRpm* mBlueBoatRpmRef; // reference robot actuation

	//
	// look-up tables
	//
	double mLeftLookupTable[TABLE_SIZE][TABLE_SIZE];
	double mRightLookupTable[TABLE_SIZE][TABLE_SIZE];

public:
	// constructor/destructor
	BlueBoatTCA(void);
    virtual ~BlueBoatTCA(void);
    void reset(void);
    // configure
    void set_mode(string  str);
    void set_left_right_rpm(double nL,double nR);

    void set_input_output(NormalisedRpm* blueBoatRpmRef,NormalisedRpm* blueBoatRpmMeas);

    void set_manual_default(void);
    void set_auto_default(void);

private:
    void execute_manual(void);
    void execute_auto(void);

    void TCA_BlueBoat(void);

    void compute_applied_force_torque(void);
    bool load_lookup_table(const char *filename, double table[TABLE_SIZE][TABLE_SIZE]);

public:
    bool read_lookup_tables(void);
};



#endif /* BLUEBOATTCA_H_ */
