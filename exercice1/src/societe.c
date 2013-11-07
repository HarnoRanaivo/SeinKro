#include "societe.h"

static int ecrire_entier(tampon_t * tampon, unsigned int * numero, comptes_t * comptes, findestemps * temps)
{
    int debut = tampon->suivant;
    int fin = tampon->curseur;
    int * tab = tampon->valeurs;

    if (est_ce_la_fin_des_temps(temps))
    {
        tab[debut] = FIN_PRODUCTION;
        return FIN_PRODUCTION;
    }

    if (fin >= TAILLE_TAMPON)
        return TAMPON_REMPLI;

    int valeur = nombre_aleatoire(numero);
    tab[fin] = valeur;
    int suivant = (fin + 1) % TAILLE_TAMPON;
    tampon->curseur = (suivant != debut) ? suivant : (suivant + TAILLE_TAMPON);
    comptes->nombre++;
    comptes->somme += valeur;

    return 0;
}

static int lire_entier(tampon_t * tampon, comptes_t * comptes)
{
    int debut = tampon->suivant;
    int fin = tampon->curseur;
    int * tab = tampon->valeurs;

    int entier = tab[debut];

    if (entier == FIN_PRODUCTION)
        return FIN_PRODUCTION;

    if (debut == fin)
        return TAMPON_VIDE;

    tampon->suivant = (debut + 1) % TAILLE_TAMPON;
    tampon->curseur = (fin >= TAILLE_TAMPON) ? (fin - TAILLE_TAMPON) : fin;
    comptes->nombre++;
    comptes->somme += entier;

    return entier;
}

void * production(void * arg)
{
    infos_t * i = (infos_t *) arg;
    comptes_t * c = creer_comptes();
    int fin = 0;

    while (fin != FIN_PRODUCTION)
    {
        sem_wait(i->tampon->semaphore);
        sem_wait(i->semaphore_acteurs);
        fin = ecrire_entier(i->tampon, &i->numero, c, i->temps);
        sem_post(i->semaphore_acteurs);
        sem_post(i->tampon->semaphore);
    }

    free(arg);

    pthread_exit(c);
}

void * consommation(void * arg)
{
    infos_t * i = (infos_t *) arg;
    comptes_t * c = creer_comptes();
    int lecture = 0;

    while (lecture != FIN_PRODUCTION)
    {
        sem_wait(i->tampon->semaphore);
        sem_wait(i->semaphore_acteurs);
        lecture = lire_entier(i->tampon, c);
        sem_post(i->semaphore_acteurs);
        sem_post(i->tampon->semaphore);
    }
    free(arg);

    pthread_exit(c);
}
