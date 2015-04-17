MYCC	= clang
MYCC	= gcc

CC	= $(MYCC) -c -Wall
LINK	= $(MYCC) -o
LIBS	=

TARGETS = mandelbrotcmd mandelbrotcmdarg


all: $(TARGETS)


clean:
	rm -f *.o $(TARGETS)


libmandelbrot.o:	libmandelbrot.c libmandelbrot.h
	$(CC)		libmandelbrot.c

mandelbrotcmd.o:	mandelbrotcmd.c libmandelbrot.h
	$(CC)		mandelbrotcmd.c

mandelbrotcmd:				mandelbrotcmd.o libmandelbrot.o
	$(LINK) mandelbrotcmd		mandelbrotcmd.o libmandelbrot.o

mandelbrotcmdarg.o:	mandelbrotcmdarg.c libmandelbrot.h
	$(CC)		mandelbrotcmdarg.c

mandelbrotcmdarg:			mandelbrotcmdarg.o libmandelbrot.o
	$(LINK) mandelbrotcmdarg	mandelbrotcmdarg.o libmandelbrot.o

