#include <bits/stdc++.h>
using namespace std;

void decimal_scaling(const string &inputFileName) {
    ifstream inFile(inputFileName);
    if (!inFile) {
        cerr << "Error: Could not open " << inputFileName << endl;
        return;
    }

    vector<double> data;
    double value;
    while (inFile >> value) {
        data.push_back(value);
    }
    inFile.close();

    if (data.empty()) {
        cerr << "Error: Input file is empty." << endl;
        return;
    }

    double max_abs = 0.0;
    for (double val : data) {
        if (fabs(val) > max_abs) {
            max_abs = fabs(val);
        }
    }
    int j = 0;
    while (max_abs >= 1) {
        max_abs /= 10;
        j++;
    }

    ofstream outFile("output.csv");
    if (!outFile) {
        cerr << "Error: Could not create output.csv" << endl;
        return;
    }

    outFile << "Original_Data,Normalized_Data" << endl;
    for (double val : data) {
        double normalized_val = val / pow(10, j);
        outFile << val << "," << normalized_val << endl;
    }
    outFile.close();

    cout << "Decimal Scaling Normalization complete. Check 'output.csv'." << endl;
}

void minmax(const string &inputFileName) {
    ifstream inFile(inputFileName);
    if (!inFile) {
        cerr << "Error: Could not open " << inputFileName << endl;
        return;
    }

    vector<double> data;
    double value;
    while (inFile >> value) {
        data.push_back(value);
    }
    inFile.close();

    if (data.empty()) {
        cerr << "Error: Input file is empty." << endl;
        return;
    }

    double min_val = *min_element(data.begin(), data.end());
    double max_val = *max_element(data.begin(), data.end());

    double new_min, new_max;
    cout << "Enter the new minimum value: ";
    cin >> new_min;
    cout << "Enter the new maximum value: ";
    cin >> new_max;

    ofstream outFile("output_minmax.csv");
    if (!outFile) {
        cerr << "Error: Could not create output_minmax.csv" << endl;
        return;
    }

    outFile << "Original_Data,Normalized_Data" << endl;
    for (double val : data) {
        double normalized_val = (((val - min_val) / (max_val - min_val)) * (new_max - new_min)) + new_min;
        outFile << val << "," << normalized_val << endl;
    }
    outFile.close();

    cout << "Min-Max Normalization complete. Check 'output_minmax.csv'." << endl;
}

void zscore(const string &inputFileName) {
    ifstream inFile(inputFileName);
    if (!inFile) {
        cerr << "Error: Could not open " << inputFileName << endl;
        return;
    }

    vector<double> data;
    double value;
    double sum = 0.0;
    while (inFile >> value) {
        data.push_back(value);
        sum += value;
    }
    inFile.close();

    if (data.empty()) {
        cerr << "Error: Input file is empty." << endl;
        return;
    }

    double mean = sum / data.size();
    double sq_sum = 0.0;
    for (double val : data) {
        sq_sum += (val - mean) * (val - mean);
    }
    double std_dev = sqrt(sq_sum / data.size()-1);

    ofstream outFile("output_zscore.csv");
    if (!outFile) {
        cerr << "Error: Could not create output_zscore.csv" << endl;
        return;
    }

    outFile << "Original_Data,Normalized_Data" << endl;
    for (double val : data) {
        double normalized_val = (val - mean) / std_dev;
        outFile << val << "," << normalized_val << endl;
    }
    outFile.close();

    cout << "Z-Score Normalization complete. Check 'output_zscore.csv'." << endl;
}

int main() {
    int option;
    string inputFileName;

    cout << "Select a normalization technique:" << endl;
    cout << "1. Min-Max Normalization" << endl;
    cout << "2. Z-Score Normalization" << endl;
    cout << "3. Decimal Scaling Normalization" << endl;
    cout << "Enter your choice (1, 2 or 3): ";
    cin >> option;
    cin.ignore(); // clear newline left by cin

    cout << "Enter input filename: ";
    getline(cin, inputFileName);

    switch (option) {
        case 1:
            minmax(inputFileName);
            break;
        case 2:
            zscore(inputFileName);
            break;
        case 3:
            decimal_scaling(inputFileName);
            break;
        default:
            cout << "Invalid option. Please run the program again and select 1, 2, or 3." << endl;
            break;
    }

    return 0;
}
