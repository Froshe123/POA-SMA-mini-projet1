# Tournoi du dilemme du prisonnier itéré

Projet 2 (partie 1) de HAI716I. Tournoi au format d'Axelrod : chaque stratégie affronte chaque autre stratégie sur 5 parties de 200 tours, et celle qui a le plus de points au total gagne.

## Compiler et lancer

```
make
./tournoi base        étape 1 : les 5 stratégies du cours
./tournoi elargi      étape 2 : les 5 du cours + ALL-C, RANCUNIER, MAJORITAIRE
./tournoi bruit       étape 3 : population élargie, 5 % des coups inversés par erreur
./tournoi horizon     étape 3 : population élargie, horizon inconnu (w = 0,995)
```

La graine vaut 1 par défaut. On peut en donner une autre à la fin : `./tournoi base 7`. Avec la même graine, on obtient toujours exactement le même classement sur un même ordinateur (`rand()` peut donner des nombres différents sur un autre système).

## Les règles

- **Gains** (moi / l'autre) : C contre C = 3 / 3, C contre D = 0 / 5, D contre C = 5 / 0, D contre D = 2 / 2.
- **200 tours** par partie et **5 parties** par paire de stratégies.
- Chaque stratégie joue contre chaque autre, **pas contre elle-même**.
- **Bruit** : chaque coup a 5 % de chances d'être inversé (C devient D, D devient C).
- **Horizon inconnu** : après chaque tour, la partie continue avec une probabilité de 0,995, donc elle dure 200 tours en moyenne mais personne ne sait quand elle s'arrête. Les longueurs des 5 parties sont tirées au début du tournoi et sont les mêmes pour toutes les paires.

## Les fichiers

```
src/
├── strategies.hpp / .cpp   le hasard, les 8 stratégies, les deux populations
├── tournoi.hpp / .cpp      les gains, le bruit, une partie, le tournoi, le classement
└── main.cpp                lit la commande et lance le bon tournoi
journal/
├── journal.md              journal de bord
└── captures/               captures d'écran
```

## Comment ça marche

```
main : choisit la population et les règles selon la commande
  └─ jouerTournoi
       ├─ tire la longueur des 5 parties (longueurPartie)
       ├─ pour chaque paire de joueurs, 5 fois jouerPartie :
       │     à chaque tour, chaque joueur choisit C ou D,
       │     le bruit peut inverser le coup, on compte les points (gain)
       └─ additionne les points de chaque joueur
  └─ afficherClassement : trie (tri à bulles) et affiche
```

### `strategies.cpp`

- `hasard(p)` : renvoie vrai avec une probabilité `p`. On tire un nombre entre 0 et 999 avec `rand() % 1000` et on regarde s'il est plus petit que `p × 1000`. Par exemple `hasard(0.1)` est vrai une fois sur dix. Le hasard est initialisé une seule fois dans `main` avec `srand(graine)`.
- `aDejaTrahi(adv)` : regarde si l'adversaire a joué D au moins une fois.
- Une fonction par stratégie. Elle reçoit les coups déjà joués par le joueur (`moi`) et par l'adversaire (`adv`), et renvoie `'C'` ou `'D'`. `adv.empty()` veut dire que c'est le premier tour, `adv.back()` donne le dernier coup de l'adversaire.

| Stratégie | Règle |
|---|---|
| ALL-D | toujours D |
| RANDOM | C ou D à pile ou face |
| TIT-FOR-TAT | C au premier tour, puis recopie le dernier coup de l'adversaire |
| TESTER | D au premier tour ; si l'adversaire a déjà joué D, fait comme TIT-FOR-TAT ; sinon répète C, C, D |
| JOSS | comme TIT-FOR-TAT, mais trahit une fois sur dix quand il devrait coopérer |
| ALL-C (nouvelle) | toujours C |
| RANCUNIER (nouvelle) | C jusqu'à la première trahison de l'adversaire, puis D jusqu'à la fin |
| MAJORITAIRE (nouvelle) | joue le coup que l'adversaire a joué le plus souvent depuis le début (C au début et en cas d'égalité) |

- `populationCours()` : les 5 stratégies du cours. `populationElargie()` : les mêmes plus les 3 nouvelles.

### `tournoi.cpp`

- `gain(moi, adv)` : les points de la matrice du cours.
- `appliquerBruit(coup, bruit)` : inverse le coup avec la probabilité `bruit`. Quand il n'y a pas de bruit, on ne tire rien, pour ne pas changer le hasard des autres tournois.
- `longueurPartie(regles)` : 200 tours avec l'horizon fixe ; sinon on part de 1 tour et on en rajoute tant que le tirage de probabilité w réussit.
- `jouerPartie(a, b, nbTours, bruit, scoreA, scoreB)` : joue une partie et met les deux scores dans `scoreA` et `scoreB`. À chaque tour, les deux joueurs choisissent avant qu'on ajoute les coups aux historiques : personne ne voit le coup de l'autre au même tour.
- `jouerTournoi(joueurs, regles)` : tire les longueurs des parties, fait jouer chaque paire (la boucle `j` commence à `i + 1`, donc chaque paire joue une fois et personne ne joue contre soi-même) et additionne les points.
- `afficherClassement(joueurs, scores)` : trie les joueurs du meilleur au moins bon avec un tri à bulles et affiche rang, nom, score.

### `main.cpp`

Lit le nom du tournoi et la graine, appelle `srand(graine)`, choisit la population et les règles, lance le tournoi et affiche le classement.
