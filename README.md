# Tournoi du dilemme du prisonnier itéré

Tournoi au format d'Axelrod : chaque stratégie affronte chaque autre stratégie sur 5 parties de 200 tours, et le meilleur score total gagne.

## Compiler et lancer

```
make
./tournoi base       étape 1 : les 5 stratégies du cours
./tournoi elargi     étape 2 : les 5 du cours + ALL-C + RANCUNIER
./tournoi bruit      étape 3 : population élargie, chaque coup inversé avec une probabilité de 5 %
./tournoi horizon    étape 3 : population élargie, horizon inconnu (w = 0,995)
```

La graine aléatoire est fixée à 42 : relancer une commande donne exactement le même classement.

## Les règles

| Règle | Valeur |
|---|---|
| Gains (moi / l'autre) | C contre C : 3 / 3 · C contre D : 0 / 5 · D contre C : 5 / 0 · D contre D : 2 / 2 |
| Tours par partie | 200 (horizon fixe) |
| Parties par paire | 5 |
| Qui affronte qui | chaque stratégie contre chaque autre, pas contre elle-même |
| Bruit (`bruit`) | 5 % : chaque coup a 5 % de chances d'être inversé (C devient D, D devient C) |
| Horizon inconnu (`horizon`) | après chaque tour, la partie continue avec la probabilité w = 0,995, soit 200 tours en moyenne ; personne ne sait quand elle s'arrête |

## Arborescence

```
tournoi-dilemme/
├── Makefile               compile tout dans l'exécutable « tournoi »
├── README.md              ce fichier
├── journal/
│   ├── journal.md         journal de bord
│   └── captures/          captures d'écran
└── src/
    ├── strategies.hpp/.cpp   les 7 stratégies et les deux populations
    ├── tournoi.hpp/.cpp      gains, bruit, une partie, le tournoi, le classement
    └── main.cpp              lit la commande (base, elargi, bruit, horizon) et lance le tournoi
```

## Comment se déroule un tournoi

```
./tournoi elargi
   │
   ▼
main : choisit la population et les règles (Regles)
   │
   ▼
jouerTournoi
   │  1. tire la longueur des 5 parties (longueurPartie)
   │  2. pour chaque paire de joueurs (i, j), 5 fois :
   │        jouerPartie(i, j)
   │           à chaque tour :
   │             - chaque joueur choisit C ou D avec sa fonction de stratégie
   │             - le bruit peut inverser le coup (appliquerBruit)
   │             - chacun marque ses points (gain)
   │             - les deux coups sont ajoutés aux historiques
   │  3. additionne les points de chaque joueur
   ▼
afficherClassement : trie du meilleur au moins bon et affiche
```

## Ce que fait chaque fichier

### `strategies.hpp` / `strategies.cpp`

**Les types.**
- `Historique` : la liste des coups déjà joués par un joueur, par exemple `C C D C`.
- `FonctionStrategie` : une fonction qui reçoit l'historique du joueur (`moi`), celui de l'adversaire (`adv`) et le générateur aléatoire (`rng`), et qui renvoie `'C'` ou `'D'`.
- `Joueur` : un nom et sa fonction de stratégie, par exemple `{"ALL-D", allD}`.

**`hasard(p, rng)`.** Renvoie vrai avec la probabilité `p`. `rng()` donne un entier entre 0 et 2³² − 1 ; en le divisant par 2³², on obtient un nombre entre 0 et 1, et on le compare à `p`. Ce calcul donne le même résultat sur tous les ordinateurs, ce qui rend les tournois relançables à l'identique.

**Les 7 stratégies.**

| Fonction | Stratégie | Règle |
|---|---|---|
| `allD` | ALL-D | toujours D |
| `aleatoire` | RANDOM | C ou D au hasard, une chance sur deux |
| `titForTat` | TIT-FOR-TAT | C au premier tour, puis recopie le dernier coup de l'adversaire |
| `tester` | TESTER | D au premier tour ; si l'adversaire a déjà joué D, fait comme TIT-FOR-TAT ; sinon répète C, C, D |
| `joss` | JOSS | comme TIT-FOR-TAT, mais un C sur dix est remplacé par D |
| `allC` | ALL-C (nouvelle) | toujours C |
| `rancunier` | RANCUNIER (nouvelle) | C tant que l'adversaire n'a jamais joué D, puis D pour toujours |

Une stratégie ne garde aucune mémoire à elle : tout ce dont elle a besoin se lit dans les historiques. Par exemple, `adv.empty()` veut dire « premier tour », et `adv.back()` donne le dernier coup de l'adversaire.

**Les populations.** `populationCours()` renvoie les 5 stratégies du cours. `populationElargie()` y ajoute ALL-C et RANCUNIER.

### `tournoi.hpp` / `tournoi.cpp`

**`Regles`.** Les paramètres d'un tournoi : `nbTours` (200), `nbParties` (5), `bruit` (0 par défaut) et `w` (0 = horizon fixe).

**`gain(moi, adv)`.** Les points de celui qui joue `moi` face à `adv`, d'après la matrice du cours.

**`appliquerBruit(coup, bruit, rng)`.** Inverse le coup avec la probabilité `bruit`. Si `bruit` vaut 0, la fonction ne tire aucun nombre aléatoire, pour ne pas changer les résultats des étapes 1 et 2.

**`longueurPartie(regles, rng)`.** Le nombre de tours d'une partie. En horizon fixe, c'est 200. En horizon inconnu, on part de 1 tour et on en ajoute un tant qu'un tirage de probabilité w réussit.

**`jouerPartie(a, b, nbTours, bruit, rng)`.** Joue une partie entre `a` et `b` et renvoie leurs deux scores. À chaque tour, les deux joueurs choisissent **avant** qu'on ajoute les coups aux historiques : aucun ne voit le coup de l'autre au même tour. `a` reçoit `(coupsA, coupsB)` et `b` reçoit `(coupsB, coupsA)`, donc chaque stratégie voit toujours ses propres coups en premier.

**`jouerTournoi(joueurs, regles, rng)`.** Tire d'abord la longueur des 5 parties, une seule fois pour tout le tournoi : toutes les paires jouent ainsi des parties de même longueur, et c'est important en horizon inconnu. Puis chaque paire de joueurs différents joue 5 parties, et chaque joueur additionne ses points.

**`afficherClassement(joueurs, scores)`.** Trie les joueurs par score décroissant et affiche rang, nom et score.

### `main.cpp`

Lit le nom du tournoi, crée le générateur aléatoire avec la graine 42, prépare la population et les règles, lance `jouerTournoi` et affiche le classement.
