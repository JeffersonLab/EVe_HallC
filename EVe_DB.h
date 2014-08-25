//************************************************************************* 
//  EVe_DB.h  - 4/14/2008
// 
//  by miham
// 
//  This file contains all data regarding the Big Bite spectrometer
//  (distances, sizes, wire numbers, etc.)
// 
//
//************************************************************************* 

#ifndef ROOT_EVe_DB
#define ROOT_EVe_DB


	/// Big Bite angle

	const double BB_angle = -54.0*3.141592654/180.0; //[deg]

// 1. MWDC variables

/// Chamber width
const double MWDC_width = 0.56;  /// [m]

	/// Height of the first chamber  
const double L1 = 1.13;  /// [m]

	/// Wire number per plane 
const int MWDC1_U1_WN =113;// 141;
const int MWDC1_U1p_WN = 113;//141;

const int MWDC1_X1_WN = 107;//142;
const int MWDC1_X1p_WN = 107;//142;

const int MWDC1_V1_WN = 52;//141;
const int MWDC1_V1p_WN = 52;//141;

const int MWDC1_X = 113;
const int MWDC1_Xp = 113;
const int MWDC1_U = 107;
const int MWDC1_V = 107;
const int MWDC1_Y = 52;
const int MWDC1_Yp = 52;

// 2. MWDC variables

	/// Height of the second chamber 
const double L2 = 1.13;  // [m] 

	/// Distance between the begining of the first chamber and 
	/// the beginning of the second
const double MWDC2_z = 0.81222; // [m]


	/// Wire number per plane 
const int MWDC2_U2_WN = 113;//200;
const int MWDC2_U2p_WN = 113;//200;

const int MWDC2_X2_WN = 107;//202;
const int MWDC2_X2p_WN = 107;//202;

const int MWDC2_V2_WN = 52;//200;
const int MWDC2_V2p_WN = 52;//200;





// s1x-plane variables

	/// number of paddles
	const int dE_PN  = 16;
	
	/// Paddle length
	const double dE_length = 0.755;

	/// Paddle height
	const double dE_height = 0.0753;

	/// Distance between the begining of the first chamber and 
	/// the beginning of the s1x scintillation plane;
	const double dE_z = 1.29753;
	 
	

// s1y-plane variables

	/// number of paddles
	const int E_PN  = 10;

	/// Paddle length
	const double E_length = 0.755;

	/// Paddle height
	const double E_height = 0.0753;

	/// Distance between the begining of the first chamber and 
	/// the beginning of the s1y scintillation plane;
	const double E_z = 1.49443;

	/// Distance between the begining of the first chamber and 
	/// the beginning of the s2x scintillation plane;
const double s2x_z = 3.50743;

	/// Distance between the begining of the first chamber and 
	/// the beginning of the s2y scintillation plane;
const double s2y_z = 3.70433;

// X-projection variables

	/// Distance between the begining of the first chamber and 
	/// the X1-plane
	const double x1_dist = 0.0; 

	/// Distance between the begining of the first chamber and 
	/// the X1p-plane
	const double x1p_dist = 0.0064;

	/// Distance between the begining of the first chamber and 
	/// the X2-plane
	const double x2_dist = 0.8122;//0.838675;

	/// Distance between the begining of the first chamber and 
	/// the X2p-plane
	const double x2p_dist = 0.812284;//0.832775;

// UV-projection variables
	
	/// Distance between the begining of the first chamber and 
	/// the U1-plane
	const double u1_dist = 0.0128;

	/// Distance between the begining of the first chamber and 
	/// the V1-plane
	const double v1_dist = 0.0192;

	/// Distance between the begining of the first chamber and 
	/// the U2-plane
	const double u2_dist = 0.825; // 0.805225;

	/// Distance between the begining of the first chamber and 
	/// the V2-plane
	const double v2_dist = 0.8314; //0.799325;



// Y-projection variables

	/// Distance between the begining of the first chamber and 
	/// the Y1-plane
const double y1_dist = 0.028775;

	/// Distance between the begining of the first chamber and 
	/// the Y1p-plane
const double y1p_dist = 0.035175;

	/// Distance between the begining of the first chamber and 
	/// the Y2-plane
	const double y2_dist = 0.840975;

	/// Distance between the begining of the first chamber and 
	/// the Y2p-plane
	const double y2p_dist = 0.847375;

// Planar view variables

	/// Canvas length equivalent in meters
const double canvas_length = 5.0; // 4.35

	/// Position of the 1. MWDC inside a canvas in pixels
	const double canvas_MWDC1_posx = 0.3; 
	const double canvas_MWDC1_posy = 0.18;

	/// Position of the 1. MWDC inside a canvas in pixels
	const double canvas_MWDC2_posx = 0.5; 
	const double canvas_MWDC2_posy = 0.18;

	/// Position of the dE-plane inside a canvas in pixels
const double canvas_dE_posx = 0.27; 
const double canvas_dE_posy = 0.5;

	/// Position of the E-plane inside a canvas in pixels
const double canvas_E_posx = 0.84; 
const double canvas_E_posy = 0.5;


// 3D view variables

	/// Position of the center of the BB magnet
	const double magnet_xpos = 141.0; // [cm]
	const double magnet_ypos = 0.0; // [cm]
	const double magnet_zpos = -30.0; // [cm]
	
	/// Distance from target to the front face of the BB magnet
	const double magnet_face_dist  = 110.0; // [cm]

	/// Position and tilt of the center of the 1. MWDC
	const double MWDC1_xpos = 225.0; // [cm] 
	const double MWDC1_ypos = 0.0; // [cm]
	const double MWDC1_zpos = -51.923; // [cm]
	const double MWDC1_length = 52.0; // [cm]
	const double MWDC1_height = 113.0; //[cm]
	const double MWDC1_tilt  = 10.0; // [deg]


	/// Position and tilt of the center of the 2. MWDC
	const double MWDC2_xpos = 225.0 + 81.0*0.9848; // [cm] 
	const double MWDC2_ypos = 0.0; // [cm]
	const double MWDC2_zpos = 29.299; // [cm]
	const double MWDC2_length = 52.0; // [cm]
	const double MWDC2_height = 113.0; //[cm]
	const double MWDC2_tilt  = 10.0; // [deg]

	/// Position and tilt of the center of the dE-plane
	const double dE_xpos = 225.0 + 100.0*0.9848; // [cm] 
	const double dE_ypos = 0.0; // [cm]
	const double dE_zpos = 77.83; // [cm]
	const double dE_paddle_length = 50.0; // [cm]
	const double dE_paddle_height = 8.6; //[cm]
	const double dE_paddle_thickness = 0.3; //[cm]
	const double dE_tilt  = 10.0; // [deg]

	/// Position and tilt of the center of the E-plane
	const double E_xpos = 225.0 + 110.0*0.9848; // [cm] 
	const double E_ypos = 0.0; // [cm]
	const double E_zpos = 97.520; // [cm]
	const double E_paddle_length = 75.5; // [cm]
	const double E_paddle_height = 7.53; //[cm]
	const double E_paddle_thickness = 1.067; //[cm]
	const double E_tilt  = 10.0; // [deg]

// Technical Variables for internal puposes of ev. display
	
	/// Maximum number of paddles in the scin. plane
	const int MAX_PADDLE_NUM =  100; 

	const int MAX_ROADS_NUM =  500;
	const int MAX_TRACK_NUM =  77;
	const int MAX_HITS_NUM = 100;


#endif
