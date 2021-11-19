# bbb-greenhouse-manager
Greenhouse manager for BeagleBone Black, using Node.js and C++.

## Requeriments
First of all, make sure to have installed:
+ [nodemailer](https://www.npmjs.com/package/nodemailer)
+ [socket.io](https://www.npmjs.com/package/socket.io)
+ [dotenv](https://www.npmjs.com/package/dotenv)

##  Usage
A .env file must be created, with the following environment variables:
\begin{itemize}
+ SERVER_EMAIL_SERVICE = The name of the email service to be used. In our case we tried "hotmail".
+ SERVER_EMAIL_ADDRESS = The email address where the notifications will be sent from.
+ SERVER_EMAIL_PASSWORD =  The password to the specified service associated with the email address.

With the .env file, you can run the programme with the following command:

```console
foo@bar:~$ sudo node server.js
```

Sudo permits are necessary because Greenhouse Manager interacts with some sensors in driver mode.

## C++ programs
You can find the source of the cpp programs in cpp directory (ofc).