#include "Optim.h"
using namespace std;
global::Pointer global::PP()
{
    RZ rz;//контейнер
    Pointer point;//структура - результат
    point.steps = 0;//шаги
    currentE = right - left; //текущая точность решения
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
    double maxR = Calculate_R(m, num, backnum);//инициализация временной макс R

    double itcurE;//точность на текущем интервале


                  //#pragma omp critical
                  //    { double st = omp_get_wtime();
                  //    double fn = omp_get_wtime();
                  //    double time = fn - st;
                  //    alltime += time;
                  //    }


                  //------------------------------------------------основной цикл алгоритма

    vector <bord> bb(procs);
    while (true)
    {
        maxR = (++(XRZ.begin()))->second.R;
        m = Calculate_m();//поиск m       
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
        else//если недостаточно интервалов или procs==1
        {
            double xr;
            double xl;
            double zr;
            double zl;
            double R;
            for (map <double, RZ>::iterator it = ++(XRZ.begin()), back = XRZ.begin() // Определение интервала с максимальным R
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
                    num = it;//запоминаю номер интервала (индекс его правой границы)
                    backnum = back;

                }
            }


        }
        newX = Calculate_X(m, num, backnum);//нахожу следующую точку испытания в найденном интервале
                                            //заноc x и z в базу
        rz.R = 0;
        rz.z = func(newX);
        XRZ.insert(pair<double, RZ>(newX, rz));

        currentE = newX - (--XRZ.find(newX))->first;
        if (currentE <= E)//выход по точности
        {
            break;
        }
        ////////////////////////////////////////////////////стартовое распределение

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

        //схема с весом

        if (XRZ.size() > procs + 1)
        {
            for (int i = 0; i < procs; i++)
            {
                if ((newX < bb[i].shiftr) && (newX > bb[i].shiftl))//определение где новый х
                {
                    bb[i].weight++;

                }

                if ((i != procs - 1) && (bb[i].weight != 0))//узел i влево
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

        if (point.steps == Nmax)//выход по числу шагов
        {
            break;
        }
    }

    point.x = newX;
    point.z = rz.z;
    return point;


}