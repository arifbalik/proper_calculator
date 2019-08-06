
SRC=../../euler
PARSER=$(SRC)/parser/parser
LEXER=$(SRC)/parser/lexer

BUILD_DIR = ../../build

C_FILES= $(SRC)/src/variables.c $(SRC)/src/function.c $(SRC)/src/error.c $(LEXER).c $(PARSER).c main.c 

all: $(PARSER).c $(LEXER).c
	mkdir $(BUILD_DIR)
	gcc -o $(BUILD_DIR)/linux $(C_FILES) -lm
	cd $(BUILD_DIR) && ./linux

clean:
	rm $(LEXER).c $(PARSER).c $(PARSER).h $(PARSER).out -rf $(BUILD_DIR)
	cd arch/$(ARCH)/$(MCU) && $(MAKE) clean
