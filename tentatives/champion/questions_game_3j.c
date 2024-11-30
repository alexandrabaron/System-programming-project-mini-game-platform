#include "pse.h"
#include "questions.h"
#include <time.h>
#include "questions_commons.h"

#define MAX_SCORE 10
#define BUFFER_SIZE 1024  // Définir une taille de buffer plus grande pour éviter la troncature


int poser_question_3j(int canal1, int canal2, int canal3, Question *question);
void afficher_scores_3j(int score1, int score2, int score3, int canal1, int canal2, int canal3);

void jouer_questions_pour_un_champion_3j(int canal1, int canal2, int canal3) {
    srand(time(NULL));
    int score1 = 0, score2 = 0, score3 = 0;
    int questions_posees[MAX_SCORE] = {0};

    for (int i = 0; i < MAX_SCORE; i++) {
        int question_index;
        do {
            question_index = rand() % MAX_QUESTIONS;
        } while (questions_posees[question_index]);

        questions_posees[question_index] = 1;
        int gagnant = poser_question_3j(canal1, canal2, canal3, &questions[question_index]);

        if (gagnant == 1) {
            score1++;
        } else if (gagnant == 2) {
            score2++;
        } else if (gagnant == 3) {
            score3++;
        }
    }

    afficher_scores_3j(score1, score2, score3, canal1, canal2, canal3);
}


int poser_question_3j(int canal1, int canal2, int canal3, Question *question) {
    char shuffled_choices[MAX_CHOICES][MAX_QUESTION_LENGTH];
    memcpy(shuffled_choices, question->choices, sizeof(shuffled_choices));
    shuffle(shuffled_choices, MAX_CHOICES);

    char buffer[BUFFER_SIZE];
    int buffer_size = sizeof(buffer);
    int n = snprintf(buffer, buffer_size, "Question: %s\n1. %s\n2. %s\n3. %s\n4. %s\n",
                     question->question, shuffled_choices[0], shuffled_choices[1],
                     shuffled_choices[2], shuffled_choices[3]);

    if (n >= buffer_size) {
        fprintf(stderr, "Erreur: Buffer trop petit pour contenir la question et les réponses.\n");
        exit(EXIT_FAILURE);
    }

    ecrireLigne(canal1, buffer);
    ecrireLigne(canal2, buffer);
    ecrireLigne(canal3, buffer);

    char reponse1[LIGNE_MAX], reponse2[LIGNE_MAX], reponse3[LIGNE_MAX];
    int reponse1_int = 0, reponse2_int = 0, reponse3_int = 0;

    lireLigne(canal1, reponse1);
    reponse1_int = atoi(reponse1);

    if (reponse1_int > 0 && reponse1_int <= 4 && 
        strcmp(shuffled_choices[reponse1_int - 1], question->choices[0]) == 0) {
        ecrireLigne(canal1, "Correct! Vous gagnez un point.\n");
        ecrireLigne(canal2, "L'autre joueur a répondu correctement.\n");
        ecrireLigne(canal3, "L'autre joueur a répondu correctement.\n");
        return 1;
    }

    ecrireLigne(canal2, "Votre tour de répondre.\n");
    lireLigne(canal2, reponse2);
    reponse2_int = atoi(reponse2);

    if (reponse2_int > 0 && reponse2_int <= 4 && 
        strcmp(shuffled_choices[reponse2_int - 1], question->choices[0]) == 0) {
        ecrireLigne(canal2, "Correct! Vous gagnez un point.\n");
        ecrireLigne(canal1, "L'autre joueur a répondu correctement.\n");
        ecrireLigne(canal3, "L'autre joueur a répondu correctement.\n");
        return 2;
    }

    ecrireLigne(canal3, "Votre tour de répondre.\n");
    lireLigne(canal3, reponse3);
    reponse3_int = atoi(reponse3);

    if (reponse3_int > 0 && reponse3_int <= 4 && 
        strcmp(shuffled_choices[reponse3_int - 1], question->choices[0]) == 0) {
        ecrireLigne(canal3, "Correct! Vous gagnez un point.\n");
        ecrireLigne(canal1, "L'autre joueur a répondu correctement.\n");
        ecrireLigne(canal2, "L'autre joueur a répondu correctement.\n");
        return 3;
    }

    ecrireLigne(canal1, "Les trois joueurs ont répondu incorrectement.\n");
    ecrireLigne(canal2, "Les trois joueurs ont répondu incorrectement.\n");
    ecrireLigne(canal3, "Les trois joueurs ont répondu incorrectement.\n");
    return 0;
}

void afficher_scores_3j(int score1, int score2, int score3, int canal1, int canal2, int canal3) {
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "Scores finaux:\nJoueur 1: %d\nJoueur 2: %d\nJoueur 3: %d\n",
             score1, score2, score3);
    ecrireLigne(canal1, buffer);
    ecrireLigne(canal2, buffer);
    ecrireLigne(canal3, buffer);

    if (score1 > score2 && score1 > score3) {
        ecrireLigne(canal1, "Vous avez gagné!\n");
        ecrireLigne(canal2, "L'autre joueur a gagné.\n");
        ecrireLigne(canal3, "L'autre joueur a gagné.\n");
    } else if (score2 > score1 && score2 > score3) {
        ecrireLigne(canal1, "L'autre joueur a gagné.\n");
        ecrireLigne(canal2, "Vous avez gagné!\n");
        ecrireLigne(canal3, "L'autre joueur a gagné.\n");
    } else if (score3 > score1 && score3 > score2) {
        ecrireLigne(canal1, "L'autre joueur a gagné.\n");
        ecrireLigne(canal2, "L'autre joueur a gagné.\n");
        ecrireLigne(canal3, "Vous avez gagné!\n");
    } else {
        ecrireLigne(canal1, "Égalité!\n");
        ecrireLigne(canal2, "Égalité!\n");
        ecrireLigne(canal3, "Égalité!\n");
    }
}

