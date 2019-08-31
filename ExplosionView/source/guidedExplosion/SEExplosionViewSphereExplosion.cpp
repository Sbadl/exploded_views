#include "guidedExplosion/SEExplosionViewSphereExplosion.hpp"

/*********************************************************************************************************************/
/* CONSTRUCTOR / DECONSTRUCTOR																						 */
/*********************************************************************************************************************/

SEExplosionViewSphereExplosion::SEExplosionViewSphereExplosion() : SEExplosionViewGuidedExplosion() {

}

SEExplosionViewSphereExplosion::~SEExplosionViewSphereExplosion() {


}

/*********************************************************************************************************************/
/* PUBLIC FUNCTIONS																									 */
/*********************************************************************************************************************/
SBPosition3 SEExplosionViewSphereExplosion::explodeChild(SBNode* parent, hTypes::nodeLengthPair childDistPair,
	double childExplosionValue, double explosionSliderAdjust, SBQuantity::length forceCoeff) {

	SBNodeIndexer atomIndexer;

	SBPosition3 centreCurrentChild;
	SBPosition3 pCentre;
	SBPosition3 parentCToChildC;
	SBPosition3 centreUpdated;
	//distance between the parent centre and the child centre. gets changed to move child
	SBQuantity::length distPCtoCC;
	SBPosition3 moveVector;
	//get child
	SBNode* child = childDistPair.first;

	//get atoms of child
	child->getNodes(atomIndexer, SBNode::IsType(SBNode::Atom));

	//get original distance to parent center
	distPCtoCC = childDistPair.second;

	//change radius: explosionValue * forceCoeff * structureRadiusCoeff * positionCoeff
	if (getUsePositionCoeff()) {
		distPCtoCC += SBQuantity::length(childExplosionValue / explosionSliderAdjust)				//0-99
			* forceCoeff.getValue()																	//0-3
			* SEExplosionViewGuidedExplosion::getStructureCoeff().at(parent).getValue()				//avgDistance/structureCoeffDiv
			* SEExplosionViewGuidedExplosion::getPositionCoeffs().at(parent).at(child).getValue();	//0-1
	}
	else {
		distPCtoCC += SBQuantity::length(childExplosionValue / explosionSliderAdjust)				//0-99
			* forceCoeff.getValue()																	//0-3
			* SEExplosionViewGuidedExplosion::getStructureCoeff().at(parent).getValue();			//avgDistance/structureCoeffDiv
	}

	//get currentCenter of current child
	centreCurrentChild = vectorOps::calcCentre(atomIndexer);

	//get vector from parent centre to child centre
	pCentre = SEExplosionViewGuidedExplosion::getParentCentre().at(parent);
	parentCToChildC = centreCurrentChild - pCentre;
	//get normalized version of parent centre to child centre
	parentCToChildC.normalizeWithUnits();

	//update radius of current centre
	centreUpdated = parentCToChildC * distPCtoCC.getValue() + pCentre;

	//all atoms of the current child need to be changed in the same way the center of the child did
	moveVector = centreUpdated - centreCurrentChild;
	vectorOps::moveModel(moveVector, atomIndexer);
	atomIndexer.clear();

	return centreUpdated;
}

