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

	/* create message queues */
	if ((receiveMQ1 = msgQCreate(MAX_MESSAGES,MAX_MESSAGE_LENGTH,MSG_Q_FIFO))== NULL)
		printf("msgQCreate in failed\n");

	if ((receiveMQ2 = msgQCreate(MAX_MESSAGES,MAX_MESSAGE_LENGTH,MSG_Q_FIFO))== NULL)
		printf("msgQCreate in failed\n");

	if ((receiveMQ3 = msgQCreate(MAX_MESSAGES,MAX_MESSAGE_LENGTH,MSG_Q_FIFO))== NULL)
		printf("msgQCreate in failed\n");

	if ((sendMQ1 = msgQCreate(MAX_MESSAGES,MAX_MESSAGE_LENGTH,MSG_Q_FIFO))== NULL)
		printf("msgQCreate in failed\n");

	if ((sendMQ2 = msgQCreate(MAX_MESSAGES,MAX_MESSAGE_LENGTH,MSG_Q_FIFO))== NULL)
		printf("msgQCreate in failed\n");

	if ((sendMQ3 = msgQCreate(MAX_MESSAGES,MAX_MESSAGE_LENGTH,MSG_Q_FIFO))== NULL)
		printf("msgQCreate in failed\n");

	/* spawn the four tasks that will use the message queues */
	if((ClientId = taskSpawn("t1",110,0x100,2000,(FUNCPTR)Client1,0,0,0,0,0,0,0,0,0,0)) == ERROR)
		printf("taskSpawn taskOne failed\n");

	if((ClientId = taskSpawn("t2",110,0x100,2000,(FUNCPTR)Client2,0,0,0,0,0,0,0,0,0,0)) == ERROR)
		printf("taskSpawn taskTwo failed\n");

	if((ClientId = taskSpawn("t3",110,0x100,2000,(FUNCPTR)Client3,0,0,0,0,0,0,0,0,0,0)) == ERROR)
		printf("taskSpawn taskThree failed\n");

	if((ServerId = taskSpawn("t4",110,0x100,2000,(FUNCPTR)Server,0,0,0,0,0,0,0, 0,0,0)) == ERROR)
		printf("taskSpawn taskServer failed\n");
}


/*  function to create Client1 */
void Client1(void){
	char message[MAX_MESSAGE_LENGTH];
	int i = 0;
	while(1) {
		/* create and send message */
		sprintf(message,"message # %d from Client %d", i, taskIdSelf());
		printf("Client1 %d MESSAGE %d: at time: %d\n",taskIdSelf(), i++, Stamp()); /* print what is sent */ if((msgQSend(sendMQ1,message,MAX_MESSAGE_LENGTH, WAIT_FOREVER, MSG_PRI_NORMAL))== ERROR)
			printf("msgQSend in Client failed\n");
		taskDelay(20); /* delay sending 20 ticks */

		if(msgQReceive(receiveMQ1,message,MAX_MESSAGE_LENGTH, WAIT_FOREVER ) == ERROR)
			printf("msgQReceive1 in Client1 failed\n");
		else
			printf("Client1 %d: %s at time: %d\n",taskIdSelf(), message, Stamp());
	}
}

/*  function to create Client2 */
void Client2(void){
	char message[MAX_MESSAGE_LENGTH];
	int i = 0;
	while(1) {
		/* create and send message */
		sprintf(message,"message # %d from Client %d ", i, taskIdSelf());
		printf("Client2 %d MESSAGE %d: \n",taskIdSelf(), i++); /* print what is sent */ if((msgQSend(sendMQ2,message,MAX_MESSAGE_LENGTH, WAIT_FOREVER, MSG_PRI_NORMAL))== ERROR)
			printf("msgQSend in Client failed\n");
		taskDelay(40); /* delay sending 40 ticks */
	}
}

/*  function to create Client3 */
void Client3(void){
	char message[MAX_MESSAGE_LENGTH];
	int i = 0;
	while(1) {
		/* create and send message */
		sprintf(message,"message # %d from Client %d", i, taskIdSelf());
		printf("Client3 %d MESSAGE %d: \n",taskIdSelf(), i++); /* print what is sent */ if((msgQSend(sendMQ3,message,MAX_MESSAGE_LENGTH, WAIT_FOREVER, MSG_PRI_NORMAL))== ERROR)
			printf("msgQSend in Client failed\n");
		taskDelay(60); /* delay sending 60 ticks */
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
			printf("Server %d: %s at time: %d\n",taskIdSelf(), msgBuf, Stamp());
		/*taskDelay(sysClkRateGet()/60); /* delay for 1/60 of second (one tick) */

		if((msgQSend(receiveMQ1,msgBuf,MAX_MESSAGE_LENGTH, WAIT_FOREVER, MSG_PRI_NORMAL))== ERROR)
		 printf("msgQSend in Server failed\n");

		/* receive message sendMQ2*/
		if(msgQReceive(sendMQ2,msgBuf,MAX_MESSAGE_LENGTH, WAIT_FOREVER ) == ERROR)
			printf("msgQReceive2 in Server failed\n");
		else
			printf("Server %d: %s\n",taskIdSelf(), msgBuf);
		/*taskDelay(sysClkRateGet()/60); /* delay for 1/60 of second (one tick) */

		/* receive message sendMQ3*/
		if(msgQReceive(sendMQ3,msgBuf,MAX_MESSAGE_LENGTH, WAIT_FOREVER ) == ERROR)
			printf("msgQReceive3 in Server failed\n");
		else
			printf("Server %d: %s\n",taskIdSelf(), msgBuf);
		/*taskDelay(sysClkRateGet()/60); /* delay for 1/60 of second (one tick) */
	}
}





/*   */

/*   */
