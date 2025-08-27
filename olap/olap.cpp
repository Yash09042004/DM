#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <limits>

using namespace std;

// Struct to hold the measures (numerical data)
struct Measures {
    int ise = 0, mse = 0, ese = 0, total = 0;
};

// Global variables to define the dimensions of our cube
set<string> years;
set<string> semesters;
set<string> subjects;

// The Data Cube
map<string, Measures> dataCube;

// Function to create a unique key for the map
string getKey(const string& year, const string& semester, const string& subject) {
    return year + "|" + semester + "|" + subject;
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

        years.insert(year);
        semesters.insert(semester);
        subjects.insert(subject);

        string key = getKey(year, semester, subject);
        dataCube[key] = {ise, mse, ese, total};
    }

    file.close();
    return true;
}

// OLAP Operation: Roll-Up
void rollUp() {
    cout << "\n--- Roll-Up: Total Marks by Year ---\n";
    map<string, int> yearTotals;
    for (const auto& pair : dataCube) {
        string year = pair.first.substr(0, pair.first.find('|'));
        yearTotals[year] += pair.second.total;
    }
    for (const auto& pair : yearTotals) {
        cout << "Year: " << pair.first << " -> Total Marks: " << pair.second << "\n";
    }
}

// OLAP Operation: Drill-Down
void drillDown(const string& year) {
    cout << "\n--- Drill-Down: Subjects in Year '" << year << "' ---\n";
    cout << "Subject\t\tSemester\tYear\tISE\tMSE\tESE\tTotal\n";
    for (const auto& s : subjects) {
        for (const auto& sem : semesters) {
            string key = getKey(year, sem, s);
            if (dataCube.count(key)) {
                Measures m = dataCube[key];
                cout << s << "\t\t" << sem << "\t\t" << year << "\t"
                     << m.ise << "\t" << m.mse << "\t" << m.ese << "\t" << m.total << "\n";
            }
        }
    }
}

// OLAP Operation: Slice
void slice(const string& subject) {
    cout << "\n--- Slice: Records for Subject '" << subject << "' ---\n";
    cout << "Subject\t\tSemester\tYear\tISE\tMSE\tESE\tTotal\n";
    for (const auto& y : years) {
        for (const auto& sem : semesters) {
            string key = getKey(y, sem, subject);
            if (dataCube.count(key)) {
                Measures m = dataCube[key];
                cout << subject << "\t\t" << sem << "\t\t" << y << "\t"
                     << m.ise << "\t" << m.mse << "\t" << m.ese << "\t" << m.total << "\n";
            }
        }
    }
}

// OLAP Operation: Dice
void dice(const string& year, const string& semester) {
    cout << "\n--- Dice: Records for Year '" << year << "' AND Semester '" << semester << "' ---\n";
    cout << "Subject\t\tSemester\tYear\tISE\tMSE\tESE\tTotal\n";
    for (const auto& s : subjects) {
        string key = getKey(year, semester, s);
        if (dataCube.count(key)) {
            Measures m = dataCube[key];
            cout << s << "\t\t" << semester << "\t\t" << year << "\t"
                 << m.ise << "\t" << m.mse << "\t" << m.ese << "\t" << m.total << "\n";
        }
    }
}

void showMenu() {
    cout << "\n---------- OLAP Operations Menu ----------\n";
    cout << "1. Roll-Up (Aggregate total marks by year)\n";
    cout << "2. Drill-Down (Show details for a specific year)\n";
    cout << "3. Slice (Filter by a single subject)\n";
    cout << "4. Dice (Filter by year and semester)\n";
    cout << "5. Exit\n";
    cout << "------------------------------------------\n";
    cout << "Enter your choice: ";
}

int main() {
    if (!buildDataCube("student_marks.csv")) {
        return 1;
    }
    cout << "Data Cube built successfully from 'student_marks.csv'.\n";

    int choice;
    string year, semester, subject;

    while (true) {
        showMenu();
        cin >> choice;

        // Clear the input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

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
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}