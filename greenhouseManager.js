const exec = require('child_process').execSync;

module.exports = class GreenhouseManager{
    /**
     * Creates instance of GreenhouseManager, initializes all channels and 
     *      checks the states and measurements of all sensors and actuators
     */
    constructor(){
        this.lightIntensity = "NaN";
        this.temperature = "NaN";
        this.humidity = "NaN";
        this.windowState = "NaN";
        this.heaterState = "NaN";
        this.ledState = "NaN";
        
        //Initialize channels and everything
        this.initAllChannels();
        
        //Read the first read
        this.readLightIntensitySensor();
        this.readHIH8120();
        this.readLedState();
        this.readWindowState();
        this.readHeaterState();
    } 

    
    /**
     * Initialize all necessary channels via executing InitAllChannels script
     */    
    initAllChannels() {
        var result = exec("sudo ./InitAllChannels");
        console.log(result.toString("utf8"));
    }
  
    /**
     * Read measure from light intensity sensor
     */
    readLightIntensitySensor() {
        var result = exec("./get_light_intensity.sh").toString("utf8").trim();
        this.lightIntensity = result;
    }
    
    /**
     * Read humidity and temperature from HIH8120 sensor
     */
    readHIH8120() {
        var result = exec("sudo ./HIH8120").toString("utf8").trim();
        var splitted = result.split(/\r?\n/);
        this.humidity = splitted[0].split(/\r?=/)[1];
        this.temperature = splitted[1].split(/\r?=/)[1];
    }
   
    /**
     * Read window state from servomotor
     */ 
    readWindowState() {
        var result_duty_cycle = exec("echo $(cat /sys/class/pwm/pwm-4\:0/duty_cycle)").toString("utf8").trim();
        if (result_duty_cycle == "1500000") {
            //He canviat aixo, del on a off i del off a on
            this.windowState = "off";
        } else if(result_duty_cycle == "600000") {
            this.windowState = "on";
        }
    }

    /**
     * Read heater state from blue led
     */
    readHeaterState() {
        var result = exec("echo $(cat /sys/class/gpio/gpio49/value)").toString("utf8").trim();
        if (result == "0"){
            this.heaterState = "off";
        } else if(result == "1") {
            this.heaterState = "on";
        }
    }
    
    /**
     * Read led intensity from Greenhouse LEDs
     */
    readLedState() {
        var result_period = exec("echo $(cat /sys/class/pwm/pwm-1\:1/period)").toString("utf8").trim();
        var result_duty_cycle = exec("echo $(cat /sys/class/pwm/pwm-1\:1/duty_cycle)").toString("utf8").trim();
        this.ledState = Math.round((parseFloat(result_duty_cycle) / parseFloat(result_period))*100);
    }

    /**
     * Change window state
     * @param  {string} nState New state for the window
     */    
    handleWindow(nState) {
        var result = exec("sudo ./GreenhouseWindow " + nState);
        this.readWindowState()
    }
    
    /**
     * Change heater state
     * @param  {string} nState New state for the heater
     */
    handleHeater(nState) {
        var result = exec("sudo ./GreenhouseHeater " + nState);
        this.readHeaterState();
    }
  
    /**
     * Change led state
     * @param  {string} nState New state for the led
     */ 
    handleLed(nState) {
        var result = exec("sudo ./GreenhouseLight " + nState);
        this.readLedState();
    }
    
}