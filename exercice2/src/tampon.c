/**
 * \file tampon.c
 * \brief Tampon (code)
 * \author MEYER Jérémy, RAZANAJATO Harenome
 */
/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */
#include "tampon.h"

tampon_t * creer_tampon(int taille)
{
    tampon_t * tampon = malloc(sizeof *tampon);
    if (tampon == NULL)
        aq_erreur("malloc", EX_OSERR);

    if (taille <= 0)
        taille = TAILLE_TAMPON_DEFAUT;

    tampon->valeurs = malloc(taille * sizeof *tampon->valeurs);
    if (tampon->valeurs == NULL)
        aq_erreur("malloc", EX_OSERR);

    int * t = (int *) &tampon->taille;
    *t = taille;

    tampon->curseur = 0;
    tampon->suivant = 0;
    tampon->semaphore = creer_semaphore(1);

    return tampon;
}

tampon_t * detruire_tampon(tampon_t * tampon)
{
    detruire_semaphore(tampon->semaphore);
    free(tampon->valeurs);
    free(tampon);
    return NULL;
}

int lire_entier(tampon_t * tampon)
{
    monsem_wait(tampon->semaphore);

    int debut = tampon->suivant;
    int fin = tampon->curseur;
    int valeur = tampon->valeurs[debut];
    const int taille = tampon->taille;

    if (valeur != FIN_PRODUCTION)
    {
        if (debut == fin)
            valeur = TAMPON_VIDE;
        else
        {
            tampon->suivant = (debut + 1) % taille;
            tampon->curseur = (fin >= taille) ? (fin - taille) : fin;
        }
    }

    monsem_post(tampon->semaphore);

    return valeur;
}

int ecrire_entier(tampon_t * tampon, int valeur)
{
    monsem_wait(tampon->semaphore);

    int fin = tampon->curseur;
    int retour = 0;
    const int taille = tampon->taille;

    if (fin < taille)
    {
        int debut = tampon->suivant;
        int suivant = (fin + 1) % taille;

        tampon->valeurs[fin] = valeur;
        tampon->curseur = (suivant != debut) ? suivant : (suivant + taille);
    }
    else
        retour = TAMPON_REMPLI;

    monsem_post(tampon->semaphore);

    return retour;
}

void fin_production(tampon_t * tampon)
{
    monsem_wait(tampon->semaphore);
    tampon->valeurs[tampon->suivant] = FIN_PRODUCTION;
    monsem_post(tampon->semaphore);
}
