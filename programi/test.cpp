#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <fstream>
#include <functional>
#include <iomanip>
#include <chrono>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/operation.hpp>
#include <boost/numeric/ublas/operation_blocked.hpp>
#include "matrix.h"


void prikaz(Tmat &mat){
    for (auto &vrstica : mat){
        std::cout << "|";
        for (auto &element : vrstica){
            std::cout << std::setw(10) << element;
        }
        std::cout << "|" << std::endl;
    }
    std::cout << std::endl;
}


void zapisi_mat_v_dat(Tmat &mat, std::ofstream& datF){
    datF << "Matrika;v1;";
    int m = mat.size();
    int n = mat[0].size();

    //## velikost se prilagaja za prostor na disku
    int maxMN = (m<n)?n:m;
    int natancnost = 40;
    if(maxMN > 200){
        natancnost = 20;
        int minMN = (m<n)?m:n;
        if(minMN > 200){
            natancnost = 10;
        }
    }

    datF << m << ";" << n << ";";
    datF << "klasicna;double;" << std::endl;
    datF << "ZACETEK;" << std::endl;
    
    for (auto &vrstica : mat){
        for (auto &element : vrstica){
            datF << std::fixed 
                 << std::setprecision(natancnost) 
                 << element << ";";
        }
        datF << std::endl;
    }
    datF << "KONEC;" << std::endl;
}

void zapisi_sez_matrik(std::vector<Tmat> &sezMatrik, 
                       std::vector<std::string> &sezOpomb,
                       std::vector<std::string> &sezNaslov, 
                       std::ofstream& datF){

    int d = sezNaslov.size();
    for(int i=0; i<d; i+=1){
        datF << sezNaslov[i] << std::endl;
    }

    int l = sezMatrik.size();
    for(int i=0; i<l; i+=1){
        datF << sezOpomb[i] << std::endl;
        zapisi_mat_v_dat(sezMatrik[i], datF);
    }
}

Tmat fff_mat_nicelna(int m, int n){
    std::vector<double> nicelni(n, 0.0);
    Tmat mat(m, nicelni);
    return mat;
}

Tmat fff_mat_psevdo_random(int m, int n, int seme, double a, double b){
    std::vector<double> nicelni(n, 0.0);
    Tmat mat(m, nicelni);
    std::uniform_real_distribution<double> zIntervala(a, b);
    
    int novoSeme = (m*n*seme+1)/(m*m+n*n)+seme;
    std::default_random_engine rnd (novoSeme);
    
    for (int i = 0; i<m; i+=1){
        for (int j = 0; j<n; j+=1){
            mat[i][j] = zIntervala(rnd);
        }
    }
    return mat;
}

std::vector<std::string> fff_razbij_vrstico(std::string vrstica, 
                                            std::string locilo){
                                       
    std::vector<std::string> sezNizov;
    int d = locilo.length();
    int i  = 0;
    int j = vrstica.find(locilo, i);
    while(j != std::string::npos){
        sezNizov.push_back(vrstica.substr(i, j - i));
        i = j + d;
        j = vrstica.find(locilo, i);
    }

    return sezNizov;
}

std::vector<double> fff_sez_niz_double(std::vector<std::string> sezNizov){
    std::vector<double> sezStevil;
    int l = sezNizov.size();
    for(int i=0; i<l; i+=1){
        sezStevil.push_back(std::stod(sezNizov[i]));
    }

    return sezStevil;
}

Tmat fff_mat_iz_datoteke(std::ifstream& datF){
    std::string vrstica;
    //std::cout << "NALAGAM:" << std::endl;
    bool jeZacetek = false;
    
    Tmat mat;
    while(datF.good()){
        std::getline(datF, vrstica);
        std::vector<std::string> sezNizov = fff_razbij_vrstico(vrstica,";");
        if(sezNizov.size()>2){
            if(jeZacetek){
                mat.push_back(fff_sez_niz_double(sezNizov));
            }
        }
        else{
            //std::cout << vrstica << std::endl;
            if (sezNizov.size()>0){
                if(sezNizov[0]=="ZACETEK"){
                    jeZacetek = true;
                }
                if(sezNizov[0]=="KONEC"){
                    return mat;
                }
            }
        }
    }
    std::cout << "MANJKA KONEC" << std::endl;
    return mat;
}

void fff_naredi_test(int a, int b, int c, 
                     int seme1, int seme2, 
                     int eMin, int eMax,
                     std::string izhod){
                     
    std::ofstream datG;
    datG.open(izhod);
    datG << "Mnozenje dveh matrik;" << std::endl;
    
    datG << "Matrika1;" << std::endl; 
    Tmat mat1 = fff_mat_psevdo_random(a, b, seme1, eMin, eMax);
    zapisi_mat_v_dat(mat1, datG);
    
    datG << "Matrika2;" << std::endl;
    Tmat mat2 = fff_mat_psevdo_random(b, c, seme2, eMin, eMax);
    zapisi_mat_v_dat(mat2, datG);
}

void fff_zmnozi_iz_dat_BLAS(std::string vhod, std::string izhod, int velikostBloka, bool boIzpis = false){
 
    //>> datF
    std::ifstream datF;
    datF.open(vhod);
    //##smiselno ce vrstica naslova ena
    std::string vrstica;
    std::getline(datF, vrstica);
    //std::cout << vrstica << std::endl;
    
    Tmat mat1 = fff_mat_iz_datoteke(datF);
    Tmat mat2 = fff_mat_iz_datoteke(datF);
    
    datF.close();
    //<< datF
    
    
    
    std::chrono::time_point<std::chrono::steady_clock> _TEST_start, _TEST_end;
    _TEST_start = std::chrono::steady_clock::now();
    
    int m1 = mat1.size();
    int n1 = mat1[0].size();
    
    boost::numeric::ublas::matrix<double> M1(m1, n1);
    for(int i = 0; i < m1; i+=1){
        for(int j = 0; j < n1; j+=1){
            M1(i, j) = mat1[i][j];
        }
    }
    
    int m2 = mat2.size();
    int n2 = mat2[0].size();
    
    boost::numeric::ublas::matrix<double> M2(m2, n2);
    for(int i = 0; i < m2; i+=1){
        for(int j = 0; j < n2; j+=1){
            M2(i, j) = mat2[i][j];
        }
    }
    
    _TEST_end = std::chrono::steady_clock::now();
    auto _TEST_elapsed =std::chrono::duration_cast<std::chrono::milliseconds> (_TEST_end - _TEST_start);
    //std::cout << "Cas pretvorbe v BLAS:" << _TEST_elapsed.count() << std::endl;
    
    _TEST_start = std::chrono::steady_clock::now();
    
    boost::numeric::ublas::matrix<double> M3(m1, n2);
    //##velikost bloka zeli imeti kot konstanto
    if (velikostBloka>500){
        if (velikostBloka>2000){
            M3 = boost::numeric::ublas::block_prod<boost::numeric::ublas::matrix<double>, 4096>(M1, M2);
        }
        else{
            M3 = boost::numeric::ublas::block_prod<boost::numeric::ublas::matrix<double>, 1024>(M1, M2);
        }
    }
    else{
        if (velikostBloka>50){
            if (velikostBloka>100){
                M3 = boost::numeric::ublas::block_prod<boost::numeric::ublas::matrix<double>, 256>(M1, M2);
            }
            else{
                M3 = boost::numeric::ublas::block_prod<boost::numeric::ublas::matrix<double>, 64>(M1, M2);
            }
        }
        else{
            if (velikostBloka>10){
                M3 = boost::numeric::ublas::block_prod<boost::numeric::ublas::matrix<double>, 16>(M1, M2);
            }
            else{
                M3 = boost::numeric::ublas::block_prod<boost::numeric::ublas::matrix<double>, 4>(M1, M2);
            } 
        }   
    }
    
    _TEST_end = std::chrono::steady_clock::now();
    _TEST_elapsed =std::chrono::duration_cast<std::chrono::milliseconds> (_TEST_end - _TEST_start);
    //std::cout << "Cas mnozenja:" << _TEST_elapsed.count() << std::endl;

    _TEST_start = std::chrono::steady_clock::now();
    
    std::vector<double> nicelni(n2, 0.0);
    Tmat mat3(m1, nicelni);
    
    for(int i = 0; i < m1; i+=1){
        for(int j = 0; j < n2; j+=1){
            mat3[i][j] = M3(i, j);
        }
    }
    
    _TEST_end = std::chrono::steady_clock::now();
    _TEST_elapsed =std::chrono::duration_cast<std::chrono::milliseconds> (_TEST_end - _TEST_start);
    //std::cout << "Cas pretvorbe iz BLAS:" << _TEST_elapsed.count() << std::endl;
    
    
    
    Tmat tabelaMatrik[] = {mat1,mat2,mat3};
    std::string tabelaOpomb[] = {"ZMNOZI;","ZMNOZI;","REZULTAT;"};
    std::string tabelaNaslov[] = {"Zmnozek dveh matrik;"};
    
    std::vector<Tmat> sezMatrik(begin(tabelaMatrik), end(tabelaMatrik));
    std::vector<std::string> sezOpomb(begin(tabelaOpomb), end(tabelaOpomb));
    std::vector<std::string> sezNaslov(begin(tabelaNaslov), end(tabelaNaslov));
    
    if(boIzpis){
        //>> datG
        std::ofstream datG;
        datG.open(izhod);
        zapisi_sez_matrik(sezMatrik, sezOpomb, sezNaslov, datG);
        datG.close();
        //<< datG
    }                        
}

void fff_zmnozi_iz_dat(std::function<Tmat(Tmat &,Tmat &)> mnozneje_mat, 
                       std::string vhod, std::string izhod, bool boIzpis = false){
                       
    //>> datF
    std::ifstream datF;
    datF.open(vhod);
    //##smiselno ce vrstica naslova ena
    std::string vrstica;
    std::getline(datF, vrstica);
    //std::cout << vrstica << std::endl;
    
    Tmat mat1 = fff_mat_iz_datoteke(datF);
    Tmat mat2 = fff_mat_iz_datoteke(datF);
    
    datF.close();
    //<< datF
    
    std::chrono::time_point<std::chrono::steady_clock> _TEST_start, _TEST_end;
    _TEST_start = std::chrono::steady_clock::now();
    
    Tmat mat3 = mnozneje_mat(mat1,mat2);
    
    _TEST_end = std::chrono::steady_clock::now();
    auto _TEST_elapsed =std::chrono::duration_cast<std::chrono::milliseconds> (_TEST_end - _TEST_start);
    //std::cout << "Cas mnozenja:" << _TEST_elapsed.count() << std::endl;
    
    Tmat tabelaMatrik[] = {mat1,mat2,mat3};
    std::string tabelaOpomb[] = {"ZMNOZI;","ZMNOZI;","REZULTAT;"};
    std::string tabelaNaslov[] = {"Zmnozek dveh matrik;"};
    
    std::vector<Tmat> sezMatrik(begin(tabelaMatrik), end(tabelaMatrik));
    std::vector<std::string> sezOpomb(begin(tabelaOpomb), end(tabelaOpomb));
    std::vector<std::string> sezNaslov(begin(tabelaNaslov), end(tabelaNaslov));
    
    if(boIzpis){
        //>> datG
        std::ofstream datG;
        datG.open(izhod);
        zapisi_sez_matrik(sezMatrik, sezOpomb, sezNaslov, datG);
        datG.close();
        //<< datG
    }
}

/*
void splosni_stari_testi(){
    std::string imeTesta = "t_3s2s1s1701c";
    fff_naredi_test(300, 200, 100, 3, 7, 
                    0, 1, imeTesta+".csv");
         
    std::string imeFunkcije;
    std::function<Tmat(Tmat &,Tmat &)> mnozneje_mat;
    std::string imeRezultata;
    //>>
    imeFunkcije = "kl";
    mnozneje_mat = klasicno_mnozenje;
    
    imeRezultata = imeTesta+"_r_"+imeFunkcije;
    std::cout << imeFunkcije << std::endl;
    fff_zmnozi_iz_dat(mnozneje_mat, 
                      imeTesta+".csv", 
                      imeRezultata+".csv");
    //<<
    
    //>>
    imeFunkcije = "tr";
    mnozneje_mat = transponirano_mnozenje;
    
    imeRezultata = imeTesta+"_r_"+imeFunkcije;
    std::cout << imeFunkcije << std::endl;
    fff_zmnozi_iz_dat(mnozneje_mat, 
                      imeTesta+".csv", 
                      imeRezultata+".csv");
    //<<
    
    //>>
    imeFunkcije = "re4";
    mnozneje_mat = rekurzivno_mnozenje;
    
    imeRezultata = imeTesta+"_r_"+imeFunkcije;
    std::cout << imeFunkcije << std::endl;
    fff_zmnozi_iz_dat(mnozneje_mat, 
                      imeTesta+".csv", 
                      imeRezultata+".csv");
    //<<
    
    //>>
    imeFunkcije = "re4tr";
    mnozneje_mat = rekurzivno_transponirano_mnozenje;
    
    imeRezultata = imeTesta+"_r_"+imeFunkcije;
    std::cout << imeFunkcije << std::endl;
    fff_zmnozi_iz_dat(mnozneje_mat, 
                      imeTesta+".csv", 
                      imeRezultata+".csv");
    //<<
    
    //>>
    imeFunkcije = "podku";
    mnozneje_mat = podkubicen_enkrat;
    
    imeRezultata = imeTesta+"_r_"+imeFunkcije;
    std::cout << imeFunkcije << std::endl;
    fff_zmnozi_iz_dat(mnozneje_mat, 
                      imeTesta+".csv", 
                      imeRezultata+".csv");
    //<<
    
    //>>
    imeFunkcije = "podku2";
    mnozneje_mat = podkubicen;
    
    imeRezultata = imeTesta+"_r_"+imeFunkcije;
    std::cout << imeFunkcije << std::endl;
    fff_zmnozi_iz_dat(mnozneje_mat, 
                      imeTesta+".csv", 
                      imeRezultata+".csv");
    //<<
    
    int velikostBloka;
    //>>
    velikostBloka = 4048;
    //velikostBloka = 4096;
    //velikostBloka = 1024;
    //velikostBloka = 256;
    //velikostBloka = 64;
    //velikostBloka = 16;
    //velikostBloka = 4;
    
    imeFunkcije = "BLAS"+std::to_string(velikostBloka);
    
    imeRezultata = imeTesta+"_r_"+imeFunkcije;
    std::cout << imeFunkcije << std::endl;
    fff_zmnozi_iz_dat_BLAS(imeTesta+".csv", imeRezultata+".csv", velikostBloka);
    //<<

    //>>
    velikostBloka = 1024;
    imeFunkcije = "BLAS"+std::to_string(velikostBloka);
    
    imeRezultata = imeTesta+"_r_"+imeFunkcije;
    std::cout << imeFunkcije << std::endl;
    fff_zmnozi_iz_dat_BLAS(imeTesta+".csv", imeRezultata+".csv", velikostBloka);
    //<< 

    //>>
    velikostBloka = 256;
    imeFunkcije = "BLAS"+std::to_string(velikostBloka);
    
    imeRezultata = imeTesta+"_r_"+imeFunkcije;
    std::cout << imeFunkcije << std::endl;
    fff_zmnozi_iz_dat_BLAS(imeTesta+".csv", imeRezultata+".csv", velikostBloka);
    //<<  
    
    //>>
    velikostBloka = 64;
    imeFunkcije = "BLAS"+std::to_string(velikostBloka);
    
    imeRezultata = imeTesta+"_r_"+imeFunkcije;
    std::cout << imeFunkcije << std::endl;
    fff_zmnozi_iz_dat_BLAS(imeTesta+".csv", imeRezultata+".csv", velikostBloka);
    //<< 
    
    //>>
    velikostBloka = 16;
    imeFunkcije = "BLAS"+std::to_string(velikostBloka);
    
    imeRezultata = imeTesta+"_r_"+imeFunkcije;
    std::cout << imeFunkcije << std::endl;
    fff_zmnozi_iz_dat_BLAS(imeTesta+".csv", imeRezultata+".csv", velikostBloka);
    //<<  
    
    //>>
    velikostBloka = 4;
    imeFunkcije = "BLAS"+std::to_string(velikostBloka);
    
    imeRezultata = imeTesta+"_r_"+imeFunkcije;
    std::cout << imeFunkcije << std::endl;
    fff_zmnozi_iz_dat_BLAS(imeTesta+".csv", imeRezultata+".csv", velikostBloka);
    //<<  
}
*/


void fff_zmnozi_iz_dat_BLAS_rez(std::string vhod, std::string izhod, 
                                int velikostBloka, std::ofstream &datR, bool boIzpis = false){
 
    //>> datF
    std::ifstream datF;
    datF.open(vhod);
    //##smiselno ce vrstica naslova ena
    std::string vrstica;
    std::getline(datF, vrstica);
    //std::cout << vrstica << std::endl;
    
    Tmat mat1 = fff_mat_iz_datoteke(datF);
    Tmat mat2 = fff_mat_iz_datoteke(datF);
    
    datF.close();
    //<< datF
    
    
    
    std::chrono::time_point<std::chrono::steady_clock> _TEST_start, _TEST_end;
    _TEST_start = std::chrono::steady_clock::now();
    
    int m1 = mat1.size();
    int n1 = mat1[0].size();
    
    boost::numeric::ublas::matrix<double> M1(m1, n1);
    for(int i = 0; i < m1; i+=1){
        for(int j = 0; j < n1; j+=1){
            M1(i, j) = mat1[i][j];
        }
    }
    
    int m2 = mat2.size();
    int n2 = mat2[0].size();
    
    boost::numeric::ublas::matrix<double> M2(m2, n2);
    for(int i = 0; i < m2; i+=1){
        for(int j = 0; j < n2; j+=1){
            M2(i, j) = mat2[i][j];
        }
    }
    
    _TEST_end = std::chrono::steady_clock::now();
    auto _TEST_elapsed =std::chrono::duration_cast<std::chrono::milliseconds> (_TEST_end - _TEST_start);
    //std::cout << "Cas pretvorbe v BLAS:" << _TEST_elapsed.count() << std::endl;
    
    _TEST_start = std::chrono::steady_clock::now();
    
    boost::numeric::ublas::matrix<double> M3(m1, n2);
    //##velikost bloka zeli imeti kot konstanto
    if (velikostBloka>500){
        if (velikostBloka>2000){
            M3 = boost::numeric::ublas::block_prod<boost::numeric::ublas::matrix<double>, 8192>(M1, M2);
        }
        else{
            M3 = boost::numeric::ublas::block_prod<boost::numeric::ublas::matrix<double>, 1024>(M1, M2);
        }
    }
    else{
        if (velikostBloka>50){
            if (velikostBloka>100){
                M3 = boost::numeric::ublas::block_prod<boost::numeric::ublas::matrix<double>, 256>(M1, M2);
            }
            else{
                M3 = boost::numeric::ublas::block_prod<boost::numeric::ublas::matrix<double>, 64>(M1, M2);
            }
        }
        else{
            if (velikostBloka>10){
                M3 = boost::numeric::ublas::block_prod<boost::numeric::ublas::matrix<double>, 16>(M1, M2);
            }
            else{
                M3 = boost::numeric::ublas::block_prod<boost::numeric::ublas::matrix<double>, 4>(M1, M2);
            } 
        }   
    }
    
    _TEST_end = std::chrono::steady_clock::now();
    _TEST_elapsed =std::chrono::duration_cast<std::chrono::milliseconds> (_TEST_end - _TEST_start);
    
    int a = mat1.size();
    int b = mat1[0].size();
    int c = mat2[0].size();
    //##datR << dim1 << ";" << dim2 << ";" << dim3 << ";" << volumen << ";" << cas << ";" << prvi << ";" << std::endl;
    datR << a << ";" << b << ";" << c << ";" << ((long long int) a*b*c) << ";" << _TEST_elapsed.count() << ";" << M3(0,0) << ";" << std::endl;

    _TEST_start = std::chrono::steady_clock::now();
    
    std::vector<double> nicelni(n2, 0.0);
    Tmat mat3(m1, nicelni);
    
    for(int i = 0; i < m1; i+=1){
        for(int j = 0; j < n2; j+=1){
            mat3[i][j] = M3(i, j);
        }
    }
    
    _TEST_end = std::chrono::steady_clock::now();
    _TEST_elapsed =std::chrono::duration_cast<std::chrono::milliseconds> (_TEST_end - _TEST_start);
    //std::cout << "Cas pretvorbe iz BLAS:" << _TEST_elapsed.count() << std::endl;
    
    
    
    Tmat tabelaMatrik[] = {mat1,mat2,mat3};
    std::string tabelaOpomb[] = {"ZMNOZI;","ZMNOZI;","REZULTAT;"};
    std::string tabelaNaslov[] = {"Zmnozek dveh matrik;"};
    
    std::vector<Tmat> sezMatrik(begin(tabelaMatrik), end(tabelaMatrik));
    std::vector<std::string> sezOpomb(begin(tabelaOpomb), end(tabelaOpomb));
    std::vector<std::string> sezNaslov(begin(tabelaNaslov), end(tabelaNaslov));
    
    if(boIzpis){
        //>> datG
        std::ofstream datG;
        datG.open(izhod);
        zapisi_sez_matrik(sezMatrik, sezOpomb, sezNaslov, datG);
        datG.close();
        //<< datG
    }                        
}

void fff_zmnozi_iz_dat_rez(std::function<Tmat(Tmat &,Tmat &)> mnozneje_mat, 
                           std::string vhod, std::string izhod, std::ofstream &datR, bool boIzpis = false){
                       
    //>> datF
    std::ifstream datF;
    datF.open(vhod);
    //##smiselno ce vrstica naslova ena
    std::string vrstica;
    std::getline(datF, vrstica);
    //std::cout << vrstica << std::endl;
    
    Tmat mat1 = fff_mat_iz_datoteke(datF);
    Tmat mat2 = fff_mat_iz_datoteke(datF);
    
    datF.close();
    //<< datF
    
    std::chrono::time_point<std::chrono::steady_clock> _TEST_start, _TEST_end;
    _TEST_start = std::chrono::steady_clock::now();
    
    Tmat mat3 = mnozneje_mat(mat1,mat2);
    
    _TEST_end = std::chrono::steady_clock::now();
    auto _TEST_elapsed =std::chrono::duration_cast<std::chrono::milliseconds> (_TEST_end - _TEST_start);
    
    int a = mat1.size();
    int b = mat1[0].size();
    int c = mat2[0].size();
    //##datR << dim1 << ";" << dim2 << ";" << dim3 << ";" << volumen << ";" << cas << ";" << prvi << ";" << std::endl;
    datR << a << ";" << b << ";" << c << ";" << ((long long int) a*b*c) << ";" << _TEST_elapsed.count() << ";" << mat3[0][0] << ";" << std::endl;
    
    Tmat tabelaMatrik[] = {mat1,mat2,mat3};
    std::string tabelaOpomb[] = {"ZMNOZI;","ZMNOZI;","REZULTAT;"};
    std::string tabelaNaslov[] = {"Zmnozek dveh matrik;"};
    
    std::vector<Tmat> sezMatrik(begin(tabelaMatrik), end(tabelaMatrik));
    std::vector<std::string> sezOpomb(begin(tabelaOpomb), end(tabelaOpomb));
    std::vector<std::string> sezNaslov(begin(tabelaNaslov), end(tabelaNaslov));
    
    if(boIzpis){
        //>> datG
        std::ofstream datG;
        datG.open(izhod);
        zapisi_sez_matrik(sezMatrik, sezOpomb, sezNaslov, datG);
        datG.close();
        //<< datG
    }
}

void testiraj_enkrat(std::string imeTesta, std::string imeFunkcije, 
                     std::function<Tmat(Tmat &,Tmat &)> mnozneje_mat, std::ofstream &datR){
         
    std::string imeRezultata = imeTesta+"_r_"+imeFunkcije;
    fff_zmnozi_iz_dat_rez(mnozneje_mat, 
                          imeTesta+".csv", 
                          imeRezultata+".csv",
                          datR);
}

void testiraj_BLAS(std::string imeTesta, std::string imeFunkcije, 
                   int velikostBloka, std::ofstream &datR){
         
    std::string imeRezultata = imeTesta+"_r_"+imeFunkcije;
    fff_zmnozi_iz_dat_BLAS_rez(imeTesta+".csv", imeRezultata+".csv", velikostBloka, datR);
}

std::vector<std::string> naredi_testne_primere(std::vector<int> sezVelikosti){

    int l = sezVelikosti.size();
    std::vector<std::string> sezTestov(l,"");
    for (int i=0; i<l; i+=1){
        int velikost = sezVelikosti[i];
        std::string imeTesta = "testne_matrike/test"+std::to_string(i)+"_"+std::to_string(velikost);
        sezTestov[i] = imeTesta;
        fff_naredi_test(velikost, velikost, velikost, i+1, i*i, 0, 1, imeTesta+".csv");
    }
    return sezTestov;
}

void novejsi_testi(std::string izhodRez,std::vector<int> sezVelikosti){
    int stTestov = sezVelikosti.size();

    std::vector<std::string> sezTestov = naredi_testne_primere(sezVelikosti);
    std::vector<std::function<Tmat(Tmat &,Tmat &)>> sezAlgoritmov({
        klasicno_mnozenje,
        transponirano_mnozenje,
        rekurzivno_mnozenje,
        rekurzivno_transponirano_mnozenje,
        podkubicen
     });
     std::vector<std::string> sezImen({
        "klasicno_mnozenje",
        "transponirano_mnozenje",
        "rekurzivno_mnozenje",
        "rekurzivno_transponirano",
        "podkubicen_algoritem"
     });
     
     std::vector<int> sezBLAS({
        4048,
        1024,
        256,
        64,
        16,
        4
     });
     
     std::ofstream datR;
     datR.open(izhodRez);
     
     int stAlgoritmov = sezAlgoritmov.size();
     for (int i=0; i<stAlgoritmov; i+=1){
        std::string imeFunkcije = sezImen[i];
        std::cout << imeFunkcije << std::endl;
        datR << ";" << std::endl;
        datR << imeFunkcije << ";" << std::endl;
        datR << "dim1" << ";" << "dim2" << ";" << "dim3" << ";" << "volumen" << ";" << "cas" << ";" << "prvi" << ";" << std::endl;
        for (int j=0; j<stTestov; j+=1){
            testiraj_enkrat(sezTestov[j], imeFunkcije, sezAlgoritmov[i], datR);
        }
     }
     int stBLAS = sezBLAS.size();
     for (int i=0; i<stBLAS; i+=1){
        int velikostBloka = sezBLAS[i];
        std::string imeFunkcije = "BLAS"+std::to_string(velikostBloka);
        std::cout << imeFunkcije << std::endl;
        datR << ";" << std::endl;
        datR << imeFunkcije << ";" << std::endl;
        datR << "dim1" << ";" << "dim2" << ";" << "dim3" << ";" << "volumen" << ";" << "cas" << ";" << "prvi" << ";" << std::endl;
        for (int j=0; j<stTestov; j+=1){
            testiraj_BLAS(sezTestov[j], imeFunkcije, velikostBloka, datR);
        }
     }
     
     datR.close();
}

int main (){
    //splosni_stari_testi();
    
    int stTestov = 5;
    std::string izhodRez = "rez.txt";
    std::vector<int> sezVelikosti(stTestov,0);
    for (int i=0; i<stTestov; i+=1){
        sezVelikosti[i] = 32+64*i;
    }
    novejsi_testi(izhodRez, sezVelikosti);
    
    return 0;
}

