#ifndef DEF_PLI
#define DEF_PLI

#include <vector>

#include "Carte.h"

class Pli
{
public:
    Pli();
    Pli(int joueur);

    Pli addCarte(Carte);
    Pli vider();
    Pli setJoueurDebut(int joueur);
    int getTaille() const;
    std::vector<Carte> getCartes() const;
	int getPoints() const;
    int getCouleurDemandee() const;
    Carte getDerniereCarte() const;
    Carte getCarteMaitre() const;
    int getJoueurMaitre() const;
	Pli setDixDeDer();

private:
    int joueurDebut;
    std::vector<Carte> cartes;
	int points;
	bool dixDeDer;
};

std::ostream& operator<<(std::ostream& os, const Pli& pli);

#endif
