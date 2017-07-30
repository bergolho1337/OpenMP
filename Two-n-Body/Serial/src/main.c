#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/twoBody.h"

int main (int argc, char *argv[])
{
    TwoBody *model = newTwoBody();
    solveModel(model);
    return 0;
}