#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <algorithm>

#include "Table.h"

using namespace std;

int main()
{
    srand (time(0));
    Table table = Table();
    table.partieSuivante();
    table.distribution();
    table.annonces();
    table.jouer();
	table.finPartie();

    return 0;
}
