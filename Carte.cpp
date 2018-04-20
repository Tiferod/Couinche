#include <iostream>

#include "Carte.h"

using namespace std;

Carte::Carte(int id1)
{
    if (id1 > 32 || id1 < 0)
    {
        cout<< id1 << " n'est pas un id valide pour une carte (il doit etre entre 0 et 32).";
    }
    else
    {
        this->id = id1;
        if (id == 32)
        {
            nom = "Joker";
            valeur = -1;
            couleur = -1;
        }
        else
        {
            valeur = id % 8;
            switch(valeur) {
                case 0 : nom = "7 ";
                break;
                case 1 : nom = "8 ";
                break;
                case 2 : nom = "9 ";
                break;
                case 3 : nom = "10 ";
                break;
                case 4 : nom = "V ";
                break;
                case 5 : nom = "D ";
                break;
                case 6 : nom = "R ";
                break;
                case 7 : nom = "A ";
                break;
            }
            couleur = id / 8;
            switch(couleur) {
                case 0 : nom += "CA";
                break;
                case 1 : nom += "CO";
                break;
                case 2 : nom += "PI";
                break;
                case 3 : nom += "TR";
                break;
            }
        }
    }
}
Carte::Carte(int valeur1, int couleur1) : valeur(valeur1), couleur(couleur1)
{
    if (valeur == -1 && couleur == -1)
    {
        Carte(32);
    }
    else
    {
        if (valeur < 0 || valeur > 7)
        {
        cout<<"La valeur " << valeur1 << " n'est pas valide pour une carte." << endl;
        }
        if (couleur < 0 || couleur > 3)
        {
        cout<<"La couleur " << couleur1 << " n'est pas valide pour une carte." << endl;
        }
        Carte(valeur1 + 4*couleur1);
    }
}
Carte::Carte(std::string nom1) : nom(nom1)
{
    if (nom1 == "Joker")
    {
        Carte(32);
    }
    else
    {
        int n = nom1.find(" ");
        string strValeur = nom1.substr(0, n);
        string strCouleur = nom1.substr(n+1);

        if (strValeur == "7")
        {
            valeur = 0;
        }
        else if (strValeur == "8")
        {
            valeur = 1;
        }
        else if (strValeur == "9")
        {
            valeur = 2;
        }
        else if (strValeur == "10")
        {
            valeur = 3;
        }
        else if (strValeur == "V")
        {
            valeur = 4;
        }
        else if (strValeur == "D")
        {
            valeur = 5;
        }
        else if (strValeur == "R")
        {
            valeur = 6;
        }
        else if (strValeur == "A")
        {
            valeur = 7;
        }
        else
        {
            cout<< strValeur << " n'est pas une valeur valide pour une carte.";
        }

        if (strCouleur == "CA")
        {
            couleur = 0;
        }
        else if (strCouleur == "CO")
        {
            couleur = 1;
        }
        else if (strCouleur == "PI")
        {
            couleur = 2;
        }
        else if (strCouleur == "TR")
        {
            couleur = 3;
        }
        else
        {
            cout<< strCouleur << " n'est pas une couleur valide pour une carte.";
        }

        id = valeur + 4*couleur;
    }
}
int Carte::getId() const
{
    return id;
}
int Carte::getValeur() const
{
    return valeur;
}
int Carte::getCouleur() const
{
    return couleur;
}
string Carte::getNom() const
{
    return nom;
}
int Carte::getCouleurAnnoncee() const
{
    return *couleurAnnoncee;
}
int* Carte::getPointeur() const
{
    return couleurAnnoncee;
}
Carte Carte::setCouleurAnnoncee(int& couleur)
{
    couleurAnnoncee = &couleur;
    return *this;
}

int idToOrdre(int n, bool atout)
{
    int m = n % 8;
    if (atout)
    {
        return (n + 4*(m == 2) + (m == 3) + 3*(m == 4) - 3*(m == 5) - 3*(m == 6) - 2*(m == 7));
    }
    else
    {
        return (n + 3*(m == 3) - (m == 4) - (m == 5) - (m == 6));
    }
}
bool comparaisonParId(Carte c1, Carte c2)
{
    int id1 = idToOrdre(c1.getId(), false), id2 = idToOrdre(c2.getId(), false);;
    if (c1.getCouleurAnnoncee() == 5)
    {
        id1 = idToOrdre(c1.getId(), true);
        id2 = idToOrdre(c2.getId(), true);
    }
    else
    {
        if (c1.getCouleur() == c1.getCouleurAnnoncee())
        {
            id1 = idToOrdre(c1.getId(), true);
        }
        if (c2.getCouleur() == c2.getCouleurAnnoncee())
        {
            id2 = idToOrdre(c2.getId(), true);
        }
    }
    return (id1>id2);
}
bool operator==(Carte const& a, Carte const& b)
{
    return (a.getId() == b.getId() && a.getValeur() == b.getValeur() && a.getCouleur() == b.getCouleur() && a.getNom() == b.getNom());
}
bool operator>(Carte const& a, Carte const& b)//Attention ! Pour des cartes, a > b n'implique pas b > a ! L'ordre des cartes est important lorsque des cartes de couleurs différentes
{                                             //sont impliquées. Par exemple, un huit de pique posé en premier bat un as de pique posé en second, et vice-versa si l'ordre est inversé.
    if (a.getCouleur() != b.getCouleur())     //On considère ici que la carte déjà posée est à droite de l'inégalité. Elle aura donc "l'avantage".
    {
        return (a.getCouleur() == a.getCouleurAnnoncee());
    }
    else
    {
        return ((idToOrdre(a.getId(), a.getCouleur() == a.getCouleurAnnoncee())) > (idToOrdre(b.getId(), b.getCouleur() == b.getCouleurAnnoncee())));
    }
}
bool operator<(Carte const& a, Carte const& b)
{
    return !(a<b);
}
ostream& operator<<(ostream& os, const Carte& carte)
{
    os << carte.getNom();
    return os;
}
