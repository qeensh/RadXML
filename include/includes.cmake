set (SOURCES_files_Common__Main__Headers
	"${RADXML_SOURCE_DIR}/include/RadXml/common/com_sound.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/common/com_strconv.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/common/mai_system.h")
source_group ("Common\\Main\\Headers" FILES ${SOURCES_files_Common__Main__Headers})

set (SOURCES ${SOURCES_files_Common__Main__Headers} ${SOURCES})

set (SOURCES_files_XML__Main__Headers "${RADXML_SOURCE_DIR}/include/RadXml/xml/xml.h")
source_group ("XML\\Main\\Headers" FILES ${SOURCES_files_XML__Main__Headers})

set (SOURCES ${SOURCES_files_XML__Main__Headers} ${SOURCES})

set (SOURCES_files_RADXML__Game__Elements__Headers
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlAnimationTrack.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlAudioListener.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlCamera.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlEntity.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlLevel.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlLight.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlMovable.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlOverlay.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlOverlayElement.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlParticleSystem.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlSound.h")
source_group ("RADXML\\Game\\Elements\\Headers" FILES ${SOURCES_files_RADXML__Game__Elements__Headers})

set (SOURCES ${SOURCES_files_RADXML__Game__Elements__Headers} ${SOURCES})

set (SOURCES_files_RADXML__Game__Main__Headers "${RADXML_SOURCE_DIR}/include/RadXml/RadXmlGame.h")
source_group ("RADXML\\Game\\Main\\Headers" FILES ${SOURCES_files_RADXML__Game__Main__Headers})

set (SOURCES ${SOURCES_files_RADXML__Game__Main__Headers} ${SOURCES})

set (SOURCES_files_RADXML__App__Elements__Headers
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlAppObject.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlButton.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlComboBox.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlLabel.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlMenu.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlMenuItem.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlTextbox.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlWindow.h")
source_group ("RADXML\\App\\Elements\\Headers" FILES ${SOURCES_files_RADXML__App__Elements__Headers})

set (SOURCES ${SOURCES_files_RADXML__App__Elements__Headers} ${SOURCES})

set (SOURCES_files_RADXML__App__Main__Headers "${RADXML_SOURCE_DIR}/include/RadXml/RadXmlApp.h")
source_group ("RADXML\\App\\Main\\Headers" FILES ${SOURCES_files_RADXML__App__Main__Headers})

set (SOURCES ${SOURCES_files_RADXML__App__Main__Headers} ${SOURCES})

set (SOURCES_files_RADXML__Main__Headers
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXml.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlDatatypes.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlPreprocessor.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlXmlFile.h")
source_group ("RADXML\\Main\\Headers" FILES ${SOURCES_files_RADXML__Main__Headers})

set (SOURCES ${SOURCES_files_RADXML__Main__Headers} ${SOURCES})

set (SOURCES_files_RADXML__Math__Headers
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlVector2.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlVector3.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlVector4.h")
source_group ("RADXML\\Math\\Headers" FILES ${SOURCES_files_RADXML__Math__Headers})

set (SOURCES ${SOURCES_files_RADXML__Math__Headers} ${SOURCES})

set (SOURCES_files_RADXML__Global__Headers
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlEvents.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlGlobal.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlSystem.h"
	"${RADXML_SOURCE_DIR}/include/RadXml/RadXmlTimer.h")
source_group ("RADXML\\Global\\Headers" FILES ${SOURCES_files_RADXML__Global__Headers})

set (SOURCES ${SOURCES_files_RADXML__Global__Headers} ${SOURCES})

set (SOURCES_files_RADXML__Native__Headers "${RADXML_SOURCE_DIR}/include/RadXml/RadXmlNative.h")
source_group ("RADXML\\Native\\Headers" FILES ${SOURCES_files_RADXML__Native__Headers})

set (SOURCES ${SOURCES_files_RADXML__Native__Headers} ${SOURCES})

set (SOURCES_files_RADXML__Input__Headers "${RADXML_SOURCE_DIR}/include/RadXml/RadXmlInput.h")
source_group ("RADXML\\Input\\Headers" FILES ${SOURCES_files_RADXML__Input__Headers})

set (SOURCES ${SOURCES_files_RADXML__Input__Headers} ${SOURCES})

set (SOURCES_files_wxWidgets__Main__Headers "${RADXML_SOURCE_DIR}/include/RadXml/wxwidgets/wx_global.h")
source_group ("wxWidgets\\Main\\Headers" FILES ${SOURCES_files_wxWidgets__Main__Headers})

set (SOURCES ${SOURCES_files_wxWidgets__Main__Headers} ${SOURCES})

