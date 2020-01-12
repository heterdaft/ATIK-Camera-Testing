ver = debug
platform = x64
directory = $(shell pwd)
CC = g++
#INCLIB = /usr/local/include
#LDLIB = /usr/local/lib
OPENCV_COMPATIBILITY = --static -lzlib
OPENCV = -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs#$(shell pkg-config --cflags opencv) $(shell pkg-config --libs opencv) -I/usr/include/opencv2
#USB =  -I../libusb/include  -L../libusb/$(platform) -lusb-1.0


# Library path and includes path for atik camera SDK
DEVELOPMENT_LIBRARY_PATH = $(directory)/../atik_libraries/$(platform)
LIBSPATH = -L$(DEVELOPMENT_LIBRARY_PATH) -Iinclude

DEVELOPMENT_LIBRARY_LINK = -Wl,-rpath=$(DEVELOPMENT_LIBRARY_PATH)


ifeq ($(ver), debug)
DEFS = -D_LIN -D_DEBUG
CFLAGS = -g  -I $(INCLIB) -L $(LDLIB) $(DEFS) $(COMMON) $(LIBSPATH)  -lpthread \
 -DGLIBC_20 $(DEVELOPMENT_LIBRARY_LINK)
else
DEFS = -D_LIN
CFLAGS =  -O3 -I $(INCLIB) -L $(LDLIB) $(DEFS) $(COMMON) $(LIBSPATH)  -lpthread \
 -DGLIBC_20 $(DEVELOPMENT_LIBRARY_LINK)
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

objects = main_snap_pic.o display.o

GUI_FLAG=-DGUI=1

all: test_snap_pic.o
	mkdir -p bin/$(platform)/
	$(CC)  -o bin/$(platform)/test_snap_pic.out main_snap_pic.o $(CFLAGS) $(OPENCV) -lASICamera2

test_snap_pic.o:src/main_snap_pic.cpp
	mkdir -p bin/$(platform)/
	$(CC)  -c src/main_snap_pic.cpp $(CFLAGS) $(OPENCV) -lASICamera2

gui: test_snap_pic_gui.o gui.o
	mkdir -p bin/$(platform)/
	$(CC) -o bin/$(platform)/gui.out $(objects) $(CFLAGS) $(OPENCV) -lASICamera2

test_snap_pic_gui.o:src/main_snap_pic.cpp
	mkdir -p bin/$(platform)/
	$(CC)  -c src/main_snap_pic.cpp $(GUI_FLAG) $(CFLAGS) $(OPENCV) -lASICamera2

gui.o:src/display.cpp
	mkdir -p bin/$(platform)/
	$(CC)  -c src/display.cpp $(CFLAGS) $(OPENCV) -lASICamera2 $(ATIK_library)

clean:
	rm -f $(objects)
#pkg-config libusb-1.0 --cflags --libs
#pkg-config opencv --cflags --libs
