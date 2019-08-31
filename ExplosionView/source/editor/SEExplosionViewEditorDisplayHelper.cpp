#include "editor/SEExplosionViewEditorDisplayHelper.hpp"


void displayHelper::displayLine(SBPosition3 start, SBPosition3 end, std::string text) {
	unsigned int nLines = 1;
	unsigned int nPositions = 2;
	unsigned int* indexData = new unsigned int[2 * nLines];
	float *positionData = new float[3 * nPositions];
	unsigned int *capData = new unsigned int[2 * nLines];
	float *colorData = new float[4 * 2 * nLines];
	unsigned int *flagData = new unsigned int[2 * nLines];

	positionData[3 * 0 + 0] = start.v[0].getValue();
	positionData[3 * 0 + 1] = start.v[1].getValue();
	positionData[3 * 0 + 2] = start.v[2].getValue();
	positionData[3 * 1 + 0] = end.v[0].getValue();
	positionData[3 * 1 + 1] = end.v[1].getValue();
	positionData[3 * 1 + 2] = end.v[2].getValue();

	indexData[0] = 0;
	indexData[1] = 1;

	capData[0] = 1;
	capData[1] = 1;

	colorData[0] = 0;
	colorData[1] = 0;
	colorData[2] = 0;
	colorData[3] = 1;
	colorData[4] = 0;
	colorData[5] = 0;
	colorData[6] = 0;
	colorData[7] = 1;

	unsigned int flag = 0;
	flagData[0] = flag;
	flagData[1] = flag;

	SAMSON::displayLines(nLines, nPositions, indexData, positionData, colorData, flagData);

	delete[] indexData;
}


void displayHelper::displayCylinder(SBVector3 vec, SBPosition3 start) {

	int length = 1000;
	vec *= length;

	SBPosition3 end = SBPosition3(
		SBQuantity::picometer(vec[0].getValue()),
		SBQuantity::picometer(vec[1].getValue()),
		SBQuantity::picometer(vec[2].getValue())
	);
	end += start;

	unsigned int nCylinders = 1;
	unsigned int nPositions = 2 * nCylinders;
	unsigned int* indexData = new unsigned int[2 * nCylinders];
	float *positionData = new float[3 * nPositions];
	float *radiusData = new float[2 * nCylinders];
	unsigned int *capData = new unsigned int[2 * nCylinders];
	float *colorData = new float[4 * 2 * nCylinders];
	unsigned int *flagData = new unsigned int[2 * nCylinders];

	positionData[0 * 3 + 0] = start.v[0].getValue();
	positionData[0 * 3 + 1] = start.v[1].getValue();
	positionData[0 * 3 + 2] = start.v[2].getValue();
	positionData[1 * 3 + 0] = end.v[0].getValue();
	positionData[1 * 3 + 1] = end.v[1].getValue();
	positionData[1 * 3 + 2] = end.v[2].getValue();

	indexData[0] = 0;
	indexData[1] = 1;

	radiusData[0] = 200;
	radiusData[1] = 0;

	capData[0] = 1;
	capData[1] = 1;

	colorData[0] = 1.0f;
	colorData[1] = 1.0f;
	colorData[2] = 1.0f;
	colorData[3] = 1.0f;
	colorData[4] = 1.0f;
	colorData[5] = 1.0f;
	colorData[6] = 1.0f;
	colorData[7] = 1.0f;

	int flag = 0;
	flagData[0] = flag;
	flagData[1] = flag;

	SAMSON::displayCylinders(nCylinders, nPositions, indexData, positionData, radiusData, capData, colorData, flagData);

	delete[] indexData;
	delete[] radiusData;
	delete[] capData;
	delete[] colorData;
	delete[] flagData;
}


void displayHelper::displayArrow(SBPosition3 start, SBPosition3 end, const vector<float> groupColor) {

	SBPosition3 shaft = end - start;
	shaft *= 0.9;
	shaft += start;

	unsigned int nCylinders = 2;
	unsigned int nPositions = 2 * nCylinders;
	unsigned int* indexData = new unsigned int[2 * nCylinders];
	float *positionData = new float[3 * nPositions];
	float *radiusData = new float[2 * nCylinders];
	unsigned int *capData = new unsigned int[2 * nCylinders];
	float *colorData = new float[4 * 2 * nCylinders];
	unsigned int *flagData = new unsigned int[2 * nCylinders];

	//shaft
	positionData[0 * 3 + 0] = start.v[0].getValue();
	positionData[0 * 3 + 1] = start.v[1].getValue();
	positionData[0 * 3 + 2] = start.v[2].getValue();
	positionData[1 * 3 + 0] = shaft.v[0].getValue();
	positionData[1 * 3 + 1] = shaft.v[1].getValue();
	positionData[1 * 3 + 2] = shaft.v[2].getValue();
	//head
	positionData[2 * 3 + 0] = shaft.v[0].getValue();
	positionData[2 * 3 + 1] = shaft.v[1].getValue();
	positionData[2 * 3 + 2] = shaft.v[2].getValue();
	positionData[3 * 3 + 0] = end.v[0].getValue();
	positionData[3 * 3 + 1] = end.v[1].getValue();
	positionData[3 * 3 + 2] = end.v[2].getValue();

	for (int i = 0; i < 4; i++) {
		indexData[i] = i;
	}

	radiusData[0] = 50;
	radiusData[1] = 50;
	radiusData[2] = 100;
	radiusData[3] = 000;

	for (int i = 0; i < 4; i++) {
		capData[i] = 1;
	}

	for (int i = 0; i < 16; i++) {
		colorData[i] = groupColor.at(i % 4);
	}

	for (int i = 0; i < 4; i++) {
		flagData[i] = 0;
	}

	SAMSON::displayCylinders(nCylinders, nPositions, indexData, positionData, radiusData, capData, colorData, flagData);

	delete[] indexData;
	delete[] radiusData;
	delete[] capData;
	delete[] colorData;
	delete[] flagData;
}