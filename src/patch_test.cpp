#include "Patch.h"
#include "ProgramConfig.h"
#include "YUV.h"
#include <stdio.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

ProgramConfig Config;

int main(int argc,char**argv)
{
  if(argc<0)
  {
    fprintf(stderr,"Calculating: image: _input/00001.png box: (287,35)-(313,78) patch=(15,15)\n");
  }

  BoundingBox target;
  target.l = 287; target.r = 312;
  target.t = 35; target.b = 77;

  BoundingBox patchSize;
  patchSize.r = 15;
  patchSize.b = 15;

  IplImage * img = readYUVImage("_input/00001.png");

  Patch * p = new Patch(img,target,patchSize);

  printf("Variance: %f\n",p->variance);
//  for(int i=0;i<p->length;i++)
//    printf("Values: Index[%d] - %f\n",i,p->patch[i]);
  
  return 0;
}
