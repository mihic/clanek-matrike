#include <vector>
#include <iostream>
#include <iomanip>
#include "matrix.h"

//code for testing
void prikaz(Tmat &mat) {
  for (auto &vrstica : mat) {
    std::cout << "|";
    for (auto &element : vrstica) {
      std::cout << std::setw(8) << element;
    }
    std::cout << "|" << std::endl;
  }
  std::cout << std::endl;
}

//code for algorithms
Tmat newMat(int a, int b){
    std::vector<double> nicelni(a, 0.0);
    Tmat mat(b, nicelni);
    return mat;
}

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


void pomozna_mno_kla_trans(Tmat &mat1, Tmat &mat2, Tmat &mat3,
                     int a1, int a2, 
                     int b1, int b2,
                     int c1, int c2){

    //mat2 je transponirana
    for (int i = a1; i<a2; i+=1){
        for (int j = c1; j<c2; j+=1){
            double s = 0;
            for (int k = b1; k<b2; k+=1){
                    s+=mat1[i][k]*mat2[j][k];
            }
            mat3[i][j] += s;
        }
    }
}
