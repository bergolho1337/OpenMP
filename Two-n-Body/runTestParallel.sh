#!/bin/bash

function RunSimulation {
    echo "Number of threads $1"
    echo "---------------------------------------------------------"
    echo "[!] Running with 10 particles ..."
    ./twoBody $1 < ../Input/input10
    echo "---------------------------------------------------------"
    echo "---------------------------------------------------------"
    echo "[!] Running with 20 particles ..."
    ./twoBody $1 < ../Input/input20
    echo "---------------------------------------------------------"
    echo "---------------------------------------------------------"
    echo "[!] Running with 40 particles ..."
    ./twoBody $1 < ../Input/input40
    echo "---------------------------------------------------------"
    echo "---------------------------------------------------------"
    echo "[!] Running with 80 particles ..."
    ./twoBody $1 < ../Input/input80
    echo "---------------------------------------------------------"
    echo "---------------------------------------------------------"
    echo "[!] Running with 160 particles ..."
    ./twoBody $1 < ../Input/input160
    echo "---------------------------------------------------------"
    echo "---------------------------------------------------------"
    echo "[!] Running with 320 particles ..."
    ./twoBody $1 < ../Input/input320
    echo "---------------------------------------------------------"
}


# ================== MAIN SCRIPT ===================================
BASIC="Parallel-Basic"
REDUCE="Parallel-Reduce"
THREADS=( 1 2 4 )

# Run the simulations with the Basic solver first
#echo "=========================================================="
#echo "[!] BASIC SOLVER"
#if [ ! -f $BASIC/twoBody ]; then
#    cd $BASIC; make; cd ..
#fi

#cd $BASIC
#for THREAD in ${THREADS[@]}; do
#    echo "=========================================================="
#    RunSimulation $THREAD
#    echo "=========================================================="
#done 
#cd ..

# Run the simulations with the Reduce solver
echo "=========================================================="
echo "[!] REDUCE SOLVER"
if [ ! -f $REDUCE/twoBody ]; then
    cd $REDUCE; make; cd ..
fi

cd $REDUCE
for THREAD in ${THREADS[@]}; do
    echo "=========================================================="
    RunSimulation $THREAD
    echo "=========================================================="
done
cd ..

