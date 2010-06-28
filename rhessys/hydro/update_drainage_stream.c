/*--------------------------------------------------------------*/
/* 											*/
/*					update_drainage_stream			*/
/*											*/
/*	update_drainage_stream.c - creates a patch object				*/
/*											*/
/*	NAME										*/
/*	update_drainage_stream.c - creates a patch object				*/
/*											*/
/*	SYNOPSIS									*/
/*	void update_drainage_stream( 							*/
/*					struct patch_object *patch			*/
/*				 			double,			 	*/
/*				 			double,			 	*/
/*				 			double,			 	*/
/*							int,				*/
/*							int)				*/
/*											*/
/* 											*/
/*											*/
/*	OPTIONS										*/
/*											*/
/*											*/
/*	DESCRIPTION									*/
/*											*/
/*											*/
/*											*/
/*											*/
/*	PROGRAMMER NOTES								*/
/*											*/
/*											*/
/*--------------------------------------------------------------*/
#include <stdio.h>
#include "rhessys.h"


void  update_drainage_stream(
								 struct patch_object *patch,
								 double time_int,
								 double	sen_m,
								 double	sen_K,
								 double std_scale,
								 int verbose_flag,
								 int grow_flag)
{
	/*--------------------------------------------------------------*/
	/*	Local function definition.				*/
	/*--------------------------------------------------------------*/
	double  compute_return_flow(
		int,
		double  ,
		double  );
	
	double  compute_delta_water(
		int,
		double,
		double,
		double,
		double,
		double);
	
	double compute_N_leached(
		int,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double,
		double);
	
	double compute_transmissivity_curve(
		double,
		double,
		double,
		double);

	double compute_varbased_returnflow(
		double,
		double,
		double,
		struct litter_object *);

	double compute_varbased_flow(
		double,
		double,
		double,
		double,
		double,
		double);

	double recompute_gamma(	
		struct patch_object *,
		double);
	/*--------------------------------------------------------------*/ 
	/*	Local variable definition.				*/ 
	/*--------------------------------------------------------------*/ 
	int i, j,k, d; 
	double m, Ksat; 
	double return_flow;  /* m */ 
	double N_leached_total, N_leached_to_stream; /* kg/m2 */ 
	double patch_int_depth;  /* m of H2O */
	double available_sat_water, route_to_stream; /* m3 */
	double route_to_surface;
	double  Qin, Qout,Qstr_total;  /* m */
	double gamma, total_gamma, percent_tobe_routed;
	double Nin, Nout;  /* kg/m2 */
	double t1,t2,t3;

	d=0;
	route_to_stream = 0.0;
	N_leached_to_stream = 0.0;
	/*--------------------------------------------------------------*/
	/*	m and K are multiplied by sensitivity analysis variables */
	/*--------------------------------------------------------------*/
	m = patch[0].soil_defaults[0][0].m ;
	Ksat = patch[0].soil_defaults[0][0].Ksat_0 ;

	/*--------------------------------------------------------------*/
	/*	for now there should be no recomputing of gamma for 	*/
	/*	streams because they do not route water to downslope	*/
	/*	neighbours						*/
	/*--------------------------------------------------------------*/
	total_gamma =  patch[0].innundation_list[d].gamma;

	available_sat_water = max(((patch[0].soil_defaults[0][0].soil_water_cap
			- max(patch[0].sat_deficit,0.0))
			* patch[0].area),0.0);


	/*------------------------------------------------------------*/
	/*	calculate amuount of water output to stream as baseflow */
	/*-----------------------------------------------------------*/
	if (total_gamma < ZERO ) {
		gamma = Ksat * m * 2.0 * sqrt(patch[0].area)
			* time_int;
	}
	else {
		gamma = total_gamma * time_int;

	}

	
	route_to_stream = compute_varbased_flow(patch[0].std * std_scale, gamma, m,
		patch[0].soil_defaults[0][0].active_zone_z,
		patch[0].sat_deficit,
		patch[0].soil_defaults[0][0].soil_water_cap) ;

	if (route_to_stream < 0.0) route_to_stream = 0.0;
	if ( route_to_stream > available_sat_water) 
		route_to_stream *= (available_sat_water)/(route_to_stream);
	
	/*--------------------------------------------------------------*/
	/* compute Nitrogen leaching amount with baseflow		*/
	/*--------------------------------------------------------------*/
	if (grow_flag > 0) {

		N_leached_to_stream = compute_N_leached(
			verbose_flag,
			patch[0].soil_ns.nitrate,
			route_to_stream / patch[0].area,
			patch[0].sat_deficit,
			patch[0].soil_defaults[0][0].soil_water_cap,
			m,
			gamma / patch[0].area,
			patch[0].soil_defaults[0][0].porosity_0,
			patch[0].soil_defaults[0][0].porosity_decay,
			patch[0].soil_defaults[0][0].N_decay_rate,
			patch[0].soil_defaults[0][0].active_zone_z,
			patch[0].soil_defaults[0][0].soil_depth,
			patch[0].soil_defaults[0][0].mobile_N_proportion);
		patch[0].soil_ns.Qout += N_leached_to_stream;
	}


	patch[0].Qout += (route_to_stream / patch[0].area);
	patch[0].base_flow += route_to_stream / patch[0].area;
	patch[0].streamflow_N += N_leached_to_stream;

	/*--------------------------------------------------------------*/
	/*	calculate any return flow to the stream in this patch   */
	/*	and route any infiltration excess			*/
	/*--------------------------------------------------------------*/
	if ((patch[0].sat_deficit-patch[0].rz_storage-patch[0].unsat_storage) < -1.0*ZERO) {
		return_flow = compute_varbased_returnflow(patch[0].std * std_scale, 
			patch[0].rz_storage+patch[0].unsat_storage,
			patch[0].sat_deficit, &(patch[0].litter));
		patch[0].detention_store += return_flow;  
		patch[0].sat_deficit += (return_flow - (patch[0].unsat_storage+patch[0].rz_storage));;
		patch[0].unsat_storage = 0.0;
		patch[0].rz_storage = 0.0;
	}

	/*--------------------------------------------------------------*/
	/*	calculated any N-transport associated with return flow  */
	/*	- note available N reduced by what has already been lost  */
	/*	due to subsurface routing above				*/
	/* 	note only nitrate is assumed to follow return flow		*/
	/*--------------------------------------------------------------*/
	if (return_flow > ZERO) {
		Nout = compute_N_leached(
			verbose_flag,
			patch[0].soil_ns.nitrate - N_leached_to_stream,
			return_flow,
			0.0,
			0.0,
			m,
			gamma / patch[0].area,
			patch[0].soil_defaults[0][0].porosity_0,
			patch[0].soil_defaults[0][0].porosity_decay,
			patch[0].soil_defaults[0][0].N_decay_rate,
			patch[0].soil_defaults[0][0].active_zone_z,
			patch[0].soil_defaults[0][0].soil_depth,
			patch[0].soil_defaults[0][0].mobile_N_proportion);
		patch[0].surface_NO3 += Nout;
		patch[0].soil_ns.Qout += Nout;
	}

	/*--------------------------------------------------------------*/
	/*	route water and nitrogen lossed due to infiltration excess */
	/*	note we assume that this happens before return_flow losses */
	/*--------------------------------------------------------------*/

	if ((patch[0].detention_store > patch[0].soil_defaults[0][0].detention_store_size) &&
		(patch[0].detention_store > ZERO)) {
		Qout = (patch[0].detention_store - patch[0].soil_defaults[0][0].detention_store_size);
		Nout = (min(1.0, Qout / patch[0].detention_store)) * patch[0].surface_NO3;
		patch[0].surface_NO3  -= Nout;
		patch[0].streamflow_N += Nout;
		patch[0].surface_ns_leach += Nout;
		Nout = (min(1.0, Qout / patch[0].detention_store)) * patch[0].surface_NH4;
		patch[0].surface_NH4  -= Nout;
		patch[0].streamflow_N += Nout;
		patch[0].detention_store -= Qout;
		patch[0].return_flow += Qout; 
		}

} /*end update_drainage_stream.c*/

