#ifndef DEF_TABLE
#define DEF_TABLE

#include "Pli.h"

class Table
{
public:

    Table();

    void partieSuivante();
    void distribution();
    void annonces();
    void jouer();
    bool testCarte(int joueur, Carte carte, bool messageDErreur = true) const;
    bool jouerCarte(int joueur, Carte carte);
    void ramasserPli();
	int finPartie();

private:
    bool possedeAtoutSuperieur(int joueur, Carte atout) const;
    bool possedeCouleur(int joueur, int couleur) const;
	int pointsCoinche(bool attaque);

    int joueurDistribuant;
    int joueurActuel;
    int joueurPreneur;
    int couleurAnnoncee;
    int montantAnnonce;
    int coinche;//0=non, 1=oui, 2=surcoinche
	int litige;
    std::vector<Carte> jeux[4];
    Pli pliEnCours;
    std::vector<Pli> plisEquipe0;
    std::vector<Pli> plisEquipe1;
    int scoreEquipe[2];
	int belote;//-1=pas de belote, 0=joueur 0,etc
};

int annonceToId(std::string str);
std::string idToAnnonce(int id);

#endif
