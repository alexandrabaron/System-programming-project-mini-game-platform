#include "pse.h"
#include "mastermind.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>


typedef struct {
    int canal;
    const int *code;
    int *total_points;
    int *tentatives;
} JoueurArgs;

void generateCode(int code[], int length) {
    int used[10] = {0};
    int num;
    for (int i = 0; i < length; i++) {
        do {
            num = rand() % 10;
        } while (used[num]);
        code[i] = num;
        used[num] = 1;
    }
    // Afficher la combinaison pour vérifier la génération correcte
    printf("Code généré: ");
    for (int i = 0; i < length; i++) {
        printf("%d ", code[i]);
    }
    printf("\n");
}

void evaluateGuess(const int code[], const int guess[], int length, int *correctDigits, int *correctPositions) {
    *correctDigits = 0;
    *correctPositions = 0;
    int codeDigitCount[10] = {0};
    int guessDigitCount[10] = {0};

    for (int i = 0; i < length; i++) {
        if (guess[i] == code[i]) {
            (*correctPositions)++;
            (*correctDigits)++;
        } else {
            codeDigitCount[code[i]]++;
            guessDigitCount[guess[i]]++;
        }
    }

    for (int i = 0; i < 10; i++) {
        *correctDigits += codeDigitCount[i] < guessDigitCount[i] ? codeDigitCount[i] : guessDigitCount[i];
    }
}

void revealCode(const int code[], int length, int canal) {
    char buffer[LIGNE_MAX];
    snprintf(buffer, sizeof(buffer), "La combinaison était : ");
    ecrireLigne(canal, buffer);
    for (int i = 0; i < length; i++) {
        snprintf(buffer, sizeof(buffer), "%d", code[i]);
        ecrireLigne(canal, buffer);
    }
    ecrireLigne(canal, "\n");
}

int calculatePoints(int maxTentatives, int tentatives) {
    return maxTentatives - tentatives + 1;
}

void *jouer_mastermind_pour_joueur(void *args) {
    JoueurArgs *joueurArgs = (JoueurArgs *)args;
    int canal = joueurArgs->canal;
    const int *code = joueurArgs->code;
    int *total_points = joueurArgs->total_points;
    int *tentatives = joueurArgs->tentatives;
    char buffer[LIGNE_MAX];
    
    ecrireLigne(canal, "Bienvenue au jeu de Mastermind!\n");

    int guess[CODE_LENGTH];
    int correctDigits = 0, correctPositions = 0;
    char input[CODE_LENGTH + 1];
    int maxTentatives = 10;
    *tentatives = 0;

    while (correctPositions < CODE_LENGTH && *tentatives < maxTentatives) {
        snprintf(buffer, sizeof(buffer), "Entrez votre combinaison (%d chiffres, ou 'stop' pour arrêter) : ", CODE_LENGTH);
        ecrireLigne(canal, buffer);
        lireLigne(canal, input);
        
        if (strcmp(input, "stop") == 0) {
            revealCode(code, CODE_LENGTH, canal);
            break;
        }
        if (strlen(input) != CODE_LENGTH) {
            snprintf(buffer, sizeof(buffer), "Combinaison invalide. Entrez exactement %d chiffres.\n", CODE_LENGTH);
            ecrireLigne(canal, buffer);
            continue;
        }

        for (int i = 0; i < CODE_LENGTH; i++) {
            guess[i] = input[i] - '0';
        }

        evaluateGuess(code, guess, CODE_LENGTH, &correctDigits, &correctPositions);
        snprintf(buffer, sizeof(buffer), "Chiffres corrects: %d, Chiffres bien placés: %d\n", correctDigits, correctPositions);
        ecrireLigne(canal, buffer);
        
        (*tentatives)++;
    }

    if (correctPositions == CODE_LENGTH) {
        int points = calculatePoints(maxTentatives, *tentatives);
        *total_points += points;
        snprintf(buffer, sizeof(buffer), "Félicitations! Vous avez trouvé la combinaison en %d tentatives et vous avez gagné %d points!\n" , *tentatives, points);
        ecrireLigne(canal, buffer);
    } else {
        snprintf(buffer, sizeof(buffer),  "Vous avez épuisé vos tentatives. La combinaison était : " );
        ecrireLigne(canal, buffer);
        for (int i = 0; i < CODE_LENGTH; i++) {
            snprintf(buffer, sizeof(buffer), "%d", code[i]);
            ecrireLigne(canal, buffer);
        }
        ecrireLigne(canal, "\n");
    }

    pthread_exit(NULL);
}

void comparer_points(int score1, int score2, int tentatives1, int tentatives2, int canal1, int canal2) {
    char buffer[LIGNE_MAX];

    if (score1 > score2) {
        snprintf(buffer, sizeof(buffer), "Le joueur 1 est le vainqueur avec %d points!\n", score1);
    } else if (score2 > score1) {
        snprintf(buffer, sizeof(buffer), "Le joueur 2 est le vainqueur avec %d points!\n", score2);
    } else if (tentatives1 < tentatives2) {
        snprintf(buffer, sizeof(buffer), "Égalité en points! Le joueur 1 est le vainqueur avec moins de tentatives (%d)!\n", tentatives1);
    } else if (tentatives2 < tentatives1) {
        snprintf(buffer, sizeof(buffer), "Égalité en points! Le joueur 2 est le vainqueur avec moins de tentatives (%d)!\n", tentatives2);
    } else {
        snprintf(buffer, sizeof(buffer), "Égalité parfaite! Les deux joueurs ont %d points et ont utilisé %d tentatives!\n", score1, tentatives1);
    }

    ecrireLigne(canal1, buffer);
    ecrireLigne(canal2, buffer);
}

void jouer_mastermind(int canal1, int canal2) {
    char buffer[LIGNE_MAX];
    int code[CODE_LENGTH];

    srand(time(NULL));
    generateCode(code, CODE_LENGTH);

    int total_points1 = 0;
    int total_points2 = 0;
    int tentatives1 = 0;
    int tentatives2 = 0;

    snprintf(buffer, sizeof(buffer), "Devinez la combinaison de %d chiffres.\n", CODE_LENGTH);
    ecrireLigne(canal1, buffer);
    ecrireLigne(canal2, buffer);

    JoueurArgs args1 = {canal1, code, &total_points1, &tentatives1};
    JoueurArgs args2 = {canal2, code, &total_points2, &tentatives2};

    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, jouer_mastermind_pour_joueur, &args1);
    pthread_create(&thread2, NULL, jouer_mastermind_pour_joueur, &args2);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    snprintf(buffer, sizeof(buffer), "Temps écoulé! POINTS TOTAUX: Joueur 1: %d, Joueur 2: %d\n", total_points1, total_points2);
    ecrireLigne(canal1, buffer);
    ecrireLigne(canal2, buffer);

    comparer_points(total_points1, total_points2, tentatives1, tentatives2, canal1, canal2);
}

