#include "Optim.h"
using namespace std;
global::Pointer global::PP()
{
    RZ rz;//контейнер
    Pointer point;//структура - результат
    point.steps = 0;//шаги
    currentE = right - left; //текущая точность решения

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
    //--------------------------------------------первая итерация алгоритма
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

    double m = Calculate_m(); //оценка константы Липшица

    map <double, RZ>::iterator num = ++(XRZ.begin());//итератор для интервала с макс R
    map <double, RZ>::iterator backnum = (XRZ.begin());//итератор на его левую границу
    double maxR = Calculate_R(m, num, backnum);;//инициализация временной макс R

    double itcurE;//точность на текущем интервале


    omp_set_dynamic(0);//среда выполнения не будет динамически настроить количество потоков
    omp_set_num_threads(procs);//установим число нужных потоков

                  //------------------------------------------------основной цикл алгоритма
    while (true)
    {
        
        maxR = (++(XRZ.begin()))->second.R;
        
        //пересчёт характеристик
        m = Calculate_m();//поиск m


        
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
                //for (map <double, RZ>::iterator it = ++(XRZ.begin()), back = XRZ.begin() // Определение интервала с максимальным R
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
                    (it)->second.R = Calculate_R(m, it, back);//подсчёт R в текущем интервале

                    if (XRZ.at(it->first).R > bb[i].R)
                    {
                        //////////////////////////////
                        bb[i].R = XRZ.at(it->first).R;
                        bb[i].num = it;
                        bb[i].backnum = back;
                        /////////////////////////////////
                    }

                    itcurE = it->first - back->first;//точность на текущем итервале
                    if (itcurE < currentE)
                    {
                        currentE = itcurE;//точность решения
                    }
                    ++it; ++back;
                }
            }

            if (currentE <= E)//выход по точности
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
            for (map <double, RZ>::iterator it = ++(XRZ.begin()), back = XRZ.begin() // Определение интервала с максимальным R
                ; it != XRZ.end();
                ++back, ++it
                )
            {
                (it)->second.R = Calculate_R(m, it, back);//подсчёт R в текущем интервале

                if (XRZ.at(it->first).R > maxR)
                {
                    maxR = XRZ.at(it->first).R;
                    num = it;//запоминаю номер интервала (индекс его правой границы)
                    backnum = back;

                }

                itcurE = it->first - back->first;//точность на текущем итервале
                if (itcurE < currentE)
                {
                    currentE = itcurE;//точность решения
                }
            }
            if (currentE <= E)//выход по точности
            {
                break;
            }
        }
    
        newX = Calculate_X(m, num, backnum);//нахожу следующую точку испытания в найденном интервале


                                            //заноc x и z в базу
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

        if (point.steps == Nmax)//выход по числу шагов
        {
            break;
        }
    }

    point.x = newX;
    point.z = rz.z;
    return point;


}