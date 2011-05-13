#include "YUV.h"


IplImage * readYUVImage(const char * filename)
{
  IplImage * tmp = cvLoadImage(filename,1);
  CvSize size = cvGetSize(tmp);
  IplImage * ret = cvCreateImage(size,IPL_DEPTH_8U,1);
  unsigned char *tmp_data = &CV_IMAGE_ELEM(tmp,unsigned char, 0,0);
  unsigned char  *ret_data = &CV_IMAGE_ELEM( ret, unsigned char, 0,0 );

  for(int y=0;y<size.height;y++)
  {
    unsigned char *tmp_data = &CV_IMAGE_ELEM(tmp,unsigned char, y,0);
    unsigned char  *ret_data = &CV_IMAGE_ELEM( ret, unsigned char, y,0 );    
    for(int x=0;x<size.width;x++)
    {
     float R = tmp_data[x*3+2];
     float G = tmp_data[x*3+1];
     float B = tmp_data[x*3+0];
     //ret_data[x] = (unsigned char)(0.299 * R + 0.587 * G + 0.114 * B);
     ret_data[x] = (unsigned char)((R+G+B)/3.0);
    }
  }
  return ret;
}
