#include <vector>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include "matrix.h"
                        

Tmat strassen_mul(Tmat &, Tmat &);

void strassen_part(Tmat &mat1, Tmat &mat2, Tmat &mat3,
                        int a1, int a2,
                        int b1, int b2,
                        int c1, int c2){

    int aD = a2-a1;
    int bD = b2-b1;
    int cD = c2-c1;
    int aP = a1+aD/2;
    int bP = b1+bD/2;
    int cP = c1+cD/2;

    if(std::min({aD, bD, cD})<10){
        rek_mno_pomozna(mat1, mat2, mat3, a1, a2,  b1, b2, c1, c2);
    }
    else{
        std::cout << "abc" << aD << " " << bD << " " << cD << std::endl;
        std::cout << "M1" << std::endl;
        //M1
        Tmat mA11A22 = pomozno_sestevanje(mat1, mat1,
                                          //A11
                                          a1, aP,
                                          b1, bP,
                                          //A22
                                          aP, a2,
                                          bP, b2);

        std::cout << "MM" << std::endl;
        Tmat mB11B22 = pomozno_sestevanje(mat2, mat2,
                                          //B11
                                          b1, bP,
                                          c1, cP,
                                          //B22
                                          bP, b2,
                                          cP, c2);

        std::cout << "M2" << std::endl;
        //M2
        Tmat mA21A22 = pomozno_sestevanje(mat1, mat1,
                                          //A21
                                          aP, a2,
                                          b1, bP,
                                          //A22
                                          aP, a2,
                                          bP, b2);

        std::cout << "MM" << std::endl;
        Tmat maB11 = vrni_obmocje_matrike(mat2,
                                          //B11
                                          b1, bP,
                                          c1, cP);

        std::cout << "M3" << std::endl;
        //M3
        Tmat maA11 = vrni_obmocje_matrike(mat1,
                                          //A11
                                          a1, aP,
                                          b1, bP);
                                          
        Tmat mB12B22 = pomozno_odstevanje(mat2, mat2,
                                          //B12
                                          b1, bP,
                                          cP, c2,
                                          //B22
                                          bP, b2,
                                          cP, c2);

        std::cout << "M4" << std::endl;
        //M4
        Tmat maA22 = vrni_obmocje_matrike(mat1,
                                          //A22
                                          aP, a2,
                                          bP, b2);
                                          
        Tmat mB21B11 = pomozno_odstevanje(mat2, mat2,
                                          //B21
                                          bP, b2,
                                          c1, cP ,
                                          //B11
                                          b1, bP,
                                          c1, cP);

        std::cout << "M5" << std::endl;
        //M5
        Tmat mA11A12 = pomozno_sestevanje(mat1, mat1,
                                          //A11
                                          a1, aP,
                                          b1, bP,
                                          //A12
                                          a1, aP,
                                          bP, b2);

        Tmat maB22 = vrni_obmocje_matrike(mat2,
                                          //B22
                                          bP, b2,
                                          cP, c2);


        std::cout << "M6" << std::endl;
        //M6
        Tmat mA21A11 = pomozno_odstevanje(mat1, mat1,
                                          //A21
                                          aP, a2,
                                          b1, bP,
                                          //A11
                                          a1, aP,
                                          b1, bP);


        Tmat mB11B12 = pomozno_sestevanje(mat2, mat2,
                                          //B11
                                          b1, bP,
                                          c1, cP,
                                          //B12
                                          b1, bP,
                                          cP, c2);


        std::cout << "M7" << std::endl;
        //M7
        Tmat mA12A22 = pomozno_odstevanje(mat1, mat1,
                                          //A12
                                          a1, aP,
                                          bP, b2 ,
                                          //A22
                                          aP, a2,
                                          bP, b2);

        std::cout << "MM" << std::endl;
        Tmat mB21B22 = pomozno_sestevanje(mat2, mat2,
                                          //B21
                                          bP, b2,
                                          c1, cP,
                                          //B22
                                          bP, b2,
                                          cP, c2);

        std::cout << "MM AAA" << std::endl;

        Tmat matM1 = strassen_mul(mA11A22, mB11B22);
        std::cout << "MM AAA1" << std::endl;
        //std::cout << mA21A22.size() << " AA " << mA21A22[0].size() << std::endl;
        //std::cout << maB11.size() << " BB " << maB11[0].size() << std::endl;
        
        Tmat matM2 = strassen_mul(mA21A22, maB11);
        std::cout << "MM AAA2" << std::endl;
        Tmat matM3 = strassen_mul(maA11, mB12B22);
        std::cout << "MM AAA3" << std::endl;
        Tmat matM4 = strassen_mul(maA22, mB21B11);
        std::cout << "MM AAA4" << std::endl;
        Tmat matM5 = strassen_mul(mA11A12, maB22);
        std::cout << "MM AAA5" << std::endl;
        Tmat matM6 = strassen_mul(mA21A11, mB11B12);
        std::cout << "MM AAA6" << std::endl;
        Tmat matM7 = strassen_mul(mA12A22, mB21B22);
        std::cout << "MM AAA7" << std::endl;
        
        std::cout << "MM BBB" << std::endl;

        //C11
        pristej_rezultatu(matM1, mat3, a1, c1);
        pristej_rezultatu(matM4, mat3, a1, c1);
        odstej_rezultatu(matM5, mat3, a1, c1);
        pristej_rezultatu(matM7, mat3, a1, c1);

        //C12
        pristej_rezultatu(matM3, mat3, a1, cP);
        pristej_rezultatu(matM5, mat3, a1, cP);

        //C21
        pristej_rezultatu(matM2, mat3, aP, c1);
        pristej_rezultatu(matM4, mat3, aP, c1);

        //C22
        pristej_rezultatu(matM1, mat3, aP, cP);
        odstej_rezultatu(matM2, mat3, aP, cP);
        pristej_rezultatu(matM3, mat3, aP, cP);
        pristej_rezultatu(matM6, mat3, aP, cP);
    }
}

Tmat strassen_mul(Tmat &mat1, Tmat &mat2){
    int a = mat1.n;
    int b = mat2.n;
    int c = mat2.m;

    Tmat mat3(c,a);
    strassen_part(mat1, mat2, mat3, 0, a, 0, b, 0, c);

    return mat3;
}
