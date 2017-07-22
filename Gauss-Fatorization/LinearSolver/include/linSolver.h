#ifndef LINSOLVER_H
#define LINSOLVER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstring>

using namespace std;

class LinearSystem
{
private:
    int n, m, c;
    double **A;
    double **U;
    double *b;
    double *b_ori;
    double *x;
public:
    LinearSystem (int argc, char *argv[]);
    ~LinearSystem ();
    double** readMatrix (const char m_filename[]);
    double* readRHS (const char rhs_filename[]);
    void solve ();
    void forwardElimination();
    void backSubstitution ();
    void backSubstitution2 ();
    void swap_rows (int i, int j);
    int max_col (int k);
    bool checkSystem ();
    void printMatrix ();
    void printRHS ();
    void printSolution ();
};

void Usage (const char pName[]);

#endif