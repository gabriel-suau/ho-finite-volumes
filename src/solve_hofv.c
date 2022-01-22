#include "hofv.h"

int HOFV_estep(HOFV_Problem *problem) {
  HOFV_Info *info;
  int k;
  double *flux, *ul, *ur;

  info = problem->info;

  flux = (double*)calloc(2 * (info->nx+1), sizeof(double));

  /* Compute the numerical fluxes */
  HOFV_numFlux(&problem->u[0], &problem->u[0], &flux[0]);
  for (k = 1 ; k < problem->info->nx ; k++) {
    HOFV_numFlux(&problem->u[2 * (k - 1)], &problem->u[2 * k], &flux[2 * k]);
  }
  HOFV_numFlux(&problem->u[2 * (info->nx - 1)], &problem->u[2 * (info->nx - 1)], &flux[2 * info->nx]);

  /* Update the solution */
  ur = &problem->u[0];
  ur[0] += info->de_over_dx * flux[0];
  ur[1] += info->de_over_dx * flux[1];
  for (k = 1 ; k < info->nx ; k++) {
    ul = &problem->u[2 * (k - 1)];
    ur = &problem->u[2 * k];
    ul[0] -= info->de_over_dx * flux[2 * k];
    ul[1] -= info->de_over_dx * flux[2 * k + 1];
    ur[0] += info->de_over_dx * flux[2 * k];
    ur[1] += info->de_over_dx * flux[2 * k + 1];
  }
  ul = &problem->u[2 * (info->nx - 1)];
  ul[0] -= info->de_over_dx * flux[2 * info->nx];
  ul[1] -= info->de_over_dx * flux[2 * info->nx + 1];

  free(flux); flux = NULL;

  return HOFV_SUCCESS;
}


int HOFV_solve(HOFV_Problem *problem) {
  HOFV_Info *info;
  char *filename;
  double e;
  int n;

  info = problem->info;
  e = info->emax;
  n = 0;

  /* Save the initial solution */
  HOFV_saveSol(problem, "init.dat");

  while (e > info->emin) {
    HOFV_estep(problem);
    e -= info->de;
  }

  if (info->saveFTOnly) {
    HOFV_saveSol(problem, "caca.dat");
  }

  return HOFV_SUCCESS;
}
