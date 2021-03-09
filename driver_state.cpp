#include "driver_state.h"
#include <cstring>

driver_state::driver_state()
{
}

driver_state::~driver_state()
{
    delete [] image_color;
    delete [] image_depth;
}

// This function should allocate and initialize the arrays that store color and
// depth.  This is not done during the constructor since the width and height
// are not known when this class is constructed.
void initialize_render(driver_state& state, int width, int height)
{
    state.image_width=width;
    state.image_height=height;
    state.image_color= new pixel[width * height];
    state.image_depth= new float[width * height];
    
    for(int i = 0; i < width * height; i++){
	state.image_color[i] = make_pixel(0,0,0); //set to black
	state.image_depth[i] = 1.0;
	}
 //   std::cout<<"TODO: allocate and initialize state.image_color and state.image_depth."<<std::endl;
}

// This function will be called to render the data that has been stored in this class.
// Valid values of type are:
//   render_type::triangle - Each group of three vertices corresponds to a triangle.
//   render_type::indexed -  Each group of three indices in index_data corresponds
//                           to a triangle.  These numbers are indices into vertex_data.
//   render_type::fan -      The vertices are to be interpreted as a triangle fan.
//   render_type::strip -    The vertices are to be interpreted as a triangle strip.
void render(driver_state& state, render_type type)
{
    //std::cout<<"TODO: implement rendering."<<std::endl;
	const data_geometry *dg[3];
	data_geometry arr[3];
	data_vertex dv[3];
	int temp = 0; 
        	
    switch(type){
	
	case render_type::triangle:
	{
	 
	for(int i = 0; i < state.num_vertices/3; i++){
	   for(int j = 0; j < 3; j++){
	      dv[j].data = &state.vertex_data[temp];
	      arr[j].data = dv[j].data;
	      state.vertex_shader(dv[j],arr[j],state.uniform_data);
	      dg[j] = &arr[j]; 
		temp+= state.floats_per_vertex;}
 		clip_triangle(state, *dg[0],*dg[1],*dg[2],0);
	  } }
        break;
	
	case render_type::indexed:
	{
	for(int i = 0; i < state.num_triangles * 3; i+= 3){
		for(int j = 0; j < 3; j++){
		dv[j].data = &state.vertex_data[state.index_data[i+j]*state.floats_per_vertex];
		arr[j].data = dv[j].data;
		state.vertex_shader(dv[j],arr[j],state.uniform_data);
		dg[j] = &arr[j]; }
		clip_triangle(state, *dg[0],*dg[1],*dg[2],0);
	} }
	break;
	
	case render_type::fan:
	{
	for(int i = 0; i < state.num_vertices; i++){
		for(int j = 0; j < 3; j++){
		int index = i + j;
		if(j == 0) {index = 0;}
		dv[j].data = &state.vertex_data[index * state.floats_per_vertex];
		arr[j].data = dv[j].data;
		state.vertex_shader(dv[j],arr[j],state.uniform_data);
		dg[j] = &arr[j]; }
		clip_triangle(state, *dg[0],*dg[1],*dg[2],0);	
	} }
	break;

	case render_type::strip:
	{
	for(int i = 0; i < state.num_vertices - 2; i++){
		for(int j = 0; j < 3; j++){
		dv[j].data = &state.vertex_data[(i + j) * state.floats_per_vertex];
		arr[j].data = dv[j].data;
		state.vertex_shader(dv[j],arr[j],state.uniform_data);
		dg[j] = &arr[j]; }	
		clip_triangle(state, *dg[0],*dg[1],*dg[2],0);
	} }
	break;

	default:{
	std::cout << "default";}
	break;
    }

}


// This function clips a triangle (defined by the three vertices in the "in" array).
// It will be called recursively, once for each clipping face (face=0, 1, ..., 5) to
// clip against each of the clipping faces in turn.  When face=6, clip_triangle should
// simply pass the call on to rasterize_triangle.
void clip_triangle(driver_state& state, const data_geometry& v0,
    const data_geometry& v1, const data_geometry& v2,int face)
{ 
    if(face==6)
    {	
        rasterize_triangle(state, v0, v1, v2);
        return;
    }

    else {
    //std::cout<<"TODO: implement clipping. (The current code passes the triangle through without clipping them.)"<<std::endl;
    
      data_geometry one[3];
      data_geometry two[3];
      float temp,temp2,temp3;
      
	if(v0.gl_Position[2] < -v0.gl_Position[3] && v1.gl_Position[2] < -v1.gl_Position[3] && v2.gl_Position[2] < -v2.gl_Position[3]){
		return;}
	else{
	//std::cout << "in else statement";
	if(v0.gl_Position[2] < -v0.gl_Position[3] && v1.gl_Position[2] >= -v1.gl_Position[3] && v2.gl_Position[2] >= -v2.gl_Position[3]){
	   temp = (-v1.gl_Position[3] - v1.gl_Position[2])/(v0.gl_Position[2] + v0.gl_Position[3] - v1.gl_Position[3] - v1.gl_Position[2]);
	   temp2 = (-v0.gl_Position[3] - v0.gl_Position[2])/(v2.gl_Position[2] + v2.gl_Position[3] - v0.gl_Position[3] - v0.gl_Position[2]);
	   vec4 s = temp * v0.gl_Position + (1-temp) * v1.gl_Position;
           vec4 t = temp2 * v2.gl_Position + (1-temp2) * v0.gl_Position;
	//std::cout << "in if statement";
	   one[0].data = new float[state.floats_per_vertex];
	   one[1] = v1;
	   one[2] = v2;
	
		for(int i = 0; i < state.floats_per_vertex; i++){
			if(state.interp_rules[i] == interp_type::flat){one[0].data[i] = v0.data[i];}
			if(state.interp_rules[i] == interp_type::smooth){one[0].data[i] = temp2 * v2.data[i] + (1-temp2)*v0.data[i];}
			if(state.interp_rules[i] == interp_type::noperspective){
				temp3 = temp2 * v2.gl_Position[3] / (temp2 * v2.gl_Position[3] + (1-temp2) * v0.gl_Position[3]);
				one[0].data[i] = temp3 * v2.data[i] + (1-temp3) * v0.data[i]; }
	 }
		one[0].gl_Position = t;
		one[0] = v0; one[1] = v1; one[2] = v2;
	//	std::cout << one[0].data << " , " << v0.data;
		clip_triangle(state, v0,v1,v2,face+1);

		two[0].data = new float[state.floats_per_vertex];
		two[1] = v1;
		two[2] = one[0];
		
		 for(int i = 0; i < state.floats_per_vertex; i++){
                        if(state.interp_rules[i] == interp_type::flat){two[0].data[i] = v0.data[i];}
                        if(state.interp_rules[i] == interp_type::smooth){two[0].data[i] = temp * v0.data[i] + (1-temp)*v1.data[i];}
                        if(state.interp_rules[i] == interp_type::noperspective){
                                temp3 = temp * v0.gl_Position[3] / (temp * v0.gl_Position[3] + (1-temp) * v1.gl_Position[3]);
                                two[0].data[i] = temp3 * v0.data[i] + (1-temp3) * v1.data[i]; }
	
		}
		two[0].gl_Position = s;
		 two[0] = v0; two[1] = v1; two[2] = v2;
		//clip_triangle(state,v0,v1,v2,face+1);
		}
		clip_triangle(state,v0,v1,v2,face+1);
//		clip_triangle(state,one[0],one[1],one[2],face+1);
	} 
	//clip_triangle(state,v0,v1,v2,face+1);

}
   //clip_triangle(state, v0, v1, v2,face+1);
    

}

// Rasterize the triangle defined by the three vertices in the "in" array.  This
// function is responsible for rasterization, interpolation of data to
// fragments, calling the fragment shader, and z-buffering.
void rasterize_triangle(driver_state& state, const data_geometry& v0,
    const data_geometry& v1, const data_geometry& v2)
{
    //std::cout<<"TODO: implement rasterization"<<std::endl;
     
	//divide data position(x & y & z)  by w coordinate
      
      float a = v0.gl_Position[0]/v0.gl_Position[3];
      float a2 = v0.gl_Position[1]/v0.gl_Position[3];
      float b = v1.gl_Position[0]/v1.gl_Position[3];
      float b2 = v1.gl_Position[1]/v1.gl_Position[3];
      float c = v2.gl_Position[0]/v2.gl_Position[3];
      float c2 = v2.gl_Position[1]/v2.gl_Position[3];


      float a3 = v0.gl_Position[2]/v0.gl_Position[3];
      float b3 = v1.gl_Position[2]/v1.gl_Position[3];
      float c3 = v2.gl_Position[2]/v2.gl_Position[3];
    /* 
	std::cout << "v0: " << a << ", " << a2 << std::endl;
	std::cout << "v1: " << b << ", " << b2 <<std::endl;
	std::cout << "v2: " << c << ", " << c2 <<std::endl;*/

	//x and y positions in pixel space
      int ax = (0.5f * ((a + 1) * state.image_width))-0.5f;
      int ay = (0.5f * ((a2 + 1)* state.image_height))-0.5f;
      int bx = (0.5f * ((b + 1) * state.image_width))-0.5f;
      int by = (0.5f * ((b2 + 1)* state.image_height))-0.5f;
      int cx = (0.5f * ((c + 1) * state.image_width))-0.5f;
      int cy = (0.5f * ((c2 + 1) * state.image_height))-0.5f;

	//std::cout << "a: " << ax << ", "<< ay <<std::endl;
	//std::cout << "b: " << bx << ", " <<by <<std::endl;
	//std::cout << "c: " << cx << ", " <<cy;
	//area of the triangle
     float Tarea = 0.5f * (((bx * cy) - (cx * by)) + ((cx * ay) - (ax * cy)) + ((ax * by)-(bx * ay)));
/*	
	int i = ax +  ay * state.image_width;
	int k = bx + by * state.image_width;
	int l = cx + cy * state.image_width;
	state.image_color[i] = make_pixel(255,255,255);
	state.image_color[k] = make_pixel(255,255,255);
	state.image_color[l] = make_pixel(255,255,255); */

	for(int j = 0; j < state.image_height; j++){
		for(int i = 0; i < state.image_width; i++){
			//Bary Coordinates
			vec2 t = {(float)i, (float)j};
			float alpha = 0.5f * (((bx * cy)-(cx * by)) + ((cx * t[1])-(cy * t[0])) + ((t[0] * by)-(bx * t[1])))/Tarea;
			float beta = 0.5f * (((t[0] * cy) - (cx * t[1])) + ((ay * cx ) -(ax * cy)) +((ax * t[1]) - (t[0] * ay)))/Tarea;
			float gamma = 0.5f * (((bx * t[1]) - (by * t[0])) + ((ay * t[0]) - (ax * t[1])) + ((ax * by)-(bx * ay)))/Tarea;
			
		if(alpha >= 0 && beta >= 0 && gamma >= 0){
		int index = i + j * state.image_width;	
		//state.image_color[index] = make_pixel(255,255,255);			
		
		//std::cout << "here";
		data_fragment f;
		data_output dout;	
	  	float fdata[state.floats_per_vertex];
		f.data = fdata;
		float z = (alpha * a3) + (beta * b3) + (gamma * c3);
		

		if(z < state.image_depth[index]){
		state.image_depth[index] = z; 

		for(int k = 0; k < state.floats_per_vertex; k++){
			if(state.interp_rules[k] == interp_type::flat){
				f.data[k] = v0.data[k]; } 
			if(state.interp_rules[k] == interp_type::smooth){
				float p = (alpha/v0.gl_Position[3])+(beta/v1.gl_Position[3])+(gamma/v2.gl_Position[3]);
				float alphaP = alpha / (p * v0.gl_Position[3]);
				float betaP = beta / (p * v1.gl_Position[3]);
				float gammaP = gamma / (p * v2.gl_Position[3]);
				f.data[k] = (alphaP * v0.data[k]) + (betaP * v1.data[k]) + (gammaP * v2.data[k]); }
			if(state.interp_rules[k] == interp_type::noperspective){
				f.data[k] = (alpha * v0.data[k]) + (beta * v1.data[k]) + (gamma * v2.data[k]); }
			//else { std::cout << "invalid" << std::endl; } 
		}	
		state.fragment_shader(f,dout, state.uniform_data);
		int red = dout.output_color[0] * 255; int green = dout.output_color[1] * 255; int blue = dout.output_color[2] * 255;
		//if(state.image_depth[index] > z){
		//std::cout << "here";
			//state.image_depth[index] = z;
			state.image_color[index] = make_pixel(red,green,blue); //}
	
	}
	}
}
}
}

