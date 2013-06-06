set (SOURCES_files_Common__Main__Sources
	"${RADXML_SOURCE_DIR}/source/RadXml/common/com_sound.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/common/com_strconv.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/common/mai_system.cpp")
source_group ("Common\\Main\\Sources" FILES ${SOURCES_files_Common__Main__Sources})

set (SOURCES ${SOURCES_files_Common__Main__Sources} ${SOURCES})

set (SOURCES_files_XML__Main__Sources "${RADXML_SOURCE_DIR}/source/RadXml/xml/xml.cpp")
source_group ("XML\\Main\\Sources" FILES ${SOURCES_files_XML__Main__Sources})

set (SOURCES ${SOURCES_files_XML__Main__Sources} ${SOURCES})

set (SOURCES_files_RADXML__Game__Elements__Sources
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlAnimationTrack.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlAudioListener.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlCamera.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlEntity.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlLevel.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlLight.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlMovable.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlOverlay.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlOverlayElement.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlParticleSystem.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlSound.cpp")
source_group ("RADXML\\Game\\Elements\\Sources" FILES ${SOURCES_files_RADXML__Game__Elements__Sources})

set (SOURCES ${SOURCES_files_RADXML__Game__Elements__Sources} ${SOURCES})

set (SOURCES_files_RADXML__Game__Main__Sources "${RADXML_SOURCE_DIR}/source/radxml/RadXmlGame.cpp")
source_group ("RADXML\\Game\\Main\\Sources" FILES ${SOURCES_files_RADXML__Game__Main__Sources})

set (SOURCES ${SOURCES_files_RADXML__Game__Main__Sources} ${SOURCES})

set (SOURCES_files_RADXML__App__Elements__Sources
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlAppObject.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlButton.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlComboBox.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlLabel.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlMenu.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlMenuItem.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlTextbox.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlWindow.cpp")
source_group ("RADXML\\App\\Elements\\Sources" FILES ${SOURCES_files_RADXML__App__Elements__Sources})

set (SOURCES ${SOURCES_files_RADXML__App__Elements__Sources} ${SOURCES})

set (SOURCES_files_RADXML__App__Main__Sources "${RADXML_SOURCE_DIR}/source/RadXml/RadXmlApp.cpp")
source_group ("RADXML\\App\\Main\\Sources" FILES ${SOURCES_files_RADXML__App__Main__Sources})

set (SOURCES ${SOURCES_files_RADXML__App__Main__Sources} ${SOURCES})

set (SOURCES_files_RADXML__Main__Sources 
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXml.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlXmlFile.cpp")
source_group ("RADXML\\Main\\Sources" FILES ${SOURCES_files_RADXML__Main__Sources})

set (SOURCES ${SOURCES_files_RADXML__Main__Sources} ${SOURCES})

set (SOURCES_files_RADXML__Math__Sources
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlVector2.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlVector3.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlVector4.cpp")
source_group ("RADXML\\Math\\Sources" FILES ${SOURCES_files_RADXML__Math__Sources})

set (SOURCES ${SOURCES_files_RADXML__Math__Sources} ${SOURCES})

set (SOURCES_files_RADXML__Global__Sources
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlEvents.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlGlobal.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlSystem.cpp"
	"${RADXML_SOURCE_DIR}/source/RadXml/RadXmlTimer.cpp")
source_group ("RADXML\\Global\\Sources" FILES ${SOURCES_files_RADXML__Global__Sources})

set (SOURCES ${SOURCES_files_RADXML__Global__Sources} ${SOURCES})

set (SOURCES_files_RADXML__Native__Sources "${RADXML_SOURCE_DIR}/source/RadXml/RadXmlNative.cpp")
source_group ("RADXML\\Native\\Sources" FILES ${SOURCES_files_RADXML__Native__Sources})

set (SOURCES ${SOURCES_files_RADXML__Native__Sources} ${SOURCES})

set (SOURCES_files_RADXML__Input__Sources "${RADXML_SOURCE_DIR}/source/RadXml/RadXmlInput.cpp")
source_group ("RADXML\\Input\\Sources" FILES ${SOURCES_files_RADXML__Input__Sources})

set (SOURCES ${SOURCES_files_RADXML__Input__Sources} ${SOURCES})

set (SOURCES_files_wxWidgets__Main__Sources "${RADXML_SOURCE_DIR}/source/RadXml/wxwidgets/wx_main.cpp")
source_group ("wxWidgets\\Main\\Sources" FILES ${SOURCES_files_wxWidgets__Main__Sources})

set (SOURCES ${SOURCES_files_wxWidgets__Main__Sources} ${SOURCES})

