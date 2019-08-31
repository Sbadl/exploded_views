#pragma once

#include "SBGApp.hpp" 
#include "ui_SEExplosionViewAppGUI.h"
#include "SEExplosionViewAppFunctions.hpp"
#include <QTimer>
#include <QErrorMessage>


class SEExplosionViewApp;

/// This class implements the GUI of the app

// SAMSON Element generator pro tip: add GUI functionality in this class. The non-GUI functionality should go in the SEExplosionViewApp class

class SEExplosionViewAppGUI : public SBGApp {

	Q_OBJECT

public:

	/*********************************************************************************************************************/
	/* CONSTRUCTOR / DECONSTRUCTOR																						 */
	/*********************************************************************************************************************/
	/// \name Constructors and destructors
	//@{
	SEExplosionViewAppGUI(SEExplosionViewApp* t);												///< Constructs a GUI for the app
	virtual ~SEExplosionViewAppGUI();															///< Destructs the GUI of the app
	//@}

	/*********************************************************************************************************************/
	/* CLASS POINTER																									 */
	/*********************************************************************************************************************/
	/// \name App
	//@{
	SEExplosionViewApp* getApp() const;															///< Returns a pointer to the app
	//@}

	/*********************************************************************************************************************/
	/* SETTINGS																											 */
	/*********************************************************************************************************************/
	/// \name Settings
	//@{
	void loadSettings(SBGSettings* settings);													///< Load GUI settings
	void saveSettings(SBGSettings* settings);													///< Save GUI settings
																								//@}

	/*********************************************************************************************************************/
	/* IDENTITY																											 */
	/*********************************************************************************************************************/
	/// \name Identity
	//@{
	virtual SBCContainerUUID getUUID() const;													///< Returns the widget UUID
	virtual QString getName() const;															///< Returns the widget name (used as a title for the embedding window)
	virtual QPixmap getLogo() const;															///< Returns the widget logo
	int getFormat() const;																		///< Returns the widget format
	virtual QString getCitation() const;														///< Returns the citation information
	//@}

	/*********************************************************************************************************************/
	/* PUBLIC FUNCTIONS																								 */
	/*********************************************************************************************************************/
	/// \name Reset
	//@{
	void resetGUI();
	//@}

	/// \name getter and setter Explosion Slider
	//@{
	int getEaseIndex();
	bool getSphericalExplosion();
	int getGuidedExplosionRangeSliderMax();
	int getGuidedExplosionRangeSliderMin();
	int getPathExplosionRangeSliderMax();
	int getPathExplosionRangeSliderMin();
	void setExplosionRangeSlidersMax(int max);
	int getPCAComponent();
	int getNumberOfPeelingGroups();
	bool getDisplayGuidedArrows();
	bool getDisplayPathArrows();
	bool getExplodeNearestChild();
	bool getUsePositionCoeff();
	//@}

	/// \name Explosion Path
	//{
	void addExplosionPathEnd(int pathIndex, hTypes::atomsLengthTuple position);
	//@}

	public slots:

	/*********************************************************************************************************************/
	/* GENERAL SLOTS																									 */
	/*********************************************************************************************************************/
	void setEase(int easeIndex);
	void setTimerInterval(int interval);
	void setSlidersPageStepLength(int pageStepLength);
	void rotateCamera();
	void handleCameraRotation();
	void onReset();

	/*********************************************************************************************************************/
	/* GUIDED EXPLOSION SLOTS																							 */
	/*********************************************************************************************************************/
	void setSphericalExplosion(bool sphericalChecked);
	void setPCAExplosion(bool pcaChecked);
	void setGuidedExplosionValue(int explosionV);
	void setGuidedExplosionForce(int explosionF);
	void setGuidedExplosionParent(int explosionP);
	void setGuidedExplosionChild(int explosionC);
	void playGuidedForward();
	void playGuidedBackward();
	void setGuidedExplosionOrder(int order);
	void setNumberOfPeelingGroups(int numberOfGroups);
	void setPCAComponent(int component);
	void setExplodeNearestChild(bool explNearestChild);
	void setUsePosCoeff(bool usePosCoeff);
	void displayGuidedArrows(bool display);


	/*********************************************************************************************************************/
	/* INTERACTIVE EXPLOSION SLOTS																						 */
	/*********************************************************************************************************************/
	void setPathExplosionValue(int explosionV);
	void addPathExplosionGroup();
	void deletePathExplosionGroup();
	void setPathExplosionGroup(QString s);
	void addExplosionPathStart();
	void deleteLastExplosionPath();
	void showCurrentExplosionGroup(bool show);
	void displayPathArrows(bool display);
	void cancelAddExplosionPath();
	void playPathForward();
	void playPathBackward();

private:

	/*********************************************************************************************************************/
	/* PRIVATE FUNCTIONS																								 */
	/*********************************************************************************************************************/
	void guidedExplosion(int explosionValue, int explosionForce);

	int getCurrentExplosionGroup();

	void loadPathTableOfGroup(int groupIndex);
	void insertPathTableEntry(int pathIndex, hTypes::atomsLengthTuple pos);
	void clearPathTable();

	void resetComboBoxesTab1();
	void resetSlidersTab1();												///< if called alone also resets last explosion
	void resetComboBoxesTab2();
	void resetSlidersTab2();

	void stopTimer();

	void setToolTips();
	void showErrorMsg(QString error);

	/*********************************************************************************************************************/
	/*VARIABLES																											 */
	/*********************************************************************************************************************/
	Ui::SEExplosionViewAppGUIClass ui;
	int explosionForce;
	int explosionValue;

	QTimer forwardGuidedTimer;
	QTimer backwardGuidedTimer;

	QTimer forwardPathTimer;
	QTimer backwardPathTimer;

	QErrorMessage msg;

	QTimer cameraRotation;

};

