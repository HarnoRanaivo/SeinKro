/**
 * \file monsem_test.c
 * \brief Test trivial de monsem.[ch]
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "monsem.h"

static const int NOMBRE_THREADS = 10;
static const int NOMBRE_INCREMENTATIONS = 100000;

int valeur = 0;
monsem_t semaphore;

void * f (void * arg)
{
    for (int i = 0; i < NOMBRE_INCREMENTATIONS; i++)
    {
        #ifdef _TEST_MONSEM
        monsem_wait(&semaphore);
        #endif

        valeur++;

        #ifdef _TEST_MONSEM
        monsem_post(&semaphore);
        #endif
    }

    pthread_exit(NULL);
}

int main(int argc, char ** argv)
{
    monsem_init(&semaphore, 1);
    pthread_t threads[NOMBRE_THREADS];

    for (int i = 0; i < NOMBRE_THREADS; i++)
        pthread_create(&threads[i], NULL, f, NULL);

    for (int i = 0; i < NOMBRE_THREADS; i++)
        pthread_join(threads[i], NULL);

    #ifdef _TEST_MONSEM
        printf("Test avec monsem_t.\n");
    #else
        printf("Test sans monsem_t.\n");
    #endif

    printf("\tValeur attendue : %d\n\tValeur obtenue : %d\n",
            NOMBRE_THREADS * NOMBRE_INCREMENTATIONS, valeur);

    monsem_destroy(&semaphore);

    return 0;
}
