#include <algorithm>
#include <cstdlib>
#include "strategies.hpp"

/* Renvoie vrai avec la probabilité p : tire un nombre entre 0 et 999, vrai s'il est plus petit que p × 1000 */
bool hasard(double p) {
    int tirage = rand() % 1000;
    return tirage < p * 1000;
}

/* ALL-D : joue toujours D */
char allD(const Historique& moi, const Historique& adv) {
    return 'D';
}

/* RANDOM : joue C ou D au hasard, une chance sur deux */
char aleatoire(const Historique& moi, const Historique& adv) {
    return hasard(0.5) ? 'C' : 'D';
}

/* TIT-FOR-TAT : C au premier tour, puis rejoue le dernier coup de l'adversaire */
char titForTat(const Historique& moi, const Historique& adv) {
    if (adv.empty()) return 'C';
    return adv.back();
}

/* TESTER : D au premier tour ; si l'adversaire a déjà joué D, joue TIT-FOR-TAT ; sinon répète C, C, D */
char tester(const Historique& moi, const Historique& adv) {
    if (moi.empty()) return 'D';
    if (std::find(adv.begin(), adv.end(), 'D') != adv.end()) return adv.back();
    return moi.size() % 3 == 0 ? 'D' : 'C';
}

/* JOSS : comme TIT-FOR-TAT, mais remplace C par D une fois sur dix */
char joss(const Historique& moi, const Historique& adv) {
    char coup = adv.empty() ? 'C' : adv.back();
    if (coup == 'C' && hasard(0.1)) return 'D';
    return coup;
}

/* ALL-C : joue toujours C */
char allC(const Historique& moi, const Historique& adv) {
    return 'C';
}

/* RANCUNIER : joue C tant que l'adversaire n'a jamais joué D, puis D pour toujours */
char rancunier(const Historique& moi, const Historique& adv) {
    if (std::find(adv.begin(), adv.end(), 'D') != adv.end()) return 'D';
    return 'C';
}

/* MAJORITAIRE : joue le coup que l'adversaire a joué le plus souvent depuis le début (C au premier tour et en cas d'égalité) */
char majoritaire(const Historique& moi, const Historique& adv) {
    int nbD = std::count(adv.begin(), adv.end(), 'D');
    int nbC = adv.size() - nbD;
    return nbD > nbC ? 'D' : 'C';
}

/* Les cinq stratégies du cours (étape 1) */
std::vector<Joueur> populationCours() {
    return {{"ALL-D", allD}, {"RANDOM", aleatoire}, {"TIT-FOR-TAT", titForTat}, {"TESTER", tester}, {"JOSS", joss}};
}

/* Les cinq stratégies du cours plus ALL-C et RANCUNIER (étapes 2 et 3) */
std::vector<Joueur> populationElargie() {
    std::vector<Joueur> joueurs = populationCours();
    joueurs.push_back({"ALL-C", allC});
    joueurs.push_back({"RANCUNIER", rancunier});
    joueurs.push_back({"MAJORITAIRE", majoritaire});
    return joueurs;
}