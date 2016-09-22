void GetIMUdata(){
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
	///////////////////////////////////////////Opening the Serial Port///////////////////////////////////////////////
	//usleep(10000);
	
	// printf("Opening a serial port...   ");//Opens the Serial Port
	fhserial=init_serial();
    
	if (fhserial <0)
		printf("\n Error opening a serial port\n");
	else
		//	printf("Done!\n");
	
		////////////////////////////////////////////////Satrting the While loop/////////////////////////////////////////////////////////
    
		wbuf = 0xCF;
    
	gettimeofday(&tinit,NULL);
	tdelay.tv_sec=0.0;
	tdelay.tv_nsec=10000000;
	mtelapse=0.0;
	mtelapse1=0.0;
	mtelapse2=0.0;
	delta_t=0;
    
	wbuf_update=0x32;
	tcflush(fhserial, TCIFLUSH);
	res = write(fhserial,&wbuf_update,1);
    
    
	FILE *file;
	file = fopen("data_from_gumstix.txt","a+");
    
    
	while(exitFlag == 0)
	{
		tcflush(fhserial, TCIFLUSH);
		res = write(fhserial,&wbuf,1);
        
		res = read(fhserial,&sensor_bytes2[0],31);
        
        
		if (sensor_bytes2[0] != 0xCF)
		{
			printf("Serial data size incorrect: %x\n",sensor_bytes2[0]);
			for(i=0;i<31;i++)
			{
				sensor_bytes2[i]=sensor_bytes2_last[i];
			}
		}
		tcflush(fhserial, TCIFLUSH);
        
		//////////////////////////////Angular Velocity and Euler Angles calculated by the Sensor////////////////////////////////////////
        
		for(i=0;i<31;i++)
		{
			sensor_bytes2_last[i]=sensor_bytes2[i];
		}
        
		W[0] = FourBytes2Float(sensor_bytes2,13, 16);
		W[1] = -FourBytes2Float(sensor_bytes2,17, 20);
		W[2] = -FourBytes2Float(sensor_bytes2,21, 24);
        
		if(flag_printf == true) 
		{
			printf("%.2E %.2E %.2E\n",W[0],W[1],W[2]);
		
			gettimeofday(&tnow,NULL);
			mtelapse = (tnow.tv_sec - tinit.tv_sec)*1000 ;
			mtelapse += (tnow.tv_usec - tinit.tv_usec)/1000 ;
			mtelapse1=mtelapse/1000;
			delta_t=mtelapse1-mtelapse2;
			mtelapse2=mtelapse1;     
			if (flag_printf == false)
			{
				exitFlag=1;
				printf("10 seconds\n");
			}
		}
       
	}
}
   
