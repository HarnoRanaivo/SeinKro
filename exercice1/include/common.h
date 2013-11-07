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

sem_t * creer_semaphore(int valeur);

void detruire_semaphore(sem_t * semaphore);

comptes_t * creer_comptes();

static inline int nombre_aleatoire(unsigned int * graine)
{
    return ((double) rand_r(graine) / RAND_MAX) * 255;
}

#endif /* __COMMON_H */
