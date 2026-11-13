#include <algorithm>
#include <iomanip>
#include <iostream>
#include "tournoi.hpp"

/* Gain de celui qui joue `moi` contre un adversaire qui joue `adv` : 3/3, 0/5, 5/0, 2/2 */
/* Gain de celui qui joue `moi` contre un adversaire qui joue `adv` : 3/3, 0/5, 5/0, 2/2 */
int gain(char moi, char adv) {
    if (moi == 'C' && adv == 'C') return 3;
    if (moi == 'C' && adv == 'D') return 0;
    if (moi == 'D' && adv == 'C') return 5;
    return 2;
}

/* Bruit : inverse le coup avec la probabilité `bruit` (aucun tirage si le bruit vaut 0) */
char appliquerBruit(char coup, double bruit) {
    if (bruit > 0.0 && hasard(bruit)) return coup == 'C' ? 'D' : 'C';
    return coup;
}

/* Longueur d'une partie : nbTours si l'horizon est fixe (w = 0), sinon on rajoute un tour tant que le tirage de probabilité w réussit */
int longueurPartie(const Regles& regles) {
    if (regles.w <= 0.0) return regles.nbTours;
    int n = 1;
    while (hasard(regles.w)) n++;
    return n;
}

/* Une partie de nbTours tours entre a et b : les deux choisissent en même temps, puis on compte les gains et on ajoute les coups aux historiques */
std::pair<int, int> jouerPartie(const Joueur& a, const Joueur& b, int nbTours, double bruit) {
    Historique coupsA, coupsB;
    int scoreA = 0, scoreB = 0;
    for (int t = 0; t < nbTours; t++) {
        char ca = appliquerBruit(a.jouer(coupsA, coupsB), bruit);
        char cb = appliquerBruit(b.jouer(coupsB, coupsA), bruit);
        scoreA += gain(ca, cb);
        scoreB += gain(cb, ca);
        coupsA.push_back(ca);
        coupsB.push_back(cb);
    }
    return {scoreA, scoreB};
}
/* Tournoi : chaque joueur affronte chaque autre joueur sur nbParties parties ; les longueurs des parties sont tirées une seule fois pour que toutes les paires jouent les mêmes */
std::vector<long> jouerTournoi(const std::vector<Joueur>& joueurs, const Regles& regles) {
    std::vector<int> longueurs;
    for (int p = 0; p < regles.nbParties; p++) longueurs.push_back(longueurPartie(regles));
    std::vector<long> scores(joueurs.size(), 0);
    for (std::size_t i = 0; i < joueurs.size(); i++) {
        for (std::size_t j = i + 1; j < joueurs.size(); j++) {
            for (int p = 0; p < regles.nbParties; p++) {
                std::pair<int, int> s = jouerPartie(joueurs[i], joueurs[j], longueurs[p], regles.bruit);
                scores[i] += s.first;
                scores[j] += s.second;
            }
        }
    }
    return scores;
}

/* Affiche les joueurs du meilleur score au moins bon */
void afficherClassement(const std::vector<Joueur>& joueurs, const std::vector<long>& scores) {
    std::vector<std::size_t> ordre(joueurs.size());
    for (std::size_t i = 0; i < ordre.size(); i++) ordre[i] = i;
    std::stable_sort(ordre.begin(), ordre.end(), [&](std::size_t x, std::size_t y) { return scores[x] > scores[y]; });
    for (std::size_t k = 0; k < ordre.size(); k++) {
        std::cout << std::setw(2) << k + 1 << ". " << std::left << std::setw(12) << joueurs[ordre[k]].nom
                  << std::right << std::setw(7) << scores[ordre[k]] << '\n';
    }
}