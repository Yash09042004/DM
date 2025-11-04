#include <bits/stdc++.h>
using namespace std;
using Point = vector<double>;

double dist(const Point &a, const Point &b){
    double s=0; for(size_t i=0;i<a.size();++i){ double d=a[i]-b[i]; s+=d*d; } return sqrt(s);
}

int main(){
    string fname; cout<<"CSV file: "; if(!(cin>>fname)) return 0;
    ifstream in(fname); if(!in){ cerr<<"Cannot open\n"; return 1; }
    vector<Point> X; string line, cell;
    while(getline(in,line)){
        if(line.empty()) continue;
        stringstream ss(line); Point p;
        while(getline(ss,cell,',')){ if(cell.size()) p.push_back(stod(cell)); }
        if(!p.empty()) X.push_back(p);
    }
    in.close();
    int n = X.size(); if(n==0){ cerr<<"No data\n"; return 1; }
    int linkage; cout<<"Linkage (1=single,2=complete,3=average): "; cin>>linkage;
    int k; cout<<"Clusters wanted: "; cin>>k; if(k<1||k>n){ cerr<<"Invalid k\n"; return 1; }

    vector<vector<double>> pd(n, vector<double>(n));
    for(int i=0;i<n;++i) for(int j=i+1;j<n;++j) pd[i][j]=pd[j][i]=dist(X[i],X[j]);

    vector<set<int>> C(n); for(int i=0;i<n;++i) C[i].insert(i);
    vector<int> alive(n,1); int alive_cnt=n;

    auto cluster_dist = [&](int a,int b)->double{
        double val = (linkage==2?0:1e18); int cnt=0; double sum=0;
        for(int i: C[a]) for(int j: C[b]){
            double d=pd[i][j]; if(linkage==1) val=min(val,d);
            else if(linkage==2) val=max(val,d);
            else { sum+=d; ++cnt; }
        }
        if(linkage==3) return sum / max(1,cnt);
        return val;
    };

    while(alive_cnt>k){
        double best=1e308; int bi=-1,bj=-1;
        for(int i=0;i<n;++i) if(alive[i]) for(int j=i+1;j<n;++j) if(alive[j]){
            double d = cluster_dist(i,j);
            if(d<best){ best=d; bi=i; bj=j; }
        }
        if(bi==-1) break;
        for(int x: C[bj]) C[bi].insert(x);
        C[bj].clear(); alive[bj]=0; --alive_cnt;
    }

    vector<int> assign(n,-1); int cid=0;
    for(int i=0;i<n;++i) if(!C[i].empty()){
        for(int x: C[i]) assign[x]=cid;
        ++cid;
    }

    ofstream out("output.csv");
    out<<"Point,Cluster\n";
    for(int i=0;i<n;++i){
        cout<<"Point "<<i<<" -> Cluster "<<assign[i]<<"\n";
        out<<i<<","<<assign[i]<<"\n";
    }
    out.close();
    return 0;
}