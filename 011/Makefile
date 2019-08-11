# here need your modification
MAJOR = 2
MINOR = 2
REVISION = 0

TARGET=main
INCLUDE_DIR=include
MODULE_DIR=

all:x64
DEBUG?=1

CXXFLAGS=-std=c++17 -MMD
LDFLAGS=-Llibs/$(PLATFORM)
ifeq ($(DEBUG),1)
FSANITIZE=-fsanitize=address #-fsanitize-recover=address
CXXFLAGS+=-g $(FSANITIZE)
LDFLAGS+=$(FSANITIZE)
endif
LDFLAGS+=-Wl,-rpath='libs/$(PLATFORM)'

# main.o:CXXFLAGS+=`pkg-config --cflags opencv`
# LDLIBS=`pkg-config --libs opencv` -l3rdparty

# default link rule $(LOADLIBES) $(LDLIBS)
# It makes a difference where in the command you write this option;
# the linker searches and processes libraries and object files in the order they are
# specified.  Thus, foo.o -lz bar.o searches library z after file foo.o but before bar.o.
# If bar.o refers to functions in z, those functions may not be loaded.(man gcc)
#LOADLIBES=-luv

# supported platform 
arm:PLATFORM=arm
arm:CXX=arm-linux-gnueabihf-g++
arm:$(TARGET)
	@echo "make arm"	

x64:PLATFORM=x64
x64:CXX=g++
x64:$(TARGET)
	@echo "make x64"	

help:
	@echo "make [x64|arm] [DEBUG=1|0], the first one is the default"
	@echo "make <help|clean|distclean>"


#*************************************************************
# there is no need modification below
#*************************************************************
SRC:=$(foreach mod,$(MODULE_DIR),$(wildcard $(mod)/*.cpp))
OBJ:=$(patsubst %.cpp,%.o,$(SRC))
DEP:=$(patsubst %.cpp,%.d,$(SRC))
CXXFLAGS+=$(addprefix -I,$(INCLUDE_DIR) $(MODULE_DIR))

BUILD = $(shell date +%c)
VERSION = "$(TARGET) version $(MAJOR).$(MINOR).$(REVISION)\ncompiled $(BUILD)\n"
$(TARGET).o:CXXFLAGS += -D'VERSION=$(VERSION)' -D'NAME="$(TARGET)"'

CC=$(CXX)
CROSS_COMPILE=$(subst g++,,$(CXX))
AR=$(CROSS_COMPILE)ar

$(TARGET):$(TARGET).o $(OBJ)

# at the very beginning, there is no $(DEP), but all files would be compiled
# $(DEP) would be generated too, it will works then.
# but if you delete one .d file, and then edit related .h file, it can't works.
-include $(DEP)

distclean:clean
	rm -rf $(OBJ) $(DEP)
clean:
	rm -rf $(TARGET) *.o *.d

.PHONY:distclean clean
