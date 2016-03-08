#include <vxWorks.h>
#include <stdio.h>
#include <taskLib.h>
#include <sysLib.h>
#include <msgQLib.h>
#include <time.h>

/*  function prototypes */
void Server(void);
void Sender(void);
int Stamp(void);
/*  defines */
#define MAX_MESSAGE_LENGTH 50
#define MAX_MESSAGES 100



/*  global vars */
MSG_Q_ID receiveMQ;
MSG_Q_ID sendMQ;
int fd, len;
int msgNum = 0;
/*struct msg outMsg;*/
/*struct msg inMsg;*/

/* Stamp() initializes the clock and then prints out time in sec/nsec every step time ticks */
int Stamp(void){
	struct timespec tstamp;
	tstamp.tv_sec = 0;
	tstamp.tv_nsec = 0;
	clock_settime(CLOCK_REALTIME, &tstamp);
	while(1){
	/*	taskDelay(step);*/
		clock_gettime(CLOCK_REALTIME, &tstamp);
		return("\ntime: %d sec %d nsec", (int) tstamp.tv_sec, (int) tstamp.tv_nsec);
	}
}

/* initialize system */
void messageSetup(void){

	int server, sender1, sender2, sender3;

	/* create message queue */
}

/*  function to create Receive server */
void Server(void){
	char receiverMessage[MAX_MESSAGE_LENGTH];
	while(1) {
	/* receive message */
	if(msgQReceive(sendMQ,receiverMessage,MAX_MESSAGE_LENGTH, WAIT_FOREVER ) == ERROR)
		printf("msgQReceive in Receiver failed\n");
	else
		printf("%d - %d - %d", taskIdSelf(), receiverMessage, Stamp());

	}
}

/*  function to create 3 Sender clients */
void Sender1(void){
	char senderMessage[MAX_MESSAGE_LENGTH];
	sprintf(senderMessage, "%d - %d - %d", taskIdSelf(), msgNum++, Stamp());
	printf(senderMessage);

	msgQSend(sendMQ, senderMessage, MAX_MESSAGE_LENGTH, WAIT_FOREVER, MSG_PRI_NORMAL);
	taskDelay(20);

	/* still need to receive */

}

/*   */

/*   */
