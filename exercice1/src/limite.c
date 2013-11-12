/**
 * \file limite.h
 * \brief Limite (code)
 */
#include "limite.h"

limite_t * creer_limite()
{
    limite_t * limite = malloc(sizeof *limite);
    if (limite == NULL)
        aq_erreur("malloc", EX_OSERR);

    limite->semaphore = creer_semaphore(1);
    limite->etat = CONTINUER;
    return limite;
}

int limite_atteinte(limite_t * limite)
{
    sem_wait(limite->semaphore);
    etat_limite_t etat = limite->etat;
    sem_post(limite->semaphore);
    return etat == FIN_DES_TEMPS;
}

void fin_des_temps(limite_t * limite)
{
    sem_wait(limite->semaphore);
    limite->etat = FIN_DES_TEMPS;
    sem_post(limite->semaphore);
}

limite_t * detruire_limite(limite_t * limite)
{
    limite->semaphore = detruire_semaphore(limite->semaphore);
    free(limite);
    return NULL;
}
