# Gestion Avancée des Processus et des Signaux en C sous Linux

## Contexte

Ce projet illustre la gestion avancée des processus et des signaux sous Linux en utilisant le langage C. Le programme crée plusieurs processus fils à partir d'un processus père, gère la communication entre ces processus à l'aide de signaux, et implémente des mécanismes de synchronisation pour coordonner les tâches des processus fils.

## Objectifs

1. Créer un processus père qui génère six processus fils.
2. Le processus père envoie des signaux aux processus fils pour leur demander d'exécuter des tâches spécifiques.
3. Les processus fils traitent les signaux reçus, effectuent des tâches complexes et répondent en envoyant des signaux de confirmation au processus père.
4. Le processus père affiche un message chaque fois qu'il reçoit une confirmation d'un processus fils.
5. Implémenter une synchronisation entre les processus fils pour qu'ils ne commencent leurs tâches qu'une fois qu'ils ont tous reçu un signal de départ.
6. Utiliser un mécanisme de sémaphore pour gérer la synchronisation.

## Prérequis

Pour compiler et exécuter ce programme, vous aurez besoin de :

- Un système Linux
- Le compilateur GCC
- Les bibliothèques POSIX pour la gestion des signaux et des sémaphores

## Compilation

Pour compiler le programme, ouvrez un terminal et exécutez la commande suivante :

```sh
gcc -o process_sync process_sync.c -pthread
```
## Exécution

Après avoir compilé le programme, vous pouvez l'exécuter avec la commande suivante :
```sh
./process_sync
```
