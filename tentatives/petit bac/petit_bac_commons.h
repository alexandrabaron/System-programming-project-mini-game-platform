#ifndef PETIT_BAC_COMMONS_H
#define PETIT_BAC_COMMONS_H

#define WORD_LENGTH 100
#define MAX_WORDS 5

typedef struct {
    char *name;
    int (*validate)(char *word, char letter, char **dict, int dictSize);
    char *dict_file;
} Category;

char generateLetter();
int loadDictionary(const char *filename, char ***dict, int *dictSize);
void remove_accents(char* str);
int isValidWord(char *word, char letter, char **dict, int dictSize);
int isValidPrenom(char *word, char letter, char **dict, int dictSize);
int isValidAnimal(char *word, char letter, char **dict, int dictSize);
int isValidFruit(char *word, char letter, char **dict, int dictSize);
int isValidMetier(char *word, char letter, char **dict, int dictSize);
int isValidCity(char *word, char letter, char **dict, int dictSize);
void printMenu(int canal);

#endif // PETIT_BAC_COMMONS_H
