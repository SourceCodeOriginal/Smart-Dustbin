# Smart-Dustbin

This is part of our College mini-project in IOT subject.

We are a group of 5 members who have implemented a smart dustbin. 

The workflow of our project:

1. A Sound sensor will receive the clap signal. 
1. An Ultrasonic sensor is attached to the top of the bin.
1. The ultrasonic waves from the sensor are reflected back from the garbage in the bin. Using this, we calculate the percentage of empty space in the bin.
1. When the bin is full the data is sent to Thingspeak using ESP8266-01 (Wi-Fi module).
1. If the bin has empty space the servo motor will rotate 90 degrees so that lid could be open.
