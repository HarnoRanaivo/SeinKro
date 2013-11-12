/**
 * \file tampon.h
 * \brief Tampon (header)
 * \author MEYER Jérémy, RAZANAJATO Harenome
 */
/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://www.wtfpl.net/ for more details.
 */
#ifndef __TAMPON_H
#define __TAMPON_H

#include "commun.h"

/**
 * \brief Taille d'un tampon.
 */
#define TAILLE_TAMPON_DEFAUT 10

/**
 * \brief Valeur de fin de production.
 */
#define FIN_PRODUCTION -1

/**
 * \brief Valeur de tampon rempli.
 */
#define TAMPON_REMPLI -2

/**
 * \brief Valeur de tampon vide.
 */
#define TAMPON_VIDE -3

/**
 * \brief Tampon.
 */
typedef struct tampon_t
{
    int curseur;                    /**<- Indice de la prochaine position où écrire. */
    int suivant;                    /**<- Indice de la prochaine position à lire. */
    sem_t * semaphore;              /**<- Sémaphore du tampon. */
    int * valeurs;                  /**<- Contenu du tampon. */
    const int taille;               /**<- Taille du tampon. */
} tampon_t;

/**
 * \brief Créer un tampon.
 * \return tampon
 */
tampon_t * creer_tampon(int taille);

/**
 * \brief Détruire un tampon.
 * \return NULL
 */
tampon_t * detruire_tampon(tampon_t * tampon);

/**
 * \brief Lire un entier dans le tampon.
 * \param tampon Tampon où lire un entier.
 * \return L'entier lu en cas de succès, #TAMPON_VIDE si le tampon est vide, #FIN_PRODUCTION si la production est finie.
 */
int lire_entier(tampon_t * tampon);

/**
 * \brief Écrire un entier dans le tampon.
 * \param tampon Tampon où écrire.
 * \param valeur Valeur à écrire.
 * \return 0 en cas de succès, #TAMPON_REMPLI si le tampon est rempli.
 */
int ecrire_entier(tampon_t * tampon, int valeur);

/**
 * \brief Terminer la production.
 * \param tampon Tampon où la production est terminée.
 */
void fin_production(tampon_t * tampon);

#endif /* __TAMPON_H */
