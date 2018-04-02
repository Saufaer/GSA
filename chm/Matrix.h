

#ifndef Matrix_h
#define Matrix_h

#include <vector>
#include <cmath>
using namespace std;

class Matrix
{
public:
    static vector<vector<double> > E(int n);
    static vector<vector<double> > Add(vector<vector<double> > matrix1,
                                       vector<vector<double> > matrix2);
    static vector<double> Add(vector<double> matrix1, vector<double> matrix2);
    static vector<vector<double> > Mul(vector<vector<double> > matrix1, double x);
    static vector<double> Mul(vector<double> matrix1, double x);
    static vector<vector<double> > Mul(vector<vector<double> > matrix1,
                                       vector<vector<double> > matrix2);
    static vector<double> Mul(vector<vector<double> > matrix1, vector<double> matrix2);
    static vector<double> Mul(vector<double> matrix1, vector<double> matrix2);
};
#endif /* Matrix_hpp */
