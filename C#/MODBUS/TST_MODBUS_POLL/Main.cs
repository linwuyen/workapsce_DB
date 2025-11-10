//#define FREE_VERSION   
    

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using System.Diagnostics;
using System.Runtime.InteropServices.ComTypes;
using System.Xml.Linq;
using System.Collections;
using System.Threading;
using System.Globalization;



namespace TST_MODBUS_POLL
{



	public partial class Main : Form
	{

		public class dbContainer : Panel
		{

			public DataGridView dbDataSets { get; }
			public NumericUpDown intDataSets { get; }
			public Label labelDataSets { get; }
			public Label labelAddress { get; }
			public NumericUpDown intAddress { get; }
			public Panel rdRegX { get; }


			public dbContainer()
			{

				intDataSets = new System.Windows.Forms.NumericUpDown();
				labelDataSets = new System.Windows.Forms.Label();
				labelAddress = new System.Windows.Forms.Label();
				intAddress = new System.Windows.Forms.NumericUpDown();
				dbDataSets = new System.Windows.Forms.DataGridView();
				rdRegX = new System.Windows.Forms.Panel();
				((System.ComponentModel.ISupportInitialize)(intDataSets)).BeginInit();
				((System.ComponentModel.ISupportInitialize)(intAddress)).BeginInit();
				((System.ComponentModel.ISupportInitialize)(dbDataSets)).BeginInit();
				this.SuspendLayout();


				// 
				// intDataSets
				// 
				intDataSets.Location = new System.Drawing.Point(99, 27);
				intDataSets.Maximum = new decimal(new int[] {64,0,0,0});
				intDataSets.Name = "intDataSets";
				intDataSets.Size = new System.Drawing.Size(60, 22);
				intDataSets.TabIndex = 19;
				//intDataSets.ValueChanged += new System.EventHandler(this.intDataSets_ValueChanged);
				// 
				// labelDataSets
				// 
				labelDataSets.AutoSize = true;
				labelDataSets.Location = new System.Drawing.Point(98, 12);
				labelDataSets.Name = "labelDataSets";
				labelDataSets.Size = new System.Drawing.Size(47, 12);
				labelDataSets.TabIndex = 20;
				labelDataSets.Text = "Data Sets";
				// 
				// labelAddress
				// 
				labelAddress.AutoSize = true;
				labelAddress.Location = new System.Drawing.Point(178, 12);
				labelAddress.Name = "labelAddress";
				labelAddress.Size = new System.Drawing.Size(42, 12);
				labelAddress.TabIndex = 22;
				labelAddress.Text = "Address";
				labelAddress.UseMnemonic = false;
				// 
				// intAddress
				// 
				intAddress.Location = new System.Drawing.Point(180, 27);
				intAddress.Maximum = new decimal(new int[] {65535,0,0,0});
				intAddress.Name = "intAddress";
				intAddress.Size = new System.Drawing.Size(60, 22);
				intAddress.TabIndex = 21;
				intAddress.Value = new decimal(new int[] {1000,0,0,0});
				// 
				// dbDataSets
				// 
				dbDataSets.AllowUserToAddRows = false;
				dbDataSets.AllowUserToDeleteRows = false;
				dbDataSets.CausesValidation = false;
				dbDataSets.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
				//dbDataSets.ContextMenuStrip = this.dbgRightKeyMenu;
				dbDataSets.Location = new System.Drawing.Point(3, 55);
				dbDataSets.Name = "dbDataSets";
				dbDataSets.RowTemplate.Height = 24;
				dbDataSets.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
				dbDataSets.SelectionMode = System.Windows.Forms.DataGridViewSelectionMode.CellSelect;
				dbDataSets.Size = new System.Drawing.Size(1022, 313);
				dbDataSets.TabIndex = 23;
				//dbDataSets.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dbDataSets_CellEndEdit);
				//dbDataSets.CellValueChanged += new System.Windows.Forms.DataGridViewCellEventHandler(this.dbDataSets_CellValueChanged);
				//dbDataSets.EditingControlShowing += new System.Windows.Forms.DataGridViewEditingControlShowingEventHandler(this.dbDataSets_EditingControlShowing);
				//dbDataSets.KeyDown += new System.Windows.Forms.KeyEventHandler(this.dbDataSets_KeyDown);

				// 
				// rdRegX
				// 
				rdRegX.Controls.Add(intDataSets);
				rdRegX.Controls.Add(labelDataSets);
				rdRegX.Controls.Add(intAddress);
				rdRegX.Controls.Add(labelAddress);
				rdRegX.Controls.Add(dbDataSets);
				rdRegX.Location = new System.Drawing.Point(12, 54);
				rdRegX.Name = "rdRegX";
				rdRegX.Size = new System.Drawing.Size(1028, 371);
				rdRegX.TabIndex = 32;
			}
		}

		public SerialPort sp;

		public Main()
		{
			InitializeComponent();

			getComPortList();

			BaudrateList.Items.Clear();
			BaudrateList.Items.AddRange(u32Baudrate.Cast<object>().ToArray());
			BaudrateList.SelectedIndex = 7;

			DatabitsList.Items.Clear();
			DatabitsList.Items.AddRange(u32Databits.Cast<object>().ToArray());
			DatabitsList.SelectedIndex = 1;

			StopbitsList.Items.Clear();
			StopbitsList.Items.AddRange(u32Stopbits.Cast<object>().ToArray());
			StopbitsList.SelectedIndex = 0;

			ParityList.Items.Clear();
			ParityList.Items.AddRange(strParity);
			ParityList.SelectedIndex = 2;

			FlowControl.SelectedIndex = 0;

			timerInterval.Interval = (int)intIntervalMsec.Value;

			connectStatus.BackColor = Color.Gray;

			buildDataGrid();

			//Panel containerPanel = new Panel();
			//containerPanel.Location = new System.Drawing.Point(20, 20);
			//containerPanel.Size = new System.Drawing.Size(600, 400);
			//this.Controls.Add(containerPanel);

			//containerPanel.Controls.Add(dbDataSets);

#if FREE_VERSION
			labCopyRight.Text = "yanbai7@gmail.com";
			labVersion.Text = "Free Version";
#else
			labCopyRight.Text = "";
			labVersion.Text = "";
#endif //FREE_VERSION

		}

		// CellContextMenuStripNeeded event handler
		private void dbDataSets_CellContextMenuStripNeeded(object sender, DataGridViewCellContextMenuStripNeededEventArgs e)
		{
			//if (e.RowIndex >= 0 && e.ColumnIndex >= 0)
			//{
			//	// Specify the custom context menu strip for the desired cells
			//	e.ContextMenuStrip = advancedMenuStrip;
			//}
		}

		private void checkBoxConnect_CheckedChanged(object sender, EventArgs e)
		{
			if (checkBoxConnect.Checked)
			{
				smsg.Text = "";

				if (0 == intDataSets.Value)
				{
					checkBoxConnect.Checked = false;
					MessageBox.Show("There is not any set in data.");
					return;
				}
				sp = new SerialPort(ComPortList.Text,
				int.Parse(BaudrateList.Text),
				Parity.Even,
				int.Parse(DatabitsList.Text),
				StopBits.One);
				// Set the read and write timeouts in milliseconds
				sp.ReadTimeout = (int)intTimeoutMsec.Value; // 1 second
				sp.WriteTimeout = (int)intTimeoutMsec.Value; // 1 second


				try
				{
					sp.Open();
					// Port opened successfully, perform any further operations
					Console.WriteLine("Port opened successfully!");

					// Flush the receive buffer
					sp.DiscardInBuffer();

					// Flush the transmit buffer
					sp.DiscardOutBuffer();


					pollDataSets();

				}
				catch (UnauthorizedAccessException)
				{
					// Handle unauthorized access exception
					Console.WriteLine("Unauthorized access to the port!");
					checkBoxConnect.Checked = false;
				}
				catch (IOException)
				{
					// Handle I/O exception
					Console.WriteLine("An I/O error occurred while opening the port!");
					checkBoxConnect.Checked = false;
				}
				catch (InvalidOperationException)
				{
					// Handle invalid operation exception
					Console.WriteLine("Port is already open or cannot be opened!");
					checkBoxConnect.Checked = false;
				}
			}
			else
			{
				if (null != sp)
				{
					sp.Close();
					Console.WriteLine("Port is already closed!");
				}
			}

#if FREE_VERSION
			ComPortList.Enabled
				= intSlaveID.Enabled
				= intTimeoutMsec.Enabled
				= intDataSets.Enabled
				= intAddress.Enabled = !checkBoxConnect.Checked;
#else
			ComPortList.Enabled = DatabitsList.Enabled
								= BaudrateList.Enabled
								= intSlaveID.Enabled
								= intTimeoutMsec.Enabled
								= intDataSets.Enabled
								= intAddress.Enabled = !checkBoxConnect.Checked;
#endif //FREE_VERSION



		}



		private void SimplePoll_Resize(object sender, EventArgs e)
		{
			rdRegX.Width = this.ClientSize.Width - rdRegX.Left * 2;
			rdRegX.Height = this.ClientSize.Height - rdRegX.Top - 20;

			dbDataSets.Width = rdRegX.Width - dbDataSets.Left * 2;
			dbDataSets.Height = rdRegX.Height - dbDataSets.Top - 20;

			
		}


		private void intDataSets_ValueChanged(object sender, EventArgs e)
		{
			int rowCount = (int)intDataSets.Value;

			if (rowCount > dbDataSets.RowCount)
			{
				int rowsToAdd = rowCount - dbDataSets.RowCount;

				for (int i = 0; i < rowsToAdd; i++)
				{
					dbDataSets.Rows.Add(); // Add new
					if (1 < dbDataSets.RowCount)
					{
						dbDataSets.Rows[dbDataSets.RowCount - 1].Cells["Address"].Value = int.Parse(dbDataSets.Rows[dbDataSets.RowCount - 2].Cells["Address"].Value.ToString())
																						+ int.Parse(dbDataSets.Rows[dbDataSets.RowCount - 2].Cells["Bytes"].Value.ToString()) / 2;
						dbDataSets.Rows[dbDataSets.RowCount - 1].Cells["Bytes"].Value = 2;

					}
					else
					{
						dbDataSets.Rows[dbDataSets.RowCount - 1].Cells["Address"].Value = intAddress.Value + dbDataSets.RowCount - 1;
						dbDataSets.Rows[dbDataSets.RowCount - 1].Cells["Bytes"].Value = 2;
					}

					dbDataSets.Rows[dbDataSets.RowCount - 1].Cells["Size"].Value = "uint16_t";
					dbDataSets.Rows[dbDataSets.RowCount - 1].Cells["Type"].Value = "Hex";
					dbDataSets.Rows[dbDataSets.RowCount - 1].Cells["Gain"].Value = null;
					dbDataSets.Rows[dbDataSets.RowCount - 1].Cells["Offset"].Value = null;
					dbDataSets.Rows[dbDataSets.RowCount - 1].Cells["Value"].Value = "0xFFFF";

					dbDataSets.Rows[dbDataSets.RowCount - 1].DefaultCellStyle.ForeColor = Color.Black;
					dbDataSets.Rows[dbDataSets.RowCount - 1].DefaultCellStyle.BackColor = Color.White;
				}
			}
			else if (rowCount < dbDataSets.RowCount)
			{
				int rowsToDelete = dbDataSets.RowCount - rowCount;

				dbDataSets.EndEdit();

				for (int i = 0; i < rowsToDelete; i++)
				{
					dbDataSets.Rows.RemoveAt(dbDataSets.Rows.Count - 1); // Remove last row
				}
			}
		}

		private void timerInterval_Tick(object sender, EventArgs e)
		{
			//Console.WriteLine("timerInterval_Tick");

			if (checkBoxConnect.Checked)
			{
				if (false == fgEditing) pollDataSets();
				connectLED.Start();
			}
			else
			{
				timerInterval.Stop();
				
			}

		}

		private static bool IsHexadecimal(string input, out int decimalValue)
		{
			// Remove "0x" prefix if present (optional step)
			if (input.StartsWith("0x", StringComparison.OrdinalIgnoreCase))
			{
				input = input.Substring(2);

				bool isValidHex = int.TryParse(input, NumberStyles.HexNumber, CultureInfo.InvariantCulture, out decimalValue);
				return true;
			}

			decimalValue = 0;
			return false;

		}

		private static bool IsBinaryString(string input, out int decimalValue)
		{
			string binaryValue = string.Empty;
			if (input.StartsWith("0b", StringComparison.OrdinalIgnoreCase))
			{
				input = input.Substring(2);
				// Remove any underscore (_) separators from the binary string
				input = input.Replace("_", "");

				// Check if the remaining string contains only '0' and '1'
				if (System.Text.RegularExpressions.Regex.IsMatch(input, "^[01]+$"))
				{
					binaryValue = input;
					decimalValue = Convert.ToInt32(binaryValue, 2);
					return true;
				}
			}
			decimalValue = 0;
			return false;
		}


		private void dbDataSets_CellValueChanged(object sender, DataGridViewCellEventArgs e)
		{
			// Get the row index and column index of the changed cell
			int rowIndex = e.RowIndex;
			int columnIndex = e.ColumnIndex;
			DataGridView db = ((DataGridView)sender);


			colfunc[columnIndex](sender, e);


			if (fgEditing)
			{
				// Get the value of a cell in column 4 of the entered row
				object cellValue = db.Rows[e.RowIndex].Cells[colDataValue].Value;

				// Check if the cell value is not null
				if (cellValue != null)
				{
					ushort getAddress = ushort.Parse(db.Rows[e.RowIndex].Cells[colAddress].Value.ToString());

					double getValue = 0;
					bool IsValidValue = true;

					
					if("ASCII" == db.Rows[e.RowIndex].Cells[colDataType].Value.ToString())
					{
						ushort u16Length = (ushort)db.Rows[e.RowIndex].Cells[colDataValue].Value.ToString().Length;
						ushort[] u16Temp = new ushort[u16Length];
						string inputString = db.Rows[e.RowIndex].Cells[colDataValue].Value.ToString();
						for (int i = 0; i<u16Length; i++)
						{
							u16Temp[i] = inputString[i];	
						}

						writeMultiHoldingRegister(getAddress, u16Temp, u16Length);
						IsValidValue = false;
					}
					else if (IsHexadecimal(db.Rows[e.RowIndex].Cells[colDataValue].Value.ToString(), out int HexValue))
					{
						getValue = (double)HexValue;
					}
					else if (IsBinaryString(db.Rows[e.RowIndex].Cells[colDataValue].Value.ToString(), out int BinaryValue))
					{
						getValue = (double)BinaryValue;
					}
					else if (true == double.TryParse(db.Rows[e.RowIndex].Cells[colDataValue].Value.ToString(), out double FractinalValue))
					{
						getValue = (double)FractinalValue;
					}
					else
					{
						IsValidValue = false;
					}

					if (IsValidValue)
					{
						double d64Temp = getValue;
						Debug.Print("input: " + d64Temp.ToString());

						double d64Gain = 1.0;
						double d64Offset = 0.0;
						
						if(null != db.Rows[e.RowIndex].Cells[colDataGain].Value)
						{
							if (true == double.TryParse(db.Rows[e.RowIndex].Cells[colDataGain].Value.ToString(), out double d64GainTemp))
							{
								d64Gain = d64GainTemp;
							}
						}

						if(null != db.Rows[e.RowIndex].Cells[colDataOffset].Value)
						{
							if (true == double.TryParse(db.Rows[e.RowIndex].Cells[colDataOffset].Value.ToString(), out double d64OffsetTemp))
							{
								d64Offset = d64OffsetTemp;
							}
						}

						d64Temp = (d64Temp - d64Offset) / d64Gain;

						if(null != db.Rows[e.RowIndex].Cells[colDataMaximum].Value)
						{
							if (true == double.TryParse(db.Rows[e.RowIndex].Cells[colDataMaximum].Value.ToString(), out double d64Maximum))
							{
								double d64MaxTemp = (d64Maximum - d64Offset) / d64Gain;
								if (d64Temp > d64MaxTemp) d64Temp = d64MaxTemp;
							}
						}

						if(null != db.Rows[e.RowIndex].Cells[colDataMinimum].Value)
						{
							if (true == double.TryParse(db.Rows[e.RowIndex].Cells[colDataMinimum].Value.ToString(), out double d64Minimum))
							{
								double d64MinTemp = (d64Minimum - d64Offset) / d64Gain;
								if (d64Temp < d64MinTemp) d64Temp = d64MinTemp;
							}
						}


						Debug.Print("output: " + d64Temp.ToString());

						switch (db.Rows[e.RowIndex].Cells[colDataSize].Value.ToString())
						{
							case _F32:
								float f32Temp = (float)d64Temp;
								byte[] f32Bytes = BitConverter.GetBytes(f32Temp);

								uint uintValue = BitConverter.ToUInt32(f32Bytes, 0);

								ushort[] u16Value = new ushort[2];

								u16Value[1] = (ushort)((uintValue >> 16) & 0xFFFF); // Extract upper 16 bits
								u16Value[0] = (ushort)(uintValue & 0xFFFF); // Extract lower 16 bits
								writeMultiHoldingRegister(getAddress, u16Value, 2);
								break;
							case _Q15:
								UInt16 q15Temp = (UInt16)(d64Temp * 32767.0);
								writeSingleRegister(getAddress, q15Temp);
								break;
							case _Q31:
								UInt32 q31Temp = (UInt32)(d64Temp * 2147483647.0);
								ushort[] q31Value = new ushort[2];
								q31Value[1] = (ushort)((q31Temp >> 16) & 0xFFFF); // Extract upper 16 bits
								q31Value[0] = (ushort)(q31Temp & 0xFFFF); // Extract lower 16 bits
								writeMultiHoldingRegister(getAddress, q31Value, 2);
								break;
							default:
								ushort usdata = (ushort)d64Temp;
								writeSingleRegister(getAddress, usdata);
								break;
						}
					}
				}

				dbDataSets.ClearSelection();
			}
		}


		private void buttonExport_Click(object sender, EventArgs e)
		{
			ExportData();
		}

		private void buttonImport_Click(object sender, EventArgs e)
		{
			ImportData();

			intDataSets.Value = dbDataSets.RowCount;

		}


		private void buttonTest_Click(object sender, EventArgs e)
		{
			//pollDataSets();

			//float f32Temp = (float)0.5;
			//byte[] f32Bytes = BitConverter.GetBytes(f32Temp);

			//uint uintValue = BitConverter.ToUInt32(f32Bytes, 0);

			//ushort[] u16Value = new ushort[2];

			//u16Value[1] = (ushort)((uintValue >> 16) & 0xFFFF); // Extract upper 16 bits
			//u16Value[0] = (ushort)(uintValue & 0xFFFF); // Extract lower 16 bits
			//writeMultiHoldingRegister(1022, u16Value, 2);
		}


		private void dbDataSets_EditingControlShowing(object sender, DataGridViewEditingControlShowingEventArgs e)
		{
			Debug.Print("dbDataSets_EditingControlShowing");

			DataGridView db = ((DataGridView)sender);

			fgEditing = (colDataValue == db.CurrentCell.ColumnIndex);

			if (null != db.CurrentCell.Value) tmpOriginValue = db.CurrentCell.Value.ToString();
			else tmpOriginValue = "";



		}

		private void intIntervalMsec_ValueChanged(object sender, EventArgs e)
		{
			timerInterval.Interval = (int)intIntervalMsec.Value;
		}

		private void ComPortList_MouseClick(object sender, MouseEventArgs e)
		{
			getComPortList();
		}

		private void hideToolStripMenuItem_Click(object sender, EventArgs e)
		{
			for (int i = 0; i < dbDataSets.SelectedRows.Count; i++)
			{
				dbDataSets.Rows[dbDataSets.SelectedRows[i].Index].Visible = false;
			}
		}

		private void showAllToolStripMenuItem_Click(object sender, EventArgs e)
		{
			for (int i = 0; i < dbDataSets.Rows.Count; i++)
			{
				dbDataSets.Rows[i].Visible = true;
			}
		}

		private void RowForeColor_Click(object sender, EventArgs e)
		{
			for (int i = 0; i < dbDataSets.SelectedRows.Count; i++)
			{
				if (redToolStripMenuItem == sender)
				{
					dbDataSets.Rows[dbDataSets.SelectedRows[i].Index].DefaultCellStyle.ForeColor = Color.Red;
				}
				else if (greenToolStripMenuItem == sender)
				{
					dbDataSets.Rows[dbDataSets.SelectedRows[i].Index].DefaultCellStyle.ForeColor = Color.Green;
				}
				else if (blueToolStripMenuItem == sender)
				{
					dbDataSets.Rows[dbDataSets.SelectedRows[i].Index].DefaultCellStyle.ForeColor = Color.Blue;
				}
				else if (blackToolStripMenuItem == sender)
				{
					dbDataSets.Rows[dbDataSets.SelectedRows[i].Index].DefaultCellStyle.ForeColor = Color.Black;
				}
				else if (whiteToolStripMenuItem == sender)
				{
					dbDataSets.Rows[dbDataSets.SelectedRows[i].Index].DefaultCellStyle.ForeColor = Color.White;
				}
				else if (grayToolStripMenuItem == sender)
				{
					dbDataSets.Rows[dbDataSets.SelectedRows[i].Index].DefaultCellStyle.ForeColor = Color.Gray;
				}
				else
				{
					dbDataSets.Rows[dbDataSets.SelectedRows[i].Index].DefaultCellStyle.ForeColor = Color.Black;
				}
			}
		}

		private void RowBackColor_Click(object sender, EventArgs e)
		{
			for (int i = 0; i < dbDataSets.SelectedRows.Count; i++)
			{
				if (redToolStripMenuItem1 == sender)
				{
					dbDataSets.Rows[dbDataSets.SelectedRows[i].Index].DefaultCellStyle.BackColor = Color.Red;
				}
				else if (greenToolStripMenuItem1 == sender)
				{
					dbDataSets.Rows[dbDataSets.SelectedRows[i].Index].DefaultCellStyle.BackColor = Color.Green;
				}
				else if (blueToolStripMenuItem1 == sender)
				{
					dbDataSets.Rows[dbDataSets.SelectedRows[i].Index].DefaultCellStyle.BackColor = Color.Blue;
				}
				else if (blackToolStripMenuItem1 == sender)
				{
					dbDataSets.Rows[dbDataSets.SelectedRows[i].Index].DefaultCellStyle.BackColor = Color.Black;
				}
				else if (whiteToolStripMenuItem1 == sender)
				{
					dbDataSets.Rows[dbDataSets.SelectedRows[i].Index].DefaultCellStyle.BackColor = Color.White;
				}
				else if (grayToolStripMenuItem1 == sender)
				{
					dbDataSets.Rows[dbDataSets.SelectedRows[i].Index].DefaultCellStyle.BackColor = Color.Gray;
				}
				else
				{
					dbDataSets.Rows[dbDataSets.SelectedRows[i].Index].DefaultCellStyle.BackColor = Color.Black;
				}
			}
		}

		private void updateFontSize(object sender, EventArgs e)
		{
			if(FontSize9 == sender)
			{
				dbDataSets.Font = new Font("Consolas", 9, FontStyle.Regular);
			}
			else if(FontSize11 == sender)
			{
				dbDataSets.Font = new Font("Consolas", 11, FontStyle.Regular);
			}
			else if (FontSize13 == sender)
			{
				dbDataSets.Font = new Font("Consolas", 13, FontStyle.Regular);
			}
			else if (FontSize15 == sender)
			{
				dbDataSets.Font = new Font("Consolas", 15, FontStyle.Regular);
			}
			else
			{
				dbDataSets.Font = new Font("Consolas", 9, FontStyle.Regular);
			}

			dbDataSets.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
			dbDataSets.AutoResizeColumns();


		}

		private void connectLED_Tick(object sender, EventArgs e)
		{

			if (checkBoxConnect.Checked)
			{
				if (Color.Gray == connectStatus.BackColor) connectStatus.BackColor = Color.LightGreen;
				else connectStatus.BackColor = Color.Gray;
			}
			else
			{
				connectLED.Stop();
				connectStatus.BackColor = Color.Red;

			}
		}

		private void updateMemoryAddress()
		{
			for (int i = 2; i < dbDataSets.RowCount; i++)
			{
				dbDataSets.Rows[i].Cells["Address"].Value = int.Parse(dbDataSets.Rows[i - 1].Cells["Address"].Value.ToString())
														  + int.Parse(dbDataSets.Rows[i - 1].Cells["Bytes"].Value.ToString()) / 2;
			}
		}

		private void DeleteRow_Click(object sender, EventArgs e)
		{
			// Check if any row is selected
			if (dbDataSets.SelectedRows.Count > 0)
			{
				int rowIndex = dbDataSets.SelectedRows[0].Index;
				if (rowIndex >= 0 && rowIndex < dbDataSets.Rows.Count)
				{
					// Remove the row from the DataTable
					dbDataSets.Rows.RemoveAt(rowIndex);

					// Refresh the DataGridView to update the view
					dbDataSets.Refresh();
				}

				updateMemoryAddress();

				intDataSets.Value = dbDataSets.Rows.Count;
			}
		}

		private void DefaultRow_Click(object sender, EventArgs e)
		{
			// Check if any row is selected
			if (dbDataSets.SelectedRows.Count > 0)
			{
				int rowIndex = dbDataSets.SelectedRows[0].Index;
				if (rowIndex >= 0 && rowIndex < dbDataSets.Rows.Count)
				{
					dbDataSets.Rows[rowIndex].Cells["Name"].Value = null;
					dbDataSets.Rows[rowIndex].Cells["Size"].Value = null;
					dbDataSets.Rows[rowIndex].Cells["Value"].Value = "0xFFFF";
					dbDataSets.Rows[rowIndex].Cells["Bytes"].Value = 2;
					dbDataSets.Rows[rowIndex].Cells["Gain"].Value = null;
					dbDataSets.Rows[rowIndex].Cells["Offset"].Value = null;
					dbDataSets.Rows[rowIndex].Cells["Maximum"].Value = null;
					dbDataSets.Rows[rowIndex].Cells["Minimum"].Value = null;
				}


				updateMemoryAddress();

				intDataSets.Value = dbDataSets.Rows.Count;
			}
		}

		private void InsertRow_Click(object sender, EventArgs e)
		{
			// Check if any row is selected
			if (dbDataSets.SelectedRows.Count > 0)
			{
				int rowIndex = dbDataSets.SelectedRows[0].Index;
				if (rowIndex >= 0 && rowIndex < dbDataSets.Rows.Count)
				{
					// Remove the row from the DataTable
					dbDataSets.Rows.Insert(rowIndex);

					// Refresh the DataGridView to update the view
					dbDataSets.Refresh();
				}

				dbDataSets.Rows[rowIndex].Cells["Address"].Value = int.Parse(dbDataSets.Rows[rowIndex - 1].Cells["Address"].Value.ToString())
														         + int.Parse(dbDataSets.Rows[rowIndex - 1].Cells["Bytes"].Value.ToString()) / 2;

				dbDataSets.Rows[rowIndex].Cells["Bytes"].Value = 2;
				dbDataSets.Rows[rowIndex].Cells["Value"].Value = "0xFFFF";

				intDataSets.Value = dbDataSets.Rows.Count;
			}
		}

		private void dbDataSets_KeyDown(object sender, KeyEventArgs e)
		{
			// Check if Enter key is pressed
			if (e.KeyCode == Keys.Enter)
			{
				//Debug.Print("dbDataSets_KeyDown");
				// Prevent the default behavior (move to the next cell below)
				e.SuppressKeyPress = true;

				// Get the current cell's position
				int currentRow = dbDataSets.CurrentCell.RowIndex;
				int currentColumn = dbDataSets.CurrentCell.ColumnIndex;

				// Set the current cell to the same cell
				dbDataSets.CurrentCell = dbDataSets.Rows[currentRow].Cells[currentColumn];
			}
		}

		private void dbDataSets_CellEndEdit(object sender, DataGridViewCellEventArgs e)
		{
			// Get the current cell's position
			int currentRow = dbDataSets.CurrentCell.RowIndex;
			int currentColumn = dbDataSets.CurrentCell.ColumnIndex;

			Debug.Print("dbDataSets_CellEndEdit");
			// Use BeginInvoke to delay the selection change
			BeginInvoke(new Action(() =>
			{
				// Get the current cell's position
				int nextRow = dbDataSets.CurrentCell.RowIndex;
				int nextColumn = dbDataSets.CurrentCell.ColumnIndex;

				Debug.Print("currentRow + {0}", currentRow);
				Debug.Print("nextRow + {0}", nextRow);

				// Set the current cell to the same cell
				//if ((""!= tmpOriginValue)&&(tmpOriginValue != dbDataSets.Rows[currentRow].Cells[currentColumn].Value.ToString()))
				//{
				dbDataSets.CurrentCell = dbDataSets.Rows[currentRow].Cells[currentColumn];
				//}
			}));

			fgEditing = false;

		}

		private void intAddress_ValueChanged(object sender, EventArgs e)
		{
			// When you modify the beginning of an address, update all address IDs in this data grid.

			for (int i = 0; i < dbDataSets.RowCount; i++)
			{
				if (0 < i)
				{
					dbDataSets.Rows[i].Cells["Address"].Value = int.Parse(dbDataSets.Rows[i - 1].Cells["Address"].Value.ToString())
															  + int.Parse(dbDataSets.Rows[i - 1].Cells["Bytes"].Value.ToString()) / 2;
				}
				else
				{
					dbDataSets.Rows[i].Cells["Address"].Value = intAddress.Value;
				}
			}
		}
	}
}
