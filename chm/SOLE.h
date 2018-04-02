
#ifndef SOLE_h
#define SOLE_h

#define MAX_ITERATIONS_COUNT 1000000

#include <vector>
#include <cmath>
#include "Matrix.h"
using namespace std;

class SOLE
{
public:
    SOLE(vector<vector <double> > A, vector<double> b,
         double accuracy);
    vector<double> getResult() { return _X; }
    vector<double> geErrors() { return _errors; }
    int getIterationsCount() { return _IterationsMade; }
    bool getDiagonalDominance();
    void Solve();
    
    vector<double> _errors;
private:
    void Iteration();
    
    int _IterationsMade = 0;
    bool _DiagonalDominance;
    double _accuracy;
    
    vector<vector<double> > _A;
    vector<double> _b;
    vector<double>  _X;
};

#endif /* SOLE_hpp */
