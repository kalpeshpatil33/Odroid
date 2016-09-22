// System header files (gcc compiler on ODROID)
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <math.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <termios.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include <linux/i2c-dev.h>
#include "decode.h"
#include "serial1.h"


// 4 Threads: Keypad, Vicon data, Controller
#define NUM_THREADS 4

// Pi constant and IMU offset
#define PI 3.14159265359


// Threads (except command key) ON/OFF
bool SYSTEM_RUN = true;
// Command key thread ON/OFF
bool Accept_Commands = true;
// Motors ON/OFF
bool MOTOR_ON = false;
// Start Motors at 0 Pitch and 20 Throttle (getting going)
bool MotorWarmup = true;
// Special conditions
bool SphericalJointTest = true;
// Print command
bool flag_printf = false;

#include "AUX_Global.h"

void *Command_Key_Thread(void *thread_id)
{
	printf("joined 1...\n");
	ExecuteKeypadCommand();

	usleep(250000);
	pthread_exit(NULL);
}

#include "GetIMUdata.h"

void *IMU_Thread(void *thread_id)
{
    printf("joined 2...\n");
	GetIMUdata();
	usleep(250000);
	pthread_exit(NULL);
}


int main()
{
	int i;
	int fhtcp0, tcp_data_size, tcp_data_size_sent;
	int fhserial, res, iores, fhi2c, i2c_data_size;
	unsigned char wbuf,wbuf_update, sensor_bytes2[31],sensor_bytes2_last[31];
	char tcp_data[1024];
	double phi, theta, psi, W[3],delta_t;
	struct timespec tdelay;
	double mtelapse, mtelapse1, mtelapse2,mtelapse11;
	struct timeval tinit, tnow;
	int exitFlag=0;
	
	pthread_t threads[4];
	pthread_attr_t attr;
	struct sched_param  param;
	int fifo_max_prio, fifo_min_prio;

	
	printf(" \n");
	printf("------------------------------------------\n");
	printf("------------------------------------------\n");
	printf("-- ######## ########   ######  ## --------\n");
	printf("-- ##       ##     ## ##    ## ## --------\n");
	printf("-- ##       ##     ## ##       ## --------\n");
	printf("-- ######   ##     ## ##       ## --------\n");
	printf("-- ##       ##     ## ##       ## --------\n");
	printf("-- ##       ##     ## ##    ## ## --------\n");
	printf("-- ##       ########   ######  ######## --\n");
	printf("------------------------------------------\n");
	printf("------------------------------------------\n");
	printf("  \n");
	sleep(1);
	
	//////////////////////////////////////////Opening the Serial Port///////////////////////////////////////////////
	//usleep(10000);
	
	printf("Opening a serial port...   \n");//Opens the Serial Port
	fhserial=init_serial();
    
	if (fhserial <0)
		printf("\n Error opening a serial port\n");
	else
		printf("Done!\n");
    
	// Initialize mutex and condition variables
	//  pthread_mutex_init(&data_acq_mutex, NULL);

	// Set thread attributes
	pthread_attr_init(&attr);
	pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	fifo_max_prio = sched_get_priority_max(SCHED_FIFO);
	fifo_min_prio = sched_get_priority_min(SCHED_FIFO);


	// Create threads and associate priorities (equal):

	// Command_Key_Thread
	param.sched_priority = fifo_max_prio/NUM_THREADS;
	pthread_attr_setschedparam(&attr, &param);
	pthread_create(&threads[0], &attr, Command_Key_Thread, (void *) 0);
	
	param.sched_priority = fifo_max_prio/NUM_THREADS;
	pthread_attr_setschedparam(&attr, &param);
	pthread_create(&threads[1], &attr, IMU_Thread, (void *) 1);
	
	printf("Beginning threads...\n");
	// Wait for all threads to complete
	for (i = 0; i < 2; i++)
	{
		pthread_join(threads[i], NULL);
	}

	close(fhserial);
	pthread_attr_destroy(&attr);
	//   pthread_mutex_destroy(&data_acq_mutex);
	
	//printf("\n   Press ENTER to begin threads.\n");
	//scanf("%c", &PressEnter);


	printf("\n\n\n\nProgram is complete. Goodbye.\n");

	return 0;
}


	   