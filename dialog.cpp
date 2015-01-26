// include files and libraries

#include "dialog.h"
#include "ui_dialog.h"
#include <QtCore>
#include <SFML/Audio.hpp>
#include <QDebug>

// initialize namespaces

using namespace std;
using namespace cv;

//////////////////////////////////////////////////////

// initialize widget and user interface

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    initialize();

}
///////////////////////////////////////////////////////

// destroy widget

Dialog::~Dialog()
{
    delete ui;
}

///////////////////////////////////////////////////////
void Dialog::processFrameAndUpdateGUI(){


    // read video in opencv mat-container
    capWebCam.read(matOriginal);

    // clone container and color-mirroring
    matSrc = matOriginal.clone();
    cvtColor(matSrc, matSrc, CV_BGR2RGB);
    //from source to HSV
    cvtColor(matSrc, matHSV, CV_RGB2HSV);

    // return if container is empty
    if(matOriginal.empty() == true) return;

    // range of color-filter
    //inRange(matOriginal, Scalar(bL,gL,rL), Scalar(bH,gH,rH), matProcessed); //over qSlider  (50,30,130)
    inRange(matHSV, Scalar(lH,lS,lV), Scalar(hH,hS,hV), matProcessed);

    // smoothing pixels
    GaussianBlur(matProcessed, matProcessed, Size(9,9), 1.5);

    // filtered object
    HoughCircles(matProcessed, vecCircles, CV_HOUGH_GRADIENT, 2, matProcessed.rows / 4,100,50,10,400);

    //remove small objects from the foreground
    erode(matProcessed, matProcessed, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
    dilate(matProcessed, matProcessed, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));

    //fill small holes in the foreground
    dilate(matProcessed, matProcessed, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));
    erode(matProcessed, matProcessed, getStructuringElement(MORPH_ELLIPSE, Size(5,5)));


    for(itrCircles = vecCircles.begin(); itrCircles != vecCircles.end(); itrCircles++){

        //setting tracking point as a mouse cursor
        if(cursorTracking == true){



            //show position and radius of tracked object (just for debuging)
            ui->txtConsole->appendPlainText(QString("object position x =") + QString::number((*itrCircles)[0]).rightJustified(4, ' ')+
                    QString(", y =")+ QString::number((*itrCircles)[1]).rightJustified(4, ' ')+
                    QString(", radius =")+ QString::number((*itrCircles)[2], 'f' , 3).rightJustified(7, ' '));

            //visualisation (circle) of tracked object
            circle(matHSV, Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]), 3 ,Scalar(0,255,0), CV_FILLED);
            circle(matHSV, Point((int)(*itrCircles)[0], (int)(*itrCircles)[1]),(int)(*itrCircles)[2] ,Scalar(0,0,255),3);

            cursor->setPos(mapToGlobal(QPoint(640-(*itrCircles)[0] +10, (*itrCircles)[1] +10)));

            check4Swipe();
        }
    }

    // scaling container
    matOriginal = rescale(matOriginal);
    matHSV = rescale(matHSV);
    matProcessed = rescale(matProcessed);

    // convert opencv in QtImage
    //QImage imgHSV((uchar*)matOriginal.data, matOriginal.cols, matOriginal.rows, matOriginal.step, QImage::Format_RGB888);
    QImage imgHSV((uchar*)matHSV.data, matHSV.cols, matHSV.rows, matHSV.step, QImage::Format_RGB888);
    QImage imgProcessed((uchar*)matProcessed.data, matProcessed.cols, matProcessed.rows, matProcessed.step, QImage::Format_Indexed8);
    QImage imgSrc((uchar*)matSrc.data, matSrc.cols, matSrc.rows, matSrc.step, QImage::Format_RGB888);


    //set mirroring
    ui->lblHSV->setPixmap(QPixmap::fromImage(imgHSV.mirrored(true,false)));
    ui->lblProcessed->setPixmap(QPixmap::fromImage(imgProcessed.mirrored(true,false)));
    ui->lblSrc->setPixmap(QPixmap::fromImage(imgSrc.mirrored(true,false)));

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// audio pause button
void Dialog::on_btnPauseResume_clicked()
{
    if(audioTimer->isActive() == true){
        audioTimer->stop();
        ui->btnPauseResume->setText("Resume Audio");
    } else{
        audioTimer->start();
        ui->btnPauseResume->setText("Pause Audio");
    }
}

//capturing pause button
void Dialog::on_btnPRCapt_clicked()
{
    if(tmrTimer->isActive() == true){
        tmrTimer->stop();
        ui->btnPRCapt->setText("Resume Capturing");
    } else{
        tmrTimer->start();
        ui->btnPRCapt->setText("Pause Capturing");
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////

// see which buttons are checked and play soundfile
void Dialog::processAudio(){

    if(beatButtons[index]->isChecked()){
        beatAudio.playSound("audio/KC.wav");
    }
    if(snareButtons[index]->isChecked()){
        snareAudio.playSound("audio/snare.wav");
    }
    if(hitButtons[index]->isChecked()){
        hitAudio.playSound("audio/hit.wav");
    }
    if(drumButtons[index]->isChecked()){
        drumAudio.playSound("audio/BD.wav");
    }
    // index for soundinterval
    index = (index + 1) % 8;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////
// Button for start / stop cursor tracking
void Dialog::on_btnCursorTr_clicked()
{
    if(ui->btnCursorTr->isChecked() == false){
        cursorTracking = false;
        ui->btnCursorTr->setText("Start Cursor");
    }
    else{
        cursorTracking = true;
        ui->btnCursorTr->setText("Stop Cursor");
    }
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

// set values when slider changed

void Dialog::on_blueLow_valueChanged(int value)
{
    lH = value;
}

void Dialog::on_greenLow_valueChanged(int value)
{
    lS = value;
}

void Dialog::on_redLow_valueChanged(int value)
{
    lV = value;
}

void Dialog::on_blueHigh_valueChanged(int value)
{
    hH = value;
}

void Dialog::on_greenHigh_valueChanged(int value)
{
    hS = value;
}

void Dialog::on_redHigh_valueChanged(int value)
{
    hV = value;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

// save buttonlabels in lane-arrays

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

// create and configure video and audio timer
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

}

// initialize methods, capturing source and user interface

void Dialog::initialize()
{
    ui->setupUi(this);

    capWebCam.open(0);

    if(!capWebCam.isOpened()){
        ui->txtConsole->appendPlainText("***error***: check cammera settings");
        return;
    }

    timerStart();

    pairUiButtonstoArray();

    hsvTreshholdsInit();

    cursorTracking = false;
}

void Dialog::hsvTreshholdsInit()
{
    hH = 179;
    hS = 255;
    hV = 255;

    lH = 0;
    lS = 0;
    lV = 0;
}

void Dialog::check4Swipe()
{
    //swipe left for play
    if(ui->play->isEntered() == true && !audioTimer->isActive()){
        audioTimer->start();
        ui->btnPauseResume->setText("Pause Audio");
    }


    //swipe right for pause
    if ((ui->stop->isEntered()) == true && audioTimer->isActive()){
        audioTimer->stop();
        ui->btnPauseResume->setText("Resume Audio");
    }

}
