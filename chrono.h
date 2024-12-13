// chrono.h
// Feito por:
// Vitor Lorenzo Cumim - GRR20224757
// Rafael Urbanek Laurentino - GRR20224381

#ifndef CHRONO_H
#define CHRONO_H

#include <stdio.h>
#include <time.h>

// Declaração da struct
typedef struct {
    struct timespec xadd_time1, xadd_time2;
    long long xtotal_ns;
    long xn_events;
} chronometer_t;

// Declaração das funções
void chrono_reset(chronometer_t *chrono);
void chrono_start(chronometer_t *chrono);
void chrono_stop(chronometer_t *chrono);
long long chrono_gettotal(chronometer_t *chrono);
long long chrono_getcount(chronometer_t *chrono);
void chrono_reportTime(chronometer_t *chrono, char *s);
void chrono_report_TimeInLoop(chronometer_t *chrono, char *s, int loop_count);

#endif // CHRONO_H
