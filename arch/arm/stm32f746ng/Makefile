MCU=stm32f746ng

EULER_DIR = ../../../euler
BUILD_DIR = ../../../build
PROJECT = $(BUILD_DIR)/$(MCU)

SHARED_DIR = $(BUILD_DIR)
CFILES= $(EULER_DIR)/src/*.c $(EULER_DIR)/parser/tokenizer.c $(EULER_DIR)/parser/grammar.c $(EULER_DIR)/parser/symbol_table.c $(MCU).c
AFILES +=
INCLUDES += -I$(EULER_DIR)/inc -I$(EULER_DIR)/parser 

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
