
SRC=../../euler
PARSER=$(SRC)/parser/*
LEXER=$(SRC)/parser/tokenizer

BUILD_DIR = ../../build

C_FILES=  $(SRC)/src/*.c $(PARSER).c main.c

all: $(PARSER).c $(LEXER).c
	mkdir $(BUILD_DIR)
	gcc -o $(BUILD_DIR)/linux $(C_FILES) -Wall -Werror -std=c11 -lm -ggdb3 -D_DEFAULT_SOURCE
	cd $(BUILD_DIR)

clean:
	rm $(LEXER).c $(PARSER).c $(PARSER).h $(PARSER).out -rf $(BUILD_DIR)
	cd arch/$(ARCH)/$(MCU) && $(MAKE) clean
