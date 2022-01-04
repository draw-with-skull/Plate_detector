#include "Detect.h"

int main(int argc, char** argv) {
	std::string xml_path = "haarcascade_frontalface_default";
	std::string data_path = "img1.png";

	Detect detect(xml_path);
	detect.run(WEBCAM);
	detect.display();
	cv::waitKey(0);

	return 0;
}


