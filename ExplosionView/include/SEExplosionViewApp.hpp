#pragma once 

#include "SBDApp.hpp"
#include "SEExplosionViewAppGUI.hpp"
#include "guidedExplosion/SEExplosionViewSphereExplosion.hpp"
#include "guidedExplosion/SEExplosionViewPCAExplosion.hpp"
#include "interactiveExplosion/SEExplosionViewPathExplosion.hpp"
#include "SAMSON.hpp"
#include "SBQuantity.hpp"
#include "SBDColorSchemeConstant.hpp"
#include <QTimer>

class SEExplosionViewEditor;

/// This class implements the functionality of the app

// SAMSON Element generator pro tip: add non-GUI functionality in this class. The GUI functionality should go in the SEExplosionViewAppGUI class

class SEExplosionViewApp : public SBDApp {

	SB_CLASS

public:

	/*********************************************************************************************************************/
	/* CONSTRUCTOR / DECONSTRUCTOR																						 */
	/*********************************************************************************************************************/
	/// \name Constructors and destructors
	//@{
	SEExplosionViewApp();																			///< Constructs an app
	virtual ~SEExplosionViewApp();																	///< Destructs the app
	//@}

	/*********************************************************************************************************************/
	/* CLASS POINTER																									 */
	/*********************************************************************************************************************/
	/// \name class pointer
	//@{
	SEExplosionViewAppGUI* getGUI() const;														///< Returns a pointer to the GUI of the app
	SEExplosionViewEditor* getEditor();															///< Returns a pointer to the Editor of the element
	//@}

	/*********************************************************************************************************************/
	/* PUBLIC FUNCTIONS	GUIDED EXPLOSION																				 */
	/*********************************************************************************************************************/
	/// \name guided explosion
	//@}
	void guidedExplosion(const int explosionValue, const int explosionForce, const int explosionSliderMin, const int explosionSliderMax);	///< handles the spherical explosion
	void setSphericalExplosion(bool sphericalChecked);
	void setPCAExplosion(bool pcaChecked);
	void setExplosionParent(int explosionP);
	void setExplosionChild(int explosionC);
	void setExplosionOrder(int order);
	void setNumberOfPeelingGroups(int numberOfGroups);
	void setPCAComponent(int component);
	void setEase(int eIndex);
	map<int, hTypes::positionPair> getExplosionPathOfChildren();
	void setExplodeNearestChild(bool explNearestChild);
	void setUsePosCoeff(bool usePosCoeff);
	void displayGuidedArrows(bool display);
	void resetModelPosition();
	//@}

	/*********************************************************************************************************************/
	/* PUBLIC FUNCTIONS	INTERACTIVE EXPLOSION																			 */
	/*********************************************************************************************************************/
	/// \name interactive explosion
	//@{
	void pathExplode(int explosionValue, int explosionSliderMin, int explosionSliderMax);	///< handles the path explosion
	int addExplosionGroup();
	void showCurrentGroup(bool show, int groupIndex);
	void displayPathArrows(bool display);
	void deselectAllGroups();
	bool deleteExplosionGroup(int groupIndex);
	void startDrawingOfExplosionPath(int groupIndex, int tableRowCount);
	void finishDrawingOfExplosionPath(hTypes::positionsLengthTuple positions);
	void cancelAddExplosionPath();
	map<int, vector<hTypes::atomsLengthTuple>> getExplosionPathsOfGroups();
	int deleteLastExplosionPathOfGroup(int groupIndex);
	void resetPathExplosion();
	//@}

	/*********************************************************************************************************************/
	/* GETTER / SETTER																									 */
	/*********************************************************************************************************************/
	/// \name getter and setter
	//@{
	void setRealDocumentEvent(bool real);
	//@}

private:

	/*********************************************************************************************************************/
	/* PRIVATE FUNCTIONS																								 */
	/*********************************************************************************************************************/
	void distributeGUISettings();
	void onDocumentEvent(SBDocumentEvent* event);							///< Event Handler
	void setExplosionEditor();
	void setUpModel();														///< needs to be called whenever a new model was loaded or all values are reset
	void resetPositionOfIndexer(SBNodeIndexer nodes);

	/*********************************************************************************************************************/
	/* VARIABLES																										 */
	/*********************************************************************************************************************/
	int easeIndex;

	SEExplosionViewSphereExplosion* sphereExplosionHandler;
	SEExplosionViewPCAExplosion* pcaExplosionHandler;
	SEExplosionViewPathExplosion* pathExplosionHandler;

	bool sphericalExplosion;
	bool pcaExplosion;

	SBNodeIndexer modelAtomIndexer;					///< holds all atoms of the model
	SBConformation* originalModelPosition;			///< original position of the model in the origin

	const int start_explosionSliderMax;
	double explosionSliderAdjust;					///< is responsible for keeping explosion value between 0-99

	int currentExplosionGroup;

	bool realDocumentEvent;
};


SB_REGISTER_TARGET_TYPE(SEExplosionViewApp, "SEExplosionViewApp", "736A63D2-3832-7E0D-2008-1B50712C3979");
SB_DECLARE_BASE_TYPE(SEExplosionViewApp, SBDApp);
