#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>
using namespace std;

constexpr auto &&now = std::chrono::high_resolution_clock::now;
inline void ReadInFile(ifstream &readInFile, int** &matrix, const short rows, const short columns);
inline void FindPeaksInFirstRow(int** &matrix, const short rows, const short columns, int &count, stringstream &ss);
inline void FindPeaksInMiddleRow(int** &matrix, const short rows, const short columns, int &count, stringstream &ss);
inline void FindPeaksInLastRow(int** &matrix, const short rows, const short columns, int &count, stringstream &ss);

int main(int argc, char* argv[]) {

    auto programStartTime = now();

    // Step 1. Open file
    if (argc < 2) {
        cout << "Error: missing argument! Please specify the path to matrix.data!" << '\n';
        return 1;
    }
    ifstream readInFile(string(argv[1]) + "/matrix.data");
    if (!readInFile.is_open()) {
        cout << "Error: unable to open " << argv[1] << "/matrix.data , please check out the existence of file." << '\n';
        return 1;
    }
    stringstream coutMessage; coutMessage << "step1: open file successfully." << endl;

    // Step 2.1 Read in the first 2 numbers : rows, columns.
    auto phaseStartTime = now();
    short rows, columns;
    readInFile >> rows; readInFile >> columns;

    // clear the first '\n' in the filestream, which is at the end of first line.
    char bufForFist[1]; readInFile.read(bufForFist, sizeof(bufForFist));

    // Step 2.2 Create a matrix and read the rest of the files into matrix.
    int** matrix = new int*[rows];
    for (short i = 0 ; i < rows ; ++i) {
        matrix[i] = new int[columns];
    }
    ReadInFile(readInFile, matrix, rows, columns);

    chrono::duration<double> elapsed = now() - phaseStartTime;
    coutMessage << "step2: " << elapsed.count() * 1000 << " ms, read from file and create matrix in memory." << endl;

    // Step 3. find peaks in the matrix
    phaseStartTime = now();

    stringstream ss;
    int count = 0;
    FindPeaksInFirstRow(matrix, rows, columns, count, ss);
    if (rows > 2) FindPeaksInMiddleRow(matrix, rows, columns, count, ss);
    if (rows > 1) FindPeaksInLastRow(matrix, rows, columns, count, ss);

    elapsed = now() - phaseStartTime;
    coutMessage << "step3: " << elapsed.count() * 1000 << " ms, find peaks and save results in memory." << endl;
    
    // Step 4. output results to final.peak
    phaseStartTime = now();

    ofstream outputFile(string(argv[1]) + "/final.peak");
    outputFile << count << ss.rdbuf();

    elapsed = now() - phaseStartTime;
    coutMessage << "step4: " << elapsed.count() * 1000 << " ms, output results to the file 'final.peak'." << endl;

    // de-allocate memory
    for (short i = 0 ; i < rows ; ++i) delete[] matrix[i];
    delete [] matrix;

    elapsed = now() - programStartTime;
    coutMessage << "Total used time : " << elapsed.count() * 1000 << " ms." << endl << endl;
    cout << coutMessage.rdbuf();

    return 0;
}

inline void ReadInFile(ifstream &readInFile, int** &matrix, const short rows, const short columns) {
    int lc = 0;
    int item = 0;

    char buf[2048];
    short pi = 0, pj = 0;
    int total = rows * columns;
    bool hasMinusSign = false;
    bool isPreviousNumber = false;
    bool hasNumber = false;
    
    do {
        readInFile.read(buf, sizeof(buf));
        int k = readInFile.gcount();
        for (int i = 0; i < k && lc < total; ++i)
        {
            switch (buf[i])
            {
                case '-':
                    hasMinusSign = true;
                    break;
                case '0': case '1': case '2': case '3':
                case '4': case '5': case '6': case '7':
                case '8': case '9':
                    item = 10*item + buf[i] - '0';
                    isPreviousNumber = true; hasNumber = true;
                    break;
                default:
                    if (!isPreviousNumber) break;
                    if (hasMinusSign) item = -item;
                    matrix[pi][pj++] = item;
                    if (pj == columns) { ++pi; pj = 0; }
                    lc++; item = 0; hasNumber = false; hasMinusSign = false; isPreviousNumber = false;
                    break;
            }
        }
        if (lc == total) break;
    } while (!readInFile.eof());

    /* If the last character in the file is a number,
       it will not have opportunity to be put in matrix in the previous switch case */
    if (hasNumber) {
        if (hasMinusSign) item = -item;
        matrix[pi][pj] = item;
    }
};

inline void FindPeaksInFirstRow(int** &matrix, const short rows, const short columns, int &count, stringstream &ss) {

    const short i = 0, j0 = 0, j_end = columns - 1;
    short j;

    // 1. is front point peak?
    if ((columns > 1 && matrix[i][j0] < matrix[i][j0+1]) ||
        (rows > 1 && matrix[i][j0] < matrix[i+1][j0])) {/* point is not peak, do nothing*/}
    else { // point is peak
        ss << endl << i << ' ' << j0;
        ++count;
    }

    // 2. are middile points peak?
    if (columns > 2) {
        if (rows == 1) { // if matrix contains single row
            for (j = 1 ; j < j_end ; ++j) {
                if (matrix[i][j] < matrix[i][j-1] ||
                    matrix[i][j] < matrix[i][j+1]) {/* point is not peak, do nothing*/}
                else { // point is peak
                    ss << endl << i << ' ' << j;
                    ++count;
                }
            }
        } else { // if matrix contains multiple rows
            for (j = 1 ; j < j_end ; ++j) {
                if (matrix[i][j] < matrix[i][j-1] ||
                    matrix[i][j] < matrix[i][j+1] ||
                    matrix[i][j] < matrix[i+1][j]) {/* point is not peak, do nothing*/}
                else { // point is peak
                    ss << endl << i << ' ' << j;
                    ++count;
                }
            }
        }
    }

    // 3. is rear points peak?
    if (columns > 1) {
        if (matrix[i][j_end] < matrix[i][j_end-1] ||
            (rows > 1 && matrix[i][j_end] < matrix[i+1][j_end])) {/* point is not peak, do nothing*/}
        else { // point is peak
            ss << endl << i << ' ' << j_end;
            ++count;
        }
    }
};

// used only when rows > 2
inline void FindPeaksInMiddleRow(int** &matrix, const short rows, const short columns, int &count, stringstream &ss) {
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
            ss << endl << i << ' ' << j0;
            ++count;
        }

        // 2. are middile points peak?
        if (columns > 2) {
            for (j = 1 ; j < j_end ; ++j) {
                if (matrix[i][j] < matrix[i][j-1] ||
                    matrix[i][j] < matrix[i][j+1] ||
                    matrix[i][j] < matrix[i-1][j] ||
                    matrix[i][j] < matrix[i+1][j]) {/* point is not peak, do nothing*/}
                else { // point is peak
                    ss << endl << i << ' ' << j;
                    ++count;
                }
            }
        }

        // 3. is rear point peak?
        if (columns > 1) {
            if (matrix[i][j_end] < matrix[i][j_end-1] ||
                matrix[i][j_end] < matrix[i-1][j_end] ||
                matrix[i][j_end] < matrix[i+1][j_end]) {/* point is not peak, do nothing*/}
            else { // point is peak
                ss << endl << i << ' ' << j;
                ++count;
            }
        }
    }
};

// used only when rows > 1
inline void FindPeaksInLastRow(int** &matrix, const short rows, const short columns, int &count, stringstream  &ss) {

    if (rows < 2) throw "Error! This function: \"FindPeaksInLastRow\" can only be used when rows > 1";

    const short i = rows - 1, j0 = 0, j_end = columns - 1;
    short j;

    // 1. is front point peak?
    if ((columns > 1 && matrix[i][j0] < matrix[i][j0+1]) ||
        matrix[i][j0] < matrix[i-1][j0]) {/* point is not peak, do nothing*/}
    else { // point is peak
        ss << endl << i << ' ' << j0;
        ++count;
    }

    // 2. are middle points peak?
    if (columns > 2) {
        for (j = 1 ; j < j_end ; ++j) {
            if (matrix[i][j] < matrix[i][j-1] ||
                matrix[i][j] < matrix[i][j+1] ||
                matrix[i][j] < matrix[i-1][j]) {/* point is not peak, do nothing*/}
            else { // point is peak
                ss << endl << i << ' ' << j;
                ++count;
            }
        }
    }

    // 3. is rear point peak?
    if (columns > 1) {
        if (matrix[i][j_end] < matrix[i][j_end-1] ||
            matrix[i][j_end] < matrix[i-1][j_end]) {/* point is not peak, do nothing*/}
        else { // point is peak
            ss << endl << i << ' ' << j_end;
            ++count;
        }
    }
};