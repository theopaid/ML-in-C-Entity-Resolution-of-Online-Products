#ifndef _LOG_REG_H_
#define _LOG_REG_H_

#define LEARNING_RATE 1.2

double f_linear_function(int, double *, double *);
double p_logistic_function(double);
double j_cross_entropy_function(double, int);
double dj_gradient_function(double, int, double);
double *current_observation_weight_training(int, double*, int);
double *model_fitting(int, int, double**, int*);

#endif
