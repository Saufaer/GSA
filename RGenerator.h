
#ifndef RGenerator_h
#define RGenerator_h

#include <vector>
#include <fstream>
using namespace std;

class RGenerator
{
public:
    void CreateFile(const char* filename, int);
    RGenerator();
    ~RGenerator();
private:
    ofstream filestream_;
};

#endif /* RGenerator_hpp */
