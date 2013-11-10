/**
 * \file monsem.h
 * \brief Sémaphores custom (header)
 */
#ifndef __MONSEM_H
#define __MONSEM_H

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <limits.h>

/**
 * \brief Valeur maximale pour un sémaphore.
 */
static const int MONSEM_VALEUR_MAX = INT_MAX;

/**
 * \brief Sémaphore
 */
typedef struct monsem_t
{
    pthread_cond_t condition;   /**<- Condition. */
    pthread_mutex_t mutex;      /**<- Mutex. */
    int valeur;                 /**<- Valeur du sémaphore. */
    int initialise;             /**<- État du sémpahore. */
} monsem_t;

/**
 * \brief Initaliseur pour un sémaphore binaire.
 */
static const struct monsem_t MONSEM_INITIALISEUR_BINAIRE =
{ PTHREAD_COND_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, 1, 1 };

/**
 * \brief Initaliseur pour un sémaphore bloquant.
 */
static const struct monsem_t MONSEM_INITIALISEUR_BLOQUANT =
{ PTHREAD_COND_INITIALIZER, PTHREAD_MUTEX_INITIALIZER, 0, 1 };

/*
 * Erreurs plus ou moins calquées sur les erreurs de sem_init, sem_wait,
 * sem_post, sem_getvalue et sem_destroy.
 */

/**
 * \brief Initialiser un sémaphore.
 * \param semaphore Sémaphore à initialiser.
 * \param valeur Valeur du sémaphore.
 * \return Retourne 0 en cas de succès, retourne une valeur non nulle et écrit errno en cas d'erreur.
 *
 * Erreur :
 * - EINVAL valeur est plus grande que MONSEM_VALEUR_MAX
 */
int monsem_init(monsem_t * semaphore, int valeur);

/**
 * \brief Vérouiller le sémaphore.
 * \param semaphore Sémaphore.
 * \return Retourne 0 en cas de succès, retourne une valeur non nulle et écrit errno en cas d'erreur.
 *
 * Erreur :
 * - EINVAL semaphore n'est pas un sémaphore valide.
 */
int monsem_wait(monsem_t * semaphore);

/**
 * \brief Dévérouiller le sémaphore.
 * \param semaphore Sémaphore.
 * \return Retourne 0 en cas de succès, retourne une valeur non nulle et écrit errno en cas d'erreur.
 *
 * Erreurs :
 * - EINVAL semaphore n'est pas un sémaphore valide.
 * - EOVERFLOW La valeur maximale autorisée pour un sémaphore serait dépassée.
 */
int monsem_post(monsem_t * semaphore);

/**
 * \brief Obtenir la valeur d'un sémaphore.
 * \param semaphore Sémaphore.
 * \param valeur Pointeur.
 * \return Retourne 0 en cas de succès, retourne une valeur non nulle et écrit errno en cas d'erreur.
 *
 * Erreur :
 * - EINVAL semaphore n'est pas un sémaphore valide.
 */
int monsem_getvalue(monsem_t * semaphore, int * valeur);

/**
 * \brief Détruire un sémaphore.
 * \param semaphore Sémaphore.
 * \return Retoure 0 en cas de sucès, retourne une valeur non nulle et écrit errno en cas d'erreur.
 *
 * Erreurs :
 * - EINVAL semaphore n'est pas un sémaphore valide.
 * - EBUSY semaphore est en cours d'utilisation.
 */
int monsem_destroy(monsem_t * semaphore);

#endif
