#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <limits.h>
#include <algorithm>

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

double nearestNeighbor(const vector<vector<double> > &data, const vector<int> &currFeat, int checkFeature, bool isForwardBackward)
{
    int numCorrectClass = 0;
    
    // For each data point in data
    for(unsigned int i = 0; i < data.size(); ++i)
    {
        int nearestPointLoc = 0;
        double nearestPointDist = (double)INT_MAX;

        // Check each data point
        for(unsigned int j = 0; j < data.size(); ++j)
        {
            double pointDist = 0;
            // If the data point is not itself
            if(i != j)
            {
                // Check the distance of each added feature from data point
                for(unsigned int k = 0; k < currFeat.size(); ++k)
                {
                    // If backward selection and the feature matches the feature considered, ignore it
                    if(!isForwardBackward && currFeat[k] == checkFeature)
                    {
                        //Nothing should happen
                    }
                    else
                    {
                        pointDist += pow(data[i][currFeat[k]] - data[j][currFeat[k]], 2);
                    }
                }
                // If forward selection, then also the feature considered being added
                if(isForwardBackward)
                {
                    pointDist += pow(data[i][checkFeature] - data[j][checkFeature], 2);
                    pointDist = sqrt(pointDist);
                }

                // The lowest distance is the nearest neightbor to be considered as its guess class
                if(pointDist < nearestPointDist)
                {
                    nearestPointDist = pointDist;
                    nearestPointLoc = j;
                }
            }
        }

        // If the nearest data point is correct class, increment the correct count
        if(compareDoubles(data[i][0], data[nearestPointLoc][0]))
        {
            numCorrectClass++;
        }
    }
    
    // The total accuracy for that feature on the level is
    return (double)numCorrectClass / data.size();
}

int main()
{
    double featureNum = 0;                                    // Data input from file
    vector<double> featureVec = {};                           // All added points from file into current point
    vector<vector<double> > dataVec = {};                     // Data
    int numFeatures = 0;                                      // Total amount of features
    vector<int> featureToAdd = {};                            // Current features added on tree
    int smallLarge = 0;                                       // Indicates whether to use the small or large data set
    bool forwardBackward = true;                              // Indicates whether to use forward selection or backward elimination

    cout << "Would you like to have a small (1) or large (2) data set? ";
    cin >> smallLarge;
    while(smallLarge != 1 && smallLarge != 2)
    {
        cout << "Invalid. Try again: ";
        cin >> smallLarge;
    }

    ifstream inputFile;
    if(smallLarge == 1)
    {
        inputFile.open("CS170_Small_Data__13.txt");
    }
    else
    {
        inputFile.open("CS170_Large_Data__58.txt");
    }
    if (!inputFile)
    {
        cout << "Error opening data file" << endl;
        return 0;
    }

    // While there is still inputs on the data file
    bool firstRow = true;
    while(inputFile >> featureNum)
    {
        // If the data is 1 or 2 (This means a new data point since this indicates a class)
        if(compareDoubles(featureNum, 1.00) || compareDoubles(featureNum, 2.00))
        {
            // If this isn't the first row, create a new set of data points
            if(!firstRow)
            {
                dataVec.push_back(featureVec);
                featureVec = {};
            }
            firstRow = false;
        }
        featureVec.push_back(featureNum);
    }
    // Now the total amount of features known is size of the vector - class indicator
    numFeatures = featureVec.size() - 1;

    // Input whether using forward selection or backward elimination
    cout << "Would you like Forward Selection (1) or Backward Selection (2)? ";
    cin >> smallLarge;
    while(smallLarge != 1 && smallLarge != 2)
    {
        cout << "Invalid. Try again: ";
        cin >> smallLarge;
    }
    // If we are using backward selection, then the feature vector should have all the features initially
    if(smallLarge == 1)
    {
        forwardBackward = true;
    }
    else
    {
        forwardBackward = false;
        for(int i = 1; i <= numFeatures; ++i)
        {
            featureToAdd.push_back(i);
        }
    }

    cout << "The data set has " << numFeatures << " with " << dataVec.size() << " points." << endl;
    cout << endl;
    
    double totalHighestAcc = 0;                                   // Highest accuracy of features on tree
    vector<int> allAccFeatures;                                   // The features that are the highest accuracy

    // For all the features (indicating levels of tree)
    for(int i = 1; i <= numFeatures; ++i)
    {
        double pointAcc = 0;
        int pointAccLocation = 0;
        cout << "At level " << i << " on the search tree" << endl;
        // Run nearest neighbor onto each feature
        for(int j = 1; j <= numFeatures; ++j)
        {
            double featAcc = 0;
            bool possibleFeat = true;
            // If the feature was already added and we're using forward selection, move on to the next feature
            // Otherwise, we're using backward selection and only checking for features that aren't there
            if(forwardBackward)
            {
                for(int k = 0; k < featureToAdd.size(); ++k)
                {
                    if(j == featureToAdd[k])
                    {
                        possibleFeat = false;
                        break;
                    }
                }
            }
            else
            {
                possibleFeat = false;
                for(int k = 0; k < featureToAdd.size(); ++k)
                {
                    if(j == featureToAdd[k])
                    {
                        possibleFeat = true;
                        break;
                    }
                }
            }
            
            // Otherwise check out the feature considered
            if(possibleFeat)
            {
                featAcc = nearestNeighbor(dataVec, featureToAdd, j, forwardBackward);
                // Keep track of highest accuracy
                if(forwardBackward)
                {
                    cout << "--Considering adding feature " << j << " with accuracy " << featAcc << endl;
                }
                else
                {
                    cout << "--Removing feature " << j << " results in accuracy " << featAcc << endl;
                }
                if(featAcc > pointAcc)
                {
                    pointAcc = featAcc;
                    pointAccLocation = j;
                }
            }
        }

        // If forward selection, add the highest accuracy feature
        // If backward elimination, remove the lowest accuracy feature
        if(forwardBackward)
        {
            cout << "The highest accuracy feature added is " << pointAccLocation << " with accuracy " << pointAcc << endl;
            featureToAdd.push_back(pointAccLocation);
        }
        else
        {
            cout << "The lowest accuracy feature removed is " << pointAccLocation << " results in accuracy " << pointAcc << endl;
            featureToAdd.erase(remove(featureToAdd.begin(), featureToAdd.end(), pointAccLocation), featureToAdd.end());
        }

        //If the accuracy was the highest so far, keep track of that one
        if(pointAcc > totalHighestAcc)
        {
            totalHighestAcc = pointAcc;
            allAccFeatures = featureToAdd;
        }
    }
    cout << endl;

    // Output the highest accuracy with the appropriate features
    cout << "The most accurate features are {";
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