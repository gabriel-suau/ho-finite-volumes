#include "hofv.h"


int HOFV_setupMesh(HOFV_Problem *problem) {
  int k;

  assert(problem && problem->cc);

  for (k = 0 ; k < problem->info->nx ; k++) {
    problem->cc[k] = problem->info->xmin + (k + 0.5) * problem->info->dx;
  }

  return HOFV_SUCCESS;
}


int HOFV_setupMethod(HOFV_Problem *problem) {

  assert (problem && problem->info);

  /* Set the model related function pointers */
  switch(problem->info->method) {

  case HOFV_MET_Base:
    HOFV_isAdmissible = HOFV_isAdmissible_base;
    HOFV_consToPri = HOFV_consToPri_base;
    HOFV_priToCons = HOFV_priToCons_base;
    HOFV_waveSpeeds = HOFV_waveSpeeds_base;
    HOFV_phyFlux = HOFV_phyFlux_base;
    break;

  case HOFV_MET_CV:
    HOFV_isAdmissible = HOFV_isAdmissible_CV;
    HOFV_consToPri = HOFV_consToPri_CV;
    HOFV_priToCons = HOFV_priToCons_CV;
    HOFV_waveSpeeds = HOFV_waveSpeeds_CV;
    HOFV_phyFlux = HOFV_phyFlux_CV;
    break;

  case HOFV_MET_Proj:
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

  switch(problem->info->numflux) {
  case HOFV_NF_Rus:
    HOFV_numFlux = HOFV_numFlux_rusanov;
    break;
  case HOFV_NF_HLL:
    HOFV_numFlux = HOFV_numFlux_HLL;
    break;
  default:
    HOFV_numFlux = HOFV_numFlux_HLL;
    break;
  }
  return HOFV_SUCCESS;
}


int HOFV_setupCase(HOFV_Problem *p) {
  HOFV_Info *info;
  int k, nx;
  double x, mid, third, *tmp_mesh, r1, r2, r3;
  double xmin, xmax, dx;
  double xmint, xmaxt, dxt;
  double x1, x2, x3;

  assert (p && p->info);
  assert (p->u && p->uex && p->cc && p->rho);

  info = p->info;

  xmin = info->xmin;
  xmax = info->xmax;
  dx = info->dx;
  nx = info->nx;
  r1 = 1.0;
  r2 = 0.9;
  r3 = 1.0;

  tmp_mesh = (double*) calloc(nx, sizeof(double));

  switch (info->tcase) {

  case HOFV_TC_Riemann:
    mid = 0.5 * (xmin + xmax);
    third = 0.3 * (xmin + xmax);
    /* Rho */
    for (k = 0 ; k < nx ; k++) {
      x = p->cc[k];
      if (x < third)
        p->rho[k] = r1;
      else if (x < mid)
        p->rho[k] = r2;
      else
        p->rho[k] = r3;
    }
    p->S = 1.0;
    break;

  default:
    return HOFV_FAILURE;
    break;
  }

  /* Modify the mesh depending on the case and method */
  switch(p->info->method) {

  case HOFV_MET_Base:
    break;

  case HOFV_MET_CV:
    xmint = xmaxt = 0.0;

    for (k = 0 ; k < nx ; k++) {
      xmaxt += dx * p->rho[k];
    }

    dxt = (xmaxt - xmint) / nx;

    for (k = 0 ; k < nx ; k++) {
      tmp_mesh[k] = xmint + (k + 0.5) * dxt;
    }

    for (k = 0 ; k < nx ; k++) {
      x1 = tmp_mesh[k] / r1;
      if (x1 < third) {
        p->cc[k] = x1;
        continue;
      }
      x2 = (tmp_mesh[k] + third * (r2 - r1)) / r2;
      if (third < x2 && x2 < mid) {
        p->cc[k] = x2;
        continue;
      }
      x3 = (tmp_mesh[k] + third * (r2 - r1) + mid * (r3 - r2)) / r3;
      if (mid < x3 && x3 < xmax) {
        p->cc[k] = x3;
        continue;
      }
    }

    for (k = 0 ; k < nx ; k++) {
      x = p->cc[k];
      if (x < third)
        p->rho[k] = r1;
      else if (x < mid)
        p->rho[k] = r2;
      else
        p->rho[k] = r3;
    }

    info->dx = dxt;
    info->de_over_dx = info->de / dxt;

    break;

  default:
    break;
  }

  /* Initial condition */
  for (k = 0 ; k < nx ; k++) {
    x = p->cc[k];
    p->u[2 * k] = (x < mid) ? 0.5 : 3.0;
    p->u[2 * k + 1] = 0.0;
    HOFV_priToCons(p->rho[k], p->S, &p->u[2 * k], &p->u[2 * k]);
    memcpy(&p->uex[2 * k], &p->u[2 * k], 2 * sizeof(double));
  }

  return HOFV_SUCCESS;
}
