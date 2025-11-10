/*
?[2021] Microchip Technology Inc. and its subsidiaries.

Subject to your compliance with these terms, you may use Microchip software and
any derivatives exclusively with Microchip products. It is your responsibility
to comply with third party license terms applicable to your use of third party
software (including open source software) that may accompany Microchip software.

THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER EXPRESS,
IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES
OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.

IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER
RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF
THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY
LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY RELATED TO THIS
SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY
TO MICROCHIP FOR THIS SOFTWARE.
*/

/*
Previous steps: The corresponding .lib file for each distribution was added (could be found in Solution settings: Linker->Input->Additional Dependencies)
Prerequisites: In this example we've used a PICkit Serial I2C Demo Board. Also, this example will work only as expected only if a single MCP2221A is connected.
Description: This is a simple "visual" example on how to use the MCP23008 I/Os. The I/O pins are connected to 8 LEDs. We will make all the I/Os as
GPIO outputs and then we will send a hec value to the MCP23008's I/O port, showing a LED pattern(for MAX_NUMBER_OF_CYCLES).
*/

#include <iostream>
#include <windows.h>
//======================================
#define MCP2221_LIB        1	//for projects importing the .lib, use the MCP2221_LIB preprocessor definition
#include "mcp2221_dll_um.h"

//======================================
#define DEFAULT_VID                     0x04D8
#define DEFAULT_PID                     0x00DD
#define BYTES_TO_WRITE    2
#define NR_OF_LEDS		  8
#define MAX_NUMBER_OF_CYCLES	 10

wchar_t libVersion[64];
int result = 0;
void* mcpHandle = NULL;
unsigned int mcpIndex = 0;
unsigned int numberOfDevices = 0;

unsigned char i2cTxData[BYTES_TO_WRITE];
unsigned char slaveAddressExpander = 0x20;
unsigned char use7bitAddress = 1;
const unsigned char patternExpander[NR_OF_LEDS] = { 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80 };


//======================================

int main()
{
	result = Mcp2221_GetLibraryVersion(libVersion);

	if (result == E_NO_ERR)
	{
		std::wcout << "Library Version: " << libVersion << "\n";

		result = Mcp2221_GetConnectedDevices(DEFAULT_VID, DEFAULT_PID, &numberOfDevices);

		if (result == E_NO_ERR)
		{
			std::cout << "Number of Devices: " << numberOfDevices << "\n";

			if (numberOfDevices > 0)
			{
				mcpHandle = Mcp2221_OpenByIndex(DEFAULT_VID, DEFAULT_PID, mcpIndex);
				result = Mcp2221_GetLastError();

				if (result == E_NO_ERR)
				{
					// Set the I2C speed to 100000bps.
					result = Mcp2221_SetSpeed(mcpHandle, 100000);

					if (result == E_NO_ERR)
					{
						// Made all the MCP23008 GPIO outputs
						i2cTxData[0] = 0x00;
						i2cTxData[1] = 0x00;
						result = Mcp2221_I2cWrite(mcpHandle, BYTES_TO_WRITE, slaveAddressExpander, use7bitAddress, i2cTxData);
						if (result != E_NO_ERR)
						{
							std::cout << "ERROR >> Mcp2221_I2cWrite() returned: " << result << "\n";
							return result;
						}
						std::cout << "Send pattern, use CTRL+C to stop the program! ";

						int idx = 0;
						int numberOfCycles = 0;
						while (numberOfCycles < (NR_OF_LEDS * MAX_NUMBER_OF_CYCLES))
						{
							i2cTxData[0] = 0x0A;
							i2cTxData[1] = patternExpander[idx];
							result = Mcp2221_I2cWrite(mcpHandle, BYTES_TO_WRITE, slaveAddressExpander, use7bitAddress, i2cTxData);
							if (result != E_NO_ERR)
							{
								std::cout << "ERROR >> Mcp2221_I2cWrite() returned: " << result << "\n";
								return result;
							}
							idx++;
							numberOfCycles++;
							if (idx == NR_OF_LEDS)
							{
								idx = 0;
							}
							Sleep(100);
						}
					}
					else
					{
						std::cout << "ERROR >> Mcp2221_SetSpeed() returned: " << result << "\n";
					}

					result = Mcp2221_Close(mcpHandle);
				}
				else
				{
					std::cout << "ERROR >> Mcp2221_I2cWrite() returned: " << mcpHandle << "\n";
					std::cout << "ERROR >> Mcp2221_GetLastError() returned: " << result << "\n";
				}
			}
			else
			{
				std::cout << "ERROR >> NO DEVICES CONNECTED !\n" << "\n";
			}
		}
		else
		{
			std::cout << "ERROR >> Mcp2221_GetConnectedDevices() returned: " << result << "\n";
		}
	}
	else
	{
		std::cout << "ERROR >> Mcp2221_GetLibraryVersion() returned: " << result << "\n";
	}

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu


