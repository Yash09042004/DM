#include <bits/stdc++.h>
using namespace std;
#define ll long long

vector<string> split(const string& s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void display(ostream& out, const set<string>& itemset) {
    out << "{";
    bool first = true;
    for (const auto& item : itemset) {
        if (!first) out << ", ";
        out << item;
        first = false;
    }
    out << "}";
}

set<set<string>> candidate_gen(const set<set<string>>& Lk_1, int k) {
    set<set<string>> Ck;
    vector<set<string>> Lk_1_vec(Lk_1.begin(), Lk_1.end());

    for (size_t i = 0; i < Lk_1_vec.size(); ++i) {
        for (size_t j = i + 1; j < Lk_1_vec.size(); ++j) {
            set<string> union_set;
            set_union(Lk_1_vec[i].begin(), Lk_1_vec[i].end(),
                      Lk_1_vec[j].begin(), Lk_1_vec[j].end(),
                      inserter(union_set, union_set.begin()));

            if (union_set.size() == k) {
                Ck.insert(union_set);
            }
        }
    }
    return Ck;
}

set<set<string>> getSubsets(const set<string>& itemset) {
    set<set<string>> subsets;
    vector<string> items(itemset.begin(), itemset.end());
    int n = items.size();

    for (int i = 1; i < (1 << n); ++i) {
        set<string> subset;
        for (int j = 0; j < n; ++j) {
            if ((i >> j) & 1) {
                subset.insert(items[j]);
            }
        }
        subsets.insert(subset);
    }
    return subsets;
}

int main() {
    string filename;
    double minsup;
    double minconf;

    cout << "Enter the name of the CSV file: ";
    cin >> filename;

    cout << "Enter the minimum support (in percent, e.g., 25): ";
    cin >> minsup;

    cout << "Enter the minimum confidence (in percent, e.g., 50): ";
    cin >> minconf;

    double minisup = minsup / 100.0;
    double min_confidence = minconf / 100.0;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open the file " << filename << endl;
        return 1;
    }

    ofstream outfile("output.csv");
    if (!outfile.is_open()) {
        cerr << "Error: Could not open output.csv for writing." << endl;
        return 1;
    }

    vector<set<string>> transactions;
    string line;
    while (getline(file, line)) {
        vector<string> strs = split(line, ',');
        set<string> transaction_set(strs.begin(), strs.end());
        if (!transaction_set.empty()) {
            transactions.push_back(transaction_set);
        }
    }
    file.close();

    if (transactions.empty()) {
        cout << "The file is empty or could not be read." << endl;
        outfile << "The file is empty or could not be read." << endl;
        outfile.close();
        return 0;
    }

    int tot_trans = transactions.size();

    cout << "\nTotal transactions: " << tot_trans << endl;
    outfile << "Total transactions: " << tot_trans << endl;

    map<set<string>, double> freq_item_sp;
    set<set<string>> freq_itemsets;

    map<string, int> mpstr;
    for (const auto& transaction : transactions) {
        for (const auto& item : transaction) {
            mpstr[item]++;
        }
    }

    set<set<string>> Lk;
    for (const auto& pair : mpstr) {
        double support = static_cast<double>(pair.second) / tot_trans;
        if (support >= minisup) {
            Lk.insert({pair.first});
            freq_item_sp[{pair.first}] = support;
            freq_itemsets.insert({pair.first});
        }
    }
    
    cout << "\n--- Frequent 1-itemsets ---" << endl;
    outfile << "\n--- Frequent 1-itemsets ---" << endl;
    if (Lk.empty()) {
        cout << "No frequent 1-itemsets found." << endl;
        outfile << "No frequent 1-itemsets found." << endl;
    } else {
        for (const auto& itemset : Lk) {
            display(cout, itemset);
            cout << " - Support: " << freq_item_sp.at(itemset) * 100 << "%" << endl;
            display(outfile, itemset);
            outfile << " - Support: " << freq_item_sp.at(itemset) * 100 << "%" << endl;
        }
    }

    int k = 2;
    while (!Lk.empty()) {
        set<set<string>> Ck = candidate_gen(Lk, k);
        map<set<string>, int> Ck_counts;

        for (const auto& transaction : transactions) {
            for (const auto& candidate : Ck) {
                if (includes(transaction.begin(), transaction.end(),
                             candidate.begin(), candidate.end())) {
                    Ck_counts[candidate]++;
                }
            }
        }

        Lk.clear();
        for (const auto& pair : Ck_counts) {
            double support = static_cast<double>(pair.second) / tot_trans;
            if (support >= minisup) {
                Lk.insert(pair.first);
                freq_item_sp[pair.first] = support;
                freq_itemsets.insert(pair.first);
            }
        }
        
        cout << "\n--- Frequent " << k << "-itemsets ---" << endl;
        outfile << "\n--- Frequent " << k << "-itemsets ---" << endl;
        if (Lk.empty()) {
            cout << "No frequent " << k << "-itemsets found. Algorithm terminates." << endl;
            outfile << "No frequent " << k << "-itemsets found. Algorithm terminates." << endl;
            break;
        } else {
            for (const auto& itemset : Lk) {
                display(cout, itemset);
                cout << " - Support: " << freq_item_sp.at(itemset) * 100 << "%" << endl;
                display(outfile, itemset);
                outfile << " - Support: " << freq_item_sp.at(itemset) * 100 << "%" << endl;
            }
        }
        k++;
    }

    set<set<string>> maximal_freq_itemset;
    
    for (const auto& itemset : freq_itemsets) {
        bool is_maximal = true;
        for (const auto& larger_itemset : freq_itemsets) {
            if (itemset.size() < larger_itemset.size()) {
                if (includes(larger_itemset.begin(), larger_itemset.end(),
                             itemset.begin(), itemset.end())) {
                    is_maximal = false;
                    break;
                }
            }
        }
        if (is_maximal) {
            maximal_freq_itemset.insert(itemset);
        }
    }

    cout << "\n--- Maximal Frequent Itemsets ---" << endl;
    outfile << "\n--- Maximal Frequent Itemsets ---" << endl;
    if (maximal_freq_itemset.empty()) {
        cout << "No maximal frequent itemsets found." << endl;
        outfile << "No maximal frequent itemsets found." << endl;
    } else {
        for (const auto& itemset : maximal_freq_itemset) {
            display(cout, itemset);
            cout << " - Support: " << freq_item_sp.at(itemset) * 100 << "%" << endl;
            display(outfile, itemset);
            outfile << " - Support: " << freq_item_sp.at(itemset) * 100 << "%" << endl;
        }
    }

    cout << "\n--- Association Rules (One-Way) ---" << endl;
    outfile << "\n--- Association Rules (One-Way) ---" << endl;
    bool found_rules = false;

    for (const auto& itemset : freq_itemsets) {
        if (itemset.size() < 2) continue;

        set<set<string>> subsets = getSubsets(itemset);

        for (const auto& antecedent : subsets) {
            set<string> consequent;
            set_difference(itemset.begin(), itemset.end(),
                           antecedent.begin(), antecedent.end(),
                           inserter(consequent, consequent.begin()));

            if (consequent.empty()) continue;
            
            if (antecedent.size() > consequent.size()) {
                continue;
            }
            
            if (antecedent.size() == consequent.size()) {
                if (lexicographical_compare(consequent.begin(), consequent.end(), antecedent.begin(), antecedent.end())) {
                    continue;
                }
            }

            double support_itemset = freq_item_sp.at(itemset);
            double support_antecedent = freq_item_sp.at(antecedent);
            double confidence = support_itemset / support_antecedent;

            if (confidence >= min_confidence) {
                display(cout, antecedent);
                cout << " -> ";
                display(cout, consequent);
                cout << " - Confidence: " << confidence * 100 << "%" << endl;
                display(outfile, antecedent);
                outfile << " -> ";
                display(outfile, consequent);
                outfile << " - Confidence: " << confidence * 100 << "%" << endl;
                found_rules = true;
            }
        }
    }

    if (!found_rules) {
        cout << "No association rules found with the given minimum confidence." << endl;
        outfile << "No association rules found with the given minimum confidence." << endl;
    }

    outfile.close();

    return 0;
}


