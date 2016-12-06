#include "mainwindow.h"
#include <QApplication>
#include "qcustomplot.h"
#include "math.h"

#define _stepen 8
#define _nachalo 0
#define _konec 8

#define _tochki_polinom 1000 //сколько точек требуем от полинома

double L(double x, QVector<double> tochki_x, QVector<double> tochki_y) {
    double y = 0;
    for ( int i = 0; i < _stepen; i++ ) {
        double li_chislitel = 1.0;
        for ( int chislitel_i = 0; chislitel_i < _stepen; chislitel_i++ ) {
            if ( i != chislitel_i ) {
                li_chislitel *= x - tochki_x[chislitel_i];
            }
        }
        double li_znamenatel = 1.0;
        for ( int znamenatel_i = 0; znamenatel_i < _stepen; znamenatel_i++ ) {
            if ( i != znamenatel_i ) {
                li_znamenatel *= tochki_x[i] - tochki_x[znamenatel_i];
            }
        }
        y += (li_chislitel / li_znamenatel) * tochki_y[i];
    }
    return y;
}

double func(double x) {
    return sin(x);
}

double get_delta(int nomer_chasti, int kolichestvo_chastey) {
    return ((double) _nachalo) +
            ((((double) nomer_chasti) *
              ((double) abs(_konec - _nachalo))) /
              ((double) kolichestvo_chastey - 1));
}

double poly(double x) {
    return (-0.90929 * (x * (x-8.0) * (x-4.0) * (x-6.0)) / 96.0) +
          (-0.756802 * (x * (x-2.0) * (x-6.0) * (x-8.0)) / 64.0) +
           (0.279415 * (x * (x-2.0) * (x-4.0) * (x-8.0)) / 96.0) +
           (0.989358 * (x * (x-2.0) * (x-6.0) * (x-4.0)) / 384.0);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow window;
    QCustomPlot customPlot;
    window.setCentralWidget(&customPlot);







    // строим синий график исходной функции
    QVector<double> x_orig(10000), y_orig(10000);
    for ( int i = 0; i < 10000; i++ ) {
        x_orig[i] = get_delta(i, 10000);
        y_orig[i] = func(x_orig[i]);
    }
    customPlot.addGraph();
    customPlot.graph(0) -> setData(x_orig, y_orig);









    // выбираем точки, по которым будем строить полином
    // отмечаем красным
    QVector<double> x(_stepen), y(_stepen);
    for (int i = 0; i < _stepen; i++ ) {
        x[i] = get_delta(i, _stepen);
        y[i] = func(x[i]);
    }
    customPlot.addGraph();
    customPlot.graph(1) -> setPen(QPen(Qt::transparent));
    //customPlot.graph(1) -> setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::red, Qt::white, 7));
    customPlot.graph(1) -> setData(x, y);








    // строим график полинома
    // цвет: зеленый
    QVector<double> x_polinom(_tochki_polinom), y_polinom(_tochki_polinom);
    for (int i = 0; i < _tochki_polinom; i++ ) {
        x_polinom[i] = get_delta(i, _tochki_polinom);
        y_polinom[i] = L(x_polinom[i], x, y);
    }
    customPlot.addGraph();
    customPlot.graph(2) -> setPen(QPen(Qt::green));
    customPlot.graph(2) -> setData(x_polinom, y_polinom);



    {
        QVector<double> x_polinom(100), y_polinom(100);

        for (int i = 0; i < 100; i++ ) {
            x_polinom[i] = get_delta(i, 100);
            y_polinom[i] = poly(x_polinom[i]);
        }
//        x_polinom[0] = 0.0;
//        x_polinom[1] = 2.0;
//        x_polinom[2] = 4.0;
//        x_polinom[3] = 6.0;
//        x_polinom[4] = 8.0;

//        y_polinom[0] = 0.0;
//        y_polinom[1] = 0.90929;
//        y_polinom[2] = -0.756802;
//        y_polinom[3] = -0.279415;
//        y_polinom[4] = 0.989358;


        customPlot.addGraph();
        customPlot.graph(2) -> setAntialiased(true);
        customPlot.graph(2) -> setPen(QPen(Qt::black));
        customPlot.graph(2) -> setData(x_polinom, y_polinom);
    }







    customPlot.xAxis -> setLabel("X");
    customPlot.yAxis -> setLabel("Y");
    customPlot.rescaleAxes();
    window.setGeometry(100, 100, 1000, 600);
    window.show();
    return a.exec();
}
