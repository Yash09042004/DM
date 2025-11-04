#include<bits/stdc++.h>

using namespace std;

int main() {
    // 1. DATA STRUCTURES
    map<string, map<string, int>> salesData; // map<Region, map<Brand, Sales>>
    map<string, int> brandTotals;
    map<string, int> regionTotals;

    // 2. FILE READING AND DATA AGGREGATION
    ifstream inputFile("input.csv");
    if (!inputFile.is_open()) {
        cout << "Error: Could not open input.csv" << endl;
        return 1;
    }

    string line;
    getline(inputFile, line); // Skip header

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

    // 3. CALCULATE GRAND TOTAL
    int grandTotal = 0;
    for (auto const& [region, total] : regionTotals) {
        grandTotal += total;
    }

    // 4. PRINT FORMATTED TABLE TO TERMINAL
    cout << "\n--- Sales Data with t-weight and d-weight ---\n\n";
    ofstream outFile("output.csv");
    // Print Header Row
    cout << left << setw(15) << "Brand\\Region";
    outFile << "Brand/Region";
    for (auto const& [region, total] : regionTotals) {
        cout << "| " << left << setw(28) << region;
        outFile << "," << region;
    }
    cout << "| " << left << setw(28) << "Total" << endl;
    outFile << ",Total" << endl;

    cout << left << setw(15) << "";
    outFile << ",";
    for (auto const& [region, total] : regionTotals) {
        cout << "| " << left << setw(8) << "Sales" << setw(11) << "t-weight" << setw(9) << "d-weight";
        outFile << "Sales,t-weight,d-weight,";
    }
    cout << "| " << left << setw(8) << "Sales" << setw(11) << "t-weight" << setw(9) << "d-weight" << endl;
    outFile << "Sales,t-weight,d-weight" << endl;
    cout << string(105, '-') << endl;
    outFile << string(105, '-') << endl;

    // Print Data Rows (for each Brand)
    for (auto const& [brand, total] : brandTotals) {
        cout << left << setw(15) << brand; // Brand name
        outFile << brand;
        for (auto const& [region, reg_total] : regionTotals) {
            float currentSales = salesData.count(region) && salesData[region].count(brand) ? salesData[region][brand] : 0;
            float t_weight = (brandTotals[brand] > 0) ? (currentSales / brandTotals[brand]) * 100 : 0;
            float d_weight = (regionTotals[region] > 0) ? (currentSales / regionTotals[region]) * 100 : 0;
            cout << "| " << left << setw(8) << fixed << setprecision(2) << currentSales
                 << setw(10) << fixed << setprecision(2) << t_weight << "%"
                 << setw(9) << fixed << setprecision(2) << d_weight << "%";
            outFile << "," << currentSales << "," << t_weight << "," << d_weight;
        }
        float total_t_weight = (grandTotal > 0) ? ((float)brandTotals[brand] / grandTotal) * 100 : 0;
        cout << "| " << left << setw(8) << fixed << setprecision(2) << (float)brandTotals[brand]
             << setw(10) << fixed << setprecision(2) << total_t_weight << "%"
             << setw(9) << "100.00%" // d-weight for the brand's total is always 100%
             << endl;
        outFile << "," << brandTotals[brand] << "," << total_t_weight << ",100.00" << endl;
    }
    cout << string(105, '-') << endl;
    outFile << string(105, '-') << endl;

    // Print Final Total Row
    cout << left << setw(15) << "Total";
    outFile << "Total";
    for (auto const& [region, total] : regionTotals) {
        float total_d_weight = (grandTotal > 0) ? ((float)total / grandTotal) * 100 : 0;
        cout << "| " << left << setw(8) << fixed << setprecision(2) << (float)total
             << setw(10) << "100.00%"
             << setw(9) << fixed << setprecision(2) << total_d_weight << "%";
        outFile << "," << total << ",100.00," << total_d_weight;
    }
    cout << "| " << left << setw(8) << fixed << setprecision(2) << (float)grandTotal
         << setw(10) << "100.00%"
         << setw(9) << "100.00%"
         << endl;
    outFile << "," << grandTotal << ",100.00,100.00" << endl;
    outFile << endl;

    // 2D Aggregations
    outFile << "\n[Region Totals]\nRegion,Total\n";
    for (auto const& [region, total] : regionTotals) {
        outFile << region << "," << total << endl;
    }
    outFile << endl;
    outFile << "[Brand Totals]\nBrand,Total\n";
    for (auto const& [brand, total] : brandTotals) {
        outFile << brand << "," << total << endl;
    }
    outFile << endl;

    // 1D Aggregation
    outFile << "[Grand Total]\nTotal," << grandTotal << endl;
    outFile.close();

    return 0;
}