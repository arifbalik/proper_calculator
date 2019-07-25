
ARCH?=arm
MCU?=stm32f103c8
CMD?=

SRC=parser
PARSER=$(SRC)/parser
LEXER=$(SRC)/lexer

C_FILES = variables.c

all: $(PARSER).c $(LEXER).c
	rm $(PARSER).out

$(LEXER).c : $(PARSER).c
	@echo "Compiling the lexer.."
	@re2c --tags -i -o $(LEXER).c $(LEXER).re

$(PARSER).c:
	@echo "Compiling the parser.."
	@lemon -c $(PARSER).y

clean:
	rm $(LEXER).c graphviz.txt $(PARSER).c $(PARSER).h $(PARSER).out -rf ../build
	cd arch/$(ARCH)/$(MCU) && $(MAKE) clean

graph:
	re2c -D -o graphviz.txt $(LEXER).re

target:
	cd arch/$(ARCH)/$(MCU) && $(MAKE) $(CMD)
