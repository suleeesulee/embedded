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
OS_EVENT* TaskSem[N_TASKS];
OS_EVENT* RandomSem;
INT8U         final; // week2

OS_EVENT* mbox_to_random[4]; // weekd4
OS_EVENT* mbox_to_decision[4];

// week5
OS_FLAG_GRP* s_grp;
OS_FLAG_GRP* r_grp; // flag group for send & receive
int			  send_array[4]; // array to store sent random numbers
char          receive_array[4]; // array to store received letters

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

    RandomSem = OSSemCreate(1);                          /* Random number semaphore                  */
    final = 64;

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
    INT8U i;

    pdata = pdata;                                         /* Prevent compiler warning                 */

    TaskStartDispInit();                                   /* Initialize the display                   */

    OS_ENTER_CRITICAL();
    PC_VectSet(0x08, OSTickISR);                           /* Install uC/OS-II's clock tick ISR        */
    PC_SetTickRate(OS_TICKS_PER_SEC);                      /* Reprogram tick rate                      */
    OS_EXIT_CRITICAL();

    OSStatInit();                                          /* Initialize uC/OS-II's statistics         */

    // week1
    /*for (i = 0; i < N_TASKS; i++) {
        TaskSem[i] = OSSemCreate(i == 0 ? 1 : 0);
    }*/

    // week4
    /*for (i = 0; i < N_TASKS - 1; i++) {
        mbox_to_random[i] = OSMboxCreate(0);
        mbox_to_decision[i] = OSMboxCreate(0);
    }*/

    // week5
    /*s_grp = OSFlagCreate(0x00, &err);
    r_grp = OSFlagCreate(0x00, &err);

    for (i = 0; i < N_TASKS - 2; i++) {
        send_array[i] = 0;
        receive_array[i] = 0;
    }*/

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
        //OSTimeDly(100); /* starvation 시에만 사용 */
    }
}

/*
*********************************************************************************************************
*                                                  TASKS
*********************************************************************************************************
*/

//void  Task (void *pdata) // 4가지 색 번갈아 출력하는 코드
//{
//    INT8U p = *(char*)pdata - '0';
//    INT8U color;
//    INT8U  x;
//    INT8U  y;
//
//    for (;;) {
//        // 출력 전
//        if (p == 0) {
//            color = DISP_FGND_RED + DISP_BGND_RED;
//        }
//        else if (p == 1) {
//			OSTimeDlyHMSM(0, 0, 1, 0);
//            color = DISP_FGND_BLUE + DISP_BGND_BLUE;
//        }
//        else if (p == 2) {
//			OSTimeDlyHMSM(0, 0, 2, 0);
//            color = DISP_FGND_BROWN + DISP_BGND_BROWN;
//        }
//        else if (p == 3) {
//			OSTimeDlyHMSM(0, 0, 3, 0);
//            color = DISP_FGND_GREEN + DISP_BGND_GREEN;
//        }
//
//        // 출력
//        for (y = 0; y < 16; y++) {
//            for (x = 0; x < 80; x++) {
//                PC_DispChar(x, y + 5, ' ', color);
//            }
//        }
//
//        // 출력 후
//        if (p == 0) OSTimeDlyHMSM(0, 0, N_TASKS, 0);
//		else if (p == 1) OSTimeDlyHMSM(0, 0, N_TASKS - 1, 0);
//		else if (p == 2) OSTimeDlyHMSM(0, 0, N_TASKS - 2, 0);
//		else if (p == 3) OSTimeDlyHMSM(0, 0, N_TASKS - 3, 0);
//    }
//
//    //OSSemPend(RandomSem, 0, &err);           /* Acquire semaphore to perform random numbers        */
//    //x = random(80);                          /* Find X position where task number will appear      */
//    //y = random(16);                          /* Find Y position where task number will appear      */
//    //OSSemPost(RandomSem);                    /* Release semaphore                                  */
//    //                                         /* Display the task number on the screen              */
//    //PC_DispChar(x, y + 5, *(char *)pdata, DISP_FGND_RED + DISP_BGND_RED);
//    //OSTimeDly(1);                              /* Delay 1 clock tick                                 */
//}

//void  Task (void *pdata) // 빨간색만 출력
//{
//    INT8U p = *(char*)pdata - '0';
//    INT8U color;
//    INT8U  x;
//    INT8U  y;
//
//    for (;;) {
//        // 출력 전
//        if (p == 0) color = DISP_FGND_RED + DISP_BGND_RED;
//        else if (p == 1) color = DISP_FGND_BLUE + DISP_BGND_BLUE;
//        else if (p == 2) color = DISP_FGND_BROWN + DISP_BGND_BROWN;
//        else if (p == 3) color = DISP_FGND_GREEN + DISP_BGND_GREEN;
//
//        // 출력
//        for (y = 0; y < 16; y++) {
//            for (x = 0; x < 80; x++) {
//                PC_DispChar(x, y + 5, ' ', color);
//            }
//        }
//    }
//}
//
//void  Task(void* pdata) // 빨-파만 출력
//{
//    INT8U p = *(char*)pdata - '0';
//    INT8U color;
//    INT8U  x;
//    INT8U  y;
//    INT32U count;
//
//    for (;;) {
//        // 출력 전
//        if (p == 0) color = DISP_FGND_RED + DISP_BGND_RED;
//        else if (p == 1) color = DISP_FGND_BLUE + DISP_BGND_BLUE;
//        else if (p == 2) color = DISP_FGND_BROWN + DISP_BGND_BROWN;
//        else if (p == 3) color = DISP_FGND_GREEN + DISP_BGND_GREEN;
//
//        // 출력
//        for (y = 0; y < 16; y++) {
//            for (x = 0; x < 80; x++) {
//                PC_DispChar(x, y + 5, ' ', color);
//            }
//        }
//        if (p == 1) {
//			for (count = 0; count < 1000000; count++);
//        }
//
//        // 출력 후
//        if (p == 0) OSTimeDlyHMSM(0, 0, 1, 0);
//    }
//}
//
//void  Task(void* pdata) // 빨-파-갈만 출력
//{
//    INT8U p = *(char*)pdata - '0';
//    INT8U color;
//    INT8U  x;
//    INT8U  y;
//    INT32U count;
//
//    for (;;) {
//        // 출력 전
//        if (p == 0) color = DISP_FGND_RED + DISP_BGND_RED;
//        else if (p == 1) {
//            OSTimeDlyHMSM(0, 0, 1, 0);
//            color = DISP_FGND_BLUE + DISP_BGND_BLUE;
//        }
//        else if (p == 2) color = DISP_FGND_BROWN + DISP_BGND_BROWN;
//        else if (p == 3) color = DISP_FGND_GREEN + DISP_BGND_GREEN;
//
//        // 출력
//        for (y = 0; y < 16; y++) {
//            for (x = 0; x < 80; x++) {
//                PC_DispChar(x, y + 5, ' ', color);
//            }
//        }
//        if (p == 2) {
//			for (count = 0; count < 1000000; count++);
//        }
//
//        // 출력 후
//        if (p == 0) OSTimeDlyHMSM(0, 0, 2, 0);
//		else if (p == 1) OSTimeDlyHMSM(0, 0, 1, 0);
//    }
//}

//void  Task(void* pdata) // 세마포어 사용 코드
//{
//    INT8U p = *(char*)pdata - '0';
//    INT8U color;
//    INT8U  x;
//    INT8U  y;
//    INT8U  err;
//
//    for (;;) {
//		// 출력 전
//        if (p == 0) color = DISP_FGND_RED + DISP_BGND_RED;
//        else if (p == 1) color = DISP_FGND_BLUE + DISP_BGND_BLUE;
//        else if (p == 2) color = DISP_FGND_BROWN + DISP_BGND_BROWN;
//        else if (p == 3) color = DISP_FGND_GREEN + DISP_BGND_GREEN;
//
//        OSSemPend(TaskSem[p], 0, &err);
//        // 출력
//        for (y = 0; y < 16; y++) {
//            for (x = 0; x < 80; x++) {
//                PC_DispChar(x, y + 5, ' ', color);
//            }
//        }
//        
//		// 출력 후
//        OSTimeDlyHMSM(0, 0, 1, 0);
//        OSSemPost(TaskSem[(p + 1) % N_TASKS]);
//    }
//}

// 3주차 과제
// 전역 변수로 final 선언하고 main에서 64로 초기화 해야 함
// task 갯수는 1개
//void  Task(void* pdata)
//{
//    INT8U i; // loop index
//
//    INT8U x = 0; // column
//    INT8U y = 5; // row
//
//    INT8U a; // temp variable for finding highest priority
//    INT8U b; // temp variable for finding highest priority
//    INT8U highestPrio; // highest priority
//
//    INT8U osRdyGrp;
//    INT8U osRdyTbl[8];
//
//    INT8U randomNum[4];
//    INT8U colorType = 0; // 0: red, 1: blue, 2: green, 3: brown
//
//    pdata = pdata;
//
//    for (;;) {
//        // 1. initialize Rdy table & Rdy group
//        osRdyGrp = 0;
//        for (i = 0; i < 8; i++) {
//            osRdyTbl[i] = 0;
//        }
//
//        // 2 ~ 3. get random numbers and set in Rdy table & Rdy group
//        for (i = 0; i < 4; i++) {
//            randomNum[i] = random(63);
//            osRdyGrp |= OSMapTbl[randomNum[i] >> 3];
//            osRdyTbl[randomNum[i] >> 3] |= OSMapTbl[randomNum[i] & 0x07];
//        }
//
//        // 4. find highest priority
//        b = OSUnMapTbl[osRdyGrp];
//        a = OSUnMapTbl[osRdyTbl[b]];
//        highestPrio = (b << 3) + a;
//
//        // 5. if highest priority is less than final, update final and display
//        if (highestPrio < final) {
//            final = highestPrio;
//
//            if (colorType == 0) {
//                PC_DispChar(x + 12, y, '0' + (highestPrio / 10), DISP_FGND_LIGHT_GRAY + DISP_BGND_RED);
//                PC_DispChar(x + 13, y, '0' + (highestPrio % 10), DISP_FGND_LIGHT_GRAY + DISP_BGND_RED);
//                colorType = 1;
//            }
//            else if (colorType == 1) {
//                PC_DispChar(x + 12, y, '0' + (highestPrio / 10), DISP_FGND_LIGHT_GRAY + DISP_BGND_BLUE);
//                PC_DispChar(x + 13, y, '0' + (highestPrio % 10), DISP_FGND_LIGHT_GRAY + DISP_BGND_BLUE);
//                colorType = 2;
//            }
//            else if (colorType == 2) {
//                PC_DispChar(x + 12, y, '0' + (highestPrio / 10), DISP_FGND_LIGHT_GRAY + DISP_BGND_GREEN);
//                PC_DispChar(x + 13, y, '0' + (highestPrio % 10), DISP_FGND_LIGHT_GRAY + DISP_BGND_GREEN);
//                colorType = 3;
//            }
//            else if (colorType == 3) {
//                PC_DispChar(x + 12, y, '0' + (highestPrio / 10), DISP_FGND_LIGHT_GRAY + DISP_BGND_BROWN);
//                PC_DispChar(x + 13, y, '0' + (highestPrio % 10), DISP_FGND_LIGHT_GRAY + DISP_BGND_BROWN);
//                colorType = 0;
//            }
//        }
//        // 6. initialize final if it is 0
//        if (final == 0) {
//            final = 64;
//        }
//
//        // 7. display random numbers
//        PC_DispChar(x, y, '0' + (randomNum[0] / 10), DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//        PC_DispChar(x + 1, y, '0' + (randomNum[0] % 10), DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//
//        PC_DispChar(x + 3, y, '0' + (randomNum[1] / 10), DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//        PC_DispChar(x + 4, y, '0' + (randomNum[1] % 10), DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//
//        PC_DispChar(x + 6, y, '0' + (randomNum[2] / 10), DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//        PC_DispChar(x + 7, y, '0' + (randomNum[2] % 10), DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//
//        PC_DispChar(x + 9, y, '0' + (randomNum[3] / 10), DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//        PC_DispChar(x + 10, y, '0' + (randomNum[3] % 10), DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//
//        y++;
//
//        // 8. if screen is full, clear and start over
//        if (y > 20 && x >= 60) {
//            OSTimeDlyHMSM(0, 0, 3, 0);
//
//            for (y = 0; y < 16; y++) {
//                for (x = 0; x < 80; x++) {
//                    PC_DispChar(x, y + 5, ' ', DISP_BGND_LIGHT_GRAY);
//                }
//            }
//
//            x = 0;
//            y = 5;
//            continue;
//        }
//        // 9. if row is full, next column
//        if (y > 20) {
//            y = 5;
//            x += 15;
//        }
//
//        // 10. wait a while before next loop
//        OSTimeDly(10);
//    }
//}

// weekd 4
//void Task(void* pdata) {
//    INT8U err;
//    INT8U i, j;
//
//    INT8U push_number;
//    int get_number[4];
//
//    INT8U min;
//    INT8U min_task;
//    int task_number = (int)(*(char*)pdata - 48);
//
//    char push_letter;
//    char get_letter;
//
//    int fgnd_color, bgnd_color;
//
//    char s[10];
//
//    // If pdata is 0-3, it's a random task, if it's 4, decision task.
//    if (*(char*)pdata == '4') { // decision task
//        for (;;) {
//            for (i = 0; i < N_TASKS - 1; i++) {
//                // Wait until the random number arrives. Store the value in the get_number array.
//                get_number[i] = *(int*)OSMboxPend(mbox_to_decision[i], 0, &err);
//            }
//
//            min = get_number[0];
//            min_task = 0;
//            for (i = 1; i < N_TASKS - 1; i++) {
//                // Find the smallest number among the 4 random numbers sent by the random tasks.
//                if (get_number[i] < min) {
//                    min = get_number[i];
//                    min_task = i;
//                }
//            }
//            for (i = 0; i < N_TASKS - 1; i++) {
//                if (i == min_task) {
//                    push_letter = 'W';
//                }
//                else {
//                    push_letter = 'L';
//                }
//
//                // Send W or L to the random task.
//                OSMboxPost(mbox_to_random[i], (void*)&push_letter);
//            }
//            OSTimeDlyHMSM(0, 0, 5, 0);
//        }
//    }
//    else { // random task
//        for (;;) {
//            push_number = random(64);
//            sprintf(s, "%2d", push_number);
//
//            PC_DispStr(0 + 18 * task_number, 4, "task", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//            PC_DispChar(4 + 18 * task_number, 4, *(char*)pdata, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//            PC_DispStr(6 + 18 * task_number, 4, s, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//
//            // Post to the decision task and wait with pend.
//            OSMboxPost(mbox_to_decision[task_number], (void*)&push_number);
//            get_letter = *(char*)OSMboxPend(mbox_to_random[task_number], 0, &err);
//
//            // Save background & foreground color based on task number
//            if (*(char*)pdata == '0') {
//                bgnd_color = DISP_BGND_RED;
//                fgnd_color = DISP_FGND_RED;
//            }
//            else if (*(char*)pdata == '1') {
//                bgnd_color = DISP_BGND_BROWN;
//                fgnd_color = DISP_FGND_BROWN;
//            }
//            else if (*(char*)pdata == '2') {
//                bgnd_color = DISP_BGND_BLUE;
//                fgnd_color = DISP_FGND_BLUE;
//            }
//            else if (*(char*)pdata == '3') {
//                bgnd_color = DISP_BGND_GREEN;
//                fgnd_color = DISP_FGND_GREEN;
//            }
//
//            PC_DispStr(8 + 18 * task_number, 4, "[", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//            PC_DispStr(10 + 18 * task_number, 4, "]", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//            PC_DispChar(9 + 18 * task_number, 4, get_letter, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//
//            // If W is received, fill the screen with the task's color.
//            if (get_letter == 'W') {
//                for (j = 5; j < 20; j++) {
//                    for (i = 0; i < 80; i++) {
//                        PC_DispChar(i, j, ' ', fgnd_color + bgnd_color);
//                    }
//                }
//            }
//            OSTimeDlyHMSM(0, 0, 5, 0);
//        }
//    }
//}

// week 5
//void Task(void* pdata) {
//    INT8U err; // error variable
//
//    INT8U i, j;
//
//    INT8U min;		// Variable to store the minimum value
//    INT8U min_task;	// Variable to store which task has the minimum value
//    int task_number = (int)(*(char*)pdata - 48); // index of each task (Since pdata is of char type, subtracting 48 (ASCII '0') converts it to an int)
//
//    char get_letter; // Variable to store 'W' or 'L' 
//
//    int fgnd_color, bgnd_color; // Variables to store foreground and background colors
//
//    char s[10]; // String variable for displaying numbers
//
//    // If pdata is 0-3, it's a random task, if it's 4, decision task.
//    if (*(char*)pdata == '4') { // decision task
//        for (;;) {
//            min = send_array[0];
//            min_task = 0;
//            for (i = 1; i < N_TASKS - 1; i++) {
//                // Find the smallest number among the 4 random numbers sent by the random tasks.
//                if (send_array[i] < min) {
//                    min = send_array[i];
//                    min_task = i;
//                }
//            }
//            for (i = 0; i < N_TASKS - 1; i++) {
//                OSSemPend(RandomSem, 0, &err);
//                if (i == min_task) {
//                    receive_array[i] = 'W';
//                }
//                else {
//                    receive_array[i] = 'L';
//                }
//                OSSemPost(RandomSem);
//            }
//            // Notify all random tasks that the decision has been made.
//            OSFlagPost(r_grp, 0x0F, OS_FLAG_SET, &err);
//            OSTimeDlyHMSM(0, 0, 5, 0);
//        }
//    }
//    else { // random task
//        for (;;) {
//            OSSemPend(RandomSem, 0, &err);
//            send_array[task_number] = random(64);
//            OSSemPost(RandomSem);
//
//            sprintf(s, "%2d", send_array[task_number]);
//
//            PC_DispStr(0 + 18 * task_number, 4, "task", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//            PC_DispChar(4 + 18 * task_number, 4, *(char*)pdata, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//            PC_DispStr(6 + 18 * task_number, 4, s, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//
//            // Post to the decision task and wait with pend.
//            OSFlagPend(r_grp, 0x01 << task_number, OS_FLAG_WAIT_SET_ALL + OS_FLAG_CONSUME, 0, &err);
//            get_letter = receive_array[task_number];
//
//            if (*(char*)pdata == '0') {
//                bgnd_color = DISP_BGND_RED;
//                fgnd_color = DISP_FGND_RED;
//            }
//            else if (*(char*)pdata == '1') {
//                bgnd_color = DISP_BGND_BROWN;
//                fgnd_color = DISP_FGND_BROWN;
//            }
//            else if (*(char*)pdata == '2') {
//                bgnd_color = DISP_BGND_BLUE;
//                fgnd_color = DISP_FGND_BLUE;
//            }
//            else if (*(char*)pdata == '3') {
//                bgnd_color = DISP_BGND_GREEN;
//                fgnd_color = DISP_FGND_GREEN;
//            }
//            PC_DispStr(8 + 18 * task_number, 4, "[", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//            PC_DispStr(10 + 18 * task_number, 4, "]", DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//            PC_DispChar(9 + 18 * task_number, 4, get_letter, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
//
//            if (get_letter == 'W') {
//                for (j = 5; j < 20; j++) {
//                    for (i = 0; i < 80; i++) {
//                        PC_DispChar(i, j, ' ', fgnd_color + bgnd_color);
//                    }
//                }
//            }
//            OSTimeDlyHMSM(0, 0, 5, 0);
//        }
//    }
//}