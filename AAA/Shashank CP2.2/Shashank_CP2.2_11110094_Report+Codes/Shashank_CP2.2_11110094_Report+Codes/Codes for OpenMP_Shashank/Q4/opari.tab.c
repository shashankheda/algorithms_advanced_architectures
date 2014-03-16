#include "pomp_lib.h"


extern struct ompregdescr omp_rd_25;
extern struct ompregdescr omp_rd_26;
extern struct ompregdescr omp_rd_27;
extern struct ompregdescr omp_rd_28;
extern struct ompregdescr omp_rd_29;
extern struct ompregdescr omp_rd_30;
extern struct ompregdescr omp_rd_31;
extern struct ompregdescr omp_rd_32;

int POMP_MAX_ID = 33;

struct ompregdescr* pomp_rd_table[33] = {
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  &omp_rd_25,
  &omp_rd_26,
  &omp_rd_27,
  &omp_rd_28,
  &omp_rd_29,
  &omp_rd_30,
  &omp_rd_31,
  &omp_rd_32,
};
