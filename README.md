# Projet Attaxx

**Auteurs :** Tran Sylvain, Thai Kevin

**Date :** 2021 (L2, mi-semestre 3)

**Objectif :**  
Réaliser en C le jeu Attaxx qui est une variante du jeu [Reversi](https://www.google.com/search?channel=fs&client=ubuntu&q=jeu+reversi).

**Principe du jeu :**  
C’est un jeu à deux joueurs consistant à posséder le plus de pions à la fin du jeu.  
Pour gagner un pion adverse, il suffit de placer un pion sur l'une des cases voisines de celui-ci.  
Le jeu prend fin lorsque le plateau est rempli ou lorsque l’un des deux joueurs ne possède plus de pions de sa couleur sur le plateau.
Celui qui possède alors le plus de pions gagne.

---

## Usage :
```
make
./attaxx [-a | -g] [-h | -o]
```

**Détails des options :**  
Affichage :  
`-a` --> ASCII  
`-g` --> graphique

Mode :  
`-h` --> J1 VS J2  
`-o` --> J1 VS Bot

**Documentation Utilisateur :**  
Voir le fichier [Rapport_Attaxx.pdf](https://github.com/syltran/attaxx/blob/master/Rapport_Attaxx.pdf)