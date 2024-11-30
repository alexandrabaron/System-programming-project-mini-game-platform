// client.c
#include "pse.h"
#include <pthread.h>

#define MAGENTA "\033[1;35m"
#define RESET "\033[0m"

#define CMD "client"

void *lireMessages(void *arg) {
    int sock = *((int *)arg);
    char buffer[LIGNE_MAX];
    while (1) {
        int ret = lireLigne(sock, buffer);
        if (ret > 0) {
            printf("%s\n", buffer);
        } else if (ret == LIGNE_EOF) {
            printf("Connection closed by the server.\n");
            break;
        } else {
            perror("lireLigne");
            break;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int sock, ret;
    struct sockaddr_in *adrServ;
    int fin = FAUX;
    char ligne[LIGNE_MAX];
    int lgEcr;
    pthread_t threadId;

    signal(SIGPIPE, SIG_IGN);

    if (argc != 3)
        erreur("usage: %s machine port\n", argv[0]);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        erreur_IO("socket");

    printf("%s: DNS resolving for %s, port %s\n", CMD, argv[1], argv[2]);
    adrServ = resolv(argv[1], argv[2]);
    if (adrServ == NULL)
        erreur("adresse %s port %s inconnus\n", argv[1], argv[2]);

    ret = connect(sock, (struct sockaddr *)adrServ, sizeof(struct sockaddr_in));
    if (ret < 0)
        erreur_IO("connect");

    printf(MAGENTA "-------------Mini-jeux-------------\n" RESET);

    // Create a thread to read messages from the server
    ret = pthread_create(&threadId, NULL, lireMessages, &sock);
    if (ret != 0) {
        erreur_IO("pthread_create");
    }

    while (!fin) {
        if (fgets(ligne, LIGNE_MAX, stdin) == NULL)
            fin = VRAI;
        else {
            lgEcr = ecrireLigne(sock, ligne);
            if (lgEcr == -1)
                erreur_IO("ecriture socket");

            if (strcmp(ligne, "fin\n") == 0)
                fin = VRAI;
        }
    }

    if (close(sock) == -1)
        erreur_IO("fermeture socket");

    // Wait for the thread to finish
    pthread_join(threadId, NULL);

    exit(EXIT_SUCCESS);
}

