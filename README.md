Mandelbrot
==========
Mandelbrot set in C

There is no makefile, to compile just do:
> gcc mandelbrot.c

This will produce file called ./a.out

Then you can start it just with:

>./a.out > x.pgm

x.pgm is text based graphic format, not so different from GIF, you can see it with several image viewers.

You can see the image online here:

https://www.flickr.com/photos/nmmmnu/16892543768/

You can play with resolution (**MAX**) and zoom, just make sure there is no int overflow in **uint** type.

Increasing **ITER** will increase colors, but 16 or 32 colors give best results. I did not tried over 255 colors, but there are no reason not to work, you need just to increase **color** type.

Distribution is GPL.

Nikolay Mihaylov

2015-04-08

