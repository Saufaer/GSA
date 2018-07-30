#include "Optim.h"
using namespace std;
Pointer global::Mapa()
{
    RZ rz;
    Pointer point;//��������� - ���������
    point.steps = 1;//����


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

    //������� ������ ���� ��
    map <double, RZ>::iterator it = ++(XRZ.begin());
    MX.insert(pair<double, double>(Calculate_M(newX, left, XRZ.at(newX).z, XRZ.at(left).z), it->first));
    ++it;
    MX.insert(pair<double, double>(Calculate_M(right, newX, XRZ.at(right).z, XRZ.at(newX).z), it->first));


    double m; //������ ��������� �������
    double maxR;//������������� ��������� ���� R
    map <double, RZ>::iterator num;
    map <double, RZ>::iterator backnum;
    currentE = 1; //������� �������� �������


    while (true)
    {
        maxR = XRZ.at(left).R;
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
            if (it->first - back->first < currentE)
            {
                currentE = abs(it->first - back->first);//������� ��������
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
    point.z = func(newX);
    return point;


}