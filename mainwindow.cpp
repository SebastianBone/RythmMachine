#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "mouseevent.h"
using namespace cv;
using namespace std;

mouseevent *me;

bool capturing;
bool boxIsChecked = false;
Mat cameraFrame;
int mouseIsPressedCounter =0;

Scalar blau = Scalar(0, 0, 255);
Scalar yellow = Scalar(255, 255, 0);
Scalar red = Scalar (255,0,0);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::run(){
    capturing = true;
    qDebug() << "box is checked? :" <<boxIsChecked<<endl;
    VideoCapture capture(0);

    connect(ui->cameraImageLabel, SIGNAL(mouse_pos()), this, SLOT(mouse_current_pos()));
    connect(ui->cameraImageLabel, SIGNAL(mouse_left()), this, SLOT(mouse_left()));
    connect(ui->cameraImageLabel, SIGNAL(mouse_pressed()), this, SLOT(mouse_press()));
    connect(ui->cameraImageLabel, SIGNAL(mouse_released()), this, SLOT(mouse_rel()));


    while(capturing){


        capture.read(cameraFrame);


        drawRectangle(620,20, blau);
        drawRectangle(620,90, yellow);
        drawRectangle(620,160, red);


        QImage img = QImage((const unsigned char*)
                            (cameraFrame.data),
                            cameraFrame.cols,
                            cameraFrame.rows,
                            QImage::Format_RGB888);

        if(boxIsChecked == true){
            rectangle(cameraFrame, Point(620-70, 20), Point(620-70+50, 20+50), blau, CV_FILLED);
        }

        ui->cameraImageLabel->setPixmap(QPixmap::fromImage(img.mirrored(true,false)));
        ui->cameraImageLabel->resize(ui->cameraImageLabel->pixmap()->size());

        waitKey(50);

    }
    capture.release();
}

void MainWindow::drawRectangle(int x, int y, const Scalar &scalar){
    for(int i =1 ; i<9; i++){
        rectangle(cameraFrame, Point(x-i*70, y), Point(x-70*i+50, y+50), scalar,2,2);
    }
}


void MainWindow::on_pushButton_clicked()
{
    run();
}

void MainWindow::on_pushButton_2_clicked()
{
    capturing = false;
    exit(0);


}
void MainWindow::mouse_current_pos()
{
    cout << "X:" << ui->cameraImageLabel->x <<endl;
    cout << "Y:" << ui->cameraImageLabel->y <<endl;


}
void MainWindow::mouse_press()
{
    int mousePosX = ui->cameraImageLabel->x;
    int mousePosY = ui->cameraImageLabel->y;

    if(me->isPressed() == true){

        qDebug() << "counter? :" <<mouseIsPressedCounter<<endl;
    }

    if((mousePosX > 40) && (mousePosX< 90) && (mousePosY > 20) && (mousePosY < 70)){
        if (me->isPressed() == true){
            mouseIsPressedCounter +=1;
            if (mouseIsPressedCounter %2 != 0)
                boxIsChecked = true;

        }
    }else
        boxIsChecked = false;

}
void MainWindow::mouse_left()
{

}

void MainWindow::mouse_rel()
{

}

void MainWindow::on_pushButton_3_clicked(bool checked)
{

}
