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

You will need to install Blender 2.64 to be able to use the exporter.
You can download it from http://blender.org

*Installing Blender Exporter:
Open Blender
Go to  File>User Preferences
In the dialog box click on "Addons" tab.
Click "Install Addon" button at the bottom of the dialogbox. 
Traverse to the directory where you have the exporter script and select it.


*Loaders
There are a bunch of loaders written for personal projects.

