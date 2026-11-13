#include <cstdlib>
#include <iostream>
#include <string>
#include "tournoi.hpp"

/* Lance le tournoi demandé (base, elargi, bruit ou horizon) avec la graine donnée (1 par défaut), puis affiche le classement */
int main(int argc, char* argv[]) {
    std::string choix = argc >= 2 ? argv[1] : "";
    int graine = argc >= 3 ? atoi(argv[2]) : 1;
    srand(graine);
    Regles regles;
    std::vector<Joueur> joueurs = populationElargie();

    if (choix == "base") joueurs = populationCours();
    else if (choix == "bruit") regles.bruit = 0.05;
    else if (choix == "horizon") regles.w = 0.995;
    else if (choix != "elargi") {
        std::cout << "Usage : ./tournoi base | elargi | bruit | horizon [graine]\n";
        return 1;
    }

    std::cout << "Tournoi " << choix << " (bruit " << regles.bruit << ", w " << regles.w << ", seed " << graine << ")\n\n";
    afficherClassement(joueurs, jouerTournoi(joueurs, regles));
    return 0;
}