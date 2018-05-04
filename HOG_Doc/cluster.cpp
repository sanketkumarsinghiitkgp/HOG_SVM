#include "cluster.hpp"

//using namespace std;
//using namespace cv;


void clustering::initialise_color()
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

double clustering::distance(Point p1,Point p2)
{
	return (double)sqrt(pow(p1.x-p2.x,2)+pow(p1.y-p2.y,2));
}

vector<cluster> clustering::shift_center(vector<cluster> clusters,Mat img,int noc)
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

vector<cluster> clustering::cluster_gen(vector<cluster> clusters, Mat img,int noc)
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

vector<cluster> clustering::clustering_iter(Mat img,vector<cluster> clusters,int noc)
{
	int iter=15;
	//cout<<"E2"<<endl;
	while(iter--)
	{
		//cout<<"E3"<<endl;
		clusters = cluster_gen(clusters,img,noc);
		clusters = shift_center(clusters,img,noc);
		//cout<<"E5"<<endl;
	}
	return clusters;
}

//sanity check
void clustering::print(vector<cluster> clusters,int noc) {
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

vector<cluster> clustering::ret()
{
	return clusters;
}

vector<cluster> clustering::merge_cluster(vector<cluster> Clusters,int no_clusters) 
{
	int count=0;
	vector<cluster> Final;
	Final.resize(no_clusters+1);
	cout<<"E"<<endl;
	for(int k=0;k<no_clusters-1;k++)
	{
		for(int k1=k+1;k1<no_clusters;k1++)
		{
			double d_min=FLT_MAX;

			for(int i=0;i<Clusters[k].members.size();i++)
			{
				for(int j=0;j<Clusters[k1].members.size();j++)
				{
					if(distance(Clusters[k].members[i],Clusters[k1].members[j])<d_min)
						d_min=distance(Clusters[k].members[i],Clusters[k1].members[j]);
				}
			}
			cout<<"E1"<<endl;
			if(d_min<=D_THRESH)
			{
				cout<<"E3"<<endl;
				Final[count].members=Clusters[k].members;
				Final[count].members.insert(Final[count].members.end(),Clusters[k1].members.begin(),Clusters[k1].members.end());
				int n1=Clusters[k].members.size();
				int n2=Clusters[k1].members.size();
				Point p1;
				p1.x=Clusters[k].mean.x;
				p1.y=Clusters[k].mean.y;
				Point p2;
				p2.x=Clusters[k1].mean.x;
				p2.y=Clusters[k1].mean.y;
				Final[count].mean.x=(n1*p1.x+n2*p2.x)/(n1+n2);
				Final[count].mean.y=(n1*p1.y+n2*p2.y)/(n1+n2);
				count++;
				
			}
			else
			{
				cout<<"E4"<<endl;
				cout<<"K = "<<k<<" K1 = "<<k1<<endl;
				Final[count++]=Clusters[k];
				Final[count++]=Clusters[k1];
			}

		}
		
	}
	cout<<"E2"<<endl;
	return Final;
}

void clustering::show(vector<cluster> clusters,Mat img)
{
	//cout<<"E1"<<endl;
	for(int i=0;i<clusters.size();i++)
	{
		for(int j=0;j<clusters[i].members.size();j++)
		{
			img.at<Vec3b>(clusters[i].members[j].y,clusters[i].members[j].x)=Vec3b(color[i].b,color[i].g,color[i].r);
		}
	}
}

void clustering::init(Mat img,int n)
{
	img_org=img;
	noc=n;
	time_t t;
	unsigned int seedval = (unsigned)time(&t);
	srand(seedval);
	//cout<<"E1"<<endl;
	for(int i=0;i<noc;i++)
	{
		Point temp;
		cluster new_clust;
		//cout<<"E"<<endl;
		//cout<<"Rows = "<<img_org.rows<<" Cols = "<<img_org.cols<<endl;
		temp.x=(int)(rand()%img_org.cols);
		temp.y=(int)(rand()%img_org.rows);
		//cout<<"E0"<<endl;
		new_clust.mean.x=temp.x;
		new_clust.mean.y=temp.y;
		clusters.push_back(new_clust);
	}
	initialise_color();
	clusters=clustering_iter(img_org,clusters,noc);
	//clusters=merge_cluster(clusters,noc);
	//show(clusters,img_org);
	//cout<<"E4"<<endl;
	for(int i=0;i<clusters.size();i++)
		circle(img_org,clusters[i].mean,3,Scalar(color[i].b,color[i].g,color[i].r),-1,8,0);
	clusters.clear();
	imshow("Cluster",img_org);	
	waitKey(1);
	//print(clusters,noc);
}

