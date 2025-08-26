#include <bits/stdc++.h>
using namespace std;

// Function to calculate the median of a vector of integers
double calculateMedian(const vector<int> &data)
{
    int n = data.size();
    if (n == 0)
    {
        return 0.0;
    }
    // If the size is odd, the median is the middle element
    if (n % 2 != 0)
    {
        return data[n / 2];
    }
    // If the size is even, the median is the average of the two middle elements
    else
    {
        return (data[n / 2 - 1] + data[n / 2]) / 2.0;
    }
}

// Function to calculate the first quartile (Q1)
double calculateQ1(const vector<int> &data)
{
    int n = data.size();
    if (n == 0)
    {
        return 0.0;
    }
    // Create a sub-vector for the lower half of the data
    vector<int> lowerHalf;
    lowerHalf.assign(data.begin(), data.begin() + n / 2);
    return calculateMedian(lowerHalf);
}

// Function to calculate the third quartile (Q3)
double calculateQ3(const vector<int> &data)
{
    int n = data.size();
    if (n == 0)
    {
        return 0.0;
    }
    // Create a sub-vector for the upper half of the data
    vector<int> upperHalf;
    if (n % 2 == 0)
    {
        upperHalf.assign(data.begin() + n / 2, data.end());
    }
    else
    {
        // Exclude the median for odd-sized datasets
        upperHalf.assign(data.begin() + n / 2 + 1, data.end());
    }
    return calculateMedian(upperHalf);
}

int main()
{
    // Input file name
    string inputFileName = "data.txt";
    ifstream inputFile(inputFileName);

    // Check if the input file can be opened
    if (!inputFile.is_open())
    {
        cerr << "Error: Unable to open input file '" << inputFileName << "'" << endl;
        return 1;
    }

    // Read data from the file
    vector<int> data;
    int value;
    while (inputFile >> value)
    {
        data.push_back(value);
    }
    inputFile.close();

    // Sort the data to correctly calculate statistics
    sort(data.begin(), data.end());

    // Calculate the five-number summary
    int minVal = data.front();
    int maxVal = data.back();
    double medianVal = calculateMedian(data);
    double q1Val = calculateQ1(data);
    double q3Val = calculateQ3(data);

    double range = maxVal - minVal;
    double iqr = q3Val - q1Val;
    double whiskerLower = q1Val - 1.5 * iqr;
    double whiskerUpper = q3Val + 1.5 * iqr;
    whiskerLower = max(whiskerLower, static_cast<double>(minVal));
    whiskerUpper = min(whiskerUpper, static_cast<double>(maxVal));

    // Output to console
    cout << "--- Five-Number Summary ---" << endl;
    cout << "Minimum Value: " << minVal << endl;
    cout << "First Quartile (Q1): " << q1Val << endl;
    cout << "Median: " << medianVal << endl;
    cout << "Third Quartile (Q3): " << q3Val << endl;
    cout << "Maximum Value: " << maxVal << endl;
    cout << "Range: " << range << endl;
    cout << "Interquartile Range (IQR): " << iqr << endl;
    cout << "Whisker Lower Bound: " << whiskerLower << endl;
    cout << "Whisker Upper Bound: " << whiskerUpper << endl;

    // Output to a file
    string outputFileName = "summary.txt";
    ofstream outputFile(outputFileName);
    if (outputFile.is_open())
    {
        outputFile << "Five-Number Summary" << endl;
        outputFile << "Minimum Value," << minVal << endl;
        outputFile << "First Quartile (Q1)," << q1Val << endl;
        outputFile << "Median," << medianVal << endl;
        outputFile << "Third Quartile (Q3)," << q3Val << endl;
        outputFile << "Maximum Value," << maxVal << endl;
        outputFile << "Range," << range << endl;
        outputFile << "Interquartile Range (IQR)," << iqr << endl;
        outputFile << "Whisker Lower Bound," << whiskerLower << endl;
        outputFile << "Whisker Upper Bound," << whiskerUpper << endl;
        outputFile.close();
        cout << "\nResults also saved to '" << outputFileName << "'" << endl;
    }
    else
    {
        cerr << "\nError: Unable to create output file '" << outputFileName << "'" << endl;
    }

    return 0;
}
