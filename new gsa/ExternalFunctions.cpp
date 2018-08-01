#include "Optim.h"
using namespace std;
global::Pointer global::Serial_Search()
{
    RZ rz;//���������
    Pointer point;//��������� - ���������
    point.steps = 0;//����
    currentE = right - left; //������� �������� �������
                             //--------------------------------------------������ �������� ���������

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
    double maxR = Calculate_R(m, num, backnum);;//������������� ��������� ���� R

    double itcurE;//�������� �� ������� ���������

                  //------------------------------------------------�������� ���� ���������
    while (true)
    {
        maxR = (++(XRZ.begin()))->second.R;
        //�������� �������������
        m = Calculate_m();//����� m

        num = ++(XRZ.begin());
        backnum = (XRZ.begin());

        for (map <double, RZ>::iterator it = ++(XRZ.begin()), back = XRZ.begin() // ����������� ��������� � ������������ R
            ; it != XRZ.end();
            ++back, ++it
            )
        {
            (it)->second.R = Calculate_R(m, it, back);//������� R � ������� ���������

            if (XRZ.at(it->first).R > maxR)
            {
                maxR = XRZ.at(it->first).R;
                num = it;//��������� ����� ��������� (������ ��� ������ �������)
                backnum = back;

            }

            itcurE = it->first - back->first;//�������� �� ������� ��������
            if (itcurE < currentE)
            {
                currentE = itcurE;//�������� �������
            }
        }
        if (currentE <= E)//����� �� ��������
        {
            break;
        }

        newX = Calculate_X(m, num, backnum);//������ ��������� ����� ��������� � ��������� ���������

                                            //����c x � z � ����
        rz.R = 0;
        rz.z = func(newX);
        XRZ.insert(pair<double, RZ>(newX, rz));

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