
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <stdbool.h>
#include <time.h>

//#include <winsock2.h>


// Global deffinitions
DCB dcbSerial;        // link to Windows Serial interface
DCB dcbSerial1;
HANDLE hSerialLEFT;       // file handle for serial comms           
HANDLE hSerialRIGHT;
int temp;
bool stop = false;
char cmdBuf[6];       // The buffer to be sent to the controller


int connectToCOM(void);
int setUpCOM(void);
int setServoPos(int, int);
int constrain(int, int, int);
int scaleServoRange(int, int);

int i; // used for C66 style

int servoRanges[32][2] = 
                    { 0, 0,
                      4500, 2700,
                      4000, 1400,
                      4500, 1400,
                      4200, 2000,
                      4000, 1000, //thumb
                      4000, 1000,
                      3500, 1000,
                      3500, 1000,
                      4000, 1200,
                      1500, 3500,
                      4500, 1500,
                      2600, 3700,
                      1000, 1900,
                      2000, 4000,
                      2500, 2500,
             
                      2700, 3600,
                      4500, 2500,
                      4500, 1500,
                      4500, 1500,
                      4000, 1800,
                      4000, 1500, //thumb
                      4500, 1500,
                      4200, 1400,
                      4500, 1500,
                      4500, 1500, 
                      1000, 3800,
                      4500, 1500, //temp
                      2600, 3700, //temp
                      1000, 1900, //temp
                      2000, 4000, //temp
                      0, 0 };


enum servosRanges{ 
             LS1min = 4500,
             LS1max = 2700,
             LS2min = 4000,
             LS2max = 1400,
             LS3min = 4500,
             LS3max = 1400,
             LS4min = 4200,
             LS4max = 2000,
             LS5min = 2400,
             LS5max = 500,
             LS6min = 3000,
             LS6max = 500,
             LS7min = 2500,
             LS7max = 500,
             LS8min = 2400,
             LS8max = 500,
             LS9min = 2800,
             LS9max = 500,
             LS10min = 1500,
             LS10max = 3500,
             LS11min = 4500,
             LS11max = 1500,
             LS12min = 2600,
             LS12max = 3700,
             LS13min = 1000,
             LS13max = 1900,
             LS14min = 2000,
             LS14max = 4000,
             LS15min = 2500,
             LS15max = 2500,
             
             RS0min = 2700,
             RS0max = 3600,
             RS1min = 4500,
             RS1max = 2500,
             RS2min = 4500,
             RS2max = 1500,
             RS3min = 4500,
             RS3max = 1500,
             RS4min = 3900,
             RS4max = 1900,
             RS5min = 2300,
             RS5max = 1000,
             RS6min = 2500,
             RS6max = 500,
             RS7min = 2500,
             RS7max = 500,
             RS8min = 2500,
             RS8max = 500,
             RS9min = 2500,
             RS9max = 500, 
             RS10min = 1000,
             RS10max = 3800,
             RS11 = 0,
             RS12 = 0,
             RS13 = 0,
             RS14 = 0,
             RS15 = 0};
             
enum servos{ S0 = 0,
             S1 = 1,
             S2 = 2,
             S3 = 3,
             S4 = 4,
             S5 = 5,
             S6 = 6,
             S7 = 7,
             S8 = 8,
             S9 = 9,
             S10 = 10,
             S11 = 11,
             S12 = 12,
             S13 = 13,
             S14 = 14,
             S15 = 15,
             S16 = 16,
             S17 = 17,
             S18 = 18,
             S19 = 19,
             S20 = 20,
             S21 = 21,
             S22 = 22,
             S23 = 23,
             S24 = 24,
             S25 = 25,
             S26 = 26,
             S27 = 27,
             S28 = 28,
             S29 = 29,
             S30 = 30,
             S31 = 31,
             S32 = 32};

int main(int argc, char *argv[])
{
  char option[10];
  char option1[10];
  int theServo = -1;
  int servoPos = 500;
  connectToCOM();
  setUpCOM();
  int tempCount = 0;
  int tempCount1 = 0;
  
  printf("Comp port configured.\n");
  //system("PAUSE");	
 /*
  while (true)
  {
    if (theServo == -1)
    {
      printf("Select a servo (0-31): ");
      gets(option1);
      theServo = atoi(option1);    
    }
    printf("Enter servo pos (1-100%): ");
    gets(option);
    servoPos = atoi(option);
    if(servoPos == -1)
    {
      theServo = -1;
    }
    else
    {
      //setServoPos(servoPos, theServo);
      setServoPos(scaleServoRange(servoPos, theServo), theServo);
      printf("DEBUG: %d\n", scaleServoRange(servoPos, theServo));
    }
  }
   */
 
//   MessageBox(NULL, "sdlfj", "Test", 0);
  
  while (true)
  {
    printf("Select a command: ");
    gets(option);
    printf("You entered: %s\n", option);

    if(strcmp(option,"look") == 0)
    {
      printf("LOOKING\n");
      moveLook();
    }
    else if(strcmp(option,"shakeHands") == 0)
    {
        printf("Shaking hands\n");
        moveShakeHands();
    }
    else if(strcmp(option,"cutRibbon") == 0)
    {
        printf("Cutting a Ribbon\n");
        cutRibbon();
    }
    else if(strcmp(option,"talk") == 0)
    {
        printf("Talking\n");
        moveTalk();
    }
    else if(strcmp(option,"point") == 0)
    {
        printf("Pointing\n");
        movePoint();
    }
    else if(strcmp(option,"pride") == 0)
    {
        printf("Showing Pride\n");
        movePride();
    }
    else if(strcmp(option,"paper") == 0)
    {
        printf("Playing Paper\n");
        movePaper();
    }
    else if(strcmp(option,"rock") == 0)
    {
        printf("Playing Rock\n");
        moveRock();
    }
    else if(strcmp(option,"scissors") == 0)
    {
        printf("Playing Scissors\n");
        moveScissors();
    }
    else if(strcmp(option,"handOpenR") == 0)
    {
        printf("Opening Right Hand\n");
        moveHandOpenR();
    }
    else if(strcmp(option,"handOpenL") == 0)
    {
        printf("Opening Left Hand\n");
        moveHandOpenL();
    }
    else if(strcmp(option,"handCloseR") == 0)
    {
        printf("Closing Hand\n");
        moveHandClose();
    }
    else if(strcmp(option,"handCloseL") == 0)
    {
        printf("Closing Hand\n");
        moveHandCloseL();
    }
    
    else if(strcmp(option,"handRepeat") == 0)
    {
        printf("Closing Hand and Opening\n");
        moveHandRepeat();
    }
    else if(strcmp(option,"game") == 0)
    {
        printf("Playing Paper, Rock, Scissors\n");
        playGame();
    }
    else if(strcmp(option,"lookHand") == 0)
    {
        printf("Looking at hand\n");
        moveLookHand();
    }
    else if(strcmp(option,"countHand") == 0)
    {
        printf("Counting Single Hand\n");
        moveSingleHandCount();
    }
    else if(strcmp(option,"swear") == 0)
    {
        printf("Abuse\n");
        swear();
    }
    else if(strcmp(option,"default") == 0)
    {
        printf("Resetting...\n");
        moveDefault();
    }

    else if(strcmp(option, "joy") == 0)
    {
        printf("Move Joy\n");
        moveJoy();
    }
    else if(strcmp(option, "help") == 0)
    {
         displayCommands();
    }
    else if(strcmp(option,"holdButton") == 0)
    {
        printf("Holding Button\n");
        moveHoldButton();
    }
    else if(strcmp(option,"cheer") == 0)
    {
        printf("Cheering\n");
        moveCheer();
    }
    else if(strcmp(option,"pressButton") == 0)
    {
        printf("Pressing Button\n");
        movePressButton();
    }
    else if(strcmp(option,"thumbsUp") == 0)
    {
        printf("Thumbs Up!\n");
        moveThumbsUp();
    }
    else if(strcmp(option,"testServo") == 0)
    {
        printf("Pressing Button\n");
        testServoPos();
    }
    else if(strcmp(option,"shrug") == 0)
    {
        printf("Shrugging\n");
        moveShrug();
    }
    else if(strcmp(option,"0") == 0)
    {
        printf("init...\n");
        moveInit();
    }
    else if(strcmp(option,"wave") == 0)
    {
        printf("Waving\n");
        moveWave();
    }
    else if(strcmp(option,"exit") == 0)
    {
        printf("Exiting\n");
        exit(0);
    }
  }

  return 0;
}

void wait() {
    printf( "\n" );
    printf( "Press the ENTER key...\n" );
    fflush( stdout );
    getc( stdin );
}

int moveLoseGame( void )
{
    moveHandOpenR( );
    setServoPos(scaleServoRange(60, 18), 18);
    setServoPos(scaleServoRange(15, 19), 19);
    setServoPos(scaleServoRange(100, 20), 20);    
    sleep(2000);
    setServoPos(scaleServoRange(20, 27), 27);
    setServoPos(scaleServoRange(50, 28), 28);
    moveHandClose( );
    sleep(6000);
    moveDefault();
}

int movePointGesture()
{
    setServoPos(scaleServoRange(100, 27), 27);
    setServoPos(scaleServoRange(50, 28), 28);
    setServoPos(scaleServoRange(90, 3), 3);
    setServoPos(scaleServoRange(90, 4), 4);
    sleep(1000);
}

int moveInit()
{
    setServoPos(scaleServoRange(100, 27), 27);  //head backward 
    setServoPos(scaleServoRange(45, 28), 28);  //head backward 
    sleep(300);
}

int moveJoy( void )
{
    setServoPos(scaleServoRange(100, 27), 27);  //head backward 
    setServoPos(scaleServoRange(100, 2), 2);   // left arm raised
    setServoPos(scaleServoRange(80, 18), 18); 	// right arm raised 
    setServoPos(scaleServoRange(80, 4), 4);		//left elbow straight 
  	setServoPos(scaleServoRange(70, 20), 20);	//right elbow straight
    sleep(4000);

	
	moveHandOpenBoth();
    moveHandCloseBoth();
	moveHandOpenBoth();
    moveHandCloseBoth();
	moveHandOpenBoth();
    moveHandCloseBoth();
	moveHandOpenBoth();
    moveHandCloseBoth();
    sleep(2500);
    
    moveDefault( );
}

/*
DWORD WINAPI ThreadFuncSayITV4(void* data)
{
    system("sounder.exe ITV_Sound_4.wav");
    return 0;
}

int say1()
{
    //system("sounder.exe introduction1.wav");
    HANDLE thread = CreateThread(NULL, 0, ThreadFuncSay1, NULL, 0, NULL);
    //moveTalk(6);

    return 0;
}
*/

int displayCommands(void)
{
    system("cls");
    printf("Commands are:\n\n");
    printf("'pride'\t\t - \t Makes the robot display a 'pride' emotion pose.\n");
    printf("'shakeHands'\t - \t The robot shakes hands with someone.\n");
    printf("'game'\t\t - \t The Robot plays a game of Paper, Rock, Scissors.\n");
    printf("'handCloseL'\t - \t Robot closes LEFT hand.\n");
    printf("'handCloseR'\t - \t Robot closes RIGHT hand.\n");
    
}

int playGame( void )
{
    sleep(2000);
    int i = 0;
    
    for (i = 0; i < 3; i++)
    {
        printf("Playing round %d\n", i+1);
        int option = rand() % 3 + 1;
        if (option == 1)
        {
            printf("Robot playing Paper\n");
            movePaper();
        }
        else if (option == 2)
        {
            printf("Robot playing Rock\n");
            moveRock();
        }
        else if (option == 3)
        {
            printf("Robot playing Scissors.\n");
            moveScissors();
        }
        sleep(500);
    }
}

int moveThumbsUp()
{
    setServoPos(scaleServoRange(85, 2), 2);  //head backward 
    setServoPos(scaleServoRange(60, 18), 18);  //head backward 
    setServoPos(scaleServoRange(30, 19), 19);  //head backward 
    setServoPos(scaleServoRange(30, 3), 3);  //head backward 
    setServoPos(scaleServoRange(70, 20), 20);  //head backward 
    setServoPos(scaleServoRange(60, 4), 4);  //head backward 
    
    setServoPos(scaleServoRange(90, 5), 5);  //head backward 
    setServoPos(scaleServoRange(10, 6), 6);  //head backward 
    setServoPos(scaleServoRange(10, 7), 7);  //head backward 
    setServoPos(scaleServoRange(10, 8), 8);  //head backward 
    setServoPos(scaleServoRange(10, 9), 9);  //head backward 
    setServoPos(scaleServoRange(90, 21), 21);  //head backward 
    setServoPos(scaleServoRange(10, 22), 22);  //head backward 
    setServoPos(scaleServoRange(10, 23), 23);  //head backward 
    setServoPos(scaleServoRange(10, 24), 24);  //head backward 
    setServoPos(scaleServoRange(10, 25), 25);  //head backward 
    
    sleep(7000);
    moveDefault( );
}

int moveWave( )
{
    moveHandOpenL( );
    setServoPos(scaleServoRange(65, 1), 1);
    setServoPos(scaleServoRange(70, 4), 4);
    setServoPos(scaleServoRange(85, 3), 3);
    sleep(3000);
    setServoPos(scaleServoRange(60, 28), 28);
    sleep(600);
    setServoPos(scaleServoRange(90, 4), 4);
    sleep(600);
    setServoPos(scaleServoRange(50, 4), 4);
    sleep(600);
    setServoPos(scaleServoRange(90, 4), 4);
    sleep(600);
    setServoPos(scaleServoRange(50, 4), 4);
    sleep(600);
    setServoPos(scaleServoRange(90, 4), 4);
    sleep(600);
    setServoPos(scaleServoRange(50, 4), 4);
    sleep(1000);
    moveDefault();

}

int moveShrug()
{
  moveHandOpenR( );
  moveHandOpenL( );  
  setServoPos(scaleServoRange(90, 3), 3);    
  setServoPos(scaleServoRange(90, 19), 19);
  setServoPos(scaleServoRange(90, 4), 4);
  setServoPos(scaleServoRange(90, 20), 20);
  setServoPos(scaleServoRange(40, 1), 1);
  setServoPos(scaleServoRange(40, 17), 17);
  sleep(2000);
  setServoPos(scaleServoRange(30, 27), 27);
  setServoPos(scaleServoRange(90, 28), 28);
  
  sleep(3000);
  moveDefault();
  
}


int moveCheer()
{
  
  moveDefault( );  
}

int movePride( void )
{
    setServoPos(scaleServoRange(80, 27), 27);  //head backward 
    setServoPos(scaleServoRange(20, 28), 28);  //head tilt 
    //sleep(500);
	
	moveHandOpenR( );
	moveHandOpenL( );
	
	setServoPos(scaleServoRange(55, 1), 1);   // left arm raised
	setServoPos(scaleServoRange(55, 17), 17);   // left arm raised
    setServoPos(scaleServoRange(0, 3), 3);   // left arm raised
	setServoPos(scaleServoRange(0, 19), 19);   // left arm raised
    setServoPos(scaleServoRange(90, 4), 4);   // left arm raised
	setServoPos(scaleServoRange(90, 20), 20);   // left arm raised
	sleep(8000);
   	
   	moveDefault( );
}

int moveDefault( void )
{
    
    setServoPos(scaleServoRange(40, 28), 28); // Left right 
    setServoPos(scaleServoRange(70, 27), 27); // Up down
    setServoPos(scaleServoRange(10, 29), 29); // Up down
    setServoPos(scaleServoRange(50, 26), 26); 
    setServoPos(scaleServoRange(80, 4), 4);
    sleep(500);
    // Left Side
    setServoPos(scaleServoRange(5, 1), 1);
    setServoPos(scaleServoRange(40, 2), 2);
    setServoPos(scaleServoRange(55, 3), 3);
    setServoPos(scaleServoRange(20, 4), 4);
    
    //Right Side
	setServoPos(scaleServoRange(10, 17), 17);
    setServoPos(scaleServoRange(20, 18), 18);
    setServoPos(scaleServoRange(50, 19), 19);
    setServoPos(scaleServoRange(20, 20), 20);
    
    sleep(3000);
    moveHandOpenR();
	moveHandOpenL();
    
    
    setServoOffAll();
}

int setServoOffAll()
{
    setServoOff(1);
    setServoOff(2);
    setServoOff(3);
    setServoOff(4);
    setServoOff(5);
    sleep(200);
    setServoOff(6);
    setServoOff(7);
    setServoOff(8);
    setServoOff(9);
    setServoOff(10);
    setServoOff(17);
    setServoOff(18);
    sleep(200);
    setServoOff(19);
    setServoOff(20);
    setServoOff(27);
    setServoOff(28);
    setServoOff(29);
}

int moveHandOpenL( void )
{
    setServoPos(scaleServoRange(100, 5), 5);
    setServoPos(scaleServoRange(100, 6), 6);
    setServoPos(scaleServoRange(100, 7), 7);
    setServoPos(scaleServoRange(100, 8), 8);
    setServoPos(scaleServoRange(100, 9), 9);
    sleep(1000);        
}

int moveHandOpenBoth()
{
    setServoPos(scaleServoRange(100, 5), 5);
    setServoPos(scaleServoRange(100, 6), 6);
    setServoPos(scaleServoRange(100, 7), 7);
    setServoPos(scaleServoRange(100, 8), 8);
    setServoPos(scaleServoRange(100, 9), 9);

    setServoPos(scaleServoRange(100, 21), 21);
    setServoPos(scaleServoRange(100, 22), 22);
    setServoPos(scaleServoRange(100, 23), 23);
    setServoPos(scaleServoRange(100, 24), 24);
    setServoPos(scaleServoRange(100, 25), 25);
    sleep(500);

}


moveHandCloseBoth()
{
    setServoPos(scaleServoRange(10, 21), 21);
    setServoPos(scaleServoRange(10, 22), 22);
    setServoPos(scaleServoRange(10, 23), 23);
    setServoPos(scaleServoRange(10, 24), 24);
    setServoPos(scaleServoRange(10, 25), 25);

    setServoPos(scaleServoRange(10, 5), 5);
    setServoPos(scaleServoRange(10, 6), 6);
    setServoPos(scaleServoRange(10, 7), 7);
    setServoPos(scaleServoRange(10, 8), 8);
    setServoPos(scaleServoRange(10, 9), 9);
    sleep(500);
}


int moveLookHand( void )
{
    moveHandOpenR( );
    setServoPos(scaleServoRange(30, 17), 17);
    setServoPos(scaleServoRange(60, 18), 18);
    setServoPos(scaleServoRange(20, 19), 19);
    setServoPos(scaleServoRange(90, 20), 20);
    sleep(4000);
    setServoPos(scaleServoRange(60, 19), 19);
    sleep(3000);
    setServoPos(scaleServoRange(60, 28), 28);
    sleep(500);
    
    sleep(1000);
    
    setServoPos(scaleServoRange(17, 17), 17);
    setServoPos(scaleServoRange(30, 18), 18);
    setServoPos(scaleServoRange(40, 19), 19);
    setServoPos(scaleServoRange(50, 18), 20);
    setServoPos(scaleServoRange(40, 28), 28);
    sleep(4000);

    setServoOff(17);
    setServoOff(18);
    setServoOff(19);
    setServoOff(20);    
    setServoOff(28);    

}

int moveSingleHandCount( void )
{
    setServoPos(scaleServoRange(30, 17), 17);
    setServoPos(scaleServoRange(60, 18), 18);
    setServoPos(scaleServoRange(20, 19), 19);
    setServoPos(scaleServoRange(90, 20), 20);
    sleep(4000);
    
    setServoPos(scaleServoRange(20, 21), 21);
    sleep(500);
    setServoPos(scaleServoRange(10, 22), 22);
    sleep(500);
    setServoPos(scaleServoRange(10, 23), 23);
    sleep(500);
    setServoPos(scaleServoRange(10, 24), 24);
    sleep(500);
    setServoPos(scaleServoRange(10, 25), 25);
    sleep(500);
    setServoPos(scaleServoRange(90, 25), 25);
    sleep(500);
    setServoPos(scaleServoRange(90, 24), 24);
    sleep(500);
    setServoPos(scaleServoRange(90, 23), 23);
    sleep(500);
    setServoPos(scaleServoRange(90, 22), 22);
    sleep(500);
    setServoPos(scaleServoRange(90, 21), 21);
    sleep(500);
    setServoPos(scaleServoRange(20, 21), 21);
    sleep(500);
    setServoPos(scaleServoRange(10, 22), 22);
    sleep(500);
    setServoPos(scaleServoRange(10, 23), 23);
    sleep(500);
    setServoPos(scaleServoRange(10, 24), 24);
    sleep(500);
    setServoPos(scaleServoRange(10, 25), 25);
    sleep(500);
    setServoPos(scaleServoRange(90, 25), 25);
    sleep(500);
    setServoPos(scaleServoRange(90, 24), 24);
    sleep(500);
    setServoPos(scaleServoRange(90, 23), 23);
    sleep(500);
    setServoPos(scaleServoRange(90, 22), 22);
    sleep(500);
    setServoPos(scaleServoRange(90, 21), 21);
    sleep(500);
    
    
    moveHandOpenR();
    sleep(1000);

// reset pos    
    setServoPos(scaleServoRange(17, 17), 17);
    setServoPos(scaleServoRange(30, 18), 18);
    setServoPos(scaleServoRange(40, 19), 19);
    setServoPos(scaleServoRange(50, 20), 20);
    setServoPos(scaleServoRange(40, 28), 28);
    sleep(4000);

    setServoOff(17);
    setServoOff(18);
    setServoOff(19);
    setServoOff(20);    
    setServoOff(28);    

    
}

int swear( void )
{   
    setServoPos(scaleServoRange(10, 17), 17);
    setServoPos(scaleServoRange(70, 18), 18);
    setServoPos(scaleServoRange(40, 19), 19);
    setServoPos(scaleServoRange(80, 20), 20);
    sleep(4000);
    setServoPos(scaleServoRange(20, 21), 21);
    setServoPos(scaleServoRange(10, 22), 22);
    setServoPos(scaleServoRange(90, 23), 23);
    setServoPos(scaleServoRange(10, 24), 24);
    setServoPos(scaleServoRange(10, 25), 25);
    sleep(2000);
    // reset pos    
    setServoPos(scaleServoRange(17, 17), 17);
    setServoPos(scaleServoRange(30, 18), 18);
    setServoPos(scaleServoRange(40, 19), 19);
    setServoPos(scaleServoRange(50, 18), 20);
    setServoPos(scaleServoRange(40, 28), 28);
    sleep(4000);

    setServoOff(17);
    setServoOff(18);
    setServoOff(19);
    setServoOff(20);    
    setServoOff(28);    
    moveHandOpenR();

}

int cutRibbon(void)
{
    setServoPos(scaleServoRange(5, 1), 1);
    setServoPos(scaleServoRange(70, 2), 2);
    sleep(2000);
    setServoPos(scaleServoRange(10, 28), 28);
    setServoPos(scaleServoRange(30, 27), 27);
    sleep(2000);
    
    setServoPos(scaleServoRange(1, 6), 6);
    sleep(1000);
    setServoPos(scaleServoRange(50, 28), 28);
    setServoPos(scaleServoRange(80, 27), 27);
    sleep(4000);
    
    setServoPos(scaleServoRange(80, 6), 6);
    setServoPos(scaleServoRange(40, 2), 2);
    sleep(2000);
    
    setServoOff(1);
    setServoOff(2);
    setServoOff(28);
    setServoOff(27);
    
    
}

int testServoPos(void)
{
    setServoPos(scaleServoRange(90, 27), 27);
    sleep(1000);
    setServoOff(27);
}

int moveLook( void )
{
    setServoPos(scaleServoRange(20, 28), 28);
    sleep(100);
    
    for(i = 20; i < 80; i++)
    {
        setServoPos(scaleServoRange(i, 28), 28);
        sleep(10);
    }
    setServoPos(scaleServoRange(40, 27), 27);
    sleep(500);
    for(i = 80; i > 20; i--)
    {
        setServoPos(scaleServoRange(i, 28), 28);
        sleep(10);
    }
    
    setServoPos(scaleServoRange(70, 27), 27);
    sleep(200);
    setServoPos(scaleServoRange(40, 28), 28);
    sleep(200);
    setServoOff(28);
    setServoOff(27);
}

int moveTalk( int numSeconds )
{
    unsigned int x_seconds = 0;
    unsigned int x_milliseconds = 0;
    unsigned int time_left = 0;
    
    unsigned int count_down_time = 0;
    int i = 0;
    
    clock_t x_startTime, x_countTime;
    count_down_time = numSeconds;
    
    x_startTime = clock();
    
    time_left = count_down_time - x_seconds;
    //for(i = 0; i < 10; i++)
    while(time_left > 0)
    {
      setServoPos(scaleServoRange(90, 29), 29);
      sleep(400);
      setServoPos(scaleServoRange(5, 29), 29);
      sleep(400);
      
      x_countTime = clock();
      x_milliseconds = x_countTime - x_startTime;
      x_seconds = (x_milliseconds/(CLOCKS_PER_SEC));
      time_left = count_down_time - x_seconds;
    }
    
    setServoOff(29);
}

int moveShakeHands( void )
{
    setServoPos(scaleServoRange(20, 17), 17);
    setServoPos(scaleServoRange(35, 18), 18);
    setServoPos(scaleServoRange(35, 19), 19);
    setServoPos(scaleServoRange(70, 20), 20); //elbow
    sleep(3000);
    
    setServoPos(scaleServoRange(50, 21), 21);
    setServoPos(scaleServoRange(50, 22), 22);
    setServoPos(scaleServoRange(60, 23), 23);
    setServoPos(scaleServoRange(50, 24), 24);
    setServoPos(scaleServoRange(40, 25), 25);
    sleep(1000);
    
    setServoPos(scaleServoRange(50, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(70, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(50, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(70, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(50, 20), 20); //elbow
    sleep(500);
    
    setServoPos(scaleServoRange(90, 21), 21);
    setServoPos(scaleServoRange(90, 22), 22);
    setServoPos(scaleServoRange(90, 23), 23);
    setServoPos(scaleServoRange(90, 24), 24);
    setServoPos(scaleServoRange(90, 25), 25);
    sleep(1000);
    
    setServoPos(scaleServoRange(20, 17), 17);
    setServoPos(scaleServoRange(20, 18), 18);
    setServoPos(scaleServoRange(45, 19), 19);
    setServoPos(scaleServoRange(70, 20), 20); //elbow
    sleep(3000);
        
    setServoOff(17);
    setServoOff(18);
    setServoOff(19);
    setServoOff(20);
}

int SpeechMove_1( void ) 
{
	moveHandOpenR();
	moveHandOpenL();
	sleep(200);
	setServoOff(21);
	setServoOff(22);
	setServoOff(23);
	setServoOff(24);
	setServoOff(25);
	setServoOff(5);
	setServoOff(6);
	setServoOff(7);
	setServoOff(8);
	setServoOff(9);
	
    setServoPos(scaleServoRange(10, 27), 27);  //head backward 
    setServoPos(scaleServoRange(60, 28), 28);  //head tilt 
	setServoPos(scaleServoRange(60, 18), 18); 	// right arm raised 
    setServoPos(scaleServoRange(10, 19), 19);	//right arm forward
	setServoPos(scaleServoRange(98, 20), 20);	//right elbow folded 
    
	setServoPos(scaleServoRange(50, 2), 2);   // left arm bit forward
    setServoPos(scaleServoRange(10, 3), 3);   // left forearm towards right arm's elbow
    setServoPos(scaleServoRange(95, 4), 4);
    Sleep(6000);
	
	//MoveTalk(1000);
	
	setServoOff(28);
	setServoOff(27);
    setServoOff(18);
	setServoOff(19);
	setServoOff(20);
	setServoOff(2);
}

int moveHandRepeat( void )
{
    int i = 1;
    for (i = 0; i < 5; i++)
    {
        moveHandClose();
        moveHandOpenR();
    }
}

int moveHandOpenR( void )
{
    setServoPos(scaleServoRange(100, 21), 21);
    setServoPos(scaleServoRange(100, 22), 22);
    setServoPos(scaleServoRange(100, 23), 23);
    setServoPos(scaleServoRange(100, 24), 24);
    setServoPos(scaleServoRange(100, 25), 25);
    sleep(1000);
}

int moveHandClose( void )
{
    setServoPos(scaleServoRange(10, 21), 21);
    setServoPos(scaleServoRange(10, 22), 22);
    setServoPos(scaleServoRange(10, 23), 23);
    setServoPos(scaleServoRange(10, 24), 24);
    setServoPos(scaleServoRange(10, 25), 25);
    sleep(1000);
}

int moveHandCloseL( void )
{
    setServoPos(scaleServoRange(10, 5), 5);
    setServoPos(scaleServoRange(10, 6), 6);
    setServoPos(scaleServoRange(10, 7), 7);
    setServoPos(scaleServoRange(10, 8), 8);
    setServoPos(scaleServoRange(10, 9), 9);
    sleep(1000);
    setServoOff(5);
    setServoOff(6);
    setServoOff(7);
    setServoOff(8);
    setServoOff(9);
}

int moveHandLGrip( void )
{
    setServoPos(scaleServoRange(10, 5), 5);
    setServoPos(scaleServoRange(40, 6), 6);
    setServoPos(scaleServoRange(35, 7), 7);
    setServoPos(scaleServoRange(35, 8), 8);
    setServoPos(scaleServoRange(10, 9), 9);
    sleep(1000);
    setServoOff(5);
    setServoOff(9);
}


int movePaper(void)
{
    setServoPos(scaleServoRange(50, 27), 27);  //head backward 
    setServoPos(scaleServoRange(15, 28), 28);  //head backward 
    sleep(300);
    
    setServoPos(scaleServoRange(20, 21), 21); // Thumb
    setServoPos(scaleServoRange(10, 22), 22);
    setServoPos(scaleServoRange(10, 23), 23);
    setServoPos(scaleServoRange(10, 24), 24);
    setServoPos(scaleServoRange(10, 25), 25);

    setServoPos(scaleServoRange(20, 17), 17);
    setServoPos(scaleServoRange(50, 18), 18);
    setServoPos(scaleServoRange(35, 19), 19);
    setServoPos(scaleServoRange(50, 20), 20); //elbow
    sleep(3000);
    
    setServoPos(scaleServoRange(90, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(50, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(90, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(50, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(90, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(60, 20), 20);
    
    setServoPos(scaleServoRange(100, 21), 21);
    setServoPos(scaleServoRange(100, 22), 22);
    setServoPos(scaleServoRange(100, 23), 23);
    setServoPos(scaleServoRange(100, 24), 24);
    setServoPos(scaleServoRange(100, 25), 25);
    sleep(2000);
    
    setServoPos(scaleServoRange(20, 17), 17);
    setServoPos(scaleServoRange(20, 18), 18);
    setServoPos(scaleServoRange(45, 19), 19);
    setServoPos(scaleServoRange(70, 20), 20); //elbow
    sleep(2500);
        
    setServoOff(17);
    setServoOff(18);
    setServoOff(19);
    setServoOff(20);
}

int moveHoldButton(void)
{
    moveHandLGrip();
}

int movePressButton(void)
{

    setServoPos(scaleServoRange(40, 28), 28); // Head Left/Right 
    setServoPos(scaleServoRange(90, 27), 27); // Head Up/Down
    
    moveHandClose();

    setServoPos(scaleServoRange(10, 27), 27); // Head Up/Down

    setServoPos(scaleServoRange(10, 17), 17);  // Right Lift
    setServoPos(scaleServoRange(55, 18), 18);  // Right Forwards/Backwards
    setServoPos(scaleServoRange(15, 19), 19);  // Right Rotate
    setServoPos(scaleServoRange(100, 20), 20);  // Right Elbow
    setServoPos(scaleServoRange(50, 26), 26);  // Right Wrist

    sleep(2000);

    setServoPos(scaleServoRange(10, 1), 1);  // Left Lift
    setServoPos(scaleServoRange(56, 2), 2);  // Left Forwards/Backwards
    setServoPos(scaleServoRange(32, 3), 3);  // Left Rotate
    setServoPos(scaleServoRange(92, 4), 4);  // Left Elbow
    setServoPos(scaleServoRange(20, 10), 10); // Left Wrist
    sleep(6000);
    
    // MAKE SPEECH //
    setServoPos(scaleServoRange(90, 27), 27); // Head Up/Down
    sleep(500);
    
    // MAKE PRESS BUTTON
    setServoPos(scaleServoRange(10, 27), 27); // Head Up/Down
    setServoPos(scaleServoRange(5, 17), 17);  // Right Lift
    setServoPos(scaleServoRange(6, 19), 19);  // Right Rotate
    sleep(700);
    setServoPos(scaleServoRange(100, 4), 4);
    setServoPos(scaleServoRange(67, 2), 2);
    
    sleep(4000);
    setServoPos(scaleServoRange(30, 2), 2);  // Right Lift
    
    sleep(2500);
    setServoPos(scaleServoRange(25, 19), 19);  // Right Rotate
    setServoPos(scaleServoRange(90, 27), 27); // Head Up/Down
    
    
    
    sleep(5500);
    moveDefault( );
    
    setServoOff(1);
    setServoOff(2);
    setServoOff(3);
    setServoOff(4);
    setServoOff(10);
    
    setServoOff(17);
    setServoOff(18);
    setServoOff(19);
    setServoOff(20);
    setServoOff(26);
    setServoOff(27);
    setServoOff(28);
    setServoOff(29);
    
}


int moveScissors(void)
{
    setServoPos(scaleServoRange(50, 27), 27);  //head backward 
    setServoPos(scaleServoRange(15, 28), 28);  //head backward 
    sleep(300);
    
    setServoPos(scaleServoRange(20, 21), 21); // Thumb
    setServoPos(scaleServoRange(10, 22), 22);
    setServoPos(scaleServoRange(10, 23), 23);
    setServoPos(scaleServoRange(10, 24), 24);
    setServoPos(scaleServoRange(10, 25), 25);

    setServoPos(scaleServoRange(20, 17), 17);
    setServoPos(scaleServoRange(45, 18), 18);
    setServoPos(scaleServoRange(35, 19), 19);
    setServoPos(scaleServoRange(50, 20), 20); //elbow
    sleep(3000);
    
    setServoPos(scaleServoRange(90, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(50, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(90, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(50, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(90, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(60, 20), 20);
    
    
    setServoPos(scaleServoRange(20, 21), 21);
    setServoPos(scaleServoRange(100, 22), 22);
    setServoPos(scaleServoRange(100, 23), 23);
    setServoPos(scaleServoRange(10, 24), 24);
    setServoPos(scaleServoRange(10, 25), 25);
    sleep(2000);
    
    setServoPos(scaleServoRange(20, 17), 17);
    setServoPos(scaleServoRange(20, 18), 18);
    setServoPos(scaleServoRange(45, 19), 19);
    setServoPos(scaleServoRange(70, 20), 20); //elbow
    sleep(2500);
    
    setServoPos(scaleServoRange(90, 21), 21);
    setServoPos(scaleServoRange(90, 22), 22);
    setServoPos(scaleServoRange(90, 23), 23);
    setServoPos(scaleServoRange(90, 24), 24);
    setServoPos(scaleServoRange(90, 25), 25);
    sleep(500);
            
    setServoOff(17);
    setServoOff(18);
    setServoOff(19);
    setServoOff(20);
    //setServoOff(21);
    //setServoOff(22);
    //setServoOff(23);
    //setServoOff(24);
    //setServoOff(25);
}


int moveRock(void)
{
    setServoPos(scaleServoRange(50, 27), 27);  //head backward 
    setServoPos(scaleServoRange(15, 28), 28);  //head backward 
    sleep(300);
    
    setServoPos(scaleServoRange(20, 21), 21); // Thumb
    setServoPos(scaleServoRange(10, 22), 22);
    setServoPos(scaleServoRange(10, 23), 23);
    setServoPos(scaleServoRange(10, 24), 24);
    setServoPos(scaleServoRange(10, 25), 25);


    setServoPos(scaleServoRange(20, 17), 17);
    setServoPos(scaleServoRange(45, 18), 18);
    setServoPos(scaleServoRange(35, 19), 19);
    setServoPos(scaleServoRange(50, 20), 20); //elbow
    sleep(2000);
    
    setServoPos(scaleServoRange(90, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(50, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(90, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(50, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(90, 20), 20); //elbow
    sleep(500);
    setServoPos(scaleServoRange(60, 20), 20);
    
    
    setServoPos(scaleServoRange(20, 21), 21);
    setServoPos(scaleServoRange(10, 22), 22);
    setServoPos(scaleServoRange(1, 23), 23);
    setServoPos(scaleServoRange(10, 24), 24);
    setServoPos(scaleServoRange(10, 25), 25);
    sleep(2000);
    
    setServoPos(scaleServoRange(20, 17), 17);
    setServoPos(scaleServoRange(20, 18), 18);
    setServoPos(scaleServoRange(45, 19), 19);
    setServoPos(scaleServoRange(70, 20), 20); //elbow
    sleep(2500);
    
    setServoPos(scaleServoRange(90, 21), 21);
    setServoPos(scaleServoRange(90, 22), 22);
    setServoPos(scaleServoRange(90, 23), 23);
    setServoPos(scaleServoRange(90, 24), 24);
    setServoPos(scaleServoRange(90, 25), 25);
    sleep(500);
    
    setServoOff(17);
    setServoOff(18);
    setServoOff(19);
    setServoOff(20);
    //setServoOff(21);
    //setServoOff(22);
    //setServoOff(23);
    //setServoOff(24);
    //setServoOff(25);
}


int movePoint( void )
{
    setServoPos(scaleServoRange(20, 17), 17);
    setServoPos(scaleServoRange(60, 18), 18);
    setServoPos(scaleServoRange(35, 19), 19);
    setServoPos(scaleServoRange(30, 20), 20); //elbow
    sleep(4000);
    
    setServoPos(scaleServoRange(20, 21), 21);
    setServoPos(scaleServoRange(100, 22), 22);
    setServoPos(scaleServoRange(10, 23), 23);
    setServoPos(scaleServoRange(20, 24), 24);
    setServoPos(scaleServoRange(20, 25), 25);
    sleep(2500);
    
    setServoPos(scaleServoRange(90, 21), 21);
    setServoPos(scaleServoRange(90, 22), 22);
    setServoPos(scaleServoRange(90, 23), 23);
    setServoPos(scaleServoRange(90, 24), 24);
    setServoPos(scaleServoRange(90, 25), 25);
    
    setServoPos(scaleServoRange(20, 17), 17);
    setServoPos(scaleServoRange(20, 18), 18);
    setServoPos(scaleServoRange(45, 19), 19);
    setServoPos(scaleServoRange(70, 20), 20); //elbow
    sleep(4000);
        
    setServoOff(17);
    setServoOff(18);
    setServoOff(19);
    setServoOff(20);
    setServoOff(21);
    setServoOff(22);
    setServoOff(23);
    setServoOff(24);
    setServoOff(25);
    
}

/**
 * This function scales the min - max values of the servo to 5age positional values
 * The following equation is used:
 *
 *         (b-a)(x-min)
 *  f(x) = ------------   + a
 *         (max - min)
 *
 * 'a' and 'b' are the physical servo movement values.
 */

int scaleServoRange(int aValue, int aServo)
{
    int scaleValue = 0;
    
    scaleValue = (((servoRanges[aServo][1] - servoRanges[aServo][0]) * (aValue - 1)) / (100-1)) + servoRanges[aServo][0];
    
    return scaleValue;
}

int constrain(value, minVal, maxVal)
{
  if (value < minVal)
  {
    return minVal;
  }
  else if (value > maxVal)
  {
    return maxVal;
  }
  else
  {
    return value;
  }
}


int setUpCOM(void)
{
  DCB dcbSerialParams;
  DCB dcbSerialParams1;
  
  dcbSerial.DCBlength=sizeof(dcbSerialParams);
  dcbSerial1.DCBlength=sizeof(dcbSerialParams1);
  
  
  if (!GetCommState(hSerialLEFT, &dcbSerialParams)) 
  {
    printf("Error setting up COM.\r\n");
  }
  if (!GetCommState(hSerialRIGHT, &dcbSerialParams1)) 
  {
    printf("Error setting up COM.\r\n");
  }
  //dcbSerialParams.BaudRate=CBR_57600;
  //dcbSerialParams.BaudRate=CBR_19200;
  dcbSerialParams.BaudRate=CBR_9600;
  dcbSerialParams.ByteSize=8;
  dcbSerialParams.StopBits=ONESTOPBIT;
  dcbSerialParams.Parity=NOPARITY;
  if(!SetCommState(hSerialLEFT, &dcbSerialParams)) 
  {
    printf("State of COM can not be set.\r\n");
  }
   
  dcbSerialParams1.BaudRate=CBR_9600;
  dcbSerialParams1.ByteSize=8;
  dcbSerialParams1.StopBits=ONESTOPBIT;
  dcbSerialParams1.Parity=NOPARITY;
  if(!SetCommState(hSerialRIGHT, &dcbSerialParams1)) 
  {
    printf("State of COM can not be set.\r\n");
  }
   
  
    
  COMMTIMEOUTS timeouts={0};
  timeouts.ReadIntervalTimeout=50;
  timeouts.ReadTotalTimeoutConstant=50;
  timeouts.ReadTotalTimeoutMultiplier=10;
  timeouts.WriteTotalTimeoutConstant=50;
  timeouts.WriteTotalTimeoutMultiplier=10;

  if(!SetCommTimeouts(hSerialLEFT, &timeouts)) 
  {
    printf("COM port has timed out.\r\n");
  }

  if(!SetCommTimeouts(hSerialRIGHT, &timeouts)) 
  {
    printf("COM port has timed out.\r\n");
  }

}

int connectToCOM(void)
{    
  hSerialLEFT = CreateFile("\\\\.\\COM7",GENERIC_WRITE,0,0,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,0);  // Right side
  hSerialRIGHT = CreateFile("\\\\.\\COM11",GENERIC_WRITE,0,0,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,0);
  
  
  //hSerial = CreateFile("COM4",GENERIC_WRITE,0,0,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,0);
  if(hSerialLEFT==INVALID_HANDLE_VALUE) 
  {
    if(GetLastError()==ERROR_FILE_NOT_FOUND) 
    {
      printf("Invalid COM port exiting....\r\n");
    }
    printf("Invalid COM port exiting....\r\n");
  } 
  
  
  if(hSerialRIGHT==INVALID_HANDLE_VALUE) 
  {
    if(GetLastError()==ERROR_FILE_NOT_FOUND) 
    {
      printf("Invalid COM port exiting....\r\n");
    }
    printf("Invalid COM port exiting....\r\n");
  }
}

int setServoOff(int aServo)
{
    int offValue = 15;
    int onValue = 64 + offValue;
    
    writeToCOM(0, aServo, offValue, 0x00);    
}

int setServoOn(int aServo)
{
    int offValue = 15;
    int onValue = 64 + offValue;
    
    writeToCOM(0, aServo, onValue, 0x00);    
}

int setServoPos(int aPos, int aServo)
{
  int data2 = 0;
  int data1 = aPos;
  if(data1 == -1) 
  {
    stop = true;
    data1 = data2 = 0x00;         
  }
  else if(data1 >= 128) 
  {
    temp = data1;
    data1 = data1 >> 7;        
    data2 = temp - (data1 << 7);
  }
  else if(data1 < 128) 
  {
    data2 = data1;
    data1 = 0x00;    
  }
  writeToCOM(4, aServo, data1, data2);  
  return 0;
}

int setServoSpeed(int aServo)
{
    int speed = 10;
    
    writeToCOM(1, aServo, speed, 0x00);
}

int writeToCOM(int cmdOption, int aServo, int theData1, int theData2)
{
  DWORD dwBytesWrite = 6;
  int n = 6;
  // Supply Start Byte - Always exists and is set to 0x80
  cmdBuf[0] = 0x80;
  // Address of the board
  cmdBuf[1] = 0x01;
  // The command we are using
  cmdBuf[2] = cmdOption;
  // Supply the servo number to modify / use
  cmdBuf[3] = aServo;
  // The data for 1 and 2 byte commands
  cmdBuf[4] = theData1;
  cmdBuf[5] = theData2;
  if(aServo < 16)
  {
    WriteFile(hSerialRIGHT, cmdBuf, n, &dwBytesWrite, NULL);
    FlushFileBuffers(hSerialRIGHT);
  }
  else
  {          
    WriteFile(hSerialLEFT, cmdBuf, n, &dwBytesWrite, NULL);
    FlushFileBuffers(hSerialLEFT);
  }
}

int setServoNumbers()
{
  DWORD dwBytesWrite = 3;
  int n = 3;
  // Supply Start Byte - Always exists and is set to 0x80
  cmdBuf[0] = 0x80;
  // Address of the board
  cmdBuf[1] = 0x02;
  // The command we are using
  cmdBuf[2] = 0x01;
  WriteFile(hSerialLEFT, cmdBuf, n, &dwBytesWrite, NULL);
  FlushFileBuffers(hSerialLEFT);
}
