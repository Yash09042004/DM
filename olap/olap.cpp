#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <limits>
#include <iomanip>

using namespace std;

// Struct to hold the measures (numerical data)
struct Measures {
    int ise = 0, mse = 0, ese = 0, total = 0;
};

// --- Data Structure representing the 3D Cube ---
// A nested map to represent Year -> Semester -> Subject -> Measures
map<string, map<string, map<string, Measures>>> dataCube;

// Global variables to hold the distinct dimension values
set<string> years;
set<string> semesters;
set<string> subjects;


// --- Helper function to print a "slice" of the cube ---
void printCubeSlice(const map<string, map<string, Measures>>& slice, const string& sliceName) {
    cout << "\n--- Cube Slice for Year: " << sliceName << " ---\n";

    // Print header
    cout << left << setw(12) << "Semester";
    for (const auto& subject : subjects) {
        cout << setw(10) << subject;
    }
    cout << "\n" << string(12 + subjects.size() * 10, '-') << "\n";

    // Print data rows
    for (const auto& sem : semesters) {
        cout << left << setw(12) << sem;
        for (const auto& sub : subjects) {
            if (slice.count(sem) && slice.at(sem).count(sub)) {
                cout << setw(10) << slice.at(sem).at(sub).total;
            } else {
                cout << setw(10) << "N/A";
            }
        }
        cout << "\n";
    }
}


// Function to load data from CSV and build the data cube
bool buildDataCube(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << "\n";
        return false;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string subject, semester, year, item;
        int ise, mse, ese, total;

        getline(ss, subject, ',');
        getline(ss, semester, ',');
        getline(ss, year, ',');
        getline(ss, item, ','); ise = stoi(item);
        getline(ss, item, ','); mse = stoi(item);
        getline(ss, item, ','); ese = stoi(item);
        getline(ss, item, ','); total = stoi(item);

        // Populate dimension sets
        years.insert(year);
        semesters.insert(semester);
        subjects.insert(subject);

        // Insert data into the nested map structure
        dataCube[year][semester][subject] = {ise, mse, ese, total};
    }

    file.close();
    return true;
}

// --- OLAP Operations with 3D-like Representation ---

// OLAP Operation: Roll-Up (from Semesters and Subjects to Year)
void rollUp() {
    cout << "\n--- Roll-Up: Total Marks by Year (Aggregating Semesters & Subjects) ---\n";
    map<string, int> yearTotals;
    for (const auto& year_pair : dataCube) {
        int currentYearTotal = 0;
        for (const auto& sem_pair : year_pair.second) {
            for (const auto& sub_pair : sem_pair.second) {
                currentYearTotal += sub_pair.second.total;
            }
        }
        yearTotals[year_pair.first] = currentYearTotal;
    }

    cout << "\nYear       | Total Marks\n";
    cout << "-----------|-------------\n";
    for (const auto& pair : yearTotals) {
        cout << left << setw(11) << pair.first << "| " << pair.second << "\n";
    }
}

// OLAP Operation: Drill-Down (from Year to Semesters and Subjects)
void drillDown(const string& year) {
    cout << "\n--- Drill-Down: Viewing details for Year '" << year << "' ---\n";
    if (dataCube.count(year)) {
        printCubeSlice(dataCube[year], year);
    } else {
        cout << "No data found for year: " << year << "\n";
    }
}

// OLAP Operation: Slice (by a specific subject)
void slice(const string& subject) {
    cout << "\n--- Slice: Data for Subject '" << subject << "' ---\n";
    cout << left << setw(12) << "Year" << setw(12) << "Semester" << "Total Marks\n";
    cout << string(36, '-') << "\n";

    for (const auto& year : years) {
        for (const auto& sem : semesters) {
            if (dataCube.count(year) && dataCube[year].count(sem) && dataCube[year][sem].count(subject)) {
                cout << left << setw(12) << year << setw(12) << sem
                     << dataCube[year][sem][subject].total << "\n";
            }
        }
    }
}

// OLAP Operation: Dice (by year and semester)
void dice(const string& year, const string& semester) {
    cout << "\n--- Dice: Records for Year '" << year << "' AND Semester '" << semester << "' ---\n";
    if (dataCube.count(year) && dataCube[year].count(semester)) {
        cout << left << setw(12) << "Subject" << "Total Marks\n";
        cout << string(24, '-') << "\n";
        for (const auto& sub_pair : dataCube[year][semester]) {
            cout << left << setw(12) << sub_pair.first << sub_pair.second.total << "\n";
        }
    } else {
        cout << "No data found for the specified year and semester.\n";
    }
}

// OLAP Operation: Cube (Full Aggregation across all dimensions)
void cubeOperation() {
    cout << "\n--- Cube Operation (Aggregation across all dimensions) ---\n";

    // (Year, Semester, Subject)
    cout << "\n[Year, Semester, Subject] Aggregation:\n";
    for (const auto& year : years) {
        for (const auto& sem : semesters) {
            for (const auto& sub : subjects) {
                if (dataCube.count(year) && dataCube[year].count(sem) && dataCube[year][sem].count(sub)) {
                    cout << "Year=" << year << ", Semester=" << sem << ", Subject=" << sub
                         << " -> Total=" << dataCube[year][sem][sub].total << "\n";
                }
            }
        }
    }

    // (Year, Semester)
    cout << "\n[Year, Semester] Aggregation:\n";
    for (const auto& year : years) {
        for (const auto& sem : semesters) {
            int sum = 0;
            if (dataCube.count(year) && dataCube[year].count(sem)) {
                for (const auto& sub : dataCube[year][sem]) {
                    sum += sub.second.total;
                }
                cout << "Year=" << year << ", Semester=" << sem << " -> Total=" << sum << "\n";
            }
        }
    }

    // (Year)
    cout << "\n[Year] Aggregation:\n";
    for (const auto& year : years) {
        int sum = 0;
        if (dataCube.count(year)) {
            for (const auto& sem : dataCube[year]) {
                for (const auto& sub : sem.second) {
                    sum += sub.second.total;
                }
            }
            cout << "Year=" << year << " -> Total=" << sum << "\n";
        }
    }

    // (Semester)
    cout << "\n[Semester] Aggregation:\n";
    for (const auto& sem : semesters) {
        int sum = 0;
        for (const auto& year : years) {
            if (dataCube.count(year) && dataCube[year].count(sem)) {
                for (const auto& sub : dataCube[year][sem]) {
                    sum += sub.second.total;
                }
            }
        }
        cout << "Semester=" << sem << " -> Total=" << sum << "\n";
    }

    // (Subject)
    cout << "\n[Subject] Aggregation:\n";
    for (const auto& sub : subjects) {
        int sum = 0;
        for (const auto& year : years) {
            for (const auto& sem : semesters) {
                if (dataCube.count(year) && dataCube[year].count(sem) && dataCube[year][sem].count(sub)) {
                    sum += dataCube[year][sem][sub].total;
                }
            }
        }
        cout << "Subject=" << sub << " -> Total=" << sum << "\n";
    }

    // Grand Total
    int grandTotal = 0;
    for (const auto& year : years) {
        for (const auto& sem : semesters) {
            for (const auto& sub : subjects) {
                if (dataCube.count(year) && dataCube[year].count(sem) && dataCube[year][sem].count(sub)) {
                    grandTotal += dataCube[year][sem][sub].total;
                }
            }
        }
    }
    cout << "\n[Grand Total] Aggregation:\nTotal Marks = " << grandTotal << "\n";
}


void showMenu() {
    cout << "\n---------- 3D OLAP Operations Menu ----------\n";
    cout << "1. Roll-Up (Aggregate total marks by year)\n";
    cout << "2. Drill-Down (Show details for a specific year)\n";
    cout << "3. Slice (Filter by a single subject)\n";
    cout << "4. Dice (Filter by year and semester)\n";
    cout << "5. Cube (Full Aggregation)\n";
    cout << "6. Exit\n";
    cout << "------------------------------------------\n";
    cout << "Enter your choice: ";
}

int main() {
    string filename;
    cout << "Enter CSV filename: ";
    getline(cin, filename);

    if (!buildDataCube(filename)) {
        return 1;
    }
    cout << "Data Cube built successfully from '" << filename << "'.\n";

    int choice;
    string year, semester, subject;

    while (true) {
        showMenu();
        cin >> choice;

        if(cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the rest of the line

        switch (choice) {
            case 1:
                rollUp();
                break;
            case 2:
                cout << "Enter Year to drill down into (e.g., First, Second): ";
                getline(cin, year);
                drillDown(year);
                break;
            case 3:
                cout << "Enter Subject to slice by (e.g., Maths, Phys, Chem): ";
                getline(cin, subject);
                slice(subject);
                break;
            case 4:
                cout << "Enter Year (e.g., First, Second): ";
                getline(cin, year);
                cout << "Enter Semester (e.g., I, II, III, IV): ";
                getline(cin, semester);
                dice(year, semester);
                break;
            case 5:
                cubeOperation();
                break;
            case 6:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
