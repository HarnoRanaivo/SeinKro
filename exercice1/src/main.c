#include "common.h"
#include "temps.h"

static findestemps temps;

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

static inline infos_t * creer_infos(tampon_t * tampon, sem_t * semaphore_tampon, sem_t * semaphore_acteurs, findestemps * temps, unsigned int numero)
{
    infos_t * i = malloc(sizeof *i);

    if (i == NULL)
        pq_error("malloc", EX_OSERR);

    i->tampon = tampon;
    i->semaphore_tampon = semaphore_tampon;
    i->semaphore_acteurs = semaphore_acteurs;
    i->temps = temps;
    i->numero = numero;

    return i;
}

static inline sem_t * creer_semaphore(int valeur)
{
    sem_t * s = malloc(sizeof *s);
    if (s == NULL)
        pq_error("malloc", EX_OSERR);

    int erreur = sem_init(s, 0, valeur);
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

static int ecrire_entier(tampon_t * tampon, unsigned int * numero, comptes_t * comptes, findestemps * temps)
{
    int debut = tampon->suivant;
    int fin = tampon->curseur;
    int * tab = tampon->valeurs;

    if (est_ce_la_fin_des_temps(temps))
    {
        tab[debut] = FIN_PRODUCTION;
        return FIN_PRODUCTION;
    }

    if (fin >= TAILLE_TAMPON)
        return TAMPON_REMPLI;

    int valeur = nombre_aleatoire(numero);
    tab[fin] = valeur;
    int suivant = (fin + 1) % TAILLE_TAMPON;
    tampon->curseur = (suivant != debut) ? suivant : (suivant + TAILLE_TAMPON);
    comptes->nombre++;
    comptes->somme += valeur;
    printf("Ecriture : %d\n", valeur);

    return 0;
}

static int lire_entier(tampon_t * tampon, comptes_t * comptes)
{
    int debut = tampon->suivant;
    int fin = tampon->curseur;
    int * tab = tampon->valeurs;

    int entier = tab[debut];
    printf("Lecture : %d\n", entier);

    if (entier == FIN_PRODUCTION)
        return FIN_PRODUCTION;

    if (debut == fin)
        return TAMPON_VIDE;

    tampon->suivant = (debut + 1) % TAILLE_TAMPON;
    tampon->curseur = (fin >= TAILLE_TAMPON) ? (fin - TAILLE_TAMPON) : fin;
    comptes->nombre++;
    comptes->somme += entier;

    return entier;
}

void * production(void * arg)
{
    infos_t * i = (infos_t *) arg;
    comptes_t * c = creer_comptes();
    int fin = 0;
    /* masquer_signal(SIGINT); */

    while (fin != FIN_PRODUCTION)
    {
        sem_wait(i->semaphore_tampon);
        sem_wait(i->semaphore_acteurs);
        fin = ecrire_entier(i->tampon, &i->numero, c, i->temps);
        sem_post(i->semaphore_acteurs);
        sem_post(i->semaphore_tampon);
    }

    free(arg);

    pthread_exit(c);
}

void * consommation(void * arg)
{
    infos_t * i = (infos_t *) arg;
    comptes_t * c = creer_comptes();
    int lecture = 0;
    /* masquer_signal(SIGINT); */

    while (lecture != FIN_PRODUCTION)
    {
        sem_wait(i->semaphore_tampon);
        sem_wait(i->semaphore_acteurs);
        lecture = lire_entier(i->tampon, c);
        sem_post(i->semaphore_acteurs);
        sem_post(i->semaphore_tampon);
    }
    free(arg);

    pthread_exit(c);
}

void creer_acteurs(pthread_t * acteurs, void * (*fonction) (void *), tampon_t * tampon, sem_t * semaphore_tampon, findestemps * temps, int nombre)
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
    /* sem_t * semaphore_tampon = creer_semaphore(0); */
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
