# edgeObjects

* * *
This project is a step towards an audioVisualisation I will make.   
The main objective is to create a selection of independent image segments around the edges detected within a source image. This project requires openFrameworks.    
* * *
### build:
The 'Objectifier' class does the bulk of the work. It performs edge detection, searches for connected edges, and creates the image segments.    
* * *
### example:
Below shows an example. A source Image is passed to the program. The result is a collection of image sections, each of which may be moved independently of the others.

![sourceImage](/bin/images/henry1.jpg) ![output](/bin/images/henry2.png) ![outputJumbled](/bin/images/henry3.png)    

although originally designed with the henry picture in mind, it also creates a nice effect for other images as with the car photo below.   

![carObjects](/bin/images/carObjects.png)
