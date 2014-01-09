#!/bin/bash

count=0

if [ $# -lt 3 ] || [ $# -gt 4 ]; then
	echo -e "
[Randomly] Cycles between a number of different of mouseclicks at roughly min to max second intervals

usage: 
   `basename $0` <num clicks> <min-t> <max-t> [-r]
          or
   `basename $0` <array_file> <min-t> <max-t> [-r]

where:
 array_file   load click positions of a previous run
         -r   random pick avail positions, not cycle

" >&2
	exit -1
fi

numclk=$1
min=$2
max=$3
randy=$4

##### First determine window position

winname="RuneScape"
window=$(xwininfo -name "$winname")
[ "$window" = "" ] && exit -1

winid=$(echo "$window" | grep " id" | awk '{print $4}')
ulX=$(echo "$window" | grep "Absolute upper-left X" | awk -F":" '{print $2}')
ulY=$(echo "$window" | grep "Absolute upper-left Y" | awk -F":" '{print $2}')

function getRelativeXY(){
	eval $(xdotool getmouselocation --shell 2>/dev/null)
	out="$(($X - $ulX)) $(($Y - $ulY))"
	echo $out>&2
	echo $out
}


function promptXY(){
	echo -n "Press c when in pos: " >&2
	read -n 1
	getRelativeXY
}

 ########## Make Pos Array ######################
pos_array=()
mine_log=minelog.txt


if [ -f $1 ]; then #i.e is a file 
	echo "RELOADING $1"
	eval $(cat $1)
else
	#Store width, height vars
	width=$(echo "$window" | grep "Width" | awk '{print $2}')
	height=$(echo "$window" | grep "Height" | awk '{print $2}')

	echo "width=$width" > $mine_log
	echo "height=$height" >> $mine_log

	while [ $count -lt $numclk ]; do
		res=$(promptXY)
		pos_array[$count]="$res"

		### Record to mine log for -r ###
		echo "pos_array[$count]=\"$res\"" >> $mine_log

		count=$(( $count + 1 ))

	done
fi

#################################################


function sleeprand(){
	min=$1
	max=$2
	
	modd=$(( $RANDOM % $max ))
	if [ $modd -lt $min ]; then
		modd=$(( $modd + $min ))
	fi
	echo -n "w:$modd    "
	sleep $modd
}

function clicker(){
	mod=$RANDOM
	mod=$(echo ${mod:0:1})

        x=$(( $1 - $mod ))
        y=$(( $2 - $mod ))

	echo -en "[$x,$y]"
	xdotool mousemove --window $winid $x $y click 1;
}

# Change window size
xdotool windowsize $winid $width $height

countdown=3
until [ $countdown = 0 ];do
	echo -n "$countdown... ">&2
	sleep 1
	countdown=$(( $countdown - 1 ))
done
echo "GO!">&2
sleep 1

arr_size=${#pos_array[@]}

while [ 0 ]; do
	count=0;
#	lastind=1000;
	
	while [ $count -lt $arr_size ]; do

		ind=$count

		if [ "$randy" = "-r" ];then
			ind=$(( $RANDOM % $arr_size ))

			# No repeats
			while [ "$ind" = "$lastind" ]; do
				ind=$(( $RANDOM % $arr_size ))
			done
		fi

		echo -en "$ind:"
		clicker ${pos_array[$ind]}
		sleeprand $min $max

		lastind=$ind
		count=$(( $count + 1 ))
	done
	echo -en "\r"
done
