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

    string fname;
    cout<<"Enter the filename"<<endl;
    getline(cin,fname);

    ifstream inpfile(fname);
    if(!inpfile.is_open()){
        cerr<<"Error: Could not open file "<<fname<<endl;
        return 1;
    }
    vector<int>v;
    int num;
    while(inpfile>>num){
        v.push_back(num);

    }
    inpfile.close();
    sort(v.begin(),v.end());
    int mini = v.front();
    int maxi = v.back();
    double median = calculateMedian(v);
    double q1 = calculateQ1(v);
    double q3 = calculateQ3(v);
    double range = maxi - mini;
    double iqr = q3 - q1;
    
    
    ofstream outfile("output.csv");
    if(!outfile){
        cerr<<"Error: Could not create output.csv"<<endl;
        return 1;
    }

    cout<<"Five number summary is as follows: "<<endl;
    cout<<"Minimum Value: "<<mini<<endl;
    outfile<<"Minimum Value,"<<mini<<endl;
    cout<<"First Quartile (Q1): "<<q1<<endl;
    outfile<<"First Quartile (Q1),"<<q1<<endl;
    cout<<"Median: "<<median<<endl;
    outfile<<"Median,"<<median<<endl;
    cout<<"Third Quartile (Q3): "<<q3<<endl;
    outfile<<"Third Quartile (Q3),"<<q3<<endl;
    cout<<"Maximum Value: "<<maxi<<endl;
    outfile<<"Maximum Value,"<<maxi<<endl;
    cout<<"Range: "<<range<<endl;
    outfile<<"Range,"<<range<<endl;
    cout<<"Interquartile Range (IQR): "<<iqr<<endl;
    outfile<<"Interquartile Range (IQR),"<<iqr<<endl;




    outfile.close();
    return 0;
}
