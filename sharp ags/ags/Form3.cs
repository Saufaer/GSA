using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace ags
{



    public partial class Form3 : Form
    {




        public Form3()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            global Glob = new global();
            Pointer p = new Pointer();
            List<ChX> RX = new List<ChX>();

            Glob.coeff[0] = Convert.ToDouble(textBox1.Text);
            Glob.coeff[1] = Convert.ToDouble(textBox2.Text);
            Glob.coeff[2] = Convert.ToDouble(textBox3.Text);
            Glob.coeff[3] = Convert.ToDouble(textBox4.Text);

            Glob.r = Convert.ToDouble(textBox5.Text);
            Glob.left = Convert.ToDouble(textBox6.Text);
            Glob.right = Convert.ToDouble(textBox7.Text);

            Glob.E = Convert.ToDouble(Boxeps.Text);
            Glob.Nmax = Convert.ToInt32(BoxmaxN.Text);


            chart1.ChartAreas["ChartArea1"].AxisX.TitleFont = new System.Drawing.Font("Microsoft Sans Serif", 16);
            chart1.ChartAreas["ChartArea1"].AxisY.TitleFont = new System.Drawing.Font("Microsoft Sans Serif", 16);
            chart1.ChartAreas["ChartArea1"].AxisX.Title = "X";
            chart1.ChartAreas["ChartArea1"].AxisY.Title = "F(X)";
            chart1.ChartAreas["ChartArea1"].AxisX.Minimum = Glob.left - 0.5;
            chart1.ChartAreas["ChartArea1"].AxisX.Maximum = Glob.right + 0.5;

            chart1.Series["Траектория"].Points.Clear();
            chart1.Series["Результат"].Points.Clear();

            if (radioButton1.Checked)
            {
   
                p=(Glob.Strongin(RX, Glob.left, Glob.right,  Glob.coeff, Glob.r, Glob.Nmax, Glob.E));
            }

            if (radioButton2.Checked)
            {
 
                p=(Glob.Piavsky(RX, Glob.left, Glob.right,  Glob.coeff, Glob.r, Glob.Nmax, Glob.E));
            }

            if (radioButton3.Checked )
            {
 
                p=(Glob.Simple(RX, Glob.left, Glob.right,  Glob.coeff, Glob.Nmax, Glob.E));
            }
            double minOY = Glob.func(Glob.left, Glob.coeff);
            for (double i = Glob.left; i <= Glob.right; i += 0.1)
            {
                double Z = Glob.func(i, Glob.coeff);
                chart1.Series["Траектория"].Points.AddXY(i, Z);
                if (Z < minOY)
                {
                    minOY = Z;
                }
            }

            chart1.Series["Результат"].Points.AddXY(p.x, p.z);


            chart1.Series["Точки испытаний"].Points.Clear();
            for (int i = 0; i < RX.Count; i++)
            {
                chart1.Series["Точки испытаний"].Points.AddXY(RX[i].Getx(), minOY - 1);
            }


            textBox8.Text = Convert.ToString(p.steps);
            textBox9.Text = Convert.ToString(p.z);
            textBox10.Text = Convert.ToString(p.x);
        }   
    }
}

