platform = x64
CFLAGS   = -W -Wall -O3 -std=c99
CXXFLAGS = -W -Wall -O3 -std=c++11

.PHONY : clean default

default : take_pic_main

take_pic_main : libUSB.o take_pic_main.o 
	$(CXX) $^ libASICamera2_patched.a -lpthread -lusb-1.0 -o $@

take_pic_main.o : take_pic_main.c

libUSB.o : libUSB.c

libASICamera2_patched.a : libASICamera2.a

clean :
	$(RM) take_pic_main
	$(RM) take_pic_main.o libUSB.o
	$(RM) usb_control_transfer.log
	$(RM) *~


# CFLAGS = -W -Wall -O3 -std=c99
# CXXFLAGS = -W -Wall -Wextra -O3 -std=c++11
#
# # Library path for atik camera SDK
# DIR = $(shell pwd)
# DEVELOPMENT_LIBRARY_PATH = $(DIR)/lib/$(platform)
# DEVELOPMENT_LIBRARY_LINK = -Wl,-rpath=$(DEVELOPMENT_LIBRARY_PATH)
# LIB_PATHS = -L$(DEVELOPMENT_LIBRARY_PATH)
# USB_LIBS = -lusb-1.0 -lpthread
# LIBS = $(USB_LIBS)
#
# # Includes path
# INCLUDES = -Iinclude
#
# # Platform dependent flags
# ifeq ($(platform), x86)
# CFLAGS += -m32
# CFLAGS += -lrt
# endif
#
# ifeq ($(platform), x64)
# CFLAGS += -m64
# CFLAGS += -lrt
# endif
#
# .PHONY : clean default
#
# default : libUSB rev_test
#
# rev_test : take_pic_main.o
# 	$(CXX) $(CXXFLAGS) $(LIB_PATHS) $(DEVELOPMENT_LIBRARY_LINK) $^ $(DIR)/lib/$(platform)/libASICamera2.a libUSB.o $(LIBS) -o $@
#
# libUSB : libUSB.o
# 	$(CXX) -shared $(USB_LIBS) -o $(DEVELOPMENT_LIBRARY_PATH)/libUSB-1.0.so.0 libUSB.o  -lm
# 	ar rc $(DEVELOPMENT_LIBRARY_PATH)/libUSB-1.0.a libUSB.o
#
# libUSB.o : src/libUSB.c
# 	$(CXX) -c $^ $(INCLUDES)
#
# take_pic_main.o : src/take_pic_main.c
# 	$(CXX) -c $^ $(INCLUDES)
#
# clean :
# 	$(RM) rev_test $(DEVELOPMENT_LIBRARY_PATH)/libUSB-1.0.so.0 $(DEVELOPMENT_LIBRARY_PATH)/libUSB-1.0.a
# 	$(RM) libUSB.o take_pic_main.o
# 	$(RM) incoming.log outgoing.log
# 	$(RM) *~
