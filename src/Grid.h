#pragma once

#include "BoundingBox.h"

class Grid
{
  public:
   //allocate a multi-scale scannging grid using
   //dimensions of window, and image size of image_*
   Grid(int image_width, int image_height, BoundingBox window);

	 //finds the boxes that are > or < provided threshold and stores
   //indexes. stores overlap % for every box
   const float * overlap(BoundingBox search, double positive_threshold,int ** positive, int * num_positive, double negative_threshold, int ** negative, int * num_negative);

   //accessors...
   int size();
   int numScales();
   const BoundingBox * getGrid();

  private:
  //every box + pixel offsets + area
  BoundingBox * grid ;
  //parallel array of overlaps from last overlap function
  float * overlap_val ;
  //how large is each box, given a scale
  BoundingBox * scales ;
  //how many boxes/overlaps are in this object
  int total_boxes ;

  // max storage = count of boxes
  int * overlap_positive_idx ;
  int * overlap_negative_idx ;

  //how many scales are actually used in this scale-space scanning grid
  int num_scales ;

};


