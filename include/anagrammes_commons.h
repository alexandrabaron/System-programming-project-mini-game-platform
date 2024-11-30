// anagrammes_commons.h
#ifndef ANAGRAMMES_COMMONS_H
#define ANAGRAMMES_COMMONS_H

void generation_aleatoire_lettres(char lettres[7]);
int verif_lettres_utilisees(const char *mot, const char *lettres);
int is_in_dico(const char *mot, const char **dico, size_t taille_dico);
int calcul_points(const char *mot);
int compte_occurrences(char lettre, char lettres[], int taille);

#endif

