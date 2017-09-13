#!  /usr/bin/fish

set algs classic classic_transposed recursive recursive_transposed subcubic blas

# small tests

#echo "alg,a,b,c,time" | tee "tests.csv"
for alg in $algs
    for i in (seq 100 50 1000)
        set a (./main -m $alg -a $i -b $i -c $i -t 3000)
        echo $alg,$i,$i,$i,$a[1] | tee -a "tests.csv"
    end
end