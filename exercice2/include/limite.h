/**
 * \file limite.h
 * \brief Limite (header)
 */
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
    monsem_t * semaphore;      /**<- Sémaphore de la limite. */
} limite_t;

/**
 * \brief Créer une limite.
 * \return Limite créée.
 */
limite_t * creer_limite();

/**
 * \brief Tester si la limite est atteinte.
 * \param limite Limite à tester.
 */
int limite_atteinte(limite_t * limite);

/**
 * \brief Limite atteinte.
 * \param limite Limite atteinte.
 */
void fin_des_temps(limite_t * limite);

/**
 * \brief Détruire une limite.
 * \param limite Limite à détruire.
 * \return NULL
 */
limite_t * detruire_limite(limite_t * limite);

#endif /* __LIMITE_H */
