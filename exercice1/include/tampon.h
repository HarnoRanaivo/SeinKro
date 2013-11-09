/**
 * \file tampon.h
 * \brief Tampon (header)
 */
#ifndef __TAMPON_H
#define __TAMPON_H

#include "commun.h"

/**
 * \brief Taille d'un tampon.
 */
#define TAILLE_TAMPON 10

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
    int valeurs[TAILLE_TAMPON];     /**<- Contenu du tampon. */
} tampon_t;

/**
 * \brief Créer un tampon.
 * \return tampon
 */
tampon_t * creer_tampon();

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
