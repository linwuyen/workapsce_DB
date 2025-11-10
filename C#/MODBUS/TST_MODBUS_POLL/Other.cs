using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.IO.Ports;
using System.Linq;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Input;

namespace TST_MODBUS_POLL
{
	public partial class Main : Form
	{

		private void getComPortList()
		{
			// Get the available COM ports
			string[] ports = SerialPort.GetPortNames();

			ComPortList.Items.Clear();
			// Add the COM ports to the ComboBox
			ComPortList.Items.AddRange(ports);

			if (0 < ports.Length)
			{
				if(-1 == ComPortList.SelectedIndex)
				{
					ComPortList.SelectedIndex = 0;
				}				
			}
		}

		private void DelayMicroseconds(int microseconds)
		{
			var stopwatch = Stopwatch.StartNew();
			long elapsedTicks = 0;

			while (elapsedTicks < microseconds * Stopwatch.Frequency / 1000000)
			{
				elapsedTicks = stopwatch.ElapsedTicks;
				Thread.SpinWait(1); // Allow other threads to execute
			}
		}
		private void buildDataGrid()
		{
			int i = 0;
			foreach (string name in columnTitle)
			{
				if ("Size" == name)
				{
					DataGridViewComboBoxColumn cbx = new DataGridViewComboBoxColumn();
					cbx.HeaderText = name;
					cbx.Name = name;

					foreach (string ss in cboxDataLength)
					{
						cbx.Items.Add(ss);
					}
					dbDataSets.Columns.Add(cbx);
				}
				else if ("Type" == name)
				{
					DataGridViewComboBoxColumn cbx = new DataGridViewComboBoxColumn();
					cbx.HeaderText = name;
					cbx.Name = name;

					foreach (string ss in cboxDataType)
					{
						cbx.Items.Add(ss);
					}
					dbDataSets.Columns.Add(cbx);
				}
				else
				{
					dbDataSets.Columns.Add(name, name);

				}
				dbDataSets.Columns[name].SortMode = DataGridViewColumnSortMode.NotSortable;
				dbDataSets.Columns[name].Width = columnWidth[i++];
			}

			dbDataSets.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
			dbDataSets.AutoResizeColumns();
		}

		private const string ConfigFileName = "config";
		private const string InitialFileName = "init.csv";

		private void ExportData()
		{
			string cfgfs = ConfigFileName;
			string cfgpth = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, cfgfs);

			using (StreamWriter writer = new StreamWriter(cfgpth))
			{
				string strUartConfig = "";

				strUartConfig += (ComPortList.Text + ",");
				strUartConfig += (BaudrateList.Text + ",");
				strUartConfig += (DatabitsList.Text + ",");
				strUartConfig += (intSlaveID.Value.ToString() + ",");
				strUartConfig += (intTimeoutMsec.Value.ToString() + ",");
				strUartConfig += (intAddress.Value.ToString() + ",");
				strUartConfig += (intIntervalMsec.Value.ToString() + ",");
				strUartConfig += (dbDataSets.Font.Size.ToString());
				writer.Write(strUartConfig);
				writer.WriteLine();
			}


			string initfs = InitialFileName;
			string initpth = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, initfs);

			if(0 == intDataSets.Value)
			{
				MessageBox.Show("No Data in bellow the data grid.");
				return;
			}

			using (StreamWriter writer = new StreamWriter(initpth))
			{
				// Write the column headers to the file
				for (int i = 0; i < dbDataSets.Columns.Count; i++)
				{
					writer.Write(dbDataSets.Columns[i].HeaderText);
					if (i < dbDataSets.Columns.Count - 1)
					{
						writer.Write(",");
					}
				}
				writer.WriteLine();

				// Write the data rows to the file
				foreach (DataGridViewRow row in dbDataSets.Rows)
				{
					if (row.Visible) writer.Write("Show,");
					else writer.Write("Hide,");
					writer.Write(row.DefaultCellStyle.ForeColor.Name + ",");
					writer.Write(row.DefaultCellStyle.BackColor.Name + ",");

					for (int j = 0; j < dbDataSets.Columns.Count; j++)
					{
						writer.Write(row.Cells[j].Value);
						if (j < dbDataSets.Columns.Count - 1)
						{
							writer.Write(",");
						}
					}
					
					writer.WriteLine();
				}

				smsg.Text = "Export init.csv in the app folder!";
			}
		}

		private void ImportData()
		{
			string cfgfs = ConfigFileName;
			string cfgpth = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, cfgfs);

			if (File.Exists(cfgpth))
			{
				using (StreamReader reader = new StreamReader(cfgpth))
				{
					string strComponent = reader.ReadLine();
					string[] strTemp = strComponent.Split(',');

					if (!ComPortList.Items.Contains(strTemp[0]))
					{
						MessageBox.Show(strTemp[0] + " is not available.");
					}
					else
					{
						ComPortList.Text = strTemp[0];
					}
					BaudrateList.Text = strTemp[1];
					DatabitsList.Text = strTemp[2];
					intSlaveID.Value = int.Parse(strTemp[3]);
					intTimeoutMsec.Value = int.Parse(strTemp[4]);
					intAddress.Value = int.Parse(strTemp[5]);
					intIntervalMsec.Value = int.Parse(strTemp[6]);
					dbDataSets.Font = new Font("Consolas", int.Parse(strTemp[7]), FontStyle.Regular);
				}
			}

			string fileName = InitialFileName;
			string filePath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, fileName);

			if (File.Exists(filePath))
			{
				using (StreamReader reader = new StreamReader(filePath))
				{
					UInt16 u16LineCnts = 0;
					// Clear the existing data in the DataGridView
					dbDataSets.Rows.Clear();
					dbDataSets.Columns.Clear();

					// Read the column headers from the file
					string headerLine = reader.ReadLine();
					while (!reader.EndOfStream)
					{
						string[] values = headerLine.Split(',');
						if("Address" != values[0])
						{
							headerLine = reader.ReadLine();
						}
						else
						{
							break;
						}
					}

					buildDataGrid();

					// Read the data rows from the file
					while (!reader.EndOfStream)
					{
						string dataLine = reader.ReadLine();
						string[] values = dataLine.Split(',');

						string strShow = values[0];
						string strForeColor = values[1];
						string strBackColor = values[2];
						string[] strdata = new string[values.Length - 3];
						Array.Copy(values, 3, strdata, 0, strdata.Length);
						dbDataSets.Rows.Add(strdata);

						if ("Hide" == strShow) dbDataSets.Rows[dbDataSets.Rows.Count - 1].Visible = false;

						dbDataSets.Rows[dbDataSets.Rows.Count - 1].DefaultCellStyle.ForeColor = Color.FromName(strForeColor);
						dbDataSets.Rows[dbDataSets.Rows.Count - 1].DefaultCellStyle.BackColor = Color.FromName(strBackColor);
					}
					smsg.Text = "Import successful!!";
				}
			}
			else
			{
				// File does not exist
				// Handle the case when the file is not found, if needed
			}
		}



		const UInt16 colAddress = 0;
		const UInt16 colDataName = 1;
		const UInt16 colDataSize = 2;
		const UInt16 colDataType = 3;
		const UInt16 colDataValue = 4;
		const UInt16 colDataBytes = 5;
		const UInt16 colDataGain = 6;
		const UInt16 colDataOffset = 7;
		const UInt16 colDataMaximum = 8;
		const UInt16 colDataMinimum = 9;

		const UInt16 listBinary = 0;
		const UInt16 listDecimal = 1;
		const UInt16 listHex = 2;
		const UInt16 listASCII = 3;

		static string[] listDataType = new string[]
		{
			_BIN,_DEC,_HEX,_ASCII
		};

		delegate void OBJ_FUNC(object sender, DataGridViewCellEventArgs e);

		static void updateAddress(object sender, DataGridViewCellEventArgs e)
		{
			//Debug.Print("updateAddress");


			DataGridView db = ((DataGridView)sender);


			// Get the row index and column index of the changed cell
			int rowIndex = e.RowIndex;
			int columnIndex = e.ColumnIndex;
			int rowcounts = db.Rows.Count;

			if (0 < rowcounts)
			{
				for (int i = rowIndex; i < rowcounts; i++)
				{
					int getAddress = int.Parse(db.Rows[i].Cells[colAddress].Value.ToString());

					if ((i + 1) < rowcounts) // Make sure whether there is the next item.
					{
						if (null != db.Rows[i].Cells[colDataBytes].Value)
						{
							if (int.TryParse(db.Rows[i].Cells[colDataBytes].Value.ToString(), out int parseint))
							{
								db.Rows[i + 1].Cells[colAddress].Value = getAddress + parseint / 2;
							}
						}
						else
						{
							db.Rows[i + 1].Cells[colAddress].Value = getAddress + 1;
						}
					}
				}
			}
		}

		static void updateName(object sender, DataGridViewCellEventArgs e)
		{
			Debug.Print("updateName");
		}

		static void updateType(object sender, DataGridViewCellEventArgs e)
		{
			Debug.Print("updateType");

			DataGridView db = ((DataGridView)sender);

			// Get the row index and column index of the changed cell
			int rowIndex = e.RowIndex;
			int columnIndex = e.ColumnIndex;

			object colSize = db.Rows[rowIndex].Cells[colDataType].Value;

			switch (colSize)
			{
				case _BIN:
				case _DEC:
				case _HEX:
					Debug.Print("Nothing to do.");
					break;
				case _ASCII:
					db.Rows[rowIndex].Cells[colDataBytes].Value = null;
					break;

				default:
					//MessageBox.Show("Undefine the type of data.");
					break;
			}
		}

		static bool fgEditing = false;
		static string tmpOriginValue = "";


		static void updateValue(object sender, DataGridViewCellEventArgs e)
		{
			//Debug.Print("updateValue");

			//DataGridView db = ((DataGridView)sender);



		}



		static void updateBytes(object sender, DataGridViewCellEventArgs e)
		{
			Debug.Print("updateBytes");

			DataGridView db = ((DataGridView)sender);

			// Get the row index and column index of the changed cell
			int rowIndex = e.RowIndex;
			int columnIndex = e.ColumnIndex;

			object colSize = db.Rows[rowIndex].Cells[colDataType].Value;

			switch (colSize)
			{
				case _BIN:
				case _DEC:
				case _HEX:
					updateSize(sender, e);
					break;
				case _ASCII:
					if (null != db.Rows[rowIndex].Cells[colDataBytes].Value)
					{
						if (int.TryParse(db.Rows[rowIndex].Cells[colDataBytes].Value.ToString(), out int parseValue))
						{
							if (0 < (parseValue % 2))
							{
								db.Rows[rowIndex].Cells[colDataBytes].Value = parseValue + 1;
							}
							else
							{
								db.Rows[rowIndex].Cells[colDataBytes].Value = parseValue;
							}
						}
					}

					break;

				default:
					break;
			}

			updateAddress(sender, e);


		}

		static void updateSize(object sender, DataGridViewCellEventArgs e)
		{
			Debug.Print("updateSize");

			DataGridView db = ((DataGridView)sender);

			// Get the row index and column index of the changed cell
			int rowIndex = e.RowIndex;
			int columnIndex = e.ColumnIndex;

			object colSize = db.Rows[rowIndex].Cells[colDataSize].Value;

			switch (colSize)
			{
				case _U8:
				case _S8:
				case _U16:
				case _S16:
					db.Rows[rowIndex].Cells[colDataBytes].Value = 2;
					db.Rows[rowIndex].Cells[colDataType].Value = listDataType[listHex];
					break;
				case _Q15:
					db.Rows[rowIndex].Cells[colDataBytes].Value = 2;
					db.Rows[rowIndex].Cells[colDataType].Value = listDataType[listDecimal];
					break;
				case _U32:
				case _S32:
					db.Rows[rowIndex].Cells[colDataBytes].Value = 4;
					db.Rows[rowIndex].Cells[colDataType].Value = listDataType[listHex];
					break;
				case _F32:
				case _Q31:
					db.Rows[rowIndex].Cells[colDataBytes].Value = 4;
					db.Rows[rowIndex].Cells[colDataType].Value = listDataType[listDecimal];
					break;

				default:
					//MessageBox.Show("Undefine the size of data.");
					break;
			}

		}

		static void updateGainOffset(object sender, DataGridViewCellEventArgs e)
		{
			Debug.Print("updateLimitation");
		}


		static void updateLimitation(object sender, DataGridViewCellEventArgs e)
		{
			Debug.Print("updateLimitation");
		}

		OBJ_FUNC[] colfunc = new OBJ_FUNC[]
		{
			updateAddress,
			updateName,
			updateSize,
			updateType,
			updateValue,
			updateBytes,
			updateGainOffset,
			updateGainOffset,
			updateLimitation,
			updateLimitation
		};

		ushort[] wCRCTable = new ushort[]
		{
		   0X0000, 0XC0C1, 0XC181, 0X0140, 0XC301, 0X03C0, 0X0280, 0XC241,
		   0XC601, 0X06C0, 0X0780, 0XC741, 0X0500, 0XC5C1, 0XC481, 0X0440,
		   0XCC01, 0X0CC0, 0X0D80, 0XCD41, 0X0F00, 0XCFC1, 0XCE81, 0X0E40,
		   0X0A00, 0XCAC1, 0XCB81, 0X0B40, 0XC901, 0X09C0, 0X0880, 0XC841,
		   0XD801, 0X18C0, 0X1980, 0XD941, 0X1B00, 0XDBC1, 0XDA81, 0X1A40,
		   0X1E00, 0XDEC1, 0XDF81, 0X1F40, 0XDD01, 0X1DC0, 0X1C80, 0XDC41,
		   0X1400, 0XD4C1, 0XD581, 0X1540, 0XD701, 0X17C0, 0X1680, 0XD641,
		   0XD201, 0X12C0, 0X1380, 0XD341, 0X1100, 0XD1C1, 0XD081, 0X1040,
		   0XF001, 0X30C0, 0X3180, 0XF141, 0X3300, 0XF3C1, 0XF281, 0X3240,
		   0X3600, 0XF6C1, 0XF781, 0X3740, 0XF501, 0X35C0, 0X3480, 0XF441,
		   0X3C00, 0XFCC1, 0XFD81, 0X3D40, 0XFF01, 0X3FC0, 0X3E80, 0XFE41,
		   0XFA01, 0X3AC0, 0X3B80, 0XFB41, 0X3900, 0XF9C1, 0XF881, 0X3840,
		   0X2800, 0XE8C1, 0XE981, 0X2940, 0XEB01, 0X2BC0, 0X2A80, 0XEA41,
		   0XEE01, 0X2EC0, 0X2F80, 0XEF41, 0X2D00, 0XEDC1, 0XEC81, 0X2C40,
		   0XE401, 0X24C0, 0X2580, 0XE541, 0X2700, 0XE7C1, 0XE681, 0X2640,
		   0X2200, 0XE2C1, 0XE381, 0X2340, 0XE101, 0X21C0, 0X2080, 0XE041,
		   0XA001, 0X60C0, 0X6180, 0XA141, 0X6300, 0XA3C1, 0XA281, 0X6240,
		   0X6600, 0XA6C1, 0XA781, 0X6740, 0XA501, 0X65C0, 0X6480, 0XA441,
		   0X6C00, 0XACC1, 0XAD81, 0X6D40, 0XAF01, 0X6FC0, 0X6E80, 0XAE41,
		   0XAA01, 0X6AC0, 0X6B80, 0XAB41, 0X6900, 0XA9C1, 0XA881, 0X6840,
		   0X7800, 0XB8C1, 0XB981, 0X7940, 0XBB01, 0X7BC0, 0X7A80, 0XBA41,
		   0XBE01, 0X7EC0, 0X7F80, 0XBF41, 0X7D00, 0XBDC1, 0XBC81, 0X7C40,
		   0XB401, 0X74C0, 0X7580, 0XB541, 0X7700, 0XB7C1, 0XB681, 0X7640,
		   0X7200, 0XB2C1, 0XB381, 0X7340, 0XB101, 0X71C0, 0X7080, 0XB041,
		   0X5000, 0X90C1, 0X9181, 0X5140, 0X9301, 0X53C0, 0X5280, 0X9241,
		   0X9601, 0X56C0, 0X5780, 0X9741, 0X5500, 0X95C1, 0X9481, 0X5440,
		   0X9C01, 0X5CC0, 0X5D80, 0X9D41, 0X5F00, 0X9FC1, 0X9E81, 0X5E40,
		   0X5A00, 0X9AC1, 0X9B81, 0X5B40, 0X9901, 0X59C0, 0X5880, 0X9841,
		   0X8801, 0X48C0, 0X4980, 0X8941, 0X4B00, 0X8BC1, 0X8A81, 0X4A40,
		   0X4E00, 0X8EC1, 0X8F81, 0X4F40, 0X8D01, 0X4DC0, 0X4C80, 0X8C41,
		   0X4400, 0X84C1, 0X8581, 0X4540, 0X8701, 0X47C0, 0X4680, 0X8641,
		   0X8201, 0X42C0, 0X4380, 0X8341, 0X4100, 0X81C1, 0X8081, 0X4040 
		};

		ushort CRC16(byte[] nData, ushort wLength)
		{
			byte nTemp;
			ushort wCRCWord = 0xFFFF;
			int i = 0;

			while (i < wLength)
			{
				nTemp = (byte)(nData[i] ^ wCRCWord);
				wCRCWord >>= 8;
				wCRCWord ^= wCRCTable[nTemp];
				i++;
			}
			return wCRCWord;

		} // End: CRC16


		string bitString(byte[] u8data)
		{
			StringBuilder bs = new StringBuilder();

			foreach (byte b in u8data)
			{
				bs.Append(Convert.ToString(b, 2).PadLeft(8, '0'));
				bs.Append('_');
			}

			bs.Length--;

			return bs.ToString();
		}

		string decString(byte[] u8data)
		{
			UInt32 u32Data = 0;

			for (int i = 0; i< u8data.Length; i++)
			{
				u32Data |= ((UInt32)u8data[(u8data.Length-1)-i] << (i * 8));
			}

			//u32Data = (UInt32)u8data[0] << 8;
			//u32Data |= (UInt32)u8data[1] << 0;

			return u32Data.ToString(); 
		}

		uint swapU32(byte[] u8data)
		{
			uint swappedValue = ((uint)u8data[0] << 24) |
					((uint)u8data[1] << 16) |
					((uint)u8data[2] << 8) |
					u8data[3];

			return swappedValue;
		}

		void swapBytesByWord(byte[] u8data)
		{
			byte[] swapWord = new byte[4];

			swapWord[3] = u8data[1];
			swapWord[2] = u8data[0];
			swapWord[1] = u8data[3];
			swapWord[0] = u8data[2];

			u8data[0] = swapWord[0];
			u8data[1] = swapWord[1];
			u8data[2] = swapWord[2];
			u8data[3] = swapWord[3];
		}


		private string showDecimalStringInRow(DataGridViewRow row, byte[] pBytes)
		{
			Double d64Temp = 0;
			string strTemp = "";
			// Resize the array of Bytes
			switch (row.Cells[colDataSize].Value)
			{
				case _U8:
				case _S8:
					Array.Resize(ref pBytes, 1);
					break;
				case _U16:
				case _S16:
				case _Q15:
					Array.Resize(ref pBytes, 2);
					break;
				case _U32:
				case _S32:
				case _F32:
				case _Q31:
					Array.Resize(ref pBytes, 4);
					break;
				default:
					break;
			}

			// Rebuild the real value into d64Temp
			switch (row.Cells[colDataSize].Value)
			{
				case _U8:
					d64Temp = (Double)((byte)pBytes[0]);
					break;
				case _S8:
					d64Temp = (Double)((sbyte)pBytes[0]);
					break;
				case _U16:
					d64Temp = (Double)((ushort)(((ushort)pBytes[0] << 8) | ((ushort)pBytes[1] << 0)));
					break;
				case _S16:
					d64Temp = (Double)((short)(((ushort)pBytes[0] << 8) | ((ushort)pBytes[1] << 0)));
					break;
				case _Q15:
					d64Temp = (Double)((short)(((ushort)pBytes[0] << 8) | ((ushort)pBytes[1] << 0)));
					d64Temp /= 32768.0f;

					break;
				case _U32:
					d64Temp = (Double)((UInt32)(((UInt32)pBytes[0] << 24)
											  | ((UInt32)pBytes[1] << 16)
											  | ((UInt32)pBytes[2] << 8)
											  | ((UInt32)pBytes[3] << 0)));
					break;
				case _S32:
					d64Temp = (Double)((Int32)(((Int32)pBytes[0] << 24)
											 | ((Int32)pBytes[1] << 16)
											 | ((Int32)pBytes[2] << 8)
											 | ((Int32)pBytes[3] << 0)));
					break;
				case _F32:
					Array.Reverse(pBytes);
					d64Temp = (Double)BitConverter.ToSingle(pBytes, 0);
					break;
				case _Q31:
					d64Temp = (Double)((Int32)(((Int32)pBytes[0] << 24)
											 | ((Int32)pBytes[1] << 16)
											 | ((Int32)pBytes[2] << 8)
											 | ((Int32)pBytes[3] << 0)));
					d64Temp /= 2147483648.0;

					break;
				default:
					break;
			}

			double d64Gain = 1.0;
			double d64Offset = 0.0;

			if(null != row.Cells[colDataGain].Value)
			{
				if (true == double.TryParse(row.Cells[colDataGain].Value.ToString(), out double d64GainTemp))
				{
					d64Gain = d64GainTemp;
				}
			}

			if(null != row.Cells[colDataOffset].Value)
			{
				if (true == double.TryParse(row.Cells[colDataOffset].Value.ToString(), out double d64OffsetTemp))
				{
					d64Offset = d64OffsetTemp;
				}
			}

			d64Temp = d64Temp * d64Gain + d64Offset;

			strTemp = d64Temp.ToString("0.#####");


			return strTemp;

			
		}

		private void refreshDateString()
		{
			int i = 3; //

			foreach (DataGridViewRow row in dbDataSets.Rows)
			{
				if (row.Cells[colDataBytes].Value != null && uint.TryParse(row.Cells[colDataBytes].Value.ToString(), out uint cellValue))
				{
					byte[] pBytes = new byte[cellValue];
					string strTemp = "";

					for (int j = 0; j < cellValue; j++)
					{
						pBytes[j] = rxModbusBuffer[i++];
					}

					// Swap 4bytes by hi lo word
					switch (row.Cells[colDataSize].Value)
					{
						case _U8:
						case _S8:
							{
								int j, k;
								for (j = 0, k = 0; j < int.Parse(row.Cells[colDataBytes].Value.ToString()) / 2; j++, k += 2)
								{
									pBytes[j] = pBytes[k + 1];
								}

								Array.Resize(ref pBytes, j);
							}

							break;

						case _U32:
						case _S32:
						case _F32:
						case _Q31:
							swapBytesByWord(pBytes);
							break;
						default:
							break;
					}


					switch (row.Cells[colDataType].Value)
					{
						case _BIN:
							strTemp = "0b" + bitString(pBytes);

							break;

						case _DEC:
							strTemp = showDecimalStringInRow(row, pBytes);
							// row.Cells[colDataValue].Value = decString(pBytes);
							break;

						case _ASCII:
							strTemp = Encoding.ASCII.GetString(pBytes);

							break;

						case _HEX:
						default:
							strTemp = "0x" + BitConverter.ToString(pBytes).Replace("-", "");
							break;
					}

					row.Cells[colDataValue].Value = strTemp;
				}
			}
		}


		private void pollDataSets()
		{


			int rsize = readHoldingRegisters();
			if (0 < rsize)
			{
				ushort crc = 0;
				crc = CRC16(rxModbusBuffer, (ushort)rsize);

				if (0 == crc)
				{
					refreshDateString();
					smsg.Text = "";
					timerInterval.Start();
				}
				else
				{
					smsg.Text = "Get a error CRC.";
					//MessageBox.Show("Get a error CRC. ", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}
		}



	}
}
