################################################################################
PROJECT_NAME = test
PROJECT_FILE = $(PROJECT_NAME)
PRE_DEFINED_MACRO := -DEXPORT -DLINUX # multiple predefined name
CC := g++

# internal flags
CCFLAGS     := -Wall
LDFLAGS     :=
# Build Type
# make DEBUG=1
DEBUG ?= 0
ifeq ($(DEBUG), 1)
	PRE_DEFINED_MACRO += -DDEBUG
	BUILD_TYPE := debug
	CCFLAGS += -g
else
    BUILD_TYPE := release
endif

INCLUDES  := -I../../include
LIBRARIES := -L../../$(BUILD_TYPE)
LIBRARIES += -Wl,-R../../$(BUILD_TYPE)
LIBRARIES += -lutils

ALL_CCFLAGS:=
ALL_CCFLAGS+=$(PRE_DEFINED_MACRO)
ALL_CCFLAGS+=$(CCFLAGS)
ALL_CCFLAGS+=$(INCLUDES)

ALL_LDFLAGS:=
#ALL_LDFLAGS+=$(ALL_CCFLAGS)
ALL_LDFLAGS+=$(LDFLAGS)
ALL_LDFLAGS+=$(LIBRARIES)

# Source list
HFILES   := $(wildcard *.h)
CFILES   := $(wildcard *.c)
CPPFILES := $(wildcard *.cpp)

SRCS:=${CFILES}
SRCS+=${CPPFILES}

OBJS:=${CFILES:.c=.o}
OBJS+=${CPPFILES:.cpp=.o}

ALL_CCFLAGS += --std=c++20 -c
ALL_LDFLAGS += 
################################################################################
# example
# all: library.cpp main.cpp
#
# $@ evaluates to all
# $< evaluates to library.cpp
# $^ evaluates to library.cpp main.cpp
################################################################################

# Target rules
all: build

build: $(PROJECT_FILE) filemove

check.deps:
$(PROJECT_FILE):$(OBJS)
	$(EXEC) $(CC) $^ -o $@ $(ALL_LDFLAGS) 

%.o:%.c
	$(EXEC) $(CC) $^ -o $@ $(ALL_CCFLAGS) 

%.o:%.cpp
	$(EXEC) $(CC) $^ -o $@ $(ALL_CCFLAGS) 

filemove:
# object files
	$(EXEC) mkdir -p ../../obj/$(BUILD_TYPE)
	$(EXEC) mv ./*.o ../../obj/$(BUILD_TYPE)
# binary file
	$(EXEC) mkdir -p ../../$(BUILD_TYPE)
	$(EXEC) mv $(PROJECT_FILE) ../../$(BUILD_TYPE)/$(PROJECT_FILE)

run:
	../../$(BUILD_TYPE)/$(PROJECT_FILE)

clean:
	rm -f $(PROJECT_FILE)
	rm -f $(OBJS)
	rm -f $(PROJECT_FILE:%=../../$(BUILD_TYPE)/%)
	rm -f $(OBJS:%=../../obj/$(BUILD_TYPE)/%)
