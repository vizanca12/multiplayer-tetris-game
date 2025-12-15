# Documentation du Projet Tetris Multijoueur
## Fonctionnalités et Implémentation

### 1. Vue d'ensemble du projet

Ce projet implémente un jeu de Tetris complet en C++ avec trois modes de jeu distincts : solo, multijoueur en réseau et contre intelligence artificielle. Le jeu utilise la bibliothèque graphique SDL2 pour le rendu et la gestion des événements.

### 2. Bibliothèque graphique : SDL2

**SDL2 (Simple DirectMedia Layer 2)** est une bibliothèque multiplateforme qui fournit un accès bas niveau au matériel graphique, audio et de saisie. Dans ce projet, SDL2 est utilisé pour :

- **Rendu graphique** : Création de la fenêtre, rendu des rectangles colorés pour les tétriminos et le plateau de jeu
- **Gestion des textures** : Chargement et affichage du texte avec SDL_ttf
- **Gestion des événements** : Capture des entrées clavier (flèches, rotation, pause)
- **Timing** : Contrôle de la vitesse de chute des pièces avec `SDL_GetTicks()`

Les principaux composants SDL2 utilisés sont :
- `SDL_Window` : Fenêtre principale du jeu (1280x720 pixels)
- `SDL_Renderer` : Moteur de rendu pour dessiner les éléments graphiques
- `SDL_Rect` : Rectangles pour représenter chaque bloc des tétriminos
- `TTF_Font` : Polices de caractères pour le texte (Ubuntu, RobotoMono)

### 3. Mode Solo (Singleplayer)

Le mode solo permet à un joueur de jouer seul contre le temps et la gravité croissante.

**Caractéristiques principales :**
- Un plateau de jeu de 10x20 blocs
- 7 types de tétriminos différents (I, O, T, S, Z, J, L)
- Système de score basé sur les lignes complétées
- Niveau qui augmente avec le score
- Vitesse de chute qui s'accélère avec le niveau
- File d'attente montrant la prochaine pièce
- Stockage d'une pièce (fonction "hold")

**Implémentation :**
- Classe `TetrisMap` : Gère le plateau de jeu, la collision, le placement des pièces
- Classe `Tetrimino` : Représente une pièce avec ses 4 rotations possibles
- La fonction `runTetrisSingleplayer()` contient la boucle de jeu principale qui :
  1. Gère les entrées utilisateur (déplacement, rotation, drop)
  2. Applique la gravité (chute automatique des pièces)
  3. Vérifie les collisions
  4. Détecte et supprime les lignes complètes
  5. Met à jour le score et le niveau
  6. Affiche le plateau, les pièces et les informations

### 4. Mode Multijoueur en Réseau (Multiplayer)

Le mode multijoueur permet à deux joueurs de jouer simultanément via une connexion réseau TCP/IP.

**Architecture réseau :**
- **Serveur (`tetris_server`)** : Coordonne la partie entre les deux joueurs
  - Écoute sur le port 8080
  - Accepte les connexions des clients
  - Synchronise les données entre les joueurs (cartes, lignes "garbage")
  - Gère la salle d'attente jusqu'à ce que tous les joueurs soient connectés
  
- **Client** : Chaque joueur lance le jeu sur sa propre machine
  - Se connecte au serveur avec l'adresse IP/hostname
  - Envoie son état de jeu au serveur
  - Reçoit l'état de jeu de l'adversaire

**Caractéristiques principales :**
- Deux joueurs sur des machines différentes (ou même machine avec deux instances)
- Chaque joueur a sa propre fenêtre de jeu affichant :
  - Son propre plateau à gauche
  - Le plateau de l'adversaire à droite
- Contrôles identiques pour tous : Flèches directionnelles, Z/X (rotation), C (hold), Espace (hard drop)
- Système de lignes "garbage" : quand un joueur complète plusieurs lignes, des lignes incomplètes sont envoyées au plateau adverse via le serveur
- Le jeu continue jusqu'à ce qu'un joueur perde

**Implémentation :**
- Classe `Server` : Gestion bas niveau des sockets TCP/IP
- Classe `TetrisServer` : Logique du serveur de jeu, gestion des salles et diffusion des données
- Classe `Client` : Connexion au serveur et communication bidirectionnelle
- Classe `Player` : Représente un joueur connecté côté serveur
- La fonction `runTetrisMultiplayer(Client *client)` :
  1. Crée une instance de `TetrisMap` pour le joueur local
  2. Lance un thread `handleClient` qui :
     - Envoie périodiquement l'état du plateau local au serveur
     - Envoie les lignes "garbage" générées
     - Reçoit l'état du plateau adverse
     - Reçoit les lignes "garbage" de l'adversaire
  3. Le thread principal gère l'entrée, la physique et le rendu
  4. Communication via codes de protocole : `CODE_PLAYER_MAP`, `CODE_PLAYER_LINES`, `CODE_PLAYER_DEAD`, `CODE_GAME_OVER`

**Protocole de communication :**
1. Client envoie : `CODE_PLAYER_MAP` + données du plateau (200 octets)
2. Client envoie : `CODE_PLAYER_LINES` + nombre de lignes à envoyer (si applicable)
3. Serveur retransmet au joueur adverse
4. En fin de partie : `CODE_PLAYER_DEAD` ou `CODE_GAME_OVER`

### 5. Mode Contre IA (Vs IA)

Le mode contre IA permet au joueur de jouer contre une intelligence artificielle.

**Caractéristiques de l'IA :**
- Analyse stratégique du plateau en temps réel
- Évaluation de toutes les positions et rotations possibles
- Prise de décision basée sur des heuristiques :
  - **Hauteur agrégée** : Minimiser la hauteur totale des colonnes
  - **Lignes complètes** : Maximiser le nombre de lignes pouvant être complétées
  - **Trous** : Minimiser les espaces vides sous les blocs
  - **Rugosité** : Minimiser les différences de hauteur entre colonnes adjacentes
- Exécution automatique des mouvements (rotation, déplacement, drop)

**Implémentation :**
- Classe `TetrisAI` : Contient toute la logique de l'intelligence artificielle
- La méthode `makeMove()` :
  1. Détecte quand une nouvelle pièce apparaît
  2. Analyse toutes les positions possibles (10 colonnes × 4 rotations)
  3. Simule le placement de la pièce pour chaque configuration
  4. Calcule un score basé sur les heuristiques
  5. Sélectionne la meilleure position
  6. Exécute les mouvements nécessaires
- L'IA joue dans le plateau 2 pendant que le joueur contrôle le plateau 1
- Système de "garbage lines" bidirectionnel comme en multijoueur
- Affichage similaire au mode multijoueur : plateau du joueur à gauche, plateau de l'IA à droite

### 6. Architecture du code

**Organisation modulaire :**
- `main.cpp` : Point d'entrée, boucles de jeu principales
- `menu.cpp/hpp` : Interface du menu principal
- `menuRoom.cpp/hpp` : Salle d'attente multijoueur
- `client.cpp/hpp` : Client réseau pour le multijoueur
- `tetrisMap.cpp/hpp` : Logique du plateau de jeu
- `tetrimino.cpp/hpp` : Représentation et gestion des pièces
- `tetrisAI.cpp/hpp` : Intelligence artificielle
- `background.cpp/hpp` : Affichage du fond animé (grille numérique)
- `text.cpp/hpp` : Rendu du texte
- `results.cpp/hpp` : Écran de fin de partie
- **Serveur (dossier `server/`)** :
  - `server.cpp/hpp` : Gestion bas niveau des sockets
  - `tetris_server.cpp/hpp` : Logique du serveur de jeu
  - `player.cpp/hpp` : Représentation d'un joueur
  - `room.cpp/hpp` : Gestion des salles de jeu

**Gestion de la mémoire :**
- Utilisation de `std::vector` pour les tableaux dynamiques
- Utilisation de `std::thread` pour la programmation concurrente (réseau, IA)
- `std::mutex` pour la synchronisation des threads
- Libération appropriée des ressources SDL2 et des sockets à la fin du programme

### 7. Conclusion

Ce projet démontre une implémentation complète d'un jeu Tetris moderne avec plusieurs modes de jeu incluant un mode multijoueur en réseau fonctionnel, une IA compétitive et une interface graphique fluide grâce à SDL2. L'architecture modulaire et l'utilisation de sockets TCP/IP montrent une maîtrise des concepts avancés de programmation en C++.
