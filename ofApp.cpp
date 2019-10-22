#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofEnableDepthTest();
	ofSetLineWidth(5);

	this->ico_sphere = ofIcoSpherePrimitive(150, 3);
	this->threshold = 0;
}

//--------------------------------------------------------------
void ofApp::update() {

	Leap::Frame frame = this->leap.frame();
	for (Leap::Hand hand : frame.hands()) {

		if (hand.pinchStrength() > 0.5) {

			this->threshold -= this->threshold > 0 ? 0.03 : 0;
		}
		else {

			this->threshold += this->threshold <= 1 ? 0.03 : 0;
		}
	}

	this->mesh_list.clear();
	auto triangle_list = this->ico_sphere.getMesh().getUniqueFaces();

	for (auto& triangle : triangle_list) {

		glm::vec3 avg = (triangle.getVertex(0) + triangle.getVertex(1) + triangle.getVertex(2)) / 3;
		auto noise_value = ofNoise(avg.x * 0.03, avg.y * 0.03, avg.z * 0.03, ofGetFrameNum() * 0.003);
		auto noise_radius = ofMap(noise_value, 0, 1, 250, 250 + 500 * this->threshold);

		ofMesh mesh;

		mesh.addVertex(glm::normalize(avg) * (noise_radius - 50));
		mesh.addVertex(glm::normalize(triangle.getVertex(0)) * noise_radius);
		mesh.addVertex(glm::normalize(triangle.getVertex(1)) * noise_radius);
		mesh.addVertex(glm::normalize(triangle.getVertex(2)) * noise_radius);

		mesh.addIndex(0);
		mesh.addIndex(1);
		mesh.addIndex(2);

		mesh.addIndex(0);
		mesh.addIndex(2);
		mesh.addIndex(3);

		mesh.addIndex(0);
		mesh.addIndex(1);
		mesh.addIndex(3);

		mesh.addIndex(1);
		mesh.addIndex(2);
		mesh.addIndex(3);

		this->mesh_list.push_back(mesh);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.5);
	ofRotateX(ofGetFrameNum() * 0.25);

	for (auto& mesh : this->mesh_list) {

		ofSetColor(239);
		mesh.drawFaces();

		ofSetColor(39);
		mesh.drawWireframe();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}