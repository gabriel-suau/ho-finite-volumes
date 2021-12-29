#include "hofv.h"


int HOFV_setupModel(HOFV_Problem *problem) {

  assert (problem && problem->info);

  /* Set the model related function pointers */
  switch(problem->info->model) {

  case HOFV_MOD_Base:
    HOFV_isAdmissible = HOFV_isAdmissible_base;
    HOFV_consToPri = HOFV_consToPri_base;
    HOFV_priToCons = HOFV_priToCons_base;
    HOFV_waveSpeeds = HOFV_waveSpeeds_base;
    HOFV_phyFlux = HOFV_phyFlux_base;
    break;

  case HOFV_MOD_CV:
    /* HOFV_isAdmissible = HOFV_isAdmissible_CV; */
    /* HOFV_consToPri = HOFV_consToPri_CV; */
    /* HOFV_priToCons = HOFV_priToCons_CV; */
    /* HOFV_waveSpeeds = HOFV_waveSpeeds_CV; */
    /* HOFV_phyFlux = HOFV_phyFlux_CV; */
    break;

  case HOFV_MOD_Proj:
    /* HOFV_isAdmissible = HOFV_isAdmissible_proj; */
    /* HOFV_consToPri = HOFV_consToPri_proj; */
    /* HOFV_priToCons = HOFV_priToCons_proj; */
    /* HOFV_waveSpeeds = HOFV_waveSpeeds_proj; */
    /* HOFV_phyFlux = HOFV_phyFlux_proj; */
    break;

  default:
    HOFV_isAdmissible = HOFV_isAdmissible_base;
    HOFV_consToPri = HOFV_consToPri_base;
    HOFV_priToCons = HOFV_priToCons_base;
    HOFV_waveSpeeds = HOFV_waveSpeeds_base;
    HOFV_phyFlux = HOFV_phyFlux_base;
    break;
  }

  return HOFV_SUCCESS;
}

/* Common */
double HOFV_chi(double f) {
  return (3.0 + 4.0 * f * f) / (5.0 + 2.0 * sqrt(4.0 - 3.0 * f * f));
}


/* Base model */
int HOFV_isAdmissible_base(const double rho, const double S, const double u[2]) {
  if ((u[0] / (rho * S) > 0.0) && (HOFV_ABS(u[1]) <= u[0]))
    return 1;
  else
    return 0;
}

int HOFV_priToCons_base(const double rho, const double S, const double p[2], double c[2]) {
  double rho_S = rho * S;

  c[0] = p[0] * rho_S;
  c[1] = p[1] * rho_S;
  
  return HOFV_SUCCESS;
}

int HOFV_consToPri_base(const double rho, const double S, const double c[2], double p[2]) {
  double inv_rho_S = 1.0 / rho * S;

  p[0] = c[0] * inv_rho_S;
  p[1] = c[1] * inv_rho_S;

  return HOFV_SUCCESS;  
}

int HOFV_waveSpeeds_base(const double ul[2], const double ur[2], double *s1, double *s2) {
  return HOFV_SUCCESS;
}

int HOFV_phyFlux_base(const double u[2], double flux[2]) {

  flux[0] = u[1];
  flux[1] = u[0] * HOFV_chi(u[1] / u[0]);

  return HOFV_SUCCESS;
}
