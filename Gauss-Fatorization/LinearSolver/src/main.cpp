#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "../include/linSolver.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc-1 < 1)
    {
        Usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    else
    {
        LinearSystem *ls = new LinearSystem(argc,argv);
        delete ls;
    }
    return 0;
}