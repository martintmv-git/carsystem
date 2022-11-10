using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CARSYSTEMESO
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            serialPort1.Open();
            Tick.Start();
            InfoLoader();
        }
        private void button1_Click(object sender, EventArgs e)
        {
            serialPort1.WriteLine("alarm");
        }
        private void InfoLoader()
        {
            if (serialPort1.BytesToRead > 0)
            {
                string command = serialPort1.ReadLine();
                if (command.Contains("HIGHLIGHTS"))
                {
                    HiglightsLabel.Text = command;
                }
                else if (command.Contains("ALARM"))
                {
                    listBox1.Items.Add(command);
                }
                else if (command.Contains("temp"))
                {
                    tempLabel.Text = $"TEMPERATURE {command.Split()[1]} *C";
                }
                else
                {
                    listBox1.Items.Add(command);
                }

            }
        }
        private void tempLabel_Click(object sender, EventArgs e)
        {

        }

        private void Tick_Tick(object sender, EventArgs e)
        {
            InfoLoader();
        }
    }
}
