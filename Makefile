
ARCH?=linux
MCU?=
CMD?= -i

SRC=euler/parser
PARSER=$(SRC)/grammar
LEXER=$(SRC)/tokenizer

BUILD_DIR = build

C_FILES= euler/src/variables.c $(LEXER).c $(PARSER).c

all: $(PARSER).c $(LEXER).c
	cd arch/$(ARCH)/$(MCU) && $(MAKE)

$(LEXER).c : $(PARSER).c
	@echo "Compiling the lexer.."
	@re2c -W -T -o $(LEXER).c $(LEXER).re

$(PARSER).c: clean
	@echo "Compiling the parser.."
	@lemon -c -s $(PARSER).y

clean:
	rm $(LEXER).c graphviz.txt $(PARSER).c $(PARSER).h $(PARSER).out -rf $(BUILD_DIR)

graph:
	re2c -D -o graphviz.txt $(LEXER).re
