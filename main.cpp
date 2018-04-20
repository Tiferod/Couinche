#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <string>
#include <algorithm>

#include "Table.h"

using namespace std;

int main()
{
    srand (time(NULL));
    Table table = Table();
    table.partieSuivante();
    table.distribution();
    table.annonces();
    table.jouer();

    return 0;
}
