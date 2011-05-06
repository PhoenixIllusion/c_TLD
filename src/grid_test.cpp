#include <stdio.h>
#include "Grid.h"
#include "ProgramConfig.h"

ProgramConfig Config;

int main(int argc, char** argv)
{
  // based on settings from GIT hub
  BoundingBox test;
  Config.model.minwin = 24;
  Config.p_par_init.num_closest = 10;
  bb_init(&test,287,35,312,77);

  // same as bb_scan
  Grid * grid = new Grid(470,310, test);

  //number of overlapping Pos and Neg entries
  int p_overlap;
  int n_overlap;
  //pointer to index (points to Grid obj memory)
  int * p_idx;
  int * n_idx;

  //overlap % of the grid
  //points to Grid obj memory
  //thresholds taken from GIT hub
  const float * overlaps = grid->overlap(test, 0.6,&p_idx,&p_overlap,0.2,&n_idx,&n_overlap);

  //sort the positive data by overlap amount
  bb_sort(p_idx,overlaps,p_overlap,bb_weight_cmp_gt);
  
  //build a Hull with the first 10 items from the overlap data
  BoundingBox hull = bb_hull(grid->getGrid(),p_idx,10);

  //take the highest % item as BBP0
  //doesn't implement (lowest ID, highest % yet)
  BoundingBox BBP0 = grid->getGrid()[p_idx[0]];

  //data to compare with against MatLab TLD
  // is 0-indexed instead of 1-indexed, so all values are -1 different
  printf("Re-Alignment Box:\n\t");
  printBoundingBox(BBP0);
  printf("Initial Hull:\n\t");
  printBoundingBox(hull);
  return 0;
}


