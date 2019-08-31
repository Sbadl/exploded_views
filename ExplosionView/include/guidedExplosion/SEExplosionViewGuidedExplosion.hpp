#pragma once 

#include "SAMSON.hpp"
#include "SBMStructuralModel.hpp"
#include "SBQuantity.hpp"
#include "SEExplosionViewAppFunctions.hpp"


class SEExplosionViewApp;

/// This class implements the functionality of guided explosion
class SEExplosionViewGuidedExplosion {

public:

	/*********************************************************************************************************************/
	/* CONSTRUCTOR / DECONSTRUCTOR																						 */
	/*********************************************************************************************************************/
	/// \name Constructors and destructors
	//@{
	SEExplosionViewGuidedExplosion();																///< Constructs an app
	virtual ~SEExplosionViewGuidedExplosion();														///< Destructs the app
	//@}

	/*********************************************************************************************************************/
	/* CLASS POINTER																									 */
	/*********************************************************************************************************************/
	/// \name App
	//@{
	SEExplosionViewApp* getApp() const;																///< Returns a pointer to the App of the element
	//@}

	/*********************************************************************************************************************/
	/* PUBLIC FUNCTIONS																							 */
	/*********************************************************************************************************************/
	/// \name explosions
	//@{
	void guidedExplosion(const int explosionValue, const int explosionForce, const int explosionSliderMin,			///< handles the spherical explosion
		const int explosionSliderMax, double explosionSliderAdjust, const int easeIndex);
	//@}

	/*********************************************************************************************************************/
	/* GETTER/SETTER																									 */
	/*********************************************************************************************************************/
	/// \name getter and setter
	//@{
	void setExplosionParent(int explosionP);
	void setExplosionChild(int explosionC);
	void setExplosionOrder(int order);
	void setNumberOfPeelingGroups(int numberOfGroups);
	void setExplodeNearestChild(bool explNearestChild);
	void setUsePositionCoeff(bool usePosCoeff);
	void setSaveArrows(bool save);

	map<int, hTypes::positionPair> getExplosionPathOfChildren();

	void resetPreperationValues();																	///< resets all values that were set during prepare explosion	
	//@}

	/*********************************************************************************************************************/
	/* VIRTUAL																											 */
	/*********************************************************************************************************************/
	/// \name child explosion
	//@{
	virtual SBPosition3 explodeChild(SBNode* parent, hTypes::nodeLengthPair childDistPair,					///< handles the explosion of a single child
		double childExplosionValue, double explosionSliderAdjust, SBQuantity::length forceCoeff) = 0;
	//@}

protected:
	/*********************************************************************************************************************/
	/* PROTECTED GETTER/SETTER																							 */
	/*********************************************************************************************************************/
	/// \name protected getter and setter
	//@{
	map<SBNode*, SBPosition3> getParentCentre();
	map<SBNode*, SBQuantity::length> getStructureCoeff();
	map<SBNode*, map<SBNode*, SBQuantity::length>> getPositionCoeffs();
	map<SBNode*, SBPosition3> getOriginalPosOfChildCentreOfParent(SBNode* parent);
	bool getUsePositionCoeff();
	//@}

	/*********************************************************************************************************************/
	/* PROTECTED PREPARATION																							 */
	/*********************************************************************************************************************/
	/// \name protected preparation
	//@{
	virtual SBNodeIndexer prepareParent(SBNode* parent);												///< perpares given parent for explosion
	//@}
private:

	/*********************************************************************************************************************/
	/* PRIVATE FUNCTIONS																								 */
	/*********************************************************************************************************************/
	double calcExplosionValue(																			///< calculated the explosion values for the peeling groups and sequential explosion children
		const int explosionValue, const int interval, const int index, const int numberOfElements,
		const int explosionSliderMin, const int explosionSliderMax, const int easeIndex);
	
	SBNodeIndexer fillChildIndexerSelected(SBNodeIndexer childIndexer);

	void prepareForExplosion();																			///< prepares everything for a guided explosion - needs to be done whenever parent of child changes or a new model was loaded
	void preparePeeling(SBNode* parent, SBQuantity::length minLength, SBQuantity::length maxLength);	///< puts the children of parent in the corresponding peeling group
	void calcStructureCoeff(SBNode* parent);															///< calc structual given coeff = average distance of atoms to parent center
	void calcPositionCoeff(SBNode* parent, SBQuantity::length minLength, SBQuantity::length maxLength); ///< calc position coeff = relative distance of child to parent 1-0

	SBQuantity::length calcForceCoeff(int explosionForce);												///< calculates the current force coefficient
	SBQuantity::length normalizeInterval(SBQuantity::length x,											///< transforms  min<x<max into 0<x<1
		SBQuantity::length min, SBQuantity::length max);
	SBQuantity::length normalizeInterval(int x, int min, int max);										///< transforms  min<x<max into 0<x<1

	bool checkAnyChildSelected();																		///< returns true if any child is selected

	bool loadIntoParentIndexer();																		///< loads parent nodes into the parentIndexer according to explosionParent
	SBNodeIndexer loadIntoChildIndexer(SBNode* parent);													///< loads child nodes of given parent into the childIndexer according to explosionChild

	/*********************************************************************************************************************/
	/* VARIABLES																										 */
	/*********************************************************************************************************************/
	///< hierarchie indices | DO NOT CHANGE IN RESET VALUES!
	int explosionParent;							//< 0-5     0=Document, 1=Model, 2=Molecule, 3=Chain, 4=Residue, 5=Backbone/SideChain
	int explosionChild;								//< 0-5		0=Model, 1=Molecule, 2=Chain, 3=Residue, 4=Backbone/SideChain, 5=Atom

	bool explodeNearestChild;
	bool usePositionCoeff;

	bool readyToExplode;

	SBNodeIndexer parentIndexer;
	map<SBNode*, SBNodeIndexer> childIndexerOfParent;
	map<SBNode*, SBPosition3> parentCentre;

	///< original distance between the parent centre and the child centre - value is sorted by length
	map<SBNode*, vector<hTypes::nodeLengthPair>> originalDistPCtoCCSorted;

	///< original position of every child for each parent
	map<SBNode*, map<SBNode*, SBPosition3>> originalPosOfChildCentreOfParent;

	///< Peeling
	int numberOfPeelingGroups;
	map<SBNode*, map<int, set<hTypes::nodeLengthPair>>> peelingGroupsOfChildrenOfParent;

	///< force Coeff Interval
	const int fCoeffMin = 0;
	const int fCoeffMid = 5;
	const int fCoeffMax = 10;

	///< structure given explosion coefficients 
	map<SBNode*, SBQuantity::length> structureCoeff;					//< struct Radius Coeff For Each Parent
	const int structureCoeffDiv = 25;
	map<SBNode*, map<SBNode*, SBQuantity::length>> positionCoeffs;		//< between 0-1 - child Position Coeffs For Each Parent

	///< parallel, sequential or groupwise sequential explosion
	int explosionOrder;

	///< The Position Pair for the Path of each Child
	map<int, hTypes::positionPair> explosionPathOfChildren;
	bool saveArrows;
};
