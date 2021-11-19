//============================================================================
// Name        : InitAllChannels.cpp
// Author      : Marti and Xavier
// Version     :
// Copyright   : All free
// Description : Initialize all sensors and actuators from greenhouse
//============================================================================


#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <regex>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;

int main() {

	//Sets gpio49 direction to out (Heater blue LED)
	system("sudo sh -c \"echo out > /sys/class/gpio/gpio49/direction\"");

	//Sets pin P9_21 mode to PWM (Greenhouse LEDs)
	system("config-pin P9_21 pwm >/dev/null");
	struct stat info1;

	// Check if channel is initialized checking if the directory exists (Greenhouse LEDs)
	int ecode_stat_1 = stat("/sys/class/pwm/pwmchip1/pwm-1\:1/", &info1); //ignore warning

	if (ecode_stat_1 == -1) {
		// If channel is not initialized, initialize it (Greenhouse LEDs)
		system("sudo sh -c \"echo 1 > /sys/class/pwm/pwmchip1/export\"");
		cout << "LED PWM channel has been initialized." << endl;
	}



	//Sets pin P9_14 mode to PWM (Window servomotor)
	system("config-pin P9_14 pwm >/dev/null");
	struct stat info2;

	// Check if channel is initialized checking if the directory exists (Window servomotor)
	int ecode_stat_2 = stat("/sys/class/pwm/pwmchip4/pwm-4\:0/", &info2); //ignore warning

	if (ecode_stat_2 == -1) {
		// If channel is not initialized, initialize it (Window servomotor)
		system("sudo sh -c \"echo 0 > /sys/class/pwm/pwmchip4/export\"");
		cout << "Window PWM channel has been initialized." << endl;
	}

	return 0;
}
