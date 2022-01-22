#include "hofv.h"

int (*HOFV_isAdmissible)(const double rho, const double S, const double u[2]) = NULL;
int (*HOFV_priToCons)(const double rho, const double S, double p[2], double c[2]) = NULL;
int (*HOFV_consToPri)(const double rho, const double S, double c[2], double p[2]) = NULL;
int (*HOFV_waveSpeeds)(const double ul[2], const double ur[2], double *s1, double *s2) = NULL;
int (*HOFV_phyFlux)(const double u[2], double flux[2]) = NULL;
int (*HOFV_numFlux)(const double ul[2], const double ur[2], double flux[2]) = NULL;

