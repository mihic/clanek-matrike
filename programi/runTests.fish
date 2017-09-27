#!  /usr/bin/fish

#set algs classic classic_transposed recursive recursive_transposed subcubic blas
#set algs classic classic_transposed
set algs strassen
# small tests

echo "alg,a,time" | tee "strassenInT.csv"
for alg in $algs
    for i in (seq 100 100 5000)
        set a (./main -m $alg -a $i -b $i -c $i -t 10000)
        echo $alg,$i,$a[1] | tee -a "strassenInT.csv"
    end
end
