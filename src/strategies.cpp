#include <cstdlib>
#include "strategies.hpp"

/* Renvoie true avec une probabilité p (on tire un nombre entre 0 et 999) */
bool hasard(double p) {
    int tirage = rand() % 1000;
    return tirage < p * 1000;
}

/* Regarde si l'adversaire a joué D au moins une fois depuis le début */
bool aDejaTrahi(const Historique& adv) {
    for (char coup : adv) {
        if (coup == 'D') return true;
    }
    return false;
}

/* ALL-D trahit à chaque tour */
char allD(const Historique& moi, const Historique& adv) {
    return 'D';
}

/* RANDOM joue C ou D à pile ou face */
char aleatoire(const Historique& moi, const Historique& adv) {
    if (hasard(0.5)) return 'C';
    return 'D';
}

/* TIT-FOR-TAT commence par coopérer puis recopie le dernier coup de l'adversaire */
char titForTat(const Historique& moi, const Historique& adv) {
    if (adv.empty()) return 'C';
    return adv.back();
}

/* TESTER trahit au premier tour pour tester l'adversaire : s'il riposte, il passe en TIT-FOR-TAT, sinon il enchaîne C, C, D */
char tester(const Historique& moi, const Historique& adv) {
    if (moi.empty()) return 'D';
    if (aDejaTrahi(adv)) return adv.back();
    if (moi.size() % 3 == 0) return 'D';
    return 'C';
}

/* JOSS joue comme TIT-FOR-TAT mais trahit une fois sur dix quand il devrait coopérer */
char joss(const Historique& moi, const Historique& adv) {
    char coup = titForTat(moi, adv);
    if (coup == 'C' && hasard(0.1)) coup = 'D';
    return coup;
}

/* ALL-C coopère à chaque tour */
char allC(const Historique& moi, const Historique& adv) {
    return 'C';
}

/* RANCUNIER coopère jusqu'à la première trahison de l'adversaire, puis trahit jusqu'à la fin */
char rancunier(const Historique& moi, const Historique& adv) {
    if (aDejaTrahi(adv)) return 'D';
    return 'C';
}

/* MAJORITAIRE compte les C et les D de l'adversaire et joue le coup le plus fréquent (C en cas d'égalité) */
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

/* Population de l'étape 1 : les cinq stratégies vues en cours */
std::vector<Joueur> populationCours() {
    std::vector<Joueur> joueurs;
    joueurs.push_back({"ALL-D", allD});
    joueurs.push_back({"RANDOM", aleatoire});
    joueurs.push_back({"TIT-FOR-TAT", titForTat});
    joueurs.push_back({"TESTER", tester});
    joueurs.push_back({"JOSS", joss});
    return joueurs;
}

/* Population des étapes 2 et 3 : celles du cours plus ALL-C, RANCUNIER et MAJORITAIRE */
std::vector<Joueur> populationElargie() {
    std::vector<Joueur> joueurs = populationCours();
    joueurs.push_back({"ALL-C", allC});
    joueurs.push_back({"RANCUNIER", rancunier});
    joueurs.push_back({"MAJORITAIRE", majoritaire});
    return joueurs;
}
