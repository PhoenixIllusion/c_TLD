#pragma once

typedef struct Model_t //not a car
{
  int minwin;
} Model;
typedef struct P_Par_t
{
  int num_closest;
  int num_warps;
  int noise;
  float angle;
  float shift;
  float scale;
  float overlap;
} P_Par;

class ProgramConfig
{
  public:
	 Model model;
   P_Par p_par_init;
   P_Par p_par_update;
   ProgramConfig();
};

