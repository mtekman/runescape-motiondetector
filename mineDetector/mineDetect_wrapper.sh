#!/bin/bash

mine_bin="./mineDetector"
window="RuneScape"

# position
data=`xwininfo -name "$window"`

xpos=`echo "$data"   | grep Absol | awk '{ print $4 }' | head -1`
ypos=`echo "$data"   | grep Absol | awk '{ print $4 }' | tail -1`
width=`echo "$data"  | grep Width | awk '{ print $NF }' | head -1`
height=`echo "$data" | grep Height | awk '{ print $NF }' | head -1`

$mine_bin $xpos $ypos $width $height --xdo --nearest
