#include "opencv2/highgui.hpp"

#include <iostream>
#include <stdio.h>
#include <vxWorks.h>
#include <iosLib.h>

extern "C"
{
#include "fboutput.h"
}

#include "cvvxdisplay.hpp"

struct displayInfoStruct* displayHandle = NULL;

int cvVxInitDisplay() {
    if (displayHandle == NULL)
		displayHandle = displayOpen("/dev/fb2");
    if (displayHandle == NULL) {
    	printf("Unable to open display /dev/fb2.\n", 0, 0, 0, 0, 0, 0);
      	return -1;
	}
    return 0;
}

int cvVxShow(Mat &frame) {
	if (frame.data){
		if (displayHandle->drawHeight != frame.rows ||
		    displayHandle->drawWidth != frame.cols ) {
			displayCenterImage(displayHandle, frame.cols, frame.rows);
     	}
    	displayDrawImg(displayHandle, frame.data, frame.cols * 4);
        return 0;
   } else {
   		printf("frame is empty\n");
   		return -1;
   }
}
