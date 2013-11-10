#include "commun.h"
#include "limite.h"
#include "societe.h"
#include "tampon.h"

static limite_t * limite;

static void handler_sigint(int signum)
{
    /* Il vaut mieux sauvegarder errno dans les handler... */
    int ancien_errno = errno;

    printf("\n");
    fin_des_temps(limite);
    errno = ancien_errno;
}

static inline void verifier_arguments(int argc, char ** argv)
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

static inline int verifier_resultats(comptes_t production, comptes_t consommation, int taille_tampon)
{
    /* Les consommateurs s'arrêtent dès la fin de production : il peut rester
     * des valeurs à consommer, mais pas plus que la taille du tampon !
     * Ils ne peuvent pas non plus avoir consommé plus de valeurs que ce que
     * les producteurs ont produit.
     */
    int difference = production.nombre - consommation.nombre;
    if (difference < 0
        || difference > taille_tampon
        || production.somme < consommation.somme
        )
    {
        fprintf(stderr, "Les résultats sont incohérents.\n");
        return 1;
    }
    else
        return 0;
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

    acteurs_t * producteurs = creer_acteurs(nombre_producteurs, produire, tampon, limite);
    acteurs_t * consommateurs = creer_acteurs(nombre_consommateurs, consommer, tampon, limite);

    comptes_t production = bilan(producteurs);
    comptes_t consommation = bilan(consommateurs);

    printf("Bilan:\n\tNombre de valeurs produites : %llu\n"
            "\tSomme des valeurs produites : %llu\n"
            "\tNombre de valeurs consommées : %llu\n"
            "\tSomme des valeurs consommées : %llu\n",
            production.nombre, production.somme,
            consommation.nombre, consommation.somme);

    tampon = detruire_tampon(tampon);
    limite = detruire_limite(limite);
    producteurs = detruire_acteurs(producteurs);
    consommateurs = detruire_acteurs(consommateurs);

    return verifier_resultats(production, consommation, taille_tampon);
}
