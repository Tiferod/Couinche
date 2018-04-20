#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>

#include "Table.h"

using namespace std;

Table::Table()
{
    joueurDistribuant = rand() % 4;
    scoreEquipe0 = 0;
    scoreEquipe1 = 0;
    partieSuivante();
}

void Table::partieSuivante()
{
    joueurDistribuant = (joueurDistribuant + 1) % 4;
    joueurActuel = (joueurDistribuant + 1) % 4;
    joueurPreneur = -1;
    couleurAnnoncee = 4;
    montantAnnonce = -1;
    coinche = -1;
    for (int i = 0 ; i < 4 ; i++)
    {
        jeux[i].clear();
    }
    pliEnCours = Pli(joueurActuel);
    plisEquipe0.clear();
    plisEquipe1.clear();
}
void Table::distribution()
{
    vector<Carte> jeu;
    for (int i = 0 ; i < 32 ; i ++)
    {
        jeu.push_back(Carte(i).setCouleurAnnoncee(couleurAnnoncee));
    }
    for (int joueur = 0 ; joueur < 4 ; joueur++)
    {
        for (int i = 0 ; i < 8 ; i ++)
        {
            int n = rand() % jeu.size();
            jeux[joueur].push_back(jeu[n]);
            jeu.erase(jeu.begin() + n);
        }
        sort(jeux[joueur].begin(), jeux[joueur].end(), comparaisonParId);//Besoin d'un constructeur de copie voir plus
        for (int i = 0 ; i < 8 ; i ++)
        {
            cout<<jeux[joueur][i] << "   ";
        }
        cout<< endl;
    }
}
void Table::annonces()//ToDo : Surcoinche
{
    joueurActuel--;
    int dernierJoueurAnnonceur = -1;
    string strDerniereAnnonce;
    int pMontant = 0;
    int nMontant = 0;
    int annonce = 4;
    int cPasse = 0;
    string strAnnonce = "";
    while (!((cPasse > 3 || (cPasse == 3 && dernierJoueurAnnonceur != -1)) || annonce == 6))
    {
        joueurActuel = (joueurActuel + 1) % 4;
        annonce = 4;
        pMontant = nMontant;
        while (!(annonce == 7 || (annonce == 6 && nMontant != 0 && joueurActuel%2 != dernierJoueurAnnonceur%2) || (nMontant > pMontant && (nMontant <= 160 || nMontant == 250) && nMontant % 10 == 0)))
        {
            cout<< endl << "Annonce du Joueur " << joueurActuel << endl;
            cout<<"Couleur (Passe, Carreau, Coeur, Pique, Trefle, SansAtout, ToutAtout, Coinche) : ";
            cin>>strAnnonce;
            if (strAnnonce == "Passe")
            {
                cout<<"Le joueur " << joueurActuel << " a passe." << endl;
                cPasse++;
            }
            else if (strAnnonce == "Coinche")
            {
                if (nMontant == 0)
                {
                    cout<<"Vous ne pouvez pas coincher si aucun joueur n'a fait une annonce." << endl;
                }
                else if (joueurActuel%2 == dernierJoueurAnnonceur%2)
                {
                    cout<<"Vous ne pouvez pas coincher votre partenaire." << endl;
                }
                else
                {
                    cout<<"Le joueur " << joueurActuel << " a coinche l'annonce du joueur " << dernierJoueurAnnonceur << " qui etait : " << nMontant << " " << strDerniereAnnonce  << endl;
                }
            }
            else
            {
                cout<<"Montant : ";
                cin>>nMontant;
                if (nMontant < 80)
                {
                    cout<<"Le nombre de points annonces doit être superieur a 80 points." << endl;
                }
                else if (nMontant > 160 && nMontant != 250)
                {
                    cout<<"Le nombre de points annonces doit être inferieur a 160 points, a l'exception d'un capot (250)." << endl;
                }
                else if (nMontant <= pMontant)
                {
                    cout<<"Le nombre de points annonces doit être superieur a celui de l'annonce precedente." << endl;
                }
                else if (nMontant % 10 != 0)
                {
                    cout<<"Le nombre de points annonces doit être un multiple de 10 (80, 90, 100, etc)." << endl;
                }
                else
                {
                    cPasse = 0;
                    dernierJoueurAnnonceur = joueurActuel;
                    strDerniereAnnonce = strAnnonce;
                    cout<<"Le joueur " << joueurActuel << " a annonce : " << nMontant << " " << strAnnonce << endl;
                }
            }
            annonce = annonceToId(strAnnonce);
        }
    }
    if (cPasse == 3)
    {
        joueurPreneur = dernierJoueurAnnonceur;
        couleurAnnoncee = annonceToId(strDerniereAnnonce);
        montantAnnonce = nMontant;
        coinche = 0;
        cout<< endl << "Tous les joueurs ont passe. Le joueur " << joueurPreneur << " a remporte les annonces avec : " << montantAnnonce << " " << strDerniereAnnonce  << endl;
    }
    else if (cPasse == 4)
    {
        cout<< endl << "Aucun joueur n'a fait d'annonces. La partie est annulee." << endl;
    }
    else if (annonce == 6)
    {
        joueurPreneur = dernierJoueurAnnonceur;
        couleurAnnoncee = annonceToId(strDerniereAnnonce);
        montantAnnonce = nMontant;
        coinche = 1;
        cout<< endl << "Le joueur " << joueurActuel << " a coinche l'annonce du joueur " << joueurPreneur << " qui etait : " << montantAnnonce << " " << strDerniereAnnonce << endl;
    }
    for (int joueur = 0 ; joueur < 4 ; joueur++)
    {
        sort(jeux[joueur].begin(), jeux[joueur].end(), comparaisonParId);
    }
}
void Table::jouer()
{
    joueurActuel = (joueurDistribuant + 1) % 4;
    cout<< endl << "La partie commence : c'est au joueur " << joueurActuel << " de commencer. Entrez la position de la carte dans votre main pour la jouer (commence a 1)." << endl << endl;
    while (jeux[0].size() > 0)
    {
        while (pliEnCours.getTaille() < 4)
        {
            for (int i = 0 ; i < jeux[joueurActuel].size() ; i ++)
            {
                cout<<jeux[joueurActuel][i].getNom() + "   ";
            }
            cout<< endl << "Joueur " << joueurActuel << " : ";
            int posCarteJouee;
            do
            {
                cin>>posCarteJouee;
            }while(!jouerCarte(joueurActuel, jeux[joueurActuel][posCarteJouee-1]));
            joueurActuel = (joueurActuel + 1) % 4;
        }
        ramasserPli();
    }

}
bool Table::testCarte(int joueur, Carte carte, bool messageDErreur) const
{
    if (pliEnCours.getTaille() == 0)
    {
        return true;
    }
    else
    {
        if (pliEnCours.getCouleurDemandee() == carte.getCouleur())
        {
            if (couleurAnnoncee == 5 || pliEnCours.getCouleurDemandee() == couleurAnnoncee)
            {
                if (carte > pliEnCours.getCarteMaitre())
                {
                    return true;
                }
                else
                {
                    if (possedeAtoutSuperieur(joueur, pliEnCours.getCarteMaitre()))
                    {
                        if (messageDErreur) {cout<<"On doit toujours monter a l'atout." << endl;}
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
            }
            else
            {
                return true;
            }
        }
        else
        {
            if (possedeCouleur(joueur, pliEnCours.getCouleurDemandee()))
            {
                if (messageDErreur) {cout<<"Vous devez servir de la couleur demandee." << endl;}
                return false;
            }
            else
            {
                if (pliEnCours.getJoueurMaitre()%2 == joueur % 2)
                {
                    return true;
                }
                else
                {
                    if (carte.getCouleur() == couleurAnnoncee)
                    {
                        if (carte > pliEnCours.getCarteMaitre())
                        {
                            return true;
                        }
                        else
                        {
                            if (possedeAtoutSuperieur(joueur, pliEnCours.getCarteMaitre()))
                            {
                                if (messageDErreur) {cout<<"On doit toujours monter a l'atout." << endl;}
                                return false;
                            }
                            else
                            {
                                return true;
                            }
                        }
                    }
                    else
                    {
                        if (possedeCouleur(joueur, couleurAnnoncee))
                        {
                            if (messageDErreur) {cout<<"Si vous n'avez pas de la couleur demandee, vous devez couper si vous avez de l'atout." << endl;}
                            return false;
                        }
                        else
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
}
bool Table::jouerCarte(int joueur, Carte carte)
{
    if (joueur < 0 || joueur > 3)
    {
        cout<<"Le joueur " << joueur << " n'existe pas." << endl;
        return false;
    }
    else
    {
        if (joueur == joueurActuel)
        {
            vector<Carte>::iterator pos = find(jeux[joueur].begin(), jeux[joueur].end(), carte);
            if (pos != jeux[joueur].end())
            {
                if (testCarte(joueur, carte))
                {
                    cout<<"Le joueur " << joueur << " a joue : " << pos->getNom() << endl << endl;
                    jeux[joueur].erase(pos);
                    pliEnCours.addCarte(carte);
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                cout<<"Le joueur " << joueur << " ne possede pas la carte " << carte.getNom() << endl;
                return false;
            }
        }
        else
        {
            cout<<"C'est au joueur " << joueurActuel << " de jouer, et non au joueur " << joueur << endl;
            return false;
        }
    }
}

void Table::ramasserPli()
{
    cout<<pliEnCours << endl;
    cout<<"Le joueur " << pliEnCours.getJoueurMaitre() << " remporte le pli." << endl << endl;
    if (pliEnCours.getJoueurMaitre()%2)
    {
        plisEquipe1.push_back(pliEnCours);
    }
    else
    {
        plisEquipe0.push_back(pliEnCours);
    }
    joueurActuel = pliEnCours.getJoueurMaitre();
    pliEnCours.vider();
    pliEnCours.setJoueurDebut(joueurActuel);
}
Pli Table::dernierPli() const
{
	return Pli();
}
void Table::belote(int joueur)
{

}
void Table::rebelote(int joueur)
{

}
bool Table::possedeAtoutSuperieur(int joueur, Carte atout) const
{
    for (int i = 0 ; i < jeux[joueur].size() ; i++)
    {
        if (jeux[joueur][i] > atout)
        {
            return true;
        }
    }
    return false;
}
bool Table::possedeCouleur(int joueur, int couleur) const
{
    for (int i = 0 ; i < jeux[joueur].size() ; i++)
    {
        if (jeux[joueur][i].getCouleur() == couleur)
        {
            return true;
        }
    }
    return false;
}

int annonceToId(string str)
{
    if (str == "Carreau")
    {
        return 0;
    }
    else if (str == "Coeur")
    {
        return 1;
    }
    else if (str == "Pique")
    {
        return 2;
    }
    else if (str == "Trefle")
    {
        return 3;
    }
    else if (str == "SansAtout")
    {
        return 4;
    }
    else if (str == "ToutAtout")
    {
        return 5;
    }
    else if (str == "Coinche")
    {
        return 6;
    }
    else if (str == "Passe")
    {
        return 7;
    }
    return -1;
}
