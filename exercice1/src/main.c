#include "commun.h"
#include "limite.h"
#include "societe.h"
#include "tampon.h"

static limite_t * limite;
void handler_sigint(int signum)
{
    printf("\n");
    fin_des_temps(limite);
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
    producteurs = detruire_acteurs(producteurs);
    consommateurs = detruire_acteurs(consommateurs);
    limite = detruire_limite(limite);

    return 0;
}
