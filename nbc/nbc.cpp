#include <bits/stdc++.h>
using namespace std;
int main() {
    string filename;
    cout << "Enter input filename: ";
    getline(cin, filename);
    ifstream in(filename);
    ofstream out("output.csv");
    string line;
    getline(in, line);
    vector<string> attrs;
    stringstream ss(line);
    string temp;
    while(getline(ss, temp, ',')) attrs.push_back(temp);
    int n = attrs.size() - 1;
    vector<vector<string>> data;
    while(getline(in, line)) {
        vector<string> row;
        stringstream s(line);
        while(getline(s, temp, ',')) row.push_back(temp);
        data.push_back(row);
    }
    map<string, int> class_count;
    map<string, map<string, map<string, int>>> attr_count;
    for(auto& row : data) {
        string cls = row[n];
        class_count[cls]++;
        for(int i=0; i<n; i++) attr_count[attrs[i]][cls][row[i]]++;
    }
    // Predict for all rows
    out << "Predicted\n";
    for(auto& row : data) {
        string best_class;
        double best_prob = -1;
        map<string, double> likelihoods;
        double total = 0;
        for(auto& cc : class_count) {
            double prob = (double)cc.second / data.size();
            for(int i=0; i<n; i++) {
                int num = attr_count[attrs[i]][cc.first][row[i]] + 1;
                int denom = class_count[cc.first] + attr_count[attrs[i]][cc.first].size();
                prob *= (double)num / denom;
            }
            likelihoods[cc.first] = prob;
            total += prob;
        }
        for(auto& cc : class_count) {
            out << cc.first << ": " << fixed << setprecision(4) << likelihoods[cc.first]/total << ", ";
        }
        out << "\n";
    }
    // Predict for custom input
    cout << "Enter values for attributes (comma separated, e.g. TRUE,FALSE,TRUE): ";
    string input_line;
    getline(cin, input_line);
    vector<string> input_row;
    stringstream iss(input_line);
    while(getline(iss, temp, ',')) input_row.push_back(temp);
    map<string, double> custom_likelihoods;
    double custom_total = 0;
    for(auto& cc : class_count) {
        double prob = (double)cc.second / data.size();
        for(int i=0; i<n; i++) {
            int num = attr_count[attrs[i]][cc.first][input_row[i]] + 1;
            int denom = class_count[cc.first] + attr_count[attrs[i]][cc.first].size();
            prob *= (double)num / denom;
        }
        custom_likelihoods[cc.first] = prob;
        custom_total += prob;
    }
    cout << "Likelihoods for input: ";
    for(auto& cc : class_count) {
        cout << cc.first << ": " << fixed << setprecision(4) << custom_likelihoods[cc.first]/custom_total << ", ";
    }
    cout << endl;
    in.close();
    out.close();
    return 0;
}