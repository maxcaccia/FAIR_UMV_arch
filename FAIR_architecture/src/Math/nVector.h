/*
 * nVector.h
 *
 *  Created on: Jan 3, 2023
 *      Author: Massimo Caccia
 *
 *      This code is based on the following:
 *      The *n*-vector library is based on the following article, and hence it should be cited in publications using the library:
 *      > Kenneth Gade (2010): A Non-singular Horizontal Position Representation, *The Journal of Navigation*,
 *      Volume 63, Issue 03, pp 395-417, July 2010,
 *      [DOI: 10.1017/S0373463309990415](https://doi.org/10.1017/S0373463309990415).<br/>
 *      *<https://www.navlab.net/Publications/A_Nonsingular_Horizontal_Position_Representation.pdf>*
 */

#ifndef NVECTOR_H_
#define NVECTOR_H_

#include "Vector.h"
#include "Matrix.h"

#define aWGS84 6378137.
#define fWGS84 1/298.257223563


double deg2rad(double alpha);
double rad2deg(double alpha);

void lat_lon_2_nE(Vector &nE,double lat,double lon);
void nE_2_lat_lon(double &lat,double &lon,Vector nE);
void nE_2_North_East(Vector &kNorth,Vector &kEast,Vector nE);
void nE_h_2_pEB_WGS84(Vector &pEB,Vector nE,double h);
void pEB_2_nE_h_WGS84(Vector &nE,double &h,Vector pEB);
void nE_2_REN(Matrix3x3 &REN,Vector nE);

void lat_lon_plus_x_y(double &lat,double &lon,double lat0,double lon0,double x,double y);
void lat_lon_2_x_y(double &x,double &y,double lat0,double lon0,double lat1,double lon1);

void phi_theta_psi_2_Rzyx(Matrix3x3 &Rzyx,double phiDeg,double thetaDeg,double psiDeg);
void Rzyx_2_phi_theta_psi(double &phi,double &theta,double &psi,Matrix3x3 Rzyx);

#endif /* NVECTOR_H_ */
