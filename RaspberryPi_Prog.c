//The following C code is based Raspberry PI.
//Because the slave select pin of Raspberry SPI interface cannot be controlled by software independently.
//4 GPIOs in Bitbang mode are used to imitate the SPI timing.
//“JDI_LCD_bitmap.c” in the source file include C array converted from hex file generated in iCECube. A sample file is attached.
//User can call “cfg()” in the main procedure to finish the configuration.

#include "bcm2835.h" 


#define SDO RPI_BPLUS_GPIO_J8_35

#define SCLK RPI_BPLUS_GPIO_J8_36

#define CSN RPI_BPLUS_GPIO_J8_37

#define SDI RPI_BPLUS_GPIO_J8_38

#define CRESETB RPI_BPLUS_GPIO_J8_40

#define CDONE RPI_BPLUS_GPIO_J8_15


void assert_sdo ()

{

       bcm2835_gpio_write(SDO, HIGH);

}

void dessert_sdo ()

{

    bcm2835_gpio_write(SDO, LOW);

}

void assert_sclk ()

{

    bcm2835_gpio_write(SCLK, HIGH);

}

void dessert_sclk ()

{

    bcm2835_gpio_write(SCLK, LOW);

}

void sendbyte (char data)

{

       char temp;

       int  i;

       temp = data;

       for(i=0;i<8;i++)

       {

    if ((temp & 128)>0)

    assert_sdo();

    else

    dessert_sdo();

    assert_sclk();

    dessert_sclk();

    temp = temp <<1;

  } 

}

void assert_ss ()

{

    bcm2835_gpio_write(CSN, LOW);

}

void dessert_ss ()

{

    bcm2835_gpio_write(CSN, HIGH);

}

void init_spi ()

{

   bcm2835_gpio_fsel(CSN, BCM2835_GPIO_FSEL_OUTP);

   bcm2835_gpio_fsel(SCLK, BCM2835_GPIO_FSEL_OUTP);
   
   bcm2835_gpio_fsel(CRESETB, BCM2835_GPIO_FSEL_OUTP);

   bcm2835_gpio_fsel(SDO, BCM2835_GPIO_FSEL_OUTP);

   bcm2835_gpio_fsel(SDI, BCM2835_GPIO_FSEL_INPT);

   //bcm2835_gpio_write(CSN, HIGH);

   //bcm2835_gpio_write(SCLK, HIGH);

   //bcm2835_gpio_write(SDO, LOW);

}

 

void cfg(char* filename)

{
	 FILE *fp;
   char buff;

   fp = fopen(filename, "r");





int i;

    init_spi();

assert_ss();

char pdone = 0;
    

    bcm2835_gpio_write(CRESETB, LOW);

    bcm2835_delay(10);

    while (bcm2835_gpio_lev(CDONE)==HIGH) {};

    printf("CDONE is low!\n");
    bcm2835_gpio_write(CRESETB, HIGH);

    delay(2000);
    //bcm2835_gpio_write(CRESETB, LOW);
    //delay(2000);
         
    bcm2835_delay(1);   
	//No buffering stdout
	setbuf(stdout, NULL);

	i = 0;
	while ( fscanf(fp, "%c", &buff) == 1 )
	{
		sendbyte(buff);
		i++;
		if( i % 1000 == 0 )
		{
			printf("..");
		}
	}
	//49 dummy bytes after flashing
	for (i=0; i< 49; i++)
	{
		sendbyte(0);
	}
	printf("\n");
	printf("DONE\n");

    dessert_ss();
    fclose(fp);

}
