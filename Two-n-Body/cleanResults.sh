#!/bin/bash

function clean {
    echo "----------------------------------------------------"
    echo "[!] Cleaning $1 ..."
    cd $1; make clean; cd ..
    echo "----------------------------------------------------"
}

# MAIN SCRIPT
SERIAL_BASIC="Serial-Basic"
SERIAL_REDUCE="Serial-Reduce"
PARALLEL_BASIC="Parallel-Basic"
PARALLEL_REDUCE="Parallel-Reduce"

clean $SERIAL_BASIC
clean $SERIAL_REDUCE
clean $PARALLEL_BASIC
clean $PARALLEL_REDUCE