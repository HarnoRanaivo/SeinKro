/**
 * \file societe.c
 * \brief Société de consommation (code)
 */
#include "societe.h"

infos_t * creer_infos(tampon_t * tampon, sem_t * semaphore_acteurs, limite_t * limite, unsigned int numero)
{
    infos_t * i = malloc(sizeof *i);

    if (i == NULL)
        aq_erreur("malloc", EX_OSERR);

    i->tampon = tampon;
    i->semaphore_acteurs = semaphore_acteurs;
    i->limite = limite;
    i->numero = numero;

    return i;
}

acteurs_t * creer_acteurs(int nombre, void * (*fonction) (void *), tampon_t * tampon, limite_t * lim)
{
    acteurs_t * acteurs = malloc(sizeof *acteurs);
    if (acteurs == NULL)
        aq_erreur("malloc", EX_OSERR);

    acteurs->threads = malloc(nombre * sizeof *acteurs->threads);
    if (acteurs->threads == NULL)
        aq_erreur("malloc", EX_OSERR);

    acteurs->semaphore = creer_semaphore(1);

    /** acteurs>nombre est un const int pour éviter qu'on ne le modifie à
     * tout-va... */
    int * n = (int *) &acteurs->nombre;
    *n = nombre;
    for (int i = 0 ; i < nombre; i++)
    {
        infos_t * infos = creer_infos(tampon, acteurs->semaphore, lim, i);
        int erreur = pthread_create(&acteurs->threads[i], NULL, fonction, (void *) infos);
        if (erreur != 0)
            aq_erreur("pthread_create", EX_OSERR);
    }

    return acteurs;
}

acteurs_t * detruire_acteurs(acteurs_t * acteurs)
{
    acteurs->semaphore = detruire_semaphore(acteurs->semaphore);
    free(acteurs->threads);
    free(acteurs);
    return NULL;
}

comptes_t bilan (const acteurs_t * acteurs)
{
    comptes_t resultat;
    resultat.nombre = 0;
    resultat.somme = 0;

    for (int i = 0; i < acteurs->nombre; i++)
    {
        comptes_t * c;
        pthread_join(acteurs->threads[i], (void **) &c);
        resultat.nombre += c->nombre;
        resultat.somme += c->somme;
        free(c);
    }

    return resultat;
}

comptes_t * creer_comptes()
{
    comptes_t * c = malloc(sizeof *c);
    if (c == NULL)
        aq_erreur("malloc", EX_OSERR);

    c->nombre = 0;
    c->somme = 0;

    return c;
}

void * production(void * arg)
{
    infos_t * i = (infos_t *) arg;
    comptes_t * comptes = creer_comptes();

    while (! limite_atteinte(i->limite))
    {
        sem_wait(i->semaphore_acteurs);
        int valeur = nombre_aleatoire(&i->numero);
        int retour = ecrire_entier(i->tampon, valeur);
        if (retour != TAMPON_REMPLI)
        {
            comptes->nombre++;
            comptes->somme += valeur;
        }
        sem_post(i->semaphore_acteurs);
    }
    fin_production(i->tampon);

    free(arg);

    pthread_exit(comptes);
}

void * consommation(void * arg)
{
    infos_t * i = (infos_t *) arg;
    comptes_t * comptes = creer_comptes();
    int lecture = 0;

    while (lecture != FIN_PRODUCTION)
    {
        sem_wait(i->semaphore_acteurs);
        lecture = lire_entier(i->tampon);
        if (lecture != TAMPON_VIDE && lecture != FIN_PRODUCTION)
        {
            comptes->nombre++;
            comptes->somme += lecture;
        }
        sem_post(i->semaphore_acteurs);
    }
    free(arg);

    pthread_exit(comptes);
}
