#ifdef __VXWORKS__
#include "precomp.hpp"

#include <vxWorks.h>
#include <sys/select.h>
#include <evdevLib.h>

/* place-holders */

void cv::setWindowTitle(const String&, const String&)
{
    CV_Error(Error::StsNotImplemented, "The function is not implemented. "
        "Rebuild the library with Windows, GTK+ 2.x or Carbon support. "
        "If you are on Ubuntu or Debian, install libgtk2.0-dev and pkg-config, then re-run cmake or configure script");
}

#define CV_NO_GUI_ERROR(funcname) \
    cvError( CV_StsError, funcname, \
    "The function is not implemented. " \
    "Rebuild the library with Windows, GTK+ 2.x or Carbon support. "\
    "If you are on Ubuntu or Debian, install libgtk2.0-dev and pkg-config, then re-run cmake or configure script", \
    __FILE__, __LINE__ )


CV_IMPL int cvNamedWindow( const char*, int )
{
    CV_NO_GUI_ERROR("cvNamedWindow");
    return -1;
}

CV_IMPL void cvDestroyWindow( const char* )
{
    CV_NO_GUI_ERROR( "cvDestroyWindow" );
}

CV_IMPL void
cvDestroyAllWindows( void )
{
    CV_NO_GUI_ERROR( "cvDestroyAllWindows" );
}

CV_IMPL void
cvShowImage( const char*, const CvArr* )
{
    CV_NO_GUI_ERROR( "cvShowImage" );
}

CV_IMPL void cvResizeWindow( const char*, int, int )
{
    CV_NO_GUI_ERROR( "cvResizeWindow" );
}

CV_IMPL void cvMoveWindow( const char*, int, int )
{
    CV_NO_GUI_ERROR( "cvMoveWindow" );
}

CV_IMPL int
cvCreateTrackbar( const char*, const char*,
                  int*, int, CvTrackbarCallback )
{
    CV_NO_GUI_ERROR( "cvCreateTrackbar" );
    return -1;
}

CV_IMPL int
cvCreateTrackbar2( const char* /*trackbar_name*/, const char* /*window_name*/,
                   int* /*val*/, int /*count*/, CvTrackbarCallback2 /*on_notify2*/,
                   void* /*userdata*/ )
{
    CV_NO_GUI_ERROR( "cvCreateTrackbar2" );
    return -1;
}

CV_IMPL void
cvSetMouseCallback( const char*, CvMouseCallback, void* )
{
    CV_NO_GUI_ERROR( "cvSetMouseCallback" );
}

CV_IMPL int cvGetTrackbarPos( const char*, const char* )
{
    CV_NO_GUI_ERROR( "cvGetTrackbarPos" );
    return -1;
}

CV_IMPL void cvSetTrackbarPos( const char*, const char*, int )
{
    CV_NO_GUI_ERROR( "cvSetTrackbarPos" );
}

CV_IMPL void cvSetTrackbarMax(const char*, const char*, int)
{
    CV_NO_GUI_ERROR( "cvSetTrackbarMax" );
}

CV_IMPL void cvSetTrackbarMin(const char*, const char*, int)
{
    CV_NO_GUI_ERROR( "cvSetTrackbarMin" );
}

CV_IMPL void* cvGetWindowHandle( const char* )
{
    CV_NO_GUI_ERROR( "cvGetWindowHandle" );
    return 0;
}

CV_IMPL const char* cvGetWindowName( void* )
{
    CV_NO_GUI_ERROR( "cvGetWindowName" );
    return 0;
}

CV_IMPL int cvWaitKey(int delay) {
	int evdevFd = ERROR;
	int key = -1;
	int res = -1;
	unsigned int msgCount = 0;
	EV_DEV_MSG evdevMsg;

	evdevFd = open(EV_DEV_NAME, 0, 0);
	if (ERROR == evdevFd)
		return ERROR;

	/* clear messages */
	if (ERROR == ioctl(evdevFd, FIONREAD, (char *) &msgCount)) {
		msgCount = 0;
	}

	while (msgCount >= sizeof(EV_DEV_MSG)) {
		if (sizeof(EV_DEV_MSG)
				!= read(evdevFd, (char *) &evdevMsg, sizeof(EV_DEV_MSG))) {
			break;
		}

		if (ERROR == ioctl(evdevFd, FIONREAD, (char *) &msgCount)) {
			break;
		}
	}
	
	fd_set readfds;
	FD_ZERO(&readfds);
	struct timeval wt = {0};
	struct timeval *pwt = NULL;
	int readBytes = 0;
	int n = 0;
	if (delay > 0) {
		wt.tv_sec = delay / 1000;
		wt.tv_usec = (delay % 1000) * 1000;
		pwt = &wt;
	}
	FD_SET(evdevFd, &readfds);
	res = select(evdevFd + 1, &readfds, NULL, NULL, pwt);

	if (res > 0) {
		if (FD_ISSET(evdevFd, &readfds)) {
			if (ioctl(evdevFd, FIONREAD, (char *) &msgCount) == ERROR) {
				goto __FUNCTION__end;
			}
#ifdef WAIT_KEY_DBG
			printf("%s:%d - got %d messages\n", __FUNCTION__, __LINE__, msgCount);
#endif
			while (msgCount > 0) {
				readBytes = (int) read(evdevFd, (char *) &evdevMsg,
						sizeof(EV_DEV_MSG));
				if (readBytes == sizeof(EV_DEV_MSG)) {

					switch (evdevMsg.msgType) {
					case EV_DEV_MSG_KBD:
						if (evdevMsg.msgData.kbdData.value >= ' '
								&& evdevMsg.msgData.kbdData.value < 127) {
							if (evdevMsg.msgData.kbdData.state == 1) {
#ifdef WAIT_KEY_DBG
								printf("%s:%d - got key %d\n", __FUNCTION__, __LINE__, evdevMsg.msgData.kbdData.value);
#endif
								key = evdevMsg.msgData.kbdData.value;
								/* cleanup */
								//goto __FUNCTION__end;
								if (ERROR == ioctl(evdevFd, FIONREAD, (char *) &n)) {
									goto __FUNCTION__end;
								}
								while (n >= sizeof(EV_DEV_MSG)) {
									if (sizeof(EV_DEV_MSG)
											!= read(evdevFd, (char *) &evdevMsg, sizeof(EV_DEV_MSG))) {
										msgCount = 0;
										break;
									}

									if (ERROR == ioctl(evdevFd, FIONREAD, (char *) &n)) {
										goto __FUNCTION__end;
									}
								}
								msgCount = 0;
							}
						}
					}
				}
				if (ioctl(evdevFd, FIONREAD, (char *) &msgCount) == ERROR) {
					break;
				}
			}
		}
	}

__FUNCTION__end:
#ifdef WAIT_KEY_DBG
	printf("%s:%d - END - key = %d\n", __FUNCTION__, __LINE__, key);
#endif
	close(evdevFd);
	return key;
}

CV_IMPL int cvInitSystem( int , char** )
{

    CV_NO_GUI_ERROR( "cvInitSystem" );
    return -1;
}

CV_IMPL int cvStartWindowThread()
{

    CV_NO_GUI_ERROR( "cvStartWindowThread" );
    return -1;
}

CV_IMPL void cvAddText( const CvArr*, const char*, CvPoint , CvFont* )
{
    CV_NO_GUI_ERROR("cvAddText");
}

CV_IMPL void cvDisplayStatusBar(const char* , const char* , int )
{
    CV_NO_GUI_ERROR("cvDisplayStatusBar");
}

CV_IMPL void cvDisplayOverlay(const char* , const char* , int )
{
    CV_NO_GUI_ERROR("cvNamedWindow");
}

CV_IMPL int cvStartLoop(int (*)(int argc, char *argv[]), int , char* argv[])
{
    (void)argv;
    CV_NO_GUI_ERROR("cvStartLoop");
    return -1;
}

CV_IMPL void cvStopLoop()
{
    CV_NO_GUI_ERROR("cvStopLoop");
}

CV_IMPL void cvSaveWindowParameters(const char* )
{
    CV_NO_GUI_ERROR("cvSaveWindowParameters");
}

// CV_IMPL void cvLoadWindowParameterss(const char* name)
// {
//     CV_NO_GUI_ERROR("cvLoadWindowParameters");
// }

CV_IMPL int cvCreateButton(const char*, void (*)(int, void*), void*, int, int)
{
    CV_NO_GUI_ERROR("cvCreateButton");
    return -1;
}

#endif /* __VXWORKS__ */
