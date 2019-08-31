#pragma once 

#include "SAMSON.hpp"
#include "SBQuantity.hpp"
#include "SEExplosionViewAppFunctions.hpp"

class SEExplosionViewApp;


/// This class implements the functionality of the path explosion

class SEExplosionViewPathExplosion {

public:

	/*********************************************************************************************************************/
	/* CONSTRUCTOR / DECONSTRUCTOR																						 */
	/*********************************************************************************************************************/
	/// Constructors and destructors
	//@{
	SEExplosionViewPathExplosion();																	///< Constructor
	virtual ~SEExplosionViewPathExplosion();														///< Destructor
	//@}

	/*********************************************************************************************************************/
	/* CLASS POINTER																									 */
	/*********************************************************************************************************************/
	/// \name App
	//@{
	SEExplosionViewApp* getApp() const;																///< Returns a pointer to the App of the element
	//@}


	/*********************************************************************************************************************/
	/* PUBLIC FUNCTIONS																								 */
	/*********************************************************************************************************************/
	/// \name explosion group
	//@{
	void pathExplosion(int explosionValue, int explosionSliderMin,									///< handles the explosion along the predefined paths
		int explosionSliderMax, int easeIndex);

	int addExplosionGroup();										///< adds the current selection if the nodes in it are not already contained in another group and returns the assigned groupIndex
	bool deleteExplosionGroup(int groupIndex);						///< deletes the explosion group an all its paths

	void showCurrentGroup(bool show, int groupIndex);				///< selects the current group
	void deselectAllGroups();										///< deselects all groups
	//@}

	/// \name explosion path
	//@{
	int addExplosionPathToGroup(int groupIndex, hTypes::positionsLengthTuple posL);		///< adds the given position pair to the path vectors of the given group and returns the index of the path
	int deleteLastExplosionPathOfGroup(int groupIndex);
	//@}

	/*********************************************************************************************************************/
	/* GETTER/SETTER																									 */
	/*********************************************************************************************************************/
	/// \name getter and setter
	//@{
	SBNodeIndexer getExplosionGroup(int groupIndex);
	map<int, vector<hTypes::atomsLengthTuple>> getExplosionPathsOfGroups();

	void clearAllVars();
	//@}

private:

	/*********************************************************************************************************************/
	/* PRIVATE FUNCTIONS																								 */
	/*********************************************************************************************************************/
	void addPathModel();
	bool anyGroupSelected();
	bool groupSelected(int groupIndex);

	/*********************************************************************************************************************/
	/* VARIABLE																											 */
	/*********************************************************************************************************************/
	///< current number of groups
	int currentNumberOfGroups;

	///< For each group an Indexer with all of the groups Atoms
	map<int, SBNodeIndexer> explosionGroups;
	///< The Position Pair for each Path of each Group
	///< ONLY INSERT AND DELETE FROM THE BACK - Path index for new path = vector.size
	map<int, vector<hTypes::atomsLengthTuple>> explosionPathsOfGroups;


	map<int, SBMStructuralModel*>  pathModelOfGroup;

};
