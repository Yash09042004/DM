    #include <bits/stdc++.h>
    using namespace std;

    double calculateMedian(const vector<int> &data)
    {
        int n = data.size();
        if (n == 0)
        {
            return 0.0;
        }
        if (n % 2 != 0)
        {
            return data[n / 2];
        }
        else
        {
            return (data[n / 2 - 1] + data[n / 2]) / 2.0;
        }
    }

    double calculateQ1(const vector<int> &data)
    {
        int n = data.size();
        if (n == 0)
        {
            return 0.0;
        }
        vector<int> lowerHalf;
        lowerHalf.assign(data.begin(), data.begin() + n / 2);
        return calculateMedian(lowerHalf);
    }

    double calculateQ3(const vector<int> &data)
    {
        int n = data.size();
        if (n == 0)
        {
            return 0.0;
        }
        vector<int> upperHalf;
        if (n % 2 == 0)
        {
            upperHalf.assign(data.begin() + n / 2, data.end());
        }
        else
        {
            upperHalf.assign(data.begin() + n / 2 + 1, data.end());
        }
        return calculateMedian(upperHalf);
    }

    int main()
    {
        string inputFileName;
        cout << "Enter input filename: ";
        getline(cin, inputFileName);  

        ifstream inputFile(inputFileName);
        if (!inputFile.is_open())
        {
            cerr << "Error: Unable to open input file '" << inputFileName << "'" << endl;
            return 1;
        }

        vector<int> data;
        int value;
        while (inputFile >> value)
        {
            data.push_back(value);
        }
        inputFile.close();

        if (data.empty())
        {
            cerr << "Error: No data found in file." << endl;
            return 1;
        }

        sort(data.begin(), data.end());

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
        string outputFileName = "output.csv";
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
