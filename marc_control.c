#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


int fd;
int fd1;
char *commport = "/dev/ttyUSB0";
char *commport1 = "/dev/ttyUSB1"; // < 16
char cmdBuf[6];

int writeData(int, int, int, int);
int moveRollEyes( void );
int moveTestHand( void );
int moveShakeHand( void );
int movePaper( void );
int moveRock( void );
int moveScissors( void );
int setServoOff(int);
int moveLook(void);
int moveRHClose( void );
int moveLHOpen( void );
int moveRHOpen( void );
int moveLHClose( void );
int moveWave( void );
int moveDAB( void );
int moveRest( void );
int movePose( void );
int movePoint( void );
int movePhotoL();
int movePhotoR();



int setCommPort( int fd, int fd1, int speed )
{
    struct termios tty;

    if (tcgetattr (fd, &tty) != 0)
    {
        printf("Error getting COM attributes.\n");
        return -1;
    }

    if (tcgetattr (fd1, &tty) != 0)
    {
        printf("Error getting COM1 attributes.\n");
        return -1;
    }

    cfsetospeed (&tty, speed);
    cfsetospeed (&tty, speed);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;		// 8-Bit Characters
    tty.c_cc[VMIN] = 0;					// Read doesn't block
    tty.c_cc[VTIME] = 5;				// 0.5 seconds read timeout

    tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag |= 0;					// 0 = no parity, 1 = parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr (fd, TCSANOW, &tty) != 0)
    {
        printf("Some setting COM attributes.\n");
        return -1;
    }

    if (tcsetattr (fd1, TCSANOW, &tty) != 0)
    {
        printf("Error setting COM1 attributes.\n");
        return -1;
    }

    return 0;
}

int setServoOff(int aServo)
{
    int offValue = 15;
    int onValue = 64 + offValue;

    writeData(0, aServo, offValue, 0x00);
}

int setServoPos(int aPos, int aServo)
{
    int temp = 0;
    int data2 = 0;
    int data1 = aPos;

    if(data1 == -1)
    {
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

    writeData(4, aServo, data1, data2);
    return 0;
}

int setServoSpeed(int aServo)
{
    int speed = 10;
    writeData(1, aServo, speed, 0x00);
}

int writeData(int cmd, int aServo, int aData1, int aData2)
{
    cmdBuf[0] = 0x80;
    cmdBuf[1] = 0x01;
    cmdBuf[2] = cmd;
    cmdBuf[3] = aServo;
    cmdBuf[4] = aData1;
    cmdBuf[5] = aData2;

    if (aServo < 16)
    {
        write(fd1, cmdBuf, 6);
    }
    else
    {
        write(fd, cmdBuf, 6);
    }

    return 0;
}

int main (void)
{
    // Create random seed
    srand(time(NULL));
    int r;

    int theServo = -1;
    int servoPos = -1;
    int i;
    char option[10];
    char option1[10];


    fd = open(commport, O_RDWR | O_NOCTTY | O_SYNC);
    fd1 = open(commport1, O_RDWR | O_NOCTTY | O_SYNC);
    
    if (fd < 0)
    {
        printf("Error opening port.\n");
        return -1;
    }

    if (fd1 < 0)
    {
        printf("Error opening port1.\n");
        return -1;
    }


    printf("Setting up COMPORT.\n");
    setCommPort(fd, fd1, B19200);
/*
    //Manual Servo Control
    while(1)
    {
        if(theServo == -1)
        {
            printf("Select a servo (0-31): ");
            gets(option1);
            
            if(strcmp(option1,"servo") == 0)
            {
                printf("It's a match.\n");
            }
            theServo = atoi(option1);
        }
        printf("Enter Servo position (500 - 5500): ");
        gets(option);
        servoPos = atoi(option);
        if(servoPos == -1)
        {
            theServo = -1;
        }
        else
        {
            setServoPos(servoPos, theServo);
        }
    }

  */  
/*
    fgets(option, sizeof(option), stdin);
    i = strlen(option)-1;
    if(option[i] == '\n')
        option[i] = '\0';
*/
while(1){
    printf("Command: ");
    gets(option);
    
    if(strcmp(option, "shake") == 0)
    {
        printf("Selected Shake Hands.\n");
        moveShakeHand();
    }
    if(strcmp(option, "game") == 0)
    {
        printf("Playing paper, rock, scissors.\n");
        r = rand() % 3;
        if (r == 0)
        {        
            movePaper();
        }
        if (r == 1)
        {
            moveRock();
        }
        if (r == 2)
        {
            moveScissors();
        }        
    }
    if(strcmp(option, "rhclose") == 0)
    {
        printf("Closing Right Hand.\n");
        moveRHClose();
    }
    if(strcmp(option, "lhclose") == 0)
    {
        printf("Closing Left Hand.\n");
        moveLHClose();
    }
    if(strcmp(option, "lhopen") == 0)
    {
        printf("Opening Left Hand.\n");
        moveLHOpen();
    }
    if(strcmp(option, "point") == 0)
    {
        printf("Pointing.\n");
        movePoint();
    }
    if(strcmp(option, "rhopen") == 0)
    {
        printf("Opening Right Hand.\n");
        moveRHOpen();
    }
    if(strcmp(option, "rock") == 0)
    {
        printf("playing Rock.\n");
        moveRock();
    }
    if(strcmp(option, "paper") == 0)
    {
        printf("Playing Paper.\n");
        movePaper();
    }
    if(strcmp(option, "photoL") == 0)
    {
        printf("Pose photo Left.\n");
        movePhotoL();
    }
    if(strcmp(option, "photoR") == 0)
    {
        printf("Photo Right.\n");
        movePhotoR();
    }


    if(strcmp(option, "scissors") == 0)
    {
        printf("Playing Scissors.\n");
        moveScissors();
    }
    if(strcmp(option, "wave") == 0)
    {
        printf("Waving.\n");
        moveWave();
    }
    if(strcmp(option, "dab") == 0)
    {
        printf("Dabing.\n");
        moveDAB();
    }
    if(strcmp(option, "exit") == 0)
    {
        printf("Exiting.\n");
        exit(0);
    }
    if(strcmp(option, "rest") == 0)
    {
        printf("resting.\n");
        moveRest();
    }
    if(strcmp(option, "pose") == 0)
    {
        printf("posing.\n");
        movePose();
    }

  }
    return 0;
}

int moveLook( void )
{
    setServoPos(3000, 27);
    setServoPos(3000, 28);
    sleep(1);
}

int movePhotoL()
{
  setServoPos(3300, 1);
  setServoPos(3300, 2);
  
  sleep(20);
  moveRest();

  return 0;
}

int movePhotoR()
{
  setServoPos(2900, 17);
  setServoPos(4200, 18);
  
  sleep(20);
  moveRest();

  return 0;
}


int movePoint()
{
  moveRest();
  
  moveRHClose();
  setServoPos(2500, 18);
  usleep(1500000);
  setServoPos(1200, 22);
  sleep(5);
  moveRest();
  moveRHOpen();
  
  return 0;
}

int movePose (void)
{
  moveRest();
  moveRHOpen();
  moveLHOpen();

  setServoPos(3800, 1);
  setServoPos(3600, 17);
  
  setServoPos(4500, 19);
  setServoPos(4400, 3);
  
  setServoPos(3800, 4);
  setServoPos(3800, 20);

  sleep(5);

  setServoPos(2200, 11);
  usleep(1500000);
  setServoPos(3800, 11);
  usleep(1500000);
  setServoPos(2200, 11);
  usleep(1500000);
  setServoPos(3800, 11);
  usleep(1500000);
  setServoPos(3150, 11);
  usleep(1500000);

  sleep(2);

  setServoPos(3200, 19);
  setServoPos(3000, 3);
  sleep(2);
  moveRest();

  return 0;
}

int moveWave( void )
{
    moveRHOpen();
    setServoPos(2800, 17);
    setServoPos(1600, 19);
    setServoPos(4200, 20);
    sleep(4);
    setServoPos(3200, 20);
    usleep(900000);
    setServoPos(4200, 20);
    usleep(900000);
    setServoPos(3200, 20);
    usleep(900000);
    setServoPos(4200, 20);
    usleep(900000);
    setServoPos(3200, 20);
    usleep(900000);
    setServoPos(4200, 20);
    usleep(900000);
    setServoPos(3200, 20);
    usleep(900000);
    
    sleep(1);
    setServoPos(4200, 17);
    setServoPos(3200, 19);
    setServoPos(4200, 20);
    sleep(4);
   
    setServoOff(17);
    setServoOff(19);
    setServoOff(20);
    usleep(100000);
}

int moveDAB( void )
{
    moveRHOpen();
    moveLHOpen();
    setServoPos(2800, 17);
    setServoPos(1600, 19);
    setServoPos(4700, 20);
    
    setServoPos(4300, 3);
    setServoPos(1500, 2);
    setServoPos(3000, 4);
    sleep(4);

    sleep(1);
    setServoPos(4200, 17);
    setServoPos(3200, 19);
    setServoPos(4500, 20);
    setServoPos(2800, 2);
    setServoPos(3100, 3);
    setServoPos(4000, 4);
    sleep(4);
   
    setServoOff(17);
    setServoOff(19);
    setServoOff(20);
    setServoOff(3);
    setServoOff(2);
    setServoOff(4);
    usleep(100000);
}

int moveRest( void )
{
    setServoPos(4200, 17);
    setServoPos(4000, 18);
    setServoPos(3200, 19);
    setServoPos(4500, 20);
    setServoPos(4400, 1);
    setServoPos(3000, 2);
    setServoPos(3000, 3);
    setServoPos(4500, 4);

    sleep(4);
   
    setServoOff(17);
    setServoOff(19);
    setServoOff(20);
    setServoOff(3);
    setServoOff(2);
    setServoOff(4);
    setServoOff(18);
    setServoOff(1);
    setServoOff(11);

    usleep(100000);
}

int moveLHClose( void )
{
    setServoPos(2500, 5);
    setServoPos(1800, 6);
    setServoPos(1800, 7);
    setServoPos(1800, 8);
    setServoPos(1800, 9);
    usleep(700000);
}

int moveLHOpen( void )
{
    setServoPos(4200, 5);
    setServoPos(4200, 6);
    setServoPos(4200, 7);
    setServoPos(4200, 8);
    setServoPos(4200, 9);
    usleep(700000);

}


int moveRHClose( void )
{
    setServoPos(2500, 21);
    setServoPos(3700, 22);
    setServoPos(3400, 23);
    setServoPos(3700, 24);
    setServoPos(3500, 25);
    usleep(700000);

}

int moveRHOpen( void )
{
    setServoPos(1200, 21);
    setServoPos(1200, 22);
    setServoPos(1200, 23);
    setServoPos(1200, 24);
    setServoPos(1200, 25);
    usleep(700000);
}

int moveScissors( void )
{
    moveRHClose();

    setServoPos(3200, 18);
    setServoPos(4200, 20);
    sleep(3);    
    setServoPos(3700, 20);
    usleep(500000);
    setServoPos(4200, 20);//1
    usleep(500000);
    setServoPos(3700, 20);
    usleep(500000);
    setServoPos(4200, 20);//2
    usleep(500000);
    setServoPos(3700, 20);
    usleep(500000);
    setServoPos(4200, 20);//3
    usleep(500000);

    // The Move
    setServoPos(2500, 21);
    setServoPos(1200, 22);
    setServoPos(1200, 23);
    setServoPos(3700, 24);
    setServoPos(3500, 25);
    sleep(3);

    setServoPos(4000, 18);
    usleep(1500000);
    moveRHOpen();

    setServoOff(20);
    setServoOff(18);
    setServoOff(21);
    setServoOff(22);
    setServoOff(23);
    setServoOff(24);
    setServoOff(25);
    usleep(50000);
    
    return 0;
}

int moveRock( void )
{
    moveRHClose();

    setServoPos(3200, 18);
    setServoPos(4200, 20);
    sleep(3);    
    setServoPos(3700, 20);
    usleep(500000);
    setServoPos(4200, 20);//1
    usleep(500000);
    setServoPos(3700, 20);
    usleep(500000);
    setServoPos(4200, 20);//2
    usleep(500000);
    setServoPos(3700, 20);
    usleep(500000);
    setServoPos(4200, 20);//3
    usleep(500000);
    
    sleep(3);

    setServoPos(4000, 18);
    usleep(1500000);

    setServoOff(20);
    setServoOff(18);
    setServoOff(21);
    setServoOff(22);
    setServoOff(23);
    setServoOff(24);
    setServoOff(25);
    usleep(50000);
    
    return 0;

}

int movePaper( void )
{
    moveRHClose();

    setServoPos(3200, 18);
    setServoPos(4200, 20);
    sleep(3);    
    setServoPos(3700, 20);
    usleep(500000);
    setServoPos(4200, 20);//1
    usleep(500000);
    setServoPos(3700, 20);
    usleep(500000);
    setServoPos(4200, 20);//2
    usleep(500000);
    setServoPos(3700, 20);
    usleep(500000);
    setServoPos(4200, 20);//3
    usleep(500000);

    setServoPos(1200, 21);
    setServoPos(1200, 22);
    setServoPos(1200, 23);
    setServoPos(1200, 24);
    setServoPos(1200, 25);
    sleep(3);

    setServoPos(4000, 18);
    usleep(1500000);

    setServoOff(20);
    setServoOff(18);
    setServoOff(21);
    setServoOff(22);
    setServoOff(23);
    setServoOff(24);
    setServoOff(25);
    usleep(50000);
    
    return 0;
}


int moveShakeHand( void )
{

    setServoPos(3300, 18);
    setServoPos(4200, 20);
    sleep(2);

    setServoPos(2000, 21);
    setServoPos(2600, 22);
    setServoPos(2500, 23);
    setServoPos(2900, 24);
    setServoPos(2900, 25);
    sleep(2);
    
    setServoPos(4000, 20);
    usleep(500000);
    setServoPos(4400, 20);
    usleep(500000);
    setServoPos(4000, 20);
    usleep(500000);
    setServoPos(4400, 20);
    usleep(500000);
    setServoPos(4000, 20);
    usleep(500000);
    setServoPos(4400, 20);
    usleep(500000);

    setServoPos(1200, 21);
    setServoPos(1200, 22);
    setServoPos(1200, 23);
    setServoPos(1200, 24);
    setServoPos(1200, 25);
    sleep(2);

    setServoPos(4000, 18);
    sleep(3);

    setServoOff(17);
    setServoOff(18);
    setServoOff(19);
    setServoOff(20);
    setServoOff(21);
    setServoOff(22);
    setServoOff(23);
    setServoOff(24);
    setServoOff(25);
    sleep(1);

    return 0;
}


int moveRollEyes( void )
{
    int eyeLRMin = 2600;
    int eyeLRMax = 3100;
    int eyeUDMin = 2600;
    int eyeUDMax = 4200;

    setServoPos(eyeLRMin, 1);
    setServoPos(eyeUDMin, 0);

    sleep(1);

    setServoPos(eyeLRMax, 1);

    sleep(1);

    setServoPos(eyeUDMax, 0);

    sleep(1);

    setServoPos(eyeLRMin, 1);

    sleep(1);

    setServoPos(eyeUDMin, 0);

    sleep(1);

    setServoPos(3300, 0);
    setServoPos(2900, 1);
    sleep(1);


    return 0;

}
