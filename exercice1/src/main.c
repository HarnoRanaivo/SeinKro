#include "common.h"
#include "temps.h"
#include "societe.h"

static findestemps temps;

static inline void creer_acteurs(pthread_t * acteurs, void * (*fonction) (void *), tampon_t * tampon, sem_t * semaphore_tampon, findestemps * temps, int nombre)
{
    sem_t * semaphore_acteurs = creer_semaphore(1);
    for (int i = 0 ; i < nombre; i++)
    {
        infos_t * infos = creer_infos(tampon, semaphore_tampon, semaphore_acteurs, temps, i);
        int erreur = pthread_create(&acteurs[i], NULL, fonction, (void *) infos);
        if (erreur != 0)
            pq_error("pthread_create", EX_OSERR);
    }
}

static inline comptes_t bilan (const pthread_t * acteurs, int nombre_acteurs)
{
    comptes_t resultat;
    resultat.nombre = 0;
    resultat.somme = 0;

    for (int i = 0; i < nombre_acteurs; i++)
    {
        comptes_t * c;
        pthread_join(acteurs[i], (void **) &c);
        resultat.nombre += c->nombre;
        resultat.somme += c->somme;
        free(c);
    }

    return resultat;
}

void handler_sigint(int signum)
{
    cest_la_fin_des_temps(&temps);
}

int main(int argc, char ** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <producteurs> <consommateurs>", argv[0]);
        exit(EX_USAGE);
    }

    int nombre_producteurs = atoi(argv[1]);
    int nombre_consommateurs = atoi(argv[2]);

    if (nombre_producteurs <= 0 || nombre_consommateurs < 0)
    {
        fprintf(stderr, "Usage: %s <producteurs> <consommateurs>\n"
                "producteurs > 0; consommateurs >= 0\n",
                argv[0]);
        exit(EX_DATAERR);
    }

    tampon_t * tampon = creer_tampon();
    sem_t * semaphore_tampon = creer_semaphore(1);

    config_handler(SIGINT, handler_sigint);

    /* Tableaux à taille variable, C99 powa. */
    pthread_t producteurs[nombre_producteurs];
    pthread_t consommateurs[nombre_consommateurs];

    init_fin_des_temps(&temps);

    creer_acteurs(producteurs, production, tampon, semaphore_tampon, &temps, nombre_producteurs);
    creer_acteurs(consommateurs, consommation, tampon, semaphore_tampon, &temps, nombre_consommateurs);

    comptes_t comptes_production = bilan(producteurs, nombre_producteurs);
    comptes_t comptes_consommation = bilan(consommateurs, nombre_consommateurs);

    printf("Bilan:\n\tNombre de valeurs produites : %d\n\tSomme des valeurs produites : %d\n\t"
            "Nombre de valeurs consommées : %d\n\tSomme des valeurs consommées : %d\n",
            comptes_production.nombre, comptes_production.somme,
            comptes_consommation.nombre, comptes_consommation.somme);

    free(tampon);
    sem_destroy(semaphore_tampon);
    free(semaphore_tampon);
    sem_destroy(&temps._semaphore_des_temps);

    return 0;
}
