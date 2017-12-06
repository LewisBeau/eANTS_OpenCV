// ArucoInterface.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "com_eants_imagerecognition_Aruco.h"

#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>


#include <sstream>
#include <fstream>

#include <math.h>      

#define PI 3.14159265

typedef unsigned char byte;

using namespace std;
using namespace cv;

cv::VideoCapture inputVideo;
cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_250);

cv::Mat image;

std::vector<int> ids;
std::vector<std::vector<cv::Point2f> > corners;


JNIEXPORT jboolean JNICALL Java_com_eants_imagerecognition_Aruco_startVideo(JNIEnv *env, jclass clz, jint channel) {
	if (inputVideo.isOpened()) {
		return true;
	}
	else {
		inputVideo.open(channel);
		if (inputVideo.isOpened())return true;
		return false;
	}
	return false;
}

JNIEXPORT jboolean JNICALL Java_com_eants_imagerecognition_Aruco_isOpen(JNIEnv *env, jclass clz) {
	return inputVideo.isOpened();
}

JNIEXPORT jint JNICALL Java_com_eants_imagerecognition_Aruco_findRobots(JNIEnv *env, jclass clz) {
	//cv::Mat image;
	inputVideo.retrieve(image, CV_BGR2GRAY);
	cv::aruco::detectMarkers(image, dictionary, corners, ids);
	/*image.copyTo(outputImage);
	if (ids.size() > 0) {
		cv::aruco::drawDetectedMarkers(outputImage, corners, ids);
	}*/
	return ids.size();
}

JNIEXPORT jint JNICALL Java_com_eants_imagerecognition_Aruco_getRobotCentrePointX(JNIEnv *env, jclass clz, jint robot) {
	for (int i = 0; i < ids.size(); i++) {
		if (ids.at(i) == robot) {
			//At this point we have the x and y values of the requested robot
			int x1 = (int)corners.at(i).at(0).x;
			int x2 = (int)corners.at(i).at(2).x;
			int centreX = ((x2 - x1) / 2) + x1;
			return centreX;
		}
	}
	return -1;
}

JNIEXPORT jint JNICALL Java_com_eants_imagerecognition_Aruco_getRobotCentrePointY(JNIEnv *env, jclass clz, jint robot) {
	for (int i = 0; i < ids.size(); i++) {
		//At this point we have the x and y values of the requested robot
		if (ids.at(i) == robot) {
			//At this point we have the x and y values of the requested robot
			int y1 = (int)corners.at(i).at(0).y;
			int y2 = (int)corners.at(i).at(2).y;
			int centreY = ((y2 - y1) / 2) + y1;
			return centreY;
		}
	}
	return -1;
}

JNIEXPORT jint JNICALL Java_com_eants_imagerecognition_Aruco_getRobotRadius(JNIEnv *env, jclass clz, jint robot) {
	for (int i = 0; i < ids.size(); i++) {
		//At this point we have the x and y values of the requested robot
		if (ids.at(i) == robot) {
			//At this point we have the x and y values of the requested robot
			int x1 = (int)corners.at(i).at(0).x;
			int x2 = (int)corners.at(i).at(2).x;
			int y1 = (int)corners.at(i).at(0).y;
			int y2 = (int)corners.at(i).at(2).y;
			float diameter = cubeRoot((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
			int radius = diameter / 2;
			return radius;
		}
	}
	return -1;
}

JNIEXPORT jint JNICALL Java_com_eants_imagerecognition_Aruco_getRobotRotation(JNIEnv *env, jclass clzv, jint robot) {
	for (int i = 0; i < ids.size(); i++) {
		//At this point we have the x and y values of the requested robot
		if (ids.at(i) == robot) {
			//At this point we have the x and y values of the requested robot
			int x1 = (int)corners.at(i).at(0).x;
			int x4 = (int)corners.at(i).at(3).x;
			int y1 = (int)corners.at(i).at(0).y;
			int y4 = (int)corners.at(i).at(3).y;
			int angle = -1;
			//To be completed
			return angle;
		}
	}
	return -1;
}


JNIEXPORT jbyteArray JNICALL Java_com_eants_imagerecognition_Aruco_getImage(JNIEnv * env, jclass clzv) {
	//cv::Mat image;
	//inputVideo.retrieve(image);
	int size = image.total() * image.elemSize();
	byte * bytes = new byte[size];  
	std::memcpy(bytes, image.data, size * sizeof(byte));
	jbyteArray array = env->NewByteArray(size);
	env->SetByteArrayRegion(array, 0, size, reinterpret_cast<jbyte*>(bytes));
	delete[] bytes;
	return array;
}


JNIEXPORT jbyteArray JNICALL Java_com_eants_imagerecognition_Aruco_getPaintedImage(JNIEnv * env, jclass clzv) {
	//cv::Mat image;
	//inputVideo.retrieve(image);
	cv::Mat outputImage;
	image.copyTo(outputImage);

	if (ids.size() > 0) {
		cv::aruco::drawDetectedMarkers(outputImage, corners, ids);
	}

	int size = outputImage.total() * outputImage.elemSize();

	byte * bytes = new byte[size];  
	std::memcpy(bytes, outputImage.data, size * sizeof(byte));


	jbyteArray array = env->NewByteArray(size);
	env->SetByteArrayRegion(array, 0, size, reinterpret_cast<jbyte*>(bytes));

	delete[] bytes;
	return array;

}


JNIEXPORT jint JNICALL Java_com_eants_imagerecognition_Aruco_getImageWidth(JNIEnv *, jclass) {
	//cv::Mat image;
	//inputVideo.retrieve(image, CV_BGR2GRAY);
	return image.size().width;

}

JNIEXPORT jint JNICALL Java_com_eants_imagerecognition_Aruco_getImageHeigth(JNIEnv *, jclass) {
	//cv::Mat image;
	//inputVideo.retrieve(image, CV_BGR2GRAY);
	return image.size().height;

}
