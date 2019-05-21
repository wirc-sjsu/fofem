/*{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}
* Name: bat_bif2.c
* Desc: Define the CDF (comma delimited file) structures and other needed
*       to read in the Batch input file.
*
* NOTE: this file is only included in the bat_bif.c becauser there are
*        variable/struct declaration in here
*
* Note-1: 10-28-2014  removed Duff Method type column in the input file,
*                     program will always use Entire
*                     All I had to do was have the s_CI input struct get
*                     initialized with "Entire" and then comment out 
*                     the Duff-Moist-Meth lines of code below in each
*                     of the CDF table (see below)
*                     This change can easily be undone by uncommenting
*                     the lines mentioned above 
{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}{*}*/

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
/*                                                                           */
/* There are 4 separate d_CDF tables declared, 1 for each of the types       */
/* of batch input that can get used.                                         */
/*   1. Normal - where ah 1000 hour total full is used                       */
/*   2. Normal plus soil inputs                                              */
/*   3. 1000 hour fuel Sound/Rotten with all other inputs being the same     */
/*   4. 1000 hour, like 3. but with soil inputs on end                       */

/*.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- */
d_CI gs_CI;
d_SI gs_SI;

char  gcr_StdId[eC_StdId];
char  gcr_End[100];

/* This tells where in the table the User Equation Numbers (UEN) begin */
/* A switch in the batch files specifies if the file contains UEN */
/* If the file is not of the UEN format than the table gets the e_EqLitter */
/* entry marked as the end of the table, which prevent the other fields */
/* from getting read in, else an error would occur when the CDF functions */
/* try to read data that isn't there */ 
/* The e_EgLitter and following rows below must be at the end of the table */ 
#define e_EqLitter  "Eq-Litter"

/*...........................................................................*/
/* Consumed Standard Input file using the 1000 hour total load                        */
d_CDF  sr_cdr[] = {
     { "StandId",             e_Str,  eC_StdId,     &gcr_StdId      },
     { "Litter",              e_Flo,  0,            &gs_CI.f_Lit           },
     { "DW-1Hr",              e_Flo,  0,            &gs_CI.f_DW1           },
     { "DW-10Hr",             e_Flo,  0,            &gs_CI.f_DW10          },
     { "10Hr-Moisture",       e_Flo,  0,            &gs_CI.f_MoistDW10     },
     { "DW-100Hr",            e_Flo,  0,            &gs_CI.f_DW100         },
     { "DW1000Hr",            e_Flo,  0,            &gs_CI.f_DW1000  },
     { "DW1000-Moisture",     e_Flo,  0,            &gs_CI.f_MoistDW1000  },
     { "Percent-Rotten",      e_Flo,  0,            &gs_CI.f_pcRot     },
     { "Weight-Distribution", e_Str,  eC_WD,         gs_CI.cr_WD        },
     { "Duff-Load",           e_Flo,  0,             &gs_CI.f_Duff},
     { "Duff-Moisture",       e_Flo,  0,             &gs_CI.f_MoistDuff},
     { "Duff-Depth",          e_Flo,  0,             &gs_CI.f_DufDep},
// see Note-1 above   { "Duff-Moist-Method",   e_Str,  eC_DufMoiMet,   gs_CI.cr_DufMoiMet},
     { "Herbaceous",          e_Flo,  0,             &gs_CI.f_Herb},
     { "Shrub",               e_Flo,  0,             &gs_CI.f_Shrub},
     { "Crown-Foliage",       e_Flo,  0,             &gs_CI.f_CroFol},
     { "Crown-Branch",        e_Flo,  0,             &gs_CI.f_CroBra},
     { "Crown-Branch-Percent",e_Flo,  0,             &gs_CI.f_Pc_CroBrn},
     { "Region",              e_Str,  eC_Region,      gs_CI.cr_Region},
     { "Cover-Group",         e_Str,  eC_CoverGroup,  gs_CI.cr_CoverGroup},
     { "Season",              e_Str,  eC_Season,      gs_CI.cr_Season},
     { "Fuel-Category",       e_Str,  eC_FuelCategory,gs_CI.cr_FuelCategory},  
     { e_EqLitter,            e_Int,  0,              &gs_CI.i_EqLit }, 
     { "Eq-DuffLoad",         e_Int,  0,              &gs_CI.i_EgDufLoa }, 
     { "Eq-DuffDepth",        e_Int,  0,              &gs_CI.i_EqDufDep }, 
     { "Eq-MineralSoil",      e_Int,  0,              &gs_CI.i_EqMSE}, 
     { "Eq-Herb",             e_Int,  0,              &gs_CI.i_EqHerb}, 
     { "Eq-Shrub",            e_Int,  0,              &gs_CI.i_EqShrub}, 
     { "",                    e_End,  0,              gcr_End         }};

/*...........................................................................*/
/* Soil Standard Input file using the 1000 hour total load        */
d_CDF  sr_cdrS[] = {
     { "StandId",             e_Str,  eC_StdId,       &gcr_StdId    },
     { "Litter",              e_Flo,  0,              &gs_CI.f_Lit    },
     { "DW-1Hr",              e_Flo,  0,              &gs_CI.f_DW1    },
     { "DW-10Hr",             e_Flo,  0,              &gs_CI.f_DW10   },
     { "10Hr-Moisture",       e_Flo,  0,              &gs_CI.f_MoistDW10},
     { "DW-100Hr",            e_Flo,  0,              &gs_CI.f_DW100    },
     { "DW1000Hr",            e_Flo,  0,              &gs_CI.f_DW1000  },
     { "DW1000-Moisture",     e_Flo,  0,              &gs_CI.f_MoistDW1000},
     { "Percent-Rotten",      e_Flo,  0,              &gs_CI.f_pcRot    },
     { "Weight-Distribution", e_Str,  eC_WD,           gs_CI.cr_WD      },
     { "Duff-Load",           e_Flo,  0,              &gs_CI.f_Duff},
     { "Duff-Moisture",       e_Flo,  0,              &gs_CI.f_MoistDuff},
     { "Duff-Depth",          e_Flo,  0,              &gs_CI.f_DufDep},
// see Note-1 above    { "Duff-Moist-Method",   e_Str,  eC_DufMoiMet,    gs_CI.cr_DufMoiMet}, 
     { "Herbaceous",          e_Flo,  0,              &gs_CI.f_Herb},
     { "Shrub",               e_Flo,  0,              &gs_CI.f_Shrub},
     { "Crown-Foliage",       e_Flo,  0,              &gs_CI.f_CroFol},
     { "Crown-Branch",        e_Flo,  0,              &gs_CI.f_CroBra},
     { "Crown-Branch-Percent",e_Flo,  0,              &gs_CI.f_Pc_CroBrn},
     { "Region",              e_Str,  eC_Region,       gs_CI.cr_Region},
     { "Cover-Group",         e_Str,  eC_CoverGroup,   gs_CI.cr_CoverGroup},
     { "Season",              e_Str,  eC_Season,       gs_CI.cr_Season},
     { "Fuel-Category",       e_Str,  eC_FuelCategory, gs_CI.cr_FuelCategory},
     { "Soil Type",           e_Str,  eC_SoilType,     gs_SI.cr_SoilType },
     { "Soil Moisture",       e_Flo,  0,              &gs_SI.f_SoilMoist },
 
     { e_EqLitter,            e_Int,  0,              &gs_CI.i_EqLit }, 
     { "Eq-DuffLoad",         e_Int,  0,              &gs_CI.i_EgDufLoa }, 
     { "Eq-DuffDepth",        e_Int,  0,              &gs_CI.i_EqDufDep }, 
     { "Eq-MineralSoil",      e_Int,  0,              &gs_CI.i_EqMSE}, 
     { "Eq-Herb",             e_Int,  0,              &gs_CI.i_EqHerb}, 
     { "Eq-Shrub",            e_Int,  0,              &gs_CI.i_EqShrub}, 

//   { "Moisture Condition",  e_Str,  eC_MoistCond,    gs_SI.cr_MoistCond},
     { "",                    e_End,  0,               gcr_End        }};


/*...........................................................................*/
/* 1000 Hour Sound/Rotten input                                              */
d_CDF  sr_cdrSR[] = {
     { "StandId",             e_Str,  eC_StdId, &gcr_StdId      },
     { "Litter",              e_Flo,  0,   &gs_CI.f_Lit           },
     { "DW-1Hr",              e_Flo,  0,   &gs_CI.f_DW1           },
     { "DW-10Hr",             e_Flo,  0,   &gs_CI.f_DW10          },
     { "10Hr-Moisture",       e_Flo,  0,   &gs_CI.f_MoistDW10     },
     { "DW-100Hr",            e_Flo,  0,   &gs_CI.f_DW100         },
     { "SizeClass3-Sound",    e_Flo,  0,   &gs_CI.f_Snd_DW3 },
     { "SizeClass6-Sound",    e_Flo,  0,   &gs_CI.f_Snd_DW6 },
     { "SizeClass9-Sound",    e_Flo,  0,   &gs_CI.f_Snd_DW9 },
     { "SizeClass20-Sound",   e_Flo,  0,   &gs_CI.f_Snd_DW20},
     { "SizeClass3-Rotten",   e_Flo,  0,   &gs_CI.f_Rot_DW3 },
     { "SizeClass6-Rotten",   e_Flo,  0,   &gs_CI.f_Rot_DW6 },
     { "SizeClass9-Rotten",   e_Flo,  0,   &gs_CI.f_Rot_DW9 },
     { "SizeClass20-Rotten",  e_Flo,  0,   &gs_CI.f_Rot_DW20},
     { "1000 Hour Moisture",  e_Flo,  0,   &gs_CI.f_MoistDW1000},
     { "Duff-Load",           e_Flo,  0,             &gs_CI.f_Duff},
     { "Duff-Moisture",       e_Flo,  0,             &gs_CI.f_MoistDuff},
     { "Duff-Depth",          e_Flo,  0,             &gs_CI.f_DufDep},
// see Note-1 above    { "Duff-Moist-Method",   e_Str,  eC_DufMoiMet,   gs_CI.cr_DufMoiMet}, 
     { "Herbaceous",          e_Flo,  0,             &gs_CI.f_Herb},
     { "Shrub",               e_Flo,  0,             &gs_CI.f_Shrub},
     { "Crown-Foliage",       e_Flo,  0,             &gs_CI.f_CroFol},
     { "Crown-Branch",        e_Flo,  0,             &gs_CI.f_CroBra},
     { "Crown-Branch-Percent",e_Flo,  0,             &gs_CI.f_Pc_CroBrn},
     { "Region",              e_Str,  eC_Region,      gs_CI.cr_Region},
     { "Cover-Group",         e_Str,  eC_CoverGroup,  gs_CI.cr_CoverGroup},
     { "Season",              e_Str,  eC_Season,      gs_CI.cr_Season},
     { "Fuel-Category",       e_Str,  eC_FuelCategory,gs_CI.cr_FuelCategory},
 
     { e_EqLitter,            e_Int,  0,              &gs_CI.i_EqLit }, 
     { "Eq-DuffLoad",         e_Int,  0,              &gs_CI.i_EgDufLoa }, 
     { "Eq-DuffDepth",        e_Int,  0,              &gs_CI.i_EqDufDep }, 
     { "Eq-MineralSoil",      e_Int,  0,              &gs_CI.i_EqMSE}, 
     { "Eq-Herb",             e_Int,  0,              &gs_CI.i_EqHerb}, 
     { "Eq-Shrub",            e_Int,  0,              &gs_CI.i_EqShrub}, 
     { "",                    e_End,  0,               gcr_End         }};

/*...........................................................................*/
/* 1000 Hour Sound/Rotten input with SOIL inputs on end                      */
d_CDF  sr_cdrSRS[] = {
     { "StandId",             e_Str,  eC_StdId, &gcr_StdId      },
     { "Litter",              e_Flo,  0,   &gs_CI.f_Lit           },
     { "DW-1Hr",              e_Flo,  0,   &gs_CI.f_DW1           },
     { "DW-10Hr",             e_Flo,  0,   &gs_CI.f_DW10          },
     { "10Hr-Moisture",       e_Flo,  0,   &gs_CI.f_MoistDW10     },
     { "DW-100Hr",            e_Flo,  0,   &gs_CI.f_DW100         },
     { "SizeClass3-Sound",    e_Flo,  0,   &gs_CI.f_Snd_DW3 },
     { "SizeClass6-Sound",    e_Flo,  0,   &gs_CI.f_Snd_DW6 },
     { "SizeClass9-Sound",    e_Flo,  0,   &gs_CI.f_Snd_DW9 },
     { "SizeClass20-Sound",   e_Flo,  0,   &gs_CI.f_Snd_DW20},
     { "SizeClass3-Rotten",   e_Flo,  0,   &gs_CI.f_Rot_DW3 },
     { "SizeClass6-Rotten",   e_Flo,  0,   &gs_CI.f_Rot_DW6 },
     { "SizeClass9-Rotten",   e_Flo,  0,   &gs_CI.f_Rot_DW9 },
     { "SizeClass20-Rotten",  e_Flo,  0,   &gs_CI.f_Rot_DW20},
     { "1000 Hour Moisture",  e_Flo,  0,   &gs_CI.f_MoistDW1000},
     { "Duff-Load",           e_Flo,  0,             &gs_CI.f_Duff},
     { "Duff-Moisture",       e_Flo,  0,             &gs_CI.f_MoistDuff},
     { "Duff-Depth",          e_Flo,  0,             &gs_CI.f_DufDep},
// see Note-1 above    { "Duff-Moist-Method",   e_Str,  eC_DufMoiMet,   gs_CI.cr_DufMoiMet},
     { "Herbaceous",          e_Flo,  0,             &gs_CI.f_Herb},
     { "Shrub",               e_Flo,  0,             &gs_CI.f_Shrub},
     { "Crown-Foliage",       e_Flo,  0,             &gs_CI.f_CroFol},
     { "Crown-Branch",        e_Flo,  0,             &gs_CI.f_CroBra},
     { "Crown-Branch-Percent",e_Flo,  0,             &gs_CI.f_Pc_CroBrn},
     { "Region",              e_Str,  eC_Region,      gs_CI.cr_Region},
     { "Cover-Group",         e_Str,  eC_CoverGroup,  gs_CI.cr_CoverGroup},
     { "Season",              e_Str,  eC_Season,      gs_CI.cr_Season},
     { "Fuel-Category",       e_Str,  eC_FuelCategory,gs_CI.cr_FuelCategory},
     { "Soil Type",           e_Str,  eC_SoilType,     gs_SI.cr_SoilType },
     { "Soil Moisture",       e_Flo,  0,              &gs_SI.f_SoilMoist },
 //    { "Moisture Condition",  e_Str,  eC_MoistCond,    gs_SI.cr_MoistCond},

     { e_EqLitter,            e_Int,  0,              &gs_CI.i_EqLit }, 
     { "Eq-DuffLoad",         e_Int,  0,              &gs_CI.i_EgDufLoa }, 
     { "Eq-DuffDepth",        e_Int,  0,              &gs_CI.i_EqDufDep }, 
     { "Eq-MineralSoil",      e_Int,  0,              &gs_CI.i_EqMSE}, 
     { "Eq-Herb",             e_Int,  0,              &gs_CI.i_EqHerb}, 
     { "Eq-Shrub",            e_Int,  0,              &gs_CI.i_EqShrub}, 
     { "",                    e_End,  0,               gcr_End         }};


/*...........................................................................*/
/* Coastal Plain - Standard Input file                                       */
/* This file is the same as a Standard with the inclusion of Litter Moisture */
/*  immediately following the litter load                                    */
/* NOTE: So far of coast plain I haven't made it work with Soil or 1k Hour   */
/*       individual size clss loads                                          */
/*                                                                           */
d_CDF  sr_cdrCP[] = {
     { "StandId",             e_Str,  eC_StdId,     &gcr_StdId      },
     { "Litter",              e_Flo,  0,            &gs_CI.f_Lit           },
     { "Lit-Moisture",        e_Flo,  0,            &gs_CI.f_LitMoi        },
     { "DW-1Hr",              e_Flo,  0,            &gs_CI.f_DW1           },
     { "DW-10Hr",             e_Flo,  0,            &gs_CI.f_DW10          },
     { "10Hr-Moisture",       e_Flo,  0,            &gs_CI.f_MoistDW10     },
     { "DW-100Hr",            e_Flo,  0,            &gs_CI.f_DW100         },
     { "DW1000Hr",            e_Flo,  0,            &gs_CI.f_DW1000  },
     { "DW1000-Moisture",     e_Flo,  0,            &gs_CI.f_MoistDW1000  },
     { "Percent-Rotten",      e_Flo,  0,            &gs_CI.f_pcRot     },
     { "Weight-Distribution", e_Str,  eC_WD,         gs_CI.cr_WD        },
     { "Duff-Load",           e_Flo,  0,             &gs_CI.f_Duff},
     { "Duff-Moisture",       e_Flo,  0,             &gs_CI.f_MoistDuff},
     { "Duff-Depth",          e_Flo,  0,             &gs_CI.f_DufDep},
// see Note-1 above    { "Duff-Moist-Method",   e_Str,  eC_DufMoiMet,   gs_CI.cr_DufMoiMet},
     { "Herbaceous",          e_Flo,  0,             &gs_CI.f_Herb},
     { "Shrub",               e_Flo,  0,             &gs_CI.f_Shrub},
     { "Crown-Foliage",       e_Flo,  0,             &gs_CI.f_CroFol},
     { "Crown-Branch",        e_Flo,  0,             &gs_CI.f_CroBra},
     { "Crown-Branch-Percent",e_Flo,  0,             &gs_CI.f_Pc_CroBrn},
     { "Region",              e_Str,  eC_Region,      gs_CI.cr_Region},
     { "Cover-Group",         e_Str,  eC_CoverGroup,  gs_CI.cr_CoverGroup},
     { "Season",              e_Str,  eC_Season,      gs_CI.cr_Season},
     { "Fuel-Category",       e_Str,  eC_FuelCategory,gs_CI.cr_FuelCategory},
     { "",            e_End,         0,        ""         }};
