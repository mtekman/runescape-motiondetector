#!/bin/bash

input_imgs=$*;


binary=../mineDetector-build/mineDetector

sort_key_ind=$( echo $1 | egrep -o "[0-9]" -b | awk -F":" '{print $2"."($1+1)}')
ext=$(echo $1 | awk -F"." '{print $NF}')

imgs=( `echo "$input_imgs" | sed "s/\.jpg//g" | sed "s/\s/\n/g" | sort -n -k $sort_key_ind` )




movement_total=0
movement_count=0

idle_total=0
idel_count=0

for index in `seq 0 $(( $# - 1 ))`; do
	this=${imgs[$index]}.$ext;
	next=${imgs[$(( $index + 1))]}.$ext;

	output=$( $binary $this $next --debug 2>&1 | head -1)
	pix=$(echo $output | awk '{print $NF}')

	echo "$this -> $next, movement?[y/n]" >&2

	read -n 1 ans;
	if [ "$ans" = "y" ]; then
		movement_count=$(( $movement_count + 1 ))
		movement_total=$(( $movement_total + $pix ))
	else
		idle_count=$(( $idle_count + 1 ))
		idle_total=$(( $idle_total + $pix ))
	fi
done

echo "Movement ["$movement_count" imgs] = $movement_total, average="$(echo "$movement_total / $movement_count" | bc);
echo "Idle ["$idle_count" imgs] = $idle_total, average="$(echo "$idle_total / $idle_count" | bc);

