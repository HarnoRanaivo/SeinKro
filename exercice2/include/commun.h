/**
 * \file commun.h
 * \brief Fonctions communes (header)
 * \author MEYER Jérémy, RAZANAJATO Harenome
 */
/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
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
