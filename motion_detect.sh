#!/bin/bash

# Window position
window="RuneScape"
skeletonDect="./skeletonDetect-build/skeletonDetect"

[ "$(xwininfo -name $window)" = "" ] && exit


xpos=`xwininfo -name "$window" | grep Absol | awk '{ print $4 }' | head -1`
ypos=`xwininfo -name "$window" | grep Absol | awk '{ print $4 }' | tail -1`
width=`xwininfo -name "$window" | grep Width | awk '{ print $NF }' | head -1`
height=`xwininfo -name "$window" | grep Height | awk '{ print $NF }' | head -1`


$skeletonDect $xpos $ypos $width $height --xdo
