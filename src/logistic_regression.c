#include "../hdr/includes.h"

double f_linear_function(int size, double *x_array, double *w_array)
{
  double f_x = w_array[0];

  for (int i = 0; i < size; i++)
  {
    f_x = f_x + x_array[i] * w_array[i + 1];
  }

  return f_x;
}

double p_logistic_function(double f_x)
{
  double e_x = exp(-f_x);
  double p_x = 1.0 / (1.0 + e_x);

  return p_x;
}

double j_cross_entropy_function(double p_x, int y)
{
  double l_wb = -y * log(p_x) - (1 - y) * log(1 - p_x);

  return l_wb;
}

double dj_gradient_function(double p_x, int y, double x)
{
  double dj_wb = (p_x - y) * x;

  return dj_wb;
}

double *current_observation_weight_training(int size, double *x_array, int y)
{
  if (size <= 0)
  {
    printf("Error: incorrect array size in model training\n");
    exit(1);
  }
  double w_start[size + 1];
  double *w_next = (double *)safe_malloc(sizeof(double) * (size + 1));

  // init all weights to 1
  for (int i = 0; i < size + 1; i++)
  {
    w_start[i] = 1.0;
  }

  int flag = 0;
  while (flag == 0)
  {
    // we need to find the next weight values for each x_j
    // b value is always default
    w_next[0] = 1;

    double p_x = p_logistic_function(f_linear_function(size, x_array, w_start)); // same for all x_j

    // for each x_j, calculate next w, with gardient descent
    for (int j = 0; j < size; j++)
    {
      w_next[j + 1] = w_start[j + 1] - LEARNING_RATE * dj_gradient_function(p_x, y, x_array[j]);
    }

    // check for end of loop
    // TO BE REVISED IF NEEDED
    for (int j = 0; j < size; j++)
    {
      // if w_t+1 - w_t < 10^(-6) for all x_j then stop
      if (w_next[j + 1] - w_start[j + 1] < pow(10, -6))
      {
        flag = 1;
        continue;
      }
      else
      {
        flag = 0;
        break;
      }
    }
  }
  return w_next;
}

double *model_fitting(int observation_size, int x_dimention, double **x_array, int *y_array)
{
  double *w_array;
  //double **w_array = (double**)safe_malloc(observation_size*(x_dimention*sizeof(double)));

  /* full version requires weight calculation for all observation (not used)
  for ( int i = 0; i < observation_size; i++ ) {
    w_array[i] = current_observation_weight_training(x_dimention, x_array[i], y_array[i]);
  }*/

  // do only once for simplified version
  w_array = current_observation_weight_training(x_dimention, x_array[0], y_array[0]);

  return w_array;
}

double *model_fittingV2(Vector *tf_idfVector, Vector *trainingPairsVector)
{
  double *w_array;

  w_array = current_observation_weight_training(vectorItemsCount(tf_idfVector), tf_idfVector, trainingPairsVector);
}
