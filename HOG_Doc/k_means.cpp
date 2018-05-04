#include <bits/stdc++.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

struct Point{
	int x;
	int y;
	int parent;

};

double dist(Point p1, Point p2)
{
	return pow((pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2)),0.5);
}

class K_MEAN{
private:
	Mat img;
	int no_clusters;
	vector<Point> P;
	vector <Point> C;
	vector<vector<Point> > Clusters;
	int img_rows;
	int img_cols;
	int no_points;

public:
	K_MEAN(vector<Point> p,int size,int n)
	{
		img.imread(img_name);
		for(int i=0;i<img.cols;i++)
		{
			for(j=0;j<img.rows;j++)
			{
				if(img.at<uchar>(i,j)==255)
					{
						Point p;
						p.x=i;
						p.y=j;
						P.push_back(p);
					}
			}
		}
		size=P.size();
		//P=p;
		no_clusters=n;
		C.resize(n);
		Clusters.resize(k);
		//img_cols=c;
		//img_rows=r;
		no_points=size;

		time_t t;
		unsigned int seedval =(unsigned)time(&t);
		srand(seedval);
		for(int i=0;i<no_clusters;i++)
		{
			C[i].x=(int)rand()%img.cols;
			C[i].y=(int)rand()%img.rows;
		}

	}

	void k_m()
	{
		
		for (int i=0;i<no_points;i++)
		{
			double d_min=dist(C[0],P[i]);
			int k_min=0;
			for(int k=0;i<no_clusters;k++)
			{
				if(dist(C[k],P[i])<d_min)
				{
					d_min=dist(C[k],P[i]);
					k_min=k;
				}
			}
			P[i].parent=k;
		}
		for(int k=0;k<no_clusters;k++)
		{
			int count = 0;
			Point p;
			p.x=0;
			p.y=0;
			for(int i=0;i<no_points;i++)
			{
				if(p[i].parent==k)
				{
					p.x+=P[i].x;
					p.y+=P[i].y;
					count++;
				}

			}
			if(count)
			{
				C[k].x=p.x/count;
				C[k].y=p.y/count;
			}
		}
	}

	void assign()
	{
		for(int i=0;i<no_points;i++)
		{
			Cluster[P[i].parent].push_back(P[i]);
		}
		time_t t;
		unsigned int seedval=(unsigned)time(&t);
		for(int k=0;k<no_clusters;k++)
		{
			
		}
	}

};
