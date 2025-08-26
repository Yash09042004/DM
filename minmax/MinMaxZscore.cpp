#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

using namespace std;

// Function to perform Min-Max Normalization
void minmax() {
    ifstream inFile("input_minmax.csv");
    if (!inFile) {
        cerr << "Error: Could not open input_minmax.csv" << endl;
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

// Function to perform Z-Score Normalization
void zscore() {
    ifstream inFile("input_zscore.csv");
    if (!inFile) {
        cerr << "Error: Could not open input_zscore.csv" << endl;
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
    double std_dev = sqrt(sq_sum / data.size());

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
    cout << "Z-Score Normalization complete. Check 'output_Zscore.csv'." << endl;
}

int main() {
    int option;
    cout << "Select a normalization technique:" << endl;
    cout << "1. Min-Max Normalization" << endl;
    cout << "2. Z-Score Normalization" << endl;
    cout << "Enter your choice (1 or 2): ";
    cin >> option;

    switch (option) {
        case 1:
            minmax();
            break;
        case 2:
            zscore();
            break;
        default:
            cout << "Invalid option. Please run the program again and select 1 or 2." << endl;
            break;
    }

    return 0;
}