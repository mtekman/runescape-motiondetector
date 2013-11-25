#!/bin/bash

##--Method for manually inputting backslash characters using xte in GRR---#
##-- Fixes GRR bug on Pythagoras  -- mct 2012/10/19
function charbychar {
	phrase=$(echo $1 | sed -e 's/\//\\/g')

	x=1
	while [ $x -le ${#phrase} ]
	do
	    chr=$( echo $phrase | awk -F "" {'print $'$x''} )
	    x=$(( $x + 1 ))
	    xte "str $chr"
	done
}

#wine $HOME/.wine/drive_c/Program\ Files/Genetics/GRR/GRR.exe &>/dev/null &
wine $HOME/.wine/drive_c/Program\ Files\ \(x86\)/Genetics/GRR/GRR.exe &> /dev/null &
grrpid=$!

sleep 5

# grr position
xpos=`xwininfo -name 'RuneScape' | grep Absol | awk '{ print $4 }' | head -1`
ypos=`xwininfo -name 'RuneScape' | grep Absol | awk '{ print $4 }' | tail -1`

# tick other relatives
xte "mousemove `expr $xpos + 542` `expr $ypos + 193`" "mouseclick 1" "sleep 1"

# change the title
xte "mousemove `expr $xpos + 300` `expr $ypos + 50`"  "mouseclick 1" "sleep 1" "str $project" \
    "sleep 1" "mousemove `expr $xpos + 300` `expr $ypos + 205`" "mouseclick 1" "sleep 1"

# load pedin
xte "mousemove `expr $xpos + 600` `expr $ypos + 235`" "mouseclick 1" "sleep 1" 
charbychar $pedin ###HOTFIX: Enter pedin char at a time
xte    "sleep 1" "keydown Alt_L" "key O" "keyup Alt_L" "sleep 1"

sleep 2

# load dialog
dxpos=`xwininfo -name 'Load Pedigree File' | grep Absol | awk '{ print $4 }' | head -1`
dypos=`xwininfo -name 'Load Pedigree File' | grep Absol | awk '{ print $4 }' | tail -1`
xte "mousemove `expr $dxpos + 23`  `expr $dypos + 30`"  "mouseclick 1" "sleep 1" \
    "mousemove `expr $dxpos + 100` `expr $dypos + 210`" "mouseclick 1" "sleep 1"
sleep 10

# drag-select all points
xte "mousemove `expr $xpos + 75`  `expr $ypos + 85`"  "mousedown 1" "sleep 1" \
    "mousemove `expr $xpos + 480` `expr $ypos + 330`" "mouseup 1" "sleep 1"

# IBS Details dialog
dxpos=`xwininfo -name 'IBS Details' | grep Absol | awk '{ print $4 }' | head -1`
dypos=`xwininfo -name 'IBS Details' | grep Absol | awk '{ print $4 }' | tail -1`
xte "mousemove `expr $dxpos + 400`  `expr $dypos + 195`"  "mouseclick 1" "sleep 1"
charbychar $pedtxt ###HOTFIX: Enter pedtxt char at a time
xte "sleep 1" "keydown Alt_L" "key S" "keyup Alt_L" "sleep 1" \
    "mousemove `expr $dxpos + 500`  `expr $dypos + 195`" "mouseclick 1" "sleep 1"

# we want to fix the axis so it is easier to interpret
# ajm 08/04/11

# click options
xte "mousemove `expr $xpos + 600` `expr $ypos + 300`" "mouseclick 1" "sleep 1"

# find options dialog
oxpos=`xwininfo -name 'Options' | grep Absol | awk '{ print $4 }' | head -1`
oypos=`xwininfo -name 'Options' | grep Absol | awk '{ print $4 }' | tail -1`

# click fix axis
xte "mousemove `expr $oxpos + 20` `expr $oypos + 125`" "mouseclick 1" "sleep 1"

# click shade possible region
xte "mousemove `expr $oxpos + 20` `expr $oypos + 150`" "mouseclick 1" "sleep 1"

# click ok
xte "mousemove `expr $oxpos + 160` `expr $oypos + 260`" "mouseclick 1" "sleep 1"


# screenshot
import -window `xwininfo -name 'Allele Sharing Quality Control' | grep "xwininfo" | awk '{ print $4 }'` $pedpng
sleep 1

# quit grr
xte "mousemove `expr $xpos + 600` `expr $ypos + 360`" "mouseclick 1"

#kill $grrpid

