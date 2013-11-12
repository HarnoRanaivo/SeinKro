/**
 * \file limite.c
 * \brief Limite (code)
 * \author MEYER Jérémy, RAZANAJATO Harenome
 */
/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */
#include "limite.h"

limite_t * creer_limite()
{
    limite_t * limite = malloc(sizeof *limite);
    if (limite == NULL)
        aq_erreur("malloc", EX_OSERR);

    limite->semaphore = creer_semaphore(1);
    limite->etat = CONTINUER;
    return limite;
}

int limite_atteinte(limite_t * limite)
{
    monsem_wait(limite->semaphore);
    etat_limite_t etat = limite->etat;
    monsem_post(limite->semaphore);
    return etat == FIN_DES_TEMPS;
}

void fin_des_temps(limite_t * limite)
{
    monsem_wait(limite->semaphore);
    limite->etat = FIN_DES_TEMPS;
    monsem_post(limite->semaphore);
}

limite_t * detruire_limite(limite_t * limite)
{
    limite->semaphore = detruire_semaphore(limite->semaphore);
    free(limite);
    return NULL;
}
