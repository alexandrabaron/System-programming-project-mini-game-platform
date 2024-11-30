// questions.h
#ifndef QUESTIONS_H
#define QUESTIONS_H

#include <stdlib.h>
#include <string.h>

#define MAX_QUESTION_LENGTH 256
#define MAX_CHOICES 4
#define MAX_QUESTIONS 78

typedef struct {
    char question[MAX_QUESTION_LENGTH];
    char choices[MAX_CHOICES][MAX_QUESTION_LENGTH];
} Question;

extern Question questions[MAX_QUESTIONS];

#endif // QUESTIONS_H

