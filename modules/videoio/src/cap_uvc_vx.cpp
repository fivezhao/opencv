/*
 * Copyright (c) 2017 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

#include "precomp.hpp"

#ifdef __VXWORKS__

#include <stdio.h>
#include <vxWorks.h>
#include <iosLib.h>
#include <errno.h>
#include <errnoLib.h>
#include <selectLib.h>
#include <clockLib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
/* Not technically required, but needed on some UNIX distributions */
#include <sys/types.h>
#include <sys/stat.h>

#include "cap_uvc_vx.hpp"

class CvCaptureCAM_VX : public CvCapture
{
public:
    CvCaptureCAM_VX()
    {
        m_index = -1;
        m_bsize = 0;
        m_vbuf = NULL;
    }

    virtual ~CvCaptureCAM_VX()
    {
        close();
    }

    virtual bool isOpened() const;
    virtual bool open(int cameraId);
    virtual void close();
    virtual double getProperty(int) const;
    virtual bool setProperty(int, double);
    virtual bool grabFrame();
    virtual IplImage* retrieveFrame(int);

protected:
    int m_fd;
    int m_index;
    uint32_t m_frameIntervalUs;
    uint32_t m_bufferSize;
    uint16_t m_width;
    uint16_t m_height;
    uint32_t m_bsize;
    cv::Mat  m_frame;
    IplImage m_iplHeader;
    char *m_vbuf;

    bool grabbedInOpen;
    
    CvSize getSize();
    bool setSize(int, int);
    bool setFrameRate(int rate);
    bool setFormat(uint16_t width, uint16_t height, uint32_t format);
};


bool CvCaptureCAM_VX::isOpened() const {
	return (m_fd != -1);
}

bool CvCaptureCAM_VX::open(int index) {
	char dev[6] = {0};
	uint32_t bsize = 0;
	snprintf(dev, sizeof(dev), "UVC%d", index);
	if ((m_fd = ::open(dev, O_RDONLY, 0)) == -1) {
		return false;
	}

	/* set initial format */
	if (setFormat(1280, 720, VIDEO_FMT_YUY2) == false) {
		::close(m_fd);
		m_fd = -1;
		return false;
	}

#if 0	
	if (setFrameRate(30) == false) {
		::close(m_fd);
		m_fd = -1;
		return false;
	}
#endif

	if (ioctl(m_fd, USB2_VIDEO_IOCTL_GET_BUFFER_SIZE, &bsize) != 0) {
		::close(m_fd);
		m_fd = -1;
		return false;
	}
	

	m_bsize = bsize;
	m_index = index;
	
	return true;
}

void CvCaptureCAM_VX::close()
{
	::close(m_fd);
	m_fd = -1;
	if (m_vbuf) {
		free(m_vbuf);
		m_vbuf = NULL;
	}
}

bool CvCaptureCAM_VX::grabFrame()
{
	bool ret = false;
	int r = -1;
	fd_set rfdset;

	size_t size = 0;

	if (m_fd == -1)
		return false;
	
	if (m_vbuf == NULL)
		return false;
	
	FD_ZERO(&rfdset);
    FD_SET(m_fd, &rfdset);

	r = select(m_fd + 1, &rfdset, NULL, NULL, NULL);
	if (r < 0)
		return false;
	
	if (FD_ISSET(m_fd, &rfdset)) {
		if ((size = ::read(m_fd, m_vbuf, m_bsize)) > 1) {
			cv::Mat f1(m_height, m_width, CV_8UC2, m_vbuf);
			f1.copyTo(m_frame);
		    m_iplHeader = IplImage(m_frame);
		    ret = true;
		}
	}

    return ret;
}

IplImage* CvCaptureCAM_VX::retrieveFrame(int)
{
    return &m_iplHeader;
}

double CvCaptureCAM_VX::getProperty(int property_id) const
{
	switch (property_id) {
	case CV_CAP_PROP_FRAME_WIDTH:
		return m_width;
	case CV_CAP_PROP_FRAME_HEIGHT:
		return m_height;
	case CV_CAP_PROP_FPS:
		return (int)(1E6 / m_frameIntervalUs);
    case CV_CAP_PROP_FOURCC:
        return (double)CV_FOURCC('Y', 'U', 'Y', '2');
	}
	return 0;
}

bool CvCaptureCAM_VX::setFormat(uint16_t width, uint16_t height, uint32_t format) {
	bool ret = false;

	if (m_fd == -1)
		return false;

	VS_FORMAT_USR_DATA *pData = (VS_FORMAT_USR_DATA *)calloc(1, sizeof(VS_FORMAT_USR_DATA));
	if (pData == NULL)
		return false;

    pData->width = width;
    pData->heigth = height;
    pData->fourcc = format;

    if (ioctl(m_fd, USB2_VIDEO_IOCTL_SET_FORMAT, pData) == 0) {
    	ret = true;
    	m_width = pData->width;
    	m_height = pData->heigth;
    }
    free(pData);
    uint32_t bsize = 0;
	if (ret) {
		if (0 != ioctl(m_fd, USB2_VIDEO_IOCTL_GET_BUFFER_SIZE, &bsize)) {
			return false;
		}
		if (m_vbuf == NULL) {
			if ((m_vbuf = (char *)calloc(1, bsize)) == NULL) {
				return false;
			}
		} else {
			if ((realloc(m_vbuf, bsize)) == NULL) {
				return false;
			}
		}
	}
	m_bsize = bsize;

	return true;
}
bool CvCaptureCAM_VX::setFrameRate(int fps) {
	bool ret = false;

	if (fps == 0)
		return false;

	if (m_fd == -1)
		return false;

	uint32_t frameIntervalUs = 1E6 / fps - 1;

    if (ioctl(m_fd, USB2_VIDEO_IOCTL_SET_FRAME_INT, &frameIntervalUs) == 0) {
    	ret = true;
    	m_frameIntervalUs = frameIntervalUs;
    }
    return ret;
}


bool CvCaptureCAM_VX::setProperty(int property_id, double value)
{
    bool retval = false;
    int ival = cvRound(value);

    switch (property_id) {
        case CV_CAP_PROP_FRAME_WIDTH:
        case CV_CAP_PROP_FRAME_HEIGHT:
            {
                int width, height;
                if (property_id == CV_CAP_PROP_FRAME_WIDTH)
                {
                    width = ival;
                    height = width*3/4;
                }
                else {
                    height = ival;
                    width = height*4/3;
                }
                retval = setFormat(width, height, VIDEO_FMT_YUY2);
            }
            break;
        case CV_CAP_PROP_FPS:
            retval = setFrameRate(ival);
            break;
      }

    return retval;
}

CvCapture * cvCreateCameraCapture_VX (int index)
{
    CvCaptureCAM_VX* capture = new CvCaptureCAM_VX;

    if(capture->open(index)) {
        return capture;
    }

    delete capture;
    return 0;
}

#endif /* __VXWORKS__ */
