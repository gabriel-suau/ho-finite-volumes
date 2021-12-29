#include "hofv.h"


int main(int argc, char **argv) {
  HOFV_Problem *problem;
  char *filename;

  problem = NULL;
  filename = NULL;

  /** Check arguments */
  if (argc != 2) {
    fprintf(stderr, "Usage : %s filename \n", argv[0]);
    exit(EXIT_FAILURE);
  }

  filename = (char*) malloc((strlen(argv[1]) + 1) * sizeof(char));
  if (filename == NULL) {
    fprintf(stderr, "Empty filename \n");
    exit(EXIT_FAILURE);
  }
  strcpy(filename, argv[1]);

  /** Init the problem struct */
  if (HOFV_Init_Problem(&problem) == HOFV_FAILURE) {
    exit(EXIT_FAILURE);
  }

  /** Read the simulation parameters */
  if (HOFV_readParameters(problem, filename) == HOFV_FAILURE) {
    HOFV_Free_Problem(problem);
    exit(EXIT_FAILURE);
  }

  /** Set the appropriate function pointers */
  HOFV_setFunc(problem);

  /** Set the initial solution */
  HOFV_setInitialCondition(problem);

  return EXIT_SUCCESS;
}
