/**
 * \file monsem_test.c
 * \brief Test trivial de monsem.[ch]
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "monsem.h"

#ifdef _TEST_MONSEM_BLOQUANT
    #define _TEST_MONSEM
#elif _TEST_MONSEM_BINAIRE
    #define _TEST_MONSEM
#else
    #undef _TEST_MONSEM
#endif

static const int NOMBRE_THREADS = 10;
static const int NOMBRE_INCREMENTATIONS = 100000;

int valeur = 0;
monsem_t semaphore;

void * f (void * arg)
{
    printf("Entrée dans f.\n");
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
    #ifdef _TEST_MONSEM_BINAIRE
        /* monsem_init(&semaphore, 1); */
        semaphore = MONSEM_INITIALISEUR_BINAIRE;
    #elif _TEST_MONSEM_BLOQUANT
        semaphore = MONSEM_INITIALISEUR_BLOQUANT;
    #endif

    pthread_t threads[NOMBRE_THREADS];

    for (int i = 0; i < NOMBRE_THREADS; i++)
        pthread_create(&threads[i], NULL, f, NULL);

    #ifdef _TEST_MONSEM_BLOQUANT
        sleep(5);
        printf("Réveil.\n");
        monsem_post(&semaphore);
    #endif


    for (int i = 0; i < NOMBRE_THREADS; i++)
        pthread_join(threads[i], NULL);

    #ifdef _TEST_MONSEM_BINAIRE
        printf("\nTest avec monsem_t, sémaphore binaire.\n");
    #elif _TEST_MONSEM_BLOQUANT
        printf("\nTest avec monsem_t, sémaphore bloquant.\n");
    #else
        printf("\nTest sans monsem_t.\n");
    #endif

    printf("\tValeur attendue : %d\n\tValeur obtenue : %d\n",
            NOMBRE_THREADS * NOMBRE_INCREMENTATIONS, valeur);

    monsem_destroy(&semaphore);

    return 0;
}
