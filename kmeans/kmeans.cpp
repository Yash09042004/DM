#include<bits/stdc++.h>

std::random_device rd;
std::mt19937 g(rd());

using namespace std;

using Point = vector<double>;

double euclidean_distance(const Point& p1, const Point& p2) {
    double sum_sq = 0.0;
    for (size_t i = 0; i < p1.size(); ++i) {
        sum_sq += pow(p1[i] - p2[i], 2);
    }
    return sqrt(sum_sq);
}

vector<int> kmeans(const vector<Point>& data, int k, int max_iterations = 50) {
    if (data.empty() || k <= 0 || k > data.size()) {
        cerr << "Error: Invalid data or K value." << endl;
        return {};
    }

    int n = data.size();
    int d = data[0].size(); 
    vector<Point> centroids(k, Point(d));
    vector<int> assignments(n);
    
    srand(time(0));
    vector<int> indices(n);
    for (int i = 0; i < n; ++i) indices[i] = i;
    shuffle(indices.begin(), indices.end(), g); 
    
    for (int i = 0; i < k; ++i) {
        centroids[i] = data[indices[i]];
    }

    cout << "\nStarting K-means with K=" << k << " and " << d << " features...\n";
    cout << fixed << setprecision(4);

    for (int iter = 1; iter <= max_iterations; ++iter) {
        bool converged = false;
        
        cout << "\n--- Iteration " << iter << " ---\n";

        cout << "Current Centroids:\n";
        for (int j = 0; j < k; ++j) {
            cout << "  C" << j << ": "; 
            for (double v : centroids[j]) cout << v << " "; 
            cout << endl;
        }
        
        for (int i = 0; i < n; ++i) {
            double min_dist = numeric_limits<double>::max();
            int best_centroid = -1;

            for (int j = 0; j < k; ++j) {
                double dist = euclidean_distance(data[i], centroids[j]);
                if (dist < min_dist) {
                    min_dist = dist;
                    best_centroid = j;
                }
            }

            if (assignments[i] != best_centroid) {
                assignments[i] = best_centroid;
                converged = true;
            }
        }
        
        if (!converged && iter > 1) {
            cout << "\n Convergence occurred no distances changed.\n";
            break;
        }

        vector<Point> newcent(k, Point(d, 0.0));
        vector<int> counts(k, 0);

        for (int i = 0; i < n; ++i) {
            int cluster_idx = assignments[i];
            counts[cluster_idx]++;
            for (int j = 0; j < d; ++j) {
                newcent[cluster_idx][j] += data[i][j];
            }
        }
        
        for (int j = 0; j < k; ++j) {
            if (counts[j] > 0) {
                for (int l = 0; l < d; ++l) {
                    newcent[j][l] /= counts[j];
                }
                centroids[j] = newcent[j];
            } else {
                centroids[j] = data[rand() % n];
            }
        }

        if (iter == max_iterations) {
            cout << "\nReached maximum iterations.\n";
        }
    }

    return assignments;
}

int main() {
    string filen;
    cout << "Enter the name of the file for input: ";
    cin >> filen;

    ifstream file(filen);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filen << endl;
        return 1;
    }

    ofstream outfile("output.csv");
    if (!outfile.is_open()) {
        cerr << "Error: Could not open output.csv for writing." << endl;
        return 1;
    }

    vector<Point> data_points;
    string line, cell;
    
    while (getline(file, line)) {
        stringstream ss(line);
        Point point;
        while (getline(ss, cell, ',')) {
            if (!cell.empty() && cell.back() == '\r') cell.pop_back();
            try {
                if (!cell.empty()) point.push_back(stod(cell));
            } catch (...) {
                cerr << "Error reading data: non-numeric cell found." << endl;
                return 1;
            }
        }
        if (!point.empty()) data_points.push_back(point);
    }
    file.close();

    if (data_points.empty() || data_points[0].empty()) {
        cerr << "Error: Input file is empty or invalid." << endl;
        outfile << "Error: Input file is empty or invalid." << endl;
        outfile.close();
        return 1;
    }
    
    int k;
    cout << "Enter the desired number of clusters (K): ";
    outfile << "Enter the desired number of clusters (K): "; 
    if (!(cin >> k) || k <= 0 || k > data_points.size()) {
        cerr << "Error: Invalid number of clusters K. Must be between 1 and the number of data points (" << data_points.size() << ").\n";
        outfile << "Error: Invalid number of clusters K. Must be between 1 and the number of data points (" << data_points.size() << ").\n";
        outfile.close();
        return 1;
    }
    
    vector<int> final = kmeans(data_points, k);

    if (!final.empty()) {
        cout << "\n======== Final Assignments ========\n";
        outfile << "\n======== Final Assignments ========\n";
        for (size_t i = 0; i < data_points.size(); ++i) {
            cout << "Point " << i << " -> Cluster " << final[i] << endl;
            outfile << "Point " << i << " -> Cluster " << final[i] << endl;
        }
    } else {
        outfile << "K-means failed or no assignments.\n";
    }

    outfile.close();

    return 0;
}
