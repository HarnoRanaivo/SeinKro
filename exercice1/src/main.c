#include "common.h"

static inline comptes_t bilan (const pthread_t * acteurs, int nombre_acteurs)
{
    comptes_t resultat;
    resultat.nombre = 0;
    resultat.somme = 0;

    for (int i = 0; i < nombre_acteurs; i++)
    {
        comptes_t * res;
        pthread_join(acteurs[i], (void **) &res);
        resultat.nombre += res->nombre;
        resultat.somme += res->somme;
    }

    return resultat;
}

static inline infos_t * creer_infos(tampon_t * tampon, sem_t * semaphore)
{
    infos_t * i = malloc(sizeof *i);

    if (i == NULL)
        pq_error("malloc", EX_OSERR);

    i->tampon = tampon;
    i->semaphore;

    return i;
}

static inline sem_t * creer_semaphore()
{
    sem_t * s = malloc(sizeof *s);
    if (s == NULL)
        pq_error("malloc", EX_OSERR);

    int erreur = sem_init(s, 0, 1);
    if (erreur != 0)
        pq_error("sem_init", EX_OSERR);

    return s;
}

static inline tampon_t * creer_tampon()
{
    tampon_t * t = malloc(sizeof *t);
    if (t == NULL)
        pq_error("malloc", EX_OSERR);

    t->curseur = 0;
    t->suivant = 0;

    return t;
}

static inline comptes_t * creer_comptes()
{
    comptes_t * c = malloc(sizeof *c);
    if (c == NULL)
        pq_error("malloc", EX_OSERR);

    c->nombre = 0;
    c->somme = 0;

    return c;
}

void * consommation(void * arg)
{
    comptes_t * c = creer_comptes();

    tampon_t * t = (tampon_t *) arg;

    pthread_exit(c);
}

static int ecrire_entier(tampon_t * tampon, int * numero, comptes_t * comptes)
{
    int suivant;
    const int taille = TAILLE_TAMPON;
    int * debut = &tampon->curseur;
    int * fin = &tampon->suivant;
    int * tab = tampon->valeurs;

    if (*fin >= taille)
        return TAMPON_REMPLI;

    int valeur;
    if (est_ce_la_fin_des_temps())
        valeur = FIN_PRODUCTION;
    else
        valeur = nombre_aleatoire(numero);

    tab[*fin] = valeur;
    suivant = (*fin + 1) % taille;
    if (suivant != *debut)
        *fin = suivant;
    else
        *fin += taille;

    return valeur == FIN_PRODUCTION ? FIN_PRODUCTION : 0;
}

static int lire_entier(tampon_t * tampon)
{
    int entier;
    const int taille = TAILLE_TAMPON;
    int * debut = &tampon->curseur;
    int * fin = &tampon->suivant;
    int * tab = tampon->valeurs;

    if (*debut == *fin)
        return TAMPON_REMPLI;

    entier = tab[*debut];
    *debut = (*debut + 1) % taille;
    if (*fin >= taille)
        *fin -= taille;

    return entier;
}

void * production(void * arg)
{
    infos_t * i = (infos_t *) arg;
    comptes_t * c = creer_comptes();
    int fin = 0;

    while (fin != FIN_PRODUCTION)
    {
        fin = ecrire_entier(i->tampon, i->numero, c);
    }

    pthread_exit(c);
}

void creer_acteurs(pthread_t * acteurs, void * (*fonction) (void *), tampon_t * tampon, sem_t * semaphore, int nombre)
{
    for (int i = 0 ; i < nombre; i++)
    {
        infos_t * infos = creer_infos(tampon, semaphore);
        *infos->numero = i;
        int erreur = pthread_create(&acteurs[i], NULL, fonction, (void *) infos);
        if (erreur != 0)
            pq_error("pthread_create", EX_OSERR);
    }
}

void handler_sigint(int signum)
{
    cest_la_fin_des_temps();
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

    tampon_t * tampon = creer_tampon();
    sem_t * semaphore = creer_semaphore();

    config_handler(SIGINT, handler_sigint);

    /* Tableaux à taille variable, C99 powa. */
    pthread_t producteurs[nombre_producteurs];
    pthread_t consommateurs[nombre_consommateurs];

    creer_acteurs(producteurs, production, tampon, semaphore, nombre_producteurs);
    creer_acteurs(consommateurs, consommation, tampon, semaphore, nombre_consommateurs);

    comptes_t comptes_production = bilan(producteurs, nombre_producteurs);
    comptes_t comptes_consommation = bilan(consommateurs, nombre_consommateurs);

    printf("Bilan:\n\tNombre de valeurs produites : %d\n\tSomme des valeurs produites : %d\n\t"
            "Nombre de valeurs consommées : %d\n\tSomme des valeurs consommées : %d\n",
            comptes_production.nombre, comptes_production.somme,
            comptes_consommation.nombre, comptes_consommation.somme);

    return 0;
}
