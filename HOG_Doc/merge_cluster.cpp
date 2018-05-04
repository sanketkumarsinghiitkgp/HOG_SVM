#define D_THRESH 5

vector<cluster> merge_cluster(vector<cluster > Clusters,int no_clusters,int no_points) 
{
	int count=0;
	vector<cluster> Final;
	Final.resize(no_clusters);
	for(int k=0;k<no_clusters-1;k++)
	{
		for(int k1=k+1;k1<no_clusters;k1++)
		{
			double d_min=FLT_MAX;

			for(auto it=Clusters[k].members.begin();it!=Clusters[k].members.end();it++)
			{
				for(auto it1=Clusters[k1].members.begin();it1!=Clusters[k1].members.end();it1++)
				{
					if(distance(*it1,*it)<d_min)
						d_min=distance(*it1,*it);
				}
			}
			if(d_min<=D_THRESH)
			{
				Final[count].members=Clusters[k].members;
				Final[count].members.insert(Final[count].members.end(),Clusters[k1].members.begin(),Clusters[k1].members.end());
				int n1=Clusters[k].members.size();
				int n2=Clusters[k1].members.size();
				Point p1=Clusters[k].mean;
				Point p2=Clusters[k1].mean;
				Final[count].mean.x=(n1*p1.x+n2*p2.x)/(n1+n2);
				Final[count].mean.y=(n1*p1.y+n2*p2.y)/(n1+n2);
				count++;
			}
			else
			{
				Final[count].members=Clusters[k].members;
				Final[count].mean=Clusters[k].mean;
				count++;
				Final[count].members=Clusters[k1].members;
				Final[count].mean=Clusters[k1].mean;
				count++;

			}

		}
		
	}
	return Final;

}
