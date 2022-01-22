#include "hofv.h"

int HOFV_saveSol(HOFV_Problem *problem, char *filename) {
  FILE *file = NULL;
  int k;
  double p[2];

  assert (problem && problem->u);

  file = fopen(filename, "w");
  if (file == NULL) {
    fprintf(stderr, "## Error : cannot open file:  %s\n", filename);
    return HOFV_FAILURE;
  }

  fprintf(file, "# x    phi_0    phi_1   rho   S\n");

  for (k = 0 ; k < problem->info->nx ; k++) {
    HOFV_consToPri(problem->rho[k], problem->S, &problem->u[2*k], p);
    fprintf(file, "%lf %lf %lf %lf %lf\n", problem->cc[k], p[0], p[1], problem->rho[k], problem->S);
  }

  fclose(file);

  return HOFV_SUCCESS;
}
