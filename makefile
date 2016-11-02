all:
	bison -d myscanner.y
	lex myscanner.l
	gcc arvore.c myscanner.tab.c lex.yy.c -o compilador
