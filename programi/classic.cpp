#include <vector>
#include <functional>
#include <algorithm>
#include "matrix.h"

typedef std::vector<std::vector<double>> Tmat;

//>> klasicno iterativno mnozenje
Tmat klasicno_mnozenje(Tmat &mat1, Tmat &mat2){
    int a = mat1.size();
    int b = mat1[0].size();
    int c = mat2[0].size();

    std::vector<double> nicelni(c, 0.0);
    Tmat mat3(a, nicelni);
    for (int i = 0; i<a; i+=1){
        for (int j = 0; j<c; j+=1){
            double s = 0;
            for (int k = 0; k<b; k+=1){
                    s+=mat1[i][k]*mat2[k][j];
            }
            mat3[i][j] = s;
        }
    }
    return mat3;
}
//<< klasicno iterativno mnozenje
