#include <bits/stdc++.h>
using namespace std;

int main() {
    string fname;
    cout << "Enter CSV file: ";
    cin >> fname;
    ifstream fin(fname);
    if (!fin) return cout << "File not found\n", 0;

    string line;
    getline(fin, line);
    vector<string> attr;
    stringstream ss(line);
    string x;
    while (getline(ss, x, ',')) attr.push_back(x);
    int m = attr.size() - 1;

    map<string,int> clsCnt;
    map<pair<string,string>, map<string,int>> valCnt;
    map<string,set<string>> vals;

    auto trim = [](string s){ s.erase(0,s.find_first_not_of(" \t\r\n")); s.erase(s.find_last_not_of(" \t\r\n")+1); return s; };

    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream t(line); vector<string> v; string c;
        while (getline(t, c, ',')) v.push_back(trim(c));
        if (v.size()!=attr.size()) continue;
        string cls = v.back(); clsCnt[cls]++;
        for (int i=0;i<m;i++){ vals[attr[i]].insert(v[i]); valCnt[{attr[i],v[i]}][cls]++; }
    }

    int tot = 0; for (auto &p:clsCnt) tot += p.second;
    double mainEnt = 0;
    for (auto &p:clsCnt){ double pr=(double)p.second/tot; mainEnt -= pr*log2(pr); }

    cout << "\nMain Entropy: " << mainEnt << "\n";
    cout << "Attribute, Gain, Gini\n";

    string bestE="", bestG="";
    double maxGain=-1, minGini=1e9;

    for (int i=0;i<m;i++){
        string a = attr[i];
        double entA=0, giniA=0;
        for (auto &v:vals[a]){
            int sub=0; for(auto &p:valCnt[{a,v}]) sub+=p.second;
            double e=0, g=1;
            for(auto &p:valCnt[{a,v}]){
                double pr=(double)p.second/sub;
                e -= pr*log2(pr);
                g -= pr*pr;
            }
            entA += (double)sub/tot*e;
            giniA += (double)sub/tot*g;
        }
        double gain = mainEnt - entA;
        cout << a << ", " << gain << ", " << giniA << "\n";
        if (gain>maxGain) maxGain=gain, bestE=a;
        if (giniA<minGini) minGini=giniA, bestG=a;
    }

    cout << "\nBest by InfoGain: " << bestE << " ("<<maxGain<<")";
    cout << "\nBest by Gini: " << bestG << " ("<<minGini<<")\n";
}
