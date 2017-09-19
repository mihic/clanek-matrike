#!  /usr/bin/fish

#set algs classic classic_transposed recursive recursive_transposed subcubic blas
#set algs classic classic_transposed
set algs recursive recursive_transposed
# small tests

echo "alg,a,time" | tee "tests_recursive_new.csv"
for alg in $algs
    for i in (seq 100 25 2200)
        set a (./main -m $alg -a $i -b $i -c $i -t 10000)
        echo $alg,$i,$a[1] | tee -a "tests_recursive_new.csv"
    end
end
