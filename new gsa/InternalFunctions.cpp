#include "Optim.h"

double global::func(const double x)
{
    //return sin(x) + sin((10. * x) / 3.);
    //return (3 * x - 14)*sin(1.8 * x);
    //return -x + sin(3 * x) - 1;
    return coeff[0] * sin(coeff[1] * x) + coeff[2] * cos(coeff[3] * x);
};

//������� M �� �������� ���������
double global::Calculate_M(double xr, double xl, double zr, double zl)
{
    return abs(zr - zl) / (xr - xl);
};

//  ������ ��������� �������
double global::Calculate_m()
{
    std::map <double, RZ>::iterator itNewX = XRZ.find(newX);
    std::map <double, RZ>::iterator itleft = --XRZ.find(newX);
    std::map <double, RZ>::iterator itright = ++XRZ.find(newX);

    newM = Calculate_M(itNewX->first, itleft->first, itNewX->second.z, itleft->second.z);//������� ������ � ����� �� newX

    if (maxM < newM)//�������� ������������ �� ��
    {
        maxM = newM;
    }

    if (XRZ.size() > 2)
    {
        newM = Calculate_M(itright->first, itNewX->first, itright->second.z, itNewX->second.z);//������� ������ � ������ �� newX

        if (maxM < newM)//�������� ������������ �� ��
        {
            maxM = newM;
        }
    }

    //������� m
    if (maxM > 0)
    {
        return r * maxM;
    }
    else //M==0
    {
        return 1.0;
    }
};

//������� ����� ����� ���������
double global::Calculate_X(double m, std::map <double, RZ>::iterator num, std::map <double, RZ>::iterator backnum)
{
    xr = num->first;
    xl = backnum->first;

    zr = num->second.z;
    zl = backnum->second.z;

    return 0.5 * (xr + xl) - (zr - zl) / (2.0 * m);
};

//������� �������������� �� �������� ���������
double global::Calculate_R(double m, std::map <double, RZ>::iterator it, std::map <double, RZ>::iterator back)
{
    xr = it->first;
    xl = back->first;

    zr = it->second.z;
    zl = back->second.z;
    return m * (xr - xl) + (((zr - zl) *
        (zr - zl)) / (m * (xr - xl))) -
        2 * (zr + zl);
};