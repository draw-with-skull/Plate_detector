#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#pragma once
enum InputTipes
{
	PHOTO = 1,
	VIDEO = 2,
	WEBCAM = 3
};
class Detect
{
public:
	//funtions
	void run(uint input_type);

	void display();

	void set_save_settings(bool save_result, bool crop_result);
	void set_xml_path(std::string& xml_path);
	void set_data_path(std::string& data_path);
	void set_camera_id(uint camera_id);
	//variables
private:
	//funtions
	void process(cv::Mat& frame);

	bool check_xml();
	bool check_data(uint input_type);

	//variables
	std::vector<cv::Rect>obj_detected;
	std::string xml_path = "\0", data_path = "\0";
	std::vector<std::string> img_formats = { ".pbm", ".pgm", ".ppm", ".sr", ".ras", ".jpeg", ".jpg", ".jpe", ".jp2", ".tiff", ".tif", ".png" };
	cv::CascadeClassifier classifier_cascade;
	cv::Mat frame;

	uint camera_id = 0;
	bool save_result = false, crop_result = false, runnable = false;

public:
	//Constructors
	Detect(std::string& xml_path, std::string& data_path);
	Detect(std::string& xml_path);
	Detect();
	~Detect();
};