/// \headerfile SBProxy.hpp "SBProxy.hpp"
#include "SBProxy.hpp"

/// \headerfile SEExplosionViewEditor.hpp "SEExplosionViewEditor.hpp"
#include "editor\SEExplosionViewEditor.hpp"


// Class descriptor

// SAMSON Element generator pro tip: complete this descriptor to expose this class to SAMSON and other SAMSON Elements

SB_CLASS_BEGIN(SEExplosionViewEditor);

	SB_CLASS_TYPE(SBCClass::Editor);
        SB_CLASS_DESCRIPTION("SEExplosionViewEditor : Editor for Explosion View");

	SB_FACTORY_BEGIN;

		SB_CONSTRUCTOR_0(SEExplosionViewEditor);

	SB_FACTORY_END;

	SB_INTERFACE_BEGIN;

	SB_INTERFACE_END;

SB_CLASS_END(SEExplosionViewEditor);

