#include "common.h"
#include "temps.h"
#include "societe.h"
#include "tampon.h"

static findestemps temps;

static inline void creer_acteurs(acteurs_t * acteurs, void * (*fonction) (void *), tampon_t * tampon, findestemps * temps)
{
    acteurs->semaphore = creer_semaphore(1);
    for (int i = 0 ; i < acteurs->nombre; i++)
    {
        infos_t * infos = creer_infos(tampon, acteurs->semaphore, temps, i);
        int erreur = pthread_create(&acteurs->acteurs[i], NULL, fonction, (void *) infos);
        if (erreur != 0)
            pq_error("pthread_create", EX_OSERR);
    }
}

static inline comptes_t bilan (const acteurs_t * acteurs)
{
    comptes_t resultat;
    resultat.nombre = 0;
    resultat.somme = 0;

    for (int i = 0; i < acteurs->nombre; i++)
    {
        comptes_t * c;
        pthread_join(acteurs->acteurs[i], (void **) &c);
        resultat.nombre += c->nombre;
        resultat.somme += c->somme;
        free(c);
    }

    return resultat;
}

void handler_sigint(int signum)
{
    printf("\n");
    cest_la_fin_des_temps(&temps);
}

int main(int argc, char ** argv)
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <producteurs> <consommateurs>", argv[0]);
        exit(EX_USAGE);
    }

    const int nombre_producteurs = atoi(argv[1]);
    const int nombre_consommateurs = atoi(argv[2]);

    if (nombre_producteurs <= 0 || nombre_consommateurs < 0)
    {
        fprintf(stderr, "Usage: %s <producteurs> <consommateurs>\n"
                "producteurs > 0; consommateurs >= 0\n",
                argv[0]);
        exit(EX_DATAERR);
    }

    tampon_t * tampon = creer_tampon();

    config_handler(SIGINT, handler_sigint);

    /* Tableaux à taille variable, C99 powa. */
    pthread_t th_producteurs[nombre_producteurs];
    pthread_t th_consommateurs[nombre_consommateurs];
    acteurs_t producteurs = { th_producteurs, nombre_producteurs, NULL };
    acteurs_t consommateurs = { th_consommateurs, nombre_consommateurs, NULL };

    init_fin_des_temps(&temps);

    creer_acteurs(&producteurs, production, tampon, &temps);
    creer_acteurs(&consommateurs, consommation, tampon, &temps);

    comptes_t comptes_production = bilan(&producteurs);
    comptes_t comptes_consommation = bilan(&consommateurs);

    printf("Bilan:\n\tNombre de valeurs produites : %llu\n\tSomme des valeurs produites : %llu\n\t"
            "Nombre de valeurs consommées : %llu\n\tSomme des valeurs consommées : %llu\n",
            comptes_production.nombre, comptes_production.somme,
            comptes_consommation.nombre, comptes_consommation.somme);

    detruire_tampon(tampon);
    sem_destroy(consommateurs.semaphore);
    free(consommateurs.semaphore);
    sem_destroy(producteurs.semaphore);
    free(producteurs.semaphore);
    sem_destroy(&temps._semaphore_des_temps);

    return 0;
}
