# Relation du Code avec les Concepts du Cours de C++

Ce document établit les liens entre l'implémentation du projet Tetris et les concepts enseignés durant le cours de programmation orientée objet en C++.

## Séance 1 : Introduction aux objets

**Concepts appliqués :**
- **Classes et objets** : Le projet utilise plusieurs classes (`TetrisMap`, `Tetrimino`, `TetrisAI`, `Background`, `Text`)
- **Encapsulation** : Attributs privés avec accesseurs publics
  \`\`\`cpp
  class TetrisMap {
  private:
      std::vector<std::vector<int>> map;
      int score;
  public:
      int getScore() const;
      std::vector<std::vector<int>> getMap() const;
  };
  \`\`\`
- **Méthodes membres** : Chaque classe possède ses propres méthodes (`tetriminoAction()`, `checkCollision()`, etc.)

## Séance 2 : Dérivation & Héritage

**Concepts appliqués :**
- **Composition** : `TetrisMap` contient des instances de `Tetrimino`
- **Structures de données** : Utilisation de la classe `Pos` comme structure de base pour représenter les positions
  \`\`\`cpp
  class Pos {
  public:
      int x, y;
      Pos(int x = 0, int y = 0);
  };
  \`\`\`
- La classe `Tetrimino` représente différents types de pièces avec un comportement commun mais des formes différentes

## Séance 3 : Les Modèles & la Généricité

**Concepts appliqués :**
- **Conteneurs STL génériques** : Utilisation intensive de `std::vector<T>`
  \`\`\`cpp
  std::vector<std::vector<int>> map;  // Plateau 2D
  std::vector<Pos> minos;             // Positions des blocs
  \`\`\`
- **Templates implicites** : Les conteneurs STL utilisent des templates pour stocker différents types (int, Pos, etc.)

## Séance 4 : La STL: les conteneurs et itérateurs

**Concepts appliqués :**
- **std::vector** : Conteneur principal pour stocker le plateau de jeu, les positions des pièces
  \`\`\`cpp
  std::vector<std::vector<int>> map(HEIGHT, std::vector<int>(WIDTH, 0));
  \`\`\`
- **Itération** : Parcours des conteneurs avec boucles for classiques et range-based for
  \`\`\`cpp
  for (const auto& mino : piece.getMinos()) {
      // Traitement de chaque bloc
  }
  \`\`\`
- **Accès aux éléments** : Utilisation de `[]` et `.at()` pour accéder aux éléments des vectors

## Séance 5 : Les opérateurs et les flux

**Concepts appliqués :**
- **Opérateurs arithmétiques** : Calculs de positions, scores, vitesses
  \`\`\`cpp
  int newX = currentPos.x + dx;
  int dropSpeed = std::max(50, 500 - level * 40);
  \`\`\`
- **Opérateurs de comparaison** : Vérification de conditions, comparaison de scores
  \`\`\`cpp
  if (bestScore < score) { bestScore = score; }
  \`\`\`
- **Opérateurs logiques** : Conditions complexes pour les collisions
  \`\`\`cpp
  if (newY < 0 || newY >= HEIGHT || newX < 0 || newX >= WIDTH)
  \`\`\`

## Séance 6 : C++20: contrats, spécialisation de codes

**Concepts appliqués :**
- **Assertions implicites** : Vérifications de préconditions dans les fonctions
  \`\`\`cpp
  if (map.empty()) return;  // Précondition
  \`\`\`
- **Constantes** : Utilisation de `const` pour garantir l'invariabilité
  \`\`\`cpp
  const int WIDTH = 10;
  const int HEIGHT = 20;
  std::vector<std::vector<int>> getMap() const;
  \`\`\`

## Séance 7 : Les exceptions

**Concepts appliqués :**
- **Gestion d'erreurs SDL** : Vérification des retours de fonctions SDL
  \`\`\`cpp
  if (!window) {
      std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
      return -1;
  }
  \`\`\`
- **Gestion sécurisée** : Vérifications avant accès aux ressources pour éviter les crashes

## Séance 8 : L'héritage et le polymorphisme

**Concepts appliqués :**
- **Polymorphisme de comportement** : Les différents types de Tetriminos (I, O, T, S, Z, J, L) partagent une interface commune mais ont des formes différentes
- **Réutilisation de code** : Les trois modes de jeu (solo, multi, IA) utilisent les mêmes classes de base (`TetrisMap`, `Tetrimino`)
- La classe `TetrisAI` étend la fonctionnalité sans modifier les classes existantes (principe ouvert/fermé)

## Séance 9 : Parallélisme & Programmation Asynchrone

**Concepts appliqués :**
- **Synchronisation implicite** : Le jeu multijoueur gère deux plateaux dans la même boucle principale, évitant les problèmes de concurrence
- **Gestion du temps** : Utilisation de `SDL_GetTicks()` pour gérer les timing de manière asynchrone
  \`\`\`cpp
  Uint32 currentTime = SDL_GetTicks();
  if (currentTime - lastFall > dropSpeed) {
      // Chute automatique
  }
  \`\`\`
- **Événements asynchrones** : SDL gère les entrées utilisateur de manière non-bloquante
  \`\`\`cpp
  while (SDL_PollEvent(&event)) {
      // Traitement des événements
  }
  \`\`\`

## Séance 10 : Programmation Asynchrone (suite)

**Concepts appliqués :**
- **Boucle d'événements** : Pattern event loop implémenté dans les fonctions `runTetris()`, `runTetrisMultiplayer()`, `runTetrisVsAI()`
- **État du jeu** : Variables d'état pour gérer le flux asynchrone (pause, game over, etc.)

## Séance 11 : Pointeurs intelligents & Gestion de la mémoire

**Concepts appliqués :**
- **RAII (Resource Acquisition Is Initialization)** : Les ressources SDL sont acquises et libérées de manière structurée
  \`\`\`cpp
  SDL_Window* window = SDL_CreateWindow(...);
  // Utilisation
  SDL_DestroyWindow(window);  // Libération garantie
  \`\`\`
- **Gestion automatique de la mémoire** : Utilisation de `std::vector` qui gère automatiquement l'allocation/désallocation
- **Évitement des fuites mémoire** : Libération systématique des ressources SDL (fenêtre, renderer, textures, polices)

## Séance 12 : Evaluation au moment de la compilation

**Concepts appliqués :**
- **Constantes de compilation** : Définition de constantes pour les dimensions, couleurs
  \`\`\`cpp
  const int WIDTH = 10;
  const int HEIGHT = 20;
  const int BLOCK_SIZE = 30;
  \`\`\`
- **Optimisation du compilateur** : Les constantes permettent au compilateur d'optimiser le code
- **Type safety** : Utilisation de types forts (classes) plutôt que types primitifs pour éviter les erreurs

---

## Synthèse

Ce projet Tetris illustre de manière pratique la majorité des concepts enseignés durant le cours :

1. **Programmation orientée objet** : Classes bien définies avec encapsulation
2. **STL** : Utilisation intensive des conteneurs standard
3. **Gestion de la mémoire** : Utilisation appropriée des ressources et libération
4. **Conception modulaire** : Séparation claire des responsabilités
5. **Gestion du temps et événements** : Programmation événementielle asynchrone
6. **Polymorphisme** : Réutilisation du code pour différents modes de jeu

Le code démontre une compréhension solide des principes de C++ moderne et des bonnes pratiques de développement logiciel.
