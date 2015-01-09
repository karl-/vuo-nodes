TEMPLATE = aux
cache()

NODE_SOURCES += \
	co.parabox.file.text.c

OTHER_FILES += $$NODE_SOURCES

# If building from the SDK node examples folder, use this path
# VUO_FRAMEWORK_PATH = ../../../framework
VUO_FRAMEWORK_PATH = ~/sdk/vuo-1.0.0-sdk/framework

VUO_USER_MODULES_PATH = ~/Library/Application\ Support/Vuo/Modules

node.input = NODE_SOURCES
node.output = ${QMAKE_FILE_IN_BASE}.vuonode
node.commands = $${VUO_FRAMEWORK_PATH}/vuo-compile --output ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN} \
	&& mkdir -p "$${VUO_USER_MODULES_PATH}" \
	&& cp ${QMAKE_FILE_OUT} "$${VUO_USER_MODULES_PATH}"
QMAKE_EXTRA_COMPILERS += node

QMAKE_CLEAN = *.vuonode
