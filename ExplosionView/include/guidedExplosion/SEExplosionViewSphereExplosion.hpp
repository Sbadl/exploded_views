#pragma once 

#include "SAMSON.hpp"
#include "SBMStructuralModel.hpp"
#include "SBQuantity.hpp"
#include "SEExplosionViewAppFunctions.hpp"
#include "SEExplosionViewGuidedExplosion.hpp"

/// This class implements the functionality of the sphere explosion

class SEExplosionViewSphereExplosion : public SEExplosionViewGuidedExplosion {

public:

	/*********************************************************************************************************************/
	/* CONSTRUCTOR / DECONSTRUCTOR																						 */
	/*********************************************************************************************************************/
	/// Constructors and destructors
	//@{
	SEExplosionViewSphereExplosion();																	///< Constructs an app
	virtual ~SEExplosionViewSphereExplosion();															///< Destructs the app
	//@}

	/*********************************************************************************************************************/
	/* PUBLIC FUNCTIONS																									 */
	/*********************************************************************************************************************/
	/// \name child explosion
	//@{
	///< handles the explosion of a single child
	SBPosition3 explodeChild(SBNode* parent, hTypes::nodeLengthPair childDistPair, double childExplosionValue, double explosionSliderAdjust, SBQuantity::length forceCoeff);
	//}

private:

};
