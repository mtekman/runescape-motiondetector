### This repo has migrated to GitLab
https://gitlab.com/mtekman/runescape-motiondetector
###### (The copy here is archived. Please see the above link for the latest developments)

********



Script to automate runescape levelling using xte framework


## Auto-Clickers:

#### updater_rs.sh --> 

		clicks at random points around centre of window at random intervals

#### motion_detect.sh --> 

		screenshots two frames at 1/2 intervals, feeds them into a 
		motion detector, clicks at resultant position


## Motion-Detectors:

#### dilate_XY.py -->

		Python stab at openCV, differences two images, erodes, dilates,
		returns position of first white pixel. Slow.
	
#### skeletonDetect -->

		C++ stab at openCV, background subtracts two images, erodes, dilates,
		returns centre position of largest white blob. Fast. Preferred method.
