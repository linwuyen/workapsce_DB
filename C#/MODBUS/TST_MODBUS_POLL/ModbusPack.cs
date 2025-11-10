using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TST_MODBUS_POLL
{
	public partial class Main
	{
		byte[] rdHoldRegisterPack =
{
			0x00, // _SLAVE_ID
			0x03, // _FUNCTION_ID
			0x00, // _STARTING_ADDRESS_HI
			0x00, // _STARTING_ADDRESS_LO
			0x00, // _QUANTITY_REGISTERS_HI
			0x00, // _QUANTITY_REGISTERS_LO
			0x00, // _ERROR_CHECK_LO
			0x00  // _ERROR_CHECK_HI
		};

		byte[] rxModbusBuffer = new byte[256];
		private int readHoldingRegisters()
		{
			if (true == checkBoxConnect.Checked)
			{
				// Flush the receive buffer
				sp.DiscardInBuffer();

				// Flush the transmit buffer
				sp.DiscardOutBuffer();

				Thread.Sleep(TIME_INTERVAL_MSEC); // Adjust the delay time as needed

				ushort crc = 0;
				uint sum = 0;
				rdHoldRegisterPack[_SLAVE_ID] = (byte)intSlaveID.Value;
				rdHoldRegisterPack[_STARTING_ADDRESS_HI] = (byte)(((ushort)intAddress.Value >> 8) & 0x00FF);
				rdHoldRegisterPack[_STARTING_ADDRESS_LO] = (byte)(((ushort)intAddress.Value >> 0) & 0x00FF);
				foreach (DataGridViewRow row in dbDataSets.Rows)
				{
					if (row.Cells[colDataBytes].Value != null && uint.TryParse(row.Cells[colDataBytes].Value.ToString(), out uint cellValue))
					{
						sum += cellValue;
					}
				}

				sum = sum / 2; // to be a word.

				rdHoldRegisterPack[_QUANTITY_REGISTERS_HI] = (byte)(((ushort)sum >> 8) & 0x00FF);
				rdHoldRegisterPack[_QUANTITY_REGISTERS_LO] = (byte)(((ushort)sum >> 0) & 0x00FF);

				crc = CRC16(rdHoldRegisterPack, (ushort)(rdHoldRegisterPack.Length - 2));

				rdHoldRegisterPack[_ERROR_CHECK_LO] = (byte)(((ushort)crc >> 0) & 0x00FF);
				rdHoldRegisterPack[_ERROR_CHECK_HI] = (byte)(((ushort)crc >> 8) & 0x00FF);


				sp.Write(rdHoldRegisterPack, 0, rdHoldRegisterPack.Length);

				Thread.Sleep(TIME_INTERVAL_MSEC); // Adjust the delay time as needed

				try
				{
					// The words convert to bytes pluse the length of header and the length of CRC.
					int rsize = (int)(sum * 2 + 5);
					int gsize = 0;

					while (gsize < rsize)
					{
						gsize += sp.Read(rxModbusBuffer, gsize, rsize - gsize);
					}
					return gsize;
				}
				catch (TimeoutException)
				{
					// Handle timeout exception
					//MessageBox.Show("Timeout error occurred while reading data from the serial port.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					smsg.Text = "Timeout error occurred while reading data from the serial port.";
					if(enErrorStop.Checked) checkBoxConnect.Checked = false;
					return _ERROR_TIMEOUT;
				}
				catch (Exception ex)
				{
					// Handle any other exceptions
					//MessageBox.Show("An error occurred: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					smsg.Text = "An error occurred: " + ex.Message;
					if (enErrorStop.Checked) checkBoxConnect.Checked = false;
					return _ERROR_CODE;
				}
			}

			return _ERROR_NO_CONNECTED;
		}

		byte[] txModbusBuffer = new byte[256];

		byte[] wrSingleRegister =
		{
			0x00, // _SLAVE_ID
			0x06, // _FUNCTION_ID
			0x00, // _STARTING_ADDRESS_HI
			0x00, // _STARTING_ADDRESS_LO
			0x00, // _WRITE_DATA_HI
			0x00, // _WRITE_DATA_LO
			0x00, // _ERROR_CHECK_LO
			0x00  // _ERROR_CHECK_HI
		};



		private int writeSingleRegister(ushort usAddress, ushort usData)
		{
			if (true == checkBoxConnect.Checked)
			{

				// Flush the receive buffer
				sp.DiscardInBuffer();

				// Flush the transmit buffer
				sp.DiscardOutBuffer();

				ushort crc = 0;
				wrSingleRegister[_SLAVE_ID] = (byte)intSlaveID.Value;
				wrSingleRegister[_STARTING_ADDRESS_HI] = (byte)(((ushort)usAddress >> 8) & 0x00FF);
				wrSingleRegister[_STARTING_ADDRESS_LO] = (byte)(((ushort)usAddress >> 0) & 0x00FF);

				wrSingleRegister[_WRITE_DATA_HI] = (byte)(((ushort)usData >> 8) & 0x00FF);
				wrSingleRegister[_WRITE_DATA_LO] = (byte)(((ushort)usData >> 0) & 0x00FF);

				crc = CRC16(wrSingleRegister, (ushort)(wrSingleRegister.Length - 2));

				wrSingleRegister[_ERROR_CHECK_LO] = (byte)(((ushort)crc >> 0) & 0x00FF);
				wrSingleRegister[_ERROR_CHECK_HI] = (byte)(((ushort)crc >> 8) & 0x00FF);


				sp.Write(wrSingleRegister, 0, wrSingleRegister.Length);

				Thread.Sleep(TIME_INTERVAL_MSEC); // Adjust the delay time as needed

				try
				{
					// The words convert to bytes pluse the length of header and the length of CRC.
					int rsize = wrSingleRegister.Length;
					int gsize = 0;

					while (gsize < rsize)
					{
						gsize += sp.Read(rxModbusBuffer, gsize, rsize - gsize);
					}

					return gsize;
				}
				catch (TimeoutException)
				{
					// Handle timeout exception
					//MessageBox.Show("Timeout error occurred while reading data from the serial port.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					smsg.Text = "Timeout error occurred while reading data from the serial port.";
					checkBoxConnect.Checked = false;
					return _ERROR_TIMEOUT;
				}
				catch (Exception ex)
				{
					// Handle any other exceptions
					//MessageBox.Show("An error occurred: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					smsg.Text = "An error occurred: " + ex.Message;
					checkBoxConnect.Checked = false;
					return _ERROR_CODE;
				}
			}

			return _ERROR_NO_CONNECTED;
		}

		byte[] wrHoldingRegisters = new byte[256];
		//      {
		//	0x00, // _SLAVE_ID
		//	0x10, // _FUNCTION_ID
		//	0x00, // _STARTING_ADDRESS_HI
		//	0x00, // _STARTING_ADDRESS_LO
		//	0x00, // _QUANTITY_REGISTERS_HI
		//	0x00, // _QUANTITY_REGISTERS_LO
		//	0x00  // _BYTE_COUNT
		//};

		private int writeMultiHoldingRegister(ushort usAddress, ushort[] usData, ushort usQuantity)
		{
			if (true == checkBoxConnect.Checked)
			{

				// Flush the receive buffer
				sp.DiscardInBuffer();

				// Flush the transmit buffer
				sp.DiscardOutBuffer();

				ushort crc = 0;
				wrHoldingRegisters[_SLAVE_ID] = (byte)intSlaveID.Value;

				wrHoldingRegisters[_FUNCTION_ID] = 0x10;

				wrHoldingRegisters[_STARTING_ADDRESS_HI] = (byte)(((ushort)usAddress >> 8) & 0x00FF);
				wrHoldingRegisters[_STARTING_ADDRESS_LO] = (byte)(((ushort)usAddress >> 0) & 0x00FF);

				wrHoldingRegisters[_QUANTITY_REGISTERS_HI] = (byte)(((ushort)usQuantity >> 8) & 0x00FF);
				wrHoldingRegisters[_QUANTITY_REGISTERS_LO] = (byte)(((ushort)usQuantity >> 0) & 0x00FF);

				wrHoldingRegisters[_BYTE_COUNT] = (byte)(((ushort)(usQuantity * 2)) & 0x00FF);

				for (ushort i = 0; i < usQuantity; i++)
				{
					wrHoldingRegisters[_DATA_0_HI + i * 2] = (byte)(((ushort)usData[i] >> 8) & 0x00FF);
					wrHoldingRegisters[_DATA_0_LO + i * 2] = (byte)(((ushort)usData[i] >> 0) & 0x00FF);
				}

				crc = CRC16(wrHoldingRegisters, (ushort)(7 + usQuantity * 2));

				wrHoldingRegisters[7 + usQuantity * 2] = (byte)(((ushort)crc >> 0) & 0x00FF);
				wrHoldingRegisters[7 + usQuantity * 2 + 1] = (byte)(((ushort)crc >> 8) & 0x00FF);

				sp.Write(wrHoldingRegisters, 0, 7 + usQuantity * 2 + 2);

				Thread.Sleep(TIME_INTERVAL_MSEC); // Adjust the delay time as needed

				try
				{
					// The words convert to bytes pluse the length of header and the length of CRC.
					int rsize = 8;
					int gsize = 0;

					while (gsize < rsize)
					{
						gsize += sp.Read(rxModbusBuffer, gsize, rsize - gsize);
					}

					return gsize;
				}
				catch (TimeoutException)
				{
					// Handle timeout exception
					//MessageBox.Show("Timeout error occurred while reading data from the serial port.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					smsg.Text = "Timeout error occurred while reading data from the serial port.";
					checkBoxConnect.Checked = false;
					return _ERROR_TIMEOUT;
				}
				catch (Exception ex)
				{
					// Handle any other exceptions
					//MessageBox.Show("An error occurred: " + ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
					smsg.Text = "An error occurred: " + ex.Message;
					checkBoxConnect.Checked = false;
					return _ERROR_CODE;
				}
			}

			return _ERROR_NO_CONNECTED;
		}
	}
}
