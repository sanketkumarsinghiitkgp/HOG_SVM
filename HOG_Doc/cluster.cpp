#include <iostream>
#include <bits/stdc++.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <string>
#include <time.h>

using namespace std;
using namespace cv;

typedef struct color_val
{
	int b,g,r;
}color_val;

color_val color[6];

void initialise_color()
{
	color[0].b=255;
	color[0].g=0;
	color[0].r=0;

	color[1].b=0;
	color[1].g=255;
	color[1].r=0;

	color[2].b=0;
	color[2].g=0;
	color[2].r=255;

	color[3].b=255;
	color[3].g=255;
	color[3].r=0;

	color[4].b=0;
	color[4].g=255;
	color[4].r=255;

	color[5].b=255;
	color[5].g=0;
	color[5].r=255;
}

double distance(Point p1,Point p2)
{
	return (double)sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}

vector<Point> shift_center(Mat img,int noc)
{
	vector<Point> center(noc,Point(0,0));
	vector<int> count(noc,1);
	int flag=0;
	for(int i=0;i<img.cols;i++)
	{
		for(int j=0;j<img.rows;j++)
		{
			if(img.at<Vec3b>(j,i)==Vec3b(0,0,0)) continue;
			for(int k=0;k<noc;k++)
			{
				if(img.at<Vec3b>(j,i)==Vec3b(color[k].b,color[k].g,color[k].r))
				{
					count[k]++;
					flag=k;
					break;
				}
			}
			center[flag].x+=i;
			center[flag].y+=j;
		}
	}
	for(int i=0;i<noc;i++)
	{
		center[i].x/=count[i];
		center[i].y/=count[i];
	}
	return center;
}

void cluster_gen(vector<Point> cluster_cen, Mat img,int noc)
{
	int flag=0;
	double dist_min=FLT_MAX,dist;
	for(int i=0;i<img.cols;i++)
	{
		for(int j=0;j<img.rows;j++)
		{
			if(img.at<Vec3b>(j,i)==Vec3b(0,0,0)) continue;
			flag=0;
			dist_min=FLT_MAX;
			for(int k=0;k<noc;k++)
			{
				dist=distance(Point(i,j),cluster_cen[k]);
				if(dist_min>dist)
				{
					dist_min=dist;
					flag=k;
				}
			}
			img.at<Vec3b>(j,i)=Vec3b(color[flag].b,color[flag].g,color[flag].r);
		}
	}
}

void clustering(Mat img,vector<Point> cluster_center,int noc)
{
	int iter=100;
	while(iter--)
	{
		cluster_gen(cluster_center,img,noc);
		cluster_center=shift_center(img,noc);
	}
}

int main(int argc, char const *argv[])
{
	Mat test=imread(argv[1],1);
	time_t t;
	unsigned int seedval = (unsigned)time(&t);
	srand(seedval);
	int noc=atoi(argv[2]);
	vector<Point> cluster_center;
	for(int i=0;i<noc;i++)
	{
		Point temp;
		temp.x=(int)(rand()%test.cols);
		temp.y=(int)(rand()%test.rows);
		cluster_center.push_back(temp); 
	}
	initialise_color();
	clustering(test,cluster_center,noc);
	imshow("Cluster",test);
	waitKey(0);
	return 0;
}