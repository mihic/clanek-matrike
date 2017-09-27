#include <vector>
#include "matrix.h"

Tmat MultiplicationClassic(Tmat &mat1, Tmat &mat2){
    int a = mat1.n;
    int b = mat1.m;
    int c = mat2.m;

    Tmat mat3(c,a);

    pomozna_mno_kla(mat1, mat2, mat3, 0, a, 0, b, 0, c);
    return mat3;
}

