# July 6
## Summary


## Problems Encountered


## Things Learned
### Definitions
* ESM - Error Signalling Module
	* Described on page 288 of the TechRef document.
* SOC - 
* MRR - 

## Steps Taken
1. Explored lab00007 - Medium Range Radar from the Automotive Toolbox.
	1. Explored the file 'mss_main.c'
		<details>
			<summary>Explored the main method.</summary>
			* Learned about the following functions by exploring the MMWAVESDK documentation (installed locally with the SDK):
				* **`ESM_init`**
					* Used to initialize the ESM driver.
					* *This function is needed for all applications.*
					* Function is included in the `<ti/drivers/esm/esm.h>` header file.
					* Parameters:
						* uint8_t *bClearErrors*
							* Takes the value 0 or 1 to indicate if old ESM pending errors should be cleared or not.
							* If using TI RTOS, use 0 as RTOS does it. Otherwise, use 1 (or check the specific RTOS implementation).
				* **`SOC_init`**
					* Used to initialize the SOC driver.
					* *This function is needed for all applications.*
					* Function is included in the `<ti/drivers/soc/soc.h>` header file.
					* Parameters passed to the function:
						* SOC_Cfg \* *ptrCfg*
							* Pointer to an SOC configuration object.
						* uint32_t \* *errCode*
							* Error code populated on an error in the initialization.
					* Return value
						* Returns a pointer to the SOC driver on success.
						* Returns a NULL pointer on error.
						* This is why there is an 'if' statement after the function call checking if the value is NULL.
				* **`Task_Params_init`**
					* Used to initialize a Task_Params object with default values.
					* *This function is needed for all applications.* (I think)
					* Function is included in the `<ti/sysbios/knl/Task.h>` header fie.
					* Parameters:
						* Task_Params \* *params*
							* Pointer to a task parameters object.
			Learned about the following variables:
				* 
		</details>