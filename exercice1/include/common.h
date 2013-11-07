#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

#include "temps.h"


typedef struct comptes_t
{
    unsigned long long int nombre;
    unsigned long long int somme;
} comptes_t;

typedef struct acteurs_t
{
    pthread_t * acteurs;
    const int nombre;
    sem_t * semaphore;
} acteurs_t;

static inline void pq_error(const char * message, int exit_code)
{
    perror(message);
    exit(exit_code);
}

void masquer_signal(int signum);

void config_handler(int signum, void (* handler) (int));

int nombre_aleatoire(unsigned int * graine);

static inline sem_t * creer_semaphore(int valeur)
{
    sem_t * s = malloc(sizeof *s);
    if (s == NULL)
        pq_error("malloc", EX_OSERR);

    int erreur = sem_init(s, 0, valeur);
    if (erreur != 0)
        pq_error("sem_init", EX_OSERR);

    return s;
}

static inline void detruire_semaphore(sem_t * semaphore)
{
    int erreur = sem_destroy(semaphore);

    if (erreur != 0)
        pq_error("sem_destroy", EX_OSERR);

    free(semaphore);
}

static inline comptes_t * creer_comptes()
{
    comptes_t * c = malloc(sizeof *c);
    if (c == NULL)
        pq_error("malloc", EX_OSERR);

    c->nombre = 0;
    c->somme = 0;

    return c;
}

#endif /* __COMMON_H */
