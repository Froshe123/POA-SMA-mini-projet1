#include <cstdlib>
#include "strategies.hpp"

/* Renvoie true avec une probabilité p */
bool hasard(double p) {
    int tirage = rand() % 1000;
    return tirage < p * 1000;
}

/* Vrai si l'adversaire a déjà trahi au moins une fois */
bool aDejaTrahi(const Historique& adv) {
    for (char coup : adv) {
        if (coup == 'D') return true;
    }
    return false;
}

/* ALL-D : trahit tout le temps */
char allD(const Historique& moi, const Historique& adv) {
    return 'D';
}

/* RANDOM : joue au hasard */
char aleatoire(const Historique& moi, const Historique& adv) {
    if (hasard(0.5)) return 'C';
    return 'D';
}

/* TIT-FOR-TAT : coopère au début, puis copie le dernier coup de l'adversaire */
char titForTat(const Historique& moi, const Historique& adv) {
    if (adv.empty()) return 'C';
    return adv.back();
}

/* TESTER : trahit au début ; si l'autre a riposté, fait comme TIT-FOR-TAT, sinon joue C, C, D */
char tester(const Historique& moi, const Historique& adv) {
    if (moi.empty()) return 'D';
    if (aDejaTrahi(adv)) return adv.back();
    if (moi.size() % 3 == 0) return 'D';
    return 'C';
}

/* JOSS : comme TIT-FOR-TAT, mais trahit une fois sur dix */
char joss(const Historique& moi, const Historique& adv) {
    char coup = titForTat(moi, adv);
    if (coup == 'C' && hasard(0.1)) coup = 'D';
    return coup;
}

/* ALL-C : coopère tout le temps */
char allC(const Historique& moi, const Historique& adv) {
    return 'C';
}

/* RANCUNIER : coopère jusqu'à ce que l'adversaire trahisse, puis trahit jusqu'à la fin */
char rancunier(const Historique& moi, const Historique& adv) {
    if (aDejaTrahi(adv)) return 'D';
    return 'C';
}

/* MAJORITAIRE : joue le coup que l'adversaire a le plus joué */
char majoritaire(const Historique& moi, const Historique& adv) {
    int nbC = 0;
    int nbD = 0;
    for (char coup : adv) {
        if (coup == 'C') nbC++;
        else nbD++;
    }
    if (nbD > nbC) return 'D';
    return 'C';
}

/* Les 5 stratégies du cours */
std::vector<Joueur> populationCours() {
    std::vector<Joueur> joueurs;
    joueurs.push_back({"ALL-D", allD});
    joueurs.push_back({"RANDOM", aleatoire});
    joueurs.push_back({"TIT-FOR-TAT", titForTat});
    joueurs.push_back({"TESTER", tester});
    joueurs.push_back({"JOSS", joss});
    return joueurs;
}

/* Les 5 du cours + mes 3 stratégies */
std::vector<Joueur> populationElargie() {
    std::vector<Joueur> joueurs = populationCours();
    joueurs.push_back({"ALL-C", allC});
    joueurs.push_back({"RANCUNIER", rancunier});
    joueurs.push_back({"MAJORITAIRE", majoritaire});
    return joueurs;
}
