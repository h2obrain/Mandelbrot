CC     = gcc
CFLAGS = -Wall -O3 -std=gnu18
CC   = gcc $(CFLAGS) -c
LINK = gcc $(CFLAGS) -o
LIBS =
#-lquadmath

TARGETS = mandelbrotcmd mandelbrotcmdarg


all: $(TARGETS)


clean:
	rm -f *.o $(TARGETS)

color_conversions.o: color_conversions.c color_conversions.h
	$(CC) color_conversions.c

libmandelbrot.o:     libmandelbrot.c libmandelbrot.h color_conversions.h
	$(CC) libmandelbrot.c

mandelbrotcmd.o:     mandelbrotcmd.c libmandelbrot.h
	$(CC) mandelbrotcmd.c

mandelbrotcmd:       mandelbrotcmd.o libmandelbrot.o color_conversions.o
	$(LINK) mandelbrotcmd mandelbrotcmd.o libmandelbrot.o color_conversions.o

mandelbrotcmdarg.o:  mandelbrotcmdarg.c libmandelbrot.h
	$(CC) mandelbrotcmdarg.c

mandelbrotcmdarg:    mandelbrotcmdarg.o libmandelbrot.o color_conversions.o
	$(LINK) mandelbrotcmdarg mandelbrotcmdarg.o libmandelbrot.o color_conversions.o

color_conversions: color_conversions.c color_conversions.h
	$(LINK) color_conversions -DTEST color_conversions.c
