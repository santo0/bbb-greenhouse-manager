//============================================================================
// Name        : GreenhouseWindow.cpp
// Author      : Marti and Xavier
// Version     :
// Copyright   : All free
// Description : Open or close greenhouse window (servomotor)
//============================================================================

#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <regex>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;

int period = 20000000;
//He canviat aixo
int open_duty_cycle = 600000;
int close_duty_cycle  = 1500000;

//PWM-P9_14

int pwm_servo(int duty_cycle){

	//Change pin mode to pwm
	system("config-pin P9_14 pwm >/dev/null");

	struct stat info;
	// Check if channel is initialized checking if the directory exists
	int ecode_stat = stat("/sys/class/pwm/pwmchip4/pwm-4\:0/", &info); //ignore warning

	if (ecode_stat == -1) {
		// If channel is not initialized, initialize it
		cout << "PWM channel not initialized." << endl;
		system("sudo sh -c \"echo 0 > /sys/class/pwm/pwmchip4/export\"");
		cout << "PWM channel has been initialized." << endl;
	}

	// Sets period
	string cmd_period = "sudo sh -c \"echo ";
	cmd_period.append(to_string(period));
	cmd_period.append(" > /sys/class/pwm/pwmchip4/pwm-4\\:0/period\"");
	system(cmd_period.c_str());

	// Sets duty cycle
	string cmd_duty_cycle = "sudo sh -c \"echo ";
	cmd_duty_cycle.append(to_string(duty_cycle));
	cmd_duty_cycle.append(" > /sys/class/pwm/pwmchip4/pwm-4\\:0/duty_cycle\"");
	system(cmd_duty_cycle.c_str());

	// Enables configuration
	system("sudo sh -c \"echo 1 > /sys/class/pwm/pwmchip4/pwm-4\\:0/enable\"");

	cout << "Greenhouse Light settings" << endl;
	cout << "\t Period = " << period << endl;
	cout << "\t Duty cycle = " << duty_cycle << endl;

	return 0;

}


int main(int argc, char* argv[]) {
	if(argc >= 2) {

		string cmd(argv[1]);

		if (cmd=="on"){
			pwm_servo(open_duty_cycle);
		}else if(cmd=="off"){
			pwm_servo(close_duty_cycle);
		} else {
			cout << "Given parameter is not valid." << endl;
			return 1;
		}

	} else {
		cout << "No parameter given." << endl;
		return 1;
	}
	return 0;
}


