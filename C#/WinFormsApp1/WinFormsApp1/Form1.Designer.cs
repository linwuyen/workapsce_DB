namespace WinFormsApp1
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            textBox1 = new TextBox();
            button1 = new Button();
            button2 = new Button();
            comboBox1 = new ComboBox();
            comboBox2 = new ComboBox();
            comboBox3 = new ComboBox();
            comboBox4 = new ComboBox();
            comboBox5 = new ComboBox();
            button3 = new Button();
            SuspendLayout();
            // 
            // textBox1
            // 
            textBox1.Location = new Point(256, 45);
            textBox1.Multiline = true;
            textBox1.Name = "textBox1";
            textBox1.ScrollBars = ScrollBars.Vertical;
            textBox1.Size = new Size(368, 143);
            textBox1.TabIndex = 0;
            textBox1.TextChanged += textBox1_TextChanged;
            // 
            // button1
            // 
            button1.Location = new Point(158, 259);
            button1.Name = "button1";
            button1.Size = new Size(75, 23);
            button1.TabIndex = 1;
            button1.Text = "Open";
            button1.UseVisualStyleBackColor = true;
            button1.Click += button1_Click;
            // 
            // button2
            // 
            button2.Location = new Point(366, 259);
            button2.Name = "button2";
            button2.Size = new Size(75, 23);
            button2.TabIndex = 2;
            button2.Text = "Send";
            button2.UseVisualStyleBackColor = true;
            button2.Click += button2_Click;
            // 
            // comboBox1
            // 
            comboBox1.DropDownStyle = ComboBoxStyle.DropDownList;
            comboBox1.FormattingEnabled = true;
            comboBox1.Location = new Point(77, 35);
            comboBox1.Name = "comboBox1";
            comboBox1.Size = new Size(121, 23);
            comboBox1.TabIndex = 3;
            // 
            // comboBox2
            // 
            comboBox2.DropDownStyle = ComboBoxStyle.DropDownList;
            comboBox2.FormattingEnabled = true;
            comboBox2.Location = new Point(77, 64);
            comboBox2.Name = "comboBox2";
            comboBox2.Size = new Size(121, 23);
            comboBox2.TabIndex = 4;
            // 
            // comboBox3
            // 
            comboBox3.DropDownStyle = ComboBoxStyle.DropDownList;
            comboBox3.FormattingEnabled = true;
            comboBox3.Items.AddRange(new object[] { "8" });
            comboBox3.Location = new Point(77, 93);
            comboBox3.Name = "comboBox3";
            comboBox3.Size = new Size(121, 23);
            comboBox3.TabIndex = 5;
            comboBox3.SelectedIndexChanged += comboBox3_SelectedIndexChanged;
            // 
            // comboBox4
            // 
            comboBox4.DropDownStyle = ComboBoxStyle.DropDownList;
            comboBox4.FormattingEnabled = true;
            comboBox4.Items.AddRange(new object[] { "None" });
            comboBox4.Location = new Point(77, 122);
            comboBox4.Name = "comboBox4";
            comboBox4.Size = new Size(121, 23);
            comboBox4.TabIndex = 6;
            // 
            // comboBox5
            // 
            comboBox5.DropDownStyle = ComboBoxStyle.DropDownList;
            comboBox5.FormattingEnabled = true;
            comboBox5.Items.AddRange(new object[] { "1" });
            comboBox5.Location = new Point(77, 151);
            comboBox5.Name = "comboBox5";
            comboBox5.Size = new Size(121, 23);
            comboBox5.TabIndex = 7;
            // 
            // button3
            // 
            button3.Location = new Point(540, 259);
            button3.Name = "button3";
            button3.Size = new Size(75, 23);
            button3.TabIndex = 8;
            button3.Text = "Clear";
            button3.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(800, 450);
            Controls.Add(button3);
            Controls.Add(comboBox5);
            Controls.Add(comboBox4);
            Controls.Add(comboBox3);
            Controls.Add(comboBox2);
            Controls.Add(comboBox1);
            Controls.Add(button2);
            Controls.Add(button1);
            Controls.Add(textBox1);
            Name = "Form1";
            Text = "Form1";
            Load += Form1_Load;
            ResumeLayout(false);
            PerformLayout();
        }

        #endregion

        private TextBox textBox1;
        private Button button1;
        private Button button2;
        private ComboBox comboBox1;
        private ComboBox comboBox2;
        private ComboBox comboBox3;
        private ComboBox comboBox4;
        private ComboBox comboBox5;
        private Button button3;
    }
}