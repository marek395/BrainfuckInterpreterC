CC=gcc
CFLAGS=-Wall -Wextra -g
INCLUDES=	# -I Include directories for header files
LIBS=	# -l Libraries to link against

LDFLAGS=	# -L Look for libraries in lib/ directory

all:
	$(CC) $(CFLAGS) $(INCLUDES) src/main.c $(LIBS) $(LDFLAGS) -o app.exe

clean:
	del /Q build\app.exe 2>nul
