MCU=stm32f746ng

PROJECT = ../../../build/$(MCU)
BUILD_DIR = ../../../build

SHARED_DIR = ../../../build
CFILES =   ../../../euler/src/variables.c ../../../parser/parser.c ../../../parser/lexer.c $(MCU).c
AFILES +=

LDLIBS += -lm

# TODO - you will need to edit these two lines!
DEVICE=stm32f746ng

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
