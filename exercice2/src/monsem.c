/**
 * \file monsem.c
 * \brief Sémaphores custom (code)
 */
#include "monsem.h"

int monsem_init(monsem_t * semaphore, int valeur)
{
    pthread_mutex_init(&semaphore->mutex, NULL);
    pthread_cond_init(&semaphore->condition, NULL);
    semaphore->valeur = valeur;

    return 0;
}

int monsem_wait(monsem_t * semaphore)
{
    pthread_mutex_lock(&semaphore->mutex);
    while (semaphore->valeur < 1)
        pthread_cond_wait(&semaphore->condition, &semaphore->mutex);
    semaphore->valeur--;
    pthread_mutex_unlock(&semaphore->mutex);

    return 0;
}

int monsem_post(monsem_t * semaphore)
{
    pthread_mutex_lock(&semaphore->mutex);
    semaphore->valeur++;
    pthread_cond_signal(&semaphore->condition);
    pthread_mutex_unlock(&semaphore->mutex);

    return 0;
}

int monsem_getvalue(monsem_t * semaphore, int * valeur)
{
    pthread_mutex_lock(&semaphore->mutex);
    *valeur = semaphore->valeur;
    pthread_mutex_unlock(&semaphore->mutex);

    return 0;
}
