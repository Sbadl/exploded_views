#include "SEExplosionViewApp.hpp"
#include "editor/SEExplosionViewEditor.hpp"
#include "SEExplosionViewAppFunctions.hpp"

/*********************************************************************************************************************/
/* CONSTRUCTOR / DECONSTRUCTOR																						 */
/*********************************************************************************************************************/
SEExplosionViewApp::SEExplosionViewApp() :
	start_explosionSliderMax(99),
	explosionSliderAdjust(1),
	currentExplosionGroup(0),
	realDocumentEvent(true) {

	setGUI(new SEExplosionViewAppGUI(this));
	getGUI()->loadDefaultSettings();

	//create SphereExplosion Handler
	sphereExplosionHandler = new SEExplosionViewSphereExplosion();
	//create PCAExplosion Handler
	pcaExplosionHandler = new SEExplosionViewPCAExplosion();
	//create PathExplosion Handler
	pathExplosionHandler = new SEExplosionViewPathExplosion();

	distributeGUISettings();

	//set change listener on Layer
	SAMSON::getActiveLayer()->connectDocumentSignalToSlot(this, SB_SLOT(&SEExplosionViewApp::onDocumentEvent));
}

SEExplosionViewApp::~SEExplosionViewApp() {

	//release change listener on Layer
	SAMSON::getActiveLayer()->disconnectDocumentSignalFromSlot(this, SB_SLOT(&SEExplosionViewApp::onDocumentEvent));

	getGUI()->saveDefaultSettings();
	delete getGUI();
	delete sphereExplosionHandler;
	delete pcaExplosionHandler;
	delete pathExplosionHandler;
	delete originalModelPosition;

}

/*********************************************************************************************************************/
/* CLASS POINTER																									 */
/*********************************************************************************************************************/
SEExplosionViewAppGUI* SEExplosionViewApp::getGUI() const { return static_cast<SEExplosionViewAppGUI*>(SBDApp::getGUI()); }

SEExplosionViewEditor* SEExplosionViewApp::getEditor() {
	SBProxy* classProxy = SAMSON::getProxy("SEExplosionViewEditor");
	return static_cast<SEExplosionViewEditor*>(SAMSON::getEditor(classProxy->getUUID(), classProxy->getElementUUID()));
}

/*********************************************************************************************************************/
/* PUBLIC FUNCTIONS	GUIDED EXPLOSION																				 */
/*********************************************************************************************************************/
void SEExplosionViewApp::guidedExplosion(const int explosionValue, const int explosionForce, const int explosionSliderMin, const int explosionSliderMax) {

	if (sphericalExplosion) {
		sphereExplosionHandler->guidedExplosion(explosionValue, explosionForce, explosionSliderMin, explosionSliderMax, explosionSliderAdjust, easeIndex);
	}
	else if (pcaExplosion) {
		pcaExplosionHandler->guidedExplosion(explosionValue, explosionForce, explosionSliderMin, explosionSliderMax, explosionSliderAdjust, easeIndex);
	}
}

void SEExplosionViewApp::setSphericalExplosion(bool sphericalChecked) {
	sphericalExplosion = sphericalChecked;
	pcaExplosion = !sphericalChecked;

	sphereExplosionHandler->resetPreperationValues();
}

void SEExplosionViewApp::setPCAExplosion(bool pcaChecked) {
	pcaExplosion = pcaChecked;
	sphericalExplosion = !pcaChecked;

	pcaExplosionHandler->resetPreperationValues();
}

void SEExplosionViewApp::setExplosionParent(int explosionP) {
	sphereExplosionHandler->setExplosionParent(explosionP);
	pcaExplosionHandler->setExplosionParent(explosionP);
}

void SEExplosionViewApp::setExplosionChild(int explosionC) {
	sphereExplosionHandler->setExplosionChild(explosionC);
	pcaExplosionHandler->setExplosionChild(explosionC);
}

void SEExplosionViewApp::setExplosionOrder(int order) {
	sphereExplosionHandler->setExplosionOrder(order);
	pcaExplosionHandler->setExplosionOrder(order);
}

void SEExplosionViewApp::setNumberOfPeelingGroups(int numberOfGroups) {
	sphereExplosionHandler->setNumberOfPeelingGroups(numberOfGroups);
	pcaExplosionHandler->setNumberOfPeelingGroups(numberOfGroups);
}

void SEExplosionViewApp::setPCAComponent(int component) {
	pcaExplosionHandler->setPCAComponent(component);
}

void SEExplosionViewApp::setEase(int eIndex) {
	easeIndex = eIndex;
}

map<int, hTypes::positionPair> SEExplosionViewApp::getExplosionPathOfChildren() {

	if (sphericalExplosion) {
		return sphereExplosionHandler->getExplosionPathOfChildren();
	}
	else if (pcaExplosion) {
		return pcaExplosionHandler->getExplosionPathOfChildren();
	}
}

void SEExplosionViewApp::setExplodeNearestChild(bool explNearestChild) {
	sphereExplosionHandler->setExplodeNearestChild(explNearestChild);
	pcaExplosionHandler->setExplodeNearestChild(explNearestChild);
}

void SEExplosionViewApp::setUsePosCoeff(bool usePosCoeff) {
	sphereExplosionHandler->setUsePositionCoeff(usePosCoeff);
	pcaExplosionHandler->setUsePositionCoeff(usePosCoeff);
}

void SEExplosionViewApp::displayGuidedArrows(bool display) {

	setExplosionEditor();

	sphereExplosionHandler->setSaveArrows(display);
	pcaExplosionHandler->setSaveArrows(display);

	getEditor()->setDrawGuidedArrows(display);
}

void SEExplosionViewApp::resetModelPosition() {

	resetPositionOfIndexer(modelAtomIndexer);

	modelAtomIndexer.clear();
	sphereExplosionHandler->resetPreperationValues();
	pcaExplosionHandler->resetPreperationValues();
	setUpModel();
}

/*********************************************************************************************************************/
/* PUBLIC FUNCTIONS	INTERACTIVE EXPLOSION																			 */
/*********************************************************************************************************************/
void SEExplosionViewApp::pathExplode(int explosionValue, int explosionSliderMin, int explosionSliderMax) {
	pathExplosionHandler->pathExplosion(explosionValue, explosionSliderMin, explosionSliderMax, easeIndex);
}

int SEExplosionViewApp::addExplosionGroup() {
	return pathExplosionHandler->addExplosionGroup();
}

void SEExplosionViewApp::showCurrentGroup(bool show, int groupIndex) {
	pathExplosionHandler->showCurrentGroup(show, groupIndex);
}

void SEExplosionViewApp::displayPathArrows(bool display) {
	setExplosionEditor();
	getEditor()->setDrawPathArrows(display);
}

void SEExplosionViewApp::deselectAllGroups() {
	pathExplosionHandler->deselectAllGroups();
}

bool SEExplosionViewApp::deleteExplosionGroup(int groupIndex) {
	//reset Position of Group
	resetPositionOfIndexer(pathExplosionHandler->getExplosionGroup(groupIndex));
	//delete group
	return pathExplosionHandler->deleteExplosionGroup(groupIndex);
}

void SEExplosionViewApp::startDrawingOfExplosionPath(int group, int tableRowCount) {

	// if group == -1 there is no group to which we can add a path
	if (group != -1) {
		currentExplosionGroup = group;

		//get start position of arrow
		vector<hTypes::atomsLengthTuple> explosionPathsOfGroup = pathExplosionHandler->getExplosionPathsOfGroups().at(group);
		SBPosition3 start;

		//only as many paths as the table has rows can be added
		if (explosionPathsOfGroup.size() != tableRowCount) {
			if (explosionPathsOfGroup.empty()) {
				//if there is no path for this group the center of the group is the starting point
				SBNodeIndexer nodesOfGroup = pathExplosionHandler->getExplosionGroup(group);
				start = vectorOps::calcCentre(nodesOfGroup);
			}
			else {
				//else the end position of the last path is the start position
				start = get<1>(explosionPathsOfGroup.back())->getPosition();
			}

			setExplosionEditor();
			getEditor()->startDrawingArrow(group, start);
		}
	}
}

void SEExplosionViewApp::finishDrawingOfExplosionPath(hTypes::positionsLengthTuple positions) {

	int pathIndex = pathExplosionHandler->addExplosionPathToGroup(currentExplosionGroup, positions);
	getGUI()->addExplosionPathEnd(pathIndex, pathExplosionHandler->getExplosionPathsOfGroups().at(currentExplosionGroup).back());

	currentExplosionGroup = 0;
}

void SEExplosionViewApp::cancelAddExplosionPath() {
	getEditor()->cancelAddExplosionPath();
}

map<int, vector<hTypes::atomsLengthTuple>> SEExplosionViewApp::getExplosionPathsOfGroups() {
	return pathExplosionHandler->getExplosionPathsOfGroups();
}

int SEExplosionViewApp::deleteLastExplosionPathOfGroup(int groupIndex) {
	return pathExplosionHandler->deleteLastExplosionPathOfGroup(groupIndex);
}

void SEExplosionViewApp::resetPathExplosion() {
	pathExplosionHandler->clearAllVars();
}


/*********************************************************************************************************************/
/* GETTER / SETTER																									 */
/*********************************************************************************************************************/
void SEExplosionViewApp::setRealDocumentEvent(bool real) {
	realDocumentEvent = real;
}

/*********************************************************************************************************************/
/* PRIVATE FUNCTIONS																								 */
/*********************************************************************************************************************/
void SEExplosionViewApp::distributeGUISettings(){

	//App
	easeIndex = getGUI()->getEaseIndex();
	sphericalExplosion = getGUI()->getSphericalExplosion();
	pcaExplosion = !sphericalExplosion;

	//Strucural Explosion Handlers
	sphereExplosionHandler->setNumberOfPeelingGroups(getGUI()->getNumberOfPeelingGroups());
	pcaExplosionHandler->setNumberOfPeelingGroups(getGUI()->getNumberOfPeelingGroups());

	sphereExplosionHandler->setExplodeNearestChild(getGUI()->getExplodeNearestChild());
	pcaExplosionHandler->setExplodeNearestChild(getGUI()->getExplodeNearestChild());

	sphereExplosionHandler->setUsePositionCoeff(getGUI()->getUsePositionCoeff());
	pcaExplosionHandler->setUsePositionCoeff(getGUI()->getUsePositionCoeff());

	sphereExplosionHandler->setSaveArrows(getGUI()->getDisplayGuidedArrows());
	pcaExplosionHandler->setSaveArrows(getGUI()->getDisplayGuidedArrows());

	pcaExplosionHandler->setPCAComponent(getGUI()->getPCAComponent());

}


void SEExplosionViewApp::onDocumentEvent(SBDocumentEvent* event) {

	if (realDocumentEvent) {
		SBNodeIndexer atomIndexer;

		if (event->getType() == SBDDocumentEvent::StructuralModelAdded || event->getType() == SBDDocumentEvent::StructuralModelRemoved) {
			modelAtomIndexer.clear();
			sphereExplosionHandler->resetPreperationValues();
			pcaExplosionHandler->resetPreperationValues();
			setUpModel();

			getGUI()->resetGUI();

			SAMSON::getActiveDocument()->getNodes(atomIndexer, SBNode::IsType(SBNode::Atom));
			int max = atomIndexer.size();
			if (max != 0) {
				getGUI()->setExplosionRangeSlidersMax(max);
				explosionSliderAdjust = max / start_explosionSliderMax;
			}

			pathExplosionHandler->clearAllVars();
		}
	}
}

void SEExplosionViewApp::setExplosionEditor() {

	SAMSON::setActiveEditor(getEditor());
}

void SEExplosionViewApp::setUpModel() {

	//get all atoms to calculate centre of model
	SAMSON::getActiveDocument()->getNodes(modelAtomIndexer, SBNode::IsType(SBNode::Atom));

	if (modelAtomIndexer.empty()) {
		SB_INFORMATION("Nothing to set up");
		return;
	}

	//move model and camera to origin
	vectorOps::moveModel(-vectorOps::calcCentre(modelAtomIndexer), modelAtomIndexer);
	SAMSON::getActiveCamera()->center();


	//save position in origin
	originalModelPosition = new SBConformation("start", modelAtomIndexer);

}

void SEExplosionViewApp::resetPositionOfIndexer(SBNodeIndexer nodes) {
	SB_FOR(SBNode* node, nodes) {
		SBAtom* currentAtom = static_cast<SBAtom*>(node);
		SBPosition3 p;
		originalModelPosition->getPosition(currentAtom, p);
		currentAtom->setPosition(p);
	}
}
