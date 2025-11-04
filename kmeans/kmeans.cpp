#include <bits/stdc++.h>
using namespace std;

double dist(const vector<double>& a, const vector<double>& b){
    double s=0; for(size_t i=0;i<a.size();++i){ double d=a[i]-b[i]; s+=d*d; } return sqrt(s);
}

int main(){
    string fn; cout<<"Enter CSV file: "; if(!(cin>>fn)) return 0;
    ifstream f(fn); if(!f) return 1;
    vector<vector<double>> X; string line;
    while(getline(f,line)){
        if(line.empty()) continue;
        stringstream ss(line); string cell; vector<double> p;
        while(getline(ss,cell,',')) if(cell.size()) p.push_back(stod(cell));
        if(!p.empty()) X.push_back(p);
    }
    if(X.empty()) return 1;
    int n = X.size(), d = X[0].size(), k; cout<<"Enter K: "; cin>>k;
    if(k<=0 || k>n) return 1;
    vector<vector<double>> cent(k, vector<double>(d));
    for(int i=0;i<k;++i) cent[i]=X[i% n];
    vector<int> assign(n,0);
    for(int iter=0; iter<100; ++iter){
        bool changed=false;
        for(int i=0;i<n;++i){
            int best=0; double bd=dist(X[i],cent[0]);
            for(int j=1;j<k;++j){ double dd=dist(X[i],cent[j]); if(dd<bd){bd=dd; best=j;} }
            if(assign[i]!=best){ assign[i]=best; changed=true; }
        }
        if(!changed) break;
        vector<vector<double>> sum(k, vector<double>(d,0.0));
        vector<int> cnt(k,0);
        for(int i=0;i<n;++i){ cnt[assign[i]]++; for(int j=0;j<d;++j) sum[assign[i]][j]+=X[i][j]; }
        for(int j=0;j<k;++j) if(cnt[j]) for(int t=0;t<d;++t) cent[j][t]=sum[j][t]/cnt[j];
    }
    ofstream out("output.csv"); out<<"Point,Cluster\n";
    for(int i=0;i<n;++i) out<<i<<","<<assign[i]<<"\n";
    return 0;
}