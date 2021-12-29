#include "hofv.h"

int HOFV_Init_Problem(HOFV_Problem **problem) {
  (*problem) = (HOFV_Problem*)malloc(sizeof(HOFV_Problem));

  if ((*problem) == NULL) {
    fprintf(stderr, "Error: %s: mesh struct allocation failed\n", __func__);
    return HOFV_FAILURE;
  }

  (*problem)->dim = 1;
  (*problem)->S = 1.0;
  (*problem)->u = (*problem)->uex = (*problem)->rho = NULL;
  (*problem)->info = (HOFV_Info*)calloc(1, sizeof(HOFV_Info));

  return HOFV_SUCCESS;
}


int HOFV_Free_Problem(HOFV_Problem *problem) {
  HOFV_Info *info;

  if (problem == NULL) {
    return HOFV_SUCCESS;
  }

  if (problem->u) {
    free(problem->u);
    problem->u = NULL;
  }

  if (problem->uex) {
    free(problem->uex);
    problem->uex = NULL;
  }

  if (problem->rho) {
    free(problem->rho);
    problem->rho = NULL;
  }

  if (problem->info) {
    info = problem->info;
    if (info->filename) {
      free(info->filename);
      info->filename = NULL;
    }
    if (info->outdir) {
      free(info->outdir);
      info->outdir = NULL;
    }
    free(problem->info);
    problem->info = NULL;
  }

  free(problem);
  problem = NULL;

  return HOFV_SUCCESS;
}


int HOFV_allocProblem(HOFV_Problem *problem) {

  assert(problem && problem->info);

  problem->cc  = (double*)calloc(2 * problem->info->nx, sizeof(double));
  problem->u   = (double*)calloc(2 * problem->info->nx, sizeof(double));
  problem->uex = (double*)calloc(2 * problem->info->nx, sizeof(double));
  problem->rho = (double*)calloc(2 * problem->info->nx, sizeof(double));

  return HOFV_SUCCESS;
}
