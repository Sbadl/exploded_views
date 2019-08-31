#include "guidedExplosion/SEExplosionViewGuidedExplosion.hpp"
#include "SEExplosionViewApp.hpp"

/*********************************************************************************************************************/
/* CONSTRUCTOR / DECONSTRUCTOR																						 */
/*********************************************************************************************************************/
SEExplosionViewGuidedExplosion::SEExplosionViewGuidedExplosion() :
	readyToExplode(false),
	explosionParent(0),
	explosionChild(0),
	explosionOrder(0) {

}

SEExplosionViewGuidedExplosion::~SEExplosionViewGuidedExplosion() {

	resetPreperationValues();

}

/*********************************************************************************************************************/
/* CLASS POINTER																									 */
/*********************************************************************************************************************/
SEExplosionViewApp* SEExplosionViewGuidedExplosion::getApp() const {
	SBProxy* classProxy = SAMSON::getProxy("SEExplosionViewApp");
	return static_cast<SEExplosionViewApp*>(SAMSON::getApp(classProxy->getUUID(), classProxy->getElementUUID()));
}

/*********************************************************************************************************************/
/* PUBLIC FUNCTIONS																							 */
/*********************************************************************************************************************/
void SEExplosionViewGuidedExplosion::guidedExplosion(const int explosionValue, const int explosionForce,
	const int explosionSliderMin, const int explosionSliderMax, double explosionSliderAdjust, const int easeIndex) {

	if (!readyToExplode) {
		prepareForExplosion();
	}

	//paths need to be updated on every change of explosion value
	explosionPathOfChildren.clear();
	int childPathIndex = 0;

	//calc forceCoeff
	SBQuantity::length forceCoeff(calcForceCoeff(explosionForce));

	SBNodeIndexer childIndexer;
	SBNodeIndexer childIndexerSelected;

	//vars for sequential explosion
	int childIndex = 1;
	int numberOfChildren = 0;
	double childExplosionValue = 0;
	int intervalLength = 0;

	//vars for peeling explosion
	double peelingGroupExplosionValue = 0;
	int intervalLengthPeeling = explosionSliderMax / numberOfPeelingGroups;

	SB_FOR(SBNode* parent, parentIndexer) {

		//get all children
		childIndexer = childIndexerOfParent.at(parent);

		//needs to be checked because if there is only 1 child nothing needs to explode
		if (childIndexer.size() > 1) {

			childIndexerSelected = fillChildIndexerSelected(childIndexer);

			//child indexer selected is empty if no child of parent is selected -> no explosion needs to be done
			if (!childIndexerSelected.empty()) {

				//sequential explosion
				if (explosionOrder == 1) {
					//need to be reset for each parent
					childIndex = 1;
					numberOfChildren = childIndexerSelected.size();
					intervalLength = explosionSliderMax / numberOfChildren;

					//runs through children in order of distances to parent center
					SB_FOR(hTypes::nodeLengthPair childDistPair, originalDistPCtoCCSorted.at(parent)) {

						//get child
						SBNode* child = childDistPair.first;

						//child only explodes if it is contained by childIndexerSelected
						if (childIndexerSelected.hasNode(child)) {

							childExplosionValue = calcExplosionValue(
								explosionValue, intervalLength, childIndex, numberOfChildren, explosionSliderMin, explosionSliderMax, easeIndex);

							childIndex++;

							SBPosition3 updatedChildPos = explodeChild(parent, childDistPair, childExplosionValue, explosionSliderAdjust, forceCoeff);
							if (saveArrows) {
								explosionPathOfChildren.insert(make_pair(childPathIndex, make_pair(updatedChildPos, originalPosOfChildCentreOfParent.at(parent).at(child))));
								childPathIndex++;
							}
						}
					}
				}
				//parallel and peeling explosion
				//parallel = peeling with 1 group
				else {
					//need to be reset for each parent
					for (int peelingGroupIndex = 1; peelingGroupIndex <= numberOfPeelingGroups; peelingGroupIndex++) {

						peelingGroupExplosionValue = calcExplosionValue(
							explosionValue, intervalLengthPeeling, peelingGroupIndex, numberOfPeelingGroups, explosionSliderMin, explosionSliderMax, easeIndex);

						//runs through children of this peeling Interval
						SB_FOR(hTypes::nodeLengthPair childDistPair, peelingGroupsOfChildrenOfParent.at(parent).at(peelingGroupIndex)) {
							//get child
							SBNode* child = childDistPair.first;
							//child only explodes if it is contained by childIndexerSelected
							if (childIndexerSelected.hasNode(child)) {
								SBPosition3 updatedChildPos = explodeChild(parent, childDistPair, peelingGroupExplosionValue, explosionSliderAdjust, forceCoeff);
								if (saveArrows) {
									explosionPathOfChildren.insert(make_pair(childPathIndex, make_pair(updatedChildPos, originalPosOfChildCentreOfParent.at(parent).at(child))));
									childPathIndex++;
								}
							}
						}
					}
				}
			}
		}
		//clear Indexer for each parent
		childIndexer.clear();
		childIndexerSelected.clear();
	}
}

/*********************************************************************************************************************/
/* GETTER/SETTER																									 */
/*********************************************************************************************************************/
void SEExplosionViewGuidedExplosion::setExplosionParent(int explosionP) {

	//should never be the case because of save in SEExplosionViewAppGUI.cpp
	if (explosionP > explosionChild) {
		SB_ERROR("Explosion Parent has to be higher than Explosion Child");
		return;
	}

	explosionParent = explosionP;
	resetPreperationValues();
}

void SEExplosionViewGuidedExplosion::setExplosionChild(int explosionC) {

	//should never be the case because of save in SEExplosionViewAppGUI.cpp

	if (explosionParent > explosionC) {
		SB_ERROR("Explosion Parent has to be higher than Explosion Child");
		return;
	}

	explosionChild = explosionC;
	resetPreperationValues();
}

void SEExplosionViewGuidedExplosion::setExplosionOrder(int order) {
	explosionOrder = order;
	resetPreperationValues();
}

void SEExplosionViewGuidedExplosion::setNumberOfPeelingGroups(int numberOfGroups) {
	numberOfPeelingGroups = numberOfGroups;
	resetPreperationValues();
}

void SEExplosionViewGuidedExplosion::setExplodeNearestChild(bool explNearestChild) {
	explodeNearestChild = explNearestChild;
	resetPreperationValues();
}

void SEExplosionViewGuidedExplosion::setUsePositionCoeff(bool usePosCoeff) {
	usePositionCoeff = usePosCoeff;
}

void SEExplosionViewGuidedExplosion::setSaveArrows(bool save) {
	saveArrows = save;
}

map<int, hTypes::positionPair> SEExplosionViewGuidedExplosion::getExplosionPathOfChildren() {
	return explosionPathOfChildren;
}

void SEExplosionViewGuidedExplosion::resetPreperationValues() {
	readyToExplode = false;
	parentIndexer.clear();
	childIndexerOfParent.clear();
	parentCentre.clear();
	originalDistPCtoCCSorted.clear();
	originalPosOfChildCentreOfParent.clear();
	structureCoeff.clear();
	positionCoeffs.clear();
	peelingGroupsOfChildrenOfParent.clear();
	explosionPathOfChildren.clear();
}

/*********************************************************************************************************************/
/* PROTECTED GETTER/SETTER																							 */
/*********************************************************************************************************************/
map<SBNode*, SBPosition3> SEExplosionViewGuidedExplosion::getParentCentre() {
	return parentCentre;
}

map<SBNode*, SBQuantity::length> SEExplosionViewGuidedExplosion::getStructureCoeff() {
	return structureCoeff;
}

map<SBNode*, map<SBNode*, SBQuantity::length>> SEExplosionViewGuidedExplosion::getPositionCoeffs() {
	return positionCoeffs;
}

map<SBNode*, SBPosition3> SEExplosionViewGuidedExplosion::getOriginalPosOfChildCentreOfParent(SBNode * parent) {
	return originalPosOfChildCentreOfParent.at(parent);
}

bool SEExplosionViewGuidedExplosion::getUsePositionCoeff() {
	return usePositionCoeff;
}

/*********************************************************************************************************************/
/* PROTECTED PREPAREATION																							 */
/*********************************************************************************************************************/
SBNodeIndexer SEExplosionViewGuidedExplosion::prepareParent(SBNode* parent) {

	//get children
	SBNodeIndexer childIndexer;
	childIndexer = loadIntoChildIndexer(parent);

	//calculate structure coefficient
	calcStructureCoeff(parent);

	/*
	calculate the min and max distance of child centres to parent centre
	save the orignal distance of child centre to parent centre
	ordered by distance
	*/
	SBQuantity::length length;
	SBQuantity::length minLength;	//length of nearest centre
	SBQuantity::length maxLength;  //length of furthest away centre
	SBNodeIndexer atomIndexer;
	vector<hTypes::nodeLengthPair> originalDistance;
	map<SBNode*, SBPosition3> centreOfChildren;

	SB_FOR(SBNode* child, childIndexer) {
		child->getNodes(atomIndexer, SBNode::IsType(SBNode::Atom));

		SBPosition3 childCentre = vectorOps::calcCentre(atomIndexer);

		//length from parent center to child center
		length = vectorOps::calcVectorLength(parentCentre.at(parent) - childCentre);
		originalDistance.push_back(make_pair(child, length));

		minLength = length < minLength ? length : minLength;
		maxLength = length > maxLength ? length : maxLength;

		centreOfChildren.insert(make_pair(child, childCentre));

		atomIndexer.clear();
	}

	//save original positions of children
	originalPosOfChildCentreOfParent.insert(make_pair(parent, centreOfChildren));

	//descending sort of distances
	sort(originalDistance.begin(), originalDistance.end(),
		[](const hTypes::nodeLengthPair& l, const hTypes::nodeLengthPair& r) {
		if (l.second != r.second) {
			return l.second > r.second;
		}
		return l.first < r.first;
	});

	//if we don't want to explode the nearest child to the parent center, we delete it
	//mostly used to stop explosion of scaffold
	if (!explodeNearestChild) {
		originalDistance.pop_back();
	}

	//data structure for sequential explosion
	originalDistPCtoCCSorted.insert(make_pair(parent, originalDistance));

	//peeling vars won't be needed for sequential explosion
	if (explosionOrder != 1) {
		preparePeeling(parent, minLength, maxLength);
	}

	//calculate Position Coeffiscients
	calcPositionCoeff(parent, minLength, maxLength);

	return childIndexer;
}

/*********************************************************************************************************************/
/* PRIVATE FUNCTIONS																								 */
/*********************************************************************************************************************/
double SEExplosionViewGuidedExplosion::calcExplosionValue(const int explosionValue, const int interval, const int index, const int numberOfElements,
	const int explosionSliderMin, const int explosionSliderMax, const int easeIndex) {
	double updatedExplosionValue = 0;

	if (interval * (index - 1) <= explosionValue && explosionValue < interval * index) {
		updatedExplosionValue = (explosionValue % interval) * numberOfElements;
		updatedExplosionValue = ease::handleEase(updatedExplosionValue, explosionSliderMin, explosionSliderMax, easeIndex);
	}
	else if (explosionValue >= interval * index) {
		updatedExplosionValue = explosionSliderMax;
	}

	return updatedExplosionValue;
}

SBNodeIndexer SEExplosionViewGuidedExplosion::fillChildIndexerSelected(SBNodeIndexer childIndexer) {
	SBNodeIndexer childIndexerSelected;

	if (checkAnyChildSelected()) {
		//only the selected children will explode
		SB_FOR(SBNode* child, childIndexer) {
			if (child->isSelected()) {
				childIndexerSelected.push_back(child);
			}
		}
	}
	//if no child is selected every child explodes
	else {
		childIndexerSelected = childIndexer;
	}
	return childIndexerSelected;
}

void SEExplosionViewGuidedExplosion::prepareForExplosion() {

	//get parents
	loadIntoParentIndexer();

	SBNodeIndexer atomIndexer;
	SBPosition3 centre;
	SB_FOR(SBNode* parent, parentIndexer) {
		parent->getNodes(atomIndexer, SBNode::IsType(SBNode::Atom));
		parentCentre.insert(make_pair(parent, vectorOps::calcCentre(atomIndexer)));
		childIndexerOfParent.insert(make_pair(parent, prepareParent(parent)));
		atomIndexer.clear();
	}

	readyToExplode = true;
}

void SEExplosionViewGuidedExplosion::preparePeeling(SBNode* parent, SBQuantity::length minLength, SBQuantity::length maxLength) {
	//set peeling Intervals for Peeling Explosion
	double peelingInterval = (maxLength - minLength).getValue() / numberOfPeelingGroups;
	map<int, set<hTypes::nodeLengthPair>> peelingGroupsOfChildren;
	set<hTypes::nodeLengthPair> peelingGroup;

	//children get put into their correspondig peeling interval
	int reversePeelingGroupIndex = numberOfPeelingGroups;
	for (int peelingGroupIndex = 1; peelingGroupIndex <= numberOfPeelingGroups; peelingGroupIndex++, reversePeelingGroupIndex--) {
		SB_FOR(hTypes::nodeLengthPair child, originalDistPCtoCCSorted.at(parent)) {
			if (((child.second - minLength).getValue() > (reversePeelingGroupIndex - 1) * peelingInterval) && ((child.second - minLength).getValue() <= reversePeelingGroupIndex * peelingInterval)) {
				peelingGroup.insert(child);
			}
		}
		peelingGroupsOfChildren.insert(make_pair(peelingGroupIndex, peelingGroup));
		peelingGroup.clear();
	}
	peelingGroupsOfChildrenOfParent.insert(make_pair(parent, peelingGroupsOfChildren));
}

void SEExplosionViewGuidedExplosion::calcStructureCoeff(SBNode* parent) {

	SBQuantity::length atomLength;
	SBQuantity::length avgDistanceToParent;
	SBNodeIndexer parentAtomIndexer;
	parent->getNodes(parentAtomIndexer, SBNode::IsType(SBNode::Atom));
	SB_FOR(SBNode* atom, parentAtomIndexer) {
		SBAtom* currentAtom = static_cast<SBAtom*>(atom);
		SBPosition3 position = currentAtom->getPosition();

		atomLength = vectorOps::calcVectorLength(parentCentre.at(parent) - position);

		avgDistanceToParent += atomLength;
	}
	avgDistanceToParent /= parentAtomIndexer.size();
	structureCoeff.insert(make_pair(parent, avgDistanceToParent / structureCoeffDiv));
}

void SEExplosionViewGuidedExplosion::calcPositionCoeff(SBNode * parent, SBQuantity::length minLength, SBQuantity::length maxLength)
{
	map<SBNode*, SBQuantity::length> intLenghtMap;
	SB_FOR(hTypes::nodeLengthPair child, originalDistPCtoCCSorted.at(parent)) {
		//normalize using length
		SBQuantity::length posCoeff = normalizeInterval(child.second, minLength, maxLength);

		intLenghtMap.insert(make_pair(child.first, posCoeff));
	}
	positionCoeffs.insert(make_pair(parent, intLenghtMap));
	intLenghtMap.clear();
}

SBQuantity::length SEExplosionViewGuidedExplosion::calcForceCoeff(int explosionForce) {
	if (explosionForce == fCoeffMid) {
		return vectorOps::Ql_1;
	}
	else if (explosionForce == fCoeffMin) {
		return vectorOps::Ql_0;
	}
	else if (explosionForce > fCoeffMid) {  //1-3
		return normalizeInterval(explosionForce, fCoeffMid, fCoeffMax) * 2 + vectorOps::Ql_1;
	}
	else { //0-1
		return normalizeInterval(explosionForce, fCoeffMin, fCoeffMid);
	}
}

SBQuantity::length SEExplosionViewGuidedExplosion::normalizeInterval(SBQuantity::length x, SBQuantity::length min, SBQuantity::length max) {
	return (x - min) / (max - min).getValue();
}

SBQuantity::length SEExplosionViewGuidedExplosion::normalizeInterval(int x, int min, int max) {
	return normalizeInterval(SBQuantity::length(x), SBQuantity::length(min), SBQuantity::length(max));
}

bool SEExplosionViewGuidedExplosion::checkAnyChildSelected() {

	SBNodeIndexer childIndexer;

	SB_FOR(SBNode* parent, parentIndexer) {
		childIndexer = childIndexerOfParent.at(parent);
		SB_FOR(SBNode* child, childIndexer) {
			if (child->isSelected()) {
				return true;
			}
		}
		childIndexer.clear();
	}

	return false;
}

/*********************************************************************************************************************/
/* INDEXER LOAD																										 */
/*********************************************************************************************************************/
bool SEExplosionViewGuidedExplosion::loadIntoParentIndexer() {
	switch (explosionParent) {
	case 0:
		SAMSON::getActiveDocument()->getNodes(parentIndexer, SBNode::IsType(SBNode::Document));
		break;
	case 1:
		SAMSON::getActiveDocument()->getNodes(parentIndexer, SBNode::IsType(SBNode::StructuralModel));
		break;
	case 2:
		SAMSON::getActiveDocument()->getNodes(parentIndexer, SBNode::IsType(SBNode::Molecule));
		break;
	case 3:
		SAMSON::getActiveDocument()->getNodes(parentIndexer, SBNode::IsType(SBNode::Chain));
		break;
	case 4:
		SAMSON::getActiveDocument()->getNodes(parentIndexer, SBNode::IsType(SBNode::Residue));
		break;
	case 5:
		SAMSON::getActiveDocument()->getNodes(parentIndexer, SBNode::IsType(SBNode::Backbone) || SBNode::IsType(SBNode::SideChain));
		break;
	default:
		return false;
	}

	if (parentIndexer.empty()) {
		SB_ERROR("Indexer is empty!");
	}

	return true;
}


SBNodeIndexer SEExplosionViewGuidedExplosion::loadIntoChildIndexer(SBNode* parent) {

	SBNodeIndexer indexer;

	switch (explosionChild) {
	case 0:
		parent->getNodes(indexer, SBNode::IsType(SBNode::StructuralModel));
		break;
	case 1:
		parent->getNodes(indexer, SBNode::IsType(SBNode::Molecule));
		break;
	case 2:
		parent->getNodes(indexer, SBNode::IsType(SBNode::Chain));
		break;
	case 3:
		parent->getNodes(indexer, SBNode::IsType(SBNode::Residue));
		break;
	case 4:
		parent->getNodes(indexer, SBNode::IsType(SBNode::Backbone) || SBNode::IsType(SBNode::SideChain));
		break;
	case 5:
		parent->getNodes(indexer, SBNode::IsType(SBNode::Atom));
		break;
	}

	if (indexer.empty()) {
		SB_ERROR("Indexer is empty!");
	}

	return indexer;
}