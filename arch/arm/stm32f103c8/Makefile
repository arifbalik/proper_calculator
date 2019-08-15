MCU=stm32f103c8

EULER_DIR = ../../../euler
BUILD_DIR = ../../../build
PROJECT = $(BUILD_DIR)/$(MCU)


SHARED_DIR = $(BUILD_DIR)
CFILES =$(EULER_DIR)/src/*.c $(EULER_DIR)/parser/tokenizer.c $(EULER_DIR)/parser/grammar.c $(EULER_DIR)/parser/symbol_table.c $(MCU).c
AFILES +=

ARCH_FLAGS = -mfloat-abi=softfp #this series has no hard float

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
