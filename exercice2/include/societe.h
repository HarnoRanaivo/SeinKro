/**
 * \file societe.h
 * \brief Société de consommation (header)
 */
#ifndef __SOCIETE_H
#define __SOCIETE_H

#include "commun.h"
#include "limite.h"
#include "tampon.h"

/**
 * \brief Comptes.
 */
typedef struct comptes_t
{
    unsigned long long int nombre;  /**<- Nombre de valeurs traitées. */
    unsigned long long int somme;   /**<- Somme des valeurs traitées. */
} comptes_t;

/**
 * \brief Acteurs (consommateurs ou producteurs).
 */
typedef struct acteurs_t
{
    pthread_t * threads;    /**<- Threads des acteurs. */
    const int nombre;       /**<- Nombre d'acteurs. */
    sem_t * semaphore;      /**<- Sémaphore des acteurs. */
} acteurs_t;

/**
 * \brief Infos.
 */
typedef struct infos_t
{
    tampon_t * tampon;              /**<- Tampon utilisé. */
    sem_t * semaphore_acteurs;      /**<- Sémaphore. */
    limite_t * limite;              /**<- Limite. */
    unsigned int numero;            /**<- Numéro de l'acteur. */
} infos_t;

/**
 * \brief Créer les informations.
 * \param tampon Tampon à utiliser.
 * \param semaphore_acteurs Sémaphore concernant l'acteur.
 * \param temps Limite.
 * \param numero Numéro.
 * \return infos.
 */
infos_t * creer_infos(tampon_t * tampon, sem_t * semaphore_acteurs, limite_t * limite, unsigned int numero);

/**
 * \brief Créer comptes.
 * \return Comptes.
 */
comptes_t * creer_comptes();

/**
 * \brief Créer acteurs (producteurs ou consommateurs).
 * \param nombre Nombre d'acteurs.
 * \param fonction Fonction de l'acteur.
 * \param tampon Tampon à utiliser.
 * \param lim Limite à surveiller (ignorée par les consommateurs).
 * \return Acteurs créés.
 */
acteurs_t * creer_acteurs(int nombre, void * (*fonction) (void *), tampon_t * tampon, limite_t * lim);

/**
 * \brief Faire le bilan.
 * \param acteurs Acteurs (producteurs ou consommateurs).
 * \return Bilan des valeurs traitées (nombre et somme).
 */
comptes_t bilan (const acteurs_t * acteurs);

/**
 * \brief Détruire les acteurs.
 * \param acteurs Acteurs à détruire.
 * \return NULL
 *
 * Attention : bilan() doit avoir été éxécutée sur ces acteurs.
 */
acteurs_t * detruire_acteurs(acteurs_t * acteurs);
/**
 * \brief Fonction des producteurs.
 * \param arg Arguments passés au thread.
 */
void * production(void * arg);

/**
 * \brief Fonction des consommateurs.
 * \param arg Arguments passés au thread.
 */
void * consommation(void * arg);

#endif /* __SOCIETE_H */
