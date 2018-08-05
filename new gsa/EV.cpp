#include "Optim.h"
using namespace std;
global::PointerEV global::EV(double *leftBound, double *rightBound, vagrish::GrishaginFunction *gfunc)
{
    TEvolvent evolve(2, 20); // ������ ���������

    double *A = new double[2]; // ����������� �����
    double *B = new double[2]; // ����������� ������

    A[0] = leftBound[0]; // x
    A[1] = leftBound[1]; // y
    B[0] = rightBound[0]; // x
    B[1] = rightBound[1]; // y

    evolve.SetBounds(A, B); // ��������� �����������
    double *y_arr = new double[2];

    double borders[2] = { A[0], A[1] };


    RZ rz;//���������
    PointerEV pointEv;//��������� - ���������
    double x;
    double y;
    pointEv.steps = 0;//����
    currentE = right - left; //������� �������� �������
    rz.z = Func(borders, gfunc);
    rz.R = 0;
    XRZ.insert(pair<double, RZ>(0, rz));
    borders[0] = B[0]; borders[1] = B[1];
    rz.z = Func(borders, gfunc);
    rz.R = 0;
    XRZ.insert(pair<double, RZ>(1, rz));

    maxM = Calculate_M(1, 0, XRZ.at(1).z, XRZ.at(0).z);

    newX = 1;

    double m = Calculate_m(); //������ ��������� �������


    map <double, RZ>::iterator num = ++(XRZ.begin());//�������� ��� ��������� � ���� R
    map <double, RZ>::iterator backnum = (XRZ.begin());//�������� �� ��� ����� �������
    double maxR = Calculate_R(m, num, backnum);//������������� ��������� ���� R

                                               //------------------------------------------------�������� ���� ���������

    vector <bord> bb(procs);
    while (true)
    {

        ////////////////////////////////////////////////////��������� �������������


        if (XRZ.size() == procs + 1)
        {
            int i = 0;
            for (map <double, RZ>::iterator it = ++(XRZ.begin()), back = XRZ.begin()
                ; it != XRZ.end();
                ++back, ++it
                )
            {

                bb[i].shiftl = back->first;
                bb[i].shiftr = it->first;
                bb[i].weight = 0;

                i++;
            }

        }

        //����� � �����

        if (XRZ.size() > procs + 1)
        {
            for (int i = 0; i < procs; i++)
            {
                if ((newX < bb[i].shiftr) && (newX > bb[i].shiftl))//����������� ��� ����� �
                {
                    bb[i].weight++;


                }

                if ((i != procs - 1) && (bb[i].weight != 0))//���� i �����
                {
                    if (bb[i].weight > bb[i + 1].weight + 1)
                    {

                        bb[i].shiftr = (--(XRZ.find(bb[i].shiftr)))->first;
                        bb[i + 1].shiftl = (--(XRZ.find(bb[i + 1].shiftl)))->first;
                        bb[i].weight--;
                        bb[i + 1].weight++;

                        operations++;
                    }
                }
                if ((i != procs - 1) && (bb[i + 1].weight != 0))
                {

                    if (bb[i].weight < bb[i + 1].weight - 1)
                    {

                        bb[i].shiftr = (++(XRZ.find(bb[i].shiftr)))->first;
                        bb[i + 1].shiftl = (++(XRZ.find(bb[i + 1].shiftl)))->first;
                        bb[i].weight++;
                        bb[i + 1].weight--;

                        operations++;
                    }
                }


            }

        }

        /////////////////////////////////////////////////////

        maxR = (++(XRZ.begin()))->second.R;
        m = Calculate_m();//����� m 
        num = ++(XRZ.begin());
        backnum = XRZ.begin();


        if ((XRZ.size() >= procs + 1) && (procs != 1))
        {
#pragma omp parallel for num_threads(procs)
            for (int i = 0; i < procs; i++)
            {


                bb[i].num = ((XRZ.find(bb[i].shiftl)));


                bb[i].backnum = bb[i].num;
                ++bb[i].num;

                bb[i].R = bb[i].num->second.R;



                bb[i].curnum = bb[i].num;
                bb[i].curbacknum = bb[i].backnum;


                while (true)
                {


                    double xr = bb[i].curnum->first;
                    double xl = bb[i].curbacknum->first;
                    double zr = bb[i].curnum->second.z;
                    double zl = bb[i].curbacknum->second.z;

                    double R = m * (xr - xl) + (((zr - zl) *
                        (zr - zl)) / (m * (xr - xl))) -
                        2 * (zr + zl);


                    if (R > bb[i].R)
                    {
                        //////////////////////////////

                        bb[i].R = R;
                        bb[i].num = bb[i].curnum;
                        bb[i].backnum = bb[i].curbacknum;


                        /////////////////////////////////
                    }
                    ++bb[i].curnum;
                    ++bb[i].curbacknum;

                    if (bb[i].curbacknum->first == bb[i].shiftr) { break; }

                }



            }
            ///////////////////////////////////////

            for (int i = 0; i < procs; i++)
            {
                if (maxR < bb[i].R)
                {
                    maxR = bb[i].R;
                    num = bb[i].num;
                    backnum = bb[i].backnum;
                }

            }

            ///////////////////////////////////////
        }
        else//���� ������������ ���������� ��� procs==1
        {
            double xr;
            double xl;
            double zr;
            double zl;
            double R;
            for (map <double, RZ>::iterator it = ++(XRZ.begin()), back = XRZ.begin() // ����������� ��������� � ������������ R
                ; it != XRZ.end();
                ++back, ++it
                )
            {

                xr = it->first;
                xl = back->first;
                zr = it->second.z;
                zl = back->second.z;
                R = m * (xr - xl) + (((zr - zl) *
                    (zr - zl)) / (m * (xr - xl))) -
                    2 * (zr + zl);

                if (R > maxR)
                {
                    maxR = R;
                    num = it;//��������� ����� ��������� (������ ��� ������ �������)
                    backnum = back;

                }
            }


        }
        newX = Calculate_X(m, num, backnum);//������ ��������� ����� ��������� � ��������� ���������
                                            //����c x � z � ����
        evolve.GetImage(newX, y_arr);

        rz.R = 0;
        x = y_arr[0];
        y = y_arr[1];

        rz.z = Func(y_arr, gfunc);


        XRZ.insert(pair<double, RZ>(newX, rz));
        currentE = newX - (--XRZ.find(newX))->first;
        if (currentE <= E)//����� �� ��������
        {
            break;
        }




        pointEv.steps++;

        if (pointEv.steps == Nmax)//����� �� ����� �����
        {
            break;
        }
    }
    pointEv.x = x;
    pointEv.y = y;
    pointEv.z = rz.z;
    return pointEv;


}