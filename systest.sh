#!/usr/bin/fish

set EF "./a.out"
set OUT "out"
set TD "td"

for i in ( seq $argv[1] )
    if [ $argv[2] = "old" ]
        cat $TD$i | time $EF | tee $OUT$i > /dev/null
    else
        ./testdata_gen $argv[2] $argv[3] | tee $TD$i | time $EF | tee $OUT$i > /dev/null
    end
    if not ./check $TD$i $OUT$i
        exit 1
    end
    sleep 1
end
