#include <iostream>
#include<bits/stdc++.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <string>

#define SUBSTRACTION_CONSTANT 30
#define INTENSITY_TH 50
#define PI 3.14159265
#include "svm.h"

struct svm_model* model;
struct svm_node *x_space;
struct svm_problem prob;
struct svm_parameter param;



#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

using namespace std;
using namespace cv;
int j=0;


class Lanes
{
	Mat img,img_gray,bisect;
	vector<Vec2i> L,R;
public:
	Lanes(VideoCapture cap);
	void Mix_Channel();
	void Intensity_distribution();
	void display();
	void Intensity_adjust();
	void Brightest_Pixel();
	void Edge();
	void Hough();
	void Dilation();
	void control_points();
	void control_vanishing();
};

int main(int argc, char** argv)
{
	if(argc < 2) return 0;
	    VideoCapture cap(argv[1]); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
    model = svm_load_model("data.txt.model");
while(1)
{
	//Mat given=imread(argv[1],1);
	Lanes L(cap);     // L --> frame
	//L.Intensity_distribution();

	L.Intensity_adjust();
	L.Mix_Channel();
	// L.display();
	//L.Intensity_distribution();

	// L.Brightest_Pixel();
	// L.control_points();
	// L.Hough();
	// L.Intensity_distribution();
	// L.Mix_Channel();
	// L.display();
	L.Edge();
	// L.Hough();
	// L.display();
	L.Brightest_Pixel();
	L.control_points();
	// L.control_vanishing();
	waitKey(1);
}
	waitKey(0);
	return 0;

}

Lanes::Lanes(VideoCapture cap)
{
 cap >> this->img; 
	//img=given;
//cvtColor(given,img_gray,CV_BGR2GRAY);
	cvtColor(this->img, this->img_gray,CV_BGR2GRAY);
}

void Lanes::Mix_Channel()
{
	blur(img,img,Size(5,5));
	for(int i = 0; i < img.rows; i++)   // grass removal
		for(int j = 0; j < img.cols; j++)
		{
			int temp = 2 * img.at<Vec3b>(i,j)[0] - img.at<Vec3b>(i,j)[1]; 
			if(temp < 0) temp=0;
			if(temp > 255) temp=255;
			img_gray.at<uchar>(i,j) = temp;
		}
	imshow("Mix_Chann",img_gray);
	//waitKey(0);
}

void Lanes::display()
{
	imshow("input", img);
	imshow("Mix_Channel", img_gray);
	waitKey(0);
}

void Lanes::Intensity_distribution()
{
	Mat dist(256,img.rows,CV_8UC1,Scalar(0));
	for(int i = 0; i < img.rows; i++)
	{
		int sum = 0;
		for(int j = 0; j < img.cols; j++)
		{
			sum += img.at<Vec3b>(i,j)[0] + img.at<Vec3b>(i,j)[1] + img.at<Vec3b>(i,j)[2];
		}
		sum /= 3;
		sum /= img.cols;
		dist.at<uchar>(256-sum,i) = 255;
	}
	imshow("Intensity_distribution",dist);
	//waitKey(0);
}

void Lanes::Intensity_adjust()  // the top part of frame may have some intensity variation
{
	Mat filter(img.rows,img.cols,CV_8UC1,Scalar(0));
	for(int i = 0; i <= img.rows/4; i++)
		for(int j = 0; j < img.cols; j++)
			filter.at<uchar>(i,j) = (img.rows/4 - i) * (SUBSTRACTION_CONSTANT) / (img.rows/4);
	// imshow("filter",filter);
	// waitKey(0);
	for(int i = 0; i < img.rows; i++)
		for(int j = 0; j < img.cols; j++)
			for(int k = 0; k < 3; k++)
			{
				int temp = img.at<Vec3b>(i,j)[k] - filter.at<uchar>(i,j);
				if(temp < 0) temp = 0;
				img.at<Vec3b>(i,j)[k] = temp;
			}
	imshow("Filtered Image", img);
	//waitKey(0);
}

void Lanes::Brightest_Pixel()
{
	bisect = img_gray.clone();
	for(int i = 0; i < img.rows; i++)  
	{  // finding the brightest pixel in each row
		int l,r,max = 0;
		for(int j = 0; j < img.cols/2; j++)
		{
			bisect.at<uchar>(i,j) = 0;
			// if(img_gray.at<uchar>(i,j) < INTENSITY_TH) img_gray.at<uchar>(i,j) = 0;
			// else img_gray.at<uchar>(i,j) = 255;
			int sum = 0, count = 0;
			for(int m = i-1; m < i+2; m++)
				for(int n = j-1; n < j+2; n++)
				{
					if(m < 0 || n < 0 || m >= img.rows || n >= img.cols) continue;
					sum += img_gray.at<uchar>(m,n); count++;
				}
			if(count != 0) sum /= count;
			if(sum > max)
			{
				max = sum;
				l = j;
			}
			
		}
		if(max>100) bisect.at<uchar>(i,l) = 255;
		max = 0;
		for(int j = img.cols/2 + 1; j < img.cols; j++)
		{
			bisect.at<uchar>(i,j) = 0;
			// if(img_gray.at<uchar>(i,j) < INTENSITY_TH) img_gray.at<uchar>(i,j) = 0;
			// else img_gray.at<uchar>(i,j) = 255;
			int sum = 0, count = 0;
			for(int m = i-1; m < i+2; m++)
				for(int n = j-1; n < j+2; n++)
				{
					if(m < 0 || n < 0 || m >= img.rows || n >= img.cols) continue;
					sum += img_gray.at<uchar>(m,n); count++;
				}
			if(count != 0) sum /= count;
			if(sum > max)
			{
				max = sum;
				r = j;
			}
			
		}
		if(max > 100) bisect.at<uchar>(i,r) = 255;
	}
	// Dilation();
	imshow("bisect",bisect);
	// imshow("Threshold",img_gray);
	//waitKey(0);
}

void Lanes::Edge()
{
	Mat Gx, Gy, Ga(img.rows,img.cols,CV_8UC1,Scalar(0)), Gb(img.rows,img.cols,CV_8UC1,Scalar(0)), Gc(img.rows,img.cols,CV_8UC1,Scalar(0)), Gd(img.rows,img.cols,CV_8UC1,Scalar(0));
	equalizeHist(img_gray, img_gray);     // adjusting the contrast
	Mat temp = img_gray.clone();
	for(int i = 0; i < img.rows; i++)  // pixel
	{
		for(int j = 0; j < img.cols; j++)
		{
			int count=0;
			for(int m = i-1; m < i+2; m++)  // a kernel
			{
				for(int n = j-1; n < j+2; n++)
				{
					if(m<0 || n<0 || m>=img.rows || n>=img.cols) continue;   // isvalid condition
					if(img_gray.at<uchar>(m,n) < INTENSITY_TH) count++;   // erosion condition
				}
			}
			if(count != 0) temp.at<uchar>(i,j)=0;  
		}
	}
	img_gray = temp;
	vector<vector<Point> > contours;
	vector<Vec4i> heirarchy;
	vector<Rect> roi;
	vector<Mat> roi_img;
	Mat t=img_gray.clone();
	Mat t1 = img_gray.clone();
	
	cvtColor(t1,t,CV_GRAY2BGR);
	threshold(t1,t1,0,255,THRESH_BINARY+THRESH_OTSU);
	Mat element = getStructuringElement( MORPH_RECT,Size( 2*1 + 1, 2*1+1 ),Point( 1, 1 ) ); 
    int i=5;
    while(i--)
       erode(t1,t1,element);
   	i = 1;
   	// while(i--)
    // dilate(t1,t1,element);
	imshow("thesh",img_gray);
	//Canny(img_gray,img_gray,50,100,3,0);
	findContours(t1,contours,heirarchy,CV_RETR_TREE,CV_CHAIN_APPROX_NONE);
	float resize_factor=1.5;
	for(int i=0;i<contours.size();i++)
	{
		double area=contourArea(contours[i]);
		if(area<100 || area > (0.1*t1.rows*t1.cols)) continue;
		drawContours(t,contours,i,Scalar(255,0,0),2,8,heirarchy,0,Point());
		Rect rect=boundingRect(Mat(contours[i]));
		int width=rect.br().x-rect.tl().x;
		int height=rect.br().y-rect.tl().y;
		Rect roi_latest;
		Point tl=rect.tl(),br=rect.br();
		if((width*1.0)/height>1.5f) {
			tl=rect.tl()-Point(width*0.3*resize_factor,height*1*resize_factor);
			br=rect.br()+Point(width*0.5*resize_factor,height*0.5*resize_factor);
			if(!(tl.x>=0))
				tl.x=0;
			if(!(tl.y>=0))
				tl.y=0;
			if(!(br.x<img.cols))
				br.x=img.cols-1;
			if(!(br.y<img.rows))
				br.y=img.rows-1;
			}
		else {
			// tl=rect.tl()-Point(width*0.3*resize_factor,height*1*resize_factor);
			// br=rect.br()+Point(width*0.5*resize_factor,height*0.5*resize_factor);
			tl=rect.tl();
			br=rect.br();
		}
		rectangle(img,tl,br,Scalar(0,255,0),2,8,0);
		roi_latest=Rect(tl.x,tl.y,br.x-tl.x,br.y-tl.y);
		
		roi.push_back(roi_latest);
		roi_img.push_back(img(roi_latest));

		// imwrite("data/"+to_string(j)+".png",img(roi_latest));
		j++;
	}

	//roi_img now contains all the images in it which are to be predicted for lebelling
	for(int i = 0;i<roi_img.size();i++) {
		Rect rect = roi[i];
		cout << "Check" << endl;
		Point tl = rect.tl(),br = rect.br();
		rectangle(img,tl,br,Scalar(0,0,0),2,8,0);
	//SVM CODE
	vector< vector < float> > v_descriptorsValues;
 	vector< vector < Point> > v_locations;

 	resize(roi_img[i], roi_img[i], Size(64,128) );

 	HOGDescriptor d( Size(64,128), Size(16,16), Size(8,8), Size(8,8), 9);
  	vector< float> descriptorsValues;
  	vector< Point> locations;
  	d.compute( roi_img[i], descriptorsValues, Size(0,0), Size(0,0), locations);

  	double predict_label;

    //x = (struct svm_node *) malloc(sizeof(struct svm_node));
   	prob.l = 1;
    prob.y = Malloc(double,prob.l); //space for prob.l doubles
	prob.x = Malloc(struct svm_node *, prob.l); //space for prob.l pointers to struct svm_node
	x_space = Malloc(struct svm_node, (3780+1) * prob.l); //memory for pairs of index/value

	int j=0; //counter to traverse x_space[i];
	for (int i=0;i < prob.l; ++i)
	{
		//set i-th element of prob.x to the address of x_space[j]. 
		//elements from x_space[j] to x_space[j+data[i].size] get filled right after next line
		prob.x[i] = &x_space[j];
		for (int k=0; k<3780; ++k, ++j)
		{
			x_space[j].index=k+1; //index of value
			x_space[j].value=descriptorsValues[k]; //value
			// cout<<"x_space["<<j<<"].index = "<<x_space[j].index<<endl;
			// cout<<"x_space["<<j<<"].value = "<<x_space[j].value<<endl;
		}
		x_space[j].index=-1;//state the end of data vector
		x_space[j].value=0;
		// cout<<"x_space["<<j<<"].index = "<<x_space[j].index<<endl;
		// cout<<"x_space["<<j<<"].value = "<<x_space[j].value<<endl;
		j++;

	}


	//set all default parameters for param struct
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 0;	// 1/num_features
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 1;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;

	double out = svm_predict(model,x_space);

	if(out == 1) {
	rectangle(img_gray,Point(rect.x,rect.y),Point(rect.x + rect.width,rect.y + rect.height),Scalar(0),-1,8,0);
	//cvtColor(img,img_gray,CV_BGR2GRAY);
	}
	}
	rectangle(img_gray,Point(0,img.rows*0.9),Point(img.cols-1,img.rows-1),Scalar(0),-1,8,0);
	line(img,Point(0,img.rows/3),Point(img.cols-1,img.rows/3), Scalar(255,0,0), 2, CV_AA);
	imshow("erode",img_gray);
	imshow("t",t);
	// Sobel(img_gray, Gx, -1, 1, 0, 3, CV_SCHARR);
	// Sobel(img_gray, Gy, -1, 0, 1, 3, CV_SCHARR);
	// // imshow("gx",Gx);
	// // imshow("gy",Gy);
	
	// Mat temp = img_gray.clone();

	// int A[3][3] = {-1,0,1,-2,0,2,-1,0,1};
	// int B[3][3] = {-2,-1,0,-1,0,1,0,1,2};
	// int C[3][3] = {0,1,2,-1,0,1,-2,-1,0};
	// int D[3][3] = {2,2,2,0,0,0,-5,-5,-5};

	// for(int i = 0; i < img.rows; i++)
	// 	for(int j = 0; j < img.cols; j++)
	// 	{
	// 		int sum_a, sum_b, sum_c, sum_d, count_a, count_b, count_c, count_d;
	// 		sum_a = sum_b = sum_c = sum_d = count_a = count_b = count_c = count_d = 0;
	// 		for(int m = -1; m < 2; m++)
	// 			for(int n = -1; n < 2; n++)
	// 			{
	// 				if(i+m < 0 || j+n < 0 || i+m >= img.rows || j+n >= img.cols) continue;
	// 				sum_a += img_gray.at<uchar>(i+m,j+n) * A[m+1][n+1];
	// 				sum_b += img_gray.at<uchar>(i+m,j+n) * B[m+1][n+1];
	// 				sum_c += img_gray.at<uchar>(i+m,j+n) * C[m+1][n+1];
	// 				sum_d += img_gray.at<uchar>(i+m,j+n) * D[m+1][n+1];
	// 				count_a += abs(A[m+1][n+1]);
	// 				count_b += abs(B[m+1][n+1]);
	// 				count_c += abs(C[m+1][n+1]);
	// 				count_d += abs(D[m+1][n+1]);
	// 			}
	// 		sum_a /= count_a; sum_b /= count_b; sum_c /= count_c; sum_d /= count_d;
	// 		Ga.at<uchar>(i,j) = sum_a; Gb.at<uchar>(i,j) = sum_b; Gc.at<uchar>(i,j) = sum_c; Gd.at<uchar>(i,j) = sum_d;

	// 		if(sqrt(sum_a*sum_a + sum_b*sum_b + sum_c*sum_c - sum_d*sum_d) > INTENSITY_TH) temp.at<uchar>(i,j) = 255;
	// 		else temp.at<uchar>(i,j) = 0;
	// 		// int x = Gx.at<uchar>(i,j);
	// 		// int y = Gy.at<uchar>(i,j);
	// 		// float grad = atan((float)x / y) * 180.0/PI;
	// 		// // cout<<grad<<'\t';
	// 		// if(grad < 0) grad *= -1;
	// 		// if((sqrt(x*x + y*y) > INTENSITY_TH)) img_gray.at<uchar>(i,j) = 255;
	// 		// else img_gray.at<uchar>(i,j) = 0;
	// 	}
	// img_gray = temp;
	// // Canny()
	// imshow("edges",img_gray);
	// imshow("ga",Ga); imshow("gb",Gb); imshow("Gc",Gc); imshow("Gd", Gd);
	// // imshow("Gx", Gx); imshow("Gy",Gy);
	//waitKey(0);
}

void Lanes::Dilation()
{
	Mat temp = bisect.clone();
	for(int i = 0; i < img.rows; i++)
	{
		for(int j = 0; j < img.cols; j++)
		{
			int count=0;
			for(int m = i-1; m < i+2; m++)
			{
				for(int n = j-1; n < j+2; n++)
				{
					if(m<0 || n<0 || m>=img.rows || n>=img.cols) continue;
					if(img_gray.at<uchar>(m,n) > INTENSITY_TH) count++;
				}
			}
			if(count > 1) temp.at<uchar>(i,j)=255;
		}
	}
	bisect = temp;
}

void Lanes::Hough()
{
	vector<Vec4i> lines;
	Mat color_lines = img.clone();
	HoughLinesP(bisect, lines, 3.0, 2.0*CV_PI/180, 2);
	for(int i = 0; i < lines.size(); i++)
		line(color_lines, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8);
	imshow("lines_bisect",color_lines);
	waitKey(0);
	vector<Vec4i> lines_1;
	HoughLinesP(img_gray, lines_1, 3.0, 2.0*CV_PI/180, 30);
	for(int i = 0; i < lines_1.size(); i++)
		line(color_lines, Point(lines_1[i][0], lines_1[i][1]), Point(lines_1[i][2], lines_1[i][3]), Scalar(255,0,0), 3, 8);
	imshow("lines_bisect",color_lines);
	// imshow("lines_gray",img_gray);
	//waitKey(0);
}

void Lanes::control_points()
{
	Mat temp(img.rows, img.cols, CV_8UC1, Scalar(0));
	int left_prev=0,right_prev=0,first=1;
	for(int i = img.rows-1; i >= 20; i-=5)
	{
		if(i-20 < 0) break;
		int left = 0,right = 0,c_l = 0, c_r = 0;
		for(int j = i; j > i-20; j--)
		{
			if(j < 0) break;
			for(int k = 0; k < img.cols/2; k++)
			{
				if(bisect.at<uchar>(j,k) > 100) { left += k; c_l++; }
			}
			for(int k = img.cols/2+1; k < img.cols; k++)
			{
				if(bisect.at<uchar>(j,k) > 100) { right += k; c_r++; }
			}
		}
		if(c_l == 0 && c_r == 0) continue;
		if(c_l != 0) left /= c_l;
		if(c_r != 0) right /= c_r;
		if(c_l != 0) temp.at<uchar>(i-10,left) = 255;
		if(c_r != 0) temp.at<uchar>(i-10,right) = 255;
		left_prev=left;
		right_prev=right;
		first=0;
	}
	int flag_l = 0, flag_r = 0, x_l, y_l, x_r, y_r;
	for(int i = img.rows; i > img.rows/3; i--)
	{
		for(int j = 0; j < img.cols/2; j++)
		{
			if(temp.at<uchar>(i,j) != 255) continue;
			if(flag_l == 0) {flag_l = 1; x_l = j; y_l = i; continue; }
			//if(abs(x_l-j)>img.cols*0.08f) { continue;}
			line(img, Point(x_l, y_l), Point(j, i), Scalar(255,0,0), 3, 8);
			x_l = j; y_l = i;
		}
		for(int j = img.cols/2 + 1; j < img.cols; j++)
		{
			if(temp.at<uchar>(i,j) != 255) continue;
			if(flag_r == 0) {flag_r = 1; x_r = j; y_r = i; continue; }
			line(img, Point(x_r, y_r), Point(j, i), Scalar(0,0,255), 3, 8);
			x_r = j; y_r = i;
		}
	}
	imshow("points",temp);
	imshow("lanes",img);
	//waitKey(1);
}

// void Lanes::control_vanishing()
// {

// }