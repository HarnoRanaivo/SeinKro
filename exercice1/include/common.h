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

static inline infos_t * creer_infos(tampon_t * tampon, sem_t * semaphore_tampon, sem_t * semaphore_acteurs, findestemps * temps, unsigned int numero)
{
    infos_t * i = malloc(sizeof *i);

    if (i == NULL)
        pq_error("malloc", EX_OSERR);

    i->tampon = tampon;
    i->semaphore_tampon = semaphore_tampon;
    i->semaphore_acteurs = semaphore_acteurs;
    i->temps = temps;
    i->numero = numero;

    return i;
}

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

static inline tampon_t * creer_tampon()
{
    tampon_t * t = malloc(sizeof *t);
    if (t == NULL)
        pq_error("malloc", EX_OSERR);

    t->curseur = 0;
    t->suivant = 0;

    return t;
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
