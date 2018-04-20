#ifndef DEF_TABLE
#define DEF_TABLE

#include "Pli.h"

class Table
{
public:

    Table();

    void partieSuivante();
    void distribution();
    void trierJeux();
    void annonces();
    void jouer();
    bool testCarte(int joueur, Carte carte, bool messageDErreur = true) const;
    bool jouerCarte(int joueur, Carte carte);
    void ramasserPli();
    Pli dernierPli() const;
    void belote(int joueur);
    void rebelote(int joueur);

private:
    bool possedeAtoutSuperieur(int joueur, Carte atout) const;
    bool possedeCouleur(int joueur, int couleur) const;

    int joueurDistribuant;
    int joueurActuel;
    int joueurPreneur;
    int couleurAnnoncee;
    int montantAnnonce;
    int coinche;//0=non, 1=oui, 2=surcoinche
    std::vector<Carte> jeux[4];
    Pli pliEnCours;
    std::vector<Pli> plisEquipe0;
    std::vector<Pli> plisEquipe1;
    int scoreEquipe0;
    int scoreEquipe1;
};

int annonceToId(std::string str);

#endif
