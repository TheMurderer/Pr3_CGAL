#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    l = new Lienzo();
    setCentralWidget(l);

    //Construimos Delaunay
    dt = new Delaunay();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSalir_triggered()
{
    exit(0);
}

void MainWindow::on_actionInsertar_puntos_toggled(bool arg1)
{
    if(arg1){
        l->setBanderaPuntosTrue();
        statusBar()->showMessage(trUtf8("Inserción de puntos: ACTIVADA"),5000);
    }else{
        l->setBanderaPuntosFalse();
        statusBar()->showMessage(trUtf8("Inserción de puntos: DESACTIVADA"),5000);
    }
}

void MainWindow::on_actionBorra_pantalla_triggered()
{
    l->setBanderaTriangulacionFalse();
    l->puntos.clear();
    l->ejes.clear();

    //Quitamos los puntos de Dalunay
    dt = NULL;
    dt = new Delaunay();
    l->update();
}

void MainWindow::on_actionTriangular_puntos_triggered(){

    if(l->getBanderaTriangulacion() == false){
        //Poner mensaje de error: menos de tres puntos
        statusBar()->showMessage(trUtf8("Error: Triangulación no realizada. Menos de 3 puntos"),5000);
    }else{
        //Hacemos la triangulación
        vector<Point2D> vPoint2D;

        for(int i = 0; i < l->puntos.size();i++){
            Point2D p = Point2D(l->puntos[i].x(),l->puntos[i].y());
            vPoint2D.push_back(p);
        }

        vector<Point2D>::iterator begin = vPoint2D.begin();
        vector<Point2D>::iterator end = vPoint2D.end();
        dt->insertarPuntos(begin,end);

        //Realizamos la iteración de los ejes para obtener la triangulación

        list < pair<Point2D,Point2D> > listaEjes;
        listaEjes = dt->iteracionEjes();

        list < pair<Point2D,Point2D> >::iterator ejesBegin = listaEjes.begin();
        list < pair<Point2D,Point2D> >::iterator ejesEnd = listaEjes.end();

        for(;ejesBegin != ejesEnd;ejesBegin++){
            QPoint p1(ejesBegin->first.x(),ejesBegin->first.y());
            QPoint p2(ejesBegin->second.x(),ejesBegin->second.y());
            pair<QPoint,QPoint> par(p1,p2);
            l->ejes.push_back(par);
        }

        l->update();
        statusBar()->showMessage(trUtf8("Triangulación realizada"),5000);
    }
}
