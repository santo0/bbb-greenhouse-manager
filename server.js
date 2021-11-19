/*
//============================================================================
// Name        : server.js
// Author      : Marti and Xavier
// Version     :
// Copyright   : All free
// Description :  Node.js server for the greenhouse
//============================================================================
*/


//Loading modules
var http = require('http');
var fs = require('fs');
var path = require('path');
const exec = require('child_process').execSync;
const nodemailer = require('nodemailer');
const GreenhouseManager = require('./greenhouseManager.js');
require('dotenv').config();



const greenhouseManager = new GreenhouseManager();

//Configure nodemailer
const transporter = nodemailer.createTransport({
    service: process.env.SERVER_EMAIL_SERVICE,
    auth: {
        user: process.env.SERVER_EMAIL_ADDRESS,
        pass: process.env.SERVER_EMAIL_PASSWORD
    }
});


const LOW_THRESHOLD = 20.0;
const HIGH_THRESHOLD = 25.0;

var emailToSend = 'no_mail';
var alreadySent = false;



// Initialize the server on port 8888
var server = http.createServer(function(req, res) {
    // requesting files
    var file = '.' + ((req.url == '/') ? '/index.html' : req.url);
    var fileExtension = path.extname(file);
    var contentType = 'text/html';
    // Uncoment if you want to add css to your web page
    if (fileExtension == '.css') {
        contentType = 'text/css';
    }
    fs.exists(file, function(exists) {
        if (exists) {
            fs.readFile(file, function(error, content) {
                if (!error) {
                    // Page found, write content
                    res.writeHead(200, { 'content-type': contentType });
                    res.end(content);
                }
            });
        }
        else {
            // Page not found
            res.writeHead(404);
            res.end('Page not found');
        }
    });
}).listen(8888);


/**
 * Calculate measures from the sensors using GreenhouseManager
 * @param  {string} nState New state for the led
 */ 
function calculateMeasures() {
    console.log("Calculate measurements");
    greenhouseManager.readHIH8120();
    greenhouseManager.readLightIntensitySensor();
    console.log("Light intensity sensor = " + greenhouseManager.lightIntensity);
    console.log("Temperature sensor = " + greenhouseManager.temperature);
    console.log("Humidity sensor = " + greenhouseManager.humidity);
}


// Loading socket io module
var io = require('socket.io')(server);

//Listening on
io.on('connection', function(socket) {
    //When client switch on/off window
    socket.on('changeWindowState', handleWindowChange);
    //When client switch on/off heater
    socket.on('changeHeaterState', handleHeaterChange);
    //When client select light intensity of led
    socket.on('changeLedState', handleLedChange);
    //When client writes email address for notifications
    socket.on('changeEmail', handleEmailChange);
    //When client first loads page
    socket.on('updateAllData', function() {
        io.emit("updateWindowState", greenhouseManager.windowState);
        io.emit("updateHeaterState", greenhouseManager.heaterState);
        io.emit("updateLedState", greenhouseManager.ledState);
        io.emit("updateMeasurementsData", JSON.stringify({ "temp": greenhouseManager.temperature, "humi": greenhouseManager.humidity, "light": greenhouseManager.lightIntensity }));
    });
});

//Every 5 seconds calculate measures, send email notification and communicate sensor readings to clients
var interval_read = setInterval(
    function() {
        //Read sensors
        calculateMeasures();

        let floatTemp = parseFloat(greenhouseManager.temperature);

        //If temperature exceeds the minimum or a maxium
        if (!(LOW_THRESHOLD < floatTemp && floatTemp < HIGH_THRESHOLD)) {
            //If there is an email address to notify and the message hasn't already been sent
            if (emailToSend != "no_mail" && !alreadySent) {
                alreadySent = true;
                const options = {
                    from: process.env.SERVER_EMAIL_ADDRESS,
                    to: emailToSend,
                    subject: "Your greenhouse has irregular temperature!",
                    text: "Your greenhouse temperature is " + floatTemp.toFixed(2) + ".\nBe careful!"
                };
                transporter.sendMail(options, function(err, info) {
                    if (err) {
                        console.log(err);
                    }else{
                        console.log("Sent to email: " + info.response);
                    }

                });
            }
        } else {
            //Resets alreadySent flag when temperature stabilizes
            alreadySent = false;
        }
        
        //Communicate sensor readings to clients
        let sData = { "temp": greenhouseManager.temperature, 
                      "humi": greenhouseManager.humidity, 
                      "light": greenhouseManager.lightIntensity };
        io.emit("updateMeasurementsData", JSON.stringify(sData));
        console.log("Sent to client: " + sData);
    }, 5000
);


/**
 * Change the state of the window according to the data sent by the client 
 * @param  {{string:string}} data Dictionary that has the new state of the window
 */
function handleWindowChange(data) {
    var nData = JSON.parse(data);
    console.log("Window = " + nData.windowState);
    greenhouseManager.handleWindow(nData.windowState);
    io.emit("updateWindowState", nData.windowState);
}

/**
 * Change the state of the heater according to the data sent by the client 
 * @param  {{string:string}} data Dictionary that has the new state of the heater
 */
function handleHeaterChange(data) {
    var nData = JSON.parse(data);
    console.log("Heater = " + nData.heaterState);
    greenhouseManager.handleHeater(nData.heaterState);
    io.emit("updateHeaterState", nData.heaterState);
}

/**
 * Change the new light intensity of the LED according to the data sent by the client 
 * @param  {{string:string}} data Dictionary that has the new light intensity of the LED
 */
function handleLedChange(data) {
    var nData = JSON.parse(data);
    console.log("Led = " + nData.ledState);
    greenhouseManager.handleLed(nData.ledState);
    io.emit("updateLedState", nData.ledState);
}

/**
 * Change the email address where the irregular temperature notifications will be sent 
 * @param  {{string:string}} data Dictionary that has the new email which will be sent the notifications
 */
function handleEmailChange(data) {
    console.log("EMAIL!! " + data);
    let regexEmail = /^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/;
    //Only keep the email address if it's a valid email address
      if (data.match(regexEmail)) {
        emailToSend = data;
        console.log("Given email is valid!! :D")
      } else{
          console.log("Given email is invalid!")
      }
}


// Displaying a console message for user feedback
//server.listen(console.log("Server Running ..."));
console.log("Server Running ...");
