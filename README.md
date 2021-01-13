# Mass-Mailer
Mass mailer is a gui based app designed for sending lot of email fast.
It is built off of mailgun's api and requires your own account. Do not use default server and api key because aparently mailgun has banned me and they will not work.

It has 3 dependancys. Sfml for drawing the gui. Boost.Asio for multiple uses including multithreading, parsing json, and more. And finally LibCURL for sending packets.

This is one of my first completed c++ projects as I am not very good at finishing things.

Screenshots
-----------
![Alt text](Screenshots/Home.JPG?raw=true "Title")

Install Release
---------------
1. Install Visual c++ redistributables: https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads
2. Install release .zip
3. Extract .zip
4. run exe

Install Code
------------
1. Clone Src branch via git
2. Install sfml, boost.asio, and libcurl

**This can easily be done on vcpkg with the command**

```vcpkg install sfml boost-asio curl```

**This can easily be done on Linux with the commands**

```apt-get install libsfml-dev libboost-all-dev libcurl4-openssl-dev```


Coming soon
-----------
Make code neater

Make this send stmp emails independant of Mailgun

Fix the issue of mail always going to spam
