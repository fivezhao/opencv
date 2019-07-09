#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"
#include "fboutput/cvvxdisplay.hpp"
using namespace std;
using namespace cv;
int main(int, char**)
{
    cvVxInitDisplay();
    VideoCapture cap(0);
    VideoWriter out_vid;
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    cap.set(CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(CAP_PROP_FPS, 30);

    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    
    out_vid.open("canny.avi", VideoWriter::fourcc('M','J','P','G'),
                     10, Size(frame_width,frame_height),true);
    Mat edges, frame1;
    int i = 0;
    while (i < 300)
    {
        Mat frame;
        cap >> frame1; // get a new frame from camera
        if (frame1.empty())
            break;

        cvtColor(frame1, edges, COLOR_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        cvtColor(edges, edges, COLOR_GRAY2BGRA);
        cvVxShow(edges);
        if (out_vid.isOpened())
            out_vid.write(edges);
            
        i++;
    }
    out_vid.release();
    cap.release();
    return 0;
}
