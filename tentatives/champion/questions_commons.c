// questions_commons.c
#include "questions_commons.h"

void shuffle(char arr[][MAX_QUESTION_LENGTH], int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        char temp[MAX_QUESTION_LENGTH];
        strcpy(temp, arr[i]);
        strcpy(arr[i], arr[j]);
        strcpy(arr[j], temp);
    }
}

