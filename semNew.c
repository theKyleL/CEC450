/* Lab 4 part 2
*
* This program is intended to be a practice problem demonstrating the use of semaphores to
* provide synchronization between tasks.
*
*/

#include <vxWorks.h> /* Always include this as the first thing in every program */
/*#include <time.h> /* we use clock_gettime */
#include <tickLib.h> /* using tickGet() */
#include <taskLib.h> /* we use tasks */
#include <sysLib.h> /* we use sysClk... */
#include <semLib.h> /* we use semaphores */
#include <logLib.h> /* we use logMsg rather than printf */

/* define useful constants for timing */

/*#define NANOS_IN_SEC 1000000000
#define NANOS_PER_MILLI 1000000
#define TICK sysClkRateGet()/60
*/

/* global variables */
/* "shared memory": three data to be kept coherent showing the same values when printing */
struct mem{
int x; int y;int z;
} data;

int taskSensor;
int taskDisplay;
int ticks = 0;
int tIn = 0;
int tFin = 0;

/* semaphores */
SEM_ID semBin1;
SEM_ID semBin2;

/* declare functions */
void init(void);
void Sensor(void);
void Display(void);

void init(void){
  tIn = tickGet();
  semBin1 = semBCreate(SEM_Q_PRIORITY, SEM_FULL); /* create binary semaphore allowing sensor task to run first */
  semBin2 = semBCreate(SEM_Q_PRIORITY, SEM_EMPTY); /* create binary semaphore requiring display task to wait */

  /* spawn tasks */
  taskSensor = taskSpawn("sens", 95, 0x100, 2000, (FUNCPTR)Sensor, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  taskDisplay = taskSpawn("disp", 95, 0x100, 2000, (FUNCPTR)Display, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

/* sensor function atomically increments the values of data */
void Sensor(void){
  while(1){
    semTake(semBin1,WAIT_FOREVER); /* begin critical section */
    data.x++;
    data.y++;
    data.z++;
    semGive(semBin2); /* allow display to run */
  }
}

/* display function prints out then clears the values of data */
void Display(void){
  int count = 0;
  while(1){
    semTake(semBin2, WAIT_FOREVER); /* begin critical section */
    tFin = tickGet();
    ticks = tFin - tIn;
    logMsg("Display #%d=> %d %d %d at %d ticks\n", count++, data.x ,data.y ,data.z, ticks, ticks /* not sure why */);
    data.x=0;
    data.y=0;
    data.z=0;
    semGive(semBin1); /* allow sensor to run */
  }
}


/* what number comes next in the sequence? 4, 3, 3, 5, 4, 4, 3, 5, 5, 4, _ */
