#include <vector>
#include <algorithm>
#include "matrix.h"

void rek_mno_pomozna(Tmat &mat1, Tmat &mat2, Tmat &mat3,
                     int a1, int a2, 
                     int b1, int b2,
                     int c1, int c2){
    
    int aD = a2-a1;
    int bD = b2-b1;
    int cD = c2-c1;
    int aP = a1+aD/2;
    int bP = b1+bD/2;
    int cP = c1+cD/2;

    if(std::min({aD, bD, cD}) < 22){
        pomozna_mno_kla(mat1, mat2, mat3, a1, a2,  b1, b2, c1, c2);
    }
    else{
        rek_mno_pomozna(mat1, mat2, mat3, a1, aP,  b1, bP, c1, cP);
        rek_mno_pomozna(mat1, mat2, mat3, a1, aP,  b1, bP, cP, c2);
        rek_mno_pomozna(mat1, mat2, mat3, a1, aP,  bP, b2, c1, cP);
        rek_mno_pomozna(mat1, mat2, mat3, a1, aP,  bP, b2, cP, c2);
        
        rek_mno_pomozna(mat1, mat2, mat3, aP, a2,  b1, bP, c1, cP);
        rek_mno_pomozna(mat1, mat2, mat3, aP, a2,  b1, bP, cP, c2);
        rek_mno_pomozna(mat1, mat2, mat3, aP, a2,  bP, b2, c1, cP);
        rek_mno_pomozna(mat1, mat2, mat3, aP, a2,  bP, b2, cP, c2);
    }
}

Tmat MultiplicationRecursive(Tmat &mat1, Tmat &mat2){
    int a = mat1.size();
    int b = mat1[0].size();
    int c = mat2[0].size();
    
    Tmat mat3 = newMat(c,a);
    rek_mno_pomozna(mat1, mat2, mat3, 0, a,  0, b, 0, c);
    
    return mat3;
}

