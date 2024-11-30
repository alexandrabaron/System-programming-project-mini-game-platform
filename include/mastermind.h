#ifndef MASTERMIND_2J_H
#define MASTERMIND_2J_H

#define CODE_LENGTH 4
#define LIGNE_MAX 1024

void generateCode(int code[], int length);
void evaluateGuess(const int code[], const int guess[], int length, int *correctDigits, int *correctPositions);
void revealCode(const int code[], int length, int canal);
int calculatePoints(int maxTentatives, int tentatives);
void *jouer_mastermind_pour_joueur(void *args);
void jouer_mastermind(int canal1, int canal2);
void comparer_points(int score1, int score2, int tentatives1, int tentatives2, int canal1, int canal2);

#endif // MASTERMIND_2J_H

