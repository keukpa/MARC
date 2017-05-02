#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


int fd;
char *commport = "/dev/ttyUSB0";
char cmdBuf[6];

int writeData(int, int, int, int);
int moveRollEyes( void );
int moveTestHand( void );
int moveShakeHand( void );

int setCommPort( int fd, int speed )
{
    struct termios tty;

    if (tcgetattr (fd, &tty) != 0)
    {
        printf("Error getting COM attributes.\n");
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

    return 0;
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

    write(fd, cmdBuf, 6);

    return 0;
}

int main (void)
{

    int theServo = -1;
    int servoPos = -1;
    int i;
    char option[10];
    char option1[10];


    fd = open(commport, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        printf("Error opening port.\n");
        return -1;
    }

    printf("Setting up COMPORT.\n");
    setCommPort(fd, B19200);
/*
    //Manual Servo Control
    while(1)
    {
        if(theServo == -1)
        {
            printf("Select a servo (0-31): ");
            fgets(option1, sizeof(option1), stdin);

            i = strlen(option1)-1;
            if(option1[i] == '\n')
                option1[i] = '\0';

            if(strcmp(option1,"servo") == 0)
            {
                printf("It's a match.\n");
            }
            theServo = atoi(option1);
        }
        printf("Enter Servo position (500 - 5500): ");
        fgets(option, sizeof(option), stdin);
        i = strlen(option)-1;
        if(option[i] == '\n')
            option[i] = '\0';

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
moveShakeHand();

    return 0;
}

int moveTestHand( void )
{
    int f1Min = 2800;
    int f2Min = 1500;
    int f3Min = 1700;
    int f4Min = 1500;
    int f5Min = 1600;

    int f1Max = 4200;
    int f2Max = 4200;
    int f3Max = 4200;
    int f4Max = 4300;
    int f5Max = 4200;

    setServoPos(f2Min, 1);
    sleep(2);
    setServoPos(f3Min, 2);
    sleep(2);
    setServoPos(f4Min, 3);
    sleep(2);
    setServoPos(f5Min, 4);
    sleep(2);
    setServoPos(f1Min, 0);
    sleep(2);

    setServoPos(f1Max, 0);
    sleep(2);
    setServoPos(f2Max, 1);
    sleep(2);
    setServoPos(f3Max, 2);
    sleep(2);
    setServoPos(f4Max, 3);
    sleep(2);
    setServoPos(f5Max, 4);
    sleep(2);


    return 0;
}

int moveShakeHand( void )
{
    int f1Min = 3600;
    int f2Min = 2890;
    int f3Min = 2300;
    int f4Min = 2300;
    int f5Min = 2400;

    int f1Max = 4200;
    int f2Max = 4200;
    int f3Max = 4200;
    int f4Max = 4300;
    int f5Max = 4200;

    setServoPos(f2Max, 1);
    setServoPos(f3Max, 2);
    setServoPos(f4Max, 3);
    setServoPos(f5Max, 4);
    setServoPos(f1Max, 0);
    sleep(5);

    setServoPos(f1Min, 0);
    setServoPos(f2Min, 1);
    setServoPos(f3Min, 2);
    setServoPos(f4Min, 3);
    setServoPos(f5Min, 4);
    sleep(5);

    setServoPos(f2Max, 1);
    setServoPos(f3Max, 2);
    setServoPos(f4Max, 3);
    setServoPos(f5Max, 4);
    setServoPos(f1Max, 0);
    sleep(5);


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
