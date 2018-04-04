#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int tl_x,tl_y,br_x,br_y,flagger=0;

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
     if  ( event == EVENT_LBUTTONDOWN )
     {
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
        if(flagger==0) {
            tl_x=x;
            tl_y=y;
            flagger = 1;
        }
        else {
            br_x=x;
            br_y=y;
            flagger = 0;
        }
     }
     
}

int main()
{
	VideoCapture cap("vid.mp4"); // open the default camera
    Mat img;

	int i=0;
    for (int q = 0; q < 200; q++) {
            cap.read(img);
    }
	while(1) {
            if (i % 5 != 0) {

                    cap.read(img);

                    i++;
                    continue;
            }
            cout << "i: " << i << endl;
            cap.read(img);

            namedWindow("original", WINDOW_NORMAL);
            imshow("original", img);

            flagger=0;

            cout<<"Click on the corners "<<endl;
            setMouseCallback("original", CallBackFunc, NULL);
            waitKey(0);

            Mat roi;
            roi = img(Rect(tl_x,tl_y, abs(br_x-tl_x), abs(br_y-tl_y)));
            imwrite(to_string(i/5) + ".png", roi);

            imshow("data point", roi);
            waitKey(30);
            i++;
	}
}
