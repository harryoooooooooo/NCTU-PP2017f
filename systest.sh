#!/usr/bin/fish

for i in ( seq $argv[1] )
    ./testdata_gen $argv[2] $argv[3] | tee td$i | time ./brute_force | tee out$i > /dev/null
    if not ./check td$i out$i
        exit 1
    end
    sleep 1
end
