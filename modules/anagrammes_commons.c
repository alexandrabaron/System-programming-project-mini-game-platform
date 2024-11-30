// anagrammes_commons.c
#include "pse.h"
#include "anagrammes_commons.h"

int compte_occurrences(char lettre, char lettres[], int taille) {
    int compt = 0;
    for (int i = 0; i < taille; i++) {
        if (lettres[i] == lettre) {
            compt++;
        }
    }
    return compt;
}

void generation_aleatoire_lettres(char lettres[7]) {
    const char voyelles[] = "aeieeeeeaaaaaoueeaaaaaeeeyaeiouaeioueeeiieeeeeeeeeeieee";
    const char consonnes[] = "mmmbcdfghjkllllllccccccccccmnpqrstvwrrrrpppppppppprsssssxzbcdfgjlmnprstvtttsssss";
    int index = 0;
    
    while (index < 3) {
        char lettre = voyelles[rand() % 55];
        if (compte_occurrences(lettre, lettres, index) < 2) {
            lettres[index++] = lettre;
        }
    }

    while (index < 6) {
        char lettre = consonnes[rand() % 80];
        if (compte_occurrences(lettre, lettres, index) < 2) {
            lettres[index++] = lettre;
        }
    }
    lettres[6] = '\0';
}

int verif_lettres_utilisees(const char *mot, const char *lettres) {
    int compt_lettres[26] = {0};
    for (int i = 0; i < 6; i++) {
        compt_lettres[lettres[i] - 'a']++;
    }

    for (int i = 0; mot[i] != '\0'; i++) {
        if (--compt_lettres[mot[i] - 'a'] < 0) {
            return 0;
        }
    }
    return 1;
}

int is_in_dico(const char *mot, const char **dico, size_t taille_dico) {
    for (size_t i = 0; i < taille_dico; i++) {
        if (strcmp(mot, dico[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int calcul_points(const char *mot) {
    size_t length = strlen(mot);
    switch (length) {
        case 3: return 100;
        case 4: return 400;
        case 5: return 1200;
        case 6: return 2000;
        default: return 0;
    }
}

