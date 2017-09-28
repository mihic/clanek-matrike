#include <vector>
#include "matrix.h"

Tmat MultiplicationClassicTransposed(Tmat &mat1, Tmat &mat2){
    Tmat mat4 = transponiraj(mat2);

    int a = mat1.m;
    int b = mat1.n;
    int c = mat4.m;
    
    Tmat mat3(a,c);
    pomozna_mno_kla_trans(mat1, mat4, mat3, 0, a, 0, b, 0, c);
    return mat3;
}

