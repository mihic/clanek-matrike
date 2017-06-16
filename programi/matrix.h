#ifndef ALGORITMI_H
#define ALGORITMI_H

typedef std::vector<std::vector<double>> Tmat;

Tmat transponiraj(Tmat &mat);
Tmat transponirano_mnozenje(Tmat &, Tmat &);
Tmat klasicno_mnozenje(Tmat &, Tmat &);
Tmat rekurzivno_mnozenje(Tmat &, Tmat &);
Tmat rekurzivno_transponirano_mnozenje(Tmat &, Tmat &);
Tmat podkubicen(Tmat &, Tmat &);
void rek_tra_mno_pomozna(Tmat &mat1, Tmat &mat4, Tmat &mat3,
                         int a1, int a2, 
                         int b1, int b2,
                         int c1, int c2);

#endif
