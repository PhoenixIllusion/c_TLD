#pragma once

typedef struct BoundingBox_t
{

// Data for positioning
float l;
float r;
float t;
float b;

//Data for Integral Images
int ltp; //left-top-pixel;
int rtp; //right-top-pixel;
int lbp; //left-bottom-pixel;
int rbp; //right-bottom-pixel;

//current scale, for Fern Feature
int scale; 

//For overlap and Integral Images
float area;

} BoundingBox;

//allows for re-ordering index in an array based on weights
typedef struct WeightedBox_t
{
  int id;
  float weight;
} WeightedBox;

//calculates size of overlap vs. size of shared combined area
double bb_overlap(BoundingBox a, BoundingBox b);
//fills a bounding box and re-calcs area
double bb_init(BoundingBox * bb, float x0, float y0, float x1, float y1);

//finds the minimum bounding box that contains all marked (index'd) box
BoundingBox bb_hull(const BoundingBox * bb, int * idx, int idx_count);

//utility and debug... printf's the x0,y0 x1,y1
void printBoundingBox(BoundingBox bb);

//sort num elements in the id index collection based onc comparitor
void bb_sort(int * id, const float * weight, int num, int ( * comparator ) ( const void *, const void * ));
//LT - Less Than
int bb_weight_cmp_lt(const void * a, const void * b);
//GT - Greater Than
int bb_weight_cmp_gt(const void * a, const void * b);
