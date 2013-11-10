/**
 * \file commun.h
 * \brief Fonctions communes (header)
 */
#ifndef __COMMUN_H
#define __COMMUN_H

#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>

#include "monsem.h"

/**
 * \brief Afficher un message d'erreur et quitter.
 * \param message Message d'erreur.
 * \param exit_code Code d'erreur.
 *
 * aq_erreur : a(fficher et)q(uitter)_erreur.
 */
static inline void aq_erreur(const char * message, int exit_code)
{
    perror(message);
    exit(exit_code);
}

/**
 * \brief Obtenir un nombre aléatoire.
 * \param graine Graine pour l'aléatoire.
 * \return Entier aléatoire appartenant à l'intervalle [0, 256[.
 */
static inline int nombre_aleatoire(unsigned int * graine)
{
    return ((double) rand_r(graine) / RAND_MAX) * 255;
}

/**
 * \brief Masquer un signal.
 * \param signum Signal à masquer.
 */
void masquer_signal(int signum);

/**
 * \brief Associer un handler à un signal.
 * \param signum Signal.
 * \param handler Handler.
 */
void config_handler(int signum, void (* handler) (int));

/**
 * \brief Créer un semaphore partagé entre threads.
 * \param valeur Valeur initiale du sémaphore.
 * \return Sémaphore créé.
 */
monsem_t * creer_semaphore(int valeur);

/**
 * \brief Détruire un sémaphore.
 * \return NULL
 */
monsem_t * detruire_semaphore(monsem_t * semaphore);

#endif /* __COMMUN_H */
