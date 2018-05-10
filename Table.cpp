#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "Table.h"

using namespace std;

Table::Table()
{
    joueurDistribuant = rand() % 4;
    scoreEquipe[0] = 0;
    scoreEquipe[1] = 0;
	litige = 0;
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
	belote = -1;
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
void Table::annonces()
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
            cout << endl << "Annonce du Joueur " << joueurActuel << endl;
            cout <<"Couleur (Passe, Carreau, Coeur, Pique, Trefle, SansAtout, ToutAtout, Coinche) : ";
            cin>>strAnnonce;
			annonce = annonceToId(strAnnonce);
			if (annonce == -1)
			{
				cout << strAnnonce << " n'est pas une annonce valide." << endl;
			}
            else if (annonce == 7)
            {
				nMontant = pMontant;
                cout << endl << "Le joueur " << joueurActuel << " a passe." << endl;
                cPasse++;
            }
            else if (annonce == 6)
            {
				nMontant = pMontant;
                if (nMontant == 0)
                {
                    cout << "Vous ne pouvez pas coincher si aucun joueur n'a fait une annonce." << endl;
                }
                else if (joueurActuel%2 == dernierJoueurAnnonceur%2)
                {
                    cout << "Vous ne pouvez pas coincher votre partenaire." << endl;
                }
                else
                {
                    cout<< endl << "Le joueur " << joueurActuel << " a coinche l'annonce du joueur " << dernierJoueurAnnonceur << " qui etait : " << nMontant << " " << strDerniereAnnonce  << endl;
                }
            }
            else
            {
                cout << "Montant : ";
				while (!(cin >> nMontant)) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Ce montant est invalide. Reessayez." << endl << "Montant : ";
				}
                if (nMontant < 80)
                {
                    cout << "Le nombre de points annonces doit etre superieur a 80 points." << endl;
                }
                else if (nMontant > 160 && nMontant != 250)
                {
                    cout << "Le nombre de points annonces doit etre inferieur a 160 points, a l'exception d'un capot (250)." << endl;
                }
                else if (nMontant <= pMontant)
                {
                    cout << "Le nombre de points annonces doit etre superieur a celui de l'annonce precedente." << endl;
                }
                else if (nMontant % 10 != 0)
                {
                    cout << "Le nombre de points annonces doit etre un multiple de 10 (80, 90, 100, etc)." << endl;
                }
                else
                {
                    cPasse = 0;
                    dernierJoueurAnnonceur = joueurActuel;
                    strDerniereAnnonce = strAnnonce;
                    cout << endl << "Le joueur " << joueurActuel << " a annonce : " << nMontant << " " << strAnnonce << endl;
                }
            }
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
		joueurActuel = (joueurActuel + 1) % 4;
		while (strAnnonce != "oui" && cPasse < 2)
		{
			strAnnonce = "";
			while (strAnnonce != "oui" && strAnnonce != "non")
			{
				cout << endl << "Le joueur " << joueurActuel << " veut-il surcoincher ?";
				cin >> strAnnonce;
				std::transform(strAnnonce.begin(), strAnnonce.end(), strAnnonce.begin(), ::tolower);
			}
			if (strAnnonce == "oui")
			{
				cout << "Le joueur " << joueurActuel << " a surcoinche." << endl;
				coinche = 2;
			}
			else
			{
				cout << "Le joueur " << joueurActuel << " n'a pas surcoinche." << endl;
				cPasse++;
				joueurActuel = (joueurActuel + 2) % 4;
			}
		}
    }
	for (int joueur = 0; joueur < 4; joueur++)
	{
		for (int i = 0; i < 8; i++)
		{
			jeux[joueur][i].setCouleurAnnoncee(couleurAnnoncee);
		}
		sort(jeux[joueur].begin(), jeux[joueur].end(), comparaisonParId);
		if ((find(jeux[joueur].begin(), jeux[joueur].end(), Carte(5, couleurAnnoncee)) != jeux[joueur].end()) && (find(jeux[joueur].begin(), jeux[joueur].end(), Carte(6, couleurAnnoncee)) != jeux[joueur].end()))
		{
			belote = joueur;
		}
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
            } while(posCarteJouee < 1 || posCarteJouee > jeux[joueurActuel].size() || !jouerCarte(joueurActuel, jeux[joueurActuel][posCarteJouee-1]));
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
                    cout <<"Le joueur " << joueur << " a joue : " << pos->getNom() << ".";
					jeux[joueur].erase(pos);
					if (joueur == belote && carte.isAtout())
					{
						if (carte.getValeur() == 5)
						{
							if (find(jeux[joueur].begin(), jeux[joueur].end(), Carte(6, carte.getCouleur())) == jeux[joueur].end())
							{
								cout << " Rebelote.";
							}
							else
							{
								cout << " Belote.";
							}
						}
						else if (carte.getValeur() == 6)
						{
							if (find(jeux[joueur].begin(), jeux[joueur].end(), Carte(5, carte.getCouleur())) == jeux[joueur].end())
							{
								cout << " Rebelote.";
							}
							else
							{
								cout << " Belote.";
							}
						}
					}
					cout << endl << endl;
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
    cout << pliEnCours << endl;
    cout << "Le joueur " << pliEnCours.getJoueurMaitre() << " remporte le pli." << endl << endl;
	if (!jeux[0].size())
	{
		pliEnCours.setDixDeDer();
	}
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

int Table::finPartie()
{
	cout << "La partie est terminee." << endl;
	cout << "L'equipe " << joueurPreneur % 2 << " avait annonce : " << montantAnnonce << " " << idToAnnonce(couleurAnnoncee) << "." << endl;
	int score[2] = {0, 0};
	if (coinche == 1)
	{
		cout << "L'annonce avait ete coinchee." << endl;
	}
	else if (coinche == 2)
	{
		cout << "L'annonce avait ete coinchee et surcoinchee." << endl;
	}
	if (belote > -1)
	{
		cout << "L'equipe " << belote % 2 << " a la belote." << endl;
	}
	if (montantAnnonce == 250)
	{
		if (joueurPreneur % 2)
		{
			if (plisEquipe0.size())
			{
				cout << "L'equipe " << joueurPreneur % 2 << " n'a realise que " << plisEquipe1.size() << " des 8 plis." << endl;
				cout << "Elle n'a donc pas rempli son contrat." << endl;
				score[joueurPreneur % 2] = 0;
				score[(joueurPreneur + 1) % 2] = pointsCoinche(false) + 20 * (belote > -1) + litige;
			}
			else
			{
				cout << "L'equipe " << joueurPreneur % 2 << " a realise tous les plis." << endl;
				cout << "Elle a donc rempli son contrat." << endl;
				score[joueurPreneur % 2] = pointsCoinche(true) + 20 * (belote > -1) + litige;
				score[(joueurPreneur + 1) % 2] = 0;
			}
		}
		else
		{
			if (plisEquipe1.size())
			{
				cout << "L'equipe " << joueurPreneur % 2 << " n'a realise que " << plisEquipe0.size() << " des 8 plis." << endl;
				cout << "Elle n'a donc pas rempli son contrat." << endl;
				score[joueurPreneur % 2] = 0;
				score[(joueurPreneur + 1) % 2] = pointsCoinche(false) + 20 * (belote > -1) + litige;
			}
			else
			{
				cout << "L'equipe " << joueurPreneur % 2 << " a realise tous les plis." << endl;
				cout << "Elle a donc rempli son contrat." << endl;
				score[joueurPreneur % 2] = pointsCoinche(true) + 20 * (belote > -1) + litige;
				score[(joueurPreneur + 1) % 2] = 0;
			}
		}
		litige = 0;
	}
	else
	{
		cout << "Score 0 : " << score[0] << " | ";
		for (int i = 0; i < plisEquipe0.size(); i++)
		{
			score[0] += plisEquipe0[i].getPoints();
			cout << "Score 0 : " << score[0] << " | ";
		}
		cout << endl;
		cout << "Score 1 : " << score[1] << " | ";
		for (int i = 0; i < plisEquipe1.size(); i++)
		{
			score[1] += plisEquipe1[i].getPoints();
			cout << "Score 1 : " << score[1] << " | ";
		}
		cout << endl;
		score[0] = static_cast<int>(score[0] * (1 + (couleurAnnoncee == 5) * 96.0 / 162.0) + 0.5) + 20 * (belote % 2 == 0);
		score[1] = static_cast<int>(score[1] * (1 + (couleurAnnoncee == 5) * 96.0 / 162.0) + 0.5) + 20 * (belote % 2 == 1);
		cout << "Les scores sont les suivants : Equipe 0 : " << score[0] << " | Equipe 1 : " << score[1] << endl;
		if (score[joueurPreneur % 2] >= montantAnnonce && score[joueurPreneur % 2] > 81)
		{
			cout << "L'equipe " << joueurPreneur % 2 << " a donc rempli son contrat." << endl;
			score[joueurPreneur % 2] = pointsCoinche(true) + litige;
			score[(joueurPreneur + 1) % 2] = static_cast<int>(score[(joueurPreneur + 1) % 2] / 10.0 + 0.5) * 10 * (coinche == 0);
			litige = 0;
		}
		else
		{
			if (montantAnnonce == 80 && score[joueurPreneur % 2] == 81)
			{
				cout << "Litige. L'equipe " << joueurPreneur % 2 << " ne marque pas de points. L'equipe qui remportera la partie suivante les marquera." << endl;
				score[joueurPreneur % 2] = 0;
				score[(joueurPreneur + 1) % 2] = 80 * (coinche == 0) + 20 * ((belote % 2) == (joueurPreneur + 1 % 2));
				litige += pointsCoinche(true) + 20 * ((belote % 2) == (joueurPreneur % 2));
			}
			else
			{
				cout << "L'equipe " << joueurPreneur % 2 << " n'a donc pas rempli son contrat." << endl;
				score[joueurPreneur % 2] = 0;
				score[(joueurPreneur + 1) % 2] = pointsCoinche(false) + 20 * (belote > -1) + litige;
				litige = 0;
			}
		}
	}
	scoreEquipe[0] += score[0];
	scoreEquipe[1] += score[1];
	cout << "L'equipe 0 marque " << score[0] << " | L'equipe 1 marque " << score[1] << endl;
	return max(score[0], score[1]);
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

int Table::pointsCoinche(bool attaque)
{
	if (attaque)
	{
		return (montantAnnonce + 160 * (coinche > 0))*(1 + (coinche == 2));
	}
	else
	{
		return (160 + montantAnnonce * (coinche > 0))*(1 + (coinche == 2));
	}
}

int annonceToId(string str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    if (str == "carreau")
    {
        return 0;
    }
    else if (str == "coeur")
    {
        return 1;
    }
    else if (str == "pique")
    {
        return 2;
    }
    else if (str == "trefle")
    {
        return 3;
    }
    else if (str == "sansatout")
    {
        return 4;
    }
    else if (str == "toutatout")
    {
        return 5;
    }
    else if (str == "coinche")
    {
        return 6;
    }
	else if (str == "passe")
	{
		return 7;
	}
    return -1;
}

string idToAnnonce(int id)
{
	switch (id)
	{
	case 0:
		return "Carreau";
	case 1:
		return "Coeur";
	case 2:
		return "Pique";
	case 3:
		return "Trefle";
	case 4:
		return "Sans Atout";
	case 5:
		return "Tout Atout";
	default:
		return "Id non valide.";
	}
}
