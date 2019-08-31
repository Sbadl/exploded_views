/// \headerfile SESphereMappingEditorDescriptor.hpp "SESphereMappingEditorDescriptor.hpp"
#include "editor/SEExplosionViewEditorDescriptor.hpp"

/// \headerfile SESphereMappingAppDescriptor.hpp "SESphereMappingAppDescriptor.hpp"
#include "SEExplosionViewAppDescriptor.hpp"



// Describe the SAMSON Element

// SAMSON Element generator pro tip: modify the information below if necessary
// (for example when a new class is added, when the version number changes, to describe categories more precisely, etc.)

SB_ELEMENT_DESCRIPTION("This application creates explosion views\n");
SB_ELEMENT_DOCUMENTATION("Resource/Documentation/doc.html");
SB_ELEMENT_VERSION_NUMBER("0.6.0");

SB_ELEMENT_CLASSES_BEGIN;

	SB_ELEMENT_CLASS(SEExplosionViewApp);
	SB_ELEMENT_CLASS(SEExplosionViewEditor);

SB_ELEMENT_CLASSES_END;

SB_ELEMENT_CATEGORIES_BEGIN;

	SB_ELEMENT_CATEGORY(SBClass::Category::General);

SB_ELEMENT_CATEGORIES_END;
