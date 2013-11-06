#ifndef __COMMON_H
#define __COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <pthread.h>
#include <signal.h>
#include <semaphore.h>

#define TAILLE_TAMPON 10
#define TAMPON_REMPLI -2
#define FIN_PRODUCTION -1

/* 1 : fin, les producteurs enverront -1. */
int fin_des_temps = 0;

static inline int est_ce_la_fin_des_temps()
{
    return fin_des_temps == 1;
}

static inline void cest_la_fin_des_temps()
{
    fin_des_temps = 1;
}

typedef unsigned int nat;

typedef struct comptes_t
{
    nat nombre;
    nat somme;
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
    sem_t * semaphore;
    int * numero;
} infos_t;

static inline void pq_error(const char * message, int exit_code)
{
    perror(message);
    exit(exit_code);
}

void masquer_signal(int signum);

void config_handler(int signum, void (*handler)(int));

int nombre_aleatoire(int * graine);

#endif /* __COMMON_H */
