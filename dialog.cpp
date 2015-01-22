#include "dialog.h"
#include "ui_dialog.h"
#include <QtCore>
#include <SFML/Audio.hpp>
#include <QtCore>
#include <QDebug>
#include <QtCore>

using namespace std;
using namespace cv;

//////////////////////////////////////////////////////
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    capWebCam.open(0);

    if(!capWebCam.isOpened()){
        ui->txtConsole->appendPlainText("***error***: check cammera settings");
        return;
    }

    timerStart();

    pairUiButtonstoArray();

    initSlider();

    eventInstaller();

}
///////////////////////////////////////////////////////
Dialog::~Dialog()
{
    delete ui;
}

///////////////////////////////////////////////////////
void Dialog::processFrameAndUpdateGUI(){

    capWebCam.read(matOriginal);

    matSrc = matOriginal.clone();
    cvtColor(matSrc, matSrc, CV_BGR2RGB);

    if(matOriginal.empty() == true) return;

    inRange(matOriginal, Scalar(bL,gL,rL), Scalar(bH,gH,rH), matProcessed); //over qSlider  (50,30,130)
    GaussianBlur(matProcessed, matProcessed, Size(9,9), 1.5);
    HoughCircles(matProcessed, vecCircles, CV_HOUGH_GRADIENT, 2, matProcessed.rows / 4,100,50,10,400);

    for(itrCircles = vecCircles.begin(); itrCircles != vecCircles.end(); itrCircles++){

        //show position and radius of tracked object (just for debuging)
        ui->txtConsole->appendPlainText(QString("object position x =") + QString::number((*itrCircles)[0]).rightJustified(4, ' ')+
                QString(", y =")+ QString::number((*itrCircles)[1]).rightJustified(4, ' ')+
                QString(", radius =")+ QString::number((*itrCircles)[2], 'f' , 3).rightJustified(7, ' '));

        //visualisation (circle) of tracked object
        circle(matOriginal, Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), 3 ,Scalar(0,255,0), CV_FILLED);
        circle(matOriginal, Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]),(int)(*itrCircles)[2] ,Scalar(0,0,255),3);

        //setting tracking point as a mouse cursor
        cursor->setPos(mapToGlobal(QPoint(640-(*itrCircles)[0], (*itrCircles)[1])));

        if (((*itrCircles)[0]) > 630 && !audioTimer->isActive()){
            audioTimer->start();
        }
        if (((*itrCircles)[0]) < 5 && audioTimer->isActive()){
            audioTimer->stop();
        }
    }


    cvtColor(matOriginal, matOriginal, CV_BGR2HSV);  //Convert 2 HSV


    matOriginal = rescale(matOriginal);
    matProcessed = rescale(matProcessed);
    //dilate(matProcessed, matProcessed, Mat());
    //erode(matProcessed, matProcessed, Mat());

    QImage imgOriginal((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    QImage imgProcessed((uchar*)matProcessed.data, matProcessed.cols, matProcessed.rows, matProcessed.step, QImage::Format_Indexed8);
    QImage imgSrc((uchar*)matSrc.data, matSrc.cols, matSrc.rows, matSrc.step, QImage::Format_RGB888);


    //set mirroring
    ui->lblHSV->setPixmap(QPixmap::fromImage(imgOriginal.mirrored(true,false)));
    ui->lblProcessed->setPixmap(QPixmap::fromImage(imgProcessed.mirrored(true,false)));
    ui->lblSrc->setPixmap(QPixmap::fromImage(imgSrc.mirrored(true,false)));


}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Dialog::on_btnPauseResume_clicked()
{
    if(audioTimer->isActive() == true){
        audioTimer->stop();
        ui->btnPauseResume->setText("Resume");
    } else{
        audioTimer->start();
        ui->btnPauseResume->setText("Pause");
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
void Dialog::processAudio(){

    if(beatButtons[index]->isChecked()){
        beatAudio.playSound("/home/marko/Desktop/objectTrackingCV/kick.wav");
    }
    if(snareButtons[index]->isChecked()){
        snareAudio.playSound("/home/marko/Desktop/objectTrackingCV/snare.wav");
    }
    if(hitButtons[index]->isChecked()){
        hitAudio.playSound("/home/marko/Desktop/objectTrackingCV/hit.wav");
    }
    if(drumButtons[index]->isChecked()){
        drumAudio.playSound("/home/marko/Desktop/objectTrackingCV/BD.wav");
    }
    index = (index + 1) % 8;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////
Mat Dialog::rescale(Mat &mat){
    // scale factor
    float labelWidth = ui->lblHSV->width();
    float labelHeight = ui->lblHSV->height();
    float imageWidth = mat.cols;
    float imageHeight = mat.rows;
    float scale = labelWidth / imageWidth;
    if (labelHeight/imageHeight < scale){
        scale = labelHeight / imageHeight;
    }

    // compute new image ratio
    float scaledImageWidth = imageWidth * scale;
    float scaledImageHeight = imageHeight * scale;

    // scaled Mat-Object
    Mat scaledImage (scaledImageHeight, scaledImageWidth, mat.type());
    cv::resize(mat, scaledImage, Size(scaledImageWidth, scaledImageHeight));
    return scaledImage;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
void Dialog::on_blueLow_valueChanged(int value)
{
    bL = value;
}

void Dialog::on_greenLow_valueChanged(int value)
{
    gL = value;
}

void Dialog::on_redLow_valueChanged(int value)
{
    rL = value;
}

void Dialog::on_blueHigh_valueChanged(int value)
{
    bH = value;
}

void Dialog::on_greenHigh_valueChanged(int value)
{
    gH = value;
}

void Dialog::on_redHigh_valueChanged(int value)
{
    rH = value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void Dialog::pairUiButtonstoArray(){

    index = 0;

    beatButtons[0] = ui->beat_1;
    beatButtons[1] = ui->beat_2;
    beatButtons[2] = ui->beat_3;
    beatButtons[3] = ui->beat_4;
    beatButtons[4] = ui->beat_5;
    beatButtons[5] = ui->beat_6;
    beatButtons[6] = ui->beat_7;
    beatButtons[7] = ui->beat_8;

    snareButtons[0] = ui->clap_1;
    snareButtons[1] = ui->clap_2;
    snareButtons[2] = ui->clap_3;
    snareButtons[3] = ui->clap_4;
    snareButtons[4] = ui->clap_5;
    snareButtons[5] = ui->clap_6;
    snareButtons[6] = ui->clap_7;
    snareButtons[7] = ui->clap_8;

    hitButtons[0] = ui->hit_1;
    hitButtons[1] = ui->hit_2;
    hitButtons[2] = ui->hit_3;
    hitButtons[3] = ui->hit_4;
    hitButtons[4] = ui->hit_5;
    hitButtons[5] = ui->hit_6;
    hitButtons[6] = ui->hit_7;
    hitButtons[7] = ui->hit_8;

    drumButtons[0] = ui->drum_1;
    drumButtons[1] = ui->drum_2;
    drumButtons[2] = ui->drum_3;
    drumButtons[3] = ui->drum_4;
    drumButtons[4] = ui->drum_5;
    drumButtons[5] = ui->drum_6;
    drumButtons[6] = ui->drum_7;
    drumButtons[7] = ui->drum_8;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void Dialog::timerStart(){


    tmrTimer = new QTimer(this);
    tmrTimer->setInterval(100);
    tmrTimer->setTimerType(Qt::PreciseTimer);
    connect(tmrTimer, SIGNAL(timeout()), this, SLOT(processFrameAndUpdateGUI()));
    tmrTimer->start();

    audioTimer = new QTimer(this);
    audioTimer->setInterval(350);
    audioTimer->setTimerType(Qt::PreciseTimer);
    connect(audioTimer, SIGNAL(timeout()), this, SLOT(processAudio()));
    audioTimer->start();

    beatButtonsDelayTimer = new QTimer(this);
    beatButtonsDelayTimer->setInterval(1000);
    beatButtonsDelayTimer->setTimerType(Qt::PreciseTimer);
    connect(beatButtonsDelayTimer, SIGNAL(timeout()), this, SLOT(beatButtonsDelay()));

    snareButtonsDelayTimer = new QTimer(this);
    snareButtonsDelayTimer->setInterval(1000);
    snareButtonsDelayTimer->setTimerType(Qt::PreciseTimer);
    connect(snareButtonsDelayTimer, SIGNAL(timeout()), this, SLOT(snareButtonsDelay()));

    hitButtonsDelayTimer = new QTimer(this);
    hitButtonsDelayTimer->setInterval(1000);
    hitButtonsDelayTimer->setTimerType(Qt::PreciseTimer);
    connect(hitButtonsDelayTimer, SIGNAL(timeout()), this, SLOT(hitButtonsDelay()));

    drumButtonsDelayTimer = new QTimer(this);
    drumButtonsDelayTimer->setInterval(1000);
    drumButtonsDelayTimer->setTimerType(Qt::PreciseTimer);
    connect(drumButtonsDelayTimer, SIGNAL(timeout()), this, SLOT(drumButtonsDelay()));

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void Dialog::initSlider(){

    ui->blueLow->setRange(0,255);
    ui->greenLow->setRange(0,255);
    ui->redLow->setRange(0,255);
    ui->blueHigh->setRange(0,255);
    ui->greenHigh->setRange(0,255);
    ui->redHigh->setRange(0,255);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////
bool Dialog::eventFilter(QObject *button, QEvent *event){

    for(btnIndex=0;btnIndex<=7;btnIndex++){

        if(button == beatButtons[btnIndex]){
            if(event->type() == QEvent::Enter){
                indexTransfer();
                beatButtonsDelayTimer->start();
            }
            if(event->type() == QEvent::Leave)
                beatButtonsDelayTimer->stop();
        }

        else if(button == snareButtons[btnIndex]){
            if(event->type() == QEvent::Enter){
                indexTransfer();
                snareButtonsDelayTimer->start();
            }
            if(event->type() == QEvent::Leave)
                snareButtonsDelayTimer->stop();

        }
        else if(button == hitButtons[btnIndex]){
            if(event->type() == QEvent::Enter){
                indexTransfer();
                hitButtonsDelayTimer->start();
            }
            if(event->type() == QEvent::Leave)
                hitButtonsDelayTimer->stop();

        }
        else if(button == drumButtons[btnIndex]){
            if(event->type() == QEvent::Enter){
                indexTransfer();
                drumButtonsDelayTimer->start();
            }
            if(event->type() == QEvent::Leave)
                drumButtonsDelayTimer->stop();
        }

    }
    return QDialog::eventFilter(button,event);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void Dialog::eventInstaller(){

    ui->beat_1->installEventFilter(this);
    ui->beat_2->installEventFilter(this);
    ui->beat_3->installEventFilter(this);
    ui->beat_4->installEventFilter(this);
    ui->beat_5->installEventFilter(this);
    ui->beat_6->installEventFilter(this);
    ui->beat_7->installEventFilter(this);
    ui->beat_8->installEventFilter(this);

    ui->clap_1->installEventFilter(this);
    ui->clap_2->installEventFilter(this);
    ui->clap_3->installEventFilter(this);
    ui->clap_4->installEventFilter(this);
    ui->clap_5->installEventFilter(this);
    ui->clap_6->installEventFilter(this);
    ui->clap_7->installEventFilter(this);
    ui->clap_8->installEventFilter(this);

    ui->hit_1->installEventFilter(this);
    ui->hit_2->installEventFilter(this);
    ui->hit_3->installEventFilter(this);
    ui->hit_4->installEventFilter(this);
    ui->hit_5->installEventFilter(this);
    ui->hit_6->installEventFilter(this);
    ui->hit_7->installEventFilter(this);
    ui->hit_8->installEventFilter(this);

    ui->drum_1->installEventFilter(this);
    ui->drum_2->installEventFilter(this);
    ui->drum_3->installEventFilter(this);
    ui->drum_4->installEventFilter(this);
    ui->drum_5->installEventFilter(this);
    ui->drum_6->installEventFilter(this);
    ui->drum_7->installEventFilter(this);
    ui->drum_8->installEventFilter(this);
}

void Dialog::beatButtonsDelay(){

    if(beatButtons[idx]->isChecked()==false)
        beatButtons[idx]->setChecked(true);
    else
        beatButtons[idx]->setChecked(false);

    beatButtonsDelayTimer->stop();
}

void Dialog::snareButtonsDelay(){
    if(snareButtons[idx]->isChecked()==false)
        snareButtons[idx]->setChecked(true);
    else
        snareButtons[idx]->setChecked(false);

    snareButtonsDelayTimer->stop();

}

void Dialog::hitButtonsDelay(){
    if(hitButtons[idx]->isChecked()==false)
        hitButtons[idx]->setChecked(true);
    else
        hitButtons[idx]->setChecked(false);

    hitButtonsDelayTimer->stop();
}

void Dialog::drumButtonsDelay(){
    if(drumButtons[idx]->isChecked()==false)
        drumButtons[idx]->setChecked(true);
    else
        drumButtons[idx]->setChecked(false);

    drumButtonsDelayTimer->stop();

}

void Dialog::setIndex(int Index){
    btnIndex = Index;
}

int Dialog::getIndex(){
    return btnIndex;
}

void Dialog::indexTransfer(){
    idx = getIndex();
    setIndex(idx);
}
