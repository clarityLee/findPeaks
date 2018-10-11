#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {

    // Step 1. Open file
    if (argc < 2) {
        cout << "Error: please specify the path to matrix.data!" << endl;
        return 1;
    }
    string pathAndName(argv[1]);
    pathAndName += "/matrix.data";
    
    ifstream readInFile(pathAndName);
    if (!readInFile.is_open()) {
        cout << "Error: unable to open " << pathAndName << endl;
        return 1;
    }

    // Step 2.1 Read in the first 2 numbers : rows, columns.
    int rows, columns;
    readInFile >> rows;
    readInFile >> columns;

    // Step 2.2 Create matrix and read in the rest of the files
    int** matrix = new int*[rows];
    for (int i = 0 ; i < rows ; ++i) {
        matrix[i] = new int[columns];
        for (int j = 0 ; j < columns ; ++j) {
            readInFile >> matrix[i][j];
        }
    }

    // Step 3. find peaks
    std::vector<int> peakIJs; // This is the vector that stores peak i,j
    for (int i = 1 ; i < rows-1 ; ++i) {
        for (int j = 1 ; j < columns-1 ; ++j) {
            if (matrix[i][j] < matrix[i-1][j]) continue;
            if (matrix[i][j] < matrix[i+1][j]) continue;
            if (matrix[i][j] < matrix[i][j-1]) continue;
            if (matrix[i][j] < matrix[i][j+1]) continue;
            peakIJs.push_back(i);
            peakIJs.push_back(j);
        }
    }

    // Step 4. output to final.peak
    ofstream file("./final.peak");
    file << peakIJs.size()/2 << endl;
    for (int i = 0 ; i < peakIJs.size() ;)
        file << peakIJs[i++] << " " << peakIJs[i++] << endl;

    // de-allocate memory
    for (int i = 0 ; i < rows ; ++i)
        delete[] matrix[i];
    delete [] matrix;

    return 0;
}