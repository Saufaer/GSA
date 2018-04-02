using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.ComponentModel;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using System.Windows.Forms;

//#include <stdio.h>
//#include <math.h>
//#include <stdlib.h>

namespace ags
{

    using System;
    using System.Collections.Generic;


    public class Pointer
    {
        public double x { get; set; }
        public double z { get; set; }
        public int steps { get; set; }
    }


    //public class ChX
    //{

    //    public double x { get; set; }
    //    public double R { get; set; }

    //}
    public struct ChX
    {
       public double x;
       public double R;

        public ChX(double _x, double _R)
        {
            x = _x;
            R = _R;

        }
        public double Getx()
        {
            return x;
        }
        public double GetR()
        {
            return R;
        }
        public void SetRx(double _x, double _R)
        {
            x = _x;
            R = _R;
        }
        public void SetR(double _R)
        {

            R = _R;
        }
        public void Setx(double _x, double _R)
        {
            x = _x;
            R = _R;
        }
    }

    public class global
    {

        public double left;
        public double right;
        public double r;
        public double E;
        public int Nmax;
        public int S = 0;
        public double currentE = 0;
        public double[] coeff = new double[4];


        public double func(double x, double[] coeff)
        {
            //return sin(x) + sin((10. * x) / 3.);
            //return (3 * x - 14)*sin(1.8 * x);
            //return -x + sin(3 * x) - 1;
            return coeff[0] * Math.Sin(coeff[1] * x) + coeff[2] * Math.Cos(coeff[3] * x);
        }




        public Pointer Simple(List<ChX> RX, double left, double right, double[] coeff, int Nmax, double E)
        {
            //������� ������ ���� RX
            ChX RXpoint = new ChX();
            RXpoint.SetRx(left,0);

            RX.Add(RXpoint);
            RXpoint.SetRx(right, 0);

            RX.Add(RXpoint);
            Pointer point = new Pointer();
            point.steps = 2;
            point.x = left;
            point.z = func(left, coeff);

            double maxR;
            double currentE = right - left;

            double minz;
            double minx;
            
            if (func(right, coeff) < func(left, coeff))
            {
                point.x = right;
                point.z = func(right, coeff);
                minz = point.z;
                minx = point.x;
            }
            else
            {
                point.x = left;
                point.z = func(left, coeff);
                minz = point.z;
                minx = point.x;
            }

            ChX RXpoint1 = new ChX();
            RXpoint1.SetR(RSimple(RX, 1));
            RX[0] = (RXpoint1);


            double newX = 0;
            while ((point.steps < Nmax) && (currentE > E))
            {
                if (minz > func(newX, coeff))
                {
                    minz = func(newX, coeff);
                    minx = newX;
                }
                maxR = RX[0].GetR();
                newX = XSimple(RX, coeff, 1);

                //����������� ��������� � ������������ R
                for (int i = 1; i < RX.Count; i++)
                {
                    if (RX[i - 1].GetR() > maxR)
                    {
                        maxR = RX[i - 1].GetR();
                        newX = XSimple(RX, coeff, i);
                    }
                    //������ ��� ������� ������
                    if ((RX[i].Getx() - RX[i - 1].Getx() < currentE) )
                    {
                        currentE = RX[i].Getx() - RX[i - 1].Getx();
                    }
                }

                //������� RX
                RXpoint.SetRx(newX, 0);

                RX.Add(RXpoint);

                RX.Sort((a, b) => a.Getx().CompareTo(b.Getx()));

        


                //����� R
                for (int i = 1; i < RX.Count; i++)
                {
                    RXpoint1 = RX[i - 1];
                    RXpoint1.SetR(RSimple(RX, i));
                    RX[i - 1]= (RXpoint1);

                }
                point.steps++;

            }
            point.x = minx;
            point.z = minz;
            return point;
        }
        public Pointer Piavsky(List<ChX> RX, double left, double right, double[] coeff, double r, int Kmax, double E)
        {
            ChX RXpoint = new ChX();
            RXpoint.SetRx(left, 0);

            RX.Add(RXpoint);
            RXpoint.SetRx(right, 0);

            RX.Add(RXpoint);
            Pointer point = new Pointer();
            point.steps = 2;
            point.x = left;
            point.z = func(left, coeff);

            double maxR;
            double currentE = right - left;


            if (func(right, coeff) < func(left, coeff))
            {
                point.x = right;
                point.z = func(right, coeff);
            }
            else
            {
                point.x = left;
                point.z = func(left, coeff);
            }


            double m = CalculateM(r, RX, coeff);

            ChX RXpoint1 = new ChX();
            RXpoint1.SetR(RPiavsky(RX, m, 1));
            RX[0] = (RXpoint1);
           // RX[0].R = RPiavsky(RX, m, 1);
            double newX = 0;
            while ((point.steps < Kmax) && (currentE > E))
            {
                maxR = RX[0].R;
                newX = X(RX, m, coeff, 1);

                
                for (int i = 1; i < RX.Count; i++)
                {
                    if (RX[i - 1].GetR() > maxR)
                    {
                        maxR = RX[i - 1].R;
                        newX = X(RX, m, coeff, i);
                    }
                    if ((RX[i].Getx() - RX[i - 1].Getx() < currentE))
                    {
                        
                        currentE = RX[i].Getx() - RX[i - 1].Getx();
                    }
                }

                //������� RX

                //RXpoint.x = newX;
                //RXpoint.R = 0;
                //RX.Add(RXpoint);

                RXpoint.SetRx(newX, 0);

                RX.Add(RXpoint);
                //C++ TO C# CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the .NET Comparison parameter produces a tri-state result:
                //ORIGINAL LINE: std::sort(RX.begin(), RX.end(), obj);
                RX.Sort((a, b) => a.Getx().CompareTo(b.Getx()));


                //����� R
                m = CalculateM(r, RX, coeff);
                for (int i = 1; i < RX.Count; i++)
                {
                    RXpoint1 = RX[i - 1];
                    RXpoint1.SetR(RPiavsky(RX, m, i));
                    RX[i - 1] = (RXpoint1);
                    //RX[i - 1].R = RPiavsky(RX, m, i);
                }
                point.steps++;

            }
            point.x = newX;
            point.z = func(newX, coeff);
            return point;


        }
        public Pointer Strongin(List<ChX> RX, double left, double right, double[] coeff, double r, int Kmax, double E)
        {
            ChX RXpoint = new ChX();
        RXpoint.SetRx(left, 0);

            RX.Add(RXpoint);
            RXpoint.SetRx(right, 0);

            RX.Add(RXpoint);
            Pointer point = new Pointer();
        point.steps = 2;
            point.x = left;
            point.z = func(left, coeff);

        double maxR;
        double currentE = right - left;


            if (func(right, coeff) < func(left, coeff))
            {
                point.x = right;
                point.z = func(right, coeff);
    }
            else
            {
                point.x = left;
                point.z = func(left, coeff);
}


double m = CalculateM(r, RX, coeff);

ChX RXpoint1 = new ChX();
RXpoint1.SetR(RStrongin(RX, m, 1));
            RX[0] = (RXpoint1);
           // RX[0].R = RPiavsky(RX, m, 1);
            double newX = 0;
            while ((point.steps<Kmax) && (currentE > E))
            {
                maxR = RX[0].R;
                newX = X(RX, m, coeff, 1);

                
                for (int i = 1; i<RX.Count; i++)
                {
                    if (RX[i - 1].GetR() > maxR)
                    {
                        maxR = RX[i - 1].R;
                        newX = X(RX, m, coeff, i);
                    }
                    if ((RX[i].Getx() - RX[i - 1].Getx() < currentE))
                    {
                        
                        currentE = RX[i].Getx() - RX[i - 1].Getx();
                    }
                }

                //������� RX

                //RXpoint.x = newX;
                //RXpoint.R = 0;
                //RX.Add(RXpoint);

                RXpoint.SetRx(newX, 0);

                RX.Add(RXpoint);
                //C++ TO C# CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the .NET Comparison parameter produces a tri-state result:
                //ORIGINAL LINE: std::sort(RX.begin(), RX.end(), obj);
                RX.Sort((a, b) => a.Getx().CompareTo(b.Getx()));


                //����� R
                m = CalculateM(r, RX, coeff);
                for (int i = 1; i<RX.Count; i++)
                {
                    RXpoint1 = RX[i - 1];
                    RXpoint1.SetR(RStrongin(RX, m, i));
                    RX[i - 1] = (RXpoint1);
                    //RX[i - 1].R = RPiavsky(RX, m, i);
                }
                point.steps++;

            }
            point.x = newX;
            point.z = func(newX, coeff);
            return point;


        }

        //		//������� ������ ���� RX
        //		ChX RXpoint = new ChX();
        //		RXpoint.x = left;
        //		RXpoint.R = 0;
        //		RX.Add(RXpoint);
        //		RXpoint.x = right;
        //		RXpoint.R = 0;
        //		RX.Add(RXpoint);
        //		Pointer point = new Pointer();
        //		point.steps = 0;
        //		point.x = left;
        //		point.z = func(left, coeff);

        //		double maxR;
        //		double currentE = right - left;
        //		//��� ������ ��������
        //		if (func(right, coeff) < func(left, coeff))
        //		{
        //			point.x = right;
        //			point.z = func(right, coeff);
        //		}
        //		else
        //		{
        //			point.x = left;
        //			point.z = func(left, coeff);
        //		}

        //		double m = CalculateM(r, RX, coeff);
        //		RX[0].R = RStrongin(RX, m, 1);
        //		double newX = 0;
        //		while ((point.steps < Kmax) && (currentE > E))
        //		{
        //			maxR = RX[0].R;
        //			newX = X(RX, m, coeff, 1);

        //			//����������� ��������� � ������������ R
        //			for (int i = 1; i < RX.Count; i++)
        //			{
        //				if (RX[i - 1].R > maxR)
        //				{
        //					maxR = RX[i - 1].R;
        //					newX = X(RX, m, coeff, i);
        //				}
        //				if ((int)(RX[i].x - RX[i - 1].x < currentE) != 0)
        //				{
        //					currentE = RX[i].x - RX[i - 1].x;
        //				}
        //			}

        //			//������� RX

        //			RXpoint.x = newX;
        //			RXpoint.R = 0;
        //			RX.Add(RXpoint);
        //	//C++ TO C# CONVERTER TODO TASK: The 'Compare' parameter of std::sort produces a boolean value, while the .NET Comparison parameter produces a tri-state result:
        //	//ORIGINAL LINE: std::sort(RX.begin(), RX.end(), obj);
        //			RX.Sort(obj);

        //			//����� R
        //			m = CalculateM(r, RX, coeff);

        //			for (int i = 1; i < RX.Count; i++)
        //			{
        //				RX[i - 1].R = RStrongin(RX, m, i);
        //			}
        //			point.steps++;

        //		}
        //		point.x = newX;
        //		point.z = func(newX, coeff);
        //		return point;


        //	}

        public double MCh(List<ChX> RX, double[] coeff, int i)
        {
            return Math.Abs(func(RX[i].Getx(), coeff) - func(RX[i - 1].Getx(), coeff)) / (RX[i].Getx() - RX[i - 1].Getx());
        }
        //  ������ ��������� �������
        public double CalculateM(double r, List<ChX> RX, double[] coeff)
        {
            double M = MCh(RX, coeff, 1);
            for (int i = 1; i < RX.Count; i++)
            {
                double currentM = MCh(RX, coeff, i);
                if (currentM > M)
                {
                    M = currentM;
                }
            }
            if (M > 0)
            {
                return r * M;
            }
            else //M==0
            {
                return 1.0;
            }
        }
        public double X(List<ChX> RX, double m, double[] coeff, int i)
        {
            return 0.5 * (RX[i].Getx() + RX[i - 1].Getx()) - (func(RX[i].Getx(), coeff) - func(RX[i - 1].Getx(), coeff)) / (2.0 * m);
        }
        public double XSimple(List<ChX> RX, double[] coeff, int i)
        {
            return 0.5 * (RX[i].Getx() + RX[i - 1].Getx());
        }
        public double RSimple(List<ChX> RX, int i)
        {
            return (RX[i].Getx() - RX[i - 1].Getx());
        }
        public double RPiavsky(List<ChX> RX, double m, int i)
        {
            return 0.5 * m * (RX[i].Getx() - RX[i - 1].Getx()) - (func(RX[i].Getx(), coeff) + func(RX[i - 1].Getx(), coeff)) / 2.0;
        }
        public double RStrongin(List<ChX> RX, double m, int i)
        {
            return m * (RX[i].Getx() - RX[i - 1].Getx()) + (((func(RX[i].Getx(), coeff) - func(RX[i - 1].Getx(), coeff)) * (func(RX[i].Getx(), coeff) - func(RX[i - 1].Getx(), coeff))) / (m * (RX[i].Getx() - RX[i - 1].Getx()))) - 2 * (func(RX[i].Getx(), coeff) + func(RX[i - 1].Getx(), coeff));
        }

    }


}