#include <vxWorks.h>
#include <stdio.h>
#include <taskLib.h>
#include <sysLib.h>
#include <msgQLib.h>
#include <time.h>

/*  function prototypes */
void Server(void);
void Client1(void);
void Client2(void);
void Client3(void);
int Stamp(void);

/*  defines */
#define MAX_MESSAGE_LENGTH 50
#define MAX_MESSAGES 100



/*  global vars */
MSG_Q_ID receiveMQ1;
MSG_Q_ID sendMQ1;
MSG_Q_ID receiveMQ2;
MSG_Q_ID sendMQ2;
MSG_Q_ID receiveMQ3;
MSG_Q_ID sendMQ3;
/*MSG_Q_ID mqId;*/
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
void message(void){
	int ClientId, ServerId;
	/* create message queue */
	if ((mqId = msgQCreate(MAX_MESSAGES,MAX_MESSAGE_LENGTH,MSG_Q_FIFO))== NULL)
		printf("msgQCreate in failed\n");
	/* spawn the two tasks that will use the message queue */
	if((ClientId = taskSpawn("t1",110,0x100,2000,(FUNCPTR)Client1,0,0,0,0,0,0,0,0,0,0)) == ERROR)
		printf("taskSpawn taskOne failed\n");

	if((ClientId = taskSpawn("t2",110,0x100,2000,(FUNCPTR)Client2,0,0,0,0,0,0,0,0,0,0)) == ERROR)
		printf("taskSpawn taskTwo failed\n");

	if((ClientId = taskSpawn("t3",110,0x100,2000,(FUNCPTR)Client3,0,0,0,0,0,0,0,0,0,0)) == ERROR)
		printf("taskSpawn taskThree failed\n");

	if((ServerId = taskSpawn("t4",110,0x100,2000,(FUNCPTR)Server,0,0,0,0,0,0,0, 0,0,0)) == ERROR)
		printf("taskSpawn taskServer failed\n");
}


/*  function to create Receive server */
void Client1(void){
	char message[MAX_MESSAGE_LENGTH];
	int i = 0;
	while(1) {
		/* create and send message */
		sprintf(message,"message # %d from Client %d", i, taskIdSelf());
		printf("SENDER %d MESSAGE %d: \n",taskIdSelf(), i++); /* print what is sent */ if((msgQSend(sendMQ1,message,MAX_MESSAGE_LENGTH, WAIT_FOREVER, MSG_PRI_NORMAL))== ERROR)
			printf("msgQSend in Client failed\n");
		taskDelay(sysClkRateGet()); /* delay for one second */
	}
}

/*  function to create Receive server */
void Client2(void){
	char message[MAX_MESSAGE_LENGTH];
	int i = 0;
	while(1) {
		/* create and send message */
		sprintf(message,"message # %d from Client %d", i, taskIdSelf());
		printf("SENDER %d MESSAGE %d: \n",taskIdSelf(), i++); /* print what is sent */ if((msgQSend(sendMQ2,message,MAX_MESSAGE_LENGTH, WAIT_FOREVER, MSG_PRI_NORMAL))== ERROR)
			printf("msgQSend in Client failed\n");
		taskDelay(sysClkRateGet()); /* delay for one second */
	}
}

/*  function to create Receive server */
void Client3(void){
	char message[MAX_MESSAGE_LENGTH];
	int i = 0;
	while(1) {
		/* create and send message */
		sprintf(message,"message # %d from Client %d", i, taskIdSelf());
		printf("SENDER %d MESSAGE %d: \n",taskIdSelf(), i++); /* print what is sent */ if((msgQSend(sendMQ3,message,MAX_MESSAGE_LENGTH, WAIT_FOREVER, MSG_PRI_NORMAL))== ERROR)
			printf("msgQSend in Client failed\n");
		taskDelay(sysClkRateGet()); /* delay for one second */
	}
}

/*  function to create 3 Sender clients */
void Server(void){
	char msgBuf[MAX_MESSAGE_LENGTH];
	while(1) {
		/* receive message sendMQ1*/
		if(msgQReceive(sendMQ1,msgBuf,MAX_MESSAGE_LENGTH, WAIT_FOREVER ) == ERROR)
			printf("msgQReceive1 in Server failed\n");
		else
			printf("Server %d: %s\n",taskIdSelf(), msgBuf);
		taskDelay(sysClkRateGet()/60); /* delay for 1/60 of second (one tick) */

		/* receive message sendMQ2*/
		if(msgQReceive(sendMQ2,msgBuf,MAX_MESSAGE_LENGTH, WAIT_FOREVER ) == ERROR)
			printf("msgQReceive2 in Server failed\n");
		else
			printf("Server %d: %s\n",taskIdSelf(), msgBuf);
		taskDelay(sysClkRateGet()/60); /* delay for 1/60 of second (one tick) */

		/* receive message sendMQ3*/
		if(msgQReceive(sendMQ3,msgBuf,MAX_MESSAGE_LENGTH, WAIT_FOREVER ) == ERROR)
			printf("msgQReceive3 in Server failed\n");
		else
			printf("Server %d: %s\n",taskIdSelf(), msgBuf);
		taskDelay(sysClkRateGet()/60); /* delay for 1/60 of second (one tick) */
	}
}





/*   */

/*   */
