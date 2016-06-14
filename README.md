Vuo Nodes
=========

Custom nodes and subcompositions for use with Vuo.

## Available Nodes

- `Assign Name to Layer` – Sets the name of a layer (only operates on top level layer).
- `Convert Layer to SceneObject` – Converts a Layer to a Scene Object.
- `Convert 3D Object to Layer` – Converts a 3D object to a Layer.
- `Decimate Events` – Allows only one event per-X number of events through.  Useful to siphon data down to samples every X number of events.
- `Read Text File` – Read all text from a file.
- `Average List` – Keeps a rolling average of list values.  Handy for smoothing rapidly changing data.
- `Get List Bounds` – Find the minimum and maximum values of a list.
- `Get Mesh Values` – Get the elements that compose a mesh (vertices, triangles, textures, etc).  This will recurse through a sceneobject graph and fetch all elements.
- `Make Mesh with Values` – Create a new mesh with elements.
- `Make Normals Line Mesh` – Creates a new line segment mesh where each line is a position and normal debug line.
- `Make UV Line Segments` – Creates a list of VuoPoint2d lines that can be used to render the UV graph for a mesh.
- `Make Triangle Image` – Creates a plane with vertex colors stored in the tangents array.  Use only with custom shader that accepts tangents as colors.
- `Get 3d Object Local Transform` – Get the transform of a sceneobject.  As the name implies, this transform does not take hierarchy into account.
- `Make Shader with GLSL` – Create a `VuoShader` from vertex and fragment source text.  Use with `Make Shader Uniform` to pass data to the fragment shader.
- `Make Graph` – Create a simple bar or line graph from a list of real values.
- `Make Shader Uniform` – Create a uniform variable to pass to a GLSL shader.
- `Text Contains` – Returns true if `text` contains `value`, false otherwise.
- `Get Date and Time` – Get the system date and time.
- `Frames Per Second` – Keeps a rolling average of deltas.  Usually you should use with a window node's `Requested Frame Output` port.
- `Generic Slider` – Scales a value from 0-1 to range of min/max.

## Vuo Subcompositions

**Requires Vuo 1.2 or greater**

- `Render UV Image` – Create an image of an object's UVs.
- `Show Object Normals` – Create a 3d object that renders a model's normals.
- `Show Wireframe` – Adds a wireframe to the sceneobject.

## To Use

Place the `co.parabox.nodes.vuonode` & all `.vuo` files (found in the bin folder) in your `User Modules` folder: `~/Library/Application\ Support/Vuo/Modules/`.

The easiest way to find this folder to to open Vuo, then select `Tools > Open User Modules Folder`.
