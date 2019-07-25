PROJECT = ../../../build/stm32f103c8
BUILD_DIR = ../../../build

SHARED_DIR = ../../../build
CFILES =   ../../../parser/lexer.c stm32f103c8.c
AFILES +=

ARCH_FLAGS = -mfloat-abi=hard -mfpu=fpv4-sp-d16

# TODO - you will need to edit these two lines!
DEVICE=stm32f103x8
OOCD_FILE = board/stm32f103-generic.cfg

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR))
OPENCM3_DIR=../libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk

re2c:
	re2c -i -o re2c.c re2c.re
