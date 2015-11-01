//#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h> // clock_gettime & time structs
#include <algorithm> // copy arrays

#include "HC-SR04.h"


/*
class HC_SR04 {
    public:
        HC_SR04(uint8_t pin_trig, uint8_t pin_echo_count, uint8_t[] pins_echo);
		
		int Start();
		
		int Stop();
		
		SR04_Result Values_M3; // -3 result
		SR04_Result Values_M2; // -2 result
		SR04_Result Values_M1; // -1 result
		SR04_Result Values_0;  // Last result
		
		bool State;
	private:
		SR04_Result _Values_P1;  // Coming result
		uint8_t _pin_trig;
		uint8_t _pin_echo_count;
		uint8_t[] _pins_echo;
};
*/

#define GPIO_ADD 0x3f200000 /* GPIO controller PI V2 */
//#define GPIO_ADD 0x20200000L /* GPIO controller PI V1 */
// GPIO setup macros. Always use INP_GPIO(x) before using OUT_GPIO(x) or SET_GPIO_ALT(x,y)
#define INP_GPIO(g) *(gpio+((g)/10)) &= ~(7<<(((g)%10)*3))
#define OUT_GPIO(g) *(gpio+((g)/10)) |= (1<<(((g)%10)*3))
#define GPIO_SET(g) *(gpio+7) = 1<<(g) // sets bits which are 1 ignores bits which are 0
#define GPIO_CLR(g) *(gpio+10) = 1<<(g) // clears bits which are 1 ignores bits which are 0
#define GPIO_GET  *(gpio+13)

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

// Easy to change GPIOS
#define GPIO_TRIG 23
#define GPIO_ECHO 24


int  mem_fd     = 0;
unsigned char *gpio_mmap = NULL;
char *gpio_ram  = NULL;
volatile uint32_t *gpio = NULL;
int g = 0;


/*
int main(int argc, char **argv)
{
    double distance;

    struct timespec start_echo , stop_echo, diff_echo;
    double echo_time;
	int compt=0;
	int MAX_COUNT=9999999;
    

    // Set up gpi pointer for direct register access
    setup_io();

    INP_GPIO(GPIO_TRIG); // must use INP_GPIO before we can use OUT_GPIO
    OUT_GPIO(GPIO_TRIG);
    
    INP_GPIO(GPIO_ECHO); //input for echo pin
    printf("Waiting For Sensor To Settle\n");
    nsleep(500000);

	//Sens 10us to trigger
    GPIO_SET(GPIO_TRIG);
    nsleep(20); // send pulse 10us to trigger
//    nsleep(10); // send pulse 10us to trigger
    GPIO_CLR(GPIO_TRIG);
    
    clock_gettime(CLOCK_REALTIME, &start_echo);
    while ((GPIO_GET >> GPIO_ECHO) & 0 )
    {} // Boucler tant que le bit 23 est à 0, puis enregistrer le temps.
    clock_gettime(CLOCK_REALTIME, &start_echo);    
    printf("Ok2\n");

    //clock_gettime(CLOCK_REALTIME, &stop_echo);
    while (((GPIO_GET >> GPIO_ECHO) & 1)&&++compt<MAX_COUNT )
    {
//		clock_gettime(CLOCK_REALTIME, &start_echo);    
	} // Boucler tant que le bit 23 est à 1, puis enregistrer le temps.
    clock_gettime(CLOCK_REALTIME, &stop_echo);
    printf("Ok\n");
	if(compt>=MAX_COUNT)
		printf("Max reached\n");
		
    

	//calcul de la duree de l'echo
	if ((stop_echo.tv_nsec - start_echo.tv_nsec) < 0 ) {
        diff_echo.tv_sec = stop_echo.tv_sec - start_echo.tv_sec - 1;
        diff_echo.tv_nsec = 1000000000  + stop_echo.tv_nsec - start_echo.tv_nsec;
    } else {
        diff_echo.tv_sec = stop_echo.tv_sec - start_echo.tv_sec;
        diff_echo.tv_nsec = stop_echo.tv_nsec - start_echo.tv_nsec;
    }
	echo_time = ((diff_echo.tv_sec*1000000000) + (diff_echo.tv_nsec)); //nsec
	echo_time /= 1000000000;//sec
	//calcul de la distance en cm
	distance = (echo_time*17150); //cm

	printf("Distance: %.2f cm\n", distance);

	close_io();

	return 0;
} // main


*/


HC_SR04::HC_SR04(){};

void HC_SR04::Init(uint8_t pin_trig, uint8_t pin_echo_count, uint8_t pins_echo[])
{
	_pin_trig=pin_trig;
	_pin_echo_count=pin_echo_count;
	std::copy(_pins_echo, _pins_echo + pin_echo_count, pins_echo);
//	_pins_echo=pins_echo;
	// Set up gpi pointer for direct register access
    setup_io();

    INP_GPIO(pin_trig); // must use INP_GPIO before we can use OUT_GPIO
    OUT_GPIO(pin_trig);
    
	INP_GPIO(GPIO_ECHO); //input for echo pin
    printf("Waiting For Sensor To Settle\n");
    nsleep(500000);

	for(int i=0;i<pin_echo_count;i++)
		INP_GPIO(pins_echo[i]); //input for echo pin
}

HC_SR04::~HC_SR04()
{
	close_io();
}

double HC_SR04::Measure()
{
    double distance;

    struct timespec start_echo , stop_echo, diff_echo;
    double echo_time;
	int compt=0;
	int MAX_COUNT=9999999;

    // GPIO_CLR(GPIO_TRIG);
    // nsleep(200); // ensure 100us blank
	
    GPIO_CLR(GPIO_TRIG);
    nsleep(100); // wait
	
	while (((GPIO_GET >> GPIO_ECHO) & 1) && ++compt<MAX_COUNT );
	
	if(compt>=MAX_COUNT)
	{
		printf("GPIO already up!\n");
		return 0;
	}

    nsleep(100); // wait
	//Sens 10us to trigger
    GPIO_SET(GPIO_TRIG);
    nsleep(20); // send pulse 10us to trigger
//    nsleep(10); // send pulse 10us to trigger
    GPIO_CLR(GPIO_TRIG);
    
//    clock_gettime(CLOCK_REALTIME, &start_echo);
//    while ((GPIO_GET >> GPIO_ECHO) & 0 )
//    {} // Boucler tant que le bit 23 est à 0, puis enregistrer le temps.
    clock_gettime(CLOCK_REALTIME, &start_echo);    
//    printf("Ok2\n");
//nsleep(10);
    //clock_gettime(CLOCK_REALTIME, &stop_echo);
    while (((GPIO_GET >> GPIO_ECHO) & 1) && ++compt<MAX_COUNT )
    {
//		clock_gettime(CLOCK_REALTIME, &start_echo);    
	} // Boucler tant que le bit 23 est à 1, puis enregistrer le temps.
    clock_gettime(CLOCK_REALTIME, &stop_echo);
//   printf("Ok\n");
	if(compt>=MAX_COUNT)
		printf("Max reached\n");
		
    

	//calcul de la duree de l'echo
	if ((stop_echo.tv_nsec - start_echo.tv_nsec) < 0 /* ns */) {
        diff_echo.tv_sec = stop_echo.tv_sec - start_echo.tv_sec - 1;
        diff_echo.tv_nsec = 1000000000 /* ns */ + stop_echo.tv_nsec - start_echo.tv_nsec;
    } else {
        diff_echo.tv_sec = stop_echo.tv_sec - start_echo.tv_sec;
        diff_echo.tv_nsec = stop_echo.tv_nsec - start_echo.tv_nsec;
    }
 //   printf("Ok!\n");
	echo_time = ((diff_echo.tv_sec*1000000000) + (diff_echo.tv_nsec)); //nsec
	echo_time /= 1000000000;//sec
	//calcul de la distance en cm
	distance = (echo_time*17150); //cm
 //   printf("Ok3 oO \n");
	
	return distance;

}


double* HC_SR04::Measures()
{
    double distance;

    struct timespec start_echo , stop_echo[_pin_echo_count], diff_echo[_pin_echo_count];
	bool echo_received[_pin_echo_count];
    double echo_time[_pin_echo_count];
	int compt=0;
	int MAX_COUNT=9999999;
	
	double distances[_pin_echo_count];

    GPIO_CLR(GPIO_TRIG);
    nsleep(200); // ensure 100us blank
	
	//Sens 10us to trigger
    GPIO_SET(GPIO_TRIG);
    nsleep(20); // send pulse 10us to trigger
//    nsleep(10); // send pulse 10us to trigger
    GPIO_CLR(GPIO_TRIG);
    
    clock_gettime(CLOCK_REALTIME, &start_echo);
    while ((GPIO_GET >> GPIO_ECHO) & 0 )
    {} // Boucler tant que le bit 23 est à 0, puis enregistrer le temps.
    clock_gettime(CLOCK_REALTIME, &start_echo);    
//    printf("Ok2\n");

    //clock_gettime(CLOCK_REALTIME, &stop_echo);
    while (((GPIO_GET >> GPIO_ECHO) & 1)&&++compt<MAX_COUNT )
    {
//		clock_gettime(CLOCK_REALTIME, &start_echo);    
	} // Boucler tant que le bit 23 est à 1, puis enregistrer le temps.
    clock_gettime(CLOCK_REALTIME, &stop_echo[0]);
 //   printf("Ok\n");
	if(compt>=MAX_COUNT)
		printf("Max reached\n");
		
    

	//calcul de la duree de l'echo
	if ((stop_echo[0].tv_nsec - start_echo.tv_nsec) < 0 /* ns */) {
        diff_echo[0].tv_sec = stop_echo[0].tv_sec - start_echo.tv_sec - 1;
        diff_echo[0].tv_nsec = 1000000000 /* ns */ + stop_echo[0].tv_nsec - start_echo.tv_nsec;
    } else {
        diff_echo[0].tv_sec = stop_echo[0].tv_sec - start_echo.tv_sec;
        diff_echo[0].tv_nsec = stop_echo[0].tv_nsec - start_echo.tv_nsec;
    }
 //   printf("Ok!\n");
	echo_time[0] = ((diff_echo[0].tv_sec*1000000000) + (diff_echo[0].tv_nsec)); //nsec
	echo_time[0] /= 1000000000;//sec
	//calcul de la distance en cm
	distance = (echo_time[0]*17150); //cm
 //   printf("Ok3 oO \n");
	
	return distances;

}



bool HC_SR04::AllReceived(uint8_t echo_count, bool received[])
{
	bool all_received=1;
	for(int i=0;i<echo_count;i++)
		all_received &= received[i];
	return all_received;
}

//
// Set up a memory regions to access GPIO
//
void HC_SR04::setup_io()
{

   /* open /dev/mem to get acess to physical ram */
   if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
      printf("can't open /dev/mem. Did you run the program with administrator rights?\n");
      exit (-1);
   }

   /* Allocate a block of virtual RAM in our application's address space */
   if ((gpio_ram = (char *)malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL) {
      printf("allocation error \n");
      exit (-1);
   }

   /* Make sure the pointer is on 4K boundary */
   if ((unsigned long)gpio_ram % PAGE_SIZE)
     gpio_ram += PAGE_SIZE - ((unsigned long)gpio_ram % PAGE_SIZE);

   /* Now map the physical addresses of the peripheral control registers 
      into our address space */
   gpio_mmap = (unsigned char *)mmap(
      (caddr_t)gpio_ram,
      BLOCK_SIZE,
      PROT_READ|PROT_WRITE,
      MAP_SHARED|MAP_FIXED,
      mem_fd,
      GPIO_ADD
   );

   if ((long)gpio_mmap < 0) {
      printf("unable to map the memory. Did you run the program with administrator rights?\n");
      exit (-1);
   }

   /* Always use a volatile pointer to hardware registers */
   gpio = (volatile uint32_t *)gpio_mmap;
} // setup_io




void HC_SR04::nsleep(long us)
{
	struct timespec wait;
	wait.tv_sec = us / (1000 * 1000);
	wait.tv_nsec = (us % (1000 * 1000)) * 1000;
	nanosleep(&wait, NULL);
}

//
/* Release GPIO memory region */
//
void HC_SR04::close_io()
{
	int ret;

	/* munmap GPIO */
	ret = munmap(gpio_mmap, BLOCK_SIZE);
	if (ret == -1) {
		perror("munmap() failed");
		exit(1);
	}

	/* close /dev/mem */
	ret = close(mem_fd);
	if (ret == -1) {
		perror("Cannot close /dev/mem");
		exit(1);
	}
}


