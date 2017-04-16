#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>


int fd;
char *commport = "/dev/ttyUSB0";
char cmdBuf[6];

int setCommPort( int fd, int speed )
{
    struct termios tty;

    if (tcgetattr (fd, &tty) != 0)
    {
        printf("Error somewhere.\n");
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
        printf("Some errormessage.\n");
        return -1;
    }

    return 0;
}


int main (void)
{

    fd = open(commport, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0)
    {
        printf("Error opening port.\n");
    }

    printf("Setting up COMPORT.\n");
    setCommPort(fd, B19200);

    cmdBuf[0] = 0x80;
    cmdBuf[1] = 0x01;
    cmdBuf[2] = 4;
    cmdBuf[3] = 3;
    cmdBuf[4] = 3;
    cmdBuf[5] = 63;

    write(fd, cmdBuf, 6);

    return 0;
}
