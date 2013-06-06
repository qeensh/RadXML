#!/bin/sh
. ./config.sh

${FLEXBIN}/mxmlc ${SOURCEDIR}/source/RadXML.mxml
mv ${SOURCEDIR}/source/RadXML.swf ${SOURCEDIR}/executable/RadXML.swf
pause
${WEBBROWSER} ${SOURCEDIR}/executable/RadXML.swf
