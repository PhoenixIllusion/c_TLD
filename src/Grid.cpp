#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "ProgramConfig.h"
#include "Grid.h"
#include "macro.h"

//pre-calculated scale space
//ranges from 1.2 ** (-10->0->10)
float scale_multipliers[] = {
	0.161505582889846,0.193806699467815,0.232568039361378,0.279081647233653,
	0.334897976680384,0.401877572016461,0.482253086419753,0.578703703703704,
  0.694444444444445,0.833333333333333,1.000000000000000,1.200000000000000,
  1.440000000000000,1.728000000000000,2.073600000000000,2.488320000000000,
  2.985984000000000,3.583180800000000,4.299816960000000,5.159780352000000,
  6.191736422400000	};

int max_scale = 21;

extern ProgramConfig Config;

Grid::Grid(int image_width, int image_height, BoundingBox window)
{


  float w = window.r-window.l+1;
  float h = window.b-window.t+1;

  int minW = Config.model.minwin;

  //Calculate Maximum Number of scales given window size
  int start_scale = 0;
  float * sM = scale_multipliers;
  for(;start_scale<max_scale;start_scale++)
  {
    if(h*sM[start_scale]>=minW && w*sM[start_scale]>=minW)
      break;
  }
  if(start_scale==max_scale)
  {
    fprintf(stderr, "Provided too small a search window. Unable to generate scanning grid\n");
    return;
  }
  num_scales = max_scale-start_scale;

  //Create scales list, and total number of BoundingBoxes needed for final grid
  total_boxes = 0;
  scales = (BoundingBox*)calloc(num_scales,sizeof(BoundingBox));
  for(int i=0;i<num_scales;i++)
  {
    //scales will be kept for fern feature multiplication
    scales[i].r = (int)(sM[start_scale+i]*w+0.5);
    scales[i].b = (int)(sM[start_scale+i]*h+0.5);
    float scan_width = image_width-((int)scales[i].r)-3;
    float scan_height = image_height-((int)scales[i].b)-3;
    int num_wide = scan_width/scales[i].r*10  +1;
    int num_high = scan_height/scales[i].b*10 +1;

    //needed for allocating memory
    total_boxes += num_wide*num_high;
  }

  // allocate the needed memory
  grid = (BoundingBox*)calloc(total_boxes,sizeof(BoundingBox));
  overlap_val = (float *)calloc(total_boxes,sizeof(float));

  overlap_positive_idx = (int *)calloc(total_boxes,sizeof(int));
  overlap_negative_idx = (int*)calloc(total_boxes,sizeof(int));

  //begin building the boxes
  int index = 0;
  for(int i=0;i<num_scales;i++)
  {
    float shiftH = 0.1* scales[i].b;
    float shiftW = 0.1* MIN(scales[i].b,scales[i].r);
    
    //this can be used for... something. It's num_wide is
    //able to be used to jump up/down between boxes at least
    float scan_width = image_width-((int)scales[i].r)-3;
    float scan_height = image_height-((int)scales[i].b)-3;
    int num_wide = scan_width/scales[i].r*10  +1;
    int num_high = scan_height/scales[i].b*10 +1;

		//this is used quite a bit throughout the program
    float scale_area = (scales[i].r)*(scales[i].b);

    for(int y=0;y<num_high;y++)
    {
      float bb_t = 1+(int)(shiftH*y+0.5);
      float bb_b = (int)(shiftH*y+0.5)+scales[i].b;
      for(int x=0;x<num_wide;x++)
      {
          BoundingBox bb;
          bb.l = 1+(int)(shiftW*x+0.5);
          bb.r = (int)(shiftW*x+0.5)+scales[i].r;
          bb.t = bb_t;
          bb.b = bb_b;

					//Integral image pointer locations(hopefully not off-by-1)
          //because MatLab's 1-indexing is confusing... will test
          bb.ltp = ((int)bb.l)+image_width*((int)bb.t);
          bb.rtp = ((int)bb.r)+image_width*((int)bb.t);
          bb.lbp = ((int)bb.l)+image_width*((int)bb.b);
          bb.rbp = ((int)bb.r)+image_width*((int)bb.b);
          bb.area = scale_area;
          if(index==total_boxes)
          {
            fprintf(stderr, "Error: Box on scale %d reached index %d\n",i,index);
          }
          grid[index++] = bb;
      }
    }
  }
}

/*
Parameters
	search - Search window that will be checked against by all the grid

  positive threshold - will be used to mark an index for storing
  **positive - pointer to this Grid's positive index collection
  *num_positive - return value for how many positive were found
  
  negative threshold - will be used to mark an index for storing
  **negative - pointer to this Grid's negative index collection
  *num_negative - return value for how many negative were found

  return - pointer to entire grid's overlap values
					 *can be used with the index collections to get specific values

*/
const float * Grid::overlap(BoundingBox search, 
		double positive_threshold,int ** positive, int * num_positive,
		double negative_threshold, int ** negative, int * num_negative)
{
   //used to total num_positive/negative
   //used to keep track of current index collection position
   int p_index = 0;
   int n_index = 0;

   //loop through and collect overlaps
   for(int i=0;i<total_boxes;i++)
   {
      double o;
      overlap_val[i] =o= bb_overlap(search,grid[i]);
      if(o<negative_threshold)
      {
         overlap_negative_idx[n_index] = i;
         n_index++;
      }
      if(o>positive_threshold)
      {
         overlap_positive_idx[p_index] = i;
         p_index++;
      }
   }
   //set all pointers.. quite dangerous if ever free'd
   *positive = overlap_positive_idx;
   *negative = overlap_negative_idx;

   *num_positive = p_index;
   *num_negative = n_index;
   return overlap_val;
}

int Grid::size()
{
  return total_boxes;
}

int Grid::numScales()
{
  return numScales();
}

const BoundingBox * Grid::getGrid()
{
  return grid;
}

