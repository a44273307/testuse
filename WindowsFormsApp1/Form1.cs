using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int sum = ControllerWrapper.testa_add(10, 20);
            
            //Console.WriteLine($"testa_add(10, 20) = {sum}");
            MessageBox.Show(sum.ToString());
            
        }

        private void Form1_Load(object sender, EventArgs e)
        {
           
        }
    }
}
