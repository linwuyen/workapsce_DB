using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using mcp2221_dll_m;


/*
© [2021] Microchip Technology Inc. and its subsidiaries.

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
Previous steps: The corresponding .dll file was added in reference tab.
Prerequisites: Microsoft Visual C++ 2010 Redistributable Package.
In this example we've used a PICkit Serial I2C Demo Board. Also, this example will work only as expected only if a single MCP2221A is connected.
Description: This is a simple "visual" example on how to use the MCP23008 I/Os. The I/O pins are connected to 8 LEDs. We will make all the I/Os as
GPIO outputs and then we will send a hex value to the MCP23008's I/O port, showing a LED pattern(for MAX_NUMBER_OF_CYCLES).
*/



namespace Managed
{
    class Program
    {
        //======================================
        const int DEFAULT_VID = 0x04D8;
        const int DEFAULT_PID = 0x00DD;
        const int BYTES_TO_WRITE = 2;
        const int NR_OF_LEDS = 8;
        const int MAX_NUMBER_OF_CYCLES = 10;

        static int result = 0;
        static IntPtr mcpHandle = new IntPtr();
        static uint mcpIndex = 0;
        static uint numberOfDevices = 0;

        static byte[] i2cTxData = new byte[BYTES_TO_WRITE];
        static byte slaveAddressExpander = 0x20;
        static byte use7bitAddress = 1;
        static byte[] patternExpander = { 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80 };

        static void Main(string[] args)
        {

            String libVersion = MCP2221.M_Mcp2221_GetLibraryVersion();

            if (result == MCP2221.M_E_NO_ERR)
            {
                Console.WriteLine("Lib version: {0}", libVersion);

                result = MCP2221.M_Mcp2221_GetConnectedDevices(DEFAULT_VID, DEFAULT_PID, ref numberOfDevices);

                if (result == MCP2221.M_E_NO_ERR)
                {
                    Console.WriteLine("Number of Devices: {0}", numberOfDevices);

                    if (numberOfDevices > 0)
                    {
                        mcpHandle = MCP2221.M_Mcp2221_OpenByIndex(DEFAULT_VID, DEFAULT_PID, mcpIndex);
                        result = MCP2221.M_Mcp2221_GetLastError();

                        if (result == MCP2221.M_E_NO_ERR)
                        {
                            // Set the I2C speed to 100000bps.
                            result = MCP2221.M_Mcp2221_SetSpeed(mcpHandle, 100000);

                            if (result == MCP2221.M_E_NO_ERR)
                            {
                                // Made all the MCP23008 GPIO outputs
                                i2cTxData[0] = 0x00;
                                i2cTxData[1] = 0x00;
                                result = MCP2221.M_Mcp2221_I2cWrite(mcpHandle, BYTES_TO_WRITE, slaveAddressExpander, use7bitAddress, i2cTxData);
                                if (result != MCP2221.M_E_NO_ERR)
                                {
                                    Console.WriteLine("ERROR >> Mcp2221_I2cWrite() returned: {0}", result);
                                    return;
                                }
                                Console.WriteLine("Start sending pattern");

                                int ledPosition = 0;
                                int numberOfCycles = 0;
                                while (numberOfCycles < (NR_OF_LEDS * MAX_NUMBER_OF_CYCLES))
                                {
                                    i2cTxData[0] = 0x0A;
                                    i2cTxData[1] = patternExpander[ledPosition];
                                    result = MCP2221.M_Mcp2221_I2cWrite(mcpHandle, BYTES_TO_WRITE, slaveAddressExpander, use7bitAddress, i2cTxData);
                                    if (result != MCP2221.M_E_NO_ERR)
                                    {
                                        Console.WriteLine("ERROR >> Mcp2221_I2cWrite() returned: {0}", result);
                                        return;
                                    }
                                    ledPosition++;
                                    numberOfCycles++;
                                    if (ledPosition == NR_OF_LEDS)
                                    {
                                        ledPosition = 0;
                                    }
                                    Thread.Sleep(100);
                                }
                            }
                            else
                            {
                                Console.WriteLine("ERROR >> Mcp2221_SetSpeed() returned: {0}", result);
                            }

                            result = MCP2221.M_Mcp2221_Close(mcpHandle);
                        }
                        else
                        {
                            Console.WriteLine("ERROR >> Mcp2221_GetLastError() returned: {0} ", result);
                        }
                    }
                    else
                    {
                        Console.WriteLine("ERROR >> NO DEVICES CONNECTED !");
                    }
                }
                else
                {
                    Console.WriteLine("ERROR >> Mcp2221_GetConnectedDevices() returned: {0} ", result);
                }
            }
            else
            {
                Console.WriteLine("ERROR >> Mcp2221_GetLibraryVersion() returned: {0}", result);
            }
        }
    }
}
