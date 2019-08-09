
ARCH?=linux
MCU?=
CMD?= -i

SRC=euler/parser
PARSER=$(SRC)/parser
LEXER=$(SRC)/lexer

BUILD_DIR = build

C_FILES= euler/src/variables.c $(LEXER).c $(PARSER).c

all: $(PARSER).c $(LEXER).c
	cd arch/$(ARCH)/$(MCU) && $(MAKE)

$(LEXER).c : $(PARSER).c
	@echo "Compiling the lexer.."
	@re2c -W- -T -o $(LEXER).c $(LEXER).re

$(PARSER).c: clean
	@echo "Compiling the parser.."
	@lemon -c -s $(PARSER).y

clean:
	rm $(LEXER).c graphviz.txt $(PARSER).c $(PARSER).h $(PARSER).out -rf $(BUILD_DIR)
	cd arch/arm/stm32f103c8 && $(MAKE) $(CMD) clean
	cd arch/arm/stm32f746ng && $(MAKE) $(CMD) clean

graph:
	re2c -D -o graphviz.txt $(LEXER).re
