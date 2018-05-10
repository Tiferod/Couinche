#include <iostream>

#include "Pli.h"

using namespace std;

Pli::Pli() : joueurDebut(0), points(0)
{

}
Pli::Pli(int joueur) : points(0)
{
    if (joueur < 0 || joueur > 3)
    {
        cout<<"Le joueur " << joueur << " n'existe pas." << endl;
        Pli();
    }
    else
    {
        joueurDebut = joueur;
    }
}

Pli Pli::addCarte(Carte carte)
{
    if (cartes.size() > 3)
    {
        cout<<"Un pli ne peut contenir plus de 4 cartes." << endl;
    }
    else
    {
        cartes.push_back(carte);
		points += carte.getPoints();
    }
    return *this;
}
Pli Pli::vider()
{
    cartes.clear();
	points = 0;
    return *this;
}
Pli Pli::setJoueurDebut(int joueur)
{
    joueurDebut = joueur;
    return *this;
}
int Pli::getTaille() const
{
    return cartes.size();
}
vector<Carte> Pli::getCartes() const
{
    return cartes;
}
int Pli::getPoints() const
{
	return points;
}
int Pli::getCouleurDemandee() const
{
    if (cartes.size())
    {
        return cartes[0].getCouleur();
    }
    else
    {
        cout<<"Le pli est vide !" << endl;
        return -1;
    }
}
Carte Pli::getDerniereCarte() const
{
    if (cartes.size())
    {
        return cartes.back();
    }
    else
    {
        cout<<"Ce pli ne contient aucune carte." << endl;
        return Carte(32);
    }
}
Carte Pli::getCarteMaitre() const
{
    if (cartes.size())
    {
        int joueurMaitre = 0;
        for (int i = 0 ; i < cartes.size() ; i++)
        {
            if (cartes[i] > cartes[joueurMaitre])
            {
                joueurMaitre = i;
            }
        }
        return cartes[joueurMaitre];
    }
    else
    {
        cout<<"Ce pli ne contient aucune carte." << endl;
        return Carte(32);
    }
}
int Pli::getJoueurMaitre() const
{
    if (cartes.size())
    {
        int joueurMaitre = 0;
        for (int i = 0 ; i < cartes.size() ; i++)
        {
            if (cartes[i] > cartes[joueurMaitre])
            {
                joueurMaitre = i;
            }
        }
        return (joueurMaitre + joueurDebut) % 4;
    }
    else
    {
        cout<<"Ce pli ne contient aucune carte." << endl;
        return -1;
    }
}

Pli Pli::setDixDeDer()
{
	dixDeDer = true;
	points += 10;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Pli& pli)
{
    for (int i = 0 ; i < pli.getTaille() ; i++)
    {
        if (i) {os << " | ";}
        os << pli.getCartes()[i];
    }
    return os;
}
