#include "interactiveExplosion/SEExplosionViewPathExplosion.hpp"
#include "SEExplosionViewApp.hpp"
#include <sstream>
#include <string>

/*********************************************************************************************************************/
/* CONSTRUCTOR / DECONSTRUCTOR																						 */
/*********************************************************************************************************************/
SEExplosionViewPathExplosion::SEExplosionViewPathExplosion() :
	currentNumberOfGroups(1) {
}

SEExplosionViewPathExplosion::~SEExplosionViewPathExplosion() {
	clearAllVars();
}

/*********************************************************************************************************************/
/* CLASS POINTER																									 */
/*********************************************************************************************************************/
SEExplosionViewApp * SEExplosionViewPathExplosion::getApp() const {
	SBProxy* classProxy = SAMSON::getProxy("SEExplosionViewApp");
	return static_cast<SEExplosionViewApp*>(SAMSON::getApp(classProxy->getUUID(), classProxy->getElementUUID()));

}

/*********************************************************************************************************************/
/* PUBLIC FUNCTIONS																								 */
/*********************************************************************************************************************/
void SEExplosionViewPathExplosion::pathExplosion(int explosionValue, int explosionSliderMin, int explosionSliderMax, int easeIndex) {

	// eGroup.first = GroupIndex, eGroup.second = Positions+Length
	for (auto const& eGroup : explosionPathsOfGroups) {

		// only if no group or this group is selected we perform explosion
		if ((!anyGroupSelected()) || groupSelected(eGroup.first)) {
			//if the group has no paths, no explosion needs to be performed
			if (!eGroup.second.empty()) {

				double easeExplosionValue = ease::handleEase(explosionValue, explosionSliderMin, explosionSliderMax, easeIndex);

				// percentage on slider (0-1)
				double explosionPerc = easeExplosionValue / (double)explosionSliderMax;

				//get total length of explosion path
				SBQuantity::length totalLength(0);
				SB_FOR(hTypes::atomsLengthTuple nL, eGroup.second) {
					totalLength += get<2>(nL);
				}

				// !get current point on total path!
				// get current length of total path
				const SBQuantity::length currentLength = SBQuantity::length(totalLength.getValue() * explosionPerc);
				// determine on which path we are
				SBQuantity::length neededLengthOfLastPath = currentLength;
				hTypes::atomsLengthTuple lastPath;
				SB_FOR(hTypes::atomsLengthTuple pL, eGroup.second) {
					if (pL != eGroup.second.back()) {
						neededLengthOfLastPath -= get<2>(pL);
						// if the end of the path is further away as we want to go
						if (neededLengthOfLastPath.getValue() <= 0) {
							// we get the length that we need to go on this path
							neededLengthOfLastPath += get<2>(pL);
							lastPath = pL;
							break;
						}
					}
					else {
						// remaining length is excactly the needed length
						lastPath = pL;
						//no break necessary, because it is last element anyway
					}
				}
				// calc how many percent of the last path length we need to go (0-1)
				double percOfLengthOfLastPath = (neededLengthOfLastPath / get<2>(lastPath)).getValue();
				// get last path vector
				SBPosition3 destinationPoint = get<1>(lastPath)->getPosition() - get<0>(lastPath)->getPosition();
				// resize last path vector to correct length
				destinationPoint *= percOfLengthOfLastPath;
				// translate last path vector back to correct position
				// destinationPoint is now the point where we want to move the center of the group
				destinationPoint += get<0>(lastPath)->getPosition();

				// !calculate move vector!
				// original center of Group is the start of the first path
				SBPosition3 currentCenter = vectorOps::calcCentre(explosionGroups.at(eGroup.first));
				SBPosition3 moveVector = destinationPoint - currentCenter;
				vectorOps::moveModel(moveVector, explosionGroups.at(eGroup.first));
			}
		}
	}

}

int SEExplosionViewPathExplosion::addExplosionGroup() {
	SBNodeIndexer group;
	SAMSON::getActiveDocument()->getNodes(group, SBNode::IsType(SBNode::Atom) && SBNode::IsSelected());

	if (!group.empty()) {
		bool incompatibleNodes = false;

		//if any node of the current selection is already inside an explosion group, the new group cannot be added
		SB_FOR(SBNode* node, group) {
			for (auto const& eGroup : explosionGroups) {
				if (eGroup.second.hasNode(node)) {
					incompatibleNodes = true;
				}
			}
		}
		if (!incompatibleNodes) {
			explosionGroups.insert(make_pair(currentNumberOfGroups, group));

			addPathModel();

			explosionPathsOfGroups.insert(make_pair(currentNumberOfGroups, vector<hTypes::atomsLengthTuple>()));
			return currentNumberOfGroups++;
		}
	}

	//if the selected group cannot be added -1 is returned
	return -1;
}

bool SEExplosionViewPathExplosion::deleteExplosionGroup(int groupIndex) {
	//deletes explosion group and its paths if it exists
	if (explosionGroups.count(groupIndex)) {
		explosionGroups.erase(groupIndex);
		getApp()->setRealDocumentEvent(false);
		pathModelOfGroup.at(groupIndex)->erase();
		getApp()->setRealDocumentEvent(true);
		pathModelOfGroup.erase(groupIndex);
		explosionPathsOfGroups.erase(groupIndex);
		return true;
	}
	return false;
}

void SEExplosionViewPathExplosion::showCurrentGroup(bool show, int groupIndex) {

	// selection is show if it is contained in explosionGroups
	if (explosionGroups.count(groupIndex)) {
		SB_FOR(SBNode* node, explosionGroups.at(groupIndex)) {
			node->setSelectionFlag(show);
		}
	}
}

void SEExplosionViewPathExplosion::deselectAllGroups() {
	SBNodeIndexer nodes;
	SAMSON::getActiveDocument()->getNodes(nodes, SBNode::IsType(SBNode::Atom) && SBNode::IsSelected());

	SB_FOR(SBNode* node, nodes) {
		node->setSelectionFlag(false);
	}
}

int SEExplosionViewPathExplosion::addExplosionPathToGroup(int groupIndex, hTypes::positionsLengthTuple posL) {

	if (explosionGroups.count(groupIndex)) {
		SBAtom* startAtom;
		SBAtom* endAtom;

		//if there are no paths for this group we need to create 2 new atoms
		if (explosionPathsOfGroups.at(groupIndex).empty()) {

			//TODO: exception is thrown when naming atom
			startAtom = new SBAtom(SBElement::Gold, get<0>(posL));
			endAtom = new SBAtom(SBElement::Gold, get<1>(posL));

			startAtom->create();
			endAtom->create();

			pathModelOfGroup.at(groupIndex)->getStructuralRoot()->addChild(startAtom);
			pathModelOfGroup.at(groupIndex)->getStructuralRoot()->addChild(endAtom);
		}
		//else we take the end atoms of the last path as start atom for the current path
		else {

			startAtom = get<1>(explosionPathsOfGroups.at(groupIndex).back());
			endAtom = new SBAtom(SBElement::Gold, get<1>(posL));

			endAtom->create();

			pathModelOfGroup.at(groupIndex)->getStructuralRoot()->addChild(endAtom);
		}
		startAtom->setVisibilityFlag(false);
		endAtom->setVisibilityFlag(false);
		hTypes::atomsLengthTuple alt = make_tuple(startAtom, endAtom, get<2>(posL));

		explosionPathsOfGroups.at(groupIndex).push_back(alt);

		return explosionPathsOfGroups.at(groupIndex).size() - 1;
	}
	else {
		//should never happen because we only can work on existing groups
		return -1;
	}
}

int SEExplosionViewPathExplosion::deleteLastExplosionPathOfGroup(int groupIndex) {

	if (explosionGroups.count(groupIndex)) {
		if (!explosionPathsOfGroups.at(groupIndex).empty()) {
			hTypes::atomsLengthTuple toDelete = explosionPathsOfGroups.at(groupIndex).back();
			explosionPathsOfGroups.at(groupIndex).pop_back();

			pathModelOfGroup.at(groupIndex)->getStructuralRoot()->removeChild(get<1>(toDelete));

			int numberOfPaths = explosionPathsOfGroups.at(groupIndex).size();
			// if the deleted path was the last one we also delete the start atom of the path
			if (numberOfPaths == 0) {
				pathModelOfGroup.at(groupIndex)->getStructuralRoot()->removeChild(get<0>(toDelete));
			}
			return numberOfPaths;
		}
	}
	return -1;
}

/*********************************************************************************************************************/
/* GETTER/SETTER																									 */
/*********************************************************************************************************************/
SBNodeIndexer SEExplosionViewPathExplosion::getExplosionGroup(int groupIndex) {

	if (explosionGroups.count(groupIndex)) {
		return explosionGroups.at(groupIndex);
	}
	else {
		return SBNodeIndexer();
	}
}

map<int, vector<hTypes::atomsLengthTuple>> SEExplosionViewPathExplosion::getExplosionPathsOfGroups() {
	return explosionPathsOfGroups;
}

void SEExplosionViewPathExplosion::clearAllVars() {
	currentNumberOfGroups = 1;
	for (auto const& pathModel : pathModelOfGroup) {
		deleteExplosionGroup(pathModel.first);
	}
	pathModelOfGroup.clear();
	explosionGroups.clear();
	explosionPathsOfGroups.clear();
}

/*********************************************************************************************************************/
/* PRIVATE FUNCTIONS																								 */
/*********************************************************************************************************************/
void SEExplosionViewPathExplosion::addPathModel() {
	getApp()->setRealDocumentEvent(false);
	pathModelOfGroup.insert(make_pair(currentNumberOfGroups, new SBMStructuralModel()));

	stringstream ss;
	ss << "Paths Of Group: " << currentNumberOfGroups;
	string name = ss.str();

	pathModelOfGroup.at(currentNumberOfGroups)->setName(name);
	pathModelOfGroup.at(currentNumberOfGroups)->create();

	SAMSON::getActiveLayer()->addChild(pathModelOfGroup.at(currentNumberOfGroups));

	getApp()->setRealDocumentEvent(true);
}

bool SEExplosionViewPathExplosion::anyGroupSelected() {
	for (auto const& eGroup : explosionGroups) {
		if (groupSelected(eGroup.first)) {
			return true;
		}
	}
	return false;
}

bool SEExplosionViewPathExplosion::groupSelected(int groupIndex) {

	SBNodeIndexer groupNodeIndexer = explosionGroups.at(groupIndex);
	SB_FOR(SBNode* node, groupNodeIndexer) {
		if (node->isSelected()) {
			return true;
		}
	}
	return false;
}