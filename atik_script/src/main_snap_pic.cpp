#include "stdio.h"
#include "opencv2/highgui/highgui_c.h"
#include "ASICamera2.h"
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "pthread.h"

#ifdef GUI
#include "display.h"
#endif

// Initilize the camera and display information about that camera
int initilize_camera()
{
	// Get number of connected cameras
	int numDevices = ASIGetNumOfConnectedCameras();
	if(numDevices <= 0) {
		printf("no camera connected, now exiting\n");
		return false;
	}

	// Display the names of the connected cameras
	ASI_CAMERA_INFO ASICameraInfo;
	for(uint i = 0; i < numDevices; i++) {
		ASIGetCameraProperty(&ASICameraInfo, i);
		printf("%d %s\n",i, ASICameraInfo.Name);
	}
	printf("\nselecting camera 0 to preview\n");
	int cam_num = 0;

	// Test to make sure we can open the camera (and that we are root)
	if(ASIOpenCamera(cam_num) != ASI_SUCCESS) {
		printf("OpenCamera error,are you root?,now exiting\n");
		return -1;
	}

	// Initilize the camera that we selected
	ASIInitCamera(cam_num);

	// Dark Subtract mode, I assume will not be used, it brightens bright colors
	// and darkens the background to pitch black.
	// ASI_ERROR_CODE err = ASIEnableDarkSubtract(cam_num, "dark.bmp");
	// if(err == ASI_SUCCESS)
	// 	printf("load dark ok\n");
	// else
	// 	printf("load dark failed %d\n", err);

	// Display info about this cameras resolution and color type
	printf("%s information\n",ASICameraInfo.Name);
	int iMaxWidth = ASICameraInfo.MaxWidth;
	int iMaxHeight =  ASICameraInfo.MaxHeight;
	printf("resolution:%dX%d\n", iMaxWidth, iMaxHeight);
	const char* bayer[] = {"RG","BG","GR","GB"};
	if(ASICameraInfo.IsColorCam)
		printf("Color Camera: bayer pattern:%s\n",bayer[ASICameraInfo.BayerPattern]);
	else
		printf("Mono camera\n");

	// Display what type of controls this camera supports
	ASI_CONTROL_CAPS ControlCaps;
	int iNumOfCtrl = 0;
	ASIGetNumOfControls(cam_num, &iNumOfCtrl);
	printf("Control Caps\n");
	for(uint i = 0; i < iNumOfCtrl; i++)
	{
		ASIGetControlCaps(cam_num, i, &ControlCaps);
		printf("%s\n", ControlCaps.Name);
	}

	// Display the current temperature of the camera sensor
	long ltemp = 0;
	ASI_BOOL bAuto = ASI_FALSE;
	ASIGetControlValue(cam_num, ASI_TEMPERATURE, &ltemp, &bAuto);
	printf("sensor temperature:%02f\n", (float)ltemp/10.0);

	// return the number of the camera the user selected
	return cam_num;
}

// Sets the camera parameters to the appropiate values
void setup_camera(int cam_num) {
	int bin = 1, width=100, height=100;
	ASI_IMG_TYPE image_type = ASI_IMG_RAW8;
	if (ASI_SUCCESS == ASISetROIFormat(cam_num, width, height, bin, image_type) )
		printf("\nSet ROI image format success width:%d height:%d bin:%d image_type:%d \n", width, height, bin, image_type);

// Used later I think
	long imgSize = width*height*(1 + (image_type==ASI_IMG_RAW16));
	unsigned char* imgBuf = new unsigned char[imgSize];
// Used later I think

	int gain = 0;
	if (ASI_SUCCESS == ASISetControlValue(cam_num, ASI_GAIN, gain, ASI_FALSE) )
		printf("Set gain: %d \n", gain);

	int exp_ms = 10;
	if (ASI_SUCCESS == ASISetControlValue(cam_num, ASI_EXPOSURE, exp_ms*1000, ASI_FALSE) )
		printf("Set exposure time(ms): %d \n");

	int bandwidth_overload = 40;
	if (ASI_SUCCESS == ASISetControlValue(cam_num, ASI_BANDWIDTHOVERLOAD, bandwidth_overload, ASI_FALSE) )
		printf("Set bandwidthoverload: %d \n");

}

void start_exposure() {return;}
void wait_until_done() {return;}
void get_img() {return;}
void clean_up() {return;}

int main() {
	int cam_num = initilize_camera();
	if (cam_num == -1)
		return -1;

	setup_camera(cam_num);

// @TODO: all this stuff below (and write display.cpp)
	start_exposure();
	wait_until_done();
	get_img();
#ifdef GUI
	display_img();
#endif

	clean_up();
}
