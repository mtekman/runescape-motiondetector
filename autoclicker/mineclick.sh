#center="660 430"
#bottom="650 550"
#top="660 320"

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

function getXY(){
	eval $(xdotool getmouselocation --shell 2>/dev/null)
	echo "$X $Y" >&2
	echo "$X $Y"
}


function promptXY(){
	echo -n "Press c when in pos: " >&2
	read -n 1
	getXY
}

 ########## Make Pos Array ######################
pos_array=()
mine_log=minelog.txt

if [ -f $1 ]; then #i.e is a file 
	echo "RELOADING $1"
	eval $(cat $1)
else
	rm $mine_log 2>/dev/null
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

	xdotool mousemove $x $y;
	echo -en "[$x,$y]"
	xdotool click 1;
}


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
