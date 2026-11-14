#include <cstdlib>
#include <iostream>
#include <string>
#include "tournoi.hpp"

/* Lit le tournoi à lancer et la graine (1 par défaut), règle les paramètres puis affiche le classement */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage : ./tournoi base | elargi | bruit | horizon [graine]\n";
        return 1;
    }
    std::string choix = argv[1];
    int graine = 1;
    if (argc >= 3) graine = atoi(argv[2]);
    srand(graine);

    Regles regles;
    std::vector<Joueur> joueurs;
    if (choix == "base") {
        joueurs = populationCours();
    } else if (choix == "elargi") {
        joueurs = populationElargie();
    } else if (choix == "bruit") {
        joueurs = populationElargie();
        regles.bruit = 0.05;
    } else if (choix == "horizon") {
        joueurs = populationElargie();
        regles.w = 0.995;
    } else {
        std::cout << "Tournoi inconnu : " << choix << "\n";
        std::cout << "Usage : ./tournoi base | elargi | bruit | horizon [graine]\n";
        return 1;
    }

    std::cout << "Tournoi " << choix << " (bruit " << regles.bruit << ", w " << regles.w << ", seed " << graine << ")\n\n";
    std::vector<long> scores = jouerTournoi(joueurs, regles);
    afficherClassement(joueurs, scores);
    return 0;
}
