#ifndef TYPES_HOFV
#define TYPES_HOFV

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>

#define HOFV_UNSET -1

#define HOFV_SUCCESS 1

#define HOFV_FAILURE 2

#define HOFV_STR "=================================================="

enum HOFV_Format {
  HOFV_FMT_None,
  HOFV_FMT_DAT,
  HOFV_FMT_VTK
};

enum HOFV_Method {
  HOFV_MET_None,
  HOFV_MET_Base,
  HOFV_MET_CV,
  HOFV_MET_Proj
};

enum HOFV_NumFLux {
  HOFV_NF_None,
  HOFV_NF_Rus,
  HOFV_NF_HLL
};

enum HOFV_TimeScheme {
  HOFV_TS_None,
  HOFV_TS_EE
};

enum HOFV_TestCase {
  HOFV_TC_None,
  HOFV_TC_Riemann
};

enum HOFV_BoundaryCondition {
  HOFV_BC_None,
  HOFV_BC_Dirichlet,
  HOFV_BC_Out,
  HOFV_BC_Periodic
};

typedef struct {
  char *filename;
  char *outdir;
  int saveFTOnly;
  int saveFreq;
  int method;
  int numflux;
  int tcase;
  double xmin, xmax, dx;
  int nx;
  int escheme;
  double emin, emax, de, cfl;
  double de_over_dx;
  int left_bc, right_bc;
} HOFV_Info;

typedef struct {
  int dim;
  double *cc;
  double *u, *uex;
  double *rho;
  double S;
  HOFV_Info *info;
} HOFV_Problem;

#endif
