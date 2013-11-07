#include "societe.h"

void * production(void * arg)
{
    infos_t * i = (infos_t *) arg;
    comptes_t * comptes = creer_comptes();

    while (! est_ce_la_fin_des_temps(i->temps))
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
