#include <iomanip>
#include <iostream>
#include <utility>
#include "tournoi.hpp"

/* Points gagnés pendant un tour (matrice du cours) */
int gain(char moi, char adv) {
    if (moi == 'C' && adv == 'C') return 3;
    if (moi == 'C' && adv == 'D') return 0;
    if (moi == 'D' && adv == 'C') return 5;
    return 2;
}

/* Inverse le coup de temps en temps, seulement s'il y a du bruit */
char appliquerBruit(char coup, double bruit) {
    if (bruit <= 0.0) return coup;
    if (hasard(bruit)) {
        if (coup == 'C') return 'D';
        return 'C';
    }
    return coup;
}

/* Nombre de tours d'une partie : 200, ou au hasard avec l'horizon inconnu */
int longueurPartie(const Regles& regles) {
    if (regles.w <= 0.0) return regles.nbTours;
    int tours = 1;
    while (hasard(regles.w)) tours++;
    return tours;
}

/* Joue une partie entre a et b et calcule leurs scores */
void jouerPartie(const Joueur& a, const Joueur& b, int nbTours, double bruit, int& scoreA, int& scoreB) {
    Historique coupsA;
    Historique coupsB;
    scoreA = 0;
    scoreB = 0;
    for (int tour = 0; tour < nbTours; tour++) {
        char coupA = appliquerBruit(a.jouer(coupsA, coupsB), bruit);
        char coupB = appliquerBruit(b.jouer(coupsB, coupsA), bruit);
        scoreA += gain(coupA, coupB);
        scoreB += gain(coupB, coupA);
        coupsA.push_back(coupA);
        coupsB.push_back(coupB);
    }
}

/* Chaque joueur affronte tous les autres, et on additionne les points */
std::vector<long> jouerTournoi(const std::vector<Joueur>& joueurs, const Regles& regles) {
    std::vector<int> longueurs;
    for (int p = 0; p < regles.nbParties; p++) {
        longueurs.push_back(longueurPartie(regles));
    }

    int n = joueurs.size();
    std::vector<long> scores(n, 0);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            for (int p = 0; p < regles.nbParties; p++) {
                int scoreI, scoreJ;
                jouerPartie(joueurs[i], joueurs[j], longueurs[p], regles.bruit, scoreI, scoreJ);
                scores[i] += scoreI;
                scores[j] += scoreJ;
            }
        }
    }
    return scores;
}

/* Trie les joueurs (tri à bulles) et affiche le classement */
void afficherClassement(const std::vector<Joueur>& joueurs, const std::vector<long>& scores) {
    std::vector<std::string> noms;
    for (const Joueur& joueur : joueurs) noms.push_back(joueur.nom);
    std::vector<long> points = scores;

    int n = noms.size();
    for (int i = 0; i < n - 1; i++) {
        for (int k = 0; k < n - 1 - i; k++) {
            if (points[k] < points[k + 1]) {
                std::swap(points[k], points[k + 1]);
                std::swap(noms[k], noms[k + 1]);
            }
        }
    }

    for (int i = 0; i < n; i++) {
        std::cout << std::setw(2) << i + 1 << ". " << std::left << std::setw(12) << noms[i]
                  << std::right << std::setw(7) << points[i] << '\n';
    }
}
