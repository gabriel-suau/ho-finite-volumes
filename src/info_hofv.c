#include "hofv.h"

int HOFV_readParameters(HOFV_Problem *problem, char *filename) {
  HOFV_Info *info;
  FILE *file = NULL;
  char chain[127];

  if (problem == NULL) {
    fprintf(stderr, "Error: %s: Problem struct not allocated\n", __func__);
    return HOFV_FAILURE;
  }

  info = problem->info;
  if (info == NULL) {
    fprintf(stderr, "Error: %s: Info struct not allocated\n", __func__);
    return HOFV_FAILURE;
  }

  file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Error: %s: can't open %s \n", __func__, filename);
    return HOFV_FAILURE;
  }

  printf("## File %s opened\n", filename);

  info->filename = filename;

  while (fscanf(file, "%s", chain) != EOF) {

    if (chain[0] == '#') continue;

    if (!strncmp(chain, "OutputDir", strlen("OutputDir"))) {
      fscanf(file, "%s", info->outdir);
      continue;
    }

    else if (!strncmp(chain, "SaveFinalTimeOnly", strlen("SaveFinalTimeOnly"))) {
      fscanf(file, "%d", &info->saveFTOnly);
      continue;
    }

    else if (!strncmp(chain, "SaveFrequency", strlen("SaveFrequency"))) {
      fscanf(file, "%d", &info->saveFreq);
      continue;
    }

    else if (!strncmp(chain, "Model", strlen("Model"))) {
      fscanf(file, "%d", &info->model);
      continue;
    }

    else if (!strncmp(chain, "TestCase", strlen("TestCase"))) {
      fscanf(file, "%d", &info->tcase);
      continue;
    }

    else if (!strncmp(chain, "leftBC", strlen("leftBC"))) {
      fscanf(file, "%d", &info->left_bc);
      continue;
    }

    else if (!strncmp(chain, "rightBC", strlen("rightBC"))) {
      fscanf(file, "%d", &info->right_bc);
      continue;
    }

    else if (!strncmp(chain, "xmin", strlen("xmin"))) {
      fscanf(file, "%lf", &info->xmin);
      continue;
    }

    else if (!strncmp(chain, "xmax", strlen("xmax"))) {
      fscanf(file, "%lf", &info->xmax);
      continue;
    }

    else if (!strncmp(chain, "NCells", strlen("NCells"))) {
      fscanf(file, "%d", &info->nx);
      continue;
    }

    else if (!strncmp(chain, "NumericalFlux", strlen("NumericalFlux"))) {
      fscanf(file, "%d", &info->numflux);
      continue;
    }

    else if (!strncmp(chain, "EnergyScheme", strlen("EnergyScheme"))) {
      fscanf(file, "%d", &info->escheme);
      continue;
    }

    else if (!strncmp(chain, "emin", strlen("emin"))) {
      fscanf(file, "%lf", &info->emin);
      continue;
    }

    else if (!strncmp(chain, "emax", strlen("emax"))) {
      fscanf(file, "%lf", &info->emax);
      continue;
    }

    else if (!strncmp(chain, "EnergyStep", strlen("EnergyStep"))) {
      fscanf(file, "%lf", &info->de);
      continue;
    }

    else if (!strncmp(chain, "CFL", strlen("CFL"))) {
      fscanf(file, "%lf", &info->cfl);
      continue;
    }

  }

  fclose(file);

  /* Check the parameters */
  if (HOFV_chkParameters(problem) != HOFV_SUCCESS) {
    return HOFV_FAILURE;
  }

  /* Allocate memory */
  if (HOFV_allocProblem(problem) != HOFV_SUCCESS) {
    return HOFV_FAILURE;
  }

  /* Print the parameters */
  HOFV_printParameters(problem);

  printf("## File %s closed\n", filename);

  return HOFV_SUCCESS;
}

int HOFV_chkParameters(HOFV_Problem *problem) {
  HOFV_Info *info;
  double tmp;

  info = problem->info;

  if (!info->saveFreq) info->saveFTOnly = 1;

  if (info->model != HOFV_MOD_Base &&
      info->model != HOFV_MOD_CV &&
      info->model != HOFV_MOD_Proj) {
    fprintf(stderr, "## Error : %s:%d : Model not implemented.\n", __func__, __LINE__);
    return HOFV_FAILURE;
  }

  if (info->tcase != HOFV_TC_Riemann) {
    fprintf(stderr, "## Error : %s:%d : Test Case not implemented.\n", __func__, __LINE__);
    return HOFV_FAILURE;
  }

  if (info->left_bc != HOFV_BC_Dirichlet ||
      info->right_bc != HOFV_BC_Dirichlet) {
    fprintf(stderr, "## Error : %s:%d : Boundary conditions not implemented.\n", __func__, __LINE__);
    return HOFV_FAILURE;
  }

  if (info->xmin == info->xmax) {
    fprintf(stderr, "## Error : %s:%d : xmax cannot be equal to xmin.\n", __func__, __LINE__);
    return HOFV_FAILURE;
  }

  if (info->xmin > info->xmax) {
    fprintf(stderr, "## Warning : %s:%d : swapping xmin and xmax.\n", __func__, __LINE__);
    tmp = info->xmin;
    info->xmin = info->xmax;
    info->xmax = tmp;
  }

  info->dx = (info->xmax - info->xmin) / info->nx;

  if (info->nx <= 0) {
    fprintf(stderr, "## Error : %s:%d : The number of cells must be a positive integer.\n", __func__, __LINE__);
    return HOFV_FAILURE;
  }

  if (info->numflux != HOFV_NF_Rusanov &&
      info->numflux != HOFV_NF_HLL) {
    fprintf(stderr, "## Error : %s:%d : Numerical flux not implemented.\n", __func__, __LINE__);
    return HOFV_FAILURE;
  }

  if (info->escheme != HOFV_TS_EE) {
    fprintf(stderr, "## Error : %s:%d : Energy scheme not implemented.\n", __func__, __LINE__);
    return HOFV_FAILURE;
  }

  if (info->emin <= 0 || info->emax <= 0) {
    fprintf(stderr, "## Error : %s:%d : emin and emax must be positive values.\n", __func__, __LINE__);
    return HOFV_FAILURE;
  }

  if (info->emin > info->emax) {
    fprintf(stderr, "## Warning : %s:%d : swapping emin and emax.\n", __func__, __LINE__);
    tmp = info->emin;
    info->emin = info->emax;
    info->emax = tmp;    
  }

  if (info->de <= 0.0) {
    fprintf(stderr, "## Error : %s:%d : The energy step must be a real positive number.\n", __func__, __LINE__);
    return HOFV_FAILURE;    
  }

  return HOFV_SUCCESS;
}


int HOFV_printParameters(HOFV_Problem *problem) {
  HOFV_Info *info;

  assert(problem && problem->info);

  info = problem->info;

  fprintf(stdout, "%s\n", HOFV_STR);
  fprintf(stdout, "Printing parameters of: %s\n", info->filename);
  fprintf(stdout, "Model      : %s\n", HOFV_MOD_to_string(info->model));
  fprintf(stdout, "Test case  : %s\n", HOFV_TC_to_string(info->tcase));
  fprintf(stdout, "Mesh : \n");
  fprintf(stdout, "  |xmin    = %f \n", info->xmin);
  fprintf(stdout, "  |xmax    = %f \n", info->xmax);
  fprintf(stdout, "  |nx      = %d \n", info->nx);
  fprintf(stdout, "  |dx      = %f \n", info->dx);
  fprintf(stdout, "NumFlux    : %s\n", HOFV_NF_to_string(info->numflux));
  fprintf(stdout, "EScheme    : %s\n", HOFV_TS_to_string(info->escheme));
  fprintf(stdout, "  |emin    = %f \n", info->emin);
  fprintf(stdout, "  |emax    = %f \n", info->emax);
  fprintf(stdout, "  |de      = %f \n", info->de);
  fprintf(stdout, "  |CFL     = %f \n", info->cfl);
  fprintf(stdout, "Left BC    : %s\n", HOFV_BC_to_string(info->left_bc));
  fprintf(stdout, "Right BC   : %s\n", HOFV_BC_to_string(info->right_bc));
  fprintf(stdout, "%s\n", HOFV_STR);

  return HOFV_SUCCESS;
}
