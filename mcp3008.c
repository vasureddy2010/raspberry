// **********************************************************************************************
//                           M C P 3 0 0 8 . c
// **********************************************************************************************
// Project    : Raspberry Pi ADC read Using MCP3008 IC supporting SPI Protocol 
// Company    : Smarttrak Solar System pvt. ltd.
// File       : mcp3008.c
// Description: 
// Date       : 19-05-2016
// Author     : M Sathish
// Compilers  : Gcc / ARM 
// **********************************************************************************************

// *******************************
// Section : Includes 
// *******************************
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

// *******************************
// Section : File Global Variables 
// *******************************
uint8_t spiChannel=0;
uint8_t mode = 0;
uint8_t spiBPW = 8;
uint32_t speed = 1000000;
uint16_t spiDelay = 0;
uint8_t  Fd;

// *******************************
// Section : Function Prototypes 
// *******************************
int loadSpiDriver();
int SPIDataRW (int channel, unsigned char *data, int len);
int myAnalogRead(int spiChannel,int channelConfig,int analogChannel);

// *******************************
// Section : Code
// *******************************
int main ()
{
  int ii,jj;
  int val,a_val;
  float vol,res,temp;
 
  if(loadSpiDriver() != 0)
    return 1;
  
  if((Fd = open("/dev/spidev0.0", O_RDWR)) < 0)
  {
    perror("Open ");
    return 1;
  }

  if (ioctl (Fd, SPI_IOC_WR_MODE, &mode) < 0)
  {
    printf("SPI Mode Change Failure: %s\n", strerror (errno));
    return 1;
  }
  if (ioctl (Fd, SPI_IOC_WR_BITS_PER_WORD, &spiBPW) < 0)
  {
    printf("SPI BPW Change Failure: %s\n", strerror (errno));
    return 1;
  }
  if (ioctl (Fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0)
  {
    printf("SPI Speed Change Failure: %s\n", strerror (errno));
    return 1;
  }
 
  printf("\n\tSMARTTRAK SOLAR SYSTEMS Pvt. Ltd.\n\t---------------------------------\n");
  printf("Setting SPI Parameters Successfull\n");
  printf("ADC - MCP3008, Using CE0, Single Ended\n");
  
  while(1)
  {
    printf("******************************\n");
    for(ii=0;ii<8;ii++)
    {
      val = myAnalogRead(0,8,ii);
      vol = (((float)val/1023)*3.3);
      printf("Channel%02d : %d\tVoltage : %f\n",ii+1,val,vol);
    }    
    sleep(2);
  }  
  close (Fd) ;
  return 0;
}

int loadSpiDriver()
{
  if(system("gpio load spi") == -1)
  {   
    fprintf (stderr, "Error: Can't load the SPI driver-> %s\n", strerror (errno)) ;
    return 1;
  }
  return 0;
}

int SPIDataRW (int channel, unsigned char *data, int len)
{
  struct spi_ioc_transfer spi ;
  channel &= 1 ;

  memset (&spi, 0, sizeof (spi)) ;

  spi.tx_buf        = (unsigned long)data;
  spi.rx_buf        = (unsigned long)data;
  spi.len           = len;
  spi.delay_usecs   = spiDelay;
  spi.speed_hz      = speed;
  spi.bits_per_word = spiBPW;

  return ioctl (Fd, SPI_IOC_MESSAGE(1), &spi) ;
}

int myAnalogRead(int spiChannel,int channelConfig,int analogChannel)
{
  unsigned char buffer[3] = {1}; 

  if(analogChannel<0 || analogChannel>7)
    return -1;
  buffer[1] = (channelConfig+analogChannel) << 4;
  SPIDataRW(spiChannel, buffer, 3);
  return ( (buffer[1] & 3 ) << 8 ) + buffer[2]; 
}


