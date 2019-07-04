#ifndef __OPENCV_UVCVX_H
#define __OPENCV_UVCVX_H


#ifdef __cplusplus
extern "C"
{
#endif

#define FOURCC(a, b, c, d) \
    ((UINT32)(a) | ((UINT32)(b) << 8) | ((UINT32)(c) << 16) | ((UINT32)(d) << 24))

/* uncompressed payload formats */

#define VIDEO_FMT_YUY2              FOURCC('Y', 'U', 'Y', '2')
#define VIDEO_FMT_NV12              FOURCC('N', 'V', '1', '2')
#define VIDEO_FMT_M420              FOURCC('M', '4', '2', '0')
#define VIDEO_FMT_I420              FOURCC('I', '4', '2', '0')

#define VIDEO_FMT_MJPG              FOURCC('M', 'J', 'P', 'G')   /* Motion-JPEG */

#define VIDEO_FMT_Y8I               FOURCC('Y', '8', 'I', ' ')   /* Greyscale 8-bit L/R interleaved */
#define VIDEO_FMT_Y12I              FOURCC('Y', '1', '2', 'I')   /* Greyscale 12-bit L/R interleaved */
#define VIDEO_FMT_Z16               FOURCC('Z', '1', '6', ' ')   /* Depth data 16-bit */
#define VIDEO_FMT_Y8                FOURCC('Y', '8', ' ', ' ')   /* Greyscale 8-bit */
#define VIDEO_FMT_Y10               FOURCC('Y', '1', '0', ' ')   /* Greyscale 10-bit */
#define VIDEO_FMT_Y12               FOURCC('Y', '1', '2', ' ')   /* Greyscale 12-bit */
#define VIDEO_FMT_Y16               FOURCC('Y', '1', '6', ' ')   /* Greyscale 16-bit */
#define VIDEO_FMT_RAW8              FOURCC('R', 'A', 'W', '8')   /* Raw data 8-bit */
#define VIDEO_FMT_RW16              FOURCC('R', 'W', '1', '6')   /* Raw data 16-bit */
#define VIDEO_FMT_INVZ              FOURCC('I', 'N', 'V', 'Z')   /* 16 Depth */
#define VIDEO_FMT_INZI              FOURCC('I', 'N', 'Z', 'I')   /* 24 Depth/IR 16:8 */
#define VIDEO_FMT_INVR              FOURCC('I', 'N', 'V', 'R')   /* 16 Depth */
#define VIDEO_FMT_INRI              FOURCC('I', 'N', 'R', 'I')   /* 24 Depth/IR 16:8 */
#define VIDEO_FMT_INVI              FOURCC('I', 'N', 'V', 'I')   /* 8 IR */
#define VIDEO_FMT_RELI              FOURCC('R', 'E', 'L', 'I')   /* 8 IR alternating on off illumination */


/* IOCTL request codes */
#define USB2_VIDEO_IOCTL_ENUM_FORMATS       0x1A00  /* enumerate the available pixel formats */
#define USB2_VIDEO_IOCTL_ENUM_FRAMES_SIZE   0x1A01  /* enumerate the available frame dimensions */
#define USB2_VIDEO_IOCTL_ENUM_FRAMES_INVL   0x1A02  /* enumerate the available frame interval */

#define USB2_VIDEO_IOCTL_GET_FORMAT         0x1A03  /* get video input device current format */
#define USB2_VIDEO_IOCTL_GET_BUFFER_SIZE    0x1A04  /* get the maximum frame buffer byte size */
#define USB2_VIDEO_IOCTL_GET_TS             0x1A05  /* timestamp of the last frame read */

#define USB2_VIDEO_IOCTL_SET_FORMAT         0x1A06  /* set video stream pixe format and frame dimensions */
#define USB2_VIDEO_IOCTL_SET_FRAME_INT      0x1A07  /* set video frame interval */

#endif
/* IOCTL argument structures */
/* USB2_VIDEO_IOCTL_ENUM_FORMATS */

typedef struct vs_format_usr_desc
    {
    UINT8           formatIndex;        /* in */
    UINT8           subtype;            /* frame descriptor sub type */
    UINT32          fourcc;             /* fourcc format identifier - out */
    }__attribute__((packed)) VS_FORMAT_USR_DESC;


/* USB2_VIDEO_IOCTL_SET_FORMAT */
typedef struct vs_format_usr_data
    {
    UINT32          fourcc;             /* in */
    UINT16          width;              /* in/out */
    UINT16          height;             /* in/out */
    }__attribute__((packed)) VS_FORMAT_USR_DATA;

#ifdef __cplusplus

#ifdef __cplusplus
}
#endif


#endif /* __OPENCV_UVCVX_H */
