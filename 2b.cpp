#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

int main(int argc, char** argv)
{
	cv::VideoCapture inputVideo;
	inputVideo.open(0); //receive image from camera

	cv::Mat cameraMatrix, distCoeffs;
	//camera parameters are read from somewhere

	cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_100);
	//Predefined dictionary for 4X4 aruco tag

	while(inputVideo.grab())
	//loop will continue as long as input stream is on
	{
		cv::Mat image, imageCopy;
		inputVideo.retrieve(image); //retrieve image
		image.copyTo(imageCopy); //imageCopy will be used for image processing

		std::vector<int> ids; //ids for each aruco tag
		std::vector<std::vector<cv::Point2f> > corners; //for determining pose
		cv::aruco::detectMarkers(image, dictionary, corners, ids);

		//The conditional below is to show that the markers are detected
		//if at least one marker detected
		if (ids.size() > 0)
		{
			cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
			//for showing detected markers

			std::vector<cv::Vec3d> rVecs, tVecs;
			cv::aruco::estimatePoseSingleMarkers(corners, 0.05, cameraMatrix, distCoeffs, rVecs, tVecs);
			//estimate pose of each marker

			//draw axis for each marker
			for (int i{0}; i < ids.size; ++i)
			{
				cv::aruco::drawAxis(imageCopy, cameraMatrix, distCoeffs, rVecs[i], tVecs[i], 0.1);
			}
		}

		cv::imshow("out", imageCopy); //to display all the result
		char key = cv::waitKey(0); //wait until esc is clicked

		if (key == 27) //esc = 27
		{
			break;
		}
	}


	return 0;
}
