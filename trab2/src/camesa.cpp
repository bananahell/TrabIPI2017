#include <iostream>
#include <opencv2/opencv.hpp>
#include <funcoes.hpp>
#include <funcoes2.hpp>

using namespace std;
using namespace cv;


int main(){
	
	bool fim = false;
    bool click = false;
	
 	VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;
	
	while(!fim){
	

	    Mat edges;
	    namedWindow("edges",1);

	    while(!click){
	        Mat frame;
	        cap >> frame; // get a new frame from camera
	        cvtColor(frame, edges, CV_BGR2GRAY);
	        GaussianBlur(edges, edges, Size(9,9), 1.5, 1.5);
	        Canny(edges, edges, 0, 30, 3);
	        imshow("edges", edges);
	        if(waitKey(30) >= 0){
	         click = !click;
		    }
	    }

	    threshold(edges,edges,50,255,CV_THRESH_BINARY+CV_THRESH_OTSU);

	    imshow("foto",edges);
	    waitKey(0);

	    cout << "deseja guardar essa foto?" << endl;
	    char resp;
	    cin >> resp;

	   	if(resp == 's' || resp == 'S'){
	   		fim = !fim;
	        imwrite("./img/foto.jpg",edges); 
	   	}else if(resp == 'x' || resp == 'X'){
	   		click = !click;
	   		fim = !fim;
	   	}else{
	   		click = !click;

	   	}  		
	}

	return 0;
}