#include "Detect.h"


void Detect::run(uint input_type)
{
	//Input type PHOTO,VIDEO,WEBCAM
	runnable = false;
	/*checks for any data/xml errors*/
	if (check_xml() && check_data(input_type)) {
		runnable = true;
		/*processing based on the data type*/
		switch (input_type)
		{
		case 1: {
			/*Images*/
			frame = cv::imread(data_path);
			process(frame);

		}break;
		case 2: {
			/*Video*/
			cv::VideoCapture cap(data_path);
			while(cap.read(frame)) {
				process(frame);
				if (display_content){
					display_detected_area();
				}
				if (cv::waitKey(10) == 27)
					break;
			}

		}break;
		case 3: {
			/*Webcam*/
			cv::VideoCapture cap(camera_id);
			while (true) {
				cap.read(frame);
				process(frame);
				if (display_content) {
					display_detected_area();
				}
				if (cv::waitKey(10) == 27)
					break;
			}
		}break;

		default:
			std::cout << "ERROR: Not a valid input type" << std::endl;
			break;
		}
	}
	else
		std::cout << "-----------------------------------------------\nAn error has occurred, no processing took place\n\n";

}

void Detect::display_detected_area()
{
	/*Checks if any proccessing took place*/
	if (!runnable) {
		std::cout << "ERROR: Nothing to display, no processing happened\n";
		return;
	}
	/*dispalys all objects detected*/
	for (uint i = 0; i < obj_detected.size(); i++)
	{
		cv::rectangle(frame, obj_detected[i].tl(), obj_detected[i].br(), cv::Scalar(255, 0, 0), 3);
	}
	cv::imshow("display", frame);

}

void Detect::save()
{
	if (runnable) {
		cv::Mat result;
		for (uint i = 0; i < obj_detected.size(); i++) {
			/*Creates the path*/
			std::string output_path = "Output/";
			output_path.append(std::to_string(i));
			output_path.append(".png");
			/*Crops the image*/
			if (crop_result)
				result = frame(obj_detected[i]);
			else
				result = frame;
			/*actual save*/
			if(!result.empty())
				cv::imwrite(output_path, result);
			
		}
		

	}
	else{
		std::cout << "ERROR: nothing to save\n";
	}
}

void Detect::set_save_settings(bool save_result, bool crop_result) {
	/*setting for saving*/
	this->save_result = save_result;
	this->crop_result = crop_result;
}

void Detect::set_data_path(std::string& data_path)
{
	this->data_path = data_path;
	/*checks if the path si empty*/
	if (this->data_path.empty()) {
		std::cout << "ERROR: Data path is empty" << std::endl;
		return;
	}
	
}

void Detect::set_camera_id(uint camera_id) {
	/*does it need a comment?*/
	this->camera_id = camera_id;
}

void Detect::display(bool dispaly_content){
	this->display_content = dispaly_content;
}

void Detect::set_xml_path(std::string& xml_path) {
	this->xml_path = xml_path;
	/*checks path if empty*/
	if (this->xml_path.empty()) {
		std::cout << "ERROR: Xml path is empty" << std::endl;
		return;
	}
	/*checks the path lenght*/
	std::string path;
	if (this->xml_path.size() > 4)
		path = this->xml_path.substr(this->xml_path.length() - 4, 4);
	else
		path = this->xml_path;
	/*checks the extention*/
	if (path.find(".") != std::string::npos) {
		if (path.find(".xml") == std::string::npos)
			std::cout << "ERROR: Extentions is wrong" << std::endl;
	}
	else {
		std::cout << "Warning: Extention was missing and was added automaticly" << std::endl;
		this->xml_path.append(".xml");
		std::cout << "Result:" << this->xml_path << std::endl;
	}
}

void Detect::process(cv::Mat& frame)
{
	cv::Mat frame_gray;
	/*detection using hassa*/
	cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(frame_gray, frame_gray);
	classifier_cascade.detectMultiScale(frame_gray, obj_detected);
}

void Detect::set_settings_false()
{
	this->save_result = false;
	this->crop_result = false;
	this->runnable = false;
	this->display_content = false;
}

bool Detect::check_extention(std::vector<std::string> formats)
{
	/*checks the lenght of the path*/
	std::string path;
	bool contains = false;
	if (this->data_path.size() > 5)
		path = this->data_path.substr(this->data_path.size() - 5, 5);
	else
		path = this->data_path;

	/*checks the extention*/
	if (path.find(".") != std::string::npos) {
		for (uint i = 0; i < img_formats.size(); i++) {
			if (path.find(img_formats[i]) != std::string::npos) {
				contains = true;
				return true;
			}
		}
		if (!contains) {
			std::cout << "ERROR: No extention found for the file\n";
			return false;
		}
	}
	else
		std::cout << "ERROR: Data file extention missing\n";
	return false;
}

bool Detect::check_xml() {
	/*checks if the path is empty*/
	if (this->xml_path.empty()) {
		std::cout << "ERROR: Xml path is empty" << std::endl;
		return false;
	}
	/*cheks if it can open the file*/
	if (!this->classifier_cascade.load(this->xml_path)) {
		std::cout << "ERROR: unable to load xml file" << std::endl;
		return false;
	}
	return true;
}

bool Detect::check_data(uint input_type) {
	/*checks the data based on it's type*/
	switch (input_type)
	{
	case PHOTO: {
		/*checks for an empty path*/
		if (this->data_path.empty()) {
			std::cout << "ERROR: Photo path is empty" << std::endl;
			return false;
		}
		/*check extention*/
		return check_extention(this->img_formats);
	}break;
	case VIDEO: {
		if (this->data_path.empty()) {
			std::cout << "ERROR: Video path is empty";
			return false;
		}
		/*check extention*/
		return check_extention(this->video_formats);
	}break;

	/*Webcam*/
	case WEBCAM: {return true; }break;
	default:{return false; }break;
	}
	return false;
}

Detect::Detect(std::string& xml_path, std::string& data_path)
{
	//Full Constructor
	this->set_xml_path(xml_path);
	this->set_data_path(data_path);
	
	this->set_settings_false();
}

Detect::Detect(std::string& xml_path)
{
	//xml data only
	this->set_xml_path(xml_path);
	
	this->set_settings_false();
}

Detect::Detect()
{
	this->set_settings_false();
	//settings
}

Detect::~Detect()
{
	//Add stuff
}