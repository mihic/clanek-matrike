#!  /usr/bin/fish

#set algs classic classic_transposed recursive recursive_transposed subcubic blas
#set algs classic classic_transposed
set algs classic
# small tests

echo "alg,a,time" | tee "anomaly_arr.csv"
for alg in $algs
    for j in (seq 1 1 10)
        for i in (seq 1300 1 1310)
            set a (./main -m $alg -a $i -b $i -c $i -r 1)
            echo $alg,$i,$a[1] | tee -a "anomaly_arr.csv"
        end
    end
end
