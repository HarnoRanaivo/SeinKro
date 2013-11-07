#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

#include "temps.h"

#define TAILLE_TAMPON 10
#define FIN_PRODUCTION -1
#define TAMPON_REMPLI -2
#define TAMPON_VIDE -3

typedef struct comptes_t
{
    unsigned int nombre;
    unsigned int somme;
} comptes_t;

typedef struct tampon_t
{
    int valeurs[TAILLE_TAMPON];
    int curseur;
    int suivant;
} tampon_t;

typedef struct infos_t
{
    tampon_t * tampon;
    sem_t * semaphore_tampon;
    sem_t * semaphore_acteurs;
    findestemps * temps;
    unsigned int numero;
} infos_t;

static inline void pq_error(const char * message, int exit_code)
{
    perror(message);
    exit(exit_code);
}

void masquer_signal(int signum);

void config_handler(int signum, void (* handler) (int));

int nombre_aleatoire(unsigned int * graine);

#endif /* __COMMON_H */
