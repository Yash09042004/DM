#include <bits/stdc++.h>
using namespace std;

using Point = vector<double>;

double euclidean_distance(const Point& p1, const Point& p2) {
    double sum_sq = 0.0;
    for (size_t i = 0; i < p1.size(); ++i) {
        sum_sq += pow(p1[i] - p2[i], 2);
    }
    return sqrt(sum_sq);
}

vector<vector<double>> compute_distance_matrix(const vector<Point>& data) {
    int n = data.size();
    vector<vector<double>> dist(n, vector<double>(n, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            dist[i][j] = dist[j][i] = euclidean_distance(data[i], data[j]);
        }
    }
    return dist;
}

vector<int> hierarchical_clustering(const vector<Point>& data, int num_clusters, int linkage_type, ostream& out = cout) {
    int n = data.size();
    if (n < num_clusters) {
        out << "Error: Number of clusters cannot exceed number of data points." << endl;
        return {};
    }

    vector<vector<double>> dist_matrix = compute_distance_matrix(data);

    vector<set<int>> clusters(n);
    for (int i = 0; i < n; ++i) {
        clusters[i].insert(i);
    }
    vector<int> vis(n, 1);

    using T = tuple<double, int, int>;
    priority_queue<T, vector<T>, greater<T>> pq;

    auto add_distances = [&]() {
        pq = priority_queue<T, vector<T>, greater<T>>();
        for (int i = 0; i < n; ++i) {
            if (!vis[i]) continue;
            for (int j = i + 1; j < n; ++j) {
                if (!vis[j]) continue;
                double dist;
                if (linkage_type == 1) { // Single linkage: min distance
                    dist = numeric_limits<double>::max();
                    for (int p : clusters[i]) {
                        for (int q : clusters[j]) {
                            dist = min(dist, dist_matrix[p][q]);
                        }
                    }
                } else if (linkage_type == 2) { // Complete linkage: max distance
                    dist = 0.0;
                    for (int p : clusters[i]) {
                        for (int q : clusters[j]) {
                            dist = max(dist, dist_matrix[p][q]);
                        }
                    }
                } else if (linkage_type == 3) { // Average linkage: average distance
                    double sum = 0.0;
                    int count = 0;
                    for (int p : clusters[i]) {
                        for (int q : clusters[j]) {
                            sum += dist_matrix[p][q];
                            count++;
                        }
                    }
                    dist = sum / count;
                }
                pq.push({dist, i, j});
            }
        }
    };

    add_distances();

    out << "Starting Hierarchical Clustering with " << num_clusters << " clusters and linkage type " << linkage_type << "...\n";

    int active_count = n;
    while (active_count > num_clusters) {
        auto [dist, c1, c2] = pq.top();
        pq.pop();

        if (!vis[c1] || !vis[c2]) continue;

        clusters[c1].insert(clusters[c2].begin(), clusters[c2].end());
        clusters[c2].clear();
        vis[c2] = 0;
        active_count--;

        out << "Merging clusters " << c1 << " and " << c2 << " with distance " << fixed << setprecision(4) << dist << endl;

        add_distances();
    }

    vector<int> assignments(n);
    int cluster_id = 0;
    for (const auto& cluster : clusters) {
        if (!cluster.empty()) {
            for (int idx : cluster) {
                assignments[idx] = cluster_id;
            }
            cluster_id++;
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

    int linkage_type;
    cout << "Choose linkage type:\n1. Single Linkage\n2. Complete Linkage\n3. Average Linkage\nEnter choice (1-3): ";
    outfile << "Choose linkage type:\n1. Single Linkage\n2. Complete Linkage\n3. Average Linkage\nEnter choice (1-3): ";
    cin >> linkage_type;
    if (linkage_type < 1 || linkage_type > 3) {
        cerr << "Invalid linkage type." << endl;
        outfile << "Invalid linkage type." << endl;
        outfile.close();
        return 1;
    }

    int num_clusters;
    cout << "Enter the desired number of clusters: ";
    outfile << "Enter the desired number of clusters: ";
    cin >> num_clusters;
    if (num_clusters <= 0 || num_clusters > data_points.size()) {
        cerr << "Error: Invalid number of clusters." << endl;
        outfile << "Error: Invalid number of clusters." << endl;
        outfile.close();
        return 1;
    }

    vector<int> final = hierarchical_clustering(data_points, num_clusters, linkage_type);

    if (!final.empty()) {
        cout << "\n======== Final Assignments ========\n";
        outfile << "\n======== Final Assignments ========\n";
        for (size_t i = 0; i < data_points.size(); ++i) {
            cout << "Point " << i << " -> Cluster " << final[i] << endl;
            outfile << "Point " << i << " -> Cluster " << final[i] << endl;
        }
    } else {
        cout << "Clustering failed." << endl;
        outfile << "Clustering failed." << endl;
    }

    outfile.close();
    return 0;
}