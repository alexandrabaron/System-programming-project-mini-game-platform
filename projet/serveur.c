// serveur.c
#include "pse.h"
#include "anagrammes_2j.h"
#include "anagrammes_3j.h"
#include "mastermind.h"

#define CMD "serveur"
#define NB_WORKERS 3 
#define MAX_JOUEURS 3

void creerCohorteWorkers(void);
int chercherWorkerLibre(void);
void *threadWorker(void *arg);
void sessionClient(int canal);

void init_semaphore(sem_t *sem, int valeur);
void wait_semaphore(sem_t *sem);
void post_semaphore(sem_t *sem);
void lock_mutex(pthread_mutex_t *mutex);
void unlock_mutex(pthread_mutex_t *mutex);

void ajouterJoueur(int canal);
void envoyerMenu(int canal, int nbJoueurs);
void gestionSelectionJeu(int canal);
void debuterJeu(int canals[], int nbJoueurs, int choix);

// Variables globales pour gérer les joueurs et l'état du jeu
int joueurs[MAX_JOUEURS];
int nbJoueurs = 0;
pthread_mutex_t mutexJoueurs = PTHREAD_MUTEX_INITIALIZER;

sem_t semJeu; // Sémaphore pour gérer l'état du jeu

typedef struct {
    int enCours; // 0 si pas de partie en cours, 1 si une partie est en cours
    int choix;   // choix du jeu actuel
} EtatJeu;

EtatJeu etatJeu = {0, 0}; // Initialisation de l'état du jeu

// Ajoute un joueur au tableau des joueurs en utilisant un mutex pour la synchronisation
void ajouterJoueur(int canal) {
    lock_mutex(&mutexJoueurs);
    joueurs[nbJoueurs++] = canal;
    unlock_mutex(&mutexJoueurs);
}

// Envoie le menu de sélection des jeux au joueur connecté
void envoyerMenu(int canal, int nbJoueurs) {
    char menu1[LIGNE_MAX];
    char menu2[LIGNE_MAX];
    char menu5[LIGNE_MAX];
    
    snprintf(menu1, sizeof(menu1), "----------MINI-JEUX----------\n ");
    snprintf(menu2, sizeof(menu2), "1. Anagrammes \n");
    
    if (nbJoueurs == 2) {
        snprintf(menu5, sizeof(menu5), "2. MasterMind\n");
    } 
    ecrireLigne(canal, menu1);
    ecrireLigne(canal, menu2);
    if (nbJoueurs == 2) {
        ecrireLigne(canal, menu5); 
    }
}

void gestionSelectionJeu(int canal) {
    char selection[LIGNE_MAX];
    lireLigne(canal, selection);
    int choix = atoi(selection);
    lock_mutex(&mutexJoueurs);
    etatJeu.choix = choix;
    etatJeu.enCours = 1; // Indiquer qu'une partie est en cours
    unlock_mutex(&mutexJoueurs);
    debuterJeu(joueurs, nbJoueurs, choix);
    lock_mutex(&mutexJoueurs);
    etatJeu.enCours = 0; // Partie terminée
    unlock_mutex(&mutexJoueurs);
}

void debuterJeu(int canals[], int nbJoueurs, int choix) {
    char compteRebours1[] = "Le jeu commence dans 3... \n";
    char compteRebours2[] = "2... \n";
    char compteRebours3[] = "1...\n";

    for (int i = 0; i < nbJoueurs; i++) {
        ecrireLigne(canals[i], compteRebours1);
    }
    sleep(1);
    for (int i = 0; i < nbJoueurs; i++) {
        ecrireLigne(canals[i], compteRebours2);
    }
    sleep(1);
    for (int i = 0; i < nbJoueurs; i++) {
        ecrireLigne(canals[i], compteRebours3);
    }
    sleep(1);

    switch (choix) {
        case 1:
            if (nbJoueurs == 2) {
                jouer_anagrammes_2j(canals[0], canals[1]);
            } else {
                jouer_anagrammes_3j(canals[0], canals[1], canals[2]);
            }
            break;
        case 2:
            if (nbJoueurs == 2) {
                jouer_mastermind(canals[0], canals[1]);
            } else {
                ecrireLigne(canals[0], "Choix invalide. Recommencez.\n");
                ecrireLigne(canals[1], "Choix invalide. Recommencez.\n");
                ecrireLigne(canals[2], "Choix invalide. Recommencez.\n");
            }
            break;
        
        
        default:
            for (int i = 0; i < nbJoueurs; i++) {
                ecrireLigne(canals[i], "Choix invalide. Recommencez.\n");
            }
            break;
    }
}

DataSpec dataWorkers[NB_WORKERS];
sem_t semWorkersLibres;

int main(int argc, char *argv[]) {
    srand(time(NULL));
    short port;
    int ecoute, canal, ret;
    struct sockaddr_in adrEcoute, adrClient;
    unsigned int lgAdrClient;
    int numWorkerLibre;

    if (argc != 2)
        erreur("usage: %s port\n", argv[0]);

    port = (short)atoi(argv[1]);

    creerCohorteWorkers();
    init_semaphore(&semWorkersLibres, NB_WORKERS);
    init_semaphore(&semJeu, 1); // Initialiser le sémaphore pour le jeu

    printf("%s: creating a socket\n", CMD);
    ecoute = socket(AF_INET, SOCK_STREAM, 0);
    if (ecoute < 0)
        erreur_IO("socket");

    adrEcoute.sin_family = AF_INET;
    adrEcoute.sin_addr.s_addr = INADDR_ANY;
    adrEcoute.sin_port = htons(port);
    printf("%s: binding to INADDR_ANY address on port %d\n", CMD, port);
    ret = bind(ecoute, (struct sockaddr *)&adrEcoute, sizeof(adrEcoute));
    if (ret < 0)
        erreur_IO("bind");

    printf("%s: listening to socket\n", CMD);
    ret = listen(ecoute, 5);
    if (ret < 0)
        erreur_IO("listen");

    while (VRAI) {
        printf("%s: accepting a connection\n", CMD);
        lgAdrClient = sizeof(adrClient);
        canal = accept(ecoute, (struct sockaddr *)&adrClient, &lgAdrClient);
        if (canal < 0) {
            erreur_IO("accept");
        }

        printf("%s: adr %s, port %hu\n", CMD,
               stringIP(ntohl(adrClient.sin_addr.s_addr)),
               ntohs(adrClient.sin_port));

        sem_wait(&semWorkersLibres);
        numWorkerLibre = chercherWorkerLibre();
        dataWorkers[numWorkerLibre].canal = canal;
        post_semaphore(&dataWorkers[numWorkerLibre].sem);
    }

    if (close(ecoute) == -1)
        erreur_IO("fermeture socket ecoute");

    exit(EXIT_SUCCESS);
}

void creerCohorteWorkers(void) {
    int i;
    int ret;

    for (i = 0; i < NB_WORKERS; i++) {
        dataWorkers[i].canal = -1;
        dataWorkers[i].tid = i;
        init_semaphore(&dataWorkers[i].sem, 0);

        ret = pthread_mutex_init(&dataWorkers[i].mutex, NULL);
        if (ret != 0)
            erreur_IO("init mutex");

        ret = pthread_create(&dataWorkers[i].id, NULL, threadWorker,
                             &dataWorkers[i]);
        if (ret != 0)
            erreur_IO("creation worker");
    }
}

int chercherWorkerLibre(void) {
    int i;
    int canal;

    for (i = 0; i < NB_WORKERS; i++) {
        lock_mutex(&dataWorkers[i].mutex);
        canal = dataWorkers[i].canal;
        unlock_mutex(&dataWorkers[i].mutex);

        if (canal == -1)
            return i;
    }

    return -1;
}

void *threadWorker(void *arg) {
    DataSpec *dataSpec = (DataSpec *)arg;

    while (VRAI) {
        wait_semaphore(&dataSpec->sem);
        printf("worker %d: reveil\n", dataSpec->tid);

        sessionClient(dataSpec->canal);

        printf("worker %d: sommeil\n", dataSpec->tid);
        lock_mutex(&dataSpec->mutex);
        dataSpec->canal = -1;
        unlock_mutex(&dataSpec->mutex);

        post_semaphore(&semWorkersLibres);
    }

    pthread_exit(NULL);
}

void sessionClient(int canal) {
    printf("sessionClient: ajout du joueur au canal %d\n", canal);
    ajouterJoueur(canal);

    if (nbJoueurs == 1) {
        ecrireLigne(canal, "En attente d'un autre joueur...\n");
        while (nbJoueurs < 2) {
            sleep(1);
        }
    } else {
        ecrireLigne(joueurs[0], "Deux joueurs sont maintenant connectés. 15s d'attente pour un 3e joueur potentiel.\n");
        ecrireLigne(joueurs[1], "Deux joueurs sont maintenant connectés. 15s d'attente pour un 3e joueur potentiel.\n");
        sleep(15);  // Attendre 15 secondes pour un troisième joueur
        if (nbJoueurs == 2) {
            envoyerMenu(joueurs[0], 2);
            envoyerMenu(joueurs[1], 2);
        } else if (nbJoueurs == 3) {
            envoyerMenu(joueurs[0], 3);
            envoyerMenu(joueurs[1], 3);
            envoyerMenu(joueurs[2], 3);
        }
    }

    while (1) {
        lock_mutex(&mutexJoueurs);
        if (etatJeu.enCours) {
            unlock_mutex(&mutexJoueurs);
            sleep(1); // Attendre que la partie se termine
            continue;
        }
        unlock_mutex(&mutexJoueurs);

        printf("sessionClient: en attente de la sélection du jeu\n");
        sem_wait(&semJeu); // Attendre que le sémaphore soit disponible
        gestionSelectionJeu(canal);
        sem_post(&semJeu); // Libérer le sémaphore

        // Réafficher le menu après chaque partie
        for (int i = 0; i < nbJoueurs; i++) {
            envoyerMenu(joueurs[i], nbJoueurs);
        }
    }

    if (close(canal) == -1)
        erreur_IO("fermeture canal");
}

void init_semaphore(sem_t *sem, int valeur) {
    int ret;
    ret = sem_init(sem, 0, valeur);
    if (ret != 0)
        erreur_IO("init semaphore");
}

void wait_semaphore(sem_t *sem) {
    int ret;
    ret = sem_wait(sem);
    if (ret != 0)
        erreur_IO("wait semaphore");
}

void post_semaphore(sem_t *sem) {
    int ret;
    ret = sem_post(sem);
    if (ret != 0)
        erreur_IO("post semaphore");
}

void lock_mutex(pthread_mutex_t *mutex) {
    int ret;
    ret = pthread_mutex_lock(mutex);
    if (ret != 0)
        erreur_IO("lock mutex");
}

void unlock_mutex(pthread_mutex_t *mutex) {
    int ret;
    ret = pthread_mutex_unlock(mutex);
    if (ret != 0)
        erreur_IO("unlock mutex");
}

