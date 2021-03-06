/**
 * \file commun.c
 * \brief Fonctions communes (code)
 * \author MEYER Jérémy, RAZANAJATO Harenome
 */
/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */
#include "commun.h"

void masquer_signal(int signum)
{
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, signum);
    int error = pthread_sigmask(SIG_SETMASK, &mask, NULL);

    if (error != 0)
        aq_erreur("pthread_sigmask", EX_OSERR);
}

void config_handler(int signum, void (*handler)(int))
{
    struct sigaction action;

    action.sa_handler = handler;
    sigfillset(&action.sa_mask);
    action.sa_flags = 0;
    int error = sigaction(signum, &action, NULL);

    if (error != 0)
        aq_erreur("sigaction", EX_OSERR);
}

sem_t * creer_semaphore(int valeur)
{
    sem_t * s = malloc(sizeof *s);
    if (s == NULL)
        aq_erreur("malloc", EX_OSERR);

    int erreur = sem_init(s, 0, valeur);
    if (erreur != 0)
        aq_erreur("sem_init", EX_OSERR);

    return s;
}

sem_t * detruire_semaphore(sem_t * semaphore)
{
    int erreur = sem_destroy(semaphore);
    if (erreur != 0)
        aq_erreur("sem_destroy", EX_OSERR);

    free(semaphore);
    return NULL;
}
