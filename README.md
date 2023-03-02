# :ghost: C++ KeyBroadcast
##### ------------------------------------------------
#### Sends keystrokes pressed to an external API.
###### 
## Can be customized to any api endpoint with simple change
#### :warning: Attention. This project is for scientific purposes only. Contains native C++ language functions to track buttons pressed within the system. It is designed to run locally on its own test API. The project is intentionally limited in the number of characters downloaded and not perfected to prevent misuse of the source code. 
### Each time the application starts, it turns on the console and informs about sending buttons to the API
### :space_invader: Before run, comment out the line responsible for boot autorun if you want to skip adding an app to registry 
------------------------------------------------------------------------
------------------------------------------------------------------------

#### I reserve the copyright to the source code from this application under the license below:
###  The software may be refactored and used only for legitimate purposes in accordance with applicable law.


-------------------------------------------------------------------------
-------------------------------------------------------------------------



#### You can customize API address by editing this line in main.cpp
````
serverName = "localhost";
const int serverPort = 3000;
````


# RUN
````
build and run main.cpp
$ node node/index.js
````
