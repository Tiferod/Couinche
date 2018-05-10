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
	int scoreMax = 1000;
	string pause = "";
	cout << "Choisissez le score de fin de partie : ";
	cin >> scoreMax;
    Table table = Table();
	do
	{
		cout << "Appuyez sur Entrée pour passer à la partie suivante.";
		cin >> pause;
		table.partieSuivante();
		table.distribution();
		table.annonces();
		table.jouer();
	} while (table.finPartie() < scoreMax); 
    return 0;
}
