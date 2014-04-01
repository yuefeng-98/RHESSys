#pragma once

#ifdef __cplusplus
extern "C" {
#endif
/************************fire.h**************************/
/* shared header file between RHESSys and WMFire	*/
/**********************************************************/
struct fire_default {
	int ID;
	double veg_fuel_weighting ;	
	double load_k1; // k1 for the fuel load sigmoid function
	double moisture_k1; //k1 for the fuel moisture sigmoid function
	double load_k2; // k2 for the fuel load sigmoid function
	double moisture_k2; // k2 for the fuel moisture sigmoid function
	double winddir_k1; // the modifying constant for wind direction, depending on windspeed
	double winddir_k2; // intercept for wind direction, giving the probability of backing fire
	double windmax; // maximum wind speed to calculate relative windspeed
	double slope_k1; // k1 for the concave up slope function (k1*exp(k2*slope^2)), where sign(k2) is sign(slope)
	double slope_k2; // k2 for the concave up slope function (k1*exp(k2*slope^2)), where sign(k2) is sign(slope)
	double ignition_tmin; // the minimum temperature for a possible ignition
	double ignition_col; // allow for ignition x and y coordinates, but not yet implemented. default to -1
	double ignition_row;
	int fire_write; // should the fire results be written to a file?
	int n_rows;
	int n_cols;
	int fire_in_buffer;
	int spread_calc_type;
	double ndays_average;
//	char **patch_file_name;
};

// this will replace the fire_nodes 
struct fire_object 
{
	double burn;			/* 0-1 */
	double fuel_veg;  		/* kgC/m2 */
	double fuel_litter; 		/* kgC/m2 */
	double fuel_moist; 		/* 0-1 */
	double soil_moist; 		/* 0-1 */
	double z; 			/* m */
	double wind; 			/* m/s */
	double wind_direction; 		/*degrees */
	double relative_humidity;	/* 0-1 */
	double temp; /* temperature of the cell */
	double et;
	double pet;
};	

#ifdef __cplusplus
}
#endif