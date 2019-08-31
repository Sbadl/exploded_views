SET ( PROJECT_FOLDER_NAME ExplosionView )
SET ( PROJECT_PATH ${ELEMENT_NAME} )
SET ( OUTPUT_NAME "SE${PROJECT_FOLDER_NAME}" )
ADD_DEFINITIONS( -DELEMENT_NAME=\"${OUTPUT_NAME}\" )

SET( OUTPUT_UUID 95B86A5B-BC5D-62C1-2B41-6C68D0BBADE3 )
SET( OUTPUT_UUID-VAR \"${OUTPUT_UUID}\" )
ADD_DEFINITIONS( -DSB_ELEMENT_UUID=${OUTPUT_UUID-VAR} )

MESSAGE ( "-- ++ CREATING PROJECT OR MAKEFILE ---> ${OUTPUT_UUID} : ${OUTPUT_NAME}" )

FILE ( GLOB ${OUTPUT_UUID}_headers include/*.hpp )
FILE ( GLOB ${OUTPUT_UUID}_headers_editor include/editor/*.hpp )
FILE ( GLOB ${OUTPUT_UUID}_headers_guidedExplosion include/guidedExplosion/*.hpp )
FILE ( GLOB ${OUTPUT_UUID}_headers_interactiveExplosion include/interactiveExplosion/*.hpp )
FILE ( GLOB ${OUTPUT_UUID}_sources source/*.cpp )
FILE ( GLOB ${OUTPUT_UUID}_sources_editor source/editor/*.cpp )
FILE ( GLOB ${OUTPUT_UUID}_sources_guidedExplosion source/guidedExplosion/*.cpp )
FILE ( GLOB ${OUTPUT_UUID}_sources_interactiveExplosion source/interactiveExplosion/*.cpp )
FILE ( GLOB_RECURSE ${OUTPUT_UUID}_resources resource/*.qrc )
FILE ( GLOB_RECURSE ${OUTPUT_UUID}_form form/*.ui )
		
FILE( GLOB_RECURSE ${OUTPUT_UUID}_ts resource/translations/*.ts )

INCLUDE_DIRECTORIES ( include )
INCLUDE_DIRECTORIES ( ${CMAKE_CURRENT_BINARY_DIR} )

IF(QT_WRAP_CPP)

	QT5_WRAP_CPP( ${OUTPUT_UUID}_MOC_GENERATED ${${OUTPUT_UUID}_headersToMOC} )

ENDIF(QT_WRAP_CPP)

QT5_WRAP_UI( ${OUTPUT_UUID}_UI_GENERATED ${${OUTPUT_UUID}_form} )

QT5_ADD_RESOURCES( ${OUTPUT_UUID}_RCC_GENERATED ${${OUTPUT_UUID}_resources} )

QT5_ADD_TRANSLATION( ${OUTPUT_UUID}_QM  ${${OUTPUT_UUID}_ts} )

SOURCE_GROUP ( include FILES ${${OUTPUT_UUID}_headers} )
SOURCE_GROUP ( include\\editor FILES ${${OUTPUT_UUID}_headers_editor} )
SOURCE_GROUP ( include\\guidedExplosion FILES ${${OUTPUT_UUID}_headers_guidedExplosion} )
SOURCE_GROUP ( include\\interactiveExplosion FILES ${${OUTPUT_UUID}_headers_interactiveExplosion} )
SOURCE_GROUP ( source FILES ${${OUTPUT_UUID}_sources} )
SOURCE_GROUP ( source\\editor FILES ${${OUTPUT_UUID}_sources_editor} )
SOURCE_GROUP ( source\\guidedExplosion FILES ${${OUTPUT_UUID}_sources_guidedExplosion} )
SOURCE_GROUP ( source\\interactiveExplosion FILES ${${OUTPUT_UUID}_sources_interactiveExplosion} )
SOURCE_GROUP ( resource FILES ${${OUTPUT_UUID}_resources} )
SOURCE_GROUP ( form FILES ${${OUTPUT_UUID}_form} )
SOURCE_GROUP ( translationFiles FILES ${${PROJECT_FOLDER_NAME}_QM} ${${OUTPUT_UUID}_ts} )
SOURCE_GROUP ( GeneratedFiles FILES ${${OUTPUT_UUID}_MOC_GENERATED} ${${OUTPUT_UUID}_UI_GENERATED} ${${OUTPUT_UUID}_RCC_GENERATED} )

ADD_LIBRARY ( ${OUTPUT_NAME} SHARED 
${${OUTPUT_UUID}_headers} 
${${OUTPUT_UUID}_headers_editor} 
${${OUTPUT_UUID}_headers_guidedExplosion} 
${${OUTPUT_UUID}_headers_interactiveExplosion} 
${${OUTPUT_UUID}_sources} 
${${OUTPUT_UUID}_sources_editor}
${${OUTPUT_UUID}_sources_guidedExplosion}
${${OUTPUT_UUID}_sources_interactiveExplosion}
${${OUTPUT_UUID}_MOC_GENERATED} ${${OUTPUT_UUID}_UI_GENERATED} ${${OUTPUT_UUID}_RCC_GENERATED} ${${OUTPUT_UUID}_QM} )

TARGET_LINK_LIBRARIES (	${OUTPUT_NAME}
						${QT_LIBRARIES}
						${OPENGL_LIBRARY}
						${SAMSON_SDK_LIBRARIES}
						)
						
SET_TARGET_PROPERTIES(${OUTPUT_NAME} PROPERTIES OUTPUT_NAME ${OUTPUT_UUID})		
						
IF( UNIX ) 
	SET_TARGET_PROPERTIES ( ${OUTPUT_NAME} PROPERTIES PREFIX "" )
ENDIF( UNIX ) 

SET_PROPERTY ( TARGET ${OUTPUT_NAME} PROPERTY FOLDER "Elements" )

SAMSON_INSTALL_ELEMENT_DIRECTORY(  data/ ${OUTPUT_UUID}/Data )
SAMSON_INSTALL_ELEMENT_DIRECTORY( resource/ ${OUTPUT_UUID}/Resource )
SAMSON_INSTALL_ELEMENT_DIRECTORY( doc/ ${OUTPUT_UUID}/Documentation )

SAMSON_INSTALL_ELEMENT ( ${OUTPUT_NAME} ${OUTPUT_UUID} )
