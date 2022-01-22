#include "hofv.h"

/* Common */
double HOFV_chi(double f) {
  return (3.0 + 4.0 * f * f) / (5.0 + 2.0 * sqrt(4.0 - 3.0 * f * f));
}

double HOFV_chi_prime(double f) {
  return (2.0 * f / sqrt(4.0 - 3.0 * f));
}

double HOFV_det_base(double f) {
  return 4 * (f * f / (4 - 3 * f) - 2 * f * f / sqrt(4 - 3 * f) + (4 * f * f + 3) / (5 + 2 * sqrt(4 - 3 * f * f)));
}

double HOFV_det_CV(double f) {
  return 4 * (f * f / (4 - 3 * f) - 2 * f * f / sqrt(4 - 3 * f) + (4 * f * f + 3) / (5 + 2 * sqrt(4 - 3 * f * f)));
}

/* Base model */
int HOFV_isAdmissible_base(const double rho, const double S, const double u[2]) {
  if ((u[0] / (rho * S) > 0.0) && (HOFV_ABS(u[1]) <= u[0]))
    return 1;
  else
    return 0;
}

int HOFV_priToCons_base(const double rho, const double S, double p[2], double c[2]) {
  double rho_S = rho * S;

  c[0] = p[0] * rho_S;
  c[1] = p[1] * rho_S;

  return HOFV_SUCCESS;
}

int HOFV_consToPri_base(const double rho, const double S, double c[2], double p[2]) {
  double inv_rho_S = 1.0 / rho * S;

  p[0] = c[0] * inv_rho_S;
  p[1] = c[1] * inv_rho_S;

  return HOFV_SUCCESS;  
}

int HOFV_waveSpeeds_base(const double ul[2], const double ur[2], double *s1, double *s2) {
  double sqrt_det;
  double l1, l2, l3, l4, f;

  f = ul[1] / ul[0];
  sqrt_det = sqrt(HOFV_det_base(f));
  l1 = 0.5 * (HOFV_chi_prime(f) - sqrt_det);
  l2 = 0.5 * (HOFV_chi_prime(f) + sqrt_det);

  f = ur[1] / ur[0];
  sqrt_det = sqrt(HOFV_det_base(f));
  l3 = 0.5 * (HOFV_chi_prime(f) - sqrt_det);
  l4 = 0.5 * (HOFV_chi_prime(f) + sqrt_det);

  *s1 = HOFV_MAX(l2, l4);
  *s2 = HOFV_MIN(l1, l3);

  return HOFV_SUCCESS;
}

int HOFV_phyFlux_base(const double u[2], double flux[2]) {

  flux[0] = u[1];
  flux[1] = u[0] * HOFV_chi(u[1] / u[0]);

  return HOFV_SUCCESS;
}

/* CV model */
int HOFV_isAdmissible_CV(const double rho, const double S, const double u[2]) {
  if ((u[0] / (rho * S) > 0.0) && (HOFV_ABS(u[1]) <= u[0]))
    return 1;
  else
    return 0;
}

int HOFV_priToCons_CV(const double rho, const double S, double p[2], double c[2]) {
  c[0] = p[0];
  c[1] = p[1];
  return HOFV_SUCCESS;
}

int HOFV_consToPri_CV(const double rho, const double S, double c[2], double p[2]) {
  p[0] = c[0];
  p[1] = c[1];
  return HOFV_SUCCESS;  
}

int HOFV_waveSpeeds_CV(const double ul[2], const double ur[2], double *s1, double *s2) {
  return HOFV_SUCCESS;
}

int HOFV_phyFlux_CV(const double u[2], double flux[2]) {

  flux[0] = u[1];
  flux[1] = u[0] * HOFV_chi(u[1] / u[0]);

  return HOFV_SUCCESS;
}


int HOFV_numFlux_rusanov(const double ul[2], const double ur[2], double flux[2]) {
  double fl[2], fr[2];
  double s1, s2, b;

  HOFV_phyFlux(ul, fl);
  HOFV_phyFlux(ur, fr);

  HOFV_waveSpeeds(ul, ur, &s1, &s2);

  b = HOFV_MAX(HOFV_ABS(s1), HOFV_ABS(s2));
  /* b = 1.0; */

  flux[0] = 0.5 * (fl[0] + fr[0] - b * (ur[0] - ul[0]));
  flux[1] = 0.5 * (fl[1] + fr[1] - b * (ur[1] - ul[1]));

  return HOFV_SUCCESS;
}

int HOFV_numFlux_HLL(const double ul[2], const double ur[2], double flux[2]) {
  double fl[2], fr[2];
  double s1, s2;

  HOFV_phyFlux(ul, fl);
  HOFV_phyFlux(ur, fr);

  HOFV_waveSpeeds(ul, ur, &s1, &s2);

  if (0 <= s2) {
    flux[0] = fl[0];
    flux[1] = fl[1];
  }
  else if (s2 < 0 && 0 < s1) {
    flux[0] = (s1 * fl[0] - s2 * fr[0] + s1 * s2 * (ur[0] - ul[0])) / (s1 - s2);
    flux[1] = (s1 * fl[1] - s2 * fr[1] + s1 * s2 * (ur[1] - ul[1])) / (s1 - s2);
  }
  else if (s1 <= 0) {  
    flux[0] = fr[0];
    flux[1] = fr[1];
  }

  return HOFV_SUCCESS;
}
