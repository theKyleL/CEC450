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
char* Stamp(void);

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


/* Stamp() prints out time in sec/nsec every step time ticks */
char* Stamp(void){
	struct timespec tstamp;
	char strstamp[] = "                           ";
	clock_settime(CLOCK_REALTIME, &tstamp);
	/*	taskDelay(step);*/
	clock_gettime(CLOCK_REALTIME, &tstamp);
	sprintf(strstamp," time: %d sec %d nsec\n", (int) tstamp.tv_sec, (int) tstamp.tv_nsec);
	return strstamp;
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
	char message1[MAX_MESSAGE_LENGTH];
	while(1) {
		/* create and send message */
		/*sprintf(message1,"message # %d from Client %d", msgNum, taskIdSelf());*/
		sprintf(message1,"%d - %d - %s", taskIdSelf(), msgNum++, Stamp());
		printf("%s\n",message1); /* print what is sent */
		if((msgQSend(sendMQ1,message1,MAX_MESSAGE_LENGTH, WAIT_FOREVER, MSG_PRI_NORMAL))== ERROR)
			printf("msgQSend in Client failed\n");
		taskDelay(20); /* delay sending 20 ticks */

		if(msgQReceive(receiveMQ1,message1,MAX_MESSAGE_LENGTH, WAIT_FOREVER ) == ERROR)
			printf("msgQReceive1 in Client1 failed\n");
		else
			printf("%s Received at %s\n", message1, Stamp());
	}
}

/*  function to create Client2 */
void Client2(void){
	char message2[MAX_MESSAGE_LENGTH];
	while(1) {
		/* create and send message */
		/*sprintf(message2,"message # %d from Client %d ", msgNum, taskIdSelf());*/
		sprintf(message2,"%d - %d - %s", taskIdSelf(), msgNum++, Stamp());
		printf("%s\n",message2); /* print what is sent */
		if((msgQSend(sendMQ2,message2,MAX_MESSAGE_LENGTH, WAIT_FOREVER, MSG_PRI_NORMAL))== ERROR)
			printf("msgQSend in Client2 failed\n");
		taskDelay(40); /* delay sending 40 ticks */

		if(msgQReceive(receiveMQ2,message2,MAX_MESSAGE_LENGTH, WAIT_FOREVER ) == ERROR)
			printf("msgQReceive2 in Client2 failed\n");
		else
			printf("%s Received at %s\n", message2, Stamp());
	}
}

/*  function to create Client3 */
void Client3(void){
	char message3[MAX_MESSAGE_LENGTH];
	while(1) {
		/* create and send message */
		/*sprintf(message3,"message # %d from Client %d", msgNum, taskIdSelf());*/
		sprintf(message3,"%d - %d - %s", taskIdSelf(), msgNum++, Stamp());
		printf("%s\n", message3); /* print what is sent */
		if((msgQSend(sendMQ3,message3,MAX_MESSAGE_LENGTH, WAIT_FOREVER, MSG_PRI_NORMAL))== ERROR)
			printf("msgQSend in Client failed\n");
		taskDelay(60); /* delay sending 60 ticks */

		if(msgQReceive(receiveMQ3,message3,MAX_MESSAGE_LENGTH, WAIT_FOREVER ) == ERROR)
			printf("msgQReceiv3 in Client3 failed\n");
		else
			printf("%s Received at %s\n", message3, Stamp());
	}
}

/*  function to create 3 Sender clients */
void Server(void){
	char msgBuf[MAX_MESSAGE_LENGTH];
	while(1) {
		/* receive message sendMQ1*/
		if(msgQReceive(sendMQ1,msgBuf,MAX_MESSAGE_LENGTH, WAIT_FOREVER ) == ERROR)
			printf("msgQReceive1 in Server failed\n");
		else{
			sprintf(msgBuf, "%s received at %s", msgBuf, Stamp());
			printf("%s", msgBuf);
		if((msgQSend(receiveMQ1,msgBuf,MAX_MESSAGE_LENGTH, WAIT_FOREVER, MSG_PRI_NORMAL))== ERROR)
			printf("msgQSend1 in Server failed\n");
		}

		/* receive message sendMQ2*/
		if(msgQReceive(sendMQ2,msgBuf,MAX_MESSAGE_LENGTH, WAIT_FOREVER ) == ERROR)
			printf("msgQReceive2 in Server failed\n");
		else{
			sprintf(msgBuf, "%s received at %s", msgBuf, Stamp());
			printf("%s", msgBuf);
			if((msgQSend(receiveMQ2,msgBuf,MAX_MESSAGE_LENGTH, WAIT_FOREVER, MSG_PRI_NORMAL))== ERROR)
				printf("msgQSend2 in Server failed\n");
		}

		/* receive message sendMQ3*/
		if(msgQReceive(sendMQ3,msgBuf,MAX_MESSAGE_LENGTH, WAIT_FOREVER ) == ERROR)
			printf("msgQReceive3 in Server failed\n");
		else{
			sprintf(msgBuf, "%s received at %s", msgBuf, Stamp());
			printf("%s", msgBuf);
			if((msgQSend(receiveMQ3,msgBuf,MAX_MESSAGE_LENGTH, WAIT_FOREVER, MSG_PRI_NORMAL))== ERROR)
				printf("msgQSend3 in Server failed\n");
		}
	}
}
