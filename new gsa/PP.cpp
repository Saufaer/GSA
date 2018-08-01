#include "Optim.h"
using namespace std;
global::Pointer global::PP()
{
    RZ rz;//���������
    Pointer point;//��������� - ���������
    point.steps = 0;//����
    currentE = right - left; //������� �������� �������

    struct resp
    {
        double R,x;
    };
    int procs = 5;        
    struct bord
    {
        double shiftl, shiftr;
        double R, x;
        map <double, RZ>::iterator num;
        map <double, RZ>::iterator backnum;
    };
    //--------------------------------------------������ �������� ���������
    vector <bord> bb(procs);
    for (int i = 0; i < procs; i++)
    {
        bb[i].shiftl = left + i * (right - left) / procs;
        bb[i].shiftr = bb[i].shiftl + (right - left) / procs;

    }


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


    omp_set_dynamic(0);//����� ���������� �� ����� ����������� ��������� ���������� �������
    omp_set_num_threads(procs);//��������� ����� ������ �������

                  //------------------------------------------------�������� ���� ���������
    while (true)
    {
        
        maxR = (++(XRZ.begin()))->second.R;
        
        //�������� �������������
        m = Calculate_m();//����� m


        
        num = ++(XRZ.begin());
        backnum = XRZ.begin();

        if (XRZ.size() >= procs+1)
        {
            #pragma omp parallel for num_threads(procs)
            for (int i = 0; i < procs; i++)
            {
                bb[i].R = (++(XRZ.find(bb[i].shiftl)))->second.R;
                bb[i].num = (++(XRZ.find(bb[i].shiftl)));
                bb[i].backnum = ((XRZ.find(bb[i].shiftl)));
                //for (map <double, RZ>::iterator it = ++(XRZ.begin()), back = XRZ.begin() // ����������� ��������� � ������������ R
                //    ; it != XRZ.end();
                //    ++back, ++it
                //    )
                map <double, RZ>::iterator it = ++(XRZ.find(bb[i].shiftl)), back = XRZ.find(bb[i].shiftl);
                while (true)
                {
                    if (i == procs - 1)
                    {
                        if (it == XRZ.end() )
                        { break; }
                    }
                    else
                    {
                        if (it == ++XRZ.find(bb[i + 1].shiftr)) { break; }
                    }
                    (it)->second.R = Calculate_R(m, it, back);//������� R � ������� ���������

                    if (XRZ.at(it->first).R > bb[i].R)
                    {
                        //////////////////////////////
                        bb[i].R = XRZ.at(it->first).R;
                        bb[i].num = it;
                        bb[i].backnum = back;
                        /////////////////////////////////
                    }

                    itcurE = it->first - back->first;//�������� �� ������� ��������
                    if (itcurE < currentE)
                    {
                        currentE = itcurE;//�������� �������
                    }
                    ++it; ++back;
                }
            }

            if (currentE <= E)//����� �� ��������
            {
                break;
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
        else
        {
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
        }
    
        newX = Calculate_X(m, num, backnum);//������ ��������� ����� ��������� � ��������� ���������


                                            //����c x � z � ����
        rz.R = 0;
        rz.z = func(newX);
        XRZ.insert(pair<double, RZ>(newX, rz));

        if (XRZ.size() == procs+1)
        {
            int i = 0;
                for (map <double, RZ>::iterator it = ++(XRZ.begin()), back = XRZ.begin()
                    ; it != XRZ.end();
                    ++back, ++it
                    )
                {
                    bb[i].shiftl = back->first;
                    bb[i].shiftr= it->first;
                    i++;
                }
            
        }
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