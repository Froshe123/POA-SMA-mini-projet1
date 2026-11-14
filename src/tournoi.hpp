#pragma once
#include "strategies.hpp"

struct Regles {
    int nbTours = 200;
    int nbParties = 5;
    double bruit = 0.0;
    double w = 0.0;
};

int gain(char moi, char adv);
char appliquerBruit(char coup, double bruit);
int longueurPartie(const Regles& regles);
void jouerPartie(const Joueur& a, const Joueur& b, int nbTours, double bruit, int& scoreA, int& scoreB);
std::vector<long> jouerTournoi(const std::vector<Joueur>& joueurs, const Regles& regles);
void afficherClassement(const std::vector<Joueur>& joueurs, const std::vector<long>& scores);
