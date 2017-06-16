#include <vector>
#include <functional>
#include <algorithm>
#include "matrix.h"

typedef std::vector<std::vector<double>> Tmat;


//>> rekurzivno mnozenje
void pomozna_mno_kla(Tmat &mat1, Tmat &mat2, Tmat &mat3,
                     int a1, int a2, 
                     int b1, int b2,
                     int c1, int c2){

    for (int i = a1; i<a2; i+=1){
        for (int j = c1; j<c2; j+=1){
            double s = 0;
            for (int k = b1; k<b2; k+=1){
                    s+=mat1[i][k]*mat2[k][j];
            }
            mat3[i][j] += s;
        }
    }
}

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
    int minDabc = std::min({aD, bD, cD});
    ////std::cout << aD << "|" << bD << "|" << cD << std::endl;
    
    if(minDabc<100){
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

Tmat rekurzivno_mnozenje(Tmat &mat1, Tmat &mat2){
    int a = mat1.size();
    int b = mat1[0].size();
    int c = mat2[0].size();
    int a1 = 0; 
    int b1 = 0;
    int c1 = 0;
    
    std::vector<double> nicelni(c, 0.0);
    Tmat mat3(a, nicelni);
    rek_mno_pomozna(mat1, mat2, mat3, a1, a,  b1, b, c1, c);
    
    return mat3;
}
//<< rekurzivno mnozenje

