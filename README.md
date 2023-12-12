# IntroductionToRobotics
A small corner of my achievements while learning the basics of robotics.👍<br>
Here I included the work, assignments and most of what I did during my 'Introduction to Robotics' course at the Faculty of Mathematics and Computer Science (University of Bucharest) in 2023-2024.

<h2>Homework #2</h2>
For this assignment I had to control an RGB LED using potentiometers. For each color channel there is a separate potentiometer that reads the analog values which are then mapped to the pin.<br><br>

<details>
<summary>Task details</summary>
Use a separate potentiometer for controlling each color of the RGB LED: Red, Green, and Blue.<br>
You need to read the potentiometer’s value and then write a mapped value to the LED pins.<br>
<br>
</details>

<img src="Assignments/Homework%202/Circuit_Picture.jpeg" width=400>
-> <a href="https://youtu.be/DwxRwo3IJ7w">a short video presentation</a>
<br><br><br>

<h2>Homework #3</h2>
This time, the task was to simulate a 3-floor elevator system, each level being represented by a LED. As an extra feature, there was a separate LED and a buzzer that signaled when the "elevator" was moving.<br><br>

<details>
<summary>Task details</summary>
This assignment simulates a 3-floor elevator control system using LEDs, buttons, and a buzzer.<br>
Each of the 3 LEDs represents one of the floors and the buttons represents the call buttons.<br>
An extra LED is used to show the elevator's current state (stationary or moving)<br>
The buzzer sounds briefly when arriving at the desired floor or when "doors are closing"<br>
The buttons are implemented with debounce to avoid unintentional repeated button presses.<br>
<br>
</details>


<img src="Assignments/Homework%203/Circuit_picture.jpeg" width=400>
-> <a href="https://youtu.be/ZlxJcPpUzy4">a short video presentation</a>
<br><br><br>

<h2>Homework #4</h2>
Using a joystick, I controlled the position of the segment and "drew" on a 7 segment display. I also added features like blinking, reset segments and a buzzer.<br><br>

<details>
<summary>Task details</summary>
Use the joystick to control the position of the segment and ”draw” on the display. The movement between segments should be natural, meaning they should jump from the current position only to neighbors.<br>
The initial position is on the DP and the current position always blinks.<br>
Short pressing the button turns the segment ON/OFF, Long pressing the button resets the entire display.<br>
<br>
</details>

<img src="Assignments/Homework%204/Circuit_Picture.jpeg" width=400>
-> <a href="https://youtu.be/MCEp_N6Ddfg">a short video presentation</a>
<br><br><br>

<h2>Homework #5</h2>
I created a timer with the basic functionalites of a stopwatch, to which I added a LED and a buzzer, which signal when the it is counting or paused.<br><br>

<details>
<summary>Task details</summary>
Using the 4 digit 7 segment display and 3 buttons, implement a stopwatch timer that counts in 10ths of a second and has a save lap functionality.<br>
The starting value should be ”000.0” and the buttons should have the following functionalities:<br>
(1) start/pause<br>
(2) reset time &saved laps<br>
(3) save lap & cycle through saved laps<br>
<br>
</details>

<img src="Assignments/Homework%205/Circuit_Picture.jpeg" width=400>
-> <a href="https://youtu.be/Ijw1ldbHLJo">a short video presentation</a>


<h2>Homework #6</h2>
Set up a pseudo-smart environment monitor and logger using 2 types of sensors: ultrasonic sensor and light-dependent resistor.<br><br>

<details>
<summary>Task details</summary> 
This system uses various sensors to gather environmental data and log this data into EEPROM.<br>
Provide both visual feedback via an RGB LED and user interaction through a Serial Menu.<br>
Focus on integrating sensor readings, memory management, Serial Communication and the general building of a menu.<br>
- Sensor Settings submenu should prompt the user to set values for sapling interval and alert thresholds<br>
- Reset Logger submenu confirms before reseting saved data<br>
- System Status submenu checks current status and health (sensor readings, settings and logged data)<br>
- LED submenu can toggle ON/OFF the automatic mode of the RGB LED, or set it manually from the console
<br>
</details>

<img src="Assignments/Homework%206/Circuit_Picture.jpeg" width=400>
-> <a href="https://youtu.be/e2Kyyh048dA">a short video presentation</a>


<h2>Homework #8</h2>
Matrix Checkpoint; Implemented the basic features of the snake game<br><br><details>
<summary>Task details</summary> 
Finished the main functionalities of the classic snake game.<br>
You can scroll through the menu (start game, adjust settings, game details).<br>
The game itself is working, with the basic features: the player can control the snake and eating fruits/losing lives are also implemented mechanisms.<br><br>
</details>

<img src="Assignments/Homework%208/Circuit_Picture.jpeg" width=400>
-> <a href="https://youtu.be/DsqZOCdHGPg?si=YOgD_jb-z0kW2eUU"> short video presentation</a>


