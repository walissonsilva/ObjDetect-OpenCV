#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

// Biblioteca que permite a comunicação serial
#include "rs232.h"

// Porta com a qual o RS232 se comunicará
#define COM 25

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay(Mat &frame);

/** Global variables */
String face_cascade_name = "haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
string window_name = "Capture - Face detection";
char bytes[1000];
unsigned char b = 100;
stringstream ss;

 /** @function main */
 int main(){
   VideoCapture cap(0);
   Mat frame;

   /// Testa a abertura da porta serial do Arduino
   /// Se a porta serial não for carregada a função retorna 1 e o programa é encerrado
   if(RS232_OpenComport(COM, 57600, "8N1")){
       printf("Erro ao carregar a porta.\n");
       return 1;
   }

   //-- 1. Load the cascades
   if( !face_cascade.load(face_cascade_name) ){ printf("--(!)Error loading\n"); return -1; };
   //if( !eyes_cascade.load(eyes_cascade_name) ){ printf("--(!)Error loading\n"); return -1; };

   //-- 2. Read the video stream
   //capture = cvCaptureFromCAM(0);
   if(cap.isOpened())
   {
     while( true )
     {
         //Mat frame;
         cap >> frame;

   //-- 3. Apply the classifier to the frame
       if( !frame.empty() )
       { detectAndDisplay(frame); }
       else
       { printf(" --(!) No captured frame -- Break!"); break; }

       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
      }
   }

   RS232_CloseComport(COM);

   return 0;
 }

/** @function detectAndDisplay */
void detectAndDisplay(Mat &frame)
{
  static vector<Rect> faces;
  static Mat frame_gray;

  cvtColor( frame, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  size_t tam = faces.size();
  for(size_t i = 0; i < tam; i++){
    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );

    /// Transforma o valor da coodenada x do pixel central da face (int --> char *)
    ss.clear();
    ss << center.x;
    ss >> bytes;

    cout << bytes << endl;

    /// Envia o valor da coodenada x do pixel central da face para a porta serial do Arduino
    RS232_cputs(COM, bytes);

    /// Cria uma elipse em volta do pixel central da face para destacá-la
    //ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 255, 255 ), 4, 8, 0 );
  }

  /// Exibir a frame do vídeo que acabou de ser processada
  //imshow( window_name, frame );
 }
