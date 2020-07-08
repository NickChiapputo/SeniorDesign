# Jun 15
## Summary
* Read through the [Programming Chirp Parameters in TI Radar Devices](https://www.ti.com/lit/swra553) document and noted important information.
* Tested mmWave Demo Visualizer with a smaller range resolution. 
	* Tested box at 6 inches. [This is the setup](https://i.imgur.com/4z0cnFV.jpg).
	* [Resulting plot](https://imgur.com/MyYAHf6.png) showed center of object at around 0.222m (8.74 in) with closest spot at 0.157m (6.18 in) and furthest at 0.286m (11.26 in). Note that box is only 2 inches deep.
	* [Range Profile](https://i.imgur.com/ui4FiBS.png) shows similar results.
	* Downloaded profile configuration setup. This gave some insight into how the profile can be configured. However, I was not able to find documentation on the format of the configuration commands. I have shown the configuration information in the Steps Taken section.

## Problems Encountered
* Document mentioned the [Chip parameter configuration utility](http://www.ti.com/tool/mmwave-dfp)
	* However, the page only mentions AWR1243 and AWR2243. I am not sure if this works with AWR1843, or if there is another tool that we need to use to that end.
* I am unable to understand how we can configure the profile, chirp, and frame for a mmWave sensor. The documentation explains, multiple times, that certain attributes can be changed when configuring them. However, I can not find an explanation for how to change them or how to place them on the board. 
	* My best guess currently is that we must create configuration files and somehow convert them into files that the system can read, and then use the UniFlash software, as used for the demo applications on Jun 15, to flash the images to the sensor.
	* The description of mmWave Studio seems like what we might need. I need to investigate it further.
	* The profile configuration files I downloaded from the Demo Visualizer give some insight into how to set the parameters, but I can not find documentation on how to format the commands.

## Things Learned
### Definitions:
* Range Resolution:
	* c / ( 2 \* B )
	* c = Speed of Light
	* B = Bandwidth (TI devices have a 4 GHz sweep bandwidth)
* Angular Range:
	* Signal arriving at receivers is delayed by d * sin(theta)
	* Causes phase shift of (2 \* pi \* d \* sin(theta))/lambda
		* d = Distance between receivers
		* theta = Angle of the object
		* lambda = Wavelength of the signal
	* Maximum unambiguous angular range = sin^(-1)(lambda/(2d))
* Angular Resolution
	* Given in degrees:
		* ( lambda / ( d \* N\_{RX} \* N\_{TX} \* cos(theta) ) \* 180 / pi
	* N\_{RX} = Number of receiver antennas
	* N\_{TX} = Number of transmit antennas
	* Having multiple transmitters allows to create a virtual receive array with N\_{RX} \* N\_{TX} virtual channels. 
	* AWR1843, with 3 TX and 4 TX, can achieve 12 virtual channels.

### Configurable Chirp Profiles
* TI radar devices allow us to control parameters of chirps ina  frame by defining chirp profiles.
* Controllable Parameters:
	* Start frequency
	* Frequency slope
	* Idle time
	* ADC start time
	* Ramp end time
* Can also have small dither values
	* Start frequency variable
	* Frequency slope variable
	* Idle time variable
	* ADC start time variable

### Chirp Timing Parameters
* Idle Time
	* Time between the end of the previous chirp and the start of the next chirp.
	* Determined based on the Synthesizer Ramp Down Time (SRDT).
		* For Ramp Bandwidth < 1 GHz, SRDT = 2 us.
		* For Ramp Bandwidth > 1 GHz and < 2 GHz, SRDT = 3.5 us.
		* For Ramp Bandwidth > 2 GHz and < 3 GHz, SRDT = 5 us.
		* For Ramp Bandwidth > 3 GHz, SRDT = 7 us.
	* When ADC sampling rate is >= 5 Msps, use the above values to directly set the idle time.
		* Otherwise, minimum idle time might need to be slightly higher
		* This is due to pipeline delays in the sigma-delta ADC decimation chain
		* [Chirp parameter configuration utility](http://www.ti.com/tool/mmwave-dfp) accounts for this and provides overall recommended idle time.
* TX Start Time
	* Time from the start of the ramp at which the transmitter is turned on.
* ADC Start Time
	* The time from the start of the ramp when the ADC starts sampling the data.
	* Delaying this can improve quality of the signal. I believe this reduces the noise that can be caused by sampling too quickly. We will want to set this to a value that is somewhat below the total round trip time for the signal to reach the tire and come back.
* Ramp End Time
	* The time from the start of the ramp until the chirp continues ramping. After this time the synthesizer frequency is reset to the start frequency of the next chirp.
	* Sum of the ADC start time, the ADC sampling time, and the excess ramping time at the end of the ramp.
	* This likely has some affect on the quality of the signal similar to teh ADC start time's effect on the quality of the signal at the beginning of the ramp.

### General Notes
* It seems that, were we to design our own antenna, we could adjust the distance between the receivers in order to improve our measurement accuracies

## Steps Taken
* Explored the document [Programming Chirp Parameters in TI Radar Devices](https://www.ti.com/lit/swra553)
* Downloaded mmWave Studio
* Used the [mmWave Demo Visualizer](https://dev.ti.com/gallery/view/mmwave/mmWave_Demo_Visualizer/ver/3.4.0/) to detect the open sensor box at ~7.75 inches away. The lid of box curved away from the sensor and ranged from ~7.75-8.5 inches away. 
	* Configuration Parameters:
		* Frequency Band (GHz) - 77-81
		* Range Resolution - 0.039 
		* Maximum Unambiguous Range (m) - 11.99
		* Maximum Radial Velocity (m/s) - 1
		* Maximum Velocity Resolution (m/s) - 0.13
		* Unselected Group Peaks from Same Object
		* Set AoA (degrees) for Azimuth Elevation to min -45 and max 45
	* Tested box at 6 inches. [This is the setup](https://i.imgur.com/4z0cnFV.jpg).
		* [Resulting plot](https://imgur.com/MyYAHf6.png) showed center of object at around 0.222m (8.74 in) with closest spot at 0.157m (6.18 in) and furthest at 0.286m (11.26 in). Note that box is only 2 inches deep.
		* [Range Profile](https://i.imgur.com/ui4FiBS.png) shows similar results.
	* Saved profile configuration file for this setup:

	% ***************************************************************
	% Created for SDK ver:03.04
	% Created using Visualizer ver:3.4.0.0
	% Frequency:77
	% Platform:xWR18xx
	% Scene Classifier:best_range_res
	% Azimuth Resolution(deg):15
	% Range Resolution(m):0.039
	% Maximum unambiguous Range(m):11.99
	% Maximum Radial Velocity(m/s):1
	% Radial velocity resolution(m/s):0.13
	% Frame Duration(msec):100
	% Range Detection Threshold (dB):15
	% Doppler Detection Threshold (dB):15
	% Range Peak Grouping:enabled
	% Doppler Peak Grouping:enabled
	% Static clutter removal:disabled
	% Angle of Arrival FoV: Full FoV
	% Range FoV: Full FoV
	% Doppler FoV: Full FoV
	% ***************************************************************
	sensorStop
	flushCfg
	dfeDataOutputMode 1
	channelCfg 15 5 0
	adcCfg 2 1
	adcbufCfg -1 0 1 1 1
	profileCfg 0 77 287 7 200 0 0 20 1 384 2000 0 0 30
	chirpCfg 0 0 0 0 0 0 0 1
	chirpCfg 1 1 0 0 0 0 0 4
	frameCfg 0 1 16 0 100 1 0
	lowPower 0 0
	guiMonitor -1 1 1 0 0 0 1
	cfarCfg -1 0 2 8 4 3 0 15 1
	cfarCfg -1 1 0 4 2 3 1 15 1
	multiObjBeamForming -1 1 0.5
	clutterRemoval -1 0
	calibDcRangeSig -1 0 -5 8 256
	extendedMaxVelocity -1 0
	lvdsStreamCfg -1 0 0 0
	compRangeBiasAndRxChanPhase 0.0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0
	measureRangeBiasAndRxChanPhase 0 1.5 0.2
	CQRxSatMonitor 0 3 19 125 0
	CQSigImgMonitor 0 127 6
	analogMonitor 0 0
	aoaFovCfg -1 -90 90 -90 90
	cfarFovCfg -1 0 0 12.00
	cfarFovCfg -1 1 -1 1.00
	sensorStart
