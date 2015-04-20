Mandelbrot
==========
Mandelbrot set in C

I kept original version, because is easy to understand how it works. It is kept in original/mandelbrot_original.c

Current version is optimized and have several advantages:

- faster,
- uses no array,
- is able to zoom
- have different coloring technics.

## Build

To build everything, do
> make

This will produce:

- **mandelbrotcmd** - program where you can play with preprocessor and recompile
- **mandelbrotcmdarg** - program that support arguments and you can use it "interactively", without recompilation.

## How to build the original version:

There is no makefile, to compile just do:
> gcc mandelbrot_original.c

This will produce file called ./a.out

## Usage:

You can start the programs that does not require any arguments simply as:
>./mandelbrotcmd > x.pgm

>./a.out > x.pgm

x.pgm is text based graphic format, not so different from GIF, you can see it with several image viewers.

Usage of **mandelbrotcmdarg** is similar:

Print arguments:
>./mandelbrotcmdarg

Produce a picture and save it in file **x.pgm**:
>./mandelbrotcmdarg 800 600 0 0 16 0 0 3 > x.pgm

Now you can use batch files and create a video using **mandelbrotcmdarg** and ffmpeg...

##Online gallery:

https://www.flickr.com/photos/nmmmnu/sets/72157651532271190/

##Licence

License GPLv3+: GNU GPL version 3 or later

http://gnu.org/licenses/gpl.html

This is free software: you are free to change and redistribute it.

Nikolay Mihaylov

2015-04-08

##Version history
- 2015-04-08 - Original release
- 2015-04-14 - Optimized release
- 2015-04-17 - Release with program arguments
- 2015-04-20 - Add Conjugate Mandelbrot
