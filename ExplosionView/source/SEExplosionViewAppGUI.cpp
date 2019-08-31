#include "SEExplosionViewAppGUI.hpp"
#include "SEExplosionViewApp.hpp"
#include "SAMSON.hpp"
#include "SBGWindow.hpp"

/*********************************************************************************************************************/
/* CONSTRUCTOR / DECONSTRUCTOR																						 */
/*********************************************************************************************************************/
SEExplosionViewAppGUI::SEExplosionViewAppGUI(SEExplosionViewApp* t) :
	SBGApp(t) {

	ui.setupUi(this);

	explosionValue = ui.sliderExplosionRange->value();
	explosionForce = ui.sliderExplosionForce->value();

	setToolTips();

	// create a timer
	forwardGuidedTimer.setInterval(10);
	// setup signal and slot
	connect(&forwardGuidedTimer, SIGNAL(timeout()),
		this, SLOT(playGuidedForward()));

	// create a timer
	backwardGuidedTimer.setInterval(10);
	// setup signal and slot
	connect(&backwardGuidedTimer, SIGNAL(timeout()),
		this, SLOT(playGuidedBackward()));

	// create a timer
	forwardPathTimer.setInterval(10);
	// setup signal and slot
	connect(&forwardPathTimer, SIGNAL(timeout()),
		this, SLOT(playPathForward()));

	// create a timer
	backwardPathTimer.setInterval(10);
	// setup signal and slot
	connect(&backwardPathTimer, SIGNAL(timeout()),
		this, SLOT(playPathBackward()));

	// create a timer
	cameraRotation.setInterval(10);
	// setup signal and slot
	connect(&cameraRotation, SIGNAL(timeout()),
		this, SLOT(handleCameraRotation()));
}

SEExplosionViewAppGUI::~SEExplosionViewAppGUI() {

}

/*********************************************************************************************************************/
/* CLASS POINTER																									 */
/*********************************************************************************************************************/
SEExplosionViewApp* SEExplosionViewAppGUI::getApp() const { return static_cast<SEExplosionViewApp*>(SBGApp::getApp()); }

/*********************************************************************************************************************/
/* SETTINGS																											 */
/*********************************************************************************************************************/
void SEExplosionViewAppGUI::loadSettings(SBGSettings *settings) {

	if (settings == NULL) return;

	// SAMSON Element generator pro tip: complete this function so your app can save its GUI state from one session to the next

}

void SEExplosionViewAppGUI::saveSettings(SBGSettings *settings) {

	if (settings == NULL) return;

	// SAMSON Element generator pro tip: complete this function so your app can save its GUI state from one session to the next

}

/*********************************************************************************************************************/
/* IDENTITY																											 */
/*********************************************************************************************************************/
SBCContainerUUID SEExplosionViewAppGUI::getUUID() const { return SBCContainerUUID("01FB35FF-E30B-ED53-F2FD-CEF8E0AB31AD"); }

QPixmap SEExplosionViewAppGUI::getLogo() const {

	// SAMSON Element generator pro tip: this icon will be visible in the GUI title bar. 
	// Modify it to better reflect the purpose of your app.

	return QPixmap(QString::fromStdString(SB_ELEMENT_PATH + "/Resource/icons/SEExplosionViewAppIcon.png"));

}

QString SEExplosionViewAppGUI::getName() const {

	// SAMSON Element generator pro tip: this string will be the GUI title. 
	// Modify this function to have a user-friendly description of your app inside SAMSON

	return "Explosion View";

}

int SEExplosionViewAppGUI::getFormat() const {

	// SAMSON Element generator pro tip: modify these default settings to configure the window
	//
	// SBGWindow::Savable : let users save and load interface settings (implement loadSettings and saveSettings)
	// SBGWindow::Lockable : let users lock the window on top
	// SBGWindow::Resizable : let users resize the window
	// SBGWindow::Citable : let users obtain citation information (implement getCitation)

	return (SBGWindow::Savable | SBGWindow::Lockable | SBGWindow::Resizable | SBGWindow::Citable);

}

QString SEExplosionViewAppGUI::getCitation() const {

	// SAMSON Element generator pro tip: modify this function to add citation information

	return
		"If you use this app in your work, please cite: <br/>"
		"<br/>"
		"[1] <a href=\"https://www.samson-connect.net\">https://www.samson-connect.net</a><br/>";

}

/*********************************************************************************************************************/
/* PUBLIC FUNCTIONS																								 */
/*********************************************************************************************************************/
void SEExplosionViewAppGUI::resetGUI() {
	stopTimer();
	resetComboBoxesTab1();
	resetSlidersTab1();
	resetComboBoxesTab2();
	resetSlidersTab2();
	//TODO: reset tab widget
}

int SEExplosionViewAppGUI::getEaseIndex() {
	return ui.comboBox_Ease->currentIndex();
}

bool SEExplosionViewAppGUI::getSphericalExplosion() {
	return ui.radioButton_sphericalExplosion->isChecked();
}

int SEExplosionViewAppGUI::getGuidedExplosionRangeSliderMax() {
	return ui.sliderExplosionRange->maximum();
}
int SEExplosionViewAppGUI::getGuidedExplosionRangeSliderMin() {
	return ui.sliderExplosionRange->minimum();
}

int SEExplosionViewAppGUI::getPathExplosionRangeSliderMax() {
	return ui.horizontalSlider_pathExplosion->maximum();
}
int SEExplosionViewAppGUI::getPathExplosionRangeSliderMin() {
	return ui.horizontalSlider_pathExplosion->minimum();
}

void SEExplosionViewAppGUI::setExplosionRangeSlidersMax(int max) {
	ui.sliderExplosionRange->setMaximum(max);
	ui.horizontalSlider_pathExplosion->setMaximum(max);
	ui.spinBox_PageStepLength->setMaximum(max / 100);
}

int SEExplosionViewAppGUI::getPCAComponent() {
	return ui.spinBox_PCA->value();
}

int SEExplosionViewAppGUI::getNumberOfPeelingGroups() {
	return ui.spinBox_Peeling->value();
}

bool SEExplosionViewAppGUI::getDisplayGuidedArrows() {
	return ui.checkBox_displayArrowsGuided->isChecked();
}

bool SEExplosionViewAppGUI::getDisplayPathArrows() {
	return ui.checkBox_displayArrows->isChecked();;
}

bool SEExplosionViewAppGUI::getExplodeNearestChild() {
	return ui.checkBox_explodeNearestChild->isChecked();
}

bool SEExplosionViewAppGUI::getUsePositionCoeff() {
	return ui.checkBox_usePosCoeff->isChecked();
}

void SEExplosionViewAppGUI::addExplosionPathEnd(int pathIndex, hTypes::atomsLengthTuple position) {
	insertPathTableEntry(pathIndex, position);
}


/*********************************************************************************************************************/
/* GENERAL SLOTS																									 */
/*********************************************************************************************************************/
void SEExplosionViewAppGUI::setEase(int easeIndex) {
	stopTimer();
	getApp()->setEase(easeIndex);
}

void SEExplosionViewAppGUI::setTimerInterval(int interval) {
	forwardGuidedTimer.setInterval(interval);
	backwardGuidedTimer.setInterval(interval);
	forwardPathTimer.setInterval(interval);
	backwardPathTimer.setInterval(interval);
}

void SEExplosionViewAppGUI::setSlidersPageStepLength(int pageStepLength) {
	ui.sliderExplosionRange->setPageStep(pageStepLength);
	ui.horizontalSlider_pathExplosion->setPageStep(pageStepLength);
}

void SEExplosionViewAppGUI::rotateCamera() {
	if (cameraRotation.isActive()) {
		cameraRotation.stop();
	}
	else {
		cameraRotation.start();
	}
}

void SEExplosionViewAppGUI::handleCameraRotation() {

	SBRadianPerSecond3 vel(SBQuantity::radianPerSecond(0), SBQuantity::radianPerSecond(0.001), SBQuantity::radianPerSecond(0));

	SAMSON::getActiveCamera()->rotate(vel, SBPosition3(SBQuantity::length(0), SBQuantity::length(0), SBQuantity::length(0)));
	SAMSON::requestViewportUpdate();
}

void SEExplosionViewAppGUI::onReset() {
	getApp()->resetModelPosition();
	getApp()->resetPathExplosion();

	//reset GUI
	resetGUI();
}

/*********************************************************************************************************************/
/* GUIDED EXPLOSION SLOTS																							 */
/*********************************************************************************************************************/
void SEExplosionViewAppGUI::setSphericalExplosion(bool sphericalChecked) {
	stopTimer();

	ui.spinBox_PCA->setEnabled(false);

	getApp()->setSphericalExplosion(sphericalChecked);

	//reset last explosion
	//explosion will not collapse, because getApp()->setSphericalExplosion(sphericalChecked) reset the prepValues
	resetSlidersTab1();
}

void SEExplosionViewAppGUI::setPCAExplosion(bool pcaChecked) {
	stopTimer();
	ui.spinBox_PCA->setEnabled(true);

	getApp()->setPCAExplosion(pcaChecked);

	//reset last explosion
	//explosion will not collapse, because getApp()->setPCAExplosion(pcaChecked) reset the prepValues
	resetSlidersTab1();
}

void SEExplosionViewAppGUI::setGuidedExplosionValue(int explosionV) {
	stopTimer();
	explosionValue = explosionV;
	guidedExplosion(explosionValue, explosionForce);
}

void SEExplosionViewAppGUI::setGuidedExplosionForce(int explosionF) {
	stopTimer();
	explosionForce = explosionF;
	guidedExplosion(explosionValue, explosionForce);
}

void SEExplosionViewAppGUI::setGuidedExplosionParent(int explosionP) {
	stopTimer();
	//Parent has to be one layer above child
	if (explosionP > ui.comboBox_Child->currentIndex()) {
		ui.comboBox_Child->setCurrentIndex(explosionP);
	}

	getApp()->setExplosionParent(explosionP);
	//reset last explosion
	//explosion will not collapse, because getApp()->setExplosionParent(explosionP) reset the prepValues
	resetSlidersTab1();
}

void SEExplosionViewAppGUI::setGuidedExplosionChild(int explosionC) {
	stopTimer();
	//Parent has to be one layer above child.
	if (ui.comboBox_Parent->currentIndex() > explosionC) {
		ui.comboBox_Parent->setCurrentIndex(explosionC);
	}

	getApp()->setExplosionChild(explosionC);
	//reset last explosion
	//explosion will not collapse, because getApp()->setExplosionChild(explosionC) reset the prepValues
	resetSlidersTab1();
}

void SEExplosionViewAppGUI::setGuidedExplosionOrder(int order) {
	stopTimer();
	//reset last explosion
	resetSlidersTab1();

	//reset to one one change so we can guarantee that we only have one peeling group for parallel explosion
	ui.spinBox_Peeling->setValue(1);
	setNumberOfPeelingGroups(1);

	if (order < 2) {
		ui.spinBox_Peeling->setEnabled(false);
	}
	else {
		ui.spinBox_Peeling->setEnabled(true);
	}
	getApp()->setExplosionOrder(order);
}

void SEExplosionViewAppGUI::setNumberOfPeelingGroups(int numberOfGroups) {
	stopTimer();
	//reset last explosion
	resetSlidersTab1();
	getApp()->setNumberOfPeelingGroups(numberOfGroups);
}

void SEExplosionViewAppGUI::setPCAComponent(int component) {
	stopTimer();

	getApp()->setPCAComponent(component);
	//reset last explosion
	resetSlidersTab1();
}

void SEExplosionViewAppGUI::setExplodeNearestChild(bool explNearestChild) {
	stopTimer();

	//reset last explosion
	resetSlidersTab1();
	getApp()->setExplodeNearestChild(explNearestChild);
}

void SEExplosionViewAppGUI::setUsePosCoeff(bool usePosCoeff) {
	stopTimer();

	//reset last explosion
	resetSlidersTab1();
	getApp()->setUsePosCoeff(usePosCoeff);
}

void SEExplosionViewAppGUI::displayGuidedArrows(bool display) {
	getApp()->displayGuidedArrows(display);
}

void SEExplosionViewAppGUI::playGuidedForward() {

	ui.sliderExplosionRange->triggerAction(QAbstractSlider::SliderPageStepAdd);

	if (backwardGuidedTimer.isActive()) {
		backwardGuidedTimer.stop();
	}

	if (ui.sliderExplosionRange->value() != ui.sliderExplosionRange->maximum()) {
		forwardGuidedTimer.start();
	}
	else {
		forwardGuidedTimer.stop();
	}
}

void SEExplosionViewAppGUI::playGuidedBackward() {
	ui.sliderExplosionRange->triggerAction(QAbstractSlider::SliderPageStepSub);

	if (forwardGuidedTimer.isActive()) {
		forwardGuidedTimer.stop();
	}

	if (ui.sliderExplosionRange->value() != ui.sliderExplosionRange->minimum()) {
		backwardGuidedTimer.start();
	}
	else {
		backwardGuidedTimer.stop();
	}
}


/*********************************************************************************************************************/
/* INTERACTIVE EXPLOSION SLOTS																						 */
/*********************************************************************************************************************/
void SEExplosionViewAppGUI::setPathExplosionValue(int explosionV) {
	stopTimer();

	getApp()->pathExplode(explosionV, getPathExplosionRangeSliderMin(), getPathExplosionRangeSliderMax());
}

void SEExplosionViewAppGUI::addPathExplosionGroup() {
	stopTimer();

	int x = getApp()->addExplosionGroup();
	//if x == -1 the group was not added
	if (x != -1) {
		QString s = QString::number(x);
		ui.comboBox_explosionGroups->addItem(s);
		ui.comboBox_explosionGroups->setCurrentIndex(ui.comboBox_explosionGroups->findText(s));
	}
	else {
		showErrorMsg(QString("The current selection is empty or contains atoms that are already in another group"));
	}
}

void SEExplosionViewAppGUI::deletePathExplosionGroup() {
	stopTimer();

	int currentGroup = getCurrentExplosionGroup();

	if (getApp()->deleteExplosionGroup(currentGroup)) {
		QString s = QString::number(currentGroup);
		ui.comboBox_explosionGroups->removeItem(ui.comboBox_explosionGroups->findText(s));

		setPathExplosionGroup(ui.comboBox_explosionGroups->currentText());
	}
}

void SEExplosionViewAppGUI::setPathExplosionGroup(QString s) {
	int newGroup = s.toInt();

	//unselect all other groups
	getApp()->deselectAllGroups();
	//select current group
	ui.checkBox_showCurrentGroup->setChecked(true);
	getApp()->showCurrentGroup(ui.checkBox_showCurrentGroup->isChecked(), newGroup);

	clearPathTable();
	loadPathTableOfGroup(newGroup);
}

void SEExplosionViewAppGUI::addExplosionPathStart() {
	stopTimer();

	int currentGroup = getCurrentExplosionGroup();

	getApp()->startDrawingOfExplosionPath(currentGroup, ui.tableWidget_explosionPaths->rowCount());
}

void SEExplosionViewAppGUI::deleteLastExplosionPath() {
	stopTimer();

	int pathIndex = getApp()->deleteLastExplosionPathOfGroup(getCurrentExplosionGroup());

	if (pathIndex != -1) {
		for (int y = 0; y < ui.tableWidget_explosionPaths->columnCount(); y++) {
			ui.tableWidget_explosionPaths->setItem(pathIndex, y, new QTableWidgetItem(""));
		}
	}
}

void SEExplosionViewAppGUI::showCurrentExplosionGroup(bool show) {
	getApp()->showCurrentGroup(show, getCurrentExplosionGroup());
}

void SEExplosionViewAppGUI::displayPathArrows(bool display) {
	getApp()->displayPathArrows(display);
}

void SEExplosionViewAppGUI::cancelAddExplosionPath() {
	getApp()->cancelAddExplosionPath();
}

void SEExplosionViewAppGUI::playPathForward() {
	ui.horizontalSlider_pathExplosion->triggerAction(QAbstractSlider::SliderPageStepAdd);

	if (backwardPathTimer.isActive()) {
		backwardPathTimer.stop();
	}

	if (ui.horizontalSlider_pathExplosion->value() != ui.horizontalSlider_pathExplosion->maximum()) {
		forwardPathTimer.start();
	}
	else {
		forwardPathTimer.stop();
	}
}

void SEExplosionViewAppGUI::playPathBackward() {
	ui.horizontalSlider_pathExplosion->triggerAction(QAbstractSlider::SliderPageStepSub);

	if (forwardPathTimer.isActive()) {
		forwardPathTimer.stop();
	}

	if (ui.horizontalSlider_pathExplosion->value() != ui.horizontalSlider_pathExplosion->minimum()) {
		backwardPathTimer.start();
	}
	else {
		backwardPathTimer.stop();
	}
}


/*********************************************************************************************************************/
/* PRIVATE FUNCTIONS																								 */
/*********************************************************************************************************************/
void SEExplosionViewAppGUI::guidedExplosion(int explosionValue, int explosionForce) {
	getApp()->guidedExplosion(explosionValue, explosionForce, getGuidedExplosionRangeSliderMin(), getGuidedExplosionRangeSliderMax());
}

int SEExplosionViewAppGUI::getCurrentExplosionGroup() {
	if (ui.comboBox_explosionGroups->currentText() == "") {
		return -1;
	}
	return ui.comboBox_explosionGroups->currentText().toInt();
}

void SEExplosionViewAppGUI::loadPathTableOfGroup(int groupIndex) {

	map<int, vector<hTypes::atomsLengthTuple>> groupPathsAllGroups = getApp()->getExplosionPathsOfGroups();

	if (groupPathsAllGroups.count(groupIndex)) {

		vector<hTypes::atomsLengthTuple> groupPaths = groupPathsAllGroups.at(groupIndex);

		for (int i = 0; i < groupPaths.size(); i++) {
			insertPathTableEntry(i, groupPaths.at(i));
		}
	}
}

void SEExplosionViewAppGUI::insertPathTableEntry(int pathIndex, hTypes::atomsLengthTuple pos) {

	QString index = QString::number(pathIndex);

	QString startx = QString::number((int)get<0>(pos)->getPosition().v[0].getValue());
	QString starty = QString::number((int)get<0>(pos)->getPosition().v[1].getValue());
	QString startz = QString::number((int)get<0>(pos)->getPosition().v[2].getValue());

	QString endx = QString::number((int)get<1>(pos)->getPosition().v[0].getValue());
	QString endy = QString::number((int)get<1>(pos)->getPosition().v[1].getValue());
	QString endz = QString::number((int)get<1>(pos)->getPosition().v[2].getValue());


	ui.tableWidget_explosionPaths->setItem(pathIndex, 0, new QTableWidgetItem(index));
	ui.tableWidget_explosionPaths->setItem(pathIndex, 1, new QTableWidgetItem(startx + ", " + starty + ", " + startz));
	ui.tableWidget_explosionPaths->setItem(pathIndex, 2, new QTableWidgetItem(endx + ", " + endy + ", " + endz));
}

void SEExplosionViewAppGUI::clearPathTable() {
	for (int x = 0; x < ui.tableWidget_explosionPaths->rowCount(); x++) {
		for (int y = 0; y < ui.tableWidget_explosionPaths->columnCount(); y++) {
			ui.tableWidget_explosionPaths->setItem(x, y, new QTableWidgetItem(""));
		}
	}
}

void SEExplosionViewAppGUI::resetComboBoxesTab1() {
	ui.comboBox_Parent->setCurrentIndex(0);
	ui.comboBox_Child->setCurrentIndex(0);
	ui.comboBox_Ease->setCurrentIndex(0);
	ui.comboBox_ExplosionOrder->setCurrentIndex(0);
}

void SEExplosionViewAppGUI::resetSlidersTab1() {
	ui.sliderExplosionRange->setValue(0);
	ui.sliderExplosionForce->setValue(5);
}

void SEExplosionViewAppGUI::stopTimer() {
	forwardGuidedTimer.stop();
	backwardGuidedTimer.stop();
	forwardPathTimer.stop();
	backwardPathTimer.stop();
}

void SEExplosionViewAppGUI::setToolTips() {
	//TODO: set for all important GUI objects

	ui.pushButton_addExplosionGroup->setToolTip("creates an explosion group from selected atoms");
	ui.pushButton_deleteExplosionGroup->setToolTip("deletes the current explosion group");
	ui.pushButton_addExplosionPath->setToolTip("start process of drawing a new explosion path");
	ui.pushButton_deleteExplosionPath->setToolTip("deletes selected explosion path");
}

void SEExplosionViewAppGUI::resetComboBoxesTab2() {
	ui.comboBox_explosionGroups->clear();
}

void SEExplosionViewAppGUI::resetSlidersTab2() {
	ui.horizontalSlider_pathExplosion->setValue(0);
}

void SEExplosionViewAppGUI::showErrorMsg(QString error) {
	msg.showMessage(error);
}