#include "Optim.h"
using namespace std;
global::Pointer global::PP()
{
    RZ rz;//���������
    Pointer point;//��������� - ���������
    point.steps = 0;//����
    currentE = right - left; //������� �������� �������
    rz.z = func(left);
    rz.R = 0;
    XRZ.insert(pair<double, RZ>(left, rz));
    rz.z = func(right);
    rz.R = 0;
    XRZ.insert(pair<double, RZ>(right, rz));

    maxM = Calculate_M(right, left, XRZ.at(right).z, XRZ.at(left).z);
    newX = right;

    double m = Calculate_m(); //������ ��������� �������

    map <double, RZ>::iterator num = ++(XRZ.begin());//�������� ��� ��������� � ���� R
    map <double, RZ>::iterator backnum = (XRZ.begin());//�������� �� ��� ����� �������
    double maxR = Calculate_R(m, num, backnum);//������������� ��������� ���� R

    double itcurE;//�������� �� ������� ���������


                  //#pragma omp critical
                  //    { double st = omp_get_wtime();
                  //    double fn = omp_get_wtime();
                  //    double time = fn - st;
                  //    alltime += time;
                  //    }


                  //------------------------------------------------�������� ���� ���������

    vector <bord> bb(procs);
    while (true)
    {
        maxR = (++(XRZ.begin()))->second.R;
        m = Calculate_m();//����� m       
        num = ++(XRZ.begin());
        backnum = XRZ.begin();


        if ((XRZ.size() >= procs + 1) && (procs != 1))
        {
#pragma omp parallel for num_threads(procs)
            for (int i = 0; i < procs; i++)
            {


                bb[i].num = (++(XRZ.find(bb[i].shiftl)));


                bb[i].backnum = ((XRZ.find(bb[i].shiftl)));


                bb[i].R = bb[i].num->second.R;

                map <double, RZ>::iterator it, back;

                it = bb[i].num; back = bb[i].backnum;


                while (true)
                {
                    double xr = it->first;
                    double xl = back->first;
                    double zr = it->second.z;
                    double zl = back->second.z;
                    double R = m * (xr - xl) + (((zr - zl) *
                        (zr - zl)) / (m * (xr - xl))) -
                        2 * (zr + zl);


                    if (R > bb[i].R)
                    {
                        //////////////////////////////

                        bb[i].R = R;
                        bb[i].num = it;
                        bb[i].backnum = back;


                        /////////////////////////////////
                    }

                    ++it; ++back;
                    if (i == procs - 1)
                    {
                        if (it == XRZ.end())
                        {
                            break;
                        }
                    }
                    else
                    {
                        if (it == ++XRZ.find(bb[i].shiftr)) { break; }
                    }
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
        rz.R = 0;
        rz.z = func(newX);
        XRZ.insert(pair<double, RZ>(newX, rz));

        currentE = newX - (--XRZ.find(newX))->first;
        if (currentE <= E)//����� �� ��������
        {
            break;
        }
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


        point.steps++;

        if (point.steps == Nmax)//����� �� ����� �����
        {
            break;
        }
    }

    point.x = newX;
    point.z = rz.z;
    return point;


}