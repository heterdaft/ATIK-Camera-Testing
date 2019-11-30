ver = debug
platform = x64

CC = g++
#INCLIB = /usr/local/include
#LDLIB = /usr/local/lib
OPENCV_COMPATIBILITY = --static -lzlib
OPENCV = -lopencv_core -lopencv_highgui -lopencv_imgproc#$(shell pkg-config --cflags opencv) $(shell pkg-config --libs opencv) -I/usr/include/opencv2
#USB =  -I../libusb/include  -L../libusb/$(platform) -lusb-1.0


# Library path and includes path for atik camera SDK
LIBSPATH = -L../atik_libraries/$(platform) -Iinclude



ifeq ($(ver), debug)
DEFS = -D_LIN -D_DEBUG
CFLAGS = -g  -I $(INCLIB) -L $(LDLIB) $(DEFS) $(COMMON) $(LIBSPATH)  -lpthread  -DGLIBC_20
else
DEFS = -D_LIN
CFLAGS =  -O3 -I $(INCLIB) -L $(LDLIB) $(DEFS) $(COMMON) $(LIBSPATH)  -lpthread  -DGLIBC_20
endif

ifeq ($(platform), x86)
CFLAGS += -m32
CFLAGS += -lrt
endif


ifeq ($(platform), x64)
CFLAGS += -m64
CFLAGS += -lrt
endif

ifeq ($(platform), armv5)
CC = arm-none-linux-gnueabi-g++
AR= arm-nonelinux-gnueabi-ar
CFLAGS += -march=armv5
CFLAGS += -lrt
endif


ifeq ($(platform), armv6)
CC = arm-bcm2708hardfp-linux-gnueabi-g++
AR= arm-bcm2708hardfp-linux-gnueabi-ar
CFLAGS += -march=armv6
CFLAGS += -lrt
endif

ifeq ($(platform), armv7)
CC = arm-linux-gnueabihf-g++
AR= arm-linux-gnueabihf-ar
CFLAGS += -march=armv7 -mcpu=cortex-m3 -mthumb
endif
#ifeq ($(platform), armhf)
#CC = arm-linux-gnueabihf-g++
#AR= arm-linux-gnueabihf-ar
#CFLAGS += -march=armv5
#LDLIB += -lrt
#endif

# Tell the program that the ATIK library is located at this specific location (useful for later testing)
ATIK_library = -Wl,-rpath=../atik_libraries/$(platform)/

objects = main_snap_pic.o display.o

all: test_snap_pic.o
	mkdir -p bin/$(platform)/
	$(CC)  -o bin/$(platform)/test_snap_pic main_snap_pic.o $(CFLAGS) $(OPENCV) -lASICamera2 $(ATIK_library)

test_snap_pic.o:src/main_snap_pic.cpp
	mkdir -p bin/$(platform)/
	$(CC)  -c src/main_snap_pic.cpp $(CFLAGS) $(OPENCV) -lASICamera2 $(ATIK_library)

GUI_FLAG = -Dcpp_variable=GUI
gui: test_snap_pic.o gui.o
	mkdir -p bin/$(platform)/
	$(CC) -o bin/$(platform)/gui $(objects) $(CFLAGS) $(OPENCV) -lASICamera2 $(ATIK_library)

gui.o:src/display.cpp
	mkdir -p bin/$(platform)/
	$(CC)  -c src/display.cpp $(GUI_FLAG) $(CFLAGS) $(OPENCV) -lASICamera2 $(ATIK_library)

clean:
	rm -f $(objects)
#pkg-config libusb-1.0 --cflags --libs
#pkg-config opencv --cflags --libs
