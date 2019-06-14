#!bash
rm -f test/*.pgm
mkdir -p test
c=0
zoom=0.02
while [ $(echo "${zoom}>0.0002" | bc) -eq 1 ]
do
	printf "\r%05d zoom=%.04f" $c ${zoom}
	./mandelbrotcmdarg 800 480  2 5  256  -0.079440000 0.658950000 ${zoom} > "test/$(printf "%05d" $c).pgm"
	c=$(expr $c + 1)
	zoom=$(echo "${zoom}*0.999" | bc)
	#sleep 0.1
done
echo
ffmpeg -r 15 -f image2 -s 800x480 -start_number 0 -i test/%05d.pgm -vcodec libx264 -crf 25  -pix_fmt yuv420p test.mp4
