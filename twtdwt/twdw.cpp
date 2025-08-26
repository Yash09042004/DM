#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip> // For setprecision, setw

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
    cout << "\n--- Sales Data with Corrected t-weight and d-weight ---\n\n";

    // Print Header Row
    cout << left << setw(15) << "Brand\\Region";
    for (auto const& [region, total] : regionTotals) {
        cout << "| " << left << setw(28) << region;
    }
    cout << "| " << left << setw(28) << "Total" << endl;

    cout << left << setw(15) << "";
    for (auto const& [region, total] : regionTotals) {
        cout << "| " << left << setw(8) << "Sales" << setw(11) << "t-weight" << setw(9) << "d-weight";
    }
    cout << "| " << left << setw(8) << "Sales" << setw(11) << "t-weight" << setw(9) << "d-weight" << endl;
    cout << string(105, '-') << endl;

    // Print Data Rows (for each Brand)
    for (auto const& [brand, brand_total] : brandTotals) {
        cout << left << setw(15) << brand; // Brand name
        for (auto const& [region, region_total] : regionTotals) {
            float currentSales = salesData.count(region) && salesData[region].count(brand) ? salesData[region][brand] : 0;
            
            // CORRECTED t-weight (Tuple Weight): % of the Region's total sales from this Brand (COLUMN-WISE %)
            float t_weight = (region_total > 0) ? (currentSales / region_total) * 100 : 0;
            
            // CORRECTED d-weight (Discriminant Weight): % of the Brand's total sales that came from this Region (ROW-WISE %)
            float d_weight = (brand_total > 0) ? (currentSales / brand_total) * 100 : 0;

            cout << "| " << left << setw(8) << fixed << setprecision(2) << currentSales
                 << setw(10) << fixed << setprecision(2) << t_weight << "%"
                 << setw(9) << fixed << setprecision(2) << d_weight << "%";
        }
        // Row totals
        float total_t_weight = (grandTotal > 0) ? ((float)brand_total / grandTotal) * 100 : 0;
        cout << "| " << left << setw(8) << fixed << setprecision(2) << (float)brand_total
             << setw(10) << fixed << setprecision(2) << total_t_weight << "%"
             << setw(9) << "100.00%" // d-weight for the brand's total is always 100%
             << endl;
    }
    cout << string(105, '-') << endl;

    // Print Final Total Row
    cout << left << setw(15) << "Total";
    for (auto const& [region, region_total] : regionTotals) {
        float total_d_weight = (grandTotal > 0) ? ((float)region_total / grandTotal) * 100 : 0;
        cout << "| " << left << setw(8) << fixed << setprecision(2) << (float)region_total
             << setw(10) << "100.00%" // t-weight for the region's total is always 100%
             << setw(9) << fixed << setprecision(2) << total_d_weight << "%";
    }
    // Grand total cell
    cout << "| " << left << setw(8) << fixed << setprecision(2) << (float)grandTotal
         << setw(10) << "100.00%"
         << setw(9) << "100.00%"
         << endl;

    return 0;
}