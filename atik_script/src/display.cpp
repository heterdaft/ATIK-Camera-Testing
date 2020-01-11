#include "stdio.h"
#include "opencv2/highgui/highgui_c.h"
#include "ASICamera2.h"
#include <unistd.h>

void display (uint width, uint height, unsigned short* pImg16bit) {
	int displayWid = 1280, displayHei = 960;
	IplImage *pRgb;
	pRgb=cvCreateImage(cvSize(displayWid, displayHei), IPL_DEPTH_16U, 1);
	unsigned short *pCv16bit = (unsigned short *)(pRgb->imageData); // OpenCV

	for(int y = 0; y < displayHei; y++)
	{
		memcpy(pCv16bit, pImg16bit, displayWid*2);
		pCv16bit+=displayWid;
		pImg16bit+=width;
	}
	cvSaveImage("jpg_image.jpg", pRgb); // OpenCV
}
