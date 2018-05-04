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

typedef struct cluster {
	Point mean;
	vector<Point> members; 
} cluster;

typedef struct color_val
{
	int b,g,r;
}color_val;

color_val color[6];

void initialise_color()
{
	color[0].b=0;
	color[0].g=0;
	color[0].r=255;

	color[1].b=0;
	color[1].g=255;
	color[1].r=0;

	color[2].b=255;
	color[2].g=0;
	color[2].r=0;

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

vector<cluster> shift_center(vector<cluster> clusters,Mat img,int noc)
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
		(clusters[i].mean).x = center[i].x;
		(clusters[i].mean).y = center[i].y;
		
	}
	return clusters;
}

vector<cluster> cluster_gen(vector<cluster> clusters, Mat img,int noc)
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
				dist=distance(Point(i,j),clusters[k].mean);
				if(dist_min>dist)
				{
					dist_min=dist;
					flag=k;
				}
			}
			img.at<Vec3b>(j,i)=Vec3b(color[flag].b,color[flag].g,color[flag].r);
			(clusters[flag].members).push_back(Point(i,j));
		}
	}

	return clusters;
}

vector<cluster> clustering(Mat img,vector<cluster> clusters,int noc)
{
	int iter=10;
	while(iter--)
	{
		clusters = cluster_gen(clusters,img,noc);
		clusters = shift_center(clusters,img,noc);
	}
	return clusters;
}

//sanity check
void print(vector<cluster> clusters,int noc) {
	for(int i = 0;i<noc;i++) {
		cout << endl << "----------------" << i << "-------------" << endl;
		for(int j = 0;j<clusters[i].members.size();j++) {
			Point t;
			t.x = clusters[i].members[j].x;
			t.y = clusters[i].members[j].y;
			cout << t.x << "," << t.y << endl;
		}
	}

	//print mean
	cout << endl << endl << endl;

	cout << "--------------" << endl;
	for(int i = 0;i < noc;i++) {
		cout << i << " : " << clusters[i].mean << endl;
	}
}
 
int main(int argc, char const *argv[])
{
	Mat test=imread(argv[1],1);
	time_t t;
	unsigned int seedval = (unsigned)time(&t);
	srand(seedval);
	int noc=atoi(argv[2]);
	vector<cluster> clusters(noc);
	for(int i=0;i<noc;i++)
	{
		Point temp;
		temp.x=(int)(rand()%test.cols);
		temp.y=(int)(rand()%test.rows);
		clusters[i].mean.x = temp.x;
		clusters[i].mean.y = temp.y; 
	}
	initialise_color();
	clusters = clustering(test,clusters,noc);
	imshow("Cluster",test);
	waitKey(0);
	print(clusters,noc);
	return 0;
}