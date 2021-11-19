//============================================================================
// Name        : GreenhouseHeater.cpp
// Author      : Marti and Xavier
// Version     :
// Copyright   : All free
// Description : Activates or deactivates greenhouse heater (blue led)
//============================================================================

#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <regex>
#include <sys/types.h>
#include <sys/stat.h>
using namespace std;


/**
 * Change heater state depending on the given value by gpio
 *
 * @param the new value for the heater gpio
 *
 * @return 0 if operation success
 *
 */
int gpio_heater(int value){

	//Sets direction to out
	system("sudo sh -c \"echo out > /sys/class/gpio/gpio49/direction\"");

	//Set given value
	string cmd_value = "sudo sh -c \"echo ";
	cmd_value.append(to_string(value));
	cmd_value.append(" > /sys/class/gpio/gpio49/value\"");
	system(cmd_value.c_str());

	return 0;

}


int main(int argc, char* argv[]) {
	if(argc >= 2) {

		string cmd(argv[1]);

		if (cmd=="on"){
			gpio_heater(1);
		}else if(cmd=="off"){
			gpio_heater(0);
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


