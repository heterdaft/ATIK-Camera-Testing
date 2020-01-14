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

const uint HEIGHT = 960;
const uint WIDTH = 1280;

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
	int bin = 1;
	ASI_IMG_TYPE image_type = ASI_IMG_RAW8;
	// This call requires width divisible by 8 and height divisible by 2
	// Also make sure width*height is divisible by 1024
	if (ASI_SUCCESS == ASISetROIFormat(cam_num, WIDTH, HEIGHT, bin, image_type) )
		printf("\nSet ROI image format success width:%d height:%d bin:%d image_type:%d \n", WIDTH, HEIGHT, bin, image_type);

	int gain = 50;
	if (ASI_SUCCESS == ASISetControlValue(cam_num, ASI_GAIN, gain, ASI_FALSE) )
		printf("Set gain: %d \n", gain);

	// For some reason this function must be called before the exposure is set
	int bandwidth_overload = 40;
	if (ASI_SUCCESS == ASISetControlValue(cam_num, ASI_BANDWIDTHOVERLOAD, bandwidth_overload, ASI_FALSE) )
		printf("Set bandwidthoverload: %d \n", bandwidth_overload);

	int exp_ms = 10;
	if (ASI_SUCCESS == ASISetControlValue(cam_num, ASI_EXPOSURE, exp_ms*1000, ASI_FALSE) )
		printf("Set exposure time(ms): %d \n", exp_ms);

}

void start_exposure(int CamNum) {
	ASIStartExposure(CamNum, ASI_FALSE);
}

void wait_until_done(int CamNum, uint sleep_time) {
	usleep(sleep_time);
	ASI_EXPOSURE_STATUS status = ASI_EXP_WORKING;
	while(status == ASI_EXP_WORKING) {
		ASIGetExpStatus(CamNum, &status);
	} // Do nothing
}

static void dump_hex_data(FILE * f, unsigned char * data, unsigned size)
{
    const unsigned MAX_BYTES_PER_LINE = 64;
    unsigned count = 0;

    for (unsigned i = 0; i < size; ++i)
    {
        if (count != 0)
        {
            fprintf(f, " ");
        }
        fprintf(f, "%02x", data[i]);
        ++count;
        if (count == MAX_BYTES_PER_LINE)
        {
            fprintf(f, "\n");
            count = 0;
        }
    }
    if (count != 0)
    {
        fprintf(f, "\n");
        count = 0;
    }
}

void get_img(int CamNum, long imgSize, unsigned char* imgBuf) {
	if (ASI_SUCCESS != ASIGetDataAfterExp(CamNum, imgBuf, imgSize) )
	{
		printf("There was an error getting the exposure\n");
	}

	// Save the binary data
	FILE *fptr;
	fptr = fopen("binary_image.bin","wb");
	if(fptr == NULL) {
	  printf("Error! Printing binary image\n");
	} else {
		//fwrite(imgBuf, imgSize, 1, fptr);
		dump_hex_data(fptr, imgBuf, imgSize);
	}

	fclose(fptr);
}

void clean_up(int CamNum, unsigned char* imgBuf) {
	printf("Finished processing image\n");
	free(imgBuf);
	ASIStopExposure(CamNum);
	ASICloseCamera(CamNum);
}

int main() {
	int cam_num = initilize_camera();
	if (cam_num == -1)
		return -1;

	setup_camera(cam_num);

	start_exposure(cam_num);
	wait_until_done(cam_num, 10*1000); // 10ms (as set in setup_camera() )

	long imgSize = WIDTH*HEIGHT*2; // times 2 because image is ASI_IMG_RAW16 type
	unsigned char* imgBuf = (unsigned char*) malloc(imgSize);
	get_img(cam_num, imgSize, imgBuf);

#ifdef GUI
	display_img(WIDTH, HEIGHT, (unsigned short *)imgBuf);
#endif

	clean_up(cam_num, imgBuf);
}
