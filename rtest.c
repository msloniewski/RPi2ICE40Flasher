
#include <stdio.h>


#include <bcm2835.h>




#include "RaspberryPi_Prog.c" 





int main(int argc, char **argv) 
{
	if(argc < 2)
	{
		printf("No file name provided!");
		return 0;
	}
	printf("Flashing %s\n",argv[1]);

	//bcm2835_set_debug(1);
	if (!bcm2835_init())
	return 1;
		
	cfg(argv[1]);

	return 0;

}
