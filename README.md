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



<h2>Extra Project - Line Follower</h2>
Assembled a robot capable of autonomously following a predefined path.<br>
This project was developed within a 3-person team, "Robo Putrezitul", along with <a href="https://github.com/Teo0o0">@Teo0o0</a> and <a href="https://github.com/blwUsama">@blwUsama</a>.<br><br><details>
<summary>Project details</summary>
This project integrates sensors, motors, and a control algorithm to enable navigating along the track marked by a black line.<br>
Our robot completed the final track in 20.5 seconds, slightly over the maximum-grade threshold (20 seconds).<br>
  
<h3>Components</h3>
  1.Arduino Uno<br>
  2.Breadboard (a small one)<br>
  3.Power source (LiPo battery)<br>
  4.QTR reflectance sensor<br>
  5.2 Wheels<br>
  6.2 DC motors<br>
  7.Ball caster<br>
  8.Wires<br>
  9.Zip-ties<br>
  10.Chassis (created with foamcore)<br>

<h3>Main Functionalities</h3>
  Line Detection - The sensors continuously monitor the surface and process the data to determine the robot's position relative to the line.<br>
  Control Algorithm - The code is implemented to make decisions regarding direction based on the line sensor data (through adjusting motor speeds).<br>
  Autonomous Navigation - The robot makes real-time adjustments to navigate through curves and stay on track.<br>

<h3>Chassis Design</h3>
The chassis was created out of foamboard, with a few points in mind when designed.<br>
Complex forms were avoided to improve stability and durableness, while securing the other pieces attached.<br>
The sensors were placed in front, along with the ball caster, to facilitate the detection of incoming path, while the motor-controlled wheels were placed at the back for more control when making turns.<br>
We placed the LiPo battery (heaviest piece) on the lowest level in the center of the chassis and the breadboard above it, to lower the center of gravity.<br>
When fixing the battery, we used a pin to hold it in place to ensure a smooth change when changing it to a charged one is needed.<br>


<h3>Calibration</h3>
The robot is able to self-calibrate, not needing manual handling when being turned on.<br>
This is done through the robot turning left-right before starting to follow the line.<br>

<h3>Parameter Tuning</h3>
The PID values were low or 0 at first, until we experimented with several sets to find the optimal numbers for our setup.<br>
The main traits we looked at when evaluating the parameter values were the sharpness of the turns and the shaking on straight paths.<br>
The final values were kp=15, ki=0, kd=7.<br>


</details>

-> <a href="https://youtu.be/uPR8K8UPLq4">video of the best lap</a>

