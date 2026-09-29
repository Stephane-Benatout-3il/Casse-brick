# 🧱 Casse-Briques

Projet réalisé dans le cadre du cours de **Principe de conception/développement en jeu vidéo**.

L'objectif du projet est de développer un jeu de type **Casse-Briques (Breakout)** avec **Unreal Engine 5**, en utilisant principalement **C++** ainsi que les **Blueprints** pour certains éléments du jeu et de l'interface.

## 🎮 Présentation

Le joueur contrôle une raquette située en bas de l'écran et doit détruire l'ensemble des briques à l'aide d'une balle.

La partie se termine lorsque :
- toutes les briques ont été détruites ;
- ou le joueur a perdu toutes ses vies.

La disposition des briques est générée aléatoirement à chaque nouvelle partie tout en conservant une symétrie horizontale.

## 🕹️ Contrôles

| Action | Touche Clavier | Manette |
|---|---|---|
| Déplacer la raquette | Gauche / Droite, A/D, Q/D | Gauche / Droite, Joystick Gauche|
| Lancer la balle | Espace | A / X (suivant la configuration de la manette) |
| Pause | Échap | Start |

## ✨ Fonctionnalités

Le jeu comprend notamment :

- système de score ;
- système de vies ;
- système de combo jusqu'à **x5** ;
- briques possédant différents niveaux de résistance ;
- génération aléatoire et symétrique des briques ;
- écran de menu principal ;
- menu pause ;
- écran de Game Over ;
- écran de victoire ;
- effets visuels lors de la destruction des briques ;
- effets sonores ;
- sauvegarde du score et des vies lors des changements de niveau.

### 🎁 Power-ups

Certaines briques peuvent faire apparaître aléatoirement un power-up :

- **Agrandissement de la raquette** : augmente temporairement sa taille ;
- **Multiball** : ajoute deux balles supplémentaires ;
- **Balle explosive** : permet à la balle de détruire également les briques situées autour de son point d'impact de manière temporaire.

## 🛠️ Technologies utilisées

- **Unreal Engine 5.8**
- **C++**
- **Blueprints**
- **Enhanced Input**
- **UMG (Unreal Motion Graphics)**
- **Niagara**
- **Git / GitHub**

## 📁 Structure du projet

```text
Casse-brick/
├── Build/
├── Config/
├── Content/
├── Source/
├── CasseBriques.uproject
└── README.md
```

Le dossier `Source` contient notamment les classes C++ responsables des principales mécaniques du jeu :

- `Ball`
- `Paddle`
- `Brick`
- `BrickSpawner`
- `DeathZone`
- `PowerUp`
- `BreakoutGameMode`
- `BreakoutGameInstance`

## 💻 Compilation

Le projet nécessite **Unreal Engine 5.8** ainsi qu'un environnement de développement C++ compatible avec Unreal Engine.

1. Cloner le dépôt.
2. Ouvrir `CasseBriques.uproject`.
3. Générer/compiler les fichiers C++ si Unreal Engine le demande.
4. Ouvrir le projet dans Unreal Engine.
5. Lancer le jeu depuis l'éditeur ou générer un build pour la plateforme souhaitée.

## 👤 Auteur

**Stéphane Benatout**

Projet étudiant réalisé en 2026.
