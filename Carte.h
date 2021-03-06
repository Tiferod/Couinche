#ifndef DEF_CARTE
#define DEF_CARTE

#include <iostream>
#include <string>

class Carte
{
public:
    Carte(int id);
    Carte(int valeur, int couleur);
    Carte(std::string nom);

    int getId() const;
    int getValeur() const;
    int getCouleur() const;
    std::string getNom() const;
	int getPoints() const;
    int getCouleurAnnoncee() const;
    int* getPointeur() const;
	bool isAtout();
    Carte setCouleurAnnoncee(int& couleur);

private:
    int id;
    int valeur;
    int couleur;
    std::string nom;
	int points;

    int *couleurAnnoncee;
};

std::string valeurCouleurToNom(int valeur, int couleur);
int valeurToPoints(int valeur);

int idToOrdre(int n, bool atout);
bool comparaisonParId(Carte c1, Carte c2);
bool operator==(Carte const& a, Carte const& b);
bool operator>(Carte const& a, Carte const& b);
bool operator<(Carte const& a, Carte const& b);
std::ostream& operator<<(std::ostream& os, const Carte& carte);

#endif
