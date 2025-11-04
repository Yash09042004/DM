#include <bits/stdc++.h>
using namespace std;

struct Measures {
    int ise = 0, mse = 0, ese = 0, total = 0;
};

map<string, map<string, map<string, Measures>>> dataCube;
set<string> years;
set<string> semesters;
set<string> subjects;

void printCubeSlice(const map<string, map<string, Measures>>& slice, const string& sliceName) {
    cout << "\n--- Cube Slice for Year: " << sliceName << " ---\n";
    cout << left << setw(12) << "Semester";
    for (const auto& subject : subjects) cout << setw(10) << subject;
    cout << "\n" << string(12 + subjects.size() * 10, '-') << "\n";
    for (const auto& sem : semesters) {
        cout << left << setw(12) << sem;
        for (const auto& sub : subjects) {
            if (slice.count(sem) && slice.at(sem).count(sub)) cout << setw(10) << slice.at(sem).at(sub).total;
            else cout << setw(10) << "N/A";
        }
        cout << "\n";
    }
}

bool buildDataCube(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << "\n";
        return false;
    }

    string line;
    getline(file, line);

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
        dataCube[year][semester][subject] = {ise, mse, ese, total};
    }

    file.close();
    return true;
}

void rollUp() {
    ofstream outFile("output.csv");
    cout << "\n--- Roll-Up: Total Marks by Year ---\n";
    map<string, int> yearTotals;
    for (const auto& y : dataCube) {
        int total = 0;
        for (const auto& s : y.second)
            for (const auto& sub : s.second)
                total += sub.second.total;
        yearTotals[y.first] = total;
    }
    cout << "\nYear       | Total Marks\n";
    cout << "-----------|-------------\n";
    outFile << "Year,Total Marks\n";
    for (const auto& p : yearTotals) {
        cout << left << setw(11) << p.first << "| " << p.second << "\n";
        outFile << p.first << "," << p.second << "\n";
    }
    outFile.close();
}

void drillDown(const string& year) {
    ofstream outFile("output.csv");
    cout << "\n--- Drill-Down: Year '" << year << "' ---\n";
    if (dataCube.count(year)) {
        outFile << "Semester";
        for (const auto& s : subjects) outFile << "," << s;
        outFile << "\n";
        for (const auto& sem : semesters) {
            outFile << sem;
            for (const auto& sub : subjects) {
                if (dataCube[year].count(sem) && dataCube[year][sem].count(sub))
                    outFile << "," << dataCube[year][sem][sub].total;
                else outFile << ",N/A";
            }
            outFile << "\n";
        }
        printCubeSlice(dataCube[year], year);
    } else {
        cout << "No data found for " << year << "\n";
        outFile << "No data found for " << year << "\n";
    }
    outFile.close();
}

void sliceOperation(const string& subject) {
    ofstream outFile("output.csv");
    cout << "\n--- Slice: Subject '" << subject << "' ---\n";
    cout << left << setw(12) << "Year" << setw(12) << "Semester" << "Total Marks\n";
    cout << string(36, '-') << "\n";
    outFile << "Year,Semester,Total Marks\n";
    for (const auto& y : years) {
        for (const auto& sem : semesters) {
            if (dataCube.count(y) && dataCube[y].count(sem) && dataCube[y][sem].count(subject)) {
                cout << left << setw(12) << y << setw(12) << sem << dataCube[y][sem][subject].total << "\n";
                outFile << y << "," << sem << "," << dataCube[y][sem][subject].total << "\n";
            }
        }
    }
    outFile.close();
}

void dice(const string& year, const string& semester) {
    ofstream outFile("output.csv");
    cout << "\n--- Dice: Year '" << year << "', Semester '" << semester << "' ---\n";
    if (dataCube.count(year) && dataCube[year].count(semester)) {
        cout << left << setw(12) << "Subject" << "Total Marks\n";
        cout << string(24, '-') << "\n";
        outFile << "Subject,Total Marks\n";
        for (const auto& sub : dataCube[year][semester]) {
            cout << left << setw(12) << sub.first << sub.second.total << "\n";
            outFile << sub.first << "," << sub.second.total << "\n";
        }
    } else {
        cout << "No data found for this selection.\n";
        outFile << "No data found for this selection.\n";
    }
    outFile.close();
}

void cubeOperation() {
    ofstream outFile("output.csv");
    cout << "\n--- Cube Operation (Full Aggregation) ---\n";

    // [Year, Semester, Subject]
    outFile << "[Year,Semester,Subject] Aggregation\n";
    outFile << "Year,Semester,Subject,Total\n";
    for (const auto& y : years) {
        for (const auto& sem : semesters) {
            for (const auto& sub : subjects) {
                if (dataCube.count(y) && dataCube[y].count(sem) && dataCube[y][sem].count(sub)) {
                    int t = dataCube[y][sem][sub].total;
                    cout << "Year=" << y << ", Semester=" << sem << ", Subject=" << sub << " -> " << t << "\n";
                    outFile << y << "," << sem << "," << sub << "," << t << "\n";
                }
            }
        }
    }

    // [Year, Semester]
    outFile << "\n[Year,Semester] Aggregation\n";
    outFile << "Year,Semester,Total\n";
    for (const auto& y : years) {
        for (const auto& sem : semesters) {
            int sum = 0;
            if (dataCube.count(y) && dataCube[y].count(sem)) {
                for (const auto& sub : dataCube[y][sem]) {
                    sum += sub.second.total;
                }
                cout << "Year=" << y << ", Semester=" << sem << " -> Total=" << sum << "\n";
                outFile << y << "," << sem << "," << sum << "\n";
            }
        }
    }

    // [Year, Subject]
    outFile << "\n[Year,Subject] Aggregation\n";
    outFile << "Year,Subject,Total\n";
    for (const auto& y : years) {
        for (const auto& sub : subjects) {
            int sum = 0;
            if (dataCube.count(y)) {
                for (const auto& sem : semesters) {
                    if (dataCube[y].count(sem) && dataCube[y][sem].count(sub)) {
                        sum += dataCube[y][sem][sub].total;
                    }
                }
                cout << "Year=" << y << ", Subject=" << sub << " -> Total=" << sum << "\n";
                outFile << y << "," << sub << "," << sum << "\n";
            }
        }
    }

    // [Semester, Subject]
    outFile << "\n[Semester,Subject] Aggregation\n";
    outFile << "Semester,Subject,Total\n";
    for (const auto& sem : semesters) {
        for (const auto& sub : subjects) {
            int sum = 0;
            for (const auto& y : years) {
                if (dataCube[y].count(sem) && dataCube[y][sem].count(sub)) {
                    sum += dataCube[y][sem][sub].total;
                }
            }
            cout << "Semester=" << sem << ", Subject=" << sub << " -> Total=" << sum << "\n";
            outFile << sem << "," << sub << "," << sum << "\n";
        }
    }

    // [Year]
    outFile << "\n[Year] Aggregation\n";
    outFile << "Year,Total\n";
    for (const auto& y : years) {
        int sum = 0;
        if (dataCube.count(y)) {
            for (const auto& sem : dataCube[y]) {
                for (const auto& sub : sem.second) {
                    sum += sub.second.total;
                }
            }
            cout << "Year=" << y << " -> Total=" << sum << "\n";
            outFile << y << "," << sum << "\n";
        }
    }

    // [Semester]
    outFile << "\n[Semester] Aggregation\n";
    outFile << "Semester,Total\n";
    for (const auto& sem : semesters) {
        int sum = 0;
        for (const auto& y : years) {
            if (dataCube[y].count(sem)) {
                for (const auto& sub : dataCube[y][sem]) {
                    sum += sub.second.total;
                }
            }
        }
        cout << "Semester=" << sem << " -> Total=" << sum << "\n";
        outFile << sem << "," << sum << "\n";
    }

    // [Subject]
    outFile << "\n[Subject] Aggregation\n";
    outFile << "Subject,Total\n";
    for (const auto& sub : subjects) {
        int sum = 0;
        for (const auto& y : years) {
            for (const auto& sem : semesters) {
                if (dataCube[y].count(sem) && dataCube[y][sem].count(sub)) {
                    sum += dataCube[y][sem][sub].total;
                }
            }
        }
        cout << "Subject=" << sub << " -> Total=" << sum << "\n";
        outFile << sub << "," << sum << "\n";
    }

    // [Grand Total]
    int grandTotal = 0;
    for (const auto& y : years) {
        for (const auto& sem : semesters) {
            for (const auto& sub : subjects) {
                if (dataCube[y].count(sem) && dataCube[y][sem].count(sub)) {
                    grandTotal += dataCube[y][sem][sub].total;
                }
            }
        }
    }
    cout << "\n[Grand Total] = " << grandTotal << "\n";
    outFile << "\n[Grand Total] Aggregation\nTotal," << grandTotal << "\n";
    outFile.close();
}

void showMenu() {
    cout << "\n---------- 3D OLAP Menu ----------\n";
    cout << "1. Roll-Up\n";
    cout << "2. Drill-Down\n";
    cout << "3. Slice\n";
    cout << "4. Dice\n";
    cout << "5. Cube\n";
    cout << "6. Exit\n";
    cout << "----------------------------------\n";
    cout << "Enter choice: ";
}

int main() {
    string filename;
    cout << "Enter CSV filename: ";
    getline(cin, filename);
    if (!buildDataCube(filename)) return 1;
    cout << "Data Cube built successfully from '" << filename << "'.\n";
    int choice;
    string year, semester, subject;
    while (true) {
        showMenu();
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (choice) {
            case 1: rollUp(); break;
            case 2: cout << "Enter Year: "; getline(cin, year); drillDown(year); break;
            case 3: cout << "Enter Subject: "; getline(cin, subject); sliceOperation(subject); break;
            case 4: cout << "Enter Year: "; getline(cin, year); cout << "Enter Semester: "; getline(cin, semester); dice(year, semester); break;
            case 5: cubeOperation(); break;
            case 6: cout << "Exiting.\n"; return 0;
            default: cout << "Invalid choice.\n";
        }
    }
}
