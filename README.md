# Rendering Pipeline
>This project was completed while taking CS130 (Computer Graphics) at University of California, Riverside in Winter 2021 with Dr.Tamar Shinar. The main purpose of the course was to become familiar and comfortable using physics and vector knowledge to render a 3D pieline.

## General
The main purpose of this project was to use generate 2D/3D objects using pixels by object-oriented design for each object. Following the graphics pipeline
below
![Screen Shot 2021-03-03 at 6 19 20 PM](https://user-images.githubusercontent.com/62925991/109901116-537da000-7c4d-11eb-9a01-9a3607a36891.png)

Implemented Things like
* Vertex and Fragment Shading Calls
* Rasterization and Interpolation
* Clipping
* Using a z-buffer for Hidden Surfaces

## Some Tests 
Tests 8,11,& 23

![Screen Shot 2021-03-01 at 4 21 05 PM](https://user-images.githubusercontent.com/62925991/109900561-6cd21c80-7c4c-11eb-8db9-66be86eff204.png)
![Screen Shot 2021-03-03 at 6 16 10 PM](https://user-images.githubusercontent.com/62925991/109900640-8e330880-7c4c-11eb-9901-a2b16e9b9285.png)
![Screen Shot 2021-03-03 at 6 15 56 PM](https://user-images.githubusercontent.com/62925991/109900633-8c694500-7c4c-11eb-9d2e-0b28f4413f06.png)


## Usage
Once the files are downloaded and all the code is in place you can run the tests or the grading script. To compile run ```scons```. To run the tests, run ```./ray_tracer -i <test-file>``` and once it runs the output of that test file will be routed to the output.png where you can see it. This project comes with a grading script, which you can run like this: ```./grading-script.py .``` and you will see all the tests that are passed.

### Technologies
* C++
* Python (for grading script) 
