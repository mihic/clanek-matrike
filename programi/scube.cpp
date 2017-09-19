#include <vector>
#include <algorithm>
#include <iostream>
#include "matrix.h"


Tmat vrni_obmocje_matrike(Tmat &mat1,
                          int x1, int x1max,
                          int x2, int x2max){
                        
    int x1D = x1max - x1;
    int x2D = x2max - x2;
    
    Tmat mat3 = newMat(x2D,x1D);
    for (int i = 0; i<x1D; i+=1){
        for (int j = 0; j<x2D; j+=1){
            mat3[i][j] = mat1[x1+i][x2+j];
        }
    }
    
    return mat3;
}

void pristej_rezultatu(Tmat &mat1, Tmat &mat3,
                       int x1,
                       int x2){

    int x1D = mat1.size();
    int x2D = mat1[0].size();
    
    for (int i = 0; i<x1D; i+=1){
        for (int j = 0; j<x2D; j+=1){
            mat3[x1+i][x2+j] += mat1[i][j];
        }
    }
}

void odstej_rezultatu(Tmat &mat1, Tmat &mat3,
                      int x1,
                      int x2){

    int x1D = mat1.size();
    int x2D = mat1[0].size();
    
    for (int i = 0; i<x1D; i+=1){
        for (int j = 0; j<x2D; j+=1){
            mat3[x1+i][x2+j] -= mat1[i][j];
        }
    }
}

Tmat pomozno_sestevanje(Tmat &mat1, Tmat &mat2,
                        int x1, int x1max,
                        int x2, int x2max, 
                        int y1, int y1max,
                        int y2, int y2max){
                        
    int x1D = x1max - x1;
    int x2D = x2max - x2;
    int y1D = y1max - y1;
    int y2D = y2max - y2;
    
    int iMax = std::max({x1D, y1D});
    int jMax = std::max({x2D, y2D});
    /*
    std::cout << mat1.size() << " size " << mat1[0].size() << std::endl;
    std::cout << "x1" << " " << "x1max" << std::endl;
    std::cout << x1 << " " << x1max << std::endl;
    std::cout << "x2" << " " << "x2max" << std::endl;
    std::cout << x2 << " " << x2max << std::endl;
    std::cout << "y1" << " " << "y1max" << std::endl;
    std::cout << y1 << " " << y1max << std::endl;
    std::cout << "y2" << " " << "y2max" << std::endl;
    std::cout << y2 << " " << y2max << std::endl;
    std::cout << "iMax" << " " << "jMax" << std::endl;
    std::cout << iMax << " " << jMax << std::endl;
    */
    
    Tmat mat3 = newMat(jMax,iMax);
    //std::cout << "AAA" << std::endl;
    for (int i = 0; i<x1D; i+=1){
        for (int j = 0; j<x2D; j+=1){
            mat3[i][j] += mat1[x1+i][x2+j];
        }
    }
    
    //std::cout << "BBB" << std::endl;
    for (int i = 0; i<y1D; i+=1){
        for (int j = 0; j<y2D; j+=1){
            mat3[i][j] += mat2[y1+i][y2+j];
        }
    }
    
    return mat3;
}

Tmat pomozno_odstevanje(Tmat &mat1, Tmat &mat2,
                        int x1, int x1max,
                        int x2, int x2max, 
                        int y1, int y1max,
                        int y2, int y2max){
                        
    int x1D = x1max - x1;
    int x2D = x2max - x2;
    int y1D = y1max - y1;
    int y2D = y2max - y2;
    
    int iMax = std::max({x1D, y1D});
    int jMax = std::max({x2D, y2D});
    
    Tmat mat3 = newMat(jMax,iMax);

    for (int i = 0; i<x1D; i+=1){
        for (int j = 0; j<x2D; j+=1){
            mat3[i][j] += mat1[x1+i][x2+j];
        }
    }
    
    for (int i = 0; i<y1D; i+=1){
        for (int j = 0; j<y2D; j+=1){
            mat3[i][j] -= mat2[y1+i][y2+j];
        }
    }
    
    return mat3;
}

Tmat gl_podkubicen(Tmat &, Tmat &);

void podkubicen_pomozna(Tmat &mat1, Tmat &mat4, Tmat &mat3,
                        int a1, int a2, 
                        int b1, int b2,
                        int c1, int c2){
    
    int aD = a2-a1;
    int bD = b2-b1;
    int cD = c2-c1;
    int aP = a1+aD/2;
    int bP = b1+bD/2;
    int cP = c1+cD/2;
    
    if(std::min({aD, bD, cD})<96){
        rek_tra_mno_pomozna(mat1, mat4, mat3, a1, a2,  b1, b2, c1, c2);
    }
    else{
        Tmat matA1B1 = pomozno_sestevanje(mat1, mat1, 
                                          //A1
                                          a1, aP, 
                                          b1, bP, 
                                          //B1
                                          aP, a2, 
                                          b1, bP);
                                          
        Tmat matX1Y1 = pomozno_sestevanje(mat4, mat4, 
                                          //X1
                                          c1, cP, 
                                          b1, bP, 
                                          //Y1
                                          cP, c2, 
                                          b1, bP);
        
        Tmat matA2B2 = pomozno_sestevanje(mat1, mat1, 
                                          //A2
                                          a1, aP, 
                                          bP, b2, 
                                          //B2
                                          aP, a2, 
                                          bP, b2);
        
        Tmat matX2Y2 = pomozno_sestevanje(mat4, mat4, 
                                          //X2
                                          c1, cP, 
                                          bP, b2, 
                                          //Y2
                                          cP, c2, 
                                          bP, b2);
        
        
        Tmat matA1mB2 = pomozno_odstevanje(mat1, mat1, 
                                          //A1
                                          a1, aP, 
                                          b1, bP, 
                                          //B2
                                          aP, a2, 
                                          bP, b2);
        
        Tmat matX2Y1 = pomozno_sestevanje(mat4, mat4, 
                                          //X2
                                          c1, cP, 
                                          bP, b2, 
                                          //Y1
                                          cP, c2, 
                                          b1, bP);
        
        
        Tmat matA1 = vrni_obmocje_matrike(mat1,
                                          //A1
                                          a1, aP, 
                                          b1, bP);
                                         
        Tmat matX1mX2 = pomozno_odstevanje(mat4, mat4, 
                                          //X1
                                          c1, cP, 
                                          b1, bP, 
                                          //X2
                                          c1, cP, 
                                          bP, b2);
        
        Tmat matA2A1 = pomozno_sestevanje(mat1, mat1, 
                                          //A2
                                          a1, aP, 
                                          bP, b2, 
                                          //A1
                                          a1, aP, 
                                          b1, bP);
                                          
        Tmat matX2 = vrni_obmocje_matrike(mat4,
                                          //X2
                                          c1, cP, 
                                          bP, b2);
        
        Tmat matB2 = vrni_obmocje_matrike(mat1,
                                          //B2
                                          aP, a2, 
                                          bP, b2);
                                          
        Tmat matY2mY1 = pomozno_odstevanje(mat4, mat4, 
                                          //Y2
                                          cP, c2, 
                                          bP, b2, 
                                          //Y1
                                          cP, c2, 
                                          b1, bP);
        
        Tmat matB1b2 = pomozno_sestevanje(mat1, mat1, 
                                          //B1
                                          aP, a2, 
                                          b1, bP, 
                                          //B2
                                          aP, a2, 
                                          bP, b2);
                                          
        Tmat matY1 = vrni_obmocje_matrike(mat4,
                                          //Y1
                                          cP, c2, 
                                          b1, bP);

        Tmat matM1 = gl_podkubicen(matA1B1, matX1Y1);
        Tmat matM2 = gl_podkubicen(matA2B2, matX2Y2);
        Tmat matM3 = gl_podkubicen(matA1mB2, matX2Y1);
        Tmat matM4 = gl_podkubicen(matA1, matX1mX2);
        Tmat matM5 = gl_podkubicen(matA2A1, matX2);
        Tmat matM6 = gl_podkubicen(matB2, matY2mY1);
        Tmat matM7 = gl_podkubicen(matB1b2, matY1);
        
        //A1*
        pristej_rezultatu(matM4, mat3, a1, c1);     
        pristej_rezultatu(matM5, mat3, a1, c1);

        //A2
        pristej_rezultatu(matM2, mat3, a1, cP);
        odstej_rezultatu(matM6, mat3, a1, cP);
        pristej_rezultatu(matM3, mat3, a1, cP);
        odstej_rezultatu(matM5, mat3, a1, cP);
                          
        //B1*                  
        pristej_rezultatu(matM1, mat3, aP, c1);
        odstej_rezultatu(matM4, mat3, aP, c1);
        odstej_rezultatu(matM3, mat3, aP, c1);
        odstej_rezultatu(matM7, mat3, aP, c1);

        //B2*
        pristej_rezultatu(matM6, mat3, aP, cP);
        pristej_rezultatu(matM7, mat3, aP, cP);
    }
}

Tmat gl_podkubicen(Tmat &mat1, Tmat &mat4){
    int a = mat1.size();
    int b = mat1[0].size();
    int c = mat4.size();
    
    Tmat mat3 = newMat(c,a);
    podkubicen_pomozna(mat1, mat4, mat3, 0, a, 0, b, 0, c);
    
    return mat3;
}

Tmat MultiplicationSubcubic(Tmat &mat1, Tmat &mat2){
    Tmat mat4 = transponiraj(mat2);
    return gl_podkubicen(mat1, mat4);
}
