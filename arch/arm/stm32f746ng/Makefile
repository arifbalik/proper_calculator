MCU=stm32f746ng

EULER_DIR = ../../../euler
BUILD_DIR = ../../../build
PROJECT = $(BUILD_DIR)/$(MCU)


SHARED_DIR = $(BUILD_DIR)
CFILES =   $(EULER_DIR)/src/variables.c $(EULER_DIR)/parser/parser.c $(EULER_DIR)/parser/lexer.c $(MCU).c
AFILES +=

LDLIBS += -lm

# TODO - you will need to edit these two lines!
DEVICE=$(MCU)

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
