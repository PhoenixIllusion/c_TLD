#include "NearestNeighbor.h"

extern ProgramConfig * Config;

#define the_same_thresh 0.95

NearestNeighbor::NearestNeighbor()
{
  pex = new vector<Patch*>();
  nex = new vector<Patch*>();
}


void NearestNeighbor::train_patch(Patch* p, int isPositive, float threshold);
{
  float conf1,conf2;
  int isP,isN;
  int maxP_idx;
    conf(p,&maxP_idx,&conf1,&conf2,&isP,&isN);
    if(isPositive && conf1 <= threshold){
      if(maxP_idx<0)
        pex.push_back(p);
      else
        pex.add(pex->begin()+maxP_idx,p);
    }
    if(!isPositive && conf1 > 0.5){
        nex.push_back(p);
    }

}

void NearestNeighbor::train(vector<Patch*> *positive,vector<Patch*> *negative)
{
  train_patch(positive[0],1,Config.model.tr_nn);
  int total = positive->size()+negative->size();
  while(total>0)
  {
    Patch * p = NULL;
    int index = rand()%total;
    if(index>positive->size())
    {
      p = positive[index];
      positive->erase(positive->begin()+index);
      train_patch(p,1,Config.model.tr_nn);
    }
    else
    {
      index-=positive->size();
      p = negative[index];
      negative->erase(negative->begin()+index);
      train_patch(p,0,Config.model.tr_nn);
    }
    total = positive->size()+negative->size();
  }

}

void NearestNeighbor::conf(Patch* p, int * maxP_idx, float * conf1, float * conf2, int * isPositive, int * isNegative)
{
  int numNX = nex.size();
  int numPX = pex.size();

  *isPositive = 0;
  *isNegative = 0;

  *maxP_idx = -1;

  if(0 == numPX)
  {
    conf1 = 0.0;
    conf2 = 0.0;
    return;
  }
  if(0 == numNX)
  {
    conf1 = 1.0;
    conf2 = 1.0;
    return;
  }

  float valid = Config.model.valid;
  
  float cMaxP = 0;
  float maxP = 0;
  float maxN = 0;

  for(int i=0;i<numNX;i++)
  {
    float c = p->NCC(negativeEx[i]);
    if(c>maxN)
     maxN = c;
  }
  int conservative = (int)Math.ceil(valid*numPX);

  for(int i=0;i<numPX;i++)
  {
    float c = p->NCC(positiveEx[i]);
    if(c>maxP)
    {
      maxP = c;
      *maxP_idx = i;
    }
    if(i<=conservative && c>cMaxP)
      cMaxP = c;      
  }

  *isPositive = maxP>the_same_thresh;
  *isNegative = maxN>the_same_thresh;

    // measure Relative Similarity
  float dN = 1 - maxN;
  float dP = 1 - maxP;
  *conf1 = dN / (dN + dP);

  // measure Conservative Similarity
  dP = 1 - cMaxP;
  *conf2 = dN / (dN + dP);

}

void NearestNeighbor::estimate_threshold(vector<Patch*> negative)
{
  float max_conf = 0.0;

  float conf1,conf2;
  int isP,isN;
  int maxP_idx;

  for(int i=0;i<negative.length;i++)
  {
    conf(negative[i],&maxP_idx,&conf1,&conf2,&isP,&isN);
    if(conf1 > max_conf)
      max_conf = conf1;
  }

  Config.model.thr_nn = MAX(Config.model.thr_nn,max_conf);
  Config.model.thr_nn_valid = MAX(Config.model.thr_nn_valid,Config.model.thr_nn);
}

