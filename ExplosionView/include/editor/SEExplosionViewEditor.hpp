#pragma once 

#include "SBGEditor.hpp"
#include "SEExplosionViewEditorDisplayHelper.hpp"
#include "SEExplosionViewApp.hpp"
#include "SBBaseEvent.hpp"
#include "SBDocumentEvent.hpp"
#include "SBDynamicalEvent.hpp"
#include "SBStructuralEvent.hpp"
#include "SBAction.hpp"

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

/// This class implements an editor

class SEExplosionViewEditor : public SBGEditor {

	SB_CLASS
		Q_OBJECT

public:

	/*********************************************************************************************************************/
	/* CONSTRUCTOR / DECONSTRUCTOR																						 */
	/*********************************************************************************************************************/
	/// \name Constructors and destructors
	//@{
	SEExplosionViewEditor();																///< Builds an editor	
	virtual ~SEExplosionViewEditor();														///< Destructs the editor
	//@}

	/*********************************************************************************************************************/
	/* CLASS POINTER																									 */
	/*********************************************************************************************************************/
	/// \name App
	//@{
	SEExplosionViewApp* getApp() const;														///< Returns a pointer to the App of the element
	//@}

	/*********************************************************************************************************************/
	/* IDENTITY																											 */
	/*********************************************************************************************************************/
	/// \name Identity
	//@{
	virtual SBCContainerUUID getUUID() const;												///< Returns the widget UUID
	virtual QString getName() const;														///< Returns the class name
	virtual QString getText() const;														///< Returns the menu item text
	virtual QPixmap getLogo() const;														///< Returns the pixmap logo
	virtual QKeySequence getShortcut() const;												///< Returns the shorcut
	virtual QString getToolTip() const;														///< Returns the tool tip
	//@}

	/*********************************************************************************************************************/
	/* EDITING																											 */
	/*********************************************************************************************************************/
	/// \name Editing
	//@{
	virtual void beginEditing();															///< Called when editing is about to begin
	virtual void endEditing();																///< Called when editing is about to end

	//@}

	/*********************************************************************************************************************/
	/* ACTIONS																											 */
	/*********************************************************************************************************************/
	/// \name Actions
	//@{
	virtual void getActions(SBVector<SBAction*>& actionVector);								///< Gets the editor's actions
	//@}

	/*********************************************************************************************************************/
	/* RENDERING																										 */
	/*********************************************************************************************************************/
	/// \name Rendering
	//@{
	virtual void display();																	///< Displays the editor
	virtual void displayForShadow();														///< Displays the editor for shadow purposes
	virtual void displayInterface();														///< Displays the 2D interface in the viewport
	//@}

	/*********************************************************************************************************************/
	/* GUI EVENTS																										 */
	/*********************************************************************************************************************/
	/// \name GUI Events
	//@{	
	virtual void mousePressEvent(QMouseEvent* event);										///< Handles mouse press event
	virtual void mouseReleaseEvent(QMouseEvent* event);										///< Handles mouse release event
	virtual void mouseMoveEvent(QMouseEvent* event);										///< Handles mouse move event
	virtual void mouseDoubleClickEvent(QMouseEvent* event);									///< Handles mouse double click event

	virtual void wheelEvent(QWheelEvent* event);											///< Handles wheel event

	virtual void keyPressEvent(QKeyEvent* event);											///< Handles key press event
	virtual void keyReleaseEvent(QKeyEvent* event);											///< Handles key release event
	//@}

	/*********************************************************************************************************************/
	/* SAMSON EVENTS																									 */
	/*********************************************************************************************************************/
	/// \name SAMSON Events
	//@{
	virtual void onBaseEvent(SBBaseEvent* baseEvent);										///< Handles base events
	virtual void onDynamicalEvent(SBDynamicalEvent* dynamicalEvent);						///< Handles dynamical events
	virtual void onDocumentEvent(SBDocumentEvent* documentEvent);							///< Handles document events
	virtual void onStructuralEvent(SBStructuralEvent* documentEvent);						///< Handles structural events
	//@}

	/*********************************************************************************************************************/
	/* PUBLIC FUNCTIONS																									 */
	/*********************************************************************************************************************/
	/// \name arrow drawing
	//@{
	void startDrawingArrow(int group, SBPosition3 start);
	void cancelAddExplosionPath();
	void setDrawGuidedArrows(bool draw);
	void setDrawPathArrows(bool draw);
	//@}

private:
	/*********************************************************************************************************************/
	/* PRIVATE FUNCTIONS																								 */
	/*********************************************************************************************************************/
	void initializeGroupColors();															///< initializes colors for arrows

	/*********************************************************************************************************************/
	/* VARIABLES																										 */
	/*********************************************************************************************************************/
	bool createArrow;																		///< true if an arrow is currently created
	bool drawGuidedArrows;																	///< true if guided arrows should be drawn
	bool drawPathArrows;																	///< true if path arrows should be drawn

	int currentGroup;																		///< currently selected explosion group
	SBPosition3 currentStart;																///< start position of arrows that is currently created
	SBPosition3 currentEnd;																	///< end position of arrows that is currently created

	map<int, const vector<float>> groupColor;												///< arrow color for each group
};


SB_REGISTER_TYPE(SEExplosionViewEditor, "SEExplosionViewEditor", "03AEC6C5-2EE6-E256-D8EB-268BE56FE53A");
SB_DECLARE_BASE_TYPE(SEExplosionViewEditor, SBGEditor);
