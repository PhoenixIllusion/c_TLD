#include <stdio.h>
#include <stdlib.h>
#include "BoundingBox.h"
#include "macro.h"

// local storage so that sorting can be done
// (non-thread-safe) without constant alloc/dealloc
// implements a 1.5x grow rate whenever size > current
// is requested
static WeightedBox * sort_struct = NULL;
static int sort_size = 0;

//LT - Less Than
int bb_weight_cmp_lt(const void * b, const void * a)
{
  float aw = ((WeightedBox*)a)->weight;
  float bw = ((WeightedBox*)b)->weight;
  return  (aw>bw)?-1:(aw<bw)?1:0;
}
//GT - Greater Than
int bb_weight_cmp_gt(const void * a, const void * b)
{
  float aw = ((WeightedBox*)a)->weight;
  float bw = ((WeightedBox*)b)->weight;

  return  (aw>bw)?-1:(aw<bw)?1:0;
}

//taken from bb_overlap.cpp
double bb_overlap(BoundingBox a, BoundingBox b)
{
  if(a.r<b.l)return 0.0;
  if(a.l>b.r)return 0.0;
  if(a.b<b.t)return 0.0;
  if(a.t>b.b)return 0.0;

	float colInt =  MIN(a.r, b.r) - MAX(a.l, b.l)+1;
	float rowInt =  MIN(a.b, b.b) - MAX(a.t, b.t)+1;

  double intersect = colInt*rowInt;
  return ((double)intersect)/(a.area+b.area-intersect);
}

double bb_init(BoundingBox * bb, float x0, float y0, float x1, float y1)
{
  bb->l = MIN(x0,x1);
  bb->r = MAX(x0,x1);
  bb->t = MIN(y0,y1);
  bb->b = MAX(y0,y1);

  bb->area = (bb->r-bb->l+1)*(bb->b-bb->t+1);
}

//taken from bb_hull.m
BoundingBox bb_hull(const BoundingBox * grid, int * index, int num_index)
{
  BoundingBox hull = grid[index[0]];
  
  for(int i=0;i<num_index;i++)
  {
    BoundingBox bb = grid[index[i]];
    hull.l = MIN(hull.l,bb.l);
    hull.r = MAX(hull.r,bb.r);
    hull.t = MIN(hull.t,bb.t);
    hull.b = MAX(hull.b,bb.b);
  }
  hull.area = (hull.r-hull.l+1)*(hull.b-hull.t+1);
  return hull;
}

//taken from printf
void printBoundingBox(BoundingBox bb)
{
  printf("Box: %.1f,%.1f - %.1f,%.1f\n",bb.l,bb.t,bb.r,bb.b);
}

// on-demand reallocs to larger size and sorts
// ID's ONLY based on weights taken from the float pointer
// (float pointer requires to be large enough to fit the 
// maximum provided index, or will Segfault
void bb_sort(int * id,const float * weight, int num, int ( * cmp ) ( const void *, const void * ))
{
 if(sort_size<num)
 {
    sort_size = num*1.5;
    sort_struct = (WeightedBox*)realloc(sort_struct,sizeof(WeightedBox)*sort_size);
 }
 for(int i=0;i<num;i++)
 {
   sort_struct[i].id = id[i];
   sort_struct[i].weight = weight[id[i]];
 }
 qsort (sort_struct, num, sizeof(WeightedBox), cmp);

 for(int i=0;i<num;i++)
 {
   id[i] = sort_struct[i].id;
 }
}
