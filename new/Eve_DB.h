///////////////////////////////////////
/*  Eve_DB  6/20/14
    
    Ben Davis-Purcell

    Constants throughout Eve. This is kept to a minimum, with ideally most input variables dynamically inputted from files to allow for multiple geometries.

*/
///////////////////////////////////////

#ifndef ROOT_Eve_DB
#define ROOT_Eve_DB

// Technical Variables for internal puposes of event display
	
/// Maximum number of paddles in the scint plane
const int MAX_PADDLE_NUM =  100; 

const int MAX_ROADS_NUM =  500;
const int MAX_TRACK_NUM =  77;
const int MAX_HITS_NUM = 100;

// Planar view variables

/// Canvas length equivalent in meters
const double canvas_length = 4.0;

/* /// Position of the 1. MWDC inside a canvas in pixels */
/* const double canvas_MWDC1_posx = 0.1;  */
/* const double canvas_MWDC1_posy = 0.5; */

/* /// Position of the 1. MWDC inside a canvas in pixels */
/* const double canvas_MWDC2_posx = 0.3;  */
/* const double canvas_MWDC2_posy = 0.5; */

/// Position of the dE-plane inside a canvas in pixels
const double canvas_dE_posx = 0.57; 
const double canvas_dE_posy = 0.5;

/// Position of the E-plane inside a canvas in pixels
const double canvas_E_posx = 0.84; 
const double canvas_E_posy = 0.5;


// dE-plane variables

/// number of paddles
const int dE_PN  = 24;

/// Paddle length
const double dE_length = 0.5;

/// Paddle height
const double dE_height = 0.086;

/// Distance between the begining of the first chamber and 
/// the beginning of the dE scintillation plane;
const double dE_z = 1.0;



// E-plane variables

/// number of paddles
const int E_PN  = 24;

/// Paddle length
const double E_length = 0.5;

/// Paddle height
const double E_height = 0.086;

/// Distance between the begining of the first chamber and 
/// the beginning of the E scintillation plane;
const double E_z = 1.1;


#endif
