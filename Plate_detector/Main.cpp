#include "Detect.h"

int main(int argc, char** argv) {
	std::string xml_path = "haarcascade_frontalface_default.xml";
	std::string data_path = "img1";

	Detect detect(xml_path,data_path);
	detect.run(PHOTO);
	detect.display();
	//printf("************************************************************************************************");
	cv::waitKey(0);

	return 0;
}


