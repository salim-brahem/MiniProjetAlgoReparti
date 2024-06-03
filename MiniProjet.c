#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>

// Nombre de processus fils
#define NUM_CHILDREN 6

// Noms des sémaphores
#define SEM_START "sem_start"
#define SEM_SYNC "sem_sync"

pid_t children[NUM_CHILDREN];
sem_t *sem_start, *sem_sync;

// Gestionnaire de signal pour les fils
void signal_handler(int sig) {
    switch(sig) {
        case SIGUSR1:
            printf("Processus fils %d : reçu SIGUSR1, commence la tâche\n", getpid());
            
            // Tâche complexe: Calcul de la somme des nombres de 1 à 99000000
            long long sum = 0;
            for (long long i = 1; i <= 99000000; ++i) {
                sum += i;
            }
            printf("Processus fils %d : tâche terminée avec sum = %lld\n", getpid(), sum);

            kill(getppid(), SIGUSR2); // Envoyer confirmation au père
            break;
        case SIGTERM:
            printf("Processus fils %d : reçu SIGTERM, terminer\n", getpid());
            exit(0);
            break;
    }
}

// Gestionnaire de signal pour le père
void parent_signal_handler(int sig) {
    if (sig == SIGUSR2) {
        printf("Processus père : reçu confirmation d'un fils\n");
    }
}

// Fonction pour les processus fils
void child_process() {
    signal(SIGUSR1, signal_handler);
    signal(SIGTERM, signal_handler);

    // Attendre le signal de départ du père
    sem_wait(sem_start);

    // Signaler que ce processus est prêt
    sem_post(sem_sync);

    // Boucle pour attendre les signaux
    while (1) {
        pause();
    }
}

int main() {
    sem_unlink(SEM_START);
    sem_unlink(SEM_SYNC);

    // Initialiser les sémaphores
    sem_start = sem_open(SEM_START, O_CREAT, 0644, 0);
    sem_sync = sem_open(SEM_SYNC, O_CREAT, 0644, 0);
    if (sem_start == SEM_FAILED || sem_sync == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    signal(SIGUSR2, parent_signal_handler);

    // Créer les processus fils
    for (int i = 0; i < NUM_CHILDREN; ++i) {
        if ((children[i] = fork()) == 0) {
            child_process();
            exit(0);
        }
    }

    // Attendre que tous les processus fils soient prêts
    for (int i = 0; i < NUM_CHILDREN; ++i) {
        sem_post(sem_start);
    }
    for (int i = 0; i < NUM_CHILDREN; ++i) {
        sem_wait(sem_sync);
    }

    // Envoyer SIGUSR1 à tous les processus fils pour démarrer la tâche
    for (int i = 0; i < NUM_CHILDREN; ++i) {
        kill(children[i], SIGUSR1);
    }

    // Attendre la fin des tâches des processus fils
    for (int i = 0; i < NUM_CHILDREN; ++i) {
        wait(NULL);
    }

    // Fermer et supprimer les sémaphores
    sem_close(sem_start);
    sem_close(sem_sync);
    sem_unlink(SEM_START);
    sem_unlink(SEM_SYNC);

    printf("Processus père : toutes les tâches des fils sont terminées\n");
    return 0;
}
