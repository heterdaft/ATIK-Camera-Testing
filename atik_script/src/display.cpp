#include "stdio.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/highgui/highgui_c.h"
#include <opencv2/imgproc/imgproc.hpp>
#include "ASICamera2.h"
#include <unistd.h>

using namespace cv;

void display_img(uint width, uint height, void* pImg8bit) {
	int displayWid = 1280, displayHei = 960;
	printf("create img\n");
	Mat img8uc1(width, height, CV_8UC1, pImg8bit);
	Mat img_scaled8uc1;

	printf("Created image matrices\n");
	resize(img8uc1, img_scaled8uc1, Size(displayWid, displayHei), 0, 0, INTER_CUBIC);

	printf("Save img\n");
	imwrite("jpg_image_scaled.jpg", img_scaled8uc1);
	imwrite("jpg_image.jpg", img8uc1);
}
