/**
 * @file
 * co.parabox.file.text node implementation.
 *
 */

#include "node.h"
#include "VuoGlContext.h"
#include <OpenGL/CGLMacro.h>
#include <stdio.h>

VuoModuleMetadata({
					 "title" : "Read Text File",
					 "keywords" : [ "i/o", "input", "data" ],
					 "version" : "1.0.0",
					 "description": "Opens a file and outputs text.",
					 "dependencies" : [
						 "VuoGlContext"
					 ],
					 "node" : {
						  "exampleCompositions" : [ "" ]
					 }
				 });

void nodeEvent
(
	VuoInputData(VuoText, {"default": "MyFile.txt"}) filePath,
	VuoOutputData(VuoText) text
)
{
	char *file_contents;
	long input_file_size;
	FILE *input_file = fopen(filePath, "rb");
	fseek(input_file, 0, SEEK_END);
	input_file_size = ftell(input_file);
	rewind(input_file);

	file_contents = malloc((input_file_size + 1) * (sizeof(char)));
	fread(file_contents, sizeof(char), input_file_size, input_file);
	fclose(input_file);
	file_contents[input_file_size] = 0;

	*text = VuoText_make(file_contents);

	free(file_contents);
}
