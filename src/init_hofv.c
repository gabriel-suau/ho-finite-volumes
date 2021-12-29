#include "hofv.h"

int HOFV_setupCase(HOFV_Problem *problem) {
  HOFV_Info *info;
  int k;
  double x, mid;

  assert (problem && problem->info);
  assert (problem->u && problem->uex);

  info = problem->info;

  switch (info->tcase) {

  case HOFV_TC_Riemann:
    mid = 0.5 * (info->xmin + info->xmax);
    /* Rho */
    for (k = 0 ; k < info->nx ; k++) {
      x = problem->cc[k];
      if (x < 0.3 * (info->xmin + info->xmax))
        problem->rho[k] = 1.0;
      else if (x < mid)
        problem->rho[k] = 0.01;
      else
        problem->rho[k] = 1.0;
    }
    /* Initial condition */
    for (k = 0 ; k < info->nx ; k++) {
      x = problem->cc[k];
      problem->u[k] = (x < mid) ? 0.5 : 3.0;
      problem->uex[k] = problem->u[k];
    }
    problem->S = 1.0;
    break;

  default:
    return HOFV_FAILURE;
    break;
  }

  return HOFV_SUCCESS;
}
