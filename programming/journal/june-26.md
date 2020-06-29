# June 26-29
## Summary
* Found links for:
	* [mmWave Sensor FAQ.](https://e2e.ti.com/support/sensors/f/1023/t/595725)
	* [Software Related FAQ for mmWave SDK.](https://e2e.ti.com/support/sensors/f/1023/t/856028)
		* This is just a link found from the mmWave Sensor FAQ.
* Was able to generate an empty project for the AWR1843 MSS.
* Ran the Medium Range Radar lab from the Automotive Toolbox
	* Not the greatest results, but I did learn a little about the program flow.
	* Contained source material that I can study in the future to learn how to write a mmWave Radar program.
	* The Developer's Guide and the source files are a good stepping point to learn more in the next step.

## Problems Encountered
* CCS says I don't have SYS/BIOS installed when I certainly do have it installed. Unsure of how to point CCS to the installation directory.
	* SOLUTION: 
		* Go to menu option Window -> Preferences -> Code Composer Studio -> Products.
		* Select 'Refresh' if SYS/BIOS does not show up in the 'Discovered Products' section. Restart CCS is necessary after discovering new products.
		* Add path to 'Product discovery path:' section.
		* Build the project (click the hammer).

## Things Learned
### Definitions
* Subsystems on AWR1843:
	* MSS
		* Master Subsystem -- ARM Cortex-R4F and associated peripherals, hosts the user application.
	* DSS
		* DSP Subsystem -- TI C674x and associare peripherals, hosts the user application.
	* Radar/millimeter wave block
		* Programmed using predefined message transactions specified by TI
		* Specific commands are explained on pp. 18-34 (Sec. 3.4) of the [MMWAVESDK User Guide](https://software-dl.ti.com/ra-processors/esd/MMWAVE-SDK/latest/exports/mmwave_sdk_user_guide.pdf)
	* Explained in the introduction in the [Application Startup Sequence](https://www.ti.com/lit/an/spraci4a/spraci4a.pdf?ts=1593205112774&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FAWR1843) document.

## Steps Taken
1. Looked through the [Automotive Toolbox](https://dev.ti.com/tirex/explore/node?a=VLyFKFf__4.1.0&node=AFeCagqbt7.hu2lHOXE4eA__AocYeEd__LATEST&r=VLyFKFf__3.6.2&r=VLyFKFf__4.0.0&r=VLyFKFf__LATEST) containing examples projects
	* Very few actually had AWR1843 examples.
	* Was only concerend about the Object Detection lab. However, this lab only had sources for the AWR1642, so I was unable to use any of it.
1. Looked through the [E2E Sensors forum FAQ](https://e2e.ti.com/support/sensors/f/1023/t/595725) for [mmWave devices](https://e2e.ti.com/support/sensors/f/1023/t/856028).
	* Found a [thread](https://e2e.ti.com/support/tools/ccs/f/81/t/836333) asking about writing programs using CCS from scratch.
		1. Followed the steps given; however, I did not have a template project option for SYS/BIOS despite having the exact version installed in the same location as specified.
		1. Installed 'mmWave Radar Device Support' through CCS Help -> Install New Software.
		1. Only one [e2e post](https://e2e.ti.com/support/tools/ccs/f/81/t/719914?CCS-AWR1642BOOST-SYS-BIOS-v6-53-2-00-is-not-currently-installed) mentioned the same issue, however their reason was because they were running it in Ubuntu. So the issue is not related.
		1. Explored the menu options Window -> Preferences -> Code Composer Studio -> Products
			1. Clicked 'Refresh' and it found new products including SYS/BIOS.
			1. Selected SYS/BIOS and installed it.
			1. Also added the product discovery path to the exact folder containing the product ('C:/ti/bios_6_73_01_01').
			1. Restarted CCS. Issue still persisted.
			1. Turns out the project needs to be built again to work properly.
			1. Click hammer to re-build project.
		1. Once SYS/BIOS is recognized and project is built again, NULL symbol was not recognized.
			* Building again seemed to fix the issue. Not sure why it was ever an issue. Is probably a bug in Eclipse.
		1. After building, started receiving error 'Periodic workspace save...could not save master table to file'. Searching found that it may be related to deleted files (I did not delete any).
		1. Decided to create project from scratch instead:
			1. Select File -> New -> CCS Project:
				1. Set project name to 'first_test'
				1. Select Target as 'mmWave Sensors' and selected AWR1843.
				1. Under 'Project type and tool-chain':
					1. Set 'Device endianness' to 'little'.
					1. Set 'Linker command file' to 'xwr1843_r4f.cmd'.
				1. Under 'Project templates and examples':
					1. Select SYS/BIOS -> TI Target Examples -> Typical.
				1. Click 'Next'.
				1. For 'Target', 'ti.targets.arm.elf.R4F' should already be added.
				1. For 'Platform', type 'ti.platforms.cortexR:AWR18XX:false:200'.
					* I do not know what these mean, and the FAQ link only specifies for the AWR16XX, so this may or may not be correct.
				1. Click 'Finish'.
			1. This creates a basic sysbios application where the default linker-command and sysbios-config (app.cfg) is generated from CCS. We need to amend these files per our application needs.
1. Explored Lab 07 in the Automotive Toolbox: ['Medium Range Radar'](https://dev.ti.com/tirex/explore/node?node=AObIl5yAPR-DjIwxgdgyuA__AocYeEd__LATEST). The following are simplified steps to prepare and run the lab. More detailed steps are found in the Getting Started Guide from the previous link.
	1. Read through 'Getting Started Guide' first to find prerequisites:
		* Download the Automotive Toolbox so that all the source files are on the local machine.
		* UniFlash to flash the precompiled binaries.
		* Install [XDS Emulation Software](https://software-dl.ti.com/ccs/esd/documents/xdsdebugprobes/emu_xds_software_package_download.html). If CCS is installed already, then XDS Emulation package was installed during the CCS installation process.
		* Install [Matlab runtime engine v8.5.1 32-bit](https://www.mathworks.com/products/compiler/matlab-runtime.html) to run the GUI. Only this version specfically will work. Also note that the GUI only works as a Windows executable. This was the only thing I did not have installed.
	1. Flash the binary file 'prebuilt_binaries/xwr18xx_mrr_demo.bin' using UniFlash.
	1. Execute the GUI in 'gui/MRRvisualization/MRRVisualizer.exe'.
		1. Changed the following parameters to attempt to better suit my environment (a small apartment).
			* 'Maximum range width (meters)' -- 1.5
			* 'Maximum range depth (meters)' -- 3
			* 'Maximum height      (meters)' -- 3
		1. Clicked 'Ok'.
	1. Walked around in front of the sensor, observing the changes. 
		* Still appeared to be approximately 2-3 inches off.
		* Stood around 62-63 inches away and readings showed around 1.5m (59 inches).
		* Plot points moved around rapidly, so it was hard to get a consistent reading.
		* [Example Plot.](https://i.imgur.com/oy8ltgu.png)
	1. Explored the Developer's Guide. Did not finish going through it, but there is a lot of good information regarding the system as well as how to integrate it with CCS.
