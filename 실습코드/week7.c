/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*
*                           (c) Copyright 1992-2002, Jean J. Labrosse, Weston, FL
*                                           All Rights Reserved
*
*                                               EXAMPLE #1
*********************************************************************************************************
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE                 512       /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                        4        /* Number of identical tasks                          */

/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_STK        TaskStartStk[TASK_STK_SIZE];
char          TaskData[N_TASKS];                      /* Parameters to pass to each task               */
OS_EVENT* RandomSem; // semaphore variable
OS_FLAG_GRP* grp; // event flag grp
OS_EVENT* mbox_to_voting; // mailbox to voting task
OS_EVENT* queue_to_master; // message queue to master task

char majority; // majority variable
void* master_queue[3]; // message queue buffer

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void  Task(void* data);                       /* Function prototypes of tasks                  */
void  TaskStart(void* data);                  /* Function prototypes of Startup task           */
static  void  TaskStartCreateTasks(void);
static  void  TaskStartDispInit(void);
static  void  TaskStartDisp(void);

/*$PAGE*/
/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/

void  main(void)
{
	PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);      /* Clear the screen                         */

	OSInit();                                              /* Initialize uC/OS-II                      */

	PC_DOSSaveReturn();                                    /* Save environment to return to DOS        */
	PC_VectSet(uCOS, OSCtxSw);                             /* Install uC/OS-II's context switch vector */

	OSTaskCreate(TaskStart, (void*)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);
	OSStart();                                             /* Start multitasking                       */
}


/*
*********************************************************************************************************
*                                              STARTUP TASK
*********************************************************************************************************
*/
void  TaskStart(void* pdata)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
	OS_CPU_SR  cpu_sr;
#endif
	char       s[100];
	INT16S     key;
	INT8U err;
	INT8U i;

	pdata = pdata;                                         /* Prevent compiler warning                 */

	TaskStartDispInit();                                   /* Initialize the display                   */

	OS_ENTER_CRITICAL();
	PC_VectSet(0x08, OSTickISR);                           /* Install uC/OS-II's clock tick ISR        */
	PC_SetTickRate(OS_TICKS_PER_SEC);                      /* Reprogram tick rate                      */
	OS_EXIT_CRITICAL();

	OSStatInit();                                          /* Initialize uC/OS-II's statistics         */

	// initialize semaphore, event flag, mailbox, message queue
	RandomSem = OSSemCreate(1);
	grp = OSFlagCreate(0x00, &err);
	mbox_to_voting = OSMboxCreate(0);
	queue_to_master = OSQCreate(&master_queue[0], 3);

	TaskStartCreateTasks();                                /* Create all the application tasks         */

	for (;;) {
		TaskStartDisp();                                  /* Update the display                       */


		if (PC_GetKey(&key) == TRUE) {                     /* See if key has been pressed              */
			if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
				PC_DOSReturn();                            /* Return to DOS                            */
			}
		}

		OSCtxSwCtr = 0;                                    /* Clear context switch counter             */
		OSTimeDlyHMSM(0, 0, 1, 0);                         /* Wait one second                          */
	}
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                        INITIALIZE THE DISPLAY
*********************************************************************************************************
*/

static  void  TaskStartDispInit(void)
{
	/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
	/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
	PC_DispStr(0, 0, "                         uC/OS-II, The Real-Time Kernel                         ", DISP_FGND_WHITE + DISP_BGND_RED + DISP_BLINK);
	PC_DispStr(0, 1, "                                Jean J. Labrosse                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 2, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 3, "                                    EXAMPLE #1                                  ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 4, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 5, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 6, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 7, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 8, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 9, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 10, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 11, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 12, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 13, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 14, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 15, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 16, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 17, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 18, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 19, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 20, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 21, "                                                                                ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 22, "#Tasks          :        CPU Usage:     %                                       ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 23, "#Task switch/sec:                                                               ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
	PC_DispStr(0, 24, "                            <-PRESS 'ESC' TO QUIT->                             ", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY + DISP_BLINK);
	/*                                1111111111222222222233333333334444444444555555555566666666667777777777 */
	/*                      01234567890123456789012345678901234567890123456789012345678901234567890123456789 */
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                           UPDATE THE DISPLAY
*********************************************************************************************************
*/

static  void  TaskStartDisp(void)
{
	char   s[80];


	sprintf(s, "%5d", OSTaskCtr);                                  /* Display #tasks running               */
	PC_DispStr(18, 22, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

#if OS_TASK_STAT_EN > 0
	sprintf(s, "%3d", OSCPUUsage);                                 /* Display CPU usage in %               */
	PC_DispStr(36, 22, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);
#endif

	sprintf(s, "%5d", OSCtxSwCtr);                                 /* Display #context switches per second */
	PC_DispStr(18, 23, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

	sprintf(s, "V%1d.%02d", OSVersion() / 100, OSVersion() % 100); /* Display uC/OS-II's version number    */
	PC_DispStr(75, 24, s, DISP_FGND_YELLOW + DISP_BGND_BLUE);

	switch (_8087) {                                               /* Display whether FPU present          */
	case 0:
		PC_DispStr(71, 22, " NO  FPU ", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;

	case 1:
		PC_DispStr(71, 22, " 8087 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;

	case 2:
		PC_DispStr(71, 22, "80287 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;

	case 3:
		PC_DispStr(71, 22, "80387 FPU", DISP_FGND_YELLOW + DISP_BGND_BLUE);
		break;
	}
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                             CREATE TASKS
*********************************************************************************************************
*/

static  void  TaskStartCreateTasks(void)
{
	INT8U  i;


	for (i = 0; i < N_TASKS; i++) {                        /* Create N_TASKS identical tasks           */
		TaskData[i] = '0' + i;                             /* Each task will display its own letter    */
		OSTaskCreate(Task, (void*)&TaskData[i], &TaskStk[i][TASK_STK_SIZE - 1], i + 1);
	}
}

/*
*********************************************************************************************************
*                                                  TASKS
*********************************************************************************************************
*/

// week 7
void Task(void* pdata) {
	INT8U err;

	INT8U i, j;

	INT8U select_master; // master_task number for decision task
	INT8U get_master; // master_task number for voting task
	INT8U get_number; // variable to decide O or X randomly
	char letters[3]; // variable to store O or X from each voting task
	char get_letter; // variable to store O or X decided randomly to be sent to master task
	char push_letter; // variable to store 0r X to be sent to decision task
	int flag; // variable to count number of O
	char get_majority; // variable to store majority value read from shared variable


	int task_number = (int)(*(char*)pdata - 48); // task number 0~3

	int fgnd_color, bgnd_color; // colors for decision task display

	char s[10]; // string for task display

	if (*(char*)pdata == '3') { //decision task
		for (;;) {
			// decide master task number randomly
			select_master = random(3);
			OSMboxPostOpt(mbox_to_voting, (void*)&select_master, OS_POST_OPT_BROADCAST);

			// wait for master task to finish to read majority
			OSFlagPend(grp, 0x01, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);

			// read majority from shared variable
			OSSemPend(RandomSem, 0, &err);
			get_majority = majority;
			OSSemPost(RandomSem);

			// display majority result
			if (get_majority == 'O') {
				bgnd_color = DISP_BGND_BLUE;
				fgnd_color = DISP_FGND_BLUE;
			}
			else {
				bgnd_color = DISP_BGND_RED;
				fgnd_color = DISP_FGND_RED;
			}

			for (j = 5; j < 20; j++) {
				for (i = 0; i < 80; i++) {
					PC_DispChar(i, j, ' ', fgnd_color + bgnd_color);
				}
			}

			OSTimeDlyHMSM(0, 0, 5, 0);
		}
	}
	else { // voting tasks
		for (;;) {
			// wait for decision task to decide master task number
			get_master = *(INT8U*)OSMboxPend(mbox_to_voting, 0, &err);

			// decide O or X randomly
			get_number = random(2);
			if (get_number == 0)
				get_letter = 'X';
			else
				get_letter = 'O';

			// if not master task, send O or X to master task
			if (task_number != get_master) {
				OSQPost(queue_to_master, (void*)&get_letter);
			}
			else { // if master task, gather O or X from other voting tasks
				letters[task_number] = get_letter;
				for (i = 0; i < N_TASKS - 1; ++i) {
					if (i == task_number)
						continue;
					// wait for other voting tasks to send their letters
					letters[i] = *(char*)OSQPend(queue_to_master, 0, &err);
				}

				// decide majority letter
				flag = 0;
				for (i = 0; i < N_TASKS - 1; ++i) {
					if (letters[i] == 'O')
						flag++;
				}
				if (flag >= 2)
					push_letter = 'O';
				else
					push_letter = 'X';

				// update majority shared variable
				OSSemPend(RandomSem, 0, &err);
				majority = push_letter;
				OSSemPost(RandomSem);

				// signal decision task that majority is decided
				OSFlagPost(grp, 0x01, OS_FLAG_SET, &err);
			}
			// display task info
			PC_DispStr(0 + 18 * task_number, 4, "task", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispChar(4 + 18 * task_number, 4, *(char*)pdata, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispStr(6 + 18 * task_number, 4, s, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispStr(8 + 18 * task_number, 4, "[", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispStr(10 + 18 * task_number, 4, "]", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
			PC_DispChar(9 + 18 * task_number, 4, get_letter, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);

			OSTimeDlyHMSM(0, 0, 5, 0);
		}
	}
}