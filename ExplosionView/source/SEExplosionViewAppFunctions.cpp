#include "SEExplosionViewAppFunctions.hpp"

/*********************************************************************************************************************/
/* EASE FUNCTIONS																									 */
/*********************************************************************************************************************/
double ease::cubicEaseIn(double t, int b, int c, int d) {
	t /= d;
	return c*t*t*t + b;
}

double ease::cubicEaseOut(double t, int b, int c, int d) {
	t /= d;
	t--;
	return c*(t*t*t + 1) + b;
}

double ease::cubicEaseInOut(double t, int b, int c, int d) {
	t /= (double)d / 2;
	if (t < 1) {
		return c / 2 * t*t*t + b;
	}
	t -= 2;
	return c / 2 * (t*t*t + 2) + b;

}

double ease::handleEase(double explosionValue, int intervalMin, int intervalMax, int easeIndex) {

	double eValue = 0;

	switch (easeIndex) {
	case 0:
		//Linear Explosion
		eValue = explosionValue;
		break;
	case 1:
		//Cubic Ease In Explosion
		eValue = ease::cubicEaseIn(explosionValue / intervalMax, intervalMin, intervalMax - intervalMin, 1);
		break;
	case 2:
		//Cubic Ease Out Explosion
		eValue = ease::cubicEaseOut(explosionValue / intervalMax, intervalMin, intervalMax - intervalMin, 1);
		break;
	case 3:
		//Cubic Ease In Out Explosion
		eValue = ease::cubicEaseInOut(explosionValue / intervalMax, intervalMin, intervalMax - intervalMin, 1);
		break;
	default:
		//Linear Explosion is default
		eValue = explosionValue;
		break;
	}
	return eValue;
}



/*********************************************************************************************************************/
/* VECTOR OEPRATIONS																								 */
/*********************************************************************************************************************/
double vectorOps::calcLengthBetweenTwoVectors(SBPosition3 a, SBPosition3 b) {

	SBPosition3 vec = b - a;

	return sqrt(pow<2>(vec.v[0]) + pow<2>(vec.v[1]) + pow<2>(vec.v[2])).getValue();

}

SBPosition3 vectorOps::calcCentre(SBNodeIndexer nodeIndexer) {

	SBPosition3 centre(Ql_0, Ql_0, Ql_0);

	SB_FOR(SBNode* node, nodeIndexer) {
		SBAtom* currentAtom = static_cast<SBAtom*>(node);
		SBPosition3 position = currentAtom->getPosition();

		centre.v[0] += position.v[0];
		centre.v[1] += position.v[1];
		centre.v[2] += position.v[2];
	}

	centre.v[0] /= nodeIndexer.size();
	centre.v[1] /= nodeIndexer.size();
	centre.v[2] /= nodeIndexer.size();

	return centre;
}

SBQuantity::length vectorOps::calcVectorLength(SBPosition3 p) {
	return sqrt(pow<2>(p.v[0]) + pow<2>(p.v[1]) + pow<2>(p.v[2]));
}


void vectorOps::moveModel(SBPosition3 moveVector, SBNodeIndexer indexer) {

	SB_FOR(SBNode* node, indexer) {
		SBAtom* currentAtom = static_cast<SBAtom*>(node);
		currentAtom->setPosition(currentAtom->getPosition() + moveVector);
	}

}


/*********************************************************************************************************************/
/* MATRIX OPERATIONS																								 */
/*********************************************************************************************************************/
matrixOps::mat3x3 matrixOps::posXpos(SBPosition3 a, SBPosition3 b)
{
	return mat3x3(
		a.v[1] * b.v[1].getValue(), a.v[1] * b.v[2].getValue(), a.v[1] * b.v[3].getValue(),
		a.v[2] * b.v[1].getValue(), a.v[2] * b.v[2].getValue(), a.v[2] * b.v[3].getValue(),
		a.v[3] * b.v[1].getValue(), a.v[3] * b.v[2].getValue(), a.v[3] * b.v[3].getValue());
}

void matrixOps::convertToArray(mat3x3 in, double out[3][3]) {
	out[0][0] = in.a1.getValue();
	out[0][1] = in.b1.getValue();
	out[0][2] = in.c1.getValue();

	out[1][0] = in.a2.getValue();
	out[1][1] = in.b2.getValue();
	out[1][2] = in.c2.getValue();

	out[2][0] = in.a3.getValue();
	out[2][1] = in.b3.getValue();
	out[2][2] = in.c3.getValue();
}

/*********************************************************************************************************************/
/* DEBUGGING																										 */
/*********************************************************************************************************************/
//for debugging only 
//renders a golden atom at the given position
void debug::debugPosition(SBPosition3 p) {

	SBMStructuralModel* structuralModel = new SBMStructuralModel();
	structuralModel->setName("Debug Position");

	SBAtom* testAtom = new SBAtom(SBElement::Gold, p);
	structuralModel->getStructuralRoot()->addChild(testAtom);

	SAMSON::beginHolding("Debug Position");

	SAMSON::hold(structuralModel);
	structuralModel->create();

	SAMSON::getActiveLayer()->addChild(structuralModel);

	//SBPosition3 pos = testAtom->getPosition();

	SAMSON::endHolding();
}
