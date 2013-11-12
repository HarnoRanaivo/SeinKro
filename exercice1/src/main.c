/**
 * \file main.c
 * \brief Main
 */
#include "commun.h"
#include "limite.h"
#include "societe.h"
#include "tampon.h"

static limite_t * limite;

static void handler_sigint(int signum)
{
    printf("\n");
    fin_des_temps(limite);
}

static void verifier_arguments(int argc, char ** argv)
{
    if (argc != 3 && argc != 4)
    {
        fprintf(stderr, "Usage: %s <producteurs> <consommateurs> [taille tampon]", argv[0]);
        exit(EX_USAGE);
    }
    else if (atoi(argv[1]) <= 0 || atoi(argv[2]) < 0 || (argc == 4 && atoi(argv[3]) < 1))
    {
        fprintf(stderr, "Usage: %s <producteurs> <consommateurs> [taille tampon]\n"
                "producteurs > 0; consommateurs >= 0; taille tampon > 0\n",
                argv[0]);
        exit(EX_DATAERR);
    }
}

int main(int argc, char ** argv)
{
    verifier_arguments(argc, argv);

    const int nombre_producteurs = atoi(argv[1]);
    const int nombre_consommateurs = atoi(argv[2]);
    const int taille_tampon = (argc != 4) ? TAILLE_TAMPON_DEFAUT : atoi(argv[3]);

    tampon_t * tampon = creer_tampon(taille_tampon);

    /* À faire avant le changement du handler de SIGINT ! */
    limite = creer_limite();

    config_handler(SIGINT, handler_sigint);

    acteurs_t * producteurs = creer_acteurs(nombre_producteurs, production, tampon, limite);
    acteurs_t * consommateurs = creer_acteurs(nombre_consommateurs, consommation, tampon, limite);

    comptes_t comptes_production = bilan(producteurs);
    comptes_t comptes_consommation = bilan(consommateurs);

    printf("Bilan:\n\tNombre de valeurs produites : %llu\n"
            "\tSomme des valeurs produites : %llu\n"
            "\tNombre de valeurs consommées : %llu\n"
            "\tSomme des valeurs consommées : %llu\n",
            comptes_production.nombre, comptes_production.somme,
            comptes_consommation.nombre, comptes_consommation.somme);

    tampon = detruire_tampon(tampon);
    limite = detruire_limite(limite);
    producteurs = detruire_acteurs(producteurs);
    consommateurs = detruire_acteurs(consommateurs);

    return 0;
}
