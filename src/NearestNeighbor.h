#pragma once
#include "Patch.h"
#include <vector>


class NearestNeighbor
{
  public:
    void train(vector<Patch*> *positive,vector<Patch*> *negative);
    void conf(Patch* p, int * maxP, float * conf1, float * conf2);

    void estimate_threshold(vector<Patch*> negative);
  private:
    void train_patch(Patch* p, int isPositive, float threshold);
    vector<Patch*> * pex;
    vector<Patch*> * nex;
};

