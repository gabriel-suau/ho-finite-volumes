#include "externs_hofv.h"

int (*HOFV_isAdmissible)(const double rho, const double S, const double u[2]);
int (*HOFV_priToCons)(const double rho, const double S, const double p[2], double c[2]);
int (*HOFV_consToPri)(const double rho, const double S, const double c[2], double p[2]);
int (*HOFV_waveSpeeds)(const double ul[2], const double ur[2], double *s1, double *s2);
int (*HOFV_phyFlux)(const double u[2], double flux[2]);
int (*HOFV_numFlux)(const double ul[2], const double ur[2], double flux[2]);

