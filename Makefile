
ARCH?=linux
MCU?=
CMD?= -i

SRC=euler/parser
PARSER=$(SRC)/grammar
LEXER=$(SRC)/tokenizer

BUILD_DIR = build

all: $(PARSER).c $(LEXER).c dconfig
	doxygen dconfig
	cd arch/$(ARCH)/$(MCU) && $(MAKE)

$(LEXER).c : $(PARSER).c
	@echo "Compiling the lexer.."
	@re2c -W -T -o $(LEXER).c $(LEXER).re

$(PARSER).c: clean
	@echo "Compiling the parser.."
	@gcc -o  $(SRC)/lemon/lemon $(SRC)/lemon/lemon.c
	@./$(SRC)/lemon/lemon -s -p $(PARSER).y

clean:
	rm $(LEXER).c graphviz.txt $(PARSER).c $(PARSER).h $(PARSER).out $(SRC)/lemon/lemon -rf $(BUILD_DIR) doc

graph:
	re2c -D -o graphviz.txt $(LEXER).re
