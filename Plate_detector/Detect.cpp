#include "Detect.h"


void Detect::run(uint input_type)
{
	//Input type PHOTO,VIDEO,WEBCAM
	if (check_xml() && check_data(input_type)) {
		switch (input_type)
		{
		case 1: {
			frame = cv::imread(data_path);
			process(frame);

		}break;
		case 2: {
			cv::VideoCapture cap(data_path);
			while (true) {
				cap.read(frame);
				process(frame);
				display();
				cv::waitKey(1);
			}

		}break;
		case 3: {
			cv::VideoCapture cap(camera_id);
			while (true) {
				cap.read(frame);
				process(frame);
				display();
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

void Detect::display()
{
	for (uint i = 0; i < obj_detected.size(); i++)
	{
		cv::rectangle(frame, obj_detected[i].tl(), obj_detected[i].br(), cv::Scalar(255, 0, 0), 3);
	}
	cv::imshow("display", frame);

}

void Detect::set_dispaly_Nsave(bool display_result, bool save_result, bool crop_result) {
	this->display_result = display_result;
	this->save_result = save_result;
	this->crop_result = crop_result;
}

void Detect::set_data_path(std::string& data_path)
{
	/*checks path*/
	this->data_path = data_path;
	if (this->data_path.empty()) {
		std::cout << "ERROR: Data path is empty" << std::endl;
		return;
	}
	/*checks extention*/
	bool contains = false;
	std::string path;
	if (this->data_path.size() > 5)
		path = this->data_path.substr(this->data_path.size() - 5, 5);
	else
		path = this->data_path;

	if (path.find(".") != std::string::npos) {
		for (uint i = 0; i < img_formats.size(); i++) {
			if (path.find(img_formats[i]) == std::string::npos) {
				contains = true;
				break;
			}
		}

		
	}
	else
		std::cout << "ERROR: Data file extention missing\n";


}

void Detect::set_camera_id(uint camera_id) {
	this->camera_id = camera_id;
}

void Detect::set_xml_path(std::string& xml_path) {
	this->xml_path = xml_path;
	/*checks path*/
	if (this->xml_path.empty()) {
		std::cout << "ERROR: Xml path is empty" << std::endl;
		return;
	}

	/*checks the extention*/

	std::string end_of_path = this->xml_path.substr(this->xml_path.length() - 4, 4);
	
	if (end_of_path.find(".") != std::string::npos) {
		if (end_of_path.find(".xml") == std::string::npos)
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

	cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
	cv::equalizeHist(frame_gray, frame_gray);
	classifier_cascade.detectMultiScale(frame_gray, obj_detected);
}

bool Detect::check_xml(){
	if (this->xml_path.empty()) {
		std::cout << "ERROR: Xml path is empty" << std::endl;
		return false;
	}
	if (!this->classifier_cascade.load(this->xml_path)) {
		std::cout << "ERROR: unable to load xml file" << std::endl;
		return false;
	}
	return true;
}

bool Detect::check_data(uint input_type){
	switch (input_type)
	{
	case PHOTO: {
		if (this->data_path.empty()) {
			std::cout << "ERROR: Photo path is empty" << std::endl;
			return false;
		}

		
		//add extention check
		return true;
	}break;
	case VIDEO: {}break;
	case WEBCAM: {return true; }break;
	default:
		break;
	}
}

Detect::Detect(std::string& xml_path, std::string& data_path)
{
	this->set_xml_path(xml_path);
	this->set_data_path(data_path);
}

Detect::Detect(std::string& xml_path)
{
	this->set_xml_path(xml_path);
}

Detect::Detect()
{
}

Detect::~Detect()
{
}
