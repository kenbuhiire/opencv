#include "cv.h"
#include "highgui.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <string>

#include <cmath>
#include <limits>
#include <iterator>

#include "helper.hpp"
#include "eigenfaces.hpp"

using namespace std;
using namespace cv;

void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels) {
	std::ifstream file(filename.c_str(), ifstream::in);
	if(file) {
		std::string line, path, classlabel;
		// for each line
		while (std::getline(file, line)) {
			// get current line
			std::stringstream liness(line);
			// split line
			std::getline(liness, path, ';');
			std::getline(liness, classlabel);
			// push pack the data
			images.push_back(imread(path,0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	} else {
		cerr << "Error: Failed to open file.";
	}
}

int main(int argc, char *argv[]) {
	vector<Mat> images;
	vector<int> labels;
	// read images
	read_csv("./at.txt", images, labels);
	// get width and height
	int width = images[0].cols;
	int height = images[0].rows;
	// get test instances
	Mat testSample = images[images.size()-1];
	int testLabel = labels[labels.size()-1];
	// ... and delete last element
	images.pop_back();
	labels.pop_back();
	// num_components eigenfaces
	int num_components = 80;
	// compute the eigenfaces
	Eigenfaces eigenfaces(images, labels, num_components);
	// get a prediction
	int predicted = eigenfaces.predict(testSample);
	cout << "actual=" << testLabel << " / predicted=" << predicted;
	// see the reconstruction with num_components
	Mat p = eigenfaces.project(images[0].reshape(1,1));
	Mat r = eigenfaces.reconstruct(p);
	imshow("original", images[0]);
	imshow("reconstruction", toGrayscale(r.reshape(1, height)));
	// get the eigenvectors
	Mat W = eigenfaces.eigenvectors();
	// show first 10 eigenfaces
	for(int i = 0; i < 10; i++) {
		Mat ev = W.col(i).clone();
		imshow(num2str(i), toGrayscale(ev.reshape(1, height)));
	}
	waitKey(0);
}
