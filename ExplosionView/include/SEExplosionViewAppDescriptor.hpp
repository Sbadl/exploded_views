/// \headerfile SBProxy.hpp "SBProxy.hpp"
#include "SBProxy.hpp"

/// \headerfile SESphereMappingApp.hpp "SESphereMappingApp.hpp"
#include "SEExplosionViewApp.hpp"


// Class descriptor

// SAMSON Element generator pro tip: complete this descriptor to expose this class to SAMSON and other SAMSON Elements

SB_CLASS_BEGIN(SEExplosionViewApp);

	SB_CLASS_TYPE(SBCClass::App);
        SB_CLASS_DESCRIPTION("SEExplosionViewApp : this app creates explosion views");

	SB_FACTORY_BEGIN;

		SB_CONSTRUCTOR_0(SEExplosionViewApp);

	SB_FACTORY_END;

	SB_INTERFACE_BEGIN;

	SB_INTERFACE_END;

SB_CLASS_END(SEExplosionViewApp);

