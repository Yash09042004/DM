#include <bits/stdc++.h>
using namespace std;
int main(){
    string fname;
    cout<<"Enter the name of the file for input: ";
    if(!(cin>>fname)) return 0;
    ifstream infile(fname);
    if(!infile) return 1;
    long long n=0;
    double sumx=0, sumy=0, sumx2=0, sumy2=0, sumxy=0;
    string line;
    while(getline(infile,line)){
        if(line.size()==0) continue;
        if(line.back()=='\r') line.pop_back();
        for(char &c: line) if(c==',') c=' ';
        stringstream ss(line);
        double x,y;
        if(!(ss>>x>>y)) continue;
        n++;
        sumx += x;
        sumy += y;
        sumx2 += x*x;
        sumy2 += y*y;
        sumxy += x*y;
    }
    infile.close();
    double numerator = n*sumxy - sumx*sumy;
    double denom_part1 = n*sumx2 - sumx*sumx;
    double denom_part2 = n*sumy2 - sumy*sumy;
    double r = 0.0;
    if(denom_part1>0 && denom_part2>0) r = numerator / sqrt(denom_part1 * denom_part2);
    ofstream outfile("output.csv");
    if(!outfile) return 1;
    outfile<<r<<"\n";
    outfile.close();
    cout<<r<<"\n";
    return 0;
}