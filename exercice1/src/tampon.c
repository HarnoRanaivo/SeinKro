/**
 * \file tampon.c
 * \brief Tampon (code)
 */
#include "tampon.h"

tampon_t * creer_tampon()
{
    tampon_t * t = malloc(sizeof *t);
    if (t == NULL)
        aq_erreur("malloc", EX_OSERR);

    t->curseur = 0;
    t->suivant = 0;
    t->semaphore = creer_semaphore(1);

    return t;
}

tampon_t * detruire_tampon(tampon_t * tampon)
{
    detruire_semaphore(tampon->semaphore);
    free(tampon);
    return NULL;
}

int lire_entier(tampon_t * tampon)
{
    sem_wait(tampon->semaphore);

    int debut = tampon->suivant;
    int fin = tampon->curseur;
    int valeur = tampon->valeurs[debut];

    if (valeur != FIN_PRODUCTION)
    {
        if (debut == fin)
            valeur = TAMPON_VIDE;
        else
        {
            tampon->suivant = (debut + 1) % TAILLE_TAMPON;
            tampon->curseur = (fin >= TAILLE_TAMPON) ? (fin - TAILLE_TAMPON) : fin;
        }
    }

    sem_post(tampon->semaphore);

    return valeur;
}

int ecrire_entier(tampon_t * tampon, int valeur)
{
    sem_wait(tampon->semaphore);

    int fin = tampon->curseur;
    int retour = 0;

    if (fin < TAILLE_TAMPON)
    {
        int debut = tampon->suivant;
        int suivant = (fin + 1) % TAILLE_TAMPON;

        tampon->valeurs[fin] = valeur;
        tampon->curseur = (suivant != debut) ? suivant : (suivant + TAILLE_TAMPON);
    }
    else
        retour = TAMPON_REMPLI;

    sem_post(tampon->semaphore);

    return retour;
}

void fin_production(tampon_t * tampon)
{
    sem_wait(tampon->semaphore);
    tampon->valeurs[tampon->suivant] = FIN_PRODUCTION;
    sem_post(tampon->semaphore);
}
