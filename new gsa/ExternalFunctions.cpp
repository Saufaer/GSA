#include "Optim.h"
using namespace std;
global::Pointer global::Serial_Search()
{
    RZ rz;//���������
    Pointer point;//��������� - ���������
    point.steps = 1;//����
    currentE = right - left; //������� �������� �������

    //--------------------------------------------������ �������� ���������
    newX = (right + left) / 2;//������ ����� ����� - �������� �������
    //������� ������ ��� XRZ
    rz.z = func(left);
    rz.R = 0;
    XRZ.insert(pair<double, RZ>(left, rz));

    rz.z = func(newX);
    rz.R = 0;
    XRZ.insert(pair<double, RZ>(newX, rz));

    rz.z = func(right);
    rz.R = 0;
    XRZ.insert(pair<double, RZ>(right, rz));

    //����� ���� � ����� ����
    double rightM = Calculate_M(right, newX, XRZ.at(right).z, XRZ.at(newX).z);

    maxM = Calculate_M(newX, left, XRZ.at(newX).z, XRZ.at(left).z);;
    if (maxM < rightM)
    {
        maxM = rightM;
    }
 
    double m; //������ ��������� �������
    double maxR;//������������� ��������� ���� R

    map <double, RZ>::iterator num;//�������� ��� ��������� � ���� R
    map <double, RZ>::iterator backnum;//�������� �� ��� ����� �������

    double itcurE;//�������� �� ������� ���������

    //------------------------------------------------�������� ���� ���������
    while (true)
    {
        maxR = XRZ.at(newX).R;
        //�������� �������������
        m = Calculate_m();//����� m
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