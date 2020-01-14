#include <unistd.h>
#include <stdio.h>
#include "libUSB.h"
#include "ASICamera2.h"
#include <stdlib.h> // free(), malloc(), etc
#include <unistd.h> // usleep()

// Definitions of constants that will always hold in our setup
#define CAMNUM 0
#define IMG_WIDTH 104
#define IMG_HEIGHT 104

int setup_camera(unsigned int exp_ms) {

  printf("ASIGetNumOfConnectedCameras()\n");
  int numDevices = ASIGetNumOfConnectedCameras();
	if(numDevices <= 0) {
		printf("no camera connected, now exiting\n");
		return 0;
	}

  printf("ASIOpenCamera()\n");
  if(ASIOpenCamera(CAMNUM) != ASI_SUCCESS) {
		printf("OpenCamera error,are you root?,now exiting\n");
		return 0;
	}
  printf("ASIInitCamera()\n");
  ASIInitCamera(CAMNUM);

  int bin = 1;
	ASI_IMG_TYPE image_type = ASI_IMG_RAW8;
  printf("ASISetROIFormat()\n");
	if (ASI_SUCCESS == ASISetROIFormat(CAMNUM, IMG_WIDTH, IMG_HEIGHT, bin, image_type) )
		printf("\nSet ROI image format success width:%d height:%d bin:%d image_type:%d \n"
      , IMG_WIDTH, IMG_HEIGHT, bin, image_type);

	int gain = 100;
  printf("ASISetControlValue(GAIN)\n");
	if (ASI_SUCCESS == ASISetControlValue(CAMNUM, ASI_GAIN, gain, ASI_FALSE) )
		printf("Set gain: %d \n", gain);

  int bandwidth_overload = 40;
  printf("ASISetControlValue(BANDWIDTHOVERLOAD)\n");
  if (ASI_SUCCESS == ASISetControlValue(CAMNUM, ASI_BANDWIDTHOVERLOAD, bandwidth_overload, ASI_FALSE) )
    printf("Set bandwidthoverload: %d \n", bandwidth_overload);

  printf("ASISetControlValue(EXPOSURE)\n");
	if (ASI_SUCCESS == ASISetControlValue(CAMNUM, ASI_EXPOSURE, exp_ms*1000, ASI_FALSE) )
		printf("Set exposure time(ms): %d \n", exp_ms);

  return 1;
}

void wait_until_done(unsigned int sleep_time) {
	sleep(sleep_time / 1000);
	ASI_EXPOSURE_STATUS status = ASI_EXP_WORKING;
	while(status == ASI_EXP_WORKING) {
    //printf("ASIGetExpStatus()\n");
    ASIGetExpStatus(CAMNUM, &status);
  }
}

void get_img(long imgSize, unsigned char* imgBuf) {
  printf("ASIGetDataAfterExp()\n");
	ASIGetDataAfterExp(CAMNUM, imgBuf, imgSize);

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

void clean_up(unsigned char* imgBuf) {
	printf("Finished processing image\n");
	free(imgBuf);
  printf("ASIStopExposure()\n");
	ASIStopExposure(CAMNUM);
  printf("ASICloseCamera()\n");
	ASICloseCamera(CAMNUM);
}

int main() {
  printf("Start taking picture\n");


  long img_size = IMG_HEIGHT * IMG_WIDTH;
  unsigned char* image = malloc(img_size);

  unsigned int exp_ms = 100;
  setup_camera(exp_ms);
  printf("ASIStartExposure()\n");
  ASIStartExposure(CAMNUM, ASI_FALSE);
  wait_until_done(exp_ms);
  get_img(img_size, image);

  //clean_up(image);

  printf("Finished taking picture\n");
}
