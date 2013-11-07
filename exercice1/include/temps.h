#ifndef __TEMPS_H
#define __TEMPS_H

/* 1 : fin, les producteurs enverront -1. */
static int _fin_des_temps = 0;
static sem_t _semaphore_des_temps;

static inline void init_fin_des_temps()
{
    sem_init(&_semaphore_des_temps, 0, 1);
}

static inline int est_ce_la_fin_des_temps()
{
    sem_wait(&_semaphore_des_temps);
    int temps = _fin_des_temps;
    sem_post(&_semaphore_des_temps);
    return temps == 1;
}

static inline void cest_la_fin_des_temps()
{
    sem_wait(&_semaphore_des_temps);
    _fin_des_temps = 1;
    sem_post(&_semaphore_des_temps);
}

#endif /* __TEMPS_H */
