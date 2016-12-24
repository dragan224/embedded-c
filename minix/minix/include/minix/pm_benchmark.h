#ifndef BENCHMARK_H
#define BENCHMARK_H

#include <stdio.h> // printf

// Konastante za masku za API
static const int PROCESS_START = 1; // Broj pokrenutih procesa
static const int PROCESS_END   = 2; // Broj zavrsenih procesa
static const int PROCESS_KILL  = 4; // Broj procesa kome je postal KILL signal
static const int USER_MEMORY   = 8; // Ukupna memorija koju procesi koriste

// Flagovi koji govore da li pratimo neku metriku
static int _flag_ps = 0; // Da li pratimo broj pokrenutih procesa
static int _flag_pe = 0; // Da li pratimo broj zavrsenih procesa
static int _flag_pk = 0; // Da li pratimo broj killovanih procesa
static int _flag_um = 0; // Da li pratimo memoriju

// Metrike koje pratimo
static int _cnt_ps = 0; // Broj pokrenutih procesa
static int _cnt_pe = 0; // Broj zavrsenih procesa
static int _cnt_pk = 0; // Broj killovanih procesa
static unsigned long _cnt_um = 0; // Korisnicka memorija (u bajtovima)

// Setuje flag za metriku
int _set(int* flag) {
  *flag = 1;
  return 0;
}

// Resetuje flag i brojac metrike
int _reset(int* flag, int* counter) {
  *flag = 0;
  *counter = 0;
  return 0;
}

int _reset(int* flag, unsigned long* counter) {
  *flag = 0;
  *counter = 0;
  return 0;
}

int _is_set(int flag) {
  return flag;
}

int start_pm_benchmark(int mask) {
  if (mask < 0 || mask > 15) return -2; // Maska nije validna.

  // Proverava da li su sve metrike iskljucene
  if ((mask & PROCESS_START) && _is_set(_flag_ps)) return -1;
  if ((mask & PROCESS_END)   && _is_set(_flag_pe)) return -1;
  if ((mask & PROCESS_KILL)  && _is_set(_flag_pk)) return -1;
  if ((mask & USER_MEMORY)   && _is_set(_flag_um)) return -1;

  if (mask & PROCESS_START) _set(&_flag_ps);
  if (mask & PROCESS_END)   _set(&_flag_pe); 
  if (mask & PROCESS_KILL)  _set(&_flag_pk); 
  if (mask & USER_MEMORY)   _set(&_flag_um); 
  
  return 0;
}

int stop_pm_benchmark(int mask) {
  if (mask < 0 || mask > 15) return -2; // Maska nije validna.
  
  // Proverava da li su sve metrike ukljucene
  if ((mask & PROCESS_START) && !_is_set(_flag_ps)) return -1;
  if ((mask & PROCESS_END)   && !_is_set(_flag_pe)) return -1;
  if ((mask & PROCESS_KILL)  && !_is_set(_flag_pk)) return -1;
  if ((mask & USER_MEMORY)   && !_is_set(_flag_um)) return -1; 

  if (mask & PROCESS_START) _reset(&_flag_ps, &_cnt_ps); 
  if (mask & PROCESS_END)   _reset(&_flag_pe, &_cnt_pe); 
  if (mask & PROCESS_KILL)  _reset(&_flag_pk, &_cnt_pk); 
  if (mask & USER_MEMORY)   _reset(&_flag_um, &_cnt_um); 
 
  return 0;
}

// Stampa na ekran sve metrike sa setovanim flagom;
void pm_metrics_dmp() {
  if (_flag_ps || _flag_pe || _flag_pk || _flag_um) {
    if (_flag_ps) printf("Broj pokrenutih procesa %d\n",  _cnt_ps);
    if (_flag_pe) printf("Broj zavrsenih procesa %d\n",   _cnt_pe);
    if (_flag_pk) printf("Broj killovanih procesa %d\n",  _cnt_pk);
    if (_flag_um) printf("Ukupna memorija procesa %uld\nB", _cnt_um);
  } else {
    printf("Trenutno ne pratimo nijednu metriku.");
  }
}

#endif // BENCHMARK_H






