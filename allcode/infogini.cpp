#include <bits/stdc++.h>

using namespace std;

int main() {
    string fname;
    cout << "Enter the input CSV filename: ";
    if (!(cin >> fname)) return 0;

    ifstream fi(fname);
    if (!fi) {
        cerr << "Error: Could not open input file." << endl;
        return 1;
    }

    ofstream fo("output.csv");
    if (!fo) {
        cerr << "Error: Could not open output file." << endl;
        return 1;
    }

    string header;
    if (!getline(fi, header)) return 1;

    vector<string> attrs;
    {
        stringstream ss(header);
        string tok;
        while (getline(ss, tok, ',')) attrs.push_back(tok);
    }

    if (attrs.size() < 2) return 1;
    int m = attrs.size() - 1;

    unordered_map<string, unordered_map<string, int>> val_count;
    unordered_map<string, unordered_set<string>> dist_val;
    unordered_map<string, int> mainClass;

    string line;

    auto trim = [](string s) {
        s.erase(0, s.find_first_not_of(" \t\r\n"));
        s.erase(s.find_last_not_of(" \t\r\n") + 1);
        return s;
    };

    while (getline(fi, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        vector<string> vals;
        string cell;
        while (getline(ss, cell, ',')) vals.push_back(trim(cell));

        if (vals.size() != attrs.size()) continue;

        string cls = vals.back();
        mainClass[cls]++;
        for (int i = 0; i < m; ++i) {
            dist_val[attrs[i]].insert(vals[i]);
            val_count[attrs[i] + "|" + vals[i]][cls]++;
        }
    }
    fi.close();

    cout << "Choose option:\n1. Information Gain\n2. Gini Index\n3. Both\nEnter choice (1-3): ";
    int choice;
    if (!(cin >> choice)) return 0;

    int total = 0;
    for (auto& p : mainClass) total += p.second;
    if (total == 0) return 1;

    if (choice == 1 || choice == 3) {
        double main_entropy = 0;
        for (auto& p : mainClass) {
            double pr = double(p.second) / total;
            if (pr > 0) main_entropy -= pr * log2(pr);
        }
        fo << "Total Records," << total << "\n";
        fo << "Main Entropy," << main_entropy << "\n\n";
        fo << "Information Gain Results\n";
        
        double best_gain = DBL_MIN; 
        string best_attr = "";

        for (int i = 1; i < m; ++i) {
            string a = attrs[i];
            double entropy_after = 0;
            for (auto& v : dist_val[a]) {
                string k = a + "|" + v;
                int subset = 0;
                for (auto& pp : val_count[k]) subset += pp.second;
                if (subset == 0) continue;

                double cur_ent = 0;
                for (auto& pp : val_count[k]) {
                    double p = double(pp.second) / subset;
                    if (p > 0) cur_ent -= p * log2(p);
                }
                entropy_after += (double(subset) / total) * cur_ent;
            }
            double gain = main_entropy - entropy_after;
            fo << a << ",EntropyAfterSplit," << entropy_after << ",Gain," << gain << "\n";

            if (gain >= best_gain) { 
                best_gain = gain;
                best_attr = a;
            }
        }
        fo << "Selected_By_InformationGain," << best_attr << ",MinGain," << best_gain << "\n";
    }

    if (choice == 2 || choice == 3) {
        double best_gini = DBL_MAX; 
        string best_attr_g = "";
        
        fo << "\nGini Index Results\n";
        for (int i = 1; i < m; ++i) { 
            string a = attrs[i];
            double gini_index = 0;
            for (auto& v : dist_val[a]) {
                string k = a + "|" + v;
                int subset = 0;
                for (auto& pp : val_count[k]) subset += pp.second;
                if (subset == 0) continue;

                double gini_sub = 1.0;
                for (auto& pp : val_count[k]) {
                    double p = double(pp.second) / subset;
                    gini_sub -= p * p;
                }
                gini_index += (double(subset) / total) * gini_sub;
            }
            fo << a << ",GiniIndex," << gini_index << "\n";

            if (gini_index <= best_gini) { 
                best_gini = gini_index;
                best_attr_g = a;
            }
        }
        fo << "Selected_By_Gini_Max," << best_attr_g << ",MaxGini," << best_gini << "\n";
    }
    fo.close();
    cout << "Processing complete. Check output.csv for results." << endl;
    return 0;
}