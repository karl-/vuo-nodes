Vuo Nodes
=========

Custom nodes for use with Vuo.

## Available Nodes

`Get Date and Time` – Get the system date and time.
`Read Text File` – Read all text from a file.
`Get Mesh Values` – Get the elements that compose a mesh (vertices, triangles, textures, etc).
`Make Mesh with Values` – Create a new mesh with elements.
`Make Triangle Image` – Creates a plane with vertex colors stored in the tangents array.  Use only with custom shader that accepts tangents as colors.
`Make Graph` – Create a simple bar or line graph from a list of real values. 
`Make Shader with GLSL` – Create a `VuoShader` from vertex and fragment source text.  Use with `Make Shader Uniform` to pass data to the fragment shader. 
`Make Shader Uniform` – Create a uniform variable to pass to a GLSL shader.
`Average List` – Keeps a rolling average of list values.  Handy for smoothing rapidly changing data.
`Get List Bounds` – Find the minimum and maximum values of a list.
`Generic Slider` – Scales a value from 0-1 to range of min/max.
`Decimate Events` – Allows only one event per-X number of events through.  Useful to siphon data down to samples every X number of events.

## To Use

Place the `co.parabox.nodes.vuonode` file in you `User Modules` folder (`~/Library/Application\ Support/Vuo/Modules/`).  The easiest way to find this folder to to open Vuo, then select `Tools / Open User Modules Folder`.
