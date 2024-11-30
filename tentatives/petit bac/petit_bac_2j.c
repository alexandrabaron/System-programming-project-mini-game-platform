#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include <locale.h>
#include "pse.h"
#include "petit_bac_commons.h"

#define MAX_PLAYERS 2

pthread_mutex_t score_mutex = PTHREAD_MUTEX_INITIALIZER;
int score = 0;

// Fonction pour générer une lettre aléatoire
char generateLetter() {
    char letter;
    do {
        letter = (char) (rand() % 26 + 'A');
    } while (letter == 'Q' || letter == 'W' || letter == 'X' || letter == 'Y' || letter == 'Z');
    return tolower(letter);
}

// Fonction pour charger les mots du dictionnaire dans un tableau
int loadDictionary(const char *filename, char ***dict, int *dictSize) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Erreur lors de l'ouverture du fichier");    
        return 0;
    }

    *dictSize = 0;
    char word[WORD_LENGTH];
    while (fgets(word, WORD_LENGTH, file)) {
        word[strcspn(word, "\n")] = '\0'; // Enlever le caractère de nouvelle ligne
        (*dictSize)++;
        *dict = (char **) realloc(*dict, sizeof(char *) * (*dictSize));
        (*dict)[*dictSize - 1] = (char *) malloc(sizeof(char) * (strlen(word) + 1));
        strcpy((*dict)[*dictSize - 1], word);
    }

    fclose(file);
    return 1;
}

// Fonction pour supprimer les accents d'une chaîne de caractères
void remove_accents(char* str) {
    char* src = str;
    char* dst = str;

    while (*src) {
        unsigned char c = *src;
        if (c >= 192 && c <= 197) *dst = 'A';
        else if (c == 199) *dst = 'C';
        else if (c >= 200 && c <= 203) *dst = 'E';
        else if (c >= 204 && c <= 207) *dst = 'I';
        else if (c == 209) *dst = 'N';
        else if (c >= 210 && c <= 214) *dst = 'O';
        else if (c >= 217 && c <= 220) *dst = 'U';
        else if (c == 221) *dst = 'Y';
        else if (c >= 224 && c <= 229) *dst = 'a';
        else if (c == 231) *dst = 'c';
        else if (c >= 232 && c <= 235) *dst = 'e';
        else if (c >= 236 && c <= 239) *dst = 'i';
        else if (c == 241) *dst = 'n';
        else if (c >= 242 && c <= 246) *dst = 'o';
        else if (c >= 249 && c <= 252) *dst = 'u';
        else if (c == 253 || c == 255) *dst = 'y';
        else *dst = c;
        src++;
        dst++;
    }
    *dst = '\0';
}

// Fonction pour vérifier si un mot est valide pour une catégorie
int isValidWord(char *word, char letter, char **dict, int dictSize) {
    char normalized_word[WORD_LENGTH];
    strcpy(normalized_word, word);
    remove_accents(normalized_word);

    if (tolower(normalized_word[0]) != letter) {
        return 0;
    }
    for (int i = 0; i < strlen(normalized_word); i++) {
        if (!isalpha(normalized_word[i]) && normalized_word[i] != '\'') {
            return 0;
        }
    }

    // Vérifier si le mot est dans le dictionnaire
    for (int i = 0; i < dictSize; i++) {
        char normalized_dict_word[WORD_LENGTH];
        strcpy(normalized_dict_word, dict[i]);
        remove_accents(normalized_dict_word);
        if (strcasecmp(normalized_word, normalized_dict_word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Fonction pour vérifier si un mot est valide pour la catégorie "Prénom"
int isValidPrenom(char *word, char letter, char **dict, int dictSize) {
    return isValidWord(word, letter, dict, dictSize);
}

// Fonction pour vérifier si un mot est valide pour la catégorie "Animal"
int isValidAnimal(char *word, char letter, char **dict, int dictSize) {
    return isValidWord(word, letter, dict, dictSize);
}

// Fonction pour vérifier si un mot est valide pour la catégorie "Fruit ou légume"
int isValidFruit(char *word, char letter, char **dict, int dictSize) {
    return isValidWord(word, letter, dict, dictSize);
}

// Fonction pour vérifier si un mot est valide pour la catégorie "Métier"
int isValidMetier(char *word, char letter, char **dict, int dictSize) {
    return isValidWord(word, letter, dict, dictSize);
}

// Fonction pour vérifier si un mot est valide pour la catégorie "Ville ou pays"
int isValidCity(char *word, char letter, char **dict, int dictSize) {
    return isValidWord(word, letter, dict, dictSize);
}

void printMenu(int canal) {
    ecrireLigne(canal, "Choisissez une catégorie ou tapez 'q' pour quitter :\n");
    ecrireLigne(canal, "1. Prénom\n");
    ecrireLigne(canal, "2. Animal\n");
    ecrireLigne(canal, "3. Fruit ou légume\n");
    ecrireLigne(canal, "4. Métier\n");
    ecrireLigne(canal, "5. Ville ou pays\n");
    ecrireLigne(canal, "q. Quitter\n");
}

void jouer_petit_bac_pour_joueur(int canal) {
    setlocale(LC_ALL, ""); // Pour gérer correctement les caractères accentués

    Category categories[] = {
        {"Prénom", isValidPrenom, "dicoprenom.txt"},
        {"Animal", isValidAnimal, "dicoanimal.txt"},
        {"Fruit ou légume", isValidFruit, "dicofruitlegume.txt"},
        {"Métier", isValidMetier, "dicometier.txt"},
        {"Pays ou capitale", isValidCity, "dicopaysville.txt"}
    };

    char words[MAX_WORDS][WORD_LENGTH];
    int local_score = 0;
    int valid;
    srand(time(NULL)); // Initialisation du générateur de nombres aléatoires

    ecrireLigne(canal, "Bienvenue au jeu du petit bac!\n");
    
    char buffer[LIGNE_MAX];    

    while (1) {
        char letter = generateLetter();
        ecrireLigne(canal, "\nNouvelle partie!\n");
        snprintf(buffer, sizeof(buffer),"La lettre est : %c\n", letter);
        ecrireLigne(canal, buffer);

        memset(words, 0, sizeof(words));

        while (1) {
            printMenu(canal);
            char choice;
            lireLigne(canal, buffer);
            sscanf(buffer, " %c", &choice);

            if (choice == 'q') {
                pthread_mutex_lock(&score_mutex);
                score += local_score;
                pthread_mutex_unlock(&score_mutex);
                snprintf(buffer, sizeof(buffer),"Merci d'avoir joué! Votre score est de : %d\n", local_score);
                ecrireLigne(canal, buffer);
                return;
            }

            if (choice < '1' || choice > '5') {
                ecrireLigne(canal, "Choix invalide, veuillez réessayer.\n");
                continue;
            }

            int categoryIndex = choice - '1';
            Category selectedCategory = categories[categoryIndex];

            char **dict = NULL;
            int dictSize = 0;
            if (!loadDictionary(selectedCategory.dict_file, &dict, &dictSize)) {
                snprintf(buffer, sizeof(buffer),"Erreur : impossible de charger le dictionnaire pour la catégorie %s\n", selectedCategory.name);
                ecrireLigne(canal, buffer);
                continue;
            }

            snprintf(buffer, sizeof(buffer),"Vous avez choisi la catégorie : %s\n", selectedCategory.name);
            ecrireLigne(canal, buffer);
            snprintf(buffer, sizeof(buffer),"Entrez un mot pour la catégorie %s commençant par la lettre %c :\n", selectedCategory.name, letter);
            ecrireLigne(canal, buffer);

            clock_t start_time = clock();
            while (1) {
                lireLigne(canal, buffer);
                sscanf(buffer, "%s", words[categoryIndex]);
                valid = selectedCategory.validate(words[categoryIndex], letter, dict, dictSize);
                if (valid) {
                    break;
                }
                snprintf(buffer, sizeof(buffer),"Le mot %s est invalide pour la catégorie %s. Veuillez réessayer :\n", words[categoryIndex], selectedCategory.name);
                ecrireLigne(canal, buffer);
            }
            clock_t end_time = clock();
            double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;

            if (valid) {
                snprintf(buffer, sizeof(buffer),"Le mot %s est valide pour la catégorie %s. Temps écoulé : %.2f secondes.\n", words[categoryIndex], selectedCategory.name, time_spent);
                ecrireLigne(canal, buffer);
                local_score++;

                // Vérifier si toutes les catégories ont été remplies
                int allCategoriesFilled = 1;
                for (int i = 0; i < MAX_WORDS; i++) {
                    if (words[i][0] == '\0') {
                        allCategoriesFilled = 0;
                        break;
                    }
                }

                if (allCategoriesFilled) {
                    pthread_mutex_lock(&score_mutex);
                    score += local_score;
                    pthread_mutex_unlock(&score_mutex);
                    snprintf(buffer, sizeof(buffer),"Félicitations, vous avez rempli toutes les catégories ! Votre score final est de : %d\n", local_score);
                    ecrireLigne(canal, buffer);
                    return;
                }
            }

            // Libérer la mémoire allouée pour le dictionnaire
            for (int i = 0; i < dictSize; i++) {
                free(dict[i]);
            }
            free(dict);
        }
    }
}

void jouer_petit_bac_2j(int canal1, int canal2) {
    pid_t pid1, pid2;
    
    if ((pid1 = fork()) == 0) {
        // Processus enfant pour le joueur 1
        jouer_petit_bac_pour_joueur(canal1);
        exit(EXIT_SUCCESS);
    }
    
    if ((pid2 = fork()) == 0) {
        // Processus enfant pour le joueur 2
        jouer_petit_bac_pour_joueur(canal2);
        exit(EXIT_SUCCESS);
    }

    // Processus parent attend que les deux enfants terminent
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}
