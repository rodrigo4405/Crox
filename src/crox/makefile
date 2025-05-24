CC = clang++
CFLAGS = -g -Wall -Wextra -Wno-switch
DEBUGFLAGS = -fno-omit-frame-pointer

all:
	$(CC) $(CFLAGS) -o crox Crox.cpp Scanner.cpp Parser.cpp Interpreter.cpp

printer:
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -o ast_print AstPrinter.cpp