/**
 * \file monsem.h
 * \brief Sémaphores custom (header)
 */
#ifndef __MONSEM_H
#define __MONSEM_H

#include "stdlib.h"
#include "stdio.h"
#include "pthread.h"

typedef struct monsem_t
{
    int dummy;
} monsem_t;

/**
 * \brief Initialiser un sémaphore.
 * \param semaphore Sémaphore à initialiser.
 * \param valeur Valeur du sémaphore.
 */
int monsem_init(monsem_t * semaphore, int valeur);

/**
 * \brief P
 * \param semaphore Sémaphore.
 */
int monsem_wait(monsem_t * semaphore);

/**
 * \brief V
 * \param semaphore Sémaphore.
 */
int monsem_post(monsem_t * semaphore);

/**
 * \brief Obtenir la valeur d'un sémaphore.
 * \param semaphore Sémaphore.
 * \param valeur Pointeur.
 */
int monsem_getvalue(monsem_t * semaphore, int * valeur);

#endif
