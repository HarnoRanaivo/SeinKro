#include "tampon.h"

tampon_t * creer_tampon()
{
    tampon_t * t = malloc(sizeof *t);
    if (t == NULL)
        pq_error("malloc", EX_OSERR);

    t->curseur = 0;
    t->suivant = 0;
    t->semaphore = creer_semaphore(1);

    return t;
}

void detruire_tampon(tampon_t * tampon)
{
    detruire_semaphore(tampon->semaphore);
    free(tampon);
}

void fin_production(tampon_t * tampon)
{
    sem_wait(tampon->semaphore);
    tampon->valeurs[tampon->suivant] = FIN_PRODUCTION;
    sem_post(tampon->semaphore);
}

int ecrire_entier(tampon_t * tampon, int valeur)
{
    sem_wait(tampon->semaphore);

    int fin = tampon->curseur;

    if (fin >= TAILLE_TAMPON)
    {
        sem_post(tampon->semaphore);
        return TAMPON_REMPLI;
    }

    int debut = tampon->suivant;

    tampon->valeurs[fin] = valeur;
    int suivant = (fin + 1) % TAILLE_TAMPON;
    tampon->curseur = (suivant != debut) ? suivant : (suivant + TAILLE_TAMPON);

    sem_post(tampon->semaphore);

    return 0;
}

int lire_entier(tampon_t * tampon)
{
    sem_wait(tampon->semaphore);

    int debut = tampon->suivant;
    int valeur = tampon->valeurs[debut];

    if (valeur == FIN_PRODUCTION)
    {
        sem_post(tampon->semaphore);
        return FIN_PRODUCTION;
    }

    int fin = tampon->curseur;

    if (debut == fin)
    {
        sem_post(tampon->semaphore);
        return TAMPON_VIDE;
    }

    tampon->suivant = (debut + 1) % TAILLE_TAMPON;
    tampon->curseur = (fin >= TAILLE_TAMPON) ? (fin - TAILLE_TAMPON) : fin;

    sem_post(tampon->semaphore);

    return valeur;
}
