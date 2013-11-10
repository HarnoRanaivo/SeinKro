/**
 * \file monsem.c
 * \brief Sémaphores custom (code)
 */
#include "monsem.h"

int monsem_init(monsem_t * semaphore, int valeur)
{
    int retour = 0;

    if (valeur > MONSEM_VALEUR_MAX)
    {
        errno = EINVAL;
        retour = -1;
    }
    else
    {
        pthread_mutex_init(&semaphore->mutex, NULL);
        pthread_cond_init(&semaphore->condition, NULL);
        semaphore->valeur = valeur;
        semaphore->initialise = 1;
    }

    return retour;
}

int monsem_wait(monsem_t * semaphore)
{
    int retour;
    if (semaphore->initialise == 1)
    {
        /* pthread_mutex_{unlock, lock} écrivent déjà errno en cas
         * d'erreur : pas besoin d'y écrire.
         */
        retour = pthread_mutex_lock(&semaphore->mutex);
        if (retour == 0)
        {
            while (semaphore->valeur < 1)
                pthread_cond_wait(&semaphore->condition, &semaphore->mutex);
            semaphore->valeur--;
            pthread_mutex_unlock(&semaphore->mutex);
        }
    }
    else
    {
        errno = EINVAL;
        retour = -1;
    }

    return retour;
}

int monsem_post(monsem_t * semaphore)
{
    int retour;
    if (semaphore->initialise == 1)
    {
        retour = pthread_mutex_lock(&semaphore->mutex);
        if (retour == 0)
        {
            if (semaphore->valeur != MONSEM_VALEUR_MAX)
            {
                semaphore->valeur++;
                pthread_cond_signal(&semaphore->condition);
            }
            else
            {
                errno = EOVERFLOW;
                retour = -1;
            }
            pthread_mutex_unlock(&semaphore->mutex);
        }
    }
    else
    {
        errno = EINVAL;
        retour = -1;
    }

    return retour;
}

int monsem_getvalue(monsem_t * semaphore, int * valeur)
{
    int retour;
    if (semaphore->initialise == 1)
    {
        retour = pthread_mutex_lock(&semaphore->mutex);
        if (retour == 0)
        {
            *valeur = semaphore->valeur;
            pthread_mutex_unlock(&semaphore->mutex);
        }
    }
    else
    {
        errno = EINVAL;
        retour = -1;
    }

    return retour;
}

int monsem_destroy(monsem_t * semaphore)
{
    int retour;
    if (semaphore->initialise == 1)
    {
        retour = pthread_cond_destroy(&semaphore->condition);
        if (retour == 0)
            retour = pthread_mutex_destroy(&semaphore->mutex);
    }
    else
    {
        errno = EINVAL;
        retour = -1;
    }

    return retour;
}
