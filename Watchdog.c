// **********************************************************************************************
//                          W a t c h d o g . c
// **********************************************************************************************
// Project    : External WatchDog Enabling for Raspberry Pi
// Company    : Smarttrak Solar System Pvt. Ltd.
// File       : Watchdog.c
// Description:
// Date       : 04-10-2016 
// Author     : FirmWare Team
// Compilers  : Gcc / ARM 
// **********************************************************************************************

// *******************************
// Section : Includes 
// *******************************
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>

// *******************************
// Section : Definitions
// *******************************
#define SYSFS_GPIO_DIR  "/sys/class/gpio" 
#define MAX_BUF         65
#define WATCHDOG_LED    4
#define INPUT           0
#define OUTPUT          1
#define LOW             0
#define HIGH            1

// ******************************
// Section : Function Prototypes
// ******************************
int gpio_export(unsigned int gpio);
int gpio_unexport(unsigned int gpio);
int gpio_set_dir(unsigned int gpio, unsigned int out_flag);
int gpio_set_value(unsigned int gpio, unsigned int value);
int gpio_get_value(unsigned int gpio, unsigned int *value);
int gpio_set_edge(unsigned int gpio, char *edge);
int gpio_fd_open(unsigned int gpio);
int gpio_fd_close(int fd);

// *******************************
// Section : File Local Variables
// *******************************

// ******************************
// Section : Code
// ******************************
int main(void)
{
  int gpio_fd;

  gpio_export(WATCHDOG_LED);
  gpio_set_dir(WATCHDOG_LED, OUTPUT);
  gpio_fd = gpio_fd_open(WATCHDOG_LED);
   
  while(1)
  {
    gpio_set_value(WATCHDOG_LED, HIGH);
    printf("High\n");
    usleep(10000);
    gpio_set_value(WATCHDOG_LED, LOW);
    printf("Low\n");
    usleep(5000000);
  }
  gpio_fd_close(gpio_fd);
  gpio_unexport(WATCHDOG_LED);

  return 0;
}
 
int gpio_export(unsigned int gpio)
{
  int fd, len;
  char buf[MAX_BUF];
 
  fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
  if (fd < 0) 
  {
   perror("gpio/export");
   return fd;
  }
 
  len = snprintf(buf, sizeof(buf), "%d", gpio);
  write(fd, buf, len);
  close(fd);
  return 0;
}

int gpio_unexport(unsigned int gpio)
{
  int fd, len;
  char buf[MAX_BUF];
 
  fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
  if (fd < 0)
  {
   perror("gpio/export");
   return fd;
  }
 
  len = snprintf(buf, sizeof(buf), "%d", gpio);
  write(fd, buf, len);
  close(fd);
  return 0;
}

int gpio_set_dir(unsigned int gpio, unsigned int out_flag)
{
  int fd, len;
  char buf[MAX_BUF];
 
  len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);
  fd = open(buf, O_WRONLY);
  if (fd < 0)
  {
   perror("gpio/direction");
   return fd;
  }
 
  if (out_flag)
    write(fd, "out", 4);
  else
    write(fd, "in", 3);
 
  close(fd);
  return 0;
}

int gpio_set_value(unsigned int gpio, unsigned int value)
{
  int fd, len;
  char buf[MAX_BUF];
 
  len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
  fd = open(buf, O_WRONLY);
  if (fd < 0) 
  {
   perror("gpio/set-value");
   return fd;
  }
 
  if (value)
   write(fd, "1", 2);
  else
   write(fd, "0", 2);
 
  close(fd);
  return 0;
}

int gpio_get_value(unsigned int gpio, unsigned int *value)
{
  int fd, len;
  char buf[MAX_BUF];
  char ch;

  len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
  fd = open(buf, O_RDONLY);
  if (fd < 0) 
  {
   perror("gpio/get-value");
   return fd;
  }
 
  read(fd, &ch, 1);
  if (ch != '0') 
    *value = 1;
  else
    *value = 0;
 
  close(fd);
  return 0;
}

int gpio_set_edge(unsigned int gpio, char *edge)
{
  int fd, len;
  char buf[MAX_BUF];

  len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);
  fd = open(buf, O_WRONLY);
  if (fd < 0)
  {
    perror("gpio/set-edge");
    return fd;
  }
 
  write(fd, edge, strlen(edge) + 1); 
  close(fd);
  return 0;
}

int gpio_fd_open(unsigned int gpio)
{
  int fd, len;
  char buf[MAX_BUF];

  len = snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);
  fd = open(buf, O_RDONLY | O_NONBLOCK );
  if (fd < 0)
  {
    perror("gpio/fd_open");
  }
  return fd;
}

int gpio_fd_close(int fd)
{
  return close(fd);
}


