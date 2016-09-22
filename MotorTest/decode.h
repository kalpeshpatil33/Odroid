#include<sys/types.h>

/*#define BIG_ENDIAN      0
#define LITTLE_ENDIAN   1
these are declaired in types.h */

 
typedef unsigned char BYTE;

/* this code is from the microstain SDK no idea how it works*/
int TestByteOrder()
{
   short int word = 0x0001;
   char *byte = (char *) &word;
   return(byte[0] ? LITTLE_ENDIAN : BIG_ENDIAN);
}

/* this code is also from the sdk and i have no idea what it does
convert bytes to float*/

float FloatFromBytes(const unsigned char* pBytes)
{
	float f = 0;
	if(TestByteOrder() != BIG_ENDIAN) {
	   ((BYTE*)(&f))[0] = pBytes[3];
	   ((BYTE*)(&f))[1] = pBytes[2];
	   ((BYTE*)(&f))[2] = pBytes[1];
	   ((BYTE*)(&f))[3] = pBytes[0];
	}else{
	   ((BYTE*)(&f))[0] = pBytes[0];
	   ((BYTE*)(&f))[1] = pBytes[1];
	   ((BYTE*)(&f))[2] = pBytes[2];
	   ((BYTE*)(&f))[3] = pBytes[3];
	}
	
	return f; 
}
/* this code is also from the sdk and i have no idea what it does to
convert bytes to float*/

unsigned long convert2ulong(unsigned char* plbyte) {
	unsigned long ul = 0;
	if(TestByteOrder() != BIG_ENDIAN) {
	   ul = (plbyte[0] <<24) + (plbyte[1] <<16) + (plbyte[2] <<8) + (plbyte[3] & 0xFF);    
	}else{
		ul = (unsigned long)plbyte;
	}
  	return ul;
}

/*Byte extractor*/
float FourBytes2Float(const unsigned char* pBytes ,int from, int to)
{
	unsigned char temp[4];	
	temp[0] = pBytes[from];
	temp[1] = pBytes[from +1];
	temp[2] = pBytes[from +2];
	temp[3] = pBytes[from +3];

return (FloatFromBytes(temp));
}

/* temp funct */
unsigned long FourBytes2ULong(const unsigned char* pBytes ,int from, int to)
{
	unsigned char temp[4];	
	temp[0] = pBytes[from];
	temp[1] = pBytes[from +1];
	temp[2] = pBytes[from +2];
	temp[3] = pBytes[from +3];

return (convert2ulong(temp));
}

void resolve_sensor_data(float *temp, unsigned char *bytes)
{ 
		
		temp[0] = FourBytes2Float(bytes,1, 4);
		temp[1] = FourBytes2Float(bytes,5, 8);
		temp[2] = FourBytes2Float(bytes,9, 12);

		/* extracting rotation rate data */
		temp[3] = FourBytes2Float(bytes,13, 16);
		temp[4] = FourBytes2Float(bytes,17, 20);
		temp[5] = FourBytes2Float(bytes,21, 24);

		/* extracting Orientation matrix data */
		/* Row 1 */
		temp[6] = FourBytes2Float(bytes,25, 28);
		temp[7] = FourBytes2Float(bytes,29, 32);
		temp[8] = FourBytes2Float(bytes,33, 36);

		/* Row 2 */
		temp[9] = FourBytes2Float(bytes,37, 40);
		temp[10] = FourBytes2Float(bytes,41, 44);
		temp[11] = FourBytes2Float(bytes,45, 48);

		/* Row 3 */
		temp [12] = FourBytes2Float(bytes,49, 52);
		temp[13] = FourBytes2Float(bytes,53, 56);
		temp[14] = FourBytes2Float(bytes,57, 60);

}



