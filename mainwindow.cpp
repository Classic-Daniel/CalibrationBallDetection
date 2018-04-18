#include "mainwindow.h"
#include "ui_mainwindow.h"


using namespace cv;
using namespace std;

Mat rgbImage, labImage, currentMask;
Vec3b redSample, blueSample, greenSample;
Ui::MainWindow* gui;
int clickNum = 0;

Mat verticalGradientFilter;

void onMouse( int event, int x, int y, int, void* )
{

    if( event != CV_EVENT_LBUTTONDOWN )
            return;

    clickNum++;
    Point pt = Point(x,y); //using lab instead of bgr here
    float b = labImage.at<Vec3b>(pt.y,pt.x)[0];
    float g = labImage.at<Vec3b>(pt.y,pt.x)[1]; //channel 2
    float r = labImage.at<Vec3b>(pt.y,pt.x)[2]; //channel 3

    cout<<"x: "<<pt.x<<" y: "<< pt.y <<" red: "<< r <<" green: "<< g <<" blue: "<< b <<endl;

    if(clickNum == 1) //assign red sample
    {
        cout<<"Assigned as red" << endl;
        redSample[0] = b;
        redSample[1] = g;
        redSample[2] = r;
    }
    if(clickNum == 2) //assign green sample
    {
        cout<<"Assigned as green" << endl;
        greenSample[0] = b;
        greenSample[1] = g;
        greenSample[2] = r;
    }
    if(clickNum == 3) //assign blue sample
    {
        cout<<"Assigned as blue" << endl;
        blueSample[0] = b;
        blueSample[1] = g;
        blueSample[2] = r;
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    gui = ui;

    rgbImage = cv::imread("/home/daniel/BallTracking/build-BallAnchoring-Desktop_Qt_5_9_0_GCC_64bit-Debug/ball2.jpg");
    cvtColor(rgbImage, labImage, cv::COLOR_BGR2Lab);

    namedWindow("Pic");
    setMouseCallback( "Pic", onMouse, 0 );
    imshow("Pic", rgbImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_rgbThreshCheckBox_stateChanged(int arg1)
{

    Mat diffMaskR = Mat::zeros(rgbImage.size(), CV_8U);
    Mat diffMaskG = Mat::zeros(rgbImage.size(), CV_8U);
    Mat diffMaskB = Mat::zeros(rgbImage.size(), CV_8U);

    if(gui->rgbThreshCheckBox->isChecked())
    {
           for(int j=0; j<rgbImage.rows; ++j)
               for(int i=0; i<rgbImage.cols; ++i)
               {
//                   cout << (int) labImage.at<Vec3b>(j,i)[0] << "    " << (int) labImage.at<Vec3b>(j,i)[1] << "    " << (int) labImage.at<Vec3b>(j,i)[2] << endl;

                   if(labImage.at<Vec3b>(j,i)[0] < gui->redASlider->value() && labImage.at<Vec3b>(j,i)[0] > gui->redASlider_2->value()
                           && labImage.at<Vec3b>(j,i)[1] < gui->redBSlider->value() && labImage.at<Vec3b>(j,i)[1] > gui->redBSlider_2->value()
                           && labImage.at<Vec3b>(j,i)[2] < gui->redCSlider->value() && labImage.at<Vec3b>(j,i)[2] > gui->redCSlider_2->value())
                   {
                       diffMaskR.at<unsigned char>(j,i) = 255;
                   }
                   //greenMask
                   if(labImage.at<Vec3b>(j,i)[0] < gui->greenASlider->value() && labImage.at<Vec3b>(j,i)[0] > gui->greenASlider_2->value()
                           && labImage.at<Vec3b>(j,i)[1] < gui->greenBSlider->value() && labImage.at<Vec3b>(j,i)[1] > gui->greenBSlider_2->value()
                           && labImage.at<Vec3b>(j,i)[2] < gui->greenCSlider->value() && labImage.at<Vec3b>(j,i)[2] > gui->greenCSlider_2->value())
                   {
                       diffMaskG.at<unsigned char>(j,i) = 255;
                   }
                   //blueMask
                   if(labImage.at<Vec3b>(j,i)[0] < gui->blueASlider->value() && labImage.at<Vec3b>(j,i)[0] > gui->blueASlider_2->value()
                           && labImage.at<Vec3b>(j,i)[1] < gui->blueBSlider->value() && labImage.at<Vec3b>(j,i)[1] > gui->blueBSlider_2->value()
                           && labImage.at<Vec3b>(j,i)[2] < gui->blueCSlider->value() && labImage.at<Vec3b>(j,i)[2] > gui->blueCSlider_2->value())
                   {
                       diffMaskB.at<unsigned char>(j,i) = 255;
                   }
               }
            //blueMask



           Mat mask, maskedImage, maskedImageR, maskedImageG, maskedImageB;
           maskedImage = cv::Mat::zeros(rgbImage.rows, rgbImage.cols, CV_8U);
           maskedImageR = cv::Mat::zeros(rgbImage.rows, rgbImage.cols, CV_8U);
           maskedImageG = cv::Mat::zeros(rgbImage.rows, rgbImage.cols, CV_8U);
           maskedImageB = cv::Mat::zeros(rgbImage.rows, rgbImage.cols, CV_8U);
           mask = cv::Mat::zeros(rgbImage.rows, rgbImage.cols, CV_8U);
           bitwise_or(diffMaskR, diffMaskG, mask);
           bitwise_or(mask, diffMaskB, mask);

           for(int i=0; i < gui->erosionSlider->value(); i++)
           {
               erode(mask, mask, Mat(), Point(-1, -1), 2, 1, 1);
           }
           for(int i=0; i < gui->dilationSlider->value(); i++)
           {
               dilate(mask, mask, Mat(), Point(-1, -1), 2, 1, 1);
           }

           rgbImage.copyTo(maskedImage, mask);
           rgbImage.copyTo(maskedImageR, mask);
           rgbImage.copyTo(maskedImageG, mask);
           rgbImage.copyTo(maskedImageB, mask);
           currentMask = mask;

           imshow("Pic", maskedImage);
//           imshow("red", diffMaskR);
//           imshow("green", diffMaskG);
//           imshow("blue", diffMaskB);
//           imshow("mask", mask);

    }
    else
    {
        imshow("Pic", rgbImage);
    }
}



void MainWindow::on_erosionSlider_sliderMoved(int position)
{
    MainWindow::on_rgbThreshCheckBox_stateChanged(1);
}

void MainWindow::on_dilationSlider_sliderMoved(int position)
{
    MainWindow::on_rgbThreshCheckBox_stateChanged(1);
}

void MainWindow::on_maskedImageRadioButton_toggled(bool checked)
{
    if(checked)
    {
        rgbImage = cv::imread("/home/daniel/BallTracking/build-BallAnchoring-Desktop_Qt_5_9_0_GCC_64bit-Debug/ball2.jpg");
        cvtColor(rgbImage, labImage, cv::COLOR_BGR2Lab);
    }
    else
    {
        rgbImage = cv::imread("/home/daniel/BallTracking/build-BallAnchoring-Desktop_Qt_5_9_0_GCC_64bit-Debug/ball1.jpg");
        cvtColor(rgbImage, labImage, cv::COLOR_BGR2Lab);
    }
    MainWindow::on_rgbThreshCheckBox_stateChanged(1);
}

void MainWindow::on_redASlider_sliderMoved(int position)
{
        MainWindow::on_rgbThreshCheckBox_stateChanged(1);
        cout << "New position:  " << position << endl;
}

void MainWindow::on_redBSlider_sliderMoved(int position)
{
        MainWindow::on_rgbThreshCheckBox_stateChanged(1);
        cout << "New position:  " << position << endl;
}


void MainWindow::on_greenASlider_sliderMoved(int position)
{
        MainWindow::on_rgbThreshCheckBox_stateChanged(1);
        cout << "New position:  " << position << endl;
}

void MainWindow::on_greenBSlider_sliderMoved(int position)
{
        MainWindow::on_rgbThreshCheckBox_stateChanged(1);
        cout << "New position:  " << position << endl;
}

void MainWindow::on_blueASlider_sliderMoved(int position)
{
        MainWindow::on_rgbThreshCheckBox_stateChanged(1);
        cout << "New position:  " << position << endl;
}

void MainWindow::on_blueBSlider_sliderMoved(int position)
{
        MainWindow::on_rgbThreshCheckBox_stateChanged(1);
        cout << "New position:  " << position << endl;
}

void MainWindow::on_redCSlider_sliderMoved(int position)
{
    MainWindow::on_rgbThreshCheckBox_stateChanged(1);
    cout << "New position:  " << position << endl;
}

void MainWindow::on_greenCSlider_sliderMoved(int position)
{
    MainWindow::on_rgbThreshCheckBox_stateChanged(1);
    cout << "New position:  " << position << endl;
}

void MainWindow::on_blueCSlider_sliderMoved(int position)
{
    MainWindow::on_rgbThreshCheckBox_stateChanged(1);
    cout << "New position:  " << position << endl;
}

void MainWindow::on_pushButton_clicked()
{
    imwrite("mask.png", currentMask);
}

void MainWindow::on_blueASlider_2_sliderMoved(int position)
{
    MainWindow::on_rgbThreshCheckBox_stateChanged(1);
    cout << "New position:  " << position << endl;
}

void MainWindow::on_blueBSlider_2_sliderMoved(int position)
{
    MainWindow::on_rgbThreshCheckBox_stateChanged(1);
    cout << "New position:  " << position << endl;
}

void MainWindow::on_blueCSlider_2_sliderMoved(int position)
{
    MainWindow::on_rgbThreshCheckBox_stateChanged(1);
    cout << "New position:  " << position << endl;
}

void MainWindow::on_redASlider_2_sliderMoved(int position)
{
    MainWindow::on_rgbThreshCheckBox_stateChanged(1);
    cout << "New position:  " << position << endl;
}

void MainWindow::on_redBSlider_2_sliderMoved(int position)
{
    MainWindow::on_rgbThreshCheckBox_stateChanged(1);
    cout << "New position:  " << position << endl;
}

void MainWindow::on_redCSlider_2_sliderMoved(int position)
{
    MainWindow::on_rgbThreshCheckBox_stateChanged(1);
    cout << "New position:  " << position << endl;
}

void MainWindow::on_WebcamCheckBox_toggled(bool checked)
{
   VideoCapture cap(1);

    if(checked)
    {
        // Create a VideoCapture object and open the input file
        // If the input is the web camera, pass 0 instead of the video file name


        // Check if camera opened successfully
        if(!cap.isOpened()){
          cout << "Error opening video stream or file" << endl;
        }

        while(1){

          Mat frame;
          // Capture frame-by-frame
          cap >> frame;

          // If the frame is empty, break immediately
          if (frame.empty())
            break;

          // Display the resulting frame
          imshow( "Frame", frame );


          if(gui->rgbThreshCheckBox->isChecked())
          {

              Mat labFrame = cv::Mat::zeros(frame.rows, frame.cols, CV_8U);;
              cvtColor(frame, labFrame, cv::COLOR_BGR2Lab);

              Mat diffMaskR = Mat::zeros(frame.size(), CV_8U);
              Mat diffMaskG = Mat::zeros(frame.size(), CV_8U);
              Mat diffMaskB = Mat::zeros(frame.size(), CV_8U);

                 for(int j=0; j<frame.rows; ++j)
                     for(int i=0; i<frame.cols; ++i)
                     {
      //                   cout << (int) labImage.at<Vec3b>(j,i)[0] << "    " << (int) labImage.at<Vec3b>(j,i)[1] << "    " << (int) labImage.at<Vec3b>(j,i)[2] << endl;

                         if(labFrame.at<Vec3b>(j,i)[0] < gui->redASlider->value() && labFrame.at<Vec3b>(j,i)[0] > gui->redASlider_2->value()
                                 && labFrame.at<Vec3b>(j,i)[1] < gui->redBSlider->value() && labFrame.at<Vec3b>(j,i)[1] > gui->redBSlider_2->value()
                                 && labFrame.at<Vec3b>(j,i)[2] < gui->redCSlider->value() && labFrame.at<Vec3b>(j,i)[2] > gui->redCSlider_2->value())
                         {
                             diffMaskR.at<unsigned char>(j,i) = 255;
                         }
                         //greenMask
                         if(labFrame.at<Vec3b>(j,i)[0] < gui->greenASlider->value() && labFrame.at<Vec3b>(j,i)[0] > gui->greenASlider_2->value()
                                 && labFrame.at<Vec3b>(j,i)[1] < gui->greenBSlider->value() && labFrame.at<Vec3b>(j,i)[1] > gui->greenBSlider_2->value()
                                 && labFrame.at<Vec3b>(j,i)[2] < gui->greenCSlider->value() && labFrame.at<Vec3b>(j,i)[2] > gui->greenCSlider_2->value())
                         {
                             diffMaskG.at<unsigned char>(j,i) = 255;
                         }
                         //blueMask
                         if(labFrame.at<Vec3b>(j,i)[0] < gui->blueASlider->value() && labFrame.at<Vec3b>(j,i)[0] > gui->blueASlider_2->value()
                                 && labFrame.at<Vec3b>(j,i)[1] < gui->blueBSlider->value() && labFrame.at<Vec3b>(j,i)[1] > gui->blueBSlider_2->value()
                                 && labFrame.at<Vec3b>(j,i)[2] < gui->blueCSlider->value() && labFrame.at<Vec3b>(j,i)[2] > gui->blueCSlider_2->value())
                         {
                             diffMaskB.at<unsigned char>(j,i) = 255;
                         }
                     }

                 Mat mask, maskedImage;
                 maskedImage = cv::Mat::zeros(frame.rows, frame.cols, CV_8U);
                 mask = cv::Mat::zeros(frame.rows, frame.cols, CV_8U);
                 bitwise_or(diffMaskR, diffMaskG, mask);
                 bitwise_or(mask, diffMaskB, mask);

                 for(int i=0; i < gui->erosionSlider->value(); i++)
                 {
                     erode(mask, mask, Mat(), Point(-1, -1), 2, 1, 1);
                 }
                 for(int i=0; i < gui->dilationSlider->value(); i++)
                 {
                     dilate(mask, mask, Mat(), Point(-1, -1), 2, 1, 1);
                 }

                 frame.copyTo(maskedImage, mask);
//                 rgbImage.copyTo(maskedImageR, mask);
//                 rgbImage.copyTo(maskedImageG, mask);
//                 rgbImage.copyTo(maskedImageB, mask);
//                 currentMask = mask;

                 imshow( "FilteredFrame", maskedImage );



                 Mat src_gray, src;
                 src = cv::Mat::zeros(frame.rows, frame.cols, CV_8U);
                 maskedImage.copyTo(src);
                 /// Convert it to gray
                 cvtColor( src, src_gray, CV_BGR2GRAY );



                 Mat grad_y;
                 Mat abs_grad_y;
                 int scale = 1;
                 int delta = 0;
                 int ddepth = CV_16S;
                 int sobelKernelSize = 5; //5;
                 int GaussianBlurKernelSize = 15;

                 /// Reduce the noise so we avoid false circle detection
//                 GaussianBlur( src_gray, src_gray, Size(GaussianBlurKernelSize, GaussianBlurKernelSize), 2, 2 );
                 medianBlur(src_gray, src_gray, 9);

                 /// Gradient Y
                 //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
                 Sobel( src_gray, grad_y, ddepth, 0, 1, sobelKernelSize, scale, delta, BORDER_DEFAULT );
                 convertScaleAbs( grad_y, abs_grad_y );

                imshow( "gradient", abs_grad_y );

                Mat gradientMask = cv::Mat::zeros(frame.rows, frame.cols, CV_8U);
                threshold(abs_grad_y, gradientMask, gui->gradientThreshSlider->value(), 255, CV_THRESH_BINARY);

                imshow( "gradientThresh", gradientMask );

                // HOUGH transform

                 vector<Vec3f> circles;

//                 CV_HOUGH_GRADIENT CV_HOUGH_MULTI_SCALE CV_HOUGH_PROBABILISTIC CV_HOUGH_STANDARD

                 Mat src2, src_gray2;
                 src2 = cv::Mat::zeros(frame.rows, frame.cols, CV_8U);
                 src_gray2 = cv::Mat::zeros(frame.rows, frame.cols, CV_8U);
                 maskedImage.copyTo(src2, gradientMask);
                 /// Convert it to gray
                 cvtColor( src2, src_gray2, CV_BGR2GRAY );

                 /// Apply the Hough Transform to find the circles
//                 HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 0.5,  20, 70, 30, 0, 100 );
                 HoughCircles( src_gray2, circles, CV_HOUGH_GRADIENT, 1,  50, 70, 30, 5, 100 );
//                 HoughCircles( src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows/8, 200, 100, 0, 100 ); //Default

                 /// Draw the circles detected
                 for( size_t i = 0; i < circles.size(); i++ )
                 {
                     Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                     int radius = cvRound(circles[i][2]);
                     // circle center
                     circle( src2, center, 3, Scalar(0,255,0), -1, 8, 0 );
                     // circle outline
                     circle( src2, center, radius, Scalar(0,0,255), 3, 8, 0 );
                  }
                 // Display the resulting frame
                 imshow( "Balls", src2 );

          }


          // Press  ESC on keyboard to exit
          char c=(char)waitKey(25);
          if(c==27)
            break;
        }
    }

    if(!checked)
    {
        // When everything done, release the video capture object
        cap.release();

        // Closes all the frames
//        destroyAllWindows();
    }
}
