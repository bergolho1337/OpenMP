#!/bin/bash

function RunSimulation {
    TAM=( 64 )
    echo "Number of processes $1"
    echo "---------------------------------------------------------"
    
    for T in ${TAM[@]}; do
	echo "---------------------------------------------------------"    	
	echo "[!] Running with [n = $T ] ..."
     	for i in $(seq 1 $3); do
        	echo "Running block schedule ..."
            export OMP_SCHEDULE="static,16"
            ./$2 $1 0 3 $T > ./Outputs/block.txt
            echo "Running cyclic schedule ..."
            export OMP_SCHEDULE="static,1"
            ./$2 $1 0 3 $T > ./Outputs/cyclic.txt
            echo "Running block/cyclic schedule ..."
            export OMP_SCHEDULE="static,4"
            ./$2 $1 0 3 $T > ./Outputs/block-cyclic.txt
            echo "Running dynamic schedule ..."
            export OMP_SCHEDULE="dynamic"
            ./$2 $1 0 3 $T > ./Outputs/dynamic.txt
            echo "Running guided schedule ..."
            export OMP_SCHEDULE="guided"
            ./$2 $1 0 3 $T > ./Outputs/guided.txt
		done
    	#./average.sh ./Outputs/times >> ./Outputs/output$T
    	#rm ./Outputs/times
    	echo "---------------------------------------------------------"
    done
}


# ================== MAIN SCRIPT ===================================
EXECUTABLE="omp_code"
#THREADS=( 1 2 4 8 16 )
NUMRUNS=1
THREADS=( 4 )

# Run the simulations with given the executable
echo "========================================================================="
echo "[!] Running speedup simulation for the executable [$EXECUTABLE]"
if [ ! -f $EXECUTABLE ]; then
    make
else
    make clean; make
fi

for THREAD in ${THREADS[@]}; do
    echo "=========================================================="
    echo "$THREAD" >> ./Outputs/output
    RunSimulation $THREAD $EXECUTABLE $NUMRUNS
    echo "=========================================================="
done

#CMD=""
#FOLDERS=$(ls ./Outputs | sort)
#for FOLDER in ${FOLDERS[@]}; do
#    CMD=$CMD$FOLDER" "
#done
#cd ./Outputs
#paste $CMD > ../Plot/times.dat
#cd ..

#make plot
