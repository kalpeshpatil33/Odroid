#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#include "tcpip.h"
#include "decode.h"
#include "serial.h"

int main (int argc, const char * argv[]) {
	
	int i;
	int fhtcp0, tcp_data_size, tcp_data_size_sent;
	int fhserial, res, iores, fhi2c, i2c_data_size;
	unsigned char wbuf, sensor_bytes2[31];  
	char tcp_data[1024];
	
	int motorspeed;
	double phi, theta, psi, W[3], R[3][3];
	double Rd[3][3]={{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}}, Wd[3]={0.0,0.0,0.0}, Wd_dot[3]={0.0,0.0,0.0};
	double motor_out[4], M[3], f=4;
	
    struct timeval tinit, tnow;
	struct timespec tdelay;
	double mtelapse;
	int exitFlag=0;
		
	printf("Opening an I2C port...   ");
	fhi2c = open("/dev/i2c-3", O_RDWR);
	if (fhi2c <0)
		printf("\n Error opening an I2C port\n");
	else
		printf("Done!\n");
	
	usleep(10000);
		
	gettimeofday(&tinit,NULL);
	tdelay.tv_sec=0;
	tdelay.tv_nsec=10000000; //20ms=20 000us=20 000 000 ns
	
	mtelapse=0;
 	
	while(exitFlag == 0)
	{
		
		int mtr_addr[4]={41,44,43,42};
		for(i=0;i<4;i++)
		{
            motor_out[i]=10.0;
			tcflush(fhi2c, TCIFLUSH);
			
			if(ioctl(fhi2c, I2C_SLAVE, mtr_addr[i])>=0)
			{
				motorspeed=(int)motor_out[i];
				i2c_data_size = write(fhi2c,&motorspeed,1);
				if(i2c_data_size == -1)
				{
					printf("\nError sending data over I2C: %d, %d, %d\n",mtr_addr[i],motorspeed);
//					exitFlag=4;
				}
			}
			tcflush(fhi2c, TCIFLUSH);
		}

		tcflush(fhi2c, TCIFLUSH);
		

		gettimeofday(&tnow,NULL);
		
		mtelapse = (tnow.tv_sec - tinit.tv_sec) * 1000.0;      // sec to ms
		mtelapse += (tnow.tv_usec - tinit.tv_usec) / 1000.0;   // us to ms
		
		
		if (mtelapse >= 40000.0)
		{	
			exitFlag=1;
			for(i=0;i<4;i++)
			{
				tcflush(fhi2c, TCIFLUSH);
				
				if(ioctl(fhi2c, I2C_SLAVE, mtr_addr[i])>=0)
				{
					motorspeed=0;
					i2c_data_size = write(fhi2c,&motorspeed,1);
				}
				tcflush(fhi2c, TCIFLUSH);
			}
			printf("10 seconds\n");
		}
		
	}
	
	printf("Exit Flag : %d\n",exitFlag);	
	
	close(fhi2c);
    return 0;
}


