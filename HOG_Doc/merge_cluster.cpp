#define D_THRESH 5

vector<cluster> merge_cluster(vector<cluster > Clusters,int no_clusters,int no_points) 
{
	int count;
	vector<cluster> Final;
	Final.clear();
	Final.resize(no_clusters);
	for(int k=0;k<no_clusters;k++)
	{
		for(int k1=k+1;k1<no_clusters;k1++)
		{
			double d_min=FLT_MIN;

			for(auto it=Clusters[k].members.begin();it!=Clusters[k].members.end();it++)
			{
				for(auto it1=Clusters[k1].members.begin();it1!=Clusters[k1].members.end();it1++)
				{
					if(distance(*it1,*it)<d_min)
						d_min=distance(*it1,*it);
				}
			}
			if(d_min<D_THRESH)
			{
				Final[count].members=Cluster[k].members;
				Final[count].insert(Final[count].members.end(),Cluster[k1].members.begin(),Cluster[k1].members.end());
				int n1=Cluster[k].members.size();
				int n2=Cluster[k1].members.size();
				Point p1=Cluster[k].mean;
				Point p2=Cluster[k1].mean;
				Final[count].mean.x=(n1*p1.x+n2*p2.x)/(n1+n2);
				Final[count].mean.y=(n1*p1.y+n2*p2.y)/(n1+n2);
				count++;
			}
			else
			{
				Final[count].members=Cluster[k].members;
				Final[count].mean=Cluster[k].mean;
				count++;
				Final[count].members=Cluster[k1].members;
				Final[count].mean=Cluster[k1].mean;
				count++;

			}

		}
		
	}
	return Final;

}
