#include <stdint.h>
#include <time.h>

struct SR04_Result {
	double distance[];
    struct timespec start_echo;
	struct timespec stop_echo[];
	struct timespec diff_echo[];
};

class HC_SR04 {
    public:
        HC_SR04();
		void Init(uint8_t pin_trig, uint8_t pin_echo_count, uint8_t pins_echo[]);
		~HC_SR04();
		
		int Start();
		
		int Stop();
		
		SR04_Result Values_M3; // -3 result
		SR04_Result Values_M2; // -2 result
		SR04_Result Values_M1; // -1 result
		SR04_Result Values_0;  // Last result
		
		bool State;
		
		
		double Measure();
		double* Measures();
	private:
		SR04_Result _Values_P1;  // Coming result
		uint8_t _pin_trig;
		uint8_t _pin_echo_count;
		uint8_t _pins_echo[];
		void close_io(); // Free memory regions
		void setup_io(); // Set up memory regions to access GPIO
		void nsleep(long us); //delay in usec using nanosleep rather than sleep
		bool AllReceived(uint8_t echo_count, bool received[]);
};
