#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
using namespace std;

inline void FindPeaksInFirstRow(int** &matrix, const short rows, const short columns, vector<short> &peakIJs);
inline void FindPeaksInMiddleRow(int** &matrix, const short rows, const short columns, vector<short> &peakIJs);
inline void FindPeaksInLastRow(int** &matrix, const short rows, const short columns, vector<short> &peakIJs);

int main(int argc, char* argv[]) {

    auto start = std::chrono::high_resolution_clock::now();

    // Step 1. Open file
    if (argc < 2) {
        cout << "Error: please specify the path to matrix.data!" << '\n';
        return 1;
    }
    string sourceData_pathAndName(argv[1]);
    sourceData_pathAndName += "/matrix.data";
    
    ifstream readInFile(sourceData_pathAndName);
    if (!readInFile.is_open()) {
        cout << "Error: unable to open " << sourceData_pathAndName << '\n';
        return 1;
    }

    // Step 2.1 Read in the first 2 numbers : rows, columns.
    short rows, columns;
    readInFile >> rows;
    readInFile >> columns;

    // Step 2.2 Create a matrix and read in the rest of the files into matrix.
    int** matrix = new int*[rows];
    for (short i = 0 ; i < rows ; ++i) {
        matrix[i] = new int[columns];
        for (short j = 0 ; j < columns ; ++j) {
            readInFile >> matrix[i][j];
        }
    }

    // Step 3. find peaks in the matrix
    std::vector<short> peakIJs; // This is the vector that stores peak i,j
    FindPeaksInFirstRow(matrix, rows, columns, peakIJs);
    if (rows > 2) FindPeaksInMiddleRow(matrix, rows, columns, peakIJs);
    if (rows > 1) FindPeaksInLastRow(matrix, rows, columns, peakIJs);

    // Step 4. output results to final.peak
    string output_pathAndName(argv[1]);
    output_pathAndName += "/final.peak";
    ofstream outputFile(output_pathAndName);
    outputFile << peakIJs.size()/2 << '\n';
    int i = 0;
    while (i < peakIJs.size() - 2)
        outputFile << peakIJs[i++] << ' ' << peakIJs[i++] << '\n';
    outputFile << peakIJs[i++] << ' ' << peakIJs[i];

    // de-allocate memory
    for (short i = 0 ; i < rows ; ++i)
        delete[] matrix[i];
    delete [] matrix;

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    cout << "total used time : " << elapsed.count() << " seconds.";

    return 0;
}

inline void FindPeaksInFirstRow(int** &matrix, const short rows, const short columns, vector<short> &peakIJs) {

    const short i = 0, j0 = 0, j_end = columns - 1;
    short j;

    // 1. is front point peak?
    if ((columns > 1 && matrix[i][j0] < matrix[i][j0+1]) ||
        (rows > 1 && matrix[i][j0] < matrix[i+1][j0])) {/* point is not peak, do nothing*/}
    else { // point is peak
        peakIJs.push_back(i);
        peakIJs.push_back(j0);
    }

    // 2. are middile points peak?
    if (columns > 2) {
        if (rows == 1) { // if matrix contains single row
            for (j = 1 ; j < j_end ; ++j) {
                if (matrix[i][j] < matrix[i][j-1] ||
                    matrix[i][j] < matrix[i][j+1]) {/* point is not peak, do nothing*/}
                else { // point is peak
                    peakIJs.push_back(i);
                    peakIJs.push_back(j);
                }
            }
        } else { // if matrix contains multiple rows
            for (j = 1 ; j < j_end ; ++j) {
                if (matrix[i][j] < matrix[i][j-1] ||
                    matrix[i][j] < matrix[i][j+1] ||
                    matrix[i][j] < matrix[i+1][j]) {/* point is not peak, do nothing*/}
                else { // point is peak
                    peakIJs.push_back(i);
                    peakIJs.push_back(j);
                }
            }
        }
    }

    // 3. is rear points peak?
    if (columns > 1) {
        if (matrix[i][j_end] < matrix[i][j_end-1] ||
            (rows > 1 && matrix[i][j_end] < matrix[i+1][j_end])) {/* point is not peak, do nothing*/}
        else { // point is peak
            peakIJs.push_back(i);
            peakIJs.push_back(j_end);
        }
    }
};

// used only when rows > 2
inline void FindPeaksInMiddleRow(int** &matrix, const short rows, const short columns, vector<short> &peakIJs) {
    if (rows < 3) throw "Error! This function: \"FindPeaksInMiddleRow\" can only be used when rows > 2";

    const short i_end = rows - 1, j0 = 0, j_end = columns - 1;
    short i = 0, j;

    // find all peaks in one row for each while loop
    while (++i < i_end) {

        // 1. is front point peak?
        if (matrix[i][j0] < matrix[i-1][j0] ||
            matrix[i][j0] < matrix[i+1][j0] ||
            (columns > 1 && matrix[i][j0] < matrix[i][j0+1])) {/* point is not peak, do nothing*/}
        else { // point is peak
            peakIJs.push_back(i);
            peakIJs.push_back(j0);
        }

        // 2. are middile points peak?
        if (columns > 2) {
            for (j = 1 ; j < j_end ; ++j) {
                if (matrix[i][j] < matrix[i][j-1] ||
                    matrix[i][j] < matrix[i][j+1] ||
                    matrix[i][j] < matrix[i-1][j] ||
                    matrix[i][j] < matrix[i+1][j]) {/* point is not peak, do nothing*/}
                else { // point is peak
                    peakIJs.push_back(i);
                    peakIJs.push_back(j);
                }
            }
        }

        // 3. is rear point peak?
        if (columns > 1) {
            if (matrix[i][j_end] < matrix[i][j_end-1] ||
                matrix[i][j_end] < matrix[i-1][j_end] ||
                matrix[i][j_end] < matrix[i+1][j_end]) {/* point is not peak, do nothing*/}
            else { // point is peak
                peakIJs.push_back(i);
                peakIJs.push_back(j);
            }
        }
    }
};

// used only when rows > 1
inline void FindPeaksInLastRow(int** &matrix, const short rows, const short columns, vector<short> &peakIJs) {

    if (rows < 2) throw "Error! This function: \"FindPeaksInLastRow\" can only be used when rows > 1";

    const short i = rows - 1, j0 = 0, j_end = columns - 1;
    short j;

    // 1. is front point peak?
    if ((columns > 1 && matrix[i][j0] < matrix[i][j0+1]) ||
        matrix[i][j0] < matrix[i-1][j0]) {/* point is not peak, do nothing*/}
    else { // point is peak
        peakIJs.push_back(i);
        peakIJs.push_back(j0);
    }

    // 2. are middle points peak?
    for (j = 1 ; j < j_end ; ++j) {
        if (matrix[i][j] < matrix[i][j-1] ||
            matrix[i][j] < matrix[i][j+1] ||
            matrix[i][j] < matrix[i-1][j]) {/* point is not peak, do nothing*/}
        else { // point is peak
            peakIJs.push_back(i);
            peakIJs.push_back(j);
        }
    }

    // 3. is rear point peak?
    if (columns > 1) {
        if (matrix[i][j_end] < matrix[i][j_end-1] ||
            matrix[i][j_end] < matrix[i-1][j_end]) {/* point is not peak, do nothing*/}
        else { // point is peak
            peakIJs.push_back(i);
            peakIJs.push_back(j_end);
        }
    }
};