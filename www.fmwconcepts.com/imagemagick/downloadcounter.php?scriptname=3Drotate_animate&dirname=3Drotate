#!/bin/bash
#
# Provide the input and perspective output names
# perspective output name must be .gif
ifile=mandril.jpg
pfile=perspective_animation.gif
# provide the frame delay in the gif animation
delay=10
# note - choose anginc and initang to keep from having a frame looking directly edge on
# otherwise get a blank white picture!
anginc=10
initang=5
tmp="perspective_animate_$$.png"
trap "rm -f $tmp; exit 0" 0
trap "rm -f $tmp; exit 1" 1 2 3 15
function imagesize
	{
	width=`identify -format %w $ifile`
	height=`identify -format %h $ifile`
	}
imagesize
ang=$initang
xx=x
while [ $ang -le 360 ]
	do
	echo "ang=$ang"
	if [ $ang -gt 180 ] 
		then
		newang=`expr $ang - 360`
	else
		newang=$ang
	fi
	echo "newang=$newang"
#   animate pan
#	3Drotate pan=$newang zoom=-1.5 $ifile $tmp
#   animate tilt
#	3Drotate tilt=$newang zoom=-1.5 $ifile $tmp
#   animate roll
#	3Drotate roll=$newang zoom=-1.5 $ifile $tmp
#	animate pan about tilt=45
#	3Drotate pan=$newang tilt=45 zoom=-1.5 $ifile $tmp
#   tilt=30 animate pan
#
#   set the fixed angle(s) and the animated angle
	3Drotate tilt=30 pan=$newang zoom=-1.5 mandril.jpg $tmp
	if [ $ang -eq $initang ]
		then
		convert $tmp -delay $delay $pfile
	else
		convert -delay $delay $pfile -page $width$xx$height+0+0 $tmp -page $width$xx$height $pfile
	fi
	ang=`expr $ang + $anginc`
done
convert $pfile -loop 0 $pfile
	