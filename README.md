# 🎮 Tetris Multijoueur

Un jeu Tetris complet en C++ avec support pour **Solo**, **Versus IA**, **Multijoueur Local** et **Multijoueur Réseau**.

![Statut](https://img.shields.io/badge/Statut-Actif-brightgreen) ![Langage](https://img.shields.io/badge/Langage-C%2B%2B-blue) ![Plateforme](https://img.shields.io/badge/Plateforme-Linux%20%7C%20Windows-lightgrey)

## 🎯 Fonctionnalités

- **7 tétriminos** : I, O, T, S, Z, J, L
- **Système de score** avec accélération progressive
- **Hold** et **Next queue** pour gérer les pièces
- **Mode Multijoueur Local** : 2 joueurs sur une même machine
- **Mode Réseau** : Jouez contre un adversaire via TCP/IP
- **IA compétitive** avec analyse heuristique
- **Graphismes SDL2** fluides et réactifs

## 📋 Prérequis

### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-ttf-dev build-essential
```

### Windows
- MinGW-w64 ou MSVC
- SDL2 et SDL2_ttf (installer selon votre compilateur)
- GCC/G++ 7.0+ avec support C++11

### macOS
```bash
brew install sdl2 sdl2_ttf
```

## 🚀 Installation et compilation

1. **Clonez le dépôt**
```bash
git clone https://github.com/vizanca12/multiplayer-tetris-game.git
cd multiplayer-tetris-game
```

2. **Compilez le client**
```bash
make
```

3. **Compilez le serveur (optionnel)**
```bash
cd server && make
```

## ▶️ Lancer le jeu

### Mode Solo / Versus IA / Multijoueur Local
```bash
./main
```
Sélectionnez le mode depuis le menu principal.

### Mode Multijoueur Réseau

**Terminal 1 - Démarrer le serveur :**
```bash
cd server
./main
```
Le serveur écoute sur le port **8080**.

**Terminal 2 - Joueur 1 :**
```bash
./main localhost
```

**Terminal 3 - Joueur 2 :**
```bash
./main localhost
```

Pour jouer sur des machines différentes, remplacez `localhost` par l'adresse IP du serveur.

## ⌨️ Contrôles

### Mode Solo / Versus IA / Réseau
| Action | Touche |
|--------|--------|
| Gauche | ← |
| Droite | → |
| Rotation | Z / X |
| Hold | C |
| Chute rapide | ↓ |
| Chute forcée | Espace |
| Pause | P |
| Menu | ESC |

### Mode Multijoueur Local

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
├── src/                     # Code source du client
│   ├── main.cpp
│   ├── tetrisMap.cpp        # Logique du plateau
│   ├── tetrimino.cpp        # Gestion des pièces
│   ├── tetrisAI.cpp         # Moteur IA
│   ├── client.cpp           # Client réseau
│   ├── menu.cpp             # Menu principal
│   └── ...
├── include/                 # Fichiers d'en-tête
├── server/                  # Code du serveur
│   ├── tetris_server.cpp
│   ├── server.cpp
│   ├── player.cpp
│   ├── room.cpp
│   └── Makefile
├── docs/                    # Documentation
├── Makefile
└── README.md
```

## 🏗️ Architecture

### Composants Client
- **TetrisMap** : Plateau 10×20, logique du jeu
- **Tetrimino** : Pièces et rotations (4 états)
- **TetrisAI** : IA avec heuristiques (hauteur agrégée, trous, rugosité, lignes)
- **Client** : Communication TCP/IP
- **Menu** : Interface de sélection

### Composants Serveur
- **Server** : Gestion des sockets TCP sur port 8080
- **TetrisServer** : Logique de coordination
- **Player** : Représentation d'un joueur
- **Room** : Gestion des salles de jeu

### Protocole Réseau
- `CODE_PLAYER_MAP` : Synchronise l'état du plateau
- `CODE_PLAYER_LINES` : Envoie les garbage lines
- `CODE_PLAYER_DEAD` : Signale la défaite
- `CODE_GAME_OVER` : Fin de partie

## 📊 Système de Score

| Lignes | Points |
|--------|--------|
| 1 | 40 |
| 2 | 100 |
| 3 | 300 |
| 4 | 1200 (Tetris) |

Le niveau augmente avec les points totaux. La vitesse de chute s'accélère progressivement.

## 🤖 Algorithme de l'IA

1. Détecte l'arrivée d'une nouvelle pièce
2. Évalue 40 positions possibles (10 colonnes × 4 rotations)
3. Simule le placement pour chaque position
4. Calcule un score selon :
   - Hauteur agrégée (à minimiser)
   - Nombre de trous (à minimiser)
   - Rugosité du plateau (à minimiser)
   - Lignes complètes (à maximiser)
5. Exécute le meilleur mouvement

## 📚 Documentation supplémentaire

- [Fonctionnalités détaillées](docs/documentation_fonctionalittes.md)
- [Relation avec le cours](docs/relation_avec_cours.md)

## 🎓 Contexte

Projet réalisé dans le cadre du cours **IN204** (Programmation Orientée Objet), démontrant :
- Programmation C++ moderne
- Programmation concurrente (threads, mutex)
- Communication réseau (TCP/IP)
- Programmation graphique (SDL2)
- Intelligence artificielle heuristique
- Architecture client-serveur

## ✍️ Auteurs

- Vinicius Zancheta
- Edilberto

## 📄 Licence

Projet à but éducatif.

---

**Dernière mise à jour** : Janvier 2026
