#include "../include/linSolver.h"

void Usage (const char pName[])
{
    cout << "=======================================================================================" << endl;
    cout << "Usage: ./" << pName << " <matrix_file> <right-hand-side_file>" << endl;
    cout << "=======================================================================================" << endl;
}

LinearSystem::LinearSystem (int argc, char *argv[])
{
    A = readMatrix(argv[1]);
    //printMatrix();

    b = readRHS(argv[2]);
    //printRHS();

    solve();
    //printSolution();
}

LinearSystem::~LinearSystem ()
{
    delete [] b;
    delete [] b_ori;
    delete [] x;
    delete [] A;
    delete [] U;
}

double** LinearSystem::readMatrix (const char m_filename[])
{
    ifstream in(m_filename);
    string line;

    getline(in,line);
    in >> n >> m >> c;
    A = new double*[n]();
    A[0] = new double[n*m];
    for (int i = 1; i < n; i++)
        A[i] = A[0] + i*m;
    
    for (int k = 0; k < c; k++)
    {
        int i, j;
        double val;
        in >> i >> j >> val; i--; j--; 
        A[i][j] = val;
    }
    
    in.close();

    return A;
}

void LinearSystem::printMatrix ()
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            cout << A[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

double* LinearSystem::readRHS (const char rhs_filename[])
{
    ifstream in(rhs_filename);
    int trash;
    string line;

    getline(in,line);
    in >> n >> trash;
    b = new double[n]();
    b_ori = new double[n]();
    
    for (int i = 0; i < n; i++)
    {
        double val;
        in >> val; 
        b[i] = val;
        b_ori[i] = val;
    }
    
    in.close();

    return b;
}

void LinearSystem::printRHS ()
{
    for (int i = 0; i < n; i++)
        cout << b[i] << endl;
    cout << endl;
}

void LinearSystem::printSolution ()
{
    for (int i = 0; i < n; i++)
        cout << x[i] << endl;
    cout << endl;
}

void LinearSystem::solve ()
{
    forwardElimination();
    backSubstitution();
    if (checkSystem())
        cout << "A solucoa do sistema esta correta" << endl;
    else
        cout << "A solucoa do sistema NAO esta correta" << endl;
}

void LinearSystem::forwardElimination ()
{
    int r;
    double *l = new double[n]();
    U = new double*[n]();
    U[0] = new double[n*m];
    for (int i = 1; i < n; i++)
        U[i] = U[0] + i*m;
    memcpy(U[0],A[0],sizeof(double)*n*m);

    // For every column
    for (int k = 0; k < n-1; k++)
    {
        r = max_col(k);
        if (r != k) swap_rows(k,r);
        // For every row
        for (int i = k+1; i < n; i++)
        {
            l[i] = U[i][k] / U[k][k];
            // Apply row operation to eliminate the elements below the pivot 
            for (int j = k+1; j < m; j++)
                U[i][j] = U[i][j] - l[i]*U[k][j];
            b[i] = b[i] - l[i]*b[k];
        }
    }

    delete [] l;
}

// Row-oriented
void LinearSystem::backSubstitution ()
{
    x = new double[n]();
    for (int k = n-1; k >= 0; k--)
    {
        double sum = 0.0;
        for (int j = k+1; j < m; j++)
            sum = sum + U[k][j]*x[j];
        x[k] = 1/U[k][k] * (b[k] - sum);
    }
}

// Column-oriented
void LinearSystem::backSubstitution2 ()
{
    x = new double[n]();
    for (int i = 0; i < n; i++)
        x[i] = b[i];

    for (int j = n-1; j >= 0; j--)
    {
        x[j] /= U[j][j];
        for (int i = 0; i < j; i++)
            x[i] -= U[i][j]*x[j];
    }
}

void LinearSystem::swap_rows (int i, int j)
{
    double *aux = U[i];
    U[i] = U[j];
    U[j] = aux;
    
    double aux2;
    aux2 = b[i];
    b[i] = b[j];
    b[j] = aux2;
}

int LinearSystem::max_col (int k)
{
    int maxIndex = k;
    double maxValue = U[k][k];
    for (int i = k; i < n; i++)
    {
        if (U[i][k] > maxValue)
        {
            maxValue = U[i][k];
            maxIndex = i;
        }
    }
    return maxIndex;
}

bool LinearSystem::checkSystem ()
{
    double error = 0.0;
    for (int i = 0; i < n; i++)
    {
        double sum = 0.0;
        for (int j = 0; j < m; j++)
            sum += A[i][j]*x[j];
        error += pow(b_ori[i]-sum,2);
    }
    cout << sqrt(error) << endl;
    if (error > 1.0e-03) return false;
    else                 return true;
}