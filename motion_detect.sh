#!/bin/bash

# Window position
window="RuneScape"
skeletonDect="./skeletonDetect/skeletonDetect"

xpos=`xwininfo -name "$window" | grep Absol | awk '{ print $4 }' | head -1`
ypos=`xwininfo -name "$window" | grep Absol | awk '{ print $4 }' | tail -1`
width=`xwininfo -name "$window" | grep Width | awk '{ print $NF }' | head -1`
height=`xwininfo -name "$window" | grep Height | awk '{ print $NF }' | head -1`

topleft=`xwininfo -name "$window" | grep Corners | awk '{ print $2 }'`


# Image location
screen1="earrly.jpg"
screen2="late.jpg"


#Crop command
crop="-crop $width"x"$height$topleft"
#Color Opts
coloropts="-colors 100"


# Sleep random interval
function interval {
	echo "scale=2;$1*$RANDOM / 32767" | bc
}



# Enter data key by key
function charbychar {
	phrase=$(echo $1 | sed -e 's/\//\\/g')

	x=1
	while [ $x -le ${#phrase} ]
	do
	    chr=$( echo $phrase | awk -F "" {'print $'$x''} )
	    x=$(( $x + 1 ))
	    xdotool key $chr
	    sleep $(interval 0.1)
	done
}


# Search for movement
function detectMove(){
	dirs=imgs/`date +%H_%M_%S`
	mkdir -p $dirs
	mv $screen1 $dirs/
	mv $screen2 $dirs/
	
	import -window root $crop $coloropts $screen1
	sleep 0.1
	import -window root $crop $coloropts $screen2
	
	# OpenCV script to diff photos, return largest blob coord
	# If nothing found it returns "0 0"
	res_XY=$($skeletonDect $screen1 $screen2)
	echo $res_XY
	echo "[x y] = $res_XY" >&2
}



function movement(){
	res="0 0"
	while [ "$res" = "0 0" ]; do
		res=$(detectMove)
		sleep 0.5  # check for movement every 1/2 second
	done

	XX=$(echo $res | awk '{print $1}')
	YY=$(echo $res | awk '{print $2}')
	
	xte "mousemove `expr $xpos + $XX` `expr $ypos + $YY`" "mouseclick 1" &
	mousepid=$!
	
	charbychar "111112111111111111111111"

	int=$(interval 2)
	echo -en "\r$int" >&2;
	sleep $int

	wait $mousepid
}


#### MAIN ####

while [ 0 ]; do
	movement
done


