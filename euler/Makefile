compile: lexer.re parser.y
	lemon -c parser.y
	re2c -o lexer.c lexer.re
	gcc -o euler lexer.c parser.c variables.c
	rm parser.out
	./euler
	
clean:
	rm euler lexer.c graphviz.txt parser.c parser.h parser.out

graph:
	re2c -D -o graphviz.txt lexer.re
