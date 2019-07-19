

pr: parser.c lexer.c variables.c
	@echo "Compiling.."
	gcc -o scientific_calculator parser.c lexer.c variables.c euler/trig.c -lm
	./scientific_calculator
	rm scientific_calculator lexer.c graphviz.txt parser.c parser.h parser.out

lexer.c: parser.c
	@echo "Compiling the lexer.."
	re2c -o lexer.c lexer.re
	re2c -D -o graphviz.txt lexer.re
parser.c:
	@echo "Compiling the parser.."
	lemon -c parser.y

clean:
	rm scientific_calculator lexer.c graphviz.txt parser.c parser.h parser.out

graph:
	re2c -D -o graphviz.txt lexer.re
