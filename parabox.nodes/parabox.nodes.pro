TEMPLATE = aux
cache()

VUO_FRAMEWORK_PATH = ~/vuo/trunk/framework
VUO_USER_MODULES_PATH = ~/Library/Application\ Support/Vuo/Modules
QMAKE_PRE_LINK += mkdir -p "$${VUO_USER_MODULES_PATH}"
FRAMEWORK_HEADERS = ~/vuo/trunk/framework/Vuo.framework/Headers


## Compile nodes

NODE_SOURCES += \
	co.parabox.data.make.keyValuePair.c \
	co.parabox.time.get.c \
	co.parabox.file.text.c \
	co.parabox.mesh.get.c \
	co.parabox.mesh.make.c \
	co.parabox.mesh.triangle.c \
	co.parabox.shader.make.graph.c \
	co.parabox.mesh.make.normals.c \
	co.parabox.mesh.make.uvlines.c \
	co.parabox.shader.make.glsl.c \
	co.parabox.layer.sceneObject.c \
	co.parabox.image.get.id.c \
	co.parabox.list.average.c \
	co.parabox.convert.quaternion.c \
	co.parabox.convert.euler.c \
	co.parabox.convert.rotationMatrix.c \
	co.parabox.text.contains.c \
	co.parabox.list.bounds.c \
	co.parabox.utility.fps.c \
	co.parabox.layer.name.c \
	co.parabox.utility.slider.c \
	co.parabox.event.drop.c \
	co.parabox.sceneObject.layer.c \
	co.parabox.image.make.pixels.c \
	co.parabox.transform.debug.c \
	co.parabox.transform.make.matrix.c \
	co.parabox.scene.get.transform.c \
	co.parabox.layer.make.material.c \
	co.parabox.scene.get.mesh.c \
	co.parabox.shader.make.uniform.c \
	## just for testing input editors
	# co.parabox.test.inputEditors.c

OTHER_FILES += $$NODE_SOURCES

node.input = NODE_SOURCES
node.output = ${QMAKE_FILE_IN_BASE}.vuonode
node.commands = $${VUO_FRAMEWORK_PATH}/vuo-compile --header-search-path $${FRAMEWORK_HEADERS} --output ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN}
QMAKE_EXTRA_COMPILERS += node


## Compile types

TYPE_SOURCES += \
	PbxShaderUniform.c \
	VuoList_PbxShaderUniform.cc

OTHER_FILES += $$TYPE_SOURCES

HEADERS += \
	PbxShaderUniform.h \
	VuoList_PbxShaderUniform.h

type.input = TYPE_SOURCES
type.output = ${QMAKE_FILE_IN_BASE}.bc
type.commands = $${VUO_FRAMEWORK_PATH}/vuo-compile --header-search-path $${FRAMEWORK_HEADERS} --output ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN}
QMAKE_EXTRA_COMPILERS += type


## Build the archive

NODE_SET_ZIP = co.parabox.nodes.vuonode

NODE_OBJECTS = $$NODE_SOURCES
NODE_OBJECTS ~= s/\\.c$/.vuonode/g

GENERIC_NODE_SOURCES = $$NODE_SOURCES

TYPE_OBJECTS = $$TYPE_SOURCES
TYPE_OBJECTS ~= s/\\.cc?$/.bc/g

NODE_SET_ZIP_CONTENTS = \
	$$NODE_OBJECTS \
	$$TYPE_OBJECTS \
	$$GENERIC_NODE_SOURCES \
	$$HEADERS

createNodeSetZip.commands = \
	( [ -f $$NODE_SET_ZIP ] && rm $$NODE_SET_ZIP ) ; \
	zip --quiet $$NODE_SET_ZIP $$NODE_SET_ZIP_CONTENTS \
	&& cp $$NODE_SET_ZIP $$VUO_USER_MODULES_PATH
createNodeSetZip.depends = $$NODE_SET_ZIP_CONTENTS
createNodeSetZip.target = $$NODE_SET_ZIP
POST_TARGETDEPS += $$NODE_SET_ZIP
QMAKE_EXTRA_TARGETS += createNodeSetZip
QMAKE_CLEAN += $$NODE_SET_ZIP
