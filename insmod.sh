#!/bin/sh

insmod ./smarthome_driver/jdq/jdq_driver.ko 
insmod ./smarthome_driver/sg90/sg90_driver.ko 
insmod ./smarthome_driver/dht11/dht11_driver.ko 
insmod ./smarthome_driver/sr501/sr501_driver.ko 
#insmod ap3216c_driver.ko 
#insmod ov2640_camera_driver.ko 
