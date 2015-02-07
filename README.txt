# 3danim
A Simple 3d animation format. 
Ascii file format for generating and loading simple animations.

If you always wanted to add animated models to your code but were afraid of 
all the complex 3d model formats etc.. you know the ones which make you do 
all kinds of silly tricks just to get the object loaded. This project intends 
to be a solution for that. 

Each Model has a couple of Actions defined
Each Action has a couple of KeyFrames
Each KeyFrame has frame number and Vertices.
And We have the face indices to draw the faces using above Vertices.

KeyFrame animation doesn't get any easier :)


*Exporter
The blender exporter is a simple export script that 
will be able to export animations created in blender.

*Loaders
There are a bunch of loaders written for personal projects.

