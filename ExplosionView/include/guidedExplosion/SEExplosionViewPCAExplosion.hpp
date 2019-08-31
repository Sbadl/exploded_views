#pragma once 

#include "SAMSON.hpp"
#include "SBMStructuralModel.hpp"
#include "SBQuantity.hpp"
#include "SEExplosionViewAppFunctions.hpp"
#include "SEExplosionViewGuidedExplosion.hpp"

/// This class implements the functionality of the sphere explosion

class SEExplosionViewPCAExplosion : public SEExplosionViewGuidedExplosion {

public:

	/*********************************************************************************************************************/
	/* CONSTRUCTOR / DECONSTRUCTOR																						 */
	/*********************************************************************************************************************/
	/// Constructors and destructors
	//@{
	SEExplosionViewPCAExplosion();																	///< Constructs an app
	virtual ~SEExplosionViewPCAExplosion();															///< Destructs the app
	//@}


	/*********************************************************************************************************************/
	/* PUBLIC FUNCTIONS																									 */
	/*********************************************************************************************************************/
	/// \name child explosion
	//@{
	SBPosition3 explodeChild(SBNode* parent, hTypes::nodeLengthPair childDistPair,							///< handles the explosion of a single child
		double childExplosionValue, double explosionSliderAdjust, SBQuantity::length forceCoeff);
	//@}

	/*********************************************************************************************************************/
	/* GETTER/SETTER																									 */
	/*********************************************************************************************************************/
	/// \name setter
	//@{
	void setPCAComponent(int component);
	//@}

protected:

	/*********************************************************************************************************************/
	/* PROTECTED FUNCTIONS																								 */
	/*********************************************************************************************************************/
	/// \name preparation
	//@{
	SBNodeIndexer prepareParent(SBNode* parent);													///< perpares given parent for explosion
	//@}

private:
	/*********************************************************************************************************************/
	/* PRIVATE FUNCTIONS																								 */
	/*********************************************************************************************************************/

	void calcDiffToMean(SBNode* parent, SBNodeIndexer childIndexer, SBPosition3 mean,
		map<SBNode*, vector<SBPosition3>>& diffToMeanOfChildrenOfParent);				///< saves the difference of each node in childIndexer to the mean in diffToMeanOfChildrenOfParent

	void calc3x3CovMatrix(vector<SBPosition3> diffToMean, matrixOps::mat3x3& cov);

	bool calcExplosionDirAlongVector(SBPosition3 originalPosition, SBPosition3 normMoveVector); ///< calculates if a point is on one site of the plane defined by the normMoveVector or the other


	/*********************************************************************************************************************/
	/* VARIABLES																										 */
	/*********************************************************************************************************************/
	map<SBNode*, hTypes::positionTriple> eigenVectorsOfParent;						///< eigenvectors of each parent - first in triple is the eigenvector with the highest eigenvalue

	int pcaComponent;

};
