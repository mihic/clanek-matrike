#!/bin/bash

optimize_recursive(){
    file_name="resoults_recursive.txt"
    resoult_string=""
    a=8
    b=2
    c=64
    d=1500
    e=50
    f=2200
    echo $a
    echo $b
    echo $c
    
    mkdir Output
    resoult_string+="Tests \n"
    
    resoult_string+="Velikosti dim: \n"
    for ((i=$a;i<$c;i+=b)); do
        resoult_string+="${i};"
    done
    resoult_string+="\n"
    
    resoult_string+="Velikosti matrik: \n"
    for ((j=$d;j<$f;j+=$e)); do
        resoult_string+="${j};"
    done
    resoult_string+="\n"
    printf ${resoult_string}
    
    for ((i=$a;i<$c;i+=b)); do
        sed -i "s/.*const int RECT_DIM.*;/const int RECT_DIM = ${i};/" matrix.h
        g++ *.cpp -O3 -march=native -lboost_program_options -o main
        
        printf "Parameter: ${i}\n"
        resoult_string+="Parameter: ${i}\n"
        for ((j=$d;j<$f;j+=$e)); do
            x=$(./main -m recursive_transposed -a ${j} -b ${j} -c ${j} -r 5)
            printf "${j};${x};\n" 
            resoult_string+="${j};${x};\n"
        done
    done
    
    now=$(date +"%T")
    printf "Testing parameters \n" > "Output/${file_name}"
    printf "Current time: $now \n" >> "Output/${file_name}"
    sys_info="$(lscpu)"
    printf "${sys_info}\n" >> "Output/${file_name}"
    resoult_string="${resoult_string}"
    
    best_resoult=$(python3 extractBest.py "${resoult_string}" 2>&1)
    echo "Best const:"
    echo "${best_resoult}"
    
    case "${best_resoult}" in
        ''|*[!0-9]*) echo "error: Best resoult is not a number ${best_resoult}" ; return 1;;
        *) echo "succes: Best resoult is number ${best_resoult}" ;;
    esac
    
    sed -i "s/.*const int RECT_DIM.*;/const int RECT_DIM = ${best_resoult};/" matrix.h
    g++ *.cpp -O3 -march=native -lboost_program_options -o main
    
    printf "Best: \n" >> "Output/${file_name}"
    printf "${best_resoult}\n" >> "Output/${file_name}"
    printf "${resoult_string}" >> "Output/${file_name}"
}

optimize_subcub(){
    file_name="resoults_subcub.txt"
    resoult_string=""
    a=32
    b=16
    c=512
    d=1500
    e=50
    f=2400
    echo $a
    echo $b
    echo $c
    
    mkdir Output
    resoult_string+="Tests \n"
    
    resoult_string+="Velikosti dim: \n"
    for ((i=$a;i<$c;i+=b)); do
        resoult_string+="${i};"
    done
    resoult_string+="\n"
    
    resoult_string+="Velikosti matrik: \n"
    for ((j=$d;j<$f;j+=$e)); do
        resoult_string+="${j};"
    done
    resoult_string+="\n"
    printf ${resoult_string}
    
    for ((i=$a;i<$c;i+=b)); do
        sed -i "s/.*const int SUBC_DIM.*;/const int SUBC_DIM = ${i};/" matrix.h
        g++ *.cpp -O3 -march=native -lboost_program_options -o main
        
        printf "Parameter: ${i}\n"
        resoult_string+="Parameter: ${i}\n"
        for ((j=$d;j<$f;j+=$e)); do
            x=$(./main -m subcubic -a ${j} -b ${j} -c ${j} -r 5)
            printf "${j};${x};\n" 
            resoult_string+="${j};${x};\n"
        done
    done
    
    now=$(date +"%T")
    printf "Testing parameters \n" > "Output/${file_name}"
    printf "Current time: $now \n" >> "Output/${file_name}"
    sys_info="$(lscpu)"
    printf "${sys_info}\n" >> "Output/${file_name}"
    resoult_string="${resoult_string}"
    
    best_resoult=$(python3 extractBest.py "${resoult_string}" 2>&1)
    echo "Best const:"
    echo "${best_resoult}"
    
    case "${best_resoult}" in
        ''|*[!0-9]*) echo "error: Best resoult is not a number ${best_resoult}" ; return 1;;
        *) echo "succes: Best resoult is number ${best_resoult}" ;;
    esac
    
    sed -i "s/.*const int SUBC_DIM.*;/const int SUBC_DIM = ${best_resoult};/" matrix.h
    g++ *.cpp -O3 -march=native -lboost_program_options -o main
    
    printf "Best: \n" >> "Output/${file_name}"
    printf "${best_resoult}\n" >> "Output/${file_name}"
    printf "${resoult_string}" >> "Output/${file_name}"
}

echo "Comand sed can take line number for fast performance for example 7s.."
sed -i "s/.*const int RECT_DIM.*;/const int RECT_DIM = 256;/" matrix.h
sed -i "s/.*const int SUBC_DIM.*;/const int SUBC_DIM = 512;/" matrix.h
sed -i "2s/.*TODO.*;/const int TODO = 2;/" matrix.h
optimize_recursive
sed -i "2s/.*TODO.*;/const int TODO = 1;/" matrix.h
optimize_subcub
sed -i "2s/.*TODO.*;/const int TODO = 0;/" matrix.h

