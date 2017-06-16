#include <vector>
#include "matrix.h"

Tmat transponirano_mnozenje(Tmat &mat1, Tmat &mat2){
    Tmat mat4 = transponiraj(mat2);

    int a = mat1.size();
    int b = mat1[0].size();
    int c = mat4.size();
    
    Tmat mat3 = newMat(c,a);
    pomozna_mno_kla_trans(mat1, mat4, mat3, 0, a, 0, b, 0, c);
    return mat3;
}

