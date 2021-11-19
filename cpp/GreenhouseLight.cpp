//============================================================================
// Name        : GreenhouseLight.cpp
// Author      : Marti and Xavier
// Version     :
// Copyright   : All free
// Description : Sets light intensity of greenhouse LEDs
//============================================================================

#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <regex>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;

int period = 4000; // default period

//Black-P9_01
//Red-P9_03
//Orange-P9_21

/**
 * Change LED light intensity given by value in parameters
 *
 * @param the new value for the light intensity of the LED
 *
 * @return 0 if operation success
 */
int pwm_light(int light_val){

	//Change pin mode to pwm
	system("config-pin P9_21 pwm >/dev/null");

	struct stat info;
	// Check if channel is initialized checking if the directory exists
	int ecode_stat = stat("/sys/class/pwm/pwmchip1/pwm-1\:1/", &info); //ignore warning

	if (ecode_stat == -1) {
		// If channel is not initialized, initialize it
		system("sudo sh -c \"echo 1 > /sys/class/pwm/pwmchip1/export\"");
		cout << "PWM channel has been initialized." << endl;
	}

	// Sets period
	string cmd_period = "sudo sh -c \"echo ";
	cmd_period.append(to_string(period));
	cmd_period.append(" > /sys/class/pwm/pwmchip1/pwm-1\\:1/period\"");
	system(cmd_period.c_str());

	// Calculate new duty cycle
	int new_duty_cycle = period * (light_val / 100.0);

	// Sets duty cycle
	string cmd_duty_cycle = "sudo sh -c \"echo ";
	cmd_duty_cycle.append(to_string(new_duty_cycle));
	cmd_duty_cycle.append(" > /sys/class/pwm/pwmchip1/pwm-1\\:1/duty_cycle\"");
	system(cmd_duty_cycle.c_str());

	// Enables configuration
	system("sudo sh -c \"echo 1 > /sys/class/pwm/pwmchip1/pwm-1\\:1/enable\"");

	cout << "Greenhouse Light settings" << endl;
	cout << "\t Period = " << period << endl;
	cout << "\t Duty cycle = " << new_duty_cycle << endl;
	cout << "\t Light value = " << light_val << endl;

	return 0;

}


/**
 * Checks if the given chars sequence represents or not an integer
 *
 * @param chars the char sequence which will be checked
 *
 * @return true if given char sequence is an integer, otherwise false
 */
bool is_integer(char* chars){
	for(int i = 0; chars[i] != '\0'; i++){//until null char
		if(!isdigit(chars[i])){
			return false;
		}
	}
	return true;

}



int main(int argc, char* argv[]) {
	if(argc > 2) {
		if (is_integer(argv[1])){
			int light_intensity = atoi(argv[1]);
			if (0 <= light_intensity && light_intensity <= 100){
				pwm_light(light_intensity);
			} else {
				cout << "Given parameter is not between 0 and 100" << endl;
				return 1;
			}
		} else {
			cout << "Given parameter is not an integer." << endl;
			return 1;
		}
	} else {
		cout << "No parameter given." << endl;
		return 1;
	}
	return 0;
}


