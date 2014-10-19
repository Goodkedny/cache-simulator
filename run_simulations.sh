#!/bin/sh

#run all configs and traces with each other
#outputs into simulations_results directory

#reset simulation_results directory
touch simulation_results
rm -r simulation_results
mkdir simulation_results

for config in `ls lab2_confs/confs`; do
    echo "Processing $config" 1>&2
    mkdir simulation_results/$config

    for trace in `ls traces`; do
        echo "   Running $trace" 1>&2

        #move files around to work with our interface...
        cp traces/$trace simulation_results/${config}/${trace}
        ./cachesim lab2_confs/confs/$config simulation_results/${config}/${trace}
        rm simulation_results/${config}/${trace}
    done;

done;
