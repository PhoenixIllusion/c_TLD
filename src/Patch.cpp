#include "Patch.h"
#include <opencv/highgui.h>

IplImage * subImage(IplImage * img, BoundingBox b, BoundingBox patchSize)
{
  cvSetImageROI(img, cvRect(b.l, b.t, b.r-b.l+1, b.b-b.t+1));
  IplImage *tmp = cvCreateImage(cvGetSize(img),
                               img->depth,
                               img->nChannels);
  cvCopy(img, tmp, NULL);
  IplImage * patch = cvCreateImage(cvSize(patchSize.r,patchSize.b),IPL_DEPTH_8U,1);
  cvResize(tmp,patch,CV_INTER_CUBIC);
  cvResetImageROI(img);
  cvReleaseImage(&tmp);
  return patch;
}

Patch::Patch(IplImage * img, BoundingBox b, BoundingBox patchSize)
{
    length = patchSize.r*patchSize.b;
    patch = (float*)calloc(length,sizeof(float));

    float mean = 0;

    /*
    IplImage * tmp = cvCreateImage(cvSize(patchSize.r,patchSize.b),IPL_DEPTH_8U,1);

    float scaleX = (b.r-b.l+1)/patchSize.r;
    float scaleY = (b.b-b.t+2)/patchSize.b;

    float m[6];
    CvMat M = cvMat( 2, 3, CV_32F, m );
    m[0]=scaleX;m[1]=0;m[2]=(b.l+b.r-1)/2;
    m[3]=0;m[4]=scaleY;m[5]=(b.t+b.b-1)/2;
    
    cvGetQuadrangleSubPix(img,tmp,&M);
    */
    IplImage *tmp = subImage(img,b,patchSize);

    unsigned char  *data = &CV_IMAGE_ELEM( tmp, unsigned char, 0,0 );
    for(int i=0;i<length;i++)
    {
      mean+=data[i];
      patch[i]=data[i];
    }
    mean/=length;
    variance = 0;
    for(int i=0;i<length;i++)
    {
      patch[i]-=mean;
      variance += (patch[i])*(patch[i]);
    }
    norm = variance;
    variance/=length;
    cvReleaseImage(&tmp);
}

float Patch::NCC(Patch *p)
{
  assert(this->length==p->length);
  float total=0;
  for(int i=0;i<this->length;i++)
    total+=patch[i]*p->patch[i];
  return (total / sqrt(norm*p->norm) + 1) / 2.0;
}
