#pragma once
#include <string>
#include <vector>

using Historique = std::vector<char>;
using FonctionStrategie = char (*)(const Historique& moi, const Historique& adv);

struct Joueur {
    std::string nom;
    FonctionStrategie jouer;
};

bool hasard(double p);

char allD(const Historique& moi, const Historique& adv);
char aleatoire(const Historique& moi, const Historique& adv);
char titForTat(const Historique& moi, const Historique& adv);
char tester(const Historique& moi, const Historique& adv);
char joss(const Historique& moi, const Historique& adv);
char allC(const Historique& moi, const Historique& adv);
char rancunier(const Historique& moi, const Historique& adv);
char majoritaire(const Historique& moi, const Historique& adv);


std::vector<Joueur> populationCours();
std::vector<Joueur> populationElargie();