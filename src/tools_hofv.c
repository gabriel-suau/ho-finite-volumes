#include "hofv.h"

const char* HOFV_MOD_to_string(int code) {
  switch(code) {
  case HOFV_MOD_Base:
    return "Base";
    break;
  case HOFV_MOD_CV:
    return "CV";
    break;
  default:
    return "None";
    break;
  }
}

const char* HOFV_TS_to_string(int code) {
  switch(code) {
  case HOFV_TS_EE:
    return "ExplicitEuler";
    break;
  default:
    return "None";
    break;
  }
}

const char* HOFV_NF_to_string(int code) {
  switch(code) {
  case HOFV_NF_Rusanov:
    return "Rusanov";
    break;
  case HOFV_NF_HLL:
    return "HLL";
    break;
  default:
    return "None";
    break;
  }
}

const char* HOFV_TC_to_string(int code) {
  switch(code) {
  case HOFV_TC_Riemann:
    return "Riemann";
    break;
  default:
    return "None";
    break;
  }
}

const char* HOFV_BC_to_string(int code) {
  switch(code) {
  case HOFV_BC_Dirichlet:
    return "Dirichlet";
    break;
  case HOFV_BC_Out:
    return "Out";
    break;
  case HOFV_BC_Periodic:
    return "Periodic";
    break;
  default:
    return "None";
    break;
  }
}
