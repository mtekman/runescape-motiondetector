#!/bin/bash

##--Method for manually inputting backslash characters using xte in GRR---#
##-- Fixes GRR bug on Pythagoras  -- mct 2012/10/19

function interval {
	echo "scale=2;$1*$RANDOM / 32767" | bc
}

function charbychar {
	phrase=$(echo $1 | sed -e 's/\//\\/g')

	x=1
	while [ $x -le ${#phrase} ]
	do
	    chr=$( echo $phrase | awk -F "" {'print $'$x''} )
	    x=$(( $x + 1 ))
	    xte "str $chr"
	    sleep $(interval 0.5)
	done
}

#window="updater_rs.sh - /home/memo/Desktop/personal/upgrader_rs - Geany"
window="RuneScape"

# grr position
xpos=`xwininfo -name "$window" | grep Absol | awk '{ print $4 }' | head -1`
ypos=`xwininfo -name "$window" | grep Absol | awk '{ print $4 }' | tail -1`
width=`xwininfo -name "$window" | grep Width | awk '{ print $NF }' | head -1`
height=`xwininfo -name "$window" | grep Height | awk '{ print $NF }' | head -1`

center_x=$(( ($width / 2) + $xpos ))
center_y=$(( ($height / 2) + $ypos ))

echo "[$xpos $ypos] [$width $height] [$center_x $center_y]"

function random_gen {
	echo $(( (($RANDOM - 16383) /( $1 / 10))  ))
}
function random_x {
	echo $(random_gen $width)
}

function random_y {
	echo $(random_gen $height)
}

#exit

while [ 0 ];do
#	echo "RANDX:`random_x`, RAND_Y: `random_y`"
#	sleep 1
	xte "mousemove `expr $center_x + $(random_x)` `expr $center_y + $(random_y)`" "mouseclick 1" &
	mousepid=$!
	
	charbychar "1 2 3 4 5"
	int=$(interval 4)
	echo $int
	sleep $int

	wait $mousepid
done
