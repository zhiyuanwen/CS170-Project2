#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits.h>

using namespace std;

// Helper function to compare two doubles properly due to IEEE double rules
bool compareDoubles(double num1, double num2)
{
    double epsilon = 0.001;
    if(abs(num1 - num2) <= epsilon)
    {
        return true;
    }
    return false;
}

double nearestNeighbor(const vector<vector<double> > &data, const vector<int> &currFeat, int checkFeature)
{
    int numCorrectClass = 0;
    
    for(unsigned int i = 0; i < data.size(); ++i)
    {
        int nearestPointLoc = 0;
        double nearestPointDist = (double)INT_MAX;

        for(unsigned int j = 0; j < data.size(); ++j)
        {
            double pointDist = 0;
            if(i != j)
            {
                for(unsigned int k = 0; k < currFeat.size(); ++k)
                {
                    pointDist += pow(data[i][currFeat[k]] - data[j][currFeat[k]], 2);
                }
                pointDist += pow(data[i][checkFeature] - data[j][checkFeature], 2);
                pointDist = sqrt(pointDist);

                if(pointDist < nearestPointDist)
                {
                    nearestPointDist = pointDist;
                    nearestPointLoc = j;
                }
            }
        }

        if(compareDoubles(data[i][0], data[nearestPointLoc][0]))
        {
            numCorrectClass++;
        }
    }
    
    return (double)numCorrectClass / data.size();
}

int main()
{
    ifstream inputSmall("CS170_Small_Data__13.txt");
    if (!inputSmall)
    {
        cout << "Error opening small data file" << endl;
        return 0;
    }

    /* ifstream inputLarge("CS170_Large_Data__58.txt");
    if (!inputLarge)
    {
        cout << "Error opening large data file" << endl;
        return 0;
    }*/

    double featureNum = 0;
    vector<double> featureVec = {};
    vector<vector<double> > dataVec = {};
    int numFeatures = 0;
    vector<int> featureToAdd = {};

    bool firstRow = true;
    while(inputSmall >> featureNum)
    {
        if(compareDoubles(featureNum, 1.00) || compareDoubles(featureNum, 2.00))
        {
            if(!firstRow)
            {
                dataVec.push_back(featureVec);
                featureVec = {};
            }
            firstRow = false;
        }
        featureVec.push_back(featureNum);
    }
    numFeatures = featureVec.size() - 1;
    //cout << "The number of features is " << numFeatures << endl;

    double totalHighestAcc = 0;
    vector<int> allAccFeatures;
    for(int i = 1; i <= numFeatures; ++i)
    {
        double highestAcc = 0;
        int highAccLocation = 0;
        cout << "At level " << i << " on the search tree" << endl;
        for(int j = 1; j <= numFeatures; ++j)
        {
            double featAcc = 0;
            bool possibleFeat = true;
            for(int k = 0; k < featureToAdd.size(); ++k)
            {
                if(j == featureToAdd[k])
                {
                    possibleFeat = false;
                    break;
                }
            }
            if(possibleFeat)
            {
                featAcc = nearestNeighbor(dataVec, featureToAdd, j);
                cout << "--Considering adding " << j << " feature with accuracy " << featAcc << endl;
                if(featAcc > highestAcc)
                {
                    highestAcc = featAcc;
                    highAccLocation = j;
                }
            }
        }

        cout << "The highest accuracy is feature " << highAccLocation << " with " << highestAcc << endl;
        featureToAdd.push_back(highAccLocation);
        if(highestAcc > totalHighestAcc)
        {
            totalHighestAcc = highestAcc;
            allAccFeatures = featureToAdd;
        }
    }

    cout << "The total amount of features is {";
    for(unsigned int i = 0; i < allAccFeatures.size(); ++i)
    {
		cout << allAccFeatures[i];
        if(i != allAccFeatures.size() - 1)
        {
            cout << ", ";
        }
	}
	cout << "} with accuracy " << totalHighestAcc << endl;

    return 0;
}