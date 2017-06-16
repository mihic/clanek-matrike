#include <vector>
#include "matrix.h"

Tmat klasicno_mnozenje(Tmat &mat1, Tmat &mat2){
    int a = mat1.size();
    int b = mat1[0].size();
    int c = mat2[0].size();

    Tmat mat3 = newMat(c,a);

    pomozna_mno_kla(mat1, mat2, mat3, 0, a, 0, b, 0, c);
    return mat3;
}

