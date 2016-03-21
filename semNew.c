/*Alejandro Baza & Kyle Latino
* CEC 450
* Dr. Kornecki
* Lab 4 part 2
*
* This program is intended to be a practice problem demonstrating the use of semaphores to
* provide synchronization between tasks.
*/

#include <vxWorks.h> /* Always include this as the first thing in every program */
#include <tickLib.h> /* using tickGet() */
#include <taskLib.h> /* we use tasks */
#include <sysLib.h> /* we use sysClk... */
#include <semLib.h> /* we use semaphores */
#include <logLib.h> /* we use logMsg rather than printf */

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
    taskDelay(5);
  }
}

/* display function prints out then clears the values of data */
void Display(void){
  int count = 0;
  while(1){
    semTake(semBin2, WAIT_FOREVER); /* begin critical section */
    tFin = tickGet();
    ticks = tFin - tIn;
    logMsg("Display #%d=> %d %d %d at %d ticks\n", count++, data.x ,data.y ,data.z, ticks, ticks /* not sure why an additional argument is needed... */);
    data.x=0;
    data.y=0;
    data.z=0;
    semGive(semBin1); /* allow sensor to run */
    taskDelay(5);
  }
}


/*
Running this program produces an output of "Display #'count'=> 1 1 1 at 'some number ticks'" to print on the arcom monitor. The program works because we use two binary semaphores, 'semBin1' and 'semBin2', to force the critical sections of the code to alternate. At the end of each critical section the task returns the semaphore for the next task to be allowed to run. This can be acheived without the use of mutex semaphores due to the binary semaphores naturally being made mutually exclusive. The use of the taskDelay function is simply to make the output more readable. Removing the delays will allow the tasks to alternate at their maximum posisble speed, however the output values of x, y, and z will remain 1. VxWorks applies a method of priority inversion when a high priority task is requesting a semaphore currently held by a lower priority task. This is done by temporarily changing the priority of the task in control of the semaphore to match the highest priority task currently waiting for that semaphore. Thus, changing the priority of the tasks will not effect the output.

Comments:
Looking back it is hard to see why this lab question caused so much trouble. The most difficult part could only be the lack of clarity. This is understandable due to fact that substantial clarification would almost be writing the code for us. As usual, once the algorithm is developed and the code is written it is difficult to imagine writing the code any other way. The solution is simple and makes perfect sense.
*/
