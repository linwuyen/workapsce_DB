using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TST_MODBUS_POLL
{
	public partial class Main
	{
		public UInt32[] u32Baudrate = new UInt32[]
	{1200,2400,4800,9600,19200,38400,76800,115200};

		public UInt32[] u32Databits = new UInt32[]
			{7,8};

		public UInt32[] u32Stopbits = new UInt32[]
			{1,2};

		public UInt32[] u32Parity = new UInt32[]
			{0,1,2};

		public string[] strParity = new string[]
			{"NONE", "ODD", "EVEN"};

		public string[] columnTitle = new string[]
{
			"Address","Name","Size","Type","Value","Bytes","Gain","Offset","Maximum","Minimum"
		};

		public int[] columnWidth = new int[]
		{
			 80,       190,   90,    60,    160,     70,     70,    70,      80,       80
		};


		const string _BIN = "Bin";
		const string _DEC = "Dec";
		const string _HEX = "Hex";
		const string _ASCII = "ASCII";

		public string[] cboxDataType = new string[]
		{
			_BIN, _DEC, _HEX, _ASCII
		};

		const string _U32 = "uint32_t";
		const string _U16 = "uint16_t";
		const string _U8 = "uint8_t";
		const string _S32 = "int32_t";
		const string _S16 = "int16_t";
		const string _S8 = "int8_t";
		const string _F32 = "float32_t";
		const string _Q15 = "q15";
		const string _Q31 = "q31";

		public string[] cboxDataLength = new string[]
		{
			_U8, _S8, _U16, _S16, _U32, _S32, _F32, _Q15, _Q31
		};


		const uint _SLAVE_ID = 0;
		const uint _FUNCTION_ID = 1;
		const uint _STARTING_ADDRESS_HI = 2;
		const uint _STARTING_ADDRESS_LO = 3;
		const uint _QUANTITY_REGISTERS_HI = 4;
		const uint _QUANTITY_REGISTERS_LO = 5;
		const uint _ERROR_CHECK_LO = 6;
		const uint _ERROR_CHECK_HI = 7;

		const uint _BYTE_COUNT = 6;
		const uint _DATA_0_HI = 7;
		const uint _DATA_0_LO = 8;

		const uint _WRITE_DATA_HI = 4;
		const uint _WRITE_DATA_LO = 5;

		const int _ERROR_TIMEOUT = -1;
		const int _ERROR_NO_CONNECTED = -2;
		const int _ERROR_CODE = -100;

		const Int32 TIME_INTERVAL_MSEC = 20;
	}
}
