#include <bits/stdc++.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;


struct point{
	int x;
	int y;
};

double dist(point p1,point p2)
{
	return sqrt(pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2));
}

double min_dist(vector<point> c1, vector<point> c2)
{
	double d_min=0;
	int total=0;
	for(int i=0;i<c1.size();i++)
	{
		for(int j=0;j<c2.size();j++)
		{
			//if(dist(c1[i],c2[j])<d)
			//	d_min=dist(c1[i],c2[j]);
			d_min+=dist(c1[i],c2[j]);
			total++;
		}
	}
	return d_min/(total);
}
vector<vector<point> > merge_cluster(vector<vector<point> > C)
{
	//C.resize(noc);
	double d_min=FLT_MAX,distance;
	int min_i, min_j;
	for(int i=0; i<C.size(); i++)
	{
		for(int j=i+1; j<C.size(); j++)
		{
			distance=min_dist(C[i], C[j]);
			if(distance<d_min)
			{
				d_min=distance;
				min_i=i;
				min_j=j;
			}
		}
	}
	C[min_i].insert(C[min_i].end(),C[min_j].begin(),C[min_j].end());
	C.erase(C.begin()+min_j);
	return C;
}
vector<vector<point> > generate_cluster(Mat img)
{
	point p;
	vector<vector<point> > cluster;
	for(int i=0; i<img.rows; i++)
	{
		for(int j=0; j<img.cols; j++)
		{
			if(img.at<uchar>(i,j)>200)
		    {
		    	vector<point> temp;
            	p.x=i;
            	p.y=j;
            	temp.push_back(p);
            	cluster.push_back(temp);
            }
		}
	}
	return cluster;
}

void print_points(Mat img, vector<vector<point>>cluster)
{
	vector<point> a;
	a=cluster[0];
	for(int i=0; i<a.size();i++)
		img.at<Vec3b>(a[i].x,a[i].y)=Vec3b(255,255,0);
    a=cluster[1];
    for(int i=0; i<a.size();i++)
		img.at<Vec3b>(a[i].x,a[i].y)=Vec3b(0,0,255);
    return;
}

int main(int argc, char** argv)
{
	Mat img=imread(argv[1],0);
	vector<vector<point>>cluster=generate_cluster(img);
	while(cluster.size()>2)
	{
		cluster=(merge_cluster(cluster));
	}
	Mat img_cluster(img.rows, img.cols, CV_8UC3, Scalar(0,0,0));
	print_points(img_cluster,cluster);
	imshow("Cluster",img_cluster);
	waitKey(0);
	return 0;
}