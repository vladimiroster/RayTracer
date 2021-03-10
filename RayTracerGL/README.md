# RayTracerGL - An OpenGL app for working with the RayTracer

![Shaded spherical cows in a vacuum](Shadows.gif)

*RayTracerGL* uses *RayTracerLib* to render 3D scenes, and then *OpenGL* to display the scenes on the screen. 

The app is very rudimentary (so far), but has the following features:
1. Render time and FPS print out to the console
2. Save *ppm* images of the scene by pressing the 's' key
3. Zoom in/out using numpad +/-
4. Rotate around the x-axis using up/down arrows
5. Rotate around the y-axis using the left/right arrows

## Setup
The app uses [GLFW](https://www.glfw.org/) to access the OpenGL API. The following directories are what is specified in the VS project. Adjust the project accordingly if you choose other install locations.
1. Copy the [sources](https://github.com/glfw/glfw/releases/download/3.3.3/glfw-3.3.3.zip) (specifically the includes) to *c:\glfw-3.3.3*
2. Copy the [*\*.lib*](https://github.com/glfw/glfw/releases/download/3.3.3/glfw-3.3.3.bin.WIN64.zip) files to c:\glfw-3.3.3\lib
3. Copy the *glfw3.dll* to the build location of *RayTracerGL* or setup the PATH accordingly

## Make a GIF:

To make a gif out of a set of saved images, install ImageMagic and the *convert* utility, then run:

```
$ magick mogrify -format jpg *.ppm
$ magick convert -delay 10 -loop 0 *.jpg <name>.gif
```
