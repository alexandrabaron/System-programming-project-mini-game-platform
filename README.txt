///////très important de lire ce fichier en entier pour comprendre le fonctionnement des jeux///////

BIENVENUE SUR NOTRE SUPERBE PLATEFORME DE MINI JEUX


------------Auteurs------------

Ce projet a été réalisé en collaboration par Alexandra Baron et Rose Cymbler. 

------------Mini-Jeux Multijoueurs------------

Ce projet implémente plusieurs mini-jeux multijoueurs (pour 2 ou 3 joueurs) en utilisant des sockets et des threads en C. Les jeux disponibles sont :

* Anagrammes
* MasterMind

------------Structure du Projet------------

Le projet est structuré de la manière suivante :

    ** projet :  Contient les exécutables du serveur et du client ainsi que les fichiers dictionnaires nécessaires pour le jeu Petit Bac.
    ** modules : Contient les fichiers source et les en-têtes nécessaires pour les différents jeux.
    ** lib : Contient les bibliothèques utilisées par le projet.
    ** include : Contient les fichiers d'en-tête (header files) partagés entre différents modules du projet.
    ** tentatives : Contient des tentatives de codes pour des mini-jeux que nous n'avons pas réussi à finaliser.


------------Compilation------------

** Compiler les modules : 
	cd modules 
	make
** Compiler projet :
	cd ../projet
	make
	
------------Exécution------------

** Lancer le serveur : ./serveur <port>

** Lancer les clients : ./client localhost <port>


------------Description jeux------------

** Anagrammes

Dans ce jeu, les joueurs doivent trouver le plus de mots possible à partir d'une série de lettres données. Le joueur avec le plus de points gagne.


** MasterMind

Chaque joueur doit deviner une combinaison de 4 chiffres différents générée aléatoirement par le serveur. Le joueur doit entrer une combinaison de 4 chiffres et reçoit des indications sur le nombre de chiffres corrects et bien placés. Le joueur qui trouve la combinaison en le moins de tentatives gagne.


!!!!!!!!!!!!!!!!!IMPORTANT!!!!!!!!!!!!!!!!!
------------Lancement d'une partie------------
Le joueur pouvant faire le choix, dans le menu, du jeu auxquel les participants vont jouer, est uniquement et spécifiquement le joueur qui s'est connecté en premier à la partie. 

PS : 
Nous avons particulièrement apprécié ce projet. Il nous a permis de dépasser nos limites et de surmonter de nombreux défis.
