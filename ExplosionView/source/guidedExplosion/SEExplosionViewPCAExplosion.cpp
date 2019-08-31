#include "guidedExplosion/SEExplosionViewPCAExplosion.hpp"
#include "guidedExplosion/eig3.hpp"


/*********************************************************************************************************************/
/* CONSTRUCTOR / DECONSTRUCTOR																						 */
/*********************************************************************************************************************/
SEExplosionViewPCAExplosion::SEExplosionViewPCAExplosion() :
	SEExplosionViewGuidedExplosion() {

}

SEExplosionViewPCAExplosion::~SEExplosionViewPCAExplosion() {


}

/*********************************************************************************************************************/
/* PUBLIC FUNCTIONS																									 */
/*********************************************************************************************************************/
SBPosition3 SEExplosionViewPCAExplosion::explodeChild(SBNode* parent, hTypes::nodeLengthPair childDistPair,
	double childExplosionValue, double explosionSliderAdjust, SBQuantity::length forceCoeff) {

	SBNodeIndexer atomIndexer;

	SBPosition3 centreCurrentChild;
	SBPosition3 centreUpdated;
	SBPosition3  origPosOfChildCenter;
	//Value that defines the distance of the explosion
	SBQuantity::length explosionLength(0);
	SBPosition3 moveVector;
	//get child
	SBNode* child = childDistPair.first;

	//get atoms of child
	child->getNodes(atomIndexer, SBNode::IsType(SBNode::Atom));
	//get currentCenter of current child
	centreCurrentChild = vectorOps::calcCentre(atomIndexer);

	//change length: explosionValue * forceCoeff * structureRadiusCoeff * positionCoeff
	explosionLength += SBQuantity::length(childExplosionValue / explosionSliderAdjust)			//0-99
		* forceCoeff.getValue()																	//0-3
		* SEExplosionViewGuidedExplosion::getStructureCoeff().at(parent).getValue();			//avgDistance/structureCoeffDiv
	if (getUsePositionCoeff()) {
		//possible to just multiply, because explosionLength is 0 at the start
		explosionLength *= SEExplosionViewGuidedExplosion::getPositionCoeffs().at(parent).at(child).getValue();	//0-1
	}

	//get PCA vectors for this parent
	//index is get must be compile time const
	SBPosition3 eigVector;
	if (pcaComponent == 0) eigVector = get<0>(eigenVectorsOfParent.at(parent));
	else if (pcaComponent == 1) eigVector = get<1>(eigenVectorsOfParent.at(parent));
	else if (pcaComponent == 2) eigVector = get<2>(eigenVectorsOfParent.at(parent));

	//update position of current centre
	origPosOfChildCenter = getOriginalPosOfChildCentreOfParent(parent).at(child);
	//if the child center is on one side of the plane that is defined by the normalized eigenvector
	//it explodes towards this direction
	if (calcExplosionDirAlongVector(origPosOfChildCenter, eigVector)) {
		centreUpdated = origPosOfChildCenter - (eigVector * explosionLength.getValue());
	}
	//else it explodes in the oppsite direction
	else {
		centreUpdated = origPosOfChildCenter + (eigVector * explosionLength.getValue());
	}

	//all atoms of the current child need to be changed in the same way the center of the child did
	moveVector = centreUpdated - centreCurrentChild;
	vectorOps::moveModel(moveVector, atomIndexer);
	atomIndexer.clear();

	return centreUpdated;
}

void SEExplosionViewPCAExplosion::setPCAComponent(int component) {
	// -1 because in GUI range is 1-3 and in collection range is 0-2
	pcaComponent = component - 1;

	resetPreperationValues();
}

/*********************************************************************************************************************/
/* PROTECTED FUNCTIONS																								 */
/*********************************************************************************************************************/
SBNodeIndexer SEExplosionViewPCAExplosion::prepareParent(SBNode* parent) {
	SBNodeIndexer childIndexer = SEExplosionViewGuidedExplosion::prepareParent(parent);

	//prepareation is only necessary, if there is more than one child, because if there is only one child nothing explodes
	if (childIndexer.size() > 1) {
		SBNodeIndexer atomIndexer;
		///< PCA
		//calc mean
		parent->getNodes(atomIndexer, SBNode::IsType(SBNode::Atom));
		SBPosition3 mean = vectorOps::calcCentre(atomIndexer);
		atomIndexer.clear();

		//calc difference to mean
		map<SBNode*, vector<SBPosition3>> diffToMeanOfChildrenOfParent;
		calcDiffToMean(parent, childIndexer, mean, diffToMeanOfChildrenOfParent);

		//calc covariance matrix
		matrixOps::mat3x3 cov;
		calc3x3CovMatrix(diffToMeanOfChildrenOfParent.at(parent), cov);

		//calc Eigenvalues d and Eigenvectors V
		double A[3][3];
		double V[3][3];
		double d[3];
		matrixOps::convertToArray(cov, A);
		eigen::eigen_decomposition(A, V, d);

		//normalized eigenvectors
		hTypes::positionTriple eigenvectors = make_tuple(
			SBPosition3(SBQuantity::length(V[2][0]), SBQuantity::length(V[2][1]), SBQuantity::length(V[2][2])).normalizedVersionWithUnits(),
			SBPosition3(SBQuantity::length(V[1][0]), SBQuantity::length(V[1][1]), SBQuantity::length(V[1][2])).normalizedVersionWithUnits(),
			SBPosition3(SBQuantity::length(V[0][0]), SBQuantity::length(V[0][1]), SBQuantity::length(V[0][2])).normalizedVersionWithUnits()
		);

		eigenVectorsOfParent.insert(make_pair(parent, eigenvectors));
	}
	return childIndexer;
}

/*********************************************************************************************************************/
/* PRIVATE FUNCTIONS																								 */
/*********************************************************************************************************************/
void SEExplosionViewPCAExplosion::calcDiffToMean(SBNode * parent, SBNodeIndexer childIndexer, SBPosition3 mean,
	map<SBNode*, vector<SBPosition3>>& diffToMeanOfChildrenOfParent) {

	vector<SBPosition3> diffOfNodesToMean;

	SBNodeIndexer atomIndexer;
	SB_FOR(SBNode* child, childIndexer) {

		child->getNodes(atomIndexer, SBNode::IsType(SBNode::Atom));
		SBPosition3 childCentre = vectorOps::calcCentre(atomIndexer);
		diffOfNodesToMean.push_back(childCentre - mean);
		atomIndexer.clear();
	}

	diffToMeanOfChildrenOfParent.insert(make_pair(parent, diffOfNodesToMean));
}

void SEExplosionViewPCAExplosion::calc3x3CovMatrix(vector<SBPosition3> diffToMean, matrixOps::mat3x3& cov) {

	if (size(diffToMean) > 0) {
		double s = 1 / (double)(size(diffToMean) - 1);

		SB_FOR(SBPosition3 pos, diffToMean) {
			cov = cov + matrixOps::posXpos(pos, pos);
		}

		cov = cov*s;
	}
}

bool SEExplosionViewPCAExplosion::calcExplosionDirAlongVector(SBPosition3 originalPosition, SBPosition3 normMoveVector) {

	// | = dot product
	SBQuantityProduct2<SBQuantity::length, SBQuantity::length>::Type dot = (originalPosition | normMoveVector);

	if (dot.getValue() <= 0) {
		return true;
	}
	else {
		return false;
	}
}


