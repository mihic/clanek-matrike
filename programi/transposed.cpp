#include <vector>
#include <functional>
#include <algorithm>
#include "matrix.h"

typedef std::vector<std::vector<double>> Tmat;

//>> mnozenje s predhodnim transponiranjem
Tmat gl_transponirano_mno(Tmat &mat1, Tmat &mat4){
    int a = mat1.size();
    int b = mat1[0].size();
    int c = mat4.size();
    
    std::vector<double> nicelni(c, 0.0);
    Tmat mat3(a, nicelni);
    for (int i = 0; i<a; i+=1){
        for (int j = 0; j<c; j+=1){
            double s = 0;
            for (int k = 0; k<b; k+=1){
                    s+=mat1[i][k]*mat4[j][k];
            }
            mat3[i][j] = s;
        }
    }
    return mat3;
}

Tmat transponirano_mnozenje(Tmat &mat1, Tmat &mat2){   
    Tmat mat4 = transponiraj(mat2);
    return gl_transponirano_mno(mat1, mat4);
}
//<< mnozenje s predhodnim transponiranjem
