#ifndef HOFV_H
#define HOFV_H

#include "types_hofv.h"
#include "externs_hofv.h"

#define HOFV_MAX(a,b) (((a) > (b)) ? (a) : (b))
#define HOFV_MIN(a,b) (((a) < (b)) ? (a) : (b))
#define HOFV_ABS(a) (((a) > 0.0) ? (a) : (-a))

/* memory_hofv.c */
int HOFV_Init_Problem(HOFV_Problem **problem);
int HOFV_Free_Problem(HOFV_Problem *problem);
int HOFV_allocProblem(HOFV_Problem *problem);

/* info_hofv.c */
int HOFV_readParameters(HOFV_Problem *problem, char *filename);
int HOFV_chkParameters(HOFV_Problem *problem);
int HOFV_printParameters(HOFV_Problem *problem);

/* inout_hofv.c */
/* int HOFV_saveSol(HOFV_Problem *problem, char *filename); */

/* mesh_hofv.c */
int HOFV_setupMesh(HOFV_Problem *problem);

/* model_hofv.c */
int HOFV_setupModel(HOFV_Problem *problem);

double HOFV_chi(double f);

int HOFV_isAdmissible_base(const double rho, const double S, const double u[2]);
int HOFV_priToCons_base(const double rho, const double S, const double p[2], double c[2]);
int HOFV_consToPri_base(const double rho, const double S, const double c[2], double p[2]);
int HOFV_waveSpeeds_base(const double ul[2], const double ur[2], double *s1, double *s2);
int HOFV_phyFlux_base(const double u[2], double flux[2]);

/* int HOFV_isAdmissible_CV(const double rho, const double S, const double u[2]); */
/* int HOFV_priToCons_CV(const double rho, const double S, const double p[2], double c[2]); */
/* int HOFV_consToPri_CV(const double rho, const double S, const double c[2], double p[2]); */
/* int HOFV_waveSpeeds_CV(const double ul[2], const double ur[2], double *s1, double *s2); */
/* int HOFV_phyFlux_CV(const double u[2], double flux[2]); */

/* int HOFV_isAdmissible_proj(const double rho, const double S, const double u[2]); */
/* int HOFV_priToCons_proj(const double rho, const double S, const double p[2], double c[2]); */
/* int HOFV_consToPri_proj(const double rho, const double S, const double c[2], double p[2]); */
/* int HOFV_waveSpeeds_proj(const double ul[2], const double ur[2], double *s1, double *s2); */
/* int HOFV_phyFlux_proj(const double u[2], double flux[2]); */

/* numflux_hofv.c */
/* int HOFV_numFlux_rusanov(const double ul[2], const double ur[2], double flux[2]); */
/* int HOFV_numFlux_HLL(const double ul[2], const double ur[2], double flux[2]); */


/* testcase_hofv.c */
int HOFV_setupCase(HOFV_Problem *problem);


/* energy_hofv.c */
/* int HOFV_estep(); */


/* solve_hofv.c */
/* int HOFV_solve(HOFV_Problem *problem); */

/* tools_hofv.c */
const char* HOFV_MOD_to_string(int code);
const char* HOFV_TS_to_string(int code);
const char* HOFV_NF_to_string(int code);
const char* HOFV_BC_to_string(int code);
const char* HOFV_TC_to_string(int code);

#endif
