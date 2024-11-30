#include "pse.h"
#include "anagrammes_3j.h"
#include "anagrammes_commons.h"
#include <pthread.h>


typedef struct {
    int canal;
    const char *lettres;
    int *total_points;
} JoueurArgs;

void comparer_scores(int score1, int score2, int score3, int canal1, int canal2, int canal3) {
    char buffer[LIGNE_MAX];

    if (score1 > score2 && score1 > score3) {
        snprintf(buffer, sizeof(buffer), "Le joueur 1 est le vainqueur avec %d points!\n", score1);
    } else if (score2 > score1 && score2 > score3) {
        snprintf(buffer, sizeof(buffer), "Le joueur 2 est le vainqueur avec %d points!\n", score2);
    } else if (score3 > score1 && score3 > score2) {
        snprintf(buffer, sizeof(buffer), "Le joueur 3 est le vainqueur avec %d points!\n", score3);
    } else {
        snprintf(buffer, sizeof(buffer), "Égalité! Les scores sont : Joueur 1 : %d, Joueur 2 : %d, Joueur 3 : %d\n", score1, score2, score3);
    }

    ecrireLigne(canal1, buffer);
    ecrireLigne(canal2, buffer);
    ecrireLigne(canal3, buffer);
}

void *jouer_pour_un_joueur_3j_thread(void *args) {
    JoueurArgs *joueurArgs = (JoueurArgs *)args;
    int canal = joueurArgs->canal;
    const char *lettres = joueurArgs->lettres;
    int *total_points = joueurArgs->total_points;

    char mot[MAX_LONG_MOT];
    char mots_entrees[MAX_MOTS][MAX_LONG_MOT];
    int mot_compt = 0;
    time_t debut = time(NULL);

    while (time(NULL) - debut < 60) {
        int temps_ecoule = time(NULL) - debut;
        int temps_restant = 60 - temps_ecoule;
        char buffer1[LIGNE_MAX];
        char buffer2[LIGNE_MAX];
        char buffer[LIGNE_MAX];

        snprintf(buffer1, sizeof(buffer1), "------------Temps restant: %d secondes\n", temps_restant);
        snprintf(buffer2, sizeof(buffer2), "Lettres : %s", lettres);
        ecrireLigne(canal, buffer1);
        ecrireLigne(canal, buffer2);

        lireLigne(canal, mot);

        int deja_ecrit = 0;
        for (int i = 0; i < mot_compt; i++) {
            if (strcmp(mots_entrees[i], mot) == 0) {
                deja_ecrit = 1;
                break;
            }
        }

        if (deja_ecrit) {
            ecrireLigne(canal,  "Mot déjà entré!\n\n" );
            continue;
        }

        int mot_valide = 0;
        if (verif_lettres_utilisees(mot, lettres)) {
            size_t length = strlen(mot);
            switch (length) {
                case 3:
                    mot_valide = is_in_dico(mot, dico_trois, DICO_TROIS_SIZE);
                    break;
                case 4:
                    mot_valide = is_in_dico(mot, dico_quatre, DICO_QUATRE_SIZE);
                    break;
                case 5:
                    mot_valide = is_in_dico(mot, dico_cinq, DICO_CINQ_SIZE);
                    break;
                case 6:
                    mot_valide = is_in_dico(mot, dico_six, DICO_SIX_SIZE);
                    break;
            }
        }

        if (mot_valide) {
            strcpy(mots_entrees[mot_compt++], mot);
            int points = calcul_points(mot);
            *total_points += points;
            snprintf(buffer, sizeof(buffer),  "Mot valide! Points: %d\n" , points);
            ecrireLigne(canal, buffer);
        } else {
            ecrireLigne(canal,  "Mot invalide!\n" );
        }
    }

    char fin_message[LIGNE_MAX];
    snprintf(fin_message, sizeof(fin_message),  "Temps écoulé! POINTS TOTAUX: %d\n" , *total_points);
    ecrireLigne(canal, fin_message);
    pthread_exit(NULL);
}

void jouer_anagrammes_3j(int canal1, int canal2, int canal3) {
    char lettres[7];
    generation_aleatoire_lettres(lettres);

    int total_points1 = 0;
    int total_points2 = 0;
    int total_points3 = 0;

    ecrireLigne(canal1, "Vous avez 1 minute pour trouver le plus de mots possible.\n");
    ecrireLigne(canal2, "Vous avez 1 minute pour trouver le plus de mots possible.\n");
    ecrireLigne(canal3, "Vous avez 1 minute pour trouver le plus de mots possible.\n");

    char buffer[LIGNE_MAX];
    snprintf(buffer, sizeof(buffer), "Lettres: %s\n", lettres);
    ecrireLigne(canal1, buffer);
    ecrireLigne(canal2, buffer);
    ecrireLigne(canal3, buffer);

    JoueurArgs args1 = {canal1, lettres, &total_points1};
    JoueurArgs args2 = {canal2, lettres, &total_points2};
    JoueurArgs args3 = {canal3, lettres, &total_points3};

    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, jouer_pour_un_joueur_3j_thread, &args1);
    pthread_create(&thread2, NULL, jouer_pour_un_joueur_3j_thread, &args2);
    pthread_create(&thread3, NULL, jouer_pour_un_joueur_3j_thread, &args3);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    snprintf(buffer, sizeof(buffer), "Temps écoulé! POINTS TOTAUX: Joueur 1: %d, Joueur 2: %d, Joueur 3: %d\n", total_points1, total_points2, total_points3);
    ecrireLigne(canal1, buffer);
    ecrireLigne(canal2, buffer);
    ecrireLigne(canal3, buffer);

    comparer_scores(total_points1, total_points2, total_points3, canal1, canal2, canal3);
}

