CC = clang++
CFLAGS = -g -Wall -Wextra
DEBUGFLAGS = -fno-omit-frame-pointer

all:
	$(CC) $(CFLAGS) -o crox Crox.cpp Scanner.cpp

printer:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -o ast_print AstPrinter.cpp