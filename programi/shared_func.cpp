#include <vector>
#include <functional>
#include <algorithm>
#include "matrix.h"

typedef std::vector<std::vector<double>> Tmat;

//>> transponiranje matrike
Tmat transponiraj(Tmat &mat){
    int a = mat.size();
    int b = mat[0].size();
    
    std::vector<double> nicelni1(a, 0.0);
    Tmat mat3(b, nicelni1);
    for (int i = 0; i<a; i+=1){
        for (int j = 0; j<b; j+=1){
            mat3[j][i] = mat[i][j];
        }   
    }
    return mat3;
}
//<< transponiranje matrike
