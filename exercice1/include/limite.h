#ifndef __LIMITE_H
#define __LIMITE_H

#include "commun.h"

/**
 * \brief États de la limite.
 */
typedef enum etat_limite_t
{
    CONTINUER,      /**<- Limite OK, continuer. */
    FIN_DES_TEMPS,  /**<- Limite atteinte, Fin des temps. */
} etat_limite_t;

/**
 * \brief Limite.
 */
typedef struct limite_t
{
    etat_limite_t etat;     /**<- État de la limite. */
    sem_t * semaphore;      /**<- Sémaphore de la limite. */
} limite_t;

/**
 * \brief Créer une limite.
 * \return Limite créée.
 */
static inline limite_t * creer_limite()
{
    limite_t * limite = malloc(sizeof *limite);
    if (limite == NULL)
        aq_erreur("malloc", EX_OSERR);

    limite->semaphore = creer_semaphore(1);
    limite->etat = CONTINUER;
    return limite;
}

/**
 * \brief Tester si la limite est atteinte.
 * \param limite Limite à tester.
 */
static inline int limite_atteinte(limite_t * limite)
{
    sem_wait(limite->semaphore);
    etat_limite_t etat = limite->etat;
    sem_post(limite->semaphore);
    return etat == FIN_DES_TEMPS;
}

/**
 * \brief Limite atteinte.
 * \param limite Limite atteinte.
 */
static inline void fin_des_temps(limite_t * limite)
{
    sem_wait(limite->semaphore);
    limite->etat = FIN_DES_TEMPS;
    sem_post(limite->semaphore);
}

/**
 * \brief Détruire une limite.
 * \param limite Limite à détruire.
 * \return NULL
 */
static inline limite_t * detruire_limite(limite_t * limite)
{
    limite->semaphore = detruire_semaphore(limite->semaphore);
    free(limite);
    return NULL;
}

#endif /* __LIMITE_H */
