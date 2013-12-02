#!/usr/bin/env python

import cv2, sys, numpy as np

def usage():
	print >> sys.stderr, '''
Produces X,Y center coordinate of largest white blob produced by the\
 difference of two images and dilating them with a mask

%s <early.jpg> <later.jpg> <mask-size> <dilate iter no>
''' % sys.argv[0].split('/')[-1]
	exit(-1)

if len(sys.argv)!=5:
	usage()

mask=int(sys.argv[3])
dilate=int(sys.argv[4])

early = cv2.imread(sys.argv[1])#, cv2.CV_LOAD_IMAGE_GRAYSCALE)
later = cv2.imread(sys.argv[2])#, cv2.CV_LOAD_IMAGE_GRAYSCALE)

kernel1 = cv2.getStructuringElement(cv2.MORPH_RECT, (mask,mask));
kernel2 = cv2.getStructuringElement(cv2.MORPH_RECT, (mask/2,mask/2));

fgbg = cv2.BackgroundSubtractorMOG()

fgmask = fgbg.apply(later)
fgmask = fgbg.apply(early)

fgmask = cv2.erode(fgmask,kernel2)
fgmask = cv2.dilate(fgmask,kernel1)

def whitePix(image):
	x=y=0
	#Find first white pixel
	for x in xrange(len(image[0])):
		for y in xrange(len(image)):
			
#			print "\r",image[y,x],
			
			if ( image[y,x]==255 and image[y+1,x+1]==255 ):
			#	image[y,x]= 0
				return y,x
	return -1

#ret,thresh = cv2.threshold(fgmask,0,100,0)
#image, contours = cv2.findContours(thresh,cv2.RETR_TREE,cv2.CHAIN_APPROX_SIMPLE)


X_Y = whitePix(fgmask)
if X_Y==-1:
	print "NOPE"
else:
	print X_Y[0], X_Y[1]

#cv2.imshow("Frame",thresh)
# Listen for ESC key
#c = cv2.waitKey(0) % 0x100
#if c == 27:
#	cv2.destroyAllWindows()




