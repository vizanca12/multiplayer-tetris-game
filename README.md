# 🎮 Jeu Tetris Multijoueur

Un jeu Tetris complet en C++ avec plusieurs modes de jeu : **Solo**, **Multijoueur en Réseau** et **contre IA**.

![Statut](https://img.shields.io/badge/Statut-Actif-brightgreen)
![Langage](https://img.shields.io/badge/Langage-C%2B%2B-blue)
![Plateforme](https://img.shields.io/badge/Plateforme-Linux%20%7C%20Windows-lightgrey)

## 📋 Caractéristiques Principales

### 🎯 Mode Solo
- Jouez contre le temps avec une difficulté progressive
- 7 types différents de tétriminos (I, O, T, S, Z, J, L)
- Système de score basé sur les lignes complétées
- Augmentation dynamique du niveau et de la vitesse
- File d'attente affichant la pièce suivante
- Fonction "hold" pour stocker une pièce
# 🎮 Jeu Tetris Multijoueur

Un jeu Tetris complet en C++ avec plusieurs modes : **Solo**, **Versus IA**, **Multijoueur Local** et **Multijoueur Réseau**.

![Statut](https://img.shields.io/badge/Statut-Actif-brightgreen) ![Langage](https://img.shields.io/badge/Langage-C%2B%2B-blue) ![Plateforme](https://img.shields.io/badge/Plateforme-Linux%20%7C%20Windows-lightgrey)

## 📋 Présentation

- 7 types de tétriminos : I, O, T, S, Z, J, L
- Système de score par lignes (1/2/3/4 lignes)
- File d'attente des pièces et fonction `hold`
- Graphismes avec SDL2 et rendu texte via SDL2_ttf

## Modes de jeu

### 🎯 Solo
- Jouez seul avec difficulté progressive.

### 🤖 Versus IA
- Affrontez l'IA qui évalue plusieurs positions et choisit le meilleur placement selon des heuristiques (hauteur agrégée, trous, rugosité, lignes complétées).

### 🎮 Multijoueur Local
- Deux joueurs sur la même machine (écran partagé). Les joueurs voient leurs deux plateaux côte à côte et peuvent envoyer des "garbage lines" à l'adversaire.
- Contrôles : voir la section `Contrôles`.

### 🌐 Multijoueur Réseau
- Jouez contre un autre joueur via TCP/IP en vous connectant à un serveur dédié. Supporte plusieurs salles de jeu.

## 🛠️ Prérequis

### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-ttf-dev build-essential
```

### Windows
- MinGW-w64 ou MSVC
- SDL2 et SDL2_ttf (installer et lier selon votre toolchain)

### Compilateur
- GCC/G++ 7.0+ ou équivalent (support C++11+)

## 📦 Installation et compilation

1. Clonez le dépôt
```bash
git clone https://github.com/vizanca12/multiplayer-tetris-game.git
cd multiplayer-tetris-game
```

2. Compilez le client
```bash
make
```

3. (Optionnel) Compilez le serveur
```bash
cd server && make
```

## 🚀 Exécution

### Mode Solo / Versus IA / Multijoueur Local
```bash
./main
```
Choisissez le mode depuis le menu principal.

### Multijoueur Réseau

1. Lancez le serveur (sur la machine hôte)
```bash
cd server
./main
```
Le serveur écoute sur le port **8080** par défaut.

2. Lancez chaque client en ciblant l'adresse du serveur
```bash
./main <server-ip>
# ex: ./main localhost
```

## ⌨️ Contrôles

### Mode Solo / Versus IA / Multijoueur Réseau (par défaut)
| Action | Touche |
|--------|--------|
| Déplacer à gauche | ← |
| Déplacer à droite | → |
| Rotation | Z / X |
| Hold | C |
| Chute rapide | ↓ |
| Chute forcée | Espace |
| Pause | P |
| Menu | ESC |

### Multijoueur Local
**Joueur 1 (Gauche)**
| Action | Touche |
|--------|--------|
| Gauche | A |
| Droite | D |
| Rotation | W |
| Hold | C |
| Chute rapide | S |
| Chute forcée | Espace |

**Joueur 2 (Droite)**
| Action | Touche |
|--------|--------|
| Gauche | ← |
| Droite | → |
| Rotation | ↑ |
| Hold | Shift droit |
| Chute rapide | ↓ |
| Chute forcée | Entrée |

## 📁 Structure du projet
```
multiplayer-tetris-game/
├── src/              # Code source du client
├── include/          # Headers
├── server/           # Code et Makefile du serveur
├── docs/             # Documentation
├── Makefile
└── README.md
```

## 🏗️ Architecture (résumé)

- `TetrisMap` : logique du plateau 10×20
- `Tetrimino` : définition des pièces et rotations
- `TetrisAI` : moteur heuristique
- `Client` / `Server` : communication réseau

## 📚 Documentation
- [Fonctionnalités détaillées](docs/documentation_fonctionalittes.md)
- [Relation avec le cours](docs/relation_avec_cours.md)

## ✍️ Auteurs

- Vinicius Zancheta
- Edilberto

## 📄 Licence

Projet à but éducatif.

---

**Dernière mise à jour** : Janvier 2026

- Utilisation de `std::thread` pour la communication non-bloquante
- `std::mutex` pour la synchronisation sécurisée des données
- Thread séparé pour l'IA et le traitement du réseau

## 📊 Détails Techniques

### Graphiques et Rendu
- **Bibliothèque**: SDL2 + SDL2_ttf
- **Résolution**: 1280x720 pixels
- **Polices**: Ubuntu, RobotoMono
- **Arrière-plan**: Grille numérique animée

### Système de Score
- 1 ligne : 100 points
- 2 lignes : 300 points
- 3 lignes : 500 points
- 4 lignes : 800 points (Tetris)
- Le niveau augmente avec les points totaux
- La vitesse de chute augmente avec le niveau

### IA - Algorithme de Décision
1. Détecte quand une nouvelle pièce apparaît
2. Évalue 40 positions possibles (10 colonnes × 4 rotations)
3. Simule le placement pour chaque configuration
4. Calcule un score en utilisant les heuristiques :
   - **Hauteur Agrégée**: Minimiser la hauteur totale
   - **Trous**: Minimiser les espaces vides
   - **Rugosité**: Minimiser la variation de hauteur
   - **Lignes Complètes**: Maximiser les possibilités
5. Exécute le meilleur mouvement trouvé

## 🐛 Traitement des Erreurs

- Validation des collisions avant chaque mouvement
- Synchronisation des timeouts dans les connexions réseau
- Détection de déconnexion avec reconnexion automatique
- Libération sécurisée des ressources SDL2

## 📝 Compilation Personnalisée

### Client uniquement
```bash
make
```

### Serveur uniquement
```bash
cd server && make
```

### Nettoyer les fichiers compilés
```bash
make clean      # Client
cd server && make clean  # Serveur
```

## 🎓 Objectif Éducatif

Ce projet a été développé dans le cadre de la discipline **IN204** (Programmation Orientée Objet), démontrant :
- Programmation en C++ moderne
- Motifs de conception (Observer, State)
- Programmation concurrente avec threads
- Communication réseau (TCP/IP)
- Programmation graphique avec SDL2
- Intelligence artificielle avec algorithmes heuristiques
- Architecture client-serveur

## 📚 Documentation Additionnelle

Consultez les fichiers de documentation pour plus de détails :
- [Fonctionnalités Détaillées](docs/documentation_fonctionalittes.md)
- [Relation avec le Cours](docs/relation_avec_cours.md)

## ✍️ Auteurs

Développé par :
- **Vinicius Zancheta**
- **Edilberto**

## 📄 Licence

Ce projet est développé à des fins éducationnelles.

---

**Dernière mise à jour** : Janvier 2026

=======

## 🚀 Comment Exécuter

### Mode Solo, contre IA ou Multijoueur Local
```bash
./main
```
Le jeu ouvrira avec le menu principal. Utilisez les options de menu pour choisir entre :
- Solo
- Versus IA
- Multijoueur Local (2 joueurs sur la même machine)
- Multijoueur Réseau

### Mode Multijoueur Local
```bash
./main
```
Sélectionnez "Multijoueur Local" dans le menu. Deux joueurs peuvent jouer ensemble :
- **Joueur 1** (Gauche) : WASD + Espace + C
- **Joueur 2** (Droite) : Flèches + Entrée + Shift Droit

### Mode Multijoueur Réseau

**Terminal 1 - Démarrez le serveur :**
```bash
cd server
./main
```
Le serveur écoute sur le port **8080**

**Terminal 2 - Joueur 1 :**
```bash
./main localhost
```

**Terminal 3 - Joueur 2 (même machine) :**
```bash
./main localhost
```

Pour jouer sur des machines différentes, utilisez l'adresse IP/hostname :
```bash
./main 192.168.1.100  # ou ./main hostname
```

## ⌨️ Contrôles

### Mode Solo / Versus IA
| Action | Touche |
|--------|--------|
| Déplacer à Gauche | ← (Flèche Gauche) |
| Déplacer à Droite | → (Flèche Droite) |
| Rotation | Z ou X |
| Hold (Stocker une Pièce) | C |
| Chute Rapide | ↓ (Flèche Bas) |
| Chute Forcée | Espace |
| Pause | P |
| Menu | ESC |

### Mode Multijoueur Local
**Joueur 1 (Gauche)**
| Action | Touche |
|--------|--------|
| Déplacer à Gauche | A |
| Déplacer à Droite | D |
| Rotation | W |
| Hold | C |
| Chute Rapide | S |
| Chute Forcée | Espace |

**Joueur 2 (Droite)**
| Action | Touche |
|--------|--------|
| Déplacer à Gauche | ← (Flèche Gauche) |
| Déplacer à Droite | → (Flèche Droite) |
| Rotation | ↑ (Flèche Haut) |
| Hold | Shift Droit |
| Chute Rapide | ↓ (Flèche Bas) |
| Chute Forcée | Entrée |

## 📁 Structure du Projet

```
multiplayer-tetris-game/
├── src/              # Code source principal
│   ├── main.cpp
│   ├── tetrisMap.cpp      # Logique du plateau
│   ├── tetrimino.cpp      # Pièces de Tetris
│   ├── tetrisAI.cpp       # IA
│   ├── client.cpp         # Client réseau
│   ├── menu.cpp           # Menu du jeu
│   └── ...
├── include/          # Fichiers d'en-tête
│   ├── tetrisMap.hpp
│   ├── client.hpp
│   └── ...
├── server/           # Code du serveur
│   ├── main.cpp
│   ├── tetris_server.cpp
│   ├── server.cpp
│   ├── player.cpp
│   ├── room.cpp
│   └── Makefile
├── docs/             # Documentation
│   ├── documentation_fonctionalittes.md
│   └── relation_avec_cours.md
├── Makefile          # Build du client
└── README.md
```

## 🏗️ Architecture Technique

### Composants Principaux

**Client :**
- `TetrisMap`: Gère le plateau de 10x20 blocs
- `Tetrimino`: Représente les pièces avec leurs 4 rotations
- `TetrisAI`: Engine d'IA avec analyse heuristique
- `Client`: Communication TCP/IP avec le serveur
- `Menu/MenuRoom`: Interface utilisateur

**Serveur :**
- `Server`: Gestion des sockets TCP/IP sur le port 8080
- `TetrisServer`: Logique de coordination du jeu
- `Player`: Représentation de chaque joueur connecté
- `Room`: Gestion des salles de jeu

### Protocole de Communication

Le jeu utilise TCP/IP pour la synchronisation entre clients et serveur :
- `CODE_PLAYER_MAP`: Synchronise l'état du plateau
- `CODE_PLAYER_LINES`: Envoie les garbage lines
- `CODE_PLAYER_DEAD`: Notifie la défaite
- `CODE_GAME_OVER`: Fin du jeu

### Threading
- Utilisation de `std::thread` pour la communication non-bloquante
- `std::mutex` pour la synchronisation sécurisée des données
- Thread séparé pour l'IA et le traitement du réseau

## 📊 Détails Techniques

### Graphiques et Rendu
- **Bibliothèque**: SDL2 + SDL2_ttf
- **Résolution**: 1280x720 pixels
- **Polices**: Ubuntu, RobotoMono
- **Arrière-plan**: Grille numérique animée

### Système de Score
- 1 ligne : 100 points
- 2 lignes : 300 points
- 3 lignes : 500 points
- 4 lignes : 800 points (Tetris)
- Le niveau augmente avec les points totaux
- La vitesse de chute augmente avec le niveau

### IA - Algorithme de Décision
1. Détecte quand une nouvelle pièce apparaît
2. Évalue 40 positions possibles (10 colonnes × 4 rotations)
3. Simule le placement pour chaque configuration
4. Calcule un score en utilisant les heuristiques :
   - **Hauteur Agrégée**: Minimiser la hauteur totale
   - **Trous**: Minimiser les espaces vides
   - **Rugosité**: Minimiser la variation de hauteur
   - **Lignes Complètes**: Maximiser les possibilités
5. Exécute le meilleur mouvement trouvé

## 🐛 Traitement des Erreurs

- Validation des collisions avant chaque mouvement
- Synchronisation des timeouts dans les connexions réseau
- Détection de déconnexion avec reconnexion automatique
- Libération sécurisée des ressources SDL2

## 📝 Compilation Personnalisée

### Client uniquement
```bash
make
```

### Serveur uniquement
```bash
cd server && make
```

### Nettoyer les fichiers compilés
```bash
make clean      # Client
cd server && make clean  # Serveur
```

## 🎓 Objectif Éducatif

Ce projet a été développé dans le cadre de la discipline **IN204** (Programmation Orientée Objet), démontrant :
- Programmation en C++ moderne
- Motifs de conception (Observer, State)
- Programmation concurrente avec threads
- Communication réseau (TCP/IP)
- Programmation graphique avec SDL2
- Intelligence artificielle avec algorithmes heuristiques
- Architecture client-serveur

## 📚 Documentation Additionnelle

Consultez les fichiers de documentation pour plus de détails :
- [Fonctionnalités Détaillées](docs/documentation_fonctionalittes.md)
- [Relation avec le Cours](docs/relation_avec_cours.md)

## ✍️ Auteurs

Développé par :
- **Vinicius Zancheta**
- **Edilberto**

## 📄 Licence

Ce projet est développé à des fins éducationnelles.

---

**Dernière mise à jour** : Janvier 2026
>>>>>>> Stashed changes
