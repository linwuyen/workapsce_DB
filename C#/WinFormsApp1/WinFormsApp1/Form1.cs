using System.IO.Ports;

namespace WinFormsApp1
{
    public partial class Form1 : Form
    {
        SerialPort port = new SerialPort();

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {


            comboBox1.Items.AddRange(System.IO.Ports.SerialPort.GetPortNames());
            port.DataReceived += new SerialDataReceivedEventHandler(SerialPort1_DataReceived);

            int i;
            for (i = 30; i <= 3840; i = i * 2)
            {
                comboBox2.Items.Add(i.ToString());
            }

            //
            string[] baud = { "43000", "56000", "57600", "115200", "128000", "230400", "256000", "460800" };
            comboBox2.Items.AddRange(baud);
            //
            comboBox1.Text = "COM1";
            comboBox2.Text = "115200";
            comboBox3.Text = "8";
            comboBox4.Text = "None";
            comboBox5.Text = "1";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {

                if (port.IsOpen)
                {

                    port.Close();
                    button1.Text = "OPEN_0";
                    button1.BackColor = Color.ForestGreen;
                    comboBox1.Enabled = true;
                    comboBox2.Enabled = true;
                    comboBox3.Enabled = true;
                    comboBox4.Enabled = true;
                    comboBox5.Enabled = true;
                    textBox1.Text = "";
                    //     textBox_send.Text = "";     
                }
                else
                {

                    comboBox1.Enabled = false;
                    comboBox2.Enabled = false;
                    comboBox3.Enabled = false;
                    comboBox4.Enabled = false;
                    comboBox5.Enabled = false;
                    port.PortName = comboBox1.Text;
                    port.BaudRate = Convert.ToInt32(comboBox2.Text);
                    port.DataBits = Convert.ToInt16(comboBox3.Text);

                    if (comboBox4.Text.Equals("None"))
                        port.Parity = System.IO.Ports.Parity.None;
                    else if (comboBox4.Text.Equals("Odd"))
                        port.Parity = System.IO.Ports.Parity.Odd;
                    else if (comboBox4.Text.Equals("Even"))
                        port.Parity = System.IO.Ports.Parity.Even;
                    else if (comboBox4.Text.Equals("Mark"))
                        port.Parity = System.IO.Ports.Parity.Mark;
                    else if (comboBox4.Text.Equals("Space"))
                        port.Parity = System.IO.Ports.Parity.Space;

                    if (comboBox5.Text.Equals("1"))
                        port.StopBits = System.IO.Ports.StopBits.One;
                    else if (comboBox5.Text.Equals("1.5"))
                        port.StopBits = System.IO.Ports.StopBits.OnePointFive;
                    else if (comboBox5.Text.Equals("2"))
                        port.StopBits = System.IO.Ports.StopBits.Two;

                    port.Open();
                    button1.Text = "CLOSE";
                    button1.BackColor = Color.Firebrick;
                }
            }
            catch (Exception ex)
            {


                port = new System.IO.Ports.SerialPort();

                comboBox1.Items.Clear();
                comboBox1.Items.AddRange(System.IO.Ports.SerialPort.GetPortNames());

                System.Media.SystemSounds.Beep.Play();
                button1.Text = "OPEN";
                button1.BackColor = Color.ForestGreen;
                MessageBox.Show(ex.Message);
                comboBox1.Enabled = true;
                comboBox2.Enabled = true;
                comboBox3.Enabled = true;
                comboBox4.Enabled = true;
                comboBox5.Enabled = true;
            }

        }

        private void button2_Click(object sender, EventArgs e)
        {
            textBox1.Text = "";
        }

        private void comboBox3_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }
        
        private void SerialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            try
            {
                
                this.Invoke((EventHandler)(delegate
                {
                    textBox1.AppendText(port.ReadExisting());
                }
                   )
                );

            }
            catch (Exception ex)
            {
                
                System.Media.SystemSounds.Beep.Play();
                MessageBox.Show(ex.Message);

            }

        }
    }
}