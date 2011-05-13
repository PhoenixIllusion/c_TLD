#pragma once
#include <opencv/cv.h>
#include "BoundingBox.h"

class Patch
{
  public:
    Patch(IplImage * img, BoundingBox b, BoundingBox patchSize);
    float NCC(Patch *p);
    float *patch;	
    float variance;
    float norm;
    int length;
};
