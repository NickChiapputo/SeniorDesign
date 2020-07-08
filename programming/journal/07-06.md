# July 6
## Index
1. [Summary](#summary)
1. [Problems Encountered](#problems-encountered)
1. [Things Learned](#things-learned)
	1. [Definitions](#definitions)
1. [Steps Taken](#steps-taken)

## Summary
* Explored the main method of the lab00007 - Medium Range Radar project from the Automotive Toolbox
	* Found API documentation for the methods in the main method and explained the parameters and what the functions do.
* Learned what the Error Signalling Module (ESM) is from the TechRef document.
* Used the SYSBIOS API documentation (version 6.73.1.01 available online [here](https://software-dl.ti.com/dsps/dsps_public_sw/sdo_sb/targetcontent/sysbios/6_73_01_01/exports/bios_6_73_01_01/docs/cdoc/index.html); use the explorer on the left to navigate to the packages with ti -> sysbios -> <package_name>).
* Used MMWAVESDK to find ESM and SOC driver API documentation. Couldn't find it online, but it is available locally after installing the SDK.

## Problems Encountered
* It was pretty difficult finding where each function came from since there's no easy, centralized database to search from.
	* Solution: TI formats their function calls generally by adding the package name to the beginning of the name. For example, the function `BIOS_start()` is from the `BIOS.h` header file. Takes minimal understanding to know that this is from the SYSBIOS library (some are not as easy, but generally it is pretty straightforward once this relation is realized). 
	* Using this knowledge, it can be easy to find the local documentation from the installed packages (e.g., the `BIOS_start()` function is in `BIOS.html` in the SYSBIOS install directory; however, you have to navigate through docs/cdoc/ti/sysbios under the install directory to find the documentation).
* Didn't know the acronyms for ESM and SOC.
	* Found ESM in the TechRef document (after finding reference on an E2E forum post answer).
	* Was unable to find SOC reference that explained what it stood for. Not sure if it means System on Chip (but the 'O' is capitalized).

## Things Learned
### Definitions
* ESM - Error Signalling Module
	* Described on page 288 of the TechRef document.
* SOC - *unknown*

## Steps Taken
1. Explored lab00007 - Medium Range Radar from the Automotive Toolbox `mss_main.c` file `main()` method.
	* Learned about the following functions by exploring the MMWAVESDK documentation (installed locally with the SDK):
		* **`ESM_init`**
			* Used to initialize the ESM driver.
			* *This function is needed for all applications.*
			* Function is included in the `<ti/drivers/esm/esm.h>` header file.
			* Parameters:
				* uint8_t *`bClearErrors`*
					* Takes the value 0 or 1 to indicate if old ESM pending errors should be cleared or not.
					* If using TI RTOS, use 0 as RTOS does it. Otherwise, use 1 (or check the specific RTOS implementation).
		* **`SOC_init`**
			* Used to initialize the SOC driver.
			* *This function is needed for all applications.*
			* Function is included in the `<ti/drivers/soc/soc.h>` header file.
			* Parameters passed to the function:
				* SOC_Cfg \* *`ptrCfg`*
					* Pointer to an SOC configuration object.
				* uint32_t \* *`errCode`*
					* Error code populated on an error in the initialization.
			* Return Value:
				* Returns a pointer to the SOC driver on success.
				* Returns a NULL pointer on error.
				* This is why there is an 'if' statement after the function call checking if the value is NULL.
		* **`Task_Params_init`**
			* Used to initialize a Task_Params object with default values.
			* *This function is needed for all applications.* (I think)
			* Function is included in the `<ti/sysbios/knl/Task.h>` header fie.
			* Parameters:
				* Task_Params \* *`params`*
					* Pointer to a task parameters object.
		* **`Task_create`**
			* Used to create a new task to execute. 
			* Priority value is set in the parameter struct to prevent it running before other system critical threads. The system runs tasks in order of priority. That is, no currently running task will have a priority higher than any waiting task (this describes the task scheduler).
			* *This function is probably needed for all applications.*
			* Function is included in the `<ti/sysbios/knl/Task.h>` header file.
			* Parameters:
				* Task_FuncPty *`fxn`*
					* Pointer to a C function. This doesn't need to be typecasted (it isn't in this lab), though it can be good for readability (and error prevention in other cases) to do so.
				* Task_Params \* *`params`*
					* Pointer to a task parameters object. Make sure the priority field is set before the function is called. This struct defines the parameters for the task.
				* Error_Block \* *`eb`*
					* Active error-handling block that can be used to handle errors that may occur during Task object creation. If left NULL, default policy is selected (I don't know what this means in terms of TI RTOS or SYSBIOS, whichever actually handles it here).
			* Return Value:
				* Task_Handle object. If successful, returns the handle of the new task object. If unsuccessful, returns NULL unless it aborts.
		* **`BIOS_start`**
			* Used to perform any SYS/BIOS initializations that are remaining.
			* Transfers control to the highest priority task if `taskEnabled` is true. Otherwise, control is transferred directly to the Idle Loop.
			* Required to call this function after performing other initializations.
			* *This function is needed for all applications.*
			* Function is included in the `<ti/sysbios/BIOS.h>` header file.
			* Parameters: None
			* Return Value: None
	* After `BIOS_start()` is called, I believe the program flow continues on to the `MRR_MSS_initTask()` function.