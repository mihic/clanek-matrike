#include <vector>
#include <functional>
#include <algorithm>
#include "matrix.h"

typedef std::vector<std::vector<double>> Tmat;

//>> transponirano rekurzivno mnozenje
/*
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
*/

void rek_tra_mno_zakljucek(Tmat &mat1, Tmat &mat2, Tmat &mat3,
                     int a1, int a2, 
                     int b1, int b2,
                     int c1, int c2){

    //mat2 je ze transponirana
    for (int i = a1; i<a2; i+=1){
        for (int j = c1; j<c2; j+=1){
            double s = 0;
            for (int k = b1; k<b2; k+=1){
                    ////std::cout << "ijk|" << i << "|" << j << "|" << k << std::endl;
                    s+=mat1[i][k]*mat2[j][k];
            }
            mat3[i][j] += s;
        }
    }
}

void rek_tra_mno_pomozna(Tmat &mat1, Tmat &mat4, Tmat &mat3,
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
    
    if(minDabc<100){
        rek_tra_mno_zakljucek(mat1, mat4, mat3, a1, a2,  b1, b2, c1, c2);
    }
    else{
        rek_tra_mno_pomozna(mat1, mat4, mat3, a1, aP,  b1, bP, c1, cP);
        rek_tra_mno_pomozna(mat1, mat4, mat3, a1, aP,  b1, bP, cP, c2);
        rek_tra_mno_pomozna(mat1, mat4, mat3, a1, aP,  bP, b2, c1, cP);
        rek_tra_mno_pomozna(mat1, mat4, mat3, a1, aP,  bP, b2, cP, c2);
        
        rek_tra_mno_pomozna(mat1, mat4, mat3, aP, a2,  b1, bP, c1, cP);
        rek_tra_mno_pomozna(mat1, mat4, mat3, aP, a2,  b1, bP, cP, c2);
        rek_tra_mno_pomozna(mat1, mat4, mat3, aP, a2,  bP, b2, c1, cP);
        rek_tra_mno_pomozna(mat1, mat4, mat3, aP, a2,  bP, b2, cP, c2);
    }
}

Tmat gl_rekurzivno_transponirano_mno(Tmat &mat1, Tmat &mat4){
    int a = mat1.size();
    int b = mat1[0].size();
    int c = mat4.size();
    int a1 = 0; 
    int b1 = 0;
    int c1 = 0;

    std::vector<double> nicelni(c, 0.0);
    Tmat mat3(a, nicelni);
    rek_tra_mno_pomozna(mat1, mat4, mat3, a1, a,  b1, b, c1, c);
    
    return mat3;
}

Tmat rekurzivno_transponirano_mnozenje(Tmat &mat1, Tmat &mat2){
    Tmat mat4 = transponiraj(mat2);
    return gl_rekurzivno_transponirano_mno(mat1, mat4);
}
//<< transponirano rekurzivno mnozenje
