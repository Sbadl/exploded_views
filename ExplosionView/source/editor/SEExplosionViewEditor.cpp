#include "editor/SEExplosionViewEditor.hpp"
#include "SAMSON.hpp"

/*********************************************************************************************************************/
/* CONSTRUCTOR / DECONSTRUCTOR																						 */
/*********************************************************************************************************************/
SEExplosionViewEditor::SEExplosionViewEditor() :
	createArrow(false)
{

	// SAMSON Element generator pro tip: this default constructor is called when unserializing the node, so it should perform all default initializations.

	drawGuidedArrows = getApp()->getGUI()->getDisplayGuidedArrows();
	drawPathArrows = getApp()->getGUI()->getDisplayPathArrows();

	initializeGroupColors();
}

SEExplosionViewEditor::~SEExplosionViewEditor() {

	// SAMSON Element generator pro tip: disconnect from signals you might have connected to.

}

/*********************************************************************************************************************/
/* CLASS POINTER																									 */
/*********************************************************************************************************************/
SEExplosionViewApp* SEExplosionViewEditor::getApp() const {

	SBProxy* classProxy = SAMSON::getProxy("SEExplosionViewApp");
	return static_cast<SEExplosionViewApp*>(SAMSON::getApp(classProxy->getUUID(), classProxy->getElementUUID()));

}

/*********************************************************************************************************************/
/* IDENTITY																											 */
/*********************************************************************************************************************/
SBCContainerUUID SEExplosionViewEditor::getUUID() const { return SBCContainerUUID("0C05FC34-A966-18E9-85D9-BC18C1A39C74"); }

QString SEExplosionViewEditor::getName() const {

	// SAMSON Element generator pro tip: this name should not be changed

	return "SEExplosionViewEditor";

}

QString SEExplosionViewEditor::getText() const {

	// SAMSON Element generator pro tip: modify this function to return a user-friendly string that will be displayed in menus

	return QObject::tr("SEExplosionViewEditor");

}

QPixmap SEExplosionViewEditor::getLogo() const {

	// SAMSON Element generator pro tip: this icon will be visible in the GUI title bar. 
	// Modify it to better reflect the purpose of your editor.

	return QPixmap(QString::fromStdString(SB_ELEMENT_PATH + "/Resource/Icons/SEExplosionViewEditorIcon.png"));

}

QKeySequence SEExplosionViewEditor::getShortcut() const {

	// SAMSON Element generator pro tip: modify this function to associate a tentative shortcut to your editor

	return QKeySequence("");

}

QString SEExplosionViewEditor::getToolTip() const {

	// SAMSON Element generator pro tip: modify this function to have your editor display a tool tip in the SAMSON GUI when the mouse hovers the editor's icon

	return QObject::tr("Explosion View Editor");

}

/*********************************************************************************************************************/
/* EDITING																											 */
/*********************************************************************************************************************/
void SEExplosionViewEditor::beginEditing() {

	// SAMSON Element generator pro tip: SAMSON calls this function when your editor becomes active. 
	// Implement this function if you need to prepare some data structures in order to be able to handle GUI or SAMSON events.

}

void SEExplosionViewEditor::endEditing() {

	// SAMSON Element generator pro tip: SAMSON calls this function immediately before your editor becomes inactive (for example when another editor becomes active). 
	// Implement this function if you need to clean some data structures.

}

/*********************************************************************************************************************/
/* ACTIONS																											 */
/*********************************************************************************************************************/
void SEExplosionViewEditor::getActions(SBVector<SBAction*>& actionVector) {

	// SAMSON Element generator pro tip: SAMSON calls this function to show the user actions associated to your editor in context menus.
	// Append actions to the actionVector if necessary.
	// Please refer to tutorials for examples.

}

/*********************************************************************************************************************/
/* RENDERING																										 */
/*********************************************************************************************************************/
void SEExplosionViewEditor::display() {

	// SAMSON Element generator pro tip: this function is called by SAMSON during the main rendering loop. 
	// is ONLY called when SHIFT is pressed

	glEnable(GL_DEPTH_TEST);

	if (createArrow) {
		displayHelper::displayArrow(currentStart, currentEnd, groupColor.at(currentGroup % color::numberOfColors));
	}

	if (drawPathArrows) {
		for (auto const& eGroup : getApp()->getExplosionPathsOfGroups()) {
			SB_FOR(hTypes::atomsLengthTuple alt, eGroup.second) {
				displayHelper::displayArrow(get<0>(alt)->getPosition(), get<1>(alt)->getPosition(), groupColor.at(eGroup.first % color::numberOfColors));
			}
		}
	}
	if (drawGuidedArrows) {
		for (auto const& childPath : getApp()->getExplosionPathOfChildren()) {
			if (vectorOps::calcLengthBetweenTwoVectors(childPath.second.first, childPath.second.second) > 100) {
				displayHelper::displayArrow(childPath.second.first, childPath.second.second, groupColor.at(childPath.first % color::numberOfColors));
			}
		}
	}

	glDisable(GL_DEPTH_TEST);
}

void SEExplosionViewEditor::displayForShadow() {

	// SAMSON Element generator pro tip: this function is called by SAMSON during the main rendering loop in order to compute shadows. 
	// Implement this function if your editor displays things in viewports, so that your editor can cast shadows
	// to other objects in SAMSON, for example thanks to the utility
	// functions provided by SAMSON (e.g. displaySpheres, displayTriangles, etc.)

}

void SEExplosionViewEditor::displayInterface() {

	// SAMSON Element generator pro tip: this function is called by SAMSON during the main rendering loop in order to display the editor 2D interface in viewports. 
	// Implement this function if your editor displays a 2D user interface. For example, a rectangle selection editor would display a 2D rectangle in the active viewport. 
	// You may use utility functions provided by SAMSON (e.g. displayLinesOrtho and displayTrianglesOrtho).

}

/*********************************************************************************************************************/
/* GUI EVENTS																										 */
/*********************************************************************************************************************/
void SEExplosionViewEditor::mousePressEvent(QMouseEvent* event) {

	if (createArrow) {
		SBPosition3 pos = SAMSON::getWorldPositionFromViewportPosition(SAMSON::getMousePositionInViewport());

		currentEnd = pos;
		getApp()->finishDrawingOfExplosionPath(make_tuple(currentStart, currentEnd, vectorOps::calcVectorLength(currentEnd - currentStart)));

		createArrow = false;
	}
}

void SEExplosionViewEditor::mouseReleaseEvent(QMouseEvent* event) {

	// SAMSON Element generator pro tip: SAMSON redirects Qt events to the active editor. 
	// Implement this function to handle this event with your editor.

}

void SEExplosionViewEditor::mouseMoveEvent(QMouseEvent* event) {

	if (createArrow) {
		// sets the current mouse position as temp end position so the arrow can be drawn without beeing finished
		currentEnd = SAMSON::getWorldPositionFromViewportPosition(SAMSON::getMousePositionInViewport());

		SAMSON::requestViewportUpdate();
	}
}

void SEExplosionViewEditor::mouseDoubleClickEvent(QMouseEvent* event) {

	// SAMSON Element generator pro tip: SAMSON redirects Qt events to the active editor. 
	// Implement this function to handle this event with your editor.

}

void SEExplosionViewEditor::wheelEvent(QWheelEvent* event) {

	// SAMSON Element generator pro tip: SAMSON redirects Qt events to the active editor. 
	// Implement this function to handle this event with your editor.

}

void SEExplosionViewEditor::keyPressEvent(QKeyEvent* event) {

	// SAMSON Element generator pro tip: SAMSON redirects Qt events to the active editor. 
	// Implement this function to handle this event with your editor.

}

void SEExplosionViewEditor::keyReleaseEvent(QKeyEvent* event) {

	// SAMSON Element generator pro tip: SAMSON redirects Qt events to the active editor. 
	// Implement this function to handle this event with your editor.

}


/*********************************************************************************************************************/
/* SAMSON EVENTS																									 */
/*********************************************************************************************************************/
void SEExplosionViewEditor::onBaseEvent(SBBaseEvent* baseEvent) {

	// SAMSON Element generator pro tip: implement this function if you need to handle base events

}

void SEExplosionViewEditor::onDocumentEvent(SBDocumentEvent* documentEvent) {

	// SAMSON Element generator pro tip: implement this function if you need to handle document events 

}

void SEExplosionViewEditor::onDynamicalEvent(SBDynamicalEvent* dynamicalEvent) {

	// SAMSON Element generator pro tip: implement this function if you need to handle dynamical events 

}

void SEExplosionViewEditor::onStructuralEvent(SBStructuralEvent* documentEvent) {

	// SAMSON Element generator pro tip: implement this function if you need to handle structural events

}

/*********************************************************************************************************************/
/* PUBLIC FUNCTIONS																									 */
/*********************************************************************************************************************/
void SEExplosionViewEditor::startDrawingArrow(int group, SBPosition3 start) {
	createArrow = true;
	currentStart = start;
	currentGroup = group;
}

void SEExplosionViewEditor::cancelAddExplosionPath() {
	createArrow = false;
}

void SEExplosionViewEditor::setDrawGuidedArrows(bool draw) {
	drawGuidedArrows = draw;
	SAMSON::requestViewportUpdate();
}

void SEExplosionViewEditor::setDrawPathArrows(bool draw) {
	drawPathArrows = draw;
	SAMSON::requestViewportUpdate();
}


/*********************************************************************************************************************/
/* PRIVATE FUNCTIONS																								 */
/*********************************************************************************************************************/

void SEExplosionViewEditor::initializeGroupColors() {
	groupColor.insert(make_pair(0, color::red));
	groupColor.insert(make_pair(1, color::green));
	groupColor.insert(make_pair(2, color::blue));
	groupColor.insert(make_pair(3, color::yellow));
	groupColor.insert(make_pair(4, color::orange));
	groupColor.insert(make_pair(5, color::purple));
	groupColor.insert(make_pair(6, color::cyan));
	groupColor.insert(make_pair(7, color::magenta));
	groupColor.insert(make_pair(8, color::lime));
	groupColor.insert(make_pair(9, color::pink));
	groupColor.insert(make_pair(10, color::teal));
	groupColor.insert(make_pair(11, color::lavender));
	groupColor.insert(make_pair(12, color::brown));
	groupColor.insert(make_pair(13, color::beige));
	groupColor.insert(make_pair(14, color::maroon));
	groupColor.insert(make_pair(15, color::mint));
	groupColor.insert(make_pair(16, color::olive));
	groupColor.insert(make_pair(17, color::coral));
	groupColor.insert(make_pair(18, color::navy));
	groupColor.insert(make_pair(19, color::grey));
	groupColor.insert(make_pair(20, color::white));
	groupColor.insert(make_pair(21, color::black));

}