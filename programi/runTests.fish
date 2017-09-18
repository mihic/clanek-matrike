#!  /usr/bin/fish

set algs classic classic_transposed recursive recursive_transposed subcubic blas
set algs classic classic_transposed

# small tests

echo "alg,a,b,c,time" | tee "tests_classic_large25.csv"
for alg in $algs
    for i in (seq 100 25 2200)
        set a (./main -m $alg -a $i -b $i -c $i -t 10000)
        echo $alg,$i,$i,$i,$a[1] | tee -a "tests_classic_large25.csv"
    end
end
