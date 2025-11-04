#include <bits/stdc++.h>
using namespace std;

int main(){
    string file; double minsupP, minconfP;
    cout<<"CSV file: "; cin>>file;
    cout<<"Min support %: "; cin>>minsupP;
    cout<<"Min confidence %: "; cin>>minconfP;
    double minsup = minsupP/100.0, minconf = minconfP/100.0;

    ifstream in(file);
    if(!in){ cerr<<"Cannot open\n"; return 1; }
    vector<vector<string>> T;
    string line;
    while(getline(in,line)){
        if(line.empty()) continue;
        vector<string> items; string tok;
        stringstream ss(line);
        while(getline(ss,tok,',')){
            if(tok.size()) items.push_back(tok);
        }
        sort(items.begin(), items.end());
        items.erase(unique(items.begin(), items.end()), items.end());
        if(!items.empty()) T.push_back(items);
    }
    in.close();
    int N = T.size();
    map<set<string>, double> support;

    // L1
    map<string,int> cnt1;
    for(auto &t: T) for(auto &i: t) cnt1[i]++;
    set<set<string>> L;
    for(auto &p: cnt1){
        double s = (double)p.second / N;
        if(s>=minsup){ L.insert({p.first}); support[{p.first}] = s; }
    }

    cout<<"\nFrequent itemsets:\n";
    ofstream out("output.csv"); out<<"Frequent itemsets\n";
    int k=2;
    set<set<string>> prev = L;
    for(auto &it: prev){ cout<<"{"<<*it.begin()<<"} "<<support[it]*100<<"%\n"; out<<"{"; out<<*it.begin(); out<<"},"<<support[it]*100<<"\n"; }
    while(!prev.empty()){
        set<set<string>> C;
        vector<set<string>> v(prev.begin(), prev.end());
        for(size_t i=0;i<v.size();++i) for(size_t j=i+1;j<v.size();++j){
            set<string> u = v[i];
            u.insert(v[j].begin(), v[j].end());
            if((int)u.size()==k) C.insert(u);
        }
        map<set<string>,int> cnt;
        for(auto &t: T){
            set<string> ts(t.begin(), t.end());
            for(auto &c: C) if(includes(ts.begin(), ts.end(), c.begin(), c.end())) cnt[c]++;
        }
        prev.clear();
        for(auto &p: cnt){
            double s = (double)p.second / N;
            if(s>=minsup){ prev.insert(p.first); support[p.first]=s; }
        }
        if(prev.empty()) break;
        for(auto &it: prev){
            cout<<"{";
            bool f=true;
            for(auto &x: it){ if(!f) cout<<","; cout<<x; f=false; }
            cout<<"} "<<support[it]*100<<"%\n";
            out<<"{";
            bool ff=true;
            for(auto &x: it){ if(!ff) out<<","; out<<x; ff=false; }
            out<<"},"<<support[it]*100<<"\n";
        }
        ++k;
    }

    // association rules
    cout<<"\nAssociation rules:\n"; out<<"\nAssociation rules\n";
    for(auto &fs : support){
        set<string> itemset = fs.first;
        if(itemset.size()<2) continue;
        vector<string> items(itemset.begin(), itemset.end());
        int m = items.size();
        for(int mask=1; mask < (1<<m)-1; ++mask){
            set<string> A, B;
            for(int i=0;i<m;++i) if(mask&(1<<i)) A.insert(items[i]); else B.insert(items[i]);
            double supAB = support[itemset];
            if(!support.count(A)) continue;
            double conf = supAB / support[A];
            if(conf >= minconf){
                cout<<"{";
                bool f=true;
                for(auto &x:A){ if(!f) cout<<","; cout<<x; f=false; }
                cout<<"} -> {";
                f=true;
                for(auto &x:B){ if(!f) cout<<","; cout<<x; f=false; }
                cout<<"}  conf="<<conf*100<<"%  supp="<<supAB*100<<"%\n";
                out<<"\"";
                bool ff=true;
                for(auto &x:A){ if(!ff) out<<","; out<<x; ff=false; }
                out<<"\",\"";
                ff=true;
                for(auto &x:B){ if(!ff) out<<","; out<<x; ff=false; }
                out<<"\", "<<conf*100<<", "<<supAB*100<<"\n";
            }
        }
    }

    out.close();
    return 0;
}