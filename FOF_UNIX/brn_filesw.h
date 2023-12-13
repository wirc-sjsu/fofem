/*****************************************************
* Name:
* Desc: Switches used in Burnup input file.             
*       I put these switch defines in their own .h 
*       because they get used by fofem dll to read in file
*       and by FOFEM GUI to create a sample file
*  Date: 5/11/2015
******************************************************/

#define e_swMAX_TIMES      "Max_Times"  
#define e_swINTENSITY      "Intensity"
#define e_swIG_TIME        "Ig_Time"
#define e_swWINDSPEED      "Windspeed"
#define e_swDEPTH          "Depth"   
#define e_swAMBIENT_TEMP   "Ambient_Temp"
#define e_swR0             "r0"         
#define e_swDR             "dr"         
#define e_swTIMESTEP       "Timestep"
#define e_swDUFF_LOAD      "Duff_Load"
#define e_swDUFF_MOIST     "Duff_Moist" 


/* NOTE - these contain the '#' on front of the switch */
#define e_swpMAX_TIMES      "#Max_Times"  
#define e_swpINTENSITY      "#Intensity"
#define e_swpIG_TIME        "#Ig_Time"
#define e_swpWINDSPEED      "#Windspeed"
#define e_swpDEPTH          "#Depth"   
#define e_swpAMBIENT_TEMP   "#Ambient_Temp"
#define e_swpR0             "#r0"         
#define e_swpDR             "#dr"   
#define e_swpTIMESTEP       "#Timestep"

