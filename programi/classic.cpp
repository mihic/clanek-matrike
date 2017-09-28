#include <vector>
#include <iostream>
#include "matrix.h"

Tmat MultiplicationClassic(Tmat &mat1, Tmat &mat2){
    if (mat1.n != mat2.m){
        std::cout << "Matrix dim not mach mat1.n=" << mat1.n << " mat1.m=" << mat2.m << std::endl;
        std::exit(-1);
    }
    std::cout << "OK AA";
    int a = mat1.m;
    int b = mat1.n;
    int c = mat2.n;

    std::cout << "OK BB";
    Tmat mat3(a,c);

    std::cout << "OK";
    pomozna_mno_kla(mat1, mat2, mat3, 0, a, 0, b, 0, c);
    std::cout << "OK mc";
    return mat3;
}

