#include<bits/stdc++.h>
using namespace std;

int main() {
    cout<< fixed << setprecision(2);
    map<string, map<string, int>> salesData;
    map<string, int> brandTotals;
    map<string, int> regionTotals;

    ifstream inputFile("input.csv");
    if (!inputFile.is_open()) {
        cout << "Error: Could not open input.csv" << endl;
        return 1;
    }

    string line;
    getline(inputFile, line);

    while (getline(inputFile, line)) {
        stringstream ss(line);
        string region, brand, sales_str;
        
        getline(ss, region, ',');
        getline(ss, brand, ',');
        getline(ss, sales_str, ',');
        
        int sales = stoi(sales_str);

        salesData[region][brand] += sales;
        brandTotals[brand] += sales;
        regionTotals[region] += sales;
    }
    inputFile.close();

    int grandTotal = 0;
    for (auto const& [region, total] : regionTotals) {
        grandTotal += total;
    }

    cout << "Sales Data with t-weight and d-weight" << endl;

    cout << "Brand\\Region";
    for (auto const& [region, total] : regionTotals) {
        cout << "\t\t" << region << "\t\t";
    }
    cout << "\t\tTotal" << endl;

    for (auto const& [brand, brand_total] : brandTotals) {
        cout << brand;
        for (auto const& [region, region_total] : regionTotals) {
            float currentSales = salesData.count(region) && salesData[region].count(brand) ? salesData[region][brand] : 0;
            
            float t_weight = (region_total > 0) ? (currentSales / region_total) * 100 : 0;
            float d_weight = (brand_total > 0) ? (currentSales / brand_total) * 100 : 0;

            cout << "\t\t" << currentSales << " (" << t_weight << "%, " << d_weight << "%)";
        }
        float total_t_weight = (grandTotal > 0) ? ((float)brand_total / grandTotal) * 100 : 0;
        cout << "\t\t" << brand_total << " (" << total_t_weight << "%, 100%)" << endl;
    }

    cout << "Total";
    for (auto const& [region, region_total] : regionTotals) {
        float total_d_weight = (grandTotal > 0) ? ((float)region_total / grandTotal) * 100 : 0;
        cout << "         " << region_total << " (100%, " << total_d_weight << "%)"<< "        ";
    }
    cout << "\t" << grandTotal << " (100%, 100%)" << endl;

    return 0;
}