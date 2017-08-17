/*
------------------------------------------------------------------------------------------------
    Basic Version of the Two-N-Body algorithm
        Autor: Lucas Berg

- It is possible to build an input with executing the Python script (makeInput.py)

- After an input has been made, you can execute the program by passing the generated file as:

    $ ./twoBody < name_input_file
------------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/twoBody.h"
#include "../include/timer.h"

int main (int argc, char *argv[])
{
    double start, finish, elapsed;
    TwoBody *model = newTwoBody();

    GET_TIME(start);
    solveModel(model);
    GET_TIME(finish);
    elapsed = finish - start;
    printf("Elapsed time = %.10lf\n",elapsed);

    return 0;
}