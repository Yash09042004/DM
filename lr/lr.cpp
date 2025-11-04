#include <bits/stdc++.h>
using namespace std;
int main() {
    string filename;
    cout << "Enter input filename: ";
    getline(cin, filename);
    ifstream in(filename);
    if (!in) return 1;
    string line;
    getline(in, line);
    vector<double> x, y;
    while (getline(in, line)) {
        if(line.empty()) continue;
        stringstream ss(line);
        string a, b;
        getline(ss, a, ',');
        getline(ss, b, ',');
        x.push_back(stod(a));
        y.push_back(stod(b));
    }
    in.close();
    int n = x.size();
    double xmean = accumulate(x.begin(), x.end(), 0.0) / n;
    double ymean = accumulate(y.begin(), y.end(), 0.0) / n;
    double num = 0, den = 0;
    for (int i = 0; i < n; i++) {
        num += (x[i] - xmean) * (y[i] - ymean);
        den += (x[i] - xmean) * (x[i] - xmean);
    }
    double slope = num / den;
    double intercept = ymean - slope * xmean;
    cout << "Slope=" << slope << " Intercept=" << intercept << "\n";
    cout << "Enter new x: ";
    string s; getline(cin, s);
    double newx = stod(s);
    double newy = slope * newx + intercept;
    cout << "Predicted y=" << newy << "\n";
    ofstream out("output.csv");
    out << "slope,intercept,new_x,new_y\n";
    out << slope << "," << intercept << "," << newx << "," << newy << "\n";
    out.close();
    return 0;
}
