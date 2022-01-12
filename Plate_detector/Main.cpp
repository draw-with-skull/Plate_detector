#include "Detect.h"

int main(int argc, char** argv) {
	std::string xml_path = "haarcascade_frontalface_default.xml";
	std::string data_path = "img1.png";

	Detect detect(xml_path,data_path);
	detect.display(true);
	detect.set_save_settings(true, true, true);
	detect.run(WEBCAM);
	//detect.save();
	//printf("************************************************************************************************");
	cv::waitKey(0);

	return 0;
}


