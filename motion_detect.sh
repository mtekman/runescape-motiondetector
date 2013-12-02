#!/bin/bash

# Window position
window="RuneScape"

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
	    sleep $(interval 1)
	done
}


# Search for movement
function detectMove(){
	rm $screen1 $screen2
	
	import -window root $crop -colors 2 $screen1
	sleep 0.1
	import -window root $crop -colors 2 $screen2
	
	# Abstract OpenCV script to diff photos, dilate them, return largest blob coord
	# If nothing found it returns "NOPE"
	res=$(python dilate_XY.py $screen1 $screen2 10 1)
	echo $res
	echo $res >&2
}



function movement(){
	res="NOPE"
	while [ "$res" = "NOPE" ]; do
		res=$(detectMove)
		sleep 0.5  # check for movement every 1/2 second
	done

	XX=$(echo $res | awk '{print $1}')
	YY=$(echo $res | awk '{print $2}')
	
	xte "mousemove `expr $XX` `expr $YY`" "mouseclick 1"
#	mousepid=$!
	
	charbychar "12435678"

	int=$(interval 2)
	echo -en "\r$int" >&2;
	sleep $int

#	wait $mousepid
}


#### MAIN ####

while [ 0 ]; do
	movement
done


