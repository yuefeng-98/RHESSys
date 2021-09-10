/*--------------------------------------------------------------*/
/* 								*/
/*		zero_patch_daily_flux				*/
/*								*/
/*								*/
/*	NAME							*/
/*		zero_patch_daily_flux
/*								*/
/*	SYNOPSIS						*/
/*	double	zero_patch_daily_flux(				*/
/*			struct cdayflux_patch_struct *,		*/
/*			struct ndayflux_patch_struct *,		*/
/*				)				*/
/*								*/
/*	returns:						*/
/*								*/
/*	OPTIONS							*/
/*								*/
/*	DESCRIPTION						*/
/*								*/
/*	zeros all daily patch fluxes (carbon and nitrogen)	*/
/*								*/
/*	PROGRAMMER NOTES					*/
/*								*/
/*								*/
/*--------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "rhessys.h"
#include "phys_constants.h"

int zero_patch_daily_flux( struct patch_object *patch,
				struct cdayflux_patch_struct* cdf,
						  struct ndayflux_patch_struct* ndf)
{
	/*------------------------------------------------------*/
	/*	Local Function Declarations.						*/
	/*------------------------------------------------------*/

	/*------------------------------------------------------*/
	/*	Local Variable Definition. 							*/
	/*------------------------------------------------------*/
	int ok=1;

	/*--------------------------------------------------------------*/
	/* daily hydro  & N fluxes					*/
	/*--------------------------------------------------------------*/
	patch[0].return_flow = 0.0;
	patch[0].streamflow = 0.0;
	patch[0].streamflow_NO3 = 0.0;
	patch[0].streamflow_NH4 = 0.0;
	patch[0].base_flow = 0.0;
	patch[0].gw_drainage = 0.0;
	patch[0].snowpack.sublimation = 0.0;
	patch[0].evaporation_surf = 0.0;
	patch[0].Kup_direct = 0.0;
	patch[0].Kup_diffuse = 0.0;

	/*--------------------------------------------------------------*/
	/* daily fire fluxes 						*/
	/*--------------------------------------------------------------*/

	patch[0].fire.litter_c_consumed = 0.0;
	/*--------------------------------------------------------------*/
	/* daily carbon fluxes */
	/*--------------------------------------------------------------*/
	/* daily  DOC fluxes */
	cdf->do_litr1c_loss = 0.0;
	cdf->do_litr2c_loss = 0.0;
	cdf->do_litr3c_loss = 0.0;
	cdf->do_litr4c_loss = 0.0;
	//below ground litter loss
	cdf->do_litr1c_loss_bg = 0.0;
	cdf->do_litr2c_loss_bg = 0.0;
	cdf->do_litr3c_loss_bg = 0.0;
	cdf->do_litr4c_loss_bg = 0.0;
	//soil
	cdf->do_soil1c_loss = 0.0;
	cdf->do_soil2c_loss = 0.0;
	cdf->do_soil3c_loss = 0.0;
	cdf->do_soil4c_loss = 0.0;
	cdf->total_DOC_loss = 0.0;
	/* daily turnover fluxes */
	cdf->leafc_to_litr1c = 0.0;
	cdf->leafc_to_litr2c = 0.0;
	cdf->leafc_to_litr3c = 0.0;
	cdf->leafc_to_litr4c = 0.0;
	cdf->frootc_to_litr1c_bg = 0.0; //root go to below ground litter
	cdf->frootc_to_litr2c_bg = 0.0;
	cdf->frootc_to_litr3c_bg = 0.0;
	cdf->frootc_to_litr4c_bg = 0.0;
	/* decomposition and heterotroph respiration fluxes */
	cdf->litr1c_hr = 0.0;
	cdf->litr1c_to_soil1c = 0.0;
	cdf->litr2c_hr = 0.0;
	cdf->litr2c_to_soil2c = 0.0;
	cdf->litr3c_hr = 0.0;
	cdf->litr3c_to_litr2c = 0.0;
	cdf->litr4c_hr = 0.0;
	cdf->litr4c_to_soil3c = 0.0;
	// below ground litter
	cdf->litr1c_hr_bg = 0.0;
	cdf->litr1c_to_soil1c_bg = 0.0;
	cdf->litr2c_hr_bg = 0.0;
	cdf->litr2c_to_soil2c_bg = 0.0;
	cdf->litr3c_hr_bg = 0.0;
	cdf->litr3c_to_litr2c_bg = 0.0;
	cdf->litr4c_hr_bg = 0.0;
	cdf->litr4c_to_soil3c_bg = 0.0;

	//soil
	cdf->soil1c_hr = 0.0;
	cdf->soil1c_to_soil2c = 0.0;
	cdf->soil2c_hr = 0.0;
	cdf->soil2c_to_soil3c = 0.0;
	cdf->soil3c_hr = 0.0;
	cdf->soil3c_to_soil4c = 0.0;
	cdf->soil4c_hr = 0.0;
	/*--------------------------------------------------------------*/
	/* daily nitrogen fluxes */
	/*--------------------------------------------------------------*/
	/* daily  DON fluxes */
	ndf->do_litr1n_loss = 0.0;
	ndf->do_litr2n_loss = 0.0;
	ndf->do_litr3n_loss = 0.0;
	ndf->do_litr4n_loss = 0.0;
	//below ground litter
	ndf->do_litr1n_loss_bg = 0.0;
	ndf->do_litr2n_loss_bg = 0.0;
	ndf->do_litr3n_loss_bg = 0.0;
	ndf->do_litr4n_loss_bg = 0.0;

	//soil
	ndf->do_soil1n_loss = 0.0;
	ndf->do_soil2n_loss = 0.0;
	ndf->do_soil3n_loss = 0.0;
	ndf->do_soil4n_loss = 0.0;
	ndf->total_DON_loss = 0.0;
	/* daily turnover fluxes */
	ndf->leafn_to_litr1n = 0.0;
	ndf->leafn_to_litr2n = 0.0;
	ndf->leafn_to_litr3n = 0.0;
	ndf->leafn_to_litr4n = 0.0;
	ndf->frootn_to_litr1n_bg = 0.0; //root go to below ground litter
	ndf->frootn_to_litr2n_bg = 0.0;
	ndf->frootn_to_litr3n_bg = 0.0;
	ndf->frootn_to_litr4n_bg = 0.0;
	/* daily N immobilization & mineralization fluxes */
	ndf->litr1n_to_soil1n = 0.0;
	ndf->sminn_to_soil1n_l1 = 0.0;
	ndf->litr2n_to_soil2n = 0.0;
	ndf->sminn_to_soil2n_l2 = 0.0;
	ndf->litr3n_to_litr2n = 0.0;
	ndf->sminn_to_soil2n_l3 = 0.0;
	ndf->litr4n_to_soil3n = 0.0;
	ndf->sminn_to_soil3n_l4 = 0.0;
	//below ground litter carbon
	ndf->litr1n_to_soil1n_bg = 0.0;
	ndf->sminn_to_soil1n_l1_bg = 0.0;
	ndf->litr2n_to_soil2n_bg = 0.0;
	ndf->sminn_to_soil2n_l2_bg = 0.0;
	ndf->litr3n_to_litr2n_bg = 0.0;
	ndf->sminn_to_soil2n_l3_bg = 0.0;
	ndf->litr4n_to_soil3n_bg = 0.0;
	ndf->sminn_to_soil3n_l4_bg = 0.0;

    //soil
	ndf->soil1n_to_soil2n = 0.0;
	ndf->sminn_to_soil2n_s1 = 0.0;
	ndf->soil2n_to_soil3n = 0.0;
	ndf->sminn_to_soil3n_s2 = 0.0;
	ndf->soil3n_to_soil4n = 0.0;
	ndf->sminn_to_soil4n_s3 = 0.0;
	ndf->soil4n_to_sminn = 0.0;
	ndf->sminn_to_npool = 0.0;
	ndf->plant_potential_ndemand = 0.0;
	/* deposition, fixation and leaching fluxes */
	ndf->nfix_to_sminn = 0.0;
	ndf->ndep_to_sminn = 0.0;
	/* denitrification fluxes */
	ndf->sminn_to_nvol = 0.0;
	ndf->denitrif = 0.0;
	ndf->sminn_to_nitrate = 0.0;
	ndf->nfix_to_sminn = 0.0;
	ndf->DON_to_gw = 0.0;
	ndf->N_to_gw = 0.0;

	/*-------------------------------
	ndf->sminn_to_nvol_l1s1 = 0.0;
		  ndf->sminn_to_nvol_l2s2 = 0.0;
		  ndf->sminn_to_nvol_l4s3 = 0.0;
		  ndf->sminn_to_nvol_s1s2 = 0.0;
		  ndf->sminn_to_nvol_s2s3 = 0.0;
		  ndf->sminn_to_nvol_s3s4 = 0.0;
		  ndf->sminn_to_nvol_s4 = 0.0;
	--------------------------------*/
	return (!ok);
}/* end zero_patch_daily_flux */





