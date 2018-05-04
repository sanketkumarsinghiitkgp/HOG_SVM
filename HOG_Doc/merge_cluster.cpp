#define D_THRESH 5

vector<vector<Point> > merge_cluster(vector<vector<Point> > Clusters,int no_clusters,int no_points) 
{
	int count;
	vector<vector<Point> > Final;
	Final.clear();
	Final.resize(no_clusters);
	for(int k=0;k<no_clusters;k++)
	{
		for(int k1=k+1;k1<no_clusters;k1++)
		{
			double d_min=FLT_MIN;

			for(auto it=Clusters[k].begin();it!=Clusters[k].end();it++)
			{
				for(auto it1=Clusters[k1].begin();it1!=Clusters[k1].end();it1++)
				{
					if(distance(*it1,*it)<d_min)
						d_min=distance(*it1,*it);
				}
			}
			if(d_min<D_THRESH)
			{
				Final[count]=Cluster[k];
				Final[count].insert(Final[count].end(),Cluster[k1].begin(),Cluster[k1].end());
				count++;
			}
			else
			{
				Final[count++]=Cluster[k];
				Final[count++]=Cluster[k1];
			}

		}
		
	}
	return Final;

}

//Call shift_center after this