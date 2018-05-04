#ifndef _CLUSTER_HPP_
#define _CLUSTER_HPP_

#include <iostream>
#include <bits/stdc++.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/gpu/gpu.hpp>
#include <string>
#include <time.h>
#define D_THRESH 5
using namespace cv;
using namespace std;

typedef struct cluster {
	Point mean;
	vector<Point> members; 
} cluster;

typedef struct color_val
{
	int b,g,r;
}color_val;

class clustering{
	private:
		color_val color[6];
		Mat img_org;
		int noc;
		vector<cluster> clusters;
		void initialise_color();
		double distance(Point ,Point );
		vector<cluster> shift_center(vector<cluster> ,Mat ,int );
		vector<cluster> cluster_gen(vector<cluster> , Mat ,int );
		vector<cluster> clustering_iter(Mat ,vector<cluster> ,int );
		vector<cluster> merge_cluster(vector<cluster > ,int );
		void show(vector<cluster> , Mat);
	public:
		void init(Mat ,int);
		void print(vector<cluster> ,int );
		vector<cluster> ret();
};

#endif