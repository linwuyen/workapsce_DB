namespace TST_MODBUS_POLL
{
	partial class Main
	{
		/// <summary>
		/// 設計工具所需的變數。
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// 清除任何使用中的資源。
		/// </summary>
		/// <param name="disposing">如果應該處置受控資源則為 true，否則為 false。</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form 設計工具產生的程式碼

		/// <summary>
		/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
		/// 這個方法的內容。
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
			this.ComPortList = new System.Windows.Forms.ComboBox();
			this.BaudrateList = new System.Windows.Forms.ComboBox();
			this.DatabitsList = new System.Windows.Forms.ComboBox();
			this.LabelComPort = new System.Windows.Forms.Label();
			this.labelBaudrate = new System.Windows.Forms.Label();
			this.StopbitsList = new System.Windows.Forms.ComboBox();
			this.ParityList = new System.Windows.Forms.ComboBox();
			this.labelDataBits = new System.Windows.Forms.Label();
			this.labelStopBits = new System.Windows.Forms.Label();
			this.labelParity = new System.Windows.Forms.Label();
			this.labelFlow = new System.Windows.Forms.Label();
			this.FlowControl = new System.Windows.Forms.ComboBox();
			this.checkBoxConnect = new System.Windows.Forms.CheckBox();
			this.timerInterval = new System.Windows.Forms.Timer(this.components);
			this.intTimeoutMsec = new System.Windows.Forms.NumericUpDown();
			this.labelTimeout = new System.Windows.Forms.Label();
			this.labelInterval = new System.Windows.Forms.Label();
			this.intIntervalMsec = new System.Windows.Forms.NumericUpDown();
			this.intDataSets = new System.Windows.Forms.NumericUpDown();
			this.labelDataSets = new System.Windows.Forms.Label();
			this.labelAddress = new System.Windows.Forms.Label();
			this.intAddress = new System.Windows.Forms.NumericUpDown();
			this.dbDataSets = new System.Windows.Forms.DataGridView();
			this.dbgRightKeyMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.fontColorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.redToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.greenToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.blueToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.blackToolStripMenuItem = new System.Windows.Forms.ToolStripSeparator();
			this.blackToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.whiteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.grayToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.backgroundColorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.redToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.greenToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.blueToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
			this.blackToolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
			this.whiteToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.grayToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
			this.hideToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.showAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.DefaultRow = new System.Windows.Forms.ToolStripMenuItem();
			this.InsertRow = new System.Windows.Forms.ToolStripMenuItem();
			this.DeleteRow = new System.Windows.Forms.ToolStripMenuItem();
			this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
			this.fontSizeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
			this.FontSize9 = new System.Windows.Forms.ToolStripMenuItem();
			this.FontSize11 = new System.Windows.Forms.ToolStripMenuItem();
			this.FontSize13 = new System.Windows.Forms.ToolStripMenuItem();
			this.FontSize15 = new System.Windows.Forms.ToolStripMenuItem();
			this.buttonExport = new System.Windows.Forms.Button();
			this.buttonImport = new System.Windows.Forms.Button();
			this.labelSlaveID = new System.Windows.Forms.Label();
			this.intSlaveID = new System.Windows.Forms.NumericUpDown();
			this.buttonTest = new System.Windows.Forms.Button();
			this.statusBar = new System.Windows.Forms.StatusStrip();
			this.smsg = new System.Windows.Forms.ToolStripStatusLabel();
			this.connectStatus = new System.Windows.Forms.CheckBox();
			this.connectLED = new System.Windows.Forms.Timer(this.components);
			this.rdRegX = new System.Windows.Forms.Panel();
			this.labCopyRight = new System.Windows.Forms.Label();
			this.labVersion = new System.Windows.Forms.Label();
			this.enErrorStop = new System.Windows.Forms.CheckBox();
			((System.ComponentModel.ISupportInitialize)(this.intTimeoutMsec)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.intIntervalMsec)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.intDataSets)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.intAddress)).BeginInit();
			((System.ComponentModel.ISupportInitialize)(this.dbDataSets)).BeginInit();
			this.dbgRightKeyMenu.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.intSlaveID)).BeginInit();
			this.statusBar.SuspendLayout();
			this.rdRegX.SuspendLayout();
			this.SuspendLayout();
			// 
			// ComPortList
			// 
			this.ComPortList.FormattingEnabled = true;
			this.ComPortList.Location = new System.Drawing.Point(14, 28);
			this.ComPortList.Name = "ComPortList";
			this.ComPortList.Size = new System.Drawing.Size(60, 20);
			this.ComPortList.TabIndex = 0;
			this.ComPortList.MouseClick += new System.Windows.Forms.MouseEventHandler(this.ComPortList_MouseClick);
			// 
			// BaudrateList
			// 
			this.BaudrateList.Enabled = false;
			this.BaudrateList.FormattingEnabled = true;
			this.BaudrateList.Location = new System.Drawing.Point(80, 28);
			this.BaudrateList.Name = "BaudrateList";
			this.BaudrateList.Size = new System.Drawing.Size(60, 20);
			this.BaudrateList.TabIndex = 1;
			// 
			// DatabitsList
			// 
			this.DatabitsList.Enabled = false;
			this.DatabitsList.FormattingEnabled = true;
			this.DatabitsList.Location = new System.Drawing.Point(146, 28);
			this.DatabitsList.Name = "DatabitsList";
			this.DatabitsList.Size = new System.Drawing.Size(60, 20);
			this.DatabitsList.TabIndex = 2;
			// 
			// LabelComPort
			// 
			this.LabelComPort.AutoSize = true;
			this.LabelComPort.Location = new System.Drawing.Point(12, 13);
			this.LabelComPort.Name = "LabelComPort";
			this.LabelComPort.Size = new System.Drawing.Size(53, 12);
			this.LabelComPort.TabIndex = 4;
			this.LabelComPort.Text = "Com Port:";
			// 
			// labelBaudrate
			// 
			this.labelBaudrate.AutoSize = true;
			this.labelBaudrate.Location = new System.Drawing.Point(78, 13);
			this.labelBaudrate.Name = "labelBaudrate";
			this.labelBaudrate.Size = new System.Drawing.Size(50, 12);
			this.labelBaudrate.TabIndex = 5;
			this.labelBaudrate.Text = "Baudrate:";
			// 
			// StopbitsList
			// 
			this.StopbitsList.Enabled = false;
			this.StopbitsList.FormattingEnabled = true;
			this.StopbitsList.Location = new System.Drawing.Point(278, 28);
			this.StopbitsList.Name = "StopbitsList";
			this.StopbitsList.Size = new System.Drawing.Size(60, 20);
			this.StopbitsList.TabIndex = 6;
			// 
			// ParityList
			// 
			this.ParityList.Enabled = false;
			this.ParityList.FormattingEnabled = true;
			this.ParityList.Location = new System.Drawing.Point(212, 28);
			this.ParityList.Name = "ParityList";
			this.ParityList.Size = new System.Drawing.Size(60, 20);
			this.ParityList.TabIndex = 7;
			// 
			// labelDataBits
			// 
			this.labelDataBits.AutoSize = true;
			this.labelDataBits.Location = new System.Drawing.Point(144, 13);
			this.labelDataBits.Name = "labelDataBits";
			this.labelDataBits.Size = new System.Drawing.Size(47, 12);
			this.labelDataBits.TabIndex = 8;
			this.labelDataBits.Text = "Data Bits";
			// 
			// labelStopBits
			// 
			this.labelStopBits.AutoSize = true;
			this.labelStopBits.Location = new System.Drawing.Point(276, 13);
			this.labelStopBits.Name = "labelStopBits";
			this.labelStopBits.Size = new System.Drawing.Size(47, 12);
			this.labelStopBits.TabIndex = 9;
			this.labelStopBits.Text = "Stop Bits";
			// 
			// labelParity
			// 
			this.labelParity.AutoSize = true;
			this.labelParity.Location = new System.Drawing.Point(210, 13);
			this.labelParity.Name = "labelParity";
			this.labelParity.Size = new System.Drawing.Size(32, 12);
			this.labelParity.TabIndex = 10;
			this.labelParity.Text = "Parity";
			// 
			// labelFlow
			// 
			this.labelFlow.AutoSize = true;
			this.labelFlow.Location = new System.Drawing.Point(342, 13);
			this.labelFlow.Name = "labelFlow";
			this.labelFlow.Size = new System.Drawing.Size(28, 12);
			this.labelFlow.TabIndex = 12;
			this.labelFlow.Text = "Flow";
			// 
			// FlowControl
			// 
			this.FlowControl.Enabled = false;
			this.FlowControl.FormattingEnabled = true;
			this.FlowControl.Items.AddRange(new object[] {
            "NONE"});
			this.FlowControl.Location = new System.Drawing.Point(344, 28);
			this.FlowControl.Name = "FlowControl";
			this.FlowControl.Size = new System.Drawing.Size(60, 20);
			this.FlowControl.TabIndex = 11;
			// 
			// checkBoxConnect
			// 
			this.checkBoxConnect.Appearance = System.Windows.Forms.Appearance.Button;
			this.checkBoxConnect.Location = new System.Drawing.Point(416, 26);
			this.checkBoxConnect.Name = "checkBoxConnect";
			this.checkBoxConnect.Size = new System.Drawing.Size(80, 22);
			this.checkBoxConnect.TabIndex = 14;
			this.checkBoxConnect.Text = "Connect";
			this.checkBoxConnect.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
			this.checkBoxConnect.UseVisualStyleBackColor = true;
			this.checkBoxConnect.CheckedChanged += new System.EventHandler(this.checkBoxConnect_CheckedChanged);
			// 
			// timerInterval
			// 
			this.timerInterval.Interval = 10;
			this.timerInterval.Tick += new System.EventHandler(this.timerInterval_Tick);
			// 
			// intTimeoutMsec
			// 
			this.intTimeoutMsec.Increment = new decimal(new int[] {
            10,
            0,
            0,
            0});
			this.intTimeoutMsec.Location = new System.Drawing.Point(591, 26);
			this.intTimeoutMsec.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
			this.intTimeoutMsec.Name = "intTimeoutMsec";
			this.intTimeoutMsec.Size = new System.Drawing.Size(60, 22);
			this.intTimeoutMsec.TabIndex = 15;
			this.intTimeoutMsec.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
			// 
			// labelTimeout
			// 
			this.labelTimeout.AutoSize = true;
			this.labelTimeout.Location = new System.Drawing.Point(589, 14);
			this.labelTimeout.Name = "labelTimeout";
			this.labelTimeout.Size = new System.Drawing.Size(78, 12);
			this.labelTimeout.TabIndex = 16;
			this.labelTimeout.Text = "Timeout (msec)";
			// 
			// labelInterval
			// 
			this.labelInterval.AutoSize = true;
			this.labelInterval.Location = new System.Drawing.Point(673, 14);
			this.labelInterval.Name = "labelInterval";
			this.labelInterval.Size = new System.Drawing.Size(75, 12);
			this.labelInterval.TabIndex = 18;
			this.labelInterval.Text = "Interval (msec)";
			// 
			// intIntervalMsec
			// 
			this.intIntervalMsec.Increment = new decimal(new int[] {
            10,
            0,
            0,
            0});
			this.intIntervalMsec.Location = new System.Drawing.Point(673, 26);
			this.intIntervalMsec.Maximum = new decimal(new int[] {
            3000,
            0,
            0,
            0});
			this.intIntervalMsec.Minimum = new decimal(new int[] {
            10,
            0,
            0,
            0});
			this.intIntervalMsec.Name = "intIntervalMsec";
			this.intIntervalMsec.Size = new System.Drawing.Size(60, 22);
			this.intIntervalMsec.TabIndex = 17;
			this.intIntervalMsec.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
			this.intIntervalMsec.ValueChanged += new System.EventHandler(this.intIntervalMsec_ValueChanged);
			// 
			// intDataSets
			// 
			this.intDataSets.Location = new System.Drawing.Point(8, 27);
			this.intDataSets.Maximum = new decimal(new int[] {
            64,
            0,
            0,
            0});
			this.intDataSets.Name = "intDataSets";
			this.intDataSets.Size = new System.Drawing.Size(60, 22);
			this.intDataSets.TabIndex = 19;
			this.intDataSets.ValueChanged += new System.EventHandler(this.intDataSets_ValueChanged);
			// 
			// labelDataSets
			// 
			this.labelDataSets.AutoSize = true;
			this.labelDataSets.Location = new System.Drawing.Point(6, 12);
			this.labelDataSets.Name = "labelDataSets";
			this.labelDataSets.Size = new System.Drawing.Size(47, 12);
			this.labelDataSets.TabIndex = 20;
			this.labelDataSets.Text = "Data Sets";
			// 
			// labelAddress
			// 
			this.labelAddress.AutoSize = true;
			this.labelAddress.Location = new System.Drawing.Point(74, 12);
			this.labelAddress.Name = "labelAddress";
			this.labelAddress.Size = new System.Drawing.Size(42, 12);
			this.labelAddress.TabIndex = 22;
			this.labelAddress.Text = "Address";
			this.labelAddress.UseMnemonic = false;
			// 
			// intAddress
			// 
			this.intAddress.Location = new System.Drawing.Point(74, 27);
			this.intAddress.Maximum = new decimal(new int[] {
            65535,
            0,
            0,
            0});
			this.intAddress.Name = "intAddress";
			this.intAddress.Size = new System.Drawing.Size(60, 22);
			this.intAddress.TabIndex = 21;
			this.intAddress.Value = new decimal(new int[] {
            1000,
            0,
            0,
            0});
			this.intAddress.ValueChanged += new System.EventHandler(this.intAddress_ValueChanged);
			// 
			// dbDataSets
			// 
			this.dbDataSets.AllowUserToAddRows = false;
			this.dbDataSets.AllowUserToDeleteRows = false;
			this.dbDataSets.CausesValidation = false;
			this.dbDataSets.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
			this.dbDataSets.ContextMenuStrip = this.dbgRightKeyMenu;
			this.dbDataSets.Location = new System.Drawing.Point(3, 55);
			this.dbDataSets.Name = "dbDataSets";
			this.dbDataSets.RowTemplate.Height = 24;
			this.dbDataSets.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
			this.dbDataSets.Size = new System.Drawing.Size(1022, 313);
			this.dbDataSets.TabIndex = 23;
			this.dbDataSets.CellEndEdit += new System.Windows.Forms.DataGridViewCellEventHandler(this.dbDataSets_CellEndEdit);
			this.dbDataSets.CellValueChanged += new System.Windows.Forms.DataGridViewCellEventHandler(this.dbDataSets_CellValueChanged);
			this.dbDataSets.EditingControlShowing += new System.Windows.Forms.DataGridViewEditingControlShowingEventHandler(this.dbDataSets_EditingControlShowing);
			this.dbDataSets.KeyDown += new System.Windows.Forms.KeyEventHandler(this.dbDataSets_KeyDown);
			// 
			// dbgRightKeyMenu
			// 
			this.dbgRightKeyMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fontColorToolStripMenuItem,
            this.backgroundColorToolStripMenuItem,
            this.toolStripMenuItem2,
            this.hideToolStripMenuItem,
            this.showAllToolStripMenuItem,
            this.DefaultRow,
            this.InsertRow,
            this.DeleteRow,
            this.toolStripMenuItem3,
            this.fontSizeToolStripMenuItem});
			this.dbgRightKeyMenu.Name = "dbgRightKeyMenu";
			this.dbgRightKeyMenu.Size = new System.Drawing.Size(171, 192);
			// 
			// fontColorToolStripMenuItem
			// 
			this.fontColorToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.redToolStripMenuItem,
            this.greenToolStripMenuItem,
            this.blueToolStripMenuItem,
            this.blackToolStripMenuItem,
            this.blackToolStripMenuItem1,
            this.whiteToolStripMenuItem,
            this.grayToolStripMenuItem});
			this.fontColorToolStripMenuItem.Name = "fontColorToolStripMenuItem";
			this.fontColorToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
			this.fontColorToolStripMenuItem.Text = "Font Color";
			// 
			// redToolStripMenuItem
			// 
			this.redToolStripMenuItem.Name = "redToolStripMenuItem";
			this.redToolStripMenuItem.Size = new System.Drawing.Size(105, 22);
			this.redToolStripMenuItem.Text = "Red";
			this.redToolStripMenuItem.Click += new System.EventHandler(this.RowForeColor_Click);
			// 
			// greenToolStripMenuItem
			// 
			this.greenToolStripMenuItem.Name = "greenToolStripMenuItem";
			this.greenToolStripMenuItem.Size = new System.Drawing.Size(105, 22);
			this.greenToolStripMenuItem.Text = "Green";
			this.greenToolStripMenuItem.Click += new System.EventHandler(this.RowForeColor_Click);
			// 
			// blueToolStripMenuItem
			// 
			this.blueToolStripMenuItem.Name = "blueToolStripMenuItem";
			this.blueToolStripMenuItem.Size = new System.Drawing.Size(105, 22);
			this.blueToolStripMenuItem.Text = "Blue";
			this.blueToolStripMenuItem.Click += new System.EventHandler(this.RowForeColor_Click);
			// 
			// blackToolStripMenuItem
			// 
			this.blackToolStripMenuItem.Name = "blackToolStripMenuItem";
			this.blackToolStripMenuItem.Size = new System.Drawing.Size(102, 6);
			// 
			// blackToolStripMenuItem1
			// 
			this.blackToolStripMenuItem1.Name = "blackToolStripMenuItem1";
			this.blackToolStripMenuItem1.Size = new System.Drawing.Size(105, 22);
			this.blackToolStripMenuItem1.Text = "Black";
			this.blackToolStripMenuItem1.Click += new System.EventHandler(this.RowForeColor_Click);
			// 
			// whiteToolStripMenuItem
			// 
			this.whiteToolStripMenuItem.Name = "whiteToolStripMenuItem";
			this.whiteToolStripMenuItem.Size = new System.Drawing.Size(105, 22);
			this.whiteToolStripMenuItem.Text = "White";
			this.whiteToolStripMenuItem.Click += new System.EventHandler(this.RowForeColor_Click);
			// 
			// grayToolStripMenuItem
			// 
			this.grayToolStripMenuItem.Name = "grayToolStripMenuItem";
			this.grayToolStripMenuItem.Size = new System.Drawing.Size(105, 22);
			this.grayToolStripMenuItem.Text = "Gray";
			this.grayToolStripMenuItem.Click += new System.EventHandler(this.RowForeColor_Click);
			// 
			// backgroundColorToolStripMenuItem
			// 
			this.backgroundColorToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.redToolStripMenuItem1,
            this.greenToolStripMenuItem1,
            this.blueToolStripMenuItem1,
            this.toolStripMenuItem1,
            this.blackToolStripMenuItem2,
            this.whiteToolStripMenuItem1,
            this.grayToolStripMenuItem1});
			this.backgroundColorToolStripMenuItem.Name = "backgroundColorToolStripMenuItem";
			this.backgroundColorToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
			this.backgroundColorToolStripMenuItem.Text = "Background Color";
			// 
			// redToolStripMenuItem1
			// 
			this.redToolStripMenuItem1.Name = "redToolStripMenuItem1";
			this.redToolStripMenuItem1.Size = new System.Drawing.Size(105, 22);
			this.redToolStripMenuItem1.Text = "Red";
			this.redToolStripMenuItem1.Click += new System.EventHandler(this.RowBackColor_Click);
			// 
			// greenToolStripMenuItem1
			// 
			this.greenToolStripMenuItem1.Name = "greenToolStripMenuItem1";
			this.greenToolStripMenuItem1.Size = new System.Drawing.Size(105, 22);
			this.greenToolStripMenuItem1.Text = "Green";
			this.greenToolStripMenuItem1.Click += new System.EventHandler(this.RowBackColor_Click);
			// 
			// blueToolStripMenuItem1
			// 
			this.blueToolStripMenuItem1.Name = "blueToolStripMenuItem1";
			this.blueToolStripMenuItem1.Size = new System.Drawing.Size(105, 22);
			this.blueToolStripMenuItem1.Text = "Blue";
			this.blueToolStripMenuItem1.Click += new System.EventHandler(this.RowBackColor_Click);
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(102, 6);
			// 
			// blackToolStripMenuItem2
			// 
			this.blackToolStripMenuItem2.Name = "blackToolStripMenuItem2";
			this.blackToolStripMenuItem2.Size = new System.Drawing.Size(105, 22);
			this.blackToolStripMenuItem2.Text = "Black";
			this.blackToolStripMenuItem2.Click += new System.EventHandler(this.RowBackColor_Click);
			// 
			// whiteToolStripMenuItem1
			// 
			this.whiteToolStripMenuItem1.Name = "whiteToolStripMenuItem1";
			this.whiteToolStripMenuItem1.Size = new System.Drawing.Size(105, 22);
			this.whiteToolStripMenuItem1.Text = "White";
			this.whiteToolStripMenuItem1.Click += new System.EventHandler(this.RowBackColor_Click);
			// 
			// grayToolStripMenuItem1
			// 
			this.grayToolStripMenuItem1.Name = "grayToolStripMenuItem1";
			this.grayToolStripMenuItem1.Size = new System.Drawing.Size(105, 22);
			this.grayToolStripMenuItem1.Text = "Gray";
			this.grayToolStripMenuItem1.Click += new System.EventHandler(this.RowBackColor_Click);
			// 
			// toolStripMenuItem2
			// 
			this.toolStripMenuItem2.Name = "toolStripMenuItem2";
			this.toolStripMenuItem2.Size = new System.Drawing.Size(167, 6);
			// 
			// hideToolStripMenuItem
			// 
			this.hideToolStripMenuItem.Name = "hideToolStripMenuItem";
			this.hideToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
			this.hideToolStripMenuItem.Text = "Hide";
			this.hideToolStripMenuItem.Click += new System.EventHandler(this.hideToolStripMenuItem_Click);
			// 
			// showAllToolStripMenuItem
			// 
			this.showAllToolStripMenuItem.Name = "showAllToolStripMenuItem";
			this.showAllToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
			this.showAllToolStripMenuItem.Text = "Show All";
			this.showAllToolStripMenuItem.Click += new System.EventHandler(this.showAllToolStripMenuItem_Click);
			// 
			// DefaultRow
			// 
			this.DefaultRow.Name = "DefaultRow";
			this.DefaultRow.Size = new System.Drawing.Size(170, 22);
			this.DefaultRow.Text = "Default this row";
			this.DefaultRow.Click += new System.EventHandler(this.DefaultRow_Click);
			// 
			// InsertRow
			// 
			this.InsertRow.Name = "InsertRow";
			this.InsertRow.Size = new System.Drawing.Size(170, 22);
			this.InsertRow.Text = "Insert one row";
			this.InsertRow.Click += new System.EventHandler(this.InsertRow_Click);
			// 
			// DeleteRow
			// 
			this.DeleteRow.Name = "DeleteRow";
			this.DeleteRow.Size = new System.Drawing.Size(170, 22);
			this.DeleteRow.Text = "Delete this row";
			this.DeleteRow.Click += new System.EventHandler(this.DeleteRow_Click);
			// 
			// toolStripMenuItem3
			// 
			this.toolStripMenuItem3.Name = "toolStripMenuItem3";
			this.toolStripMenuItem3.Size = new System.Drawing.Size(167, 6);
			// 
			// fontSizeToolStripMenuItem
			// 
			this.fontSizeToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.FontSize9,
            this.FontSize11,
            this.FontSize13,
            this.FontSize15});
			this.fontSizeToolStripMenuItem.Name = "fontSizeToolStripMenuItem";
			this.fontSizeToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
			this.fontSizeToolStripMenuItem.Text = "Font Size";
			// 
			// FontSize9
			// 
			this.FontSize9.Name = "FontSize9";
			this.FontSize9.Size = new System.Drawing.Size(86, 22);
			this.FontSize9.Text = "9";
			this.FontSize9.Click += new System.EventHandler(this.updateFontSize);
			// 
			// FontSize11
			// 
			this.FontSize11.Name = "FontSize11";
			this.FontSize11.Size = new System.Drawing.Size(86, 22);
			this.FontSize11.Text = "11";
			this.FontSize11.Click += new System.EventHandler(this.updateFontSize);
			// 
			// FontSize13
			// 
			this.FontSize13.Name = "FontSize13";
			this.FontSize13.Size = new System.Drawing.Size(86, 22);
			this.FontSize13.Text = "13";
			this.FontSize13.Click += new System.EventHandler(this.updateFontSize);
			// 
			// FontSize15
			// 
			this.FontSize15.Name = "FontSize15";
			this.FontSize15.Size = new System.Drawing.Size(86, 22);
			this.FontSize15.Text = "15";
			this.FontSize15.Click += new System.EventHandler(this.updateFontSize);
			// 
			// buttonExport
			// 
			this.buttonExport.Location = new System.Drawing.Point(842, 26);
			this.buttonExport.Name = "buttonExport";
			this.buttonExport.Size = new System.Drawing.Size(60, 22);
			this.buttonExport.TabIndex = 24;
			this.buttonExport.Text = "Export";
			this.buttonExport.UseVisualStyleBackColor = true;
			this.buttonExport.Click += new System.EventHandler(this.buttonExport_Click);
			// 
			// buttonImport
			// 
			this.buttonImport.Location = new System.Drawing.Point(908, 26);
			this.buttonImport.Name = "buttonImport";
			this.buttonImport.Size = new System.Drawing.Size(60, 22);
			this.buttonImport.TabIndex = 25;
			this.buttonImport.Text = "Import";
			this.buttonImport.UseVisualStyleBackColor = true;
			this.buttonImport.Click += new System.EventHandler(this.buttonImport_Click);
			// 
			// labelSlaveID
			// 
			this.labelSlaveID.AutoSize = true;
			this.labelSlaveID.Location = new System.Drawing.Point(521, 13);
			this.labelSlaveID.Name = "labelSlaveID";
			this.labelSlaveID.Size = new System.Drawing.Size(45, 12);
			this.labelSlaveID.TabIndex = 27;
			this.labelSlaveID.Text = "Slave ID";
			// 
			// intSlaveID
			// 
			this.intSlaveID.Location = new System.Drawing.Point(523, 26);
			this.intSlaveID.Maximum = new decimal(new int[] {
            127,
            0,
            0,
            0});
			this.intSlaveID.Name = "intSlaveID";
			this.intSlaveID.Size = new System.Drawing.Size(60, 22);
			this.intSlaveID.TabIndex = 28;
			this.intSlaveID.Value = new decimal(new int[] {
            3,
            0,
            0,
            0});
			// 
			// buttonTest
			// 
			this.buttonTest.Location = new System.Drawing.Point(974, 26);
			this.buttonTest.Name = "buttonTest";
			this.buttonTest.Size = new System.Drawing.Size(66, 23);
			this.buttonTest.TabIndex = 29;
			this.buttonTest.Text = "test";
			this.buttonTest.UseVisualStyleBackColor = true;
			this.buttonTest.Visible = false;
			this.buttonTest.Click += new System.EventHandler(this.buttonTest_Click);
			// 
			// statusBar
			// 
			this.statusBar.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.smsg});
			this.statusBar.Location = new System.Drawing.Point(0, 428);
			this.statusBar.Name = "statusBar";
			this.statusBar.Size = new System.Drawing.Size(1052, 22);
			this.statusBar.TabIndex = 30;
			this.statusBar.Text = "statusStrip1";
			// 
			// smsg
			// 
			this.smsg.Name = "smsg";
			this.smsg.Size = new System.Drawing.Size(72, 17);
			this.smsg.Text = "No Message";
			// 
			// connectStatus
			// 
			this.connectStatus.Appearance = System.Windows.Forms.Appearance.Button;
			this.connectStatus.BackColor = System.Drawing.Color.Lime;
			this.connectStatus.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
			this.connectStatus.Enabled = false;
			this.connectStatus.Location = new System.Drawing.Point(408, 19);
			this.connectStatus.Name = "connectStatus";
			this.connectStatus.Size = new System.Drawing.Size(96, 36);
			this.connectStatus.TabIndex = 31;
			this.connectStatus.UseVisualStyleBackColor = false;
			// 
			// connectLED
			// 
			this.connectLED.Interval = 500;
			this.connectLED.Tick += new System.EventHandler(this.connectLED_Tick);
			// 
			// rdRegX
			// 
			this.rdRegX.Controls.Add(this.labCopyRight);
			this.rdRegX.Controls.Add(this.labVersion);
			this.rdRegX.Controls.Add(this.intDataSets);
			this.rdRegX.Controls.Add(this.labelDataSets);
			this.rdRegX.Controls.Add(this.intAddress);
			this.rdRegX.Controls.Add(this.labelAddress);
			this.rdRegX.Controls.Add(this.dbDataSets);
			this.rdRegX.Location = new System.Drawing.Point(12, 54);
			this.rdRegX.Name = "rdRegX";
			this.rdRegX.Size = new System.Drawing.Size(1028, 371);
			this.rdRegX.TabIndex = 32;
			// 
			// labCopyRight
			// 
			this.labCopyRight.AutoSize = true;
			this.labCopyRight.Location = new System.Drawing.Point(914, 29);
			this.labCopyRight.Name = "labCopyRight";
			this.labCopyRight.Size = new System.Drawing.Size(101, 12);
			this.labCopyRight.TabIndex = 35;
			this.labCopyRight.Text = "yanbai7@gmail.com";
			// 
			// labVersion
			// 
			this.labVersion.AutoSize = true;
			this.labVersion.Location = new System.Drawing.Point(895, 12);
			this.labVersion.Name = "labVersion";
			this.labVersion.Size = new System.Drawing.Size(61, 12);
			this.labVersion.TabIndex = 34;
			this.labVersion.Text = "FreeVersion";
			// 
			// enErrorStop
			// 
			this.enErrorStop.AutoSize = true;
			this.enErrorStop.Location = new System.Drawing.Point(750, 28);
			this.enErrorStop.Name = "enErrorStop";
			this.enErrorStop.Size = new System.Drawing.Size(70, 16);
			this.enErrorStop.TabIndex = 33;
			this.enErrorStop.Text = "ErrorStop";
			this.enErrorStop.UseVisualStyleBackColor = true;
			// 
			// Main
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(1052, 450);
			this.Controls.Add(this.enErrorStop);
			this.Controls.Add(this.intTimeoutMsec);
			this.Controls.Add(this.labelTimeout);
			this.Controls.Add(this.rdRegX);
			this.Controls.Add(this.statusBar);
			this.Controls.Add(this.buttonTest);
			this.Controls.Add(this.intSlaveID);
			this.Controls.Add(this.intIntervalMsec);
			this.Controls.Add(this.labelSlaveID);
			this.Controls.Add(this.labelInterval);
			this.Controls.Add(this.buttonImport);
			this.Controls.Add(this.buttonExport);
			this.Controls.Add(this.checkBoxConnect);
			this.Controls.Add(this.labelFlow);
			this.Controls.Add(this.FlowControl);
			this.Controls.Add(this.labelParity);
			this.Controls.Add(this.labelStopBits);
			this.Controls.Add(this.labelDataBits);
			this.Controls.Add(this.ParityList);
			this.Controls.Add(this.StopbitsList);
			this.Controls.Add(this.labelBaudrate);
			this.Controls.Add(this.LabelComPort);
			this.Controls.Add(this.DatabitsList);
			this.Controls.Add(this.BaudrateList);
			this.Controls.Add(this.ComPortList);
			this.Controls.Add(this.connectStatus);
			this.Name = "Main";
			this.Text = "Simple Poll";
			this.Resize += new System.EventHandler(this.SimplePoll_Resize);
			((System.ComponentModel.ISupportInitialize)(this.intTimeoutMsec)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.intIntervalMsec)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.intDataSets)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.intAddress)).EndInit();
			((System.ComponentModel.ISupportInitialize)(this.dbDataSets)).EndInit();
			this.dbgRightKeyMenu.ResumeLayout(false);
			((System.ComponentModel.ISupportInitialize)(this.intSlaveID)).EndInit();
			this.statusBar.ResumeLayout(false);
			this.statusBar.PerformLayout();
			this.rdRegX.ResumeLayout(false);
			this.rdRegX.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.IO.Ports.SerialPort serialPort1;
		private System.Windows.Forms.ComboBox ComPortList;
		private System.Windows.Forms.ComboBox BaudrateList;
		private System.Windows.Forms.ComboBox DatabitsList;
		private System.Windows.Forms.Label LabelComPort;
		private System.Windows.Forms.Label labelBaudrate;
		private System.Windows.Forms.ComboBox StopbitsList;
		private System.Windows.Forms.ComboBox ParityList;
		private System.Windows.Forms.Label labelDataBits;
		private System.Windows.Forms.Label labelStopBits;
		private System.Windows.Forms.Label labelParity;
		private System.Windows.Forms.Label labelFlow;
		private System.Windows.Forms.ComboBox FlowControl;
		private System.Windows.Forms.CheckBox checkBoxConnect;
		private System.Windows.Forms.Timer timerInterval;
		private System.Windows.Forms.NumericUpDown intTimeoutMsec;
		private System.Windows.Forms.Label labelTimeout;
		private System.Windows.Forms.Label labelInterval;
		private System.Windows.Forms.NumericUpDown intIntervalMsec;
		private System.Windows.Forms.NumericUpDown intDataSets;
		private System.Windows.Forms.Label labelDataSets;
		private System.Windows.Forms.Label labelAddress;
		private System.Windows.Forms.NumericUpDown intAddress;
		private System.Windows.Forms.DataGridView dbDataSets;
		private System.Windows.Forms.Button buttonExport;
		private System.Windows.Forms.Button buttonImport;
		private System.Windows.Forms.Label labelSlaveID;
		private System.Windows.Forms.NumericUpDown intSlaveID;
		private System.Windows.Forms.Button buttonTest;
		private System.Windows.Forms.StatusStrip statusBar;
		private System.Windows.Forms.ToolStripStatusLabel smsg;
		private System.Windows.Forms.ContextMenuStrip dbgRightKeyMenu;
		private System.Windows.Forms.ToolStripMenuItem fontColorToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem backgroundColorToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
		private System.Windows.Forms.ToolStripMenuItem hideToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem redToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem greenToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem blueToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator blackToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem blackToolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem whiteToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem grayToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem redToolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem greenToolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem blueToolStripMenuItem1;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem blackToolStripMenuItem2;
		private System.Windows.Forms.ToolStripMenuItem whiteToolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem grayToolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem showAllToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
		private System.Windows.Forms.ToolStripMenuItem fontSizeToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem FontSize9;
		private System.Windows.Forms.ToolStripMenuItem FontSize11;
		private System.Windows.Forms.ToolStripMenuItem FontSize13;
		private System.Windows.Forms.ToolStripMenuItem FontSize15;
		private System.Windows.Forms.CheckBox connectStatus;
		private System.Windows.Forms.Timer connectLED;
		private System.Windows.Forms.ToolStripMenuItem DeleteRow;
		private System.Windows.Forms.ToolStripMenuItem DefaultRow;
		private System.Windows.Forms.ToolStripMenuItem InsertRow;
		private System.Windows.Forms.Panel rdRegX;
		private System.Windows.Forms.CheckBox enErrorStop;
		private System.Windows.Forms.Label labCopyRight;
		private System.Windows.Forms.Label labVersion;
	}
}

