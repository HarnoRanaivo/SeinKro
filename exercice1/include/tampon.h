#ifndef __TAMPON_H
#define __TAMPON_H

#include "common.h"
#include "temps.h"

#define TAILLE_TAMPON 10

#define FIN_PRODUCTION -1
#define TAMPON_REMPLI -2
#define TAMPON_VIDE -3

typedef struct tampon_t
{
    int curseur;
    int suivant;
    sem_t * occupes;
    sem_t * libres;
    sem_t * semaphore;
    int valeurs[TAILLE_TAMPON];
} tampon_t;

tampon_t * creer_tampon();

void detruire_tampon(tampon_t * tampon);
int ecrire_entier(tampon_t * tampon, int valeur);
void fin_production(tampon_t * tampon);
int lire_entier(tampon_t * tampon);

typedef struct infos_t
{
    tampon_t * tampon;
    sem_t * semaphore_acteurs;
    findestemps * temps;
    unsigned int numero;
} infos_t;

static inline infos_t * creer_infos(tampon_t * tampon, sem_t * semaphore_acteurs, findestemps * temps, unsigned int numero)
{
    infos_t * i = malloc(sizeof *i);

    if (i == NULL)
        pq_error("malloc", EX_OSERR);

    i->tampon = tampon;
    i->semaphore_acteurs = semaphore_acteurs;
    i->temps = temps;
    i->numero = numero;

    return i;
}

#endif /* __TAMPON_H */
