#pragma once   /* allow this file to be included multiple times */

/*
 * integrate.h -- module header for the integrate module
 */
#include <inttypes.h>						/* needed for this module */

/*
 * integrate -- integrates a function f over the interval [a,b] to
 * within precision p, using trapezoidal approximation.
 *
 * Expects: b>a, p>0 and p<1.
 *
 * Returns 0 for success; non-zero otherwise 
 *
 * Produces the integral approximation by reference through a result
 * pointer (rp), and the cumulative number of strip calculations used
 * to produce the approximation through a strip pointer (sp).
 */
int32_t integrate(double (*f)(double x), 
									double a, double b, double p, 
									double *rp, uint32_t *sp);
