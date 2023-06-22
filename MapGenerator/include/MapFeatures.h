#pragma once

/*
	This file stores all pre-defined terrain features (analgous to "doodads" in StarEdit) that the map generator can seed the map with during terrain generation.
*/

struct Feature
{
	int tiles[100],	// This is large enough to store the big volcano.
	    width,
	    height;
};

static Feature allFeatures[] =
{
	// Gray rock features
/* # 0 */	{{70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87},3,6},												// Gray cliff, long rounded corner, bottom left
/* # 1 */	{{ 163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186 },4,6},	// Gray cliff, long rounded corner, bottom right
/* # 2 */	{{ 223,224,225,226,227,228,229,230,231,232 },2,5},															// Gray cliff, straight, left edge
/* # 3 */	{{ 233,234,235,236,237,238,239,240,241,242 },2,5},															// Gray cliff, straight, right edge
/* # 4 */	{{ 121,122,123,124,125,126,127,128,129},3,3},																// Gray cliff, end, left
/* # 5 */	{{ 148,149,150,151,152,153,154,155,156},3,3},																// Gray cliff, end, right
/* # 6 */	{{ 88,89,90,91,92,93},2,3},																					// Gray cliff, rounded corner, top right
/* # 7 */	{{ 94,95,96,97,98,99,100,101,102},3,3},																		// Gray cliff, rounded corner, top left
/* # 8 */	{{ 214,215,216,217,218,219,220,221,222},3,3},																// Gray cliff, rounded corner, bottom left
/* # 9 */	{{ 187,188,189,190,191,192,193,194,195},3,3},																// Gray cliff, rounded corner, bottom right
/* #10 */	{{ 130,131,132,133,134,135,136,137,138},3,3},																// Gray cliff, sharp corner, top left
/* #11 */	{{ 139,140,141,142,143,144,145,146,147},3,3},																// Gray cliff, sharp corner, top right
/* #12 */	{{ 196,197,198,199,200,201,202,203,204},3,3},																// Gray cliff, sharp corner, bottom left
/* #13 */	{{ 205,206,207,208,209,210,211,212,213},3,3},																// Gray cliff, sharp corner, bottom right
/* #14 */	{{ 103,104,105,106,107,108,109,110,111},3,3},																// Gray cliff, straight, bottom edge (variant 1)
/* #15 */	{{ 112,113,114,115,116,117,118,119,120},3,3},																// Gray cliff, straight, bottom edge (variant 2)
/* #16 */	{{ 157,158,159},3,1},																						// Gray cliff, straight, top edge (variant 1)
/* #17 */	{{ 160,161,162},3,1},																						// Gray cliff, straight, top edge (variant 2)
/* #18 */	{{ 243,244,245,246},2,2},																					// Gray rock (2x2)
/* #19 */	{{ 247,248,249,250,251,252,253,254,255},3,3},																// Gray rock (3x3)
/* #20 */	{{ 380,381},2,1},																							// Gray wreckage (2x1)
/* #21 */	{{ 382,383,384,385},2,2},																					// Gray wreckage (2x2) (variant 1)
/* #22 */	{{ 386,387,388,389},2,2},																					// Gray wreckage (2x2) (variant 2)
/* #23 */	{{ 391,392,393,394},2,2},																					// Gray wreckage (2x2) (variant 3)
/* #24 */	{{ 401,402,403,404,405,406},3,2},																			// Gray wreckage (3x2)
/* #25 */	{{ 33,34,35,36,37,38},2,3},																					// Gray imprint (2x3) (variant 1)
/* #26 */	{{ 40,41,42,43,44,45},2,3},																					// Gray imprint (2x3) (variant 2)
/* #27 */	{{ 46,47,48,49,50,51},3,2},																					// Gray imprint (3x2)
/* #28 */	{{ 52,53,54,55,56,57},2,3},																					// Gray imprint (2x3) (variant 3)
/* #29 */	{{ 58,59,60,61},2,2},																						// Gray imprint (2x2) (variant 1)
/* #30 */	{{ 62,63,64,65},2,2},																						// Gray imprint (2x2) (variant 2)
/* #31 */	{{ 67,68},2,1},																								// Gray imprint (2x1)

	// Volcanic rock features
/* # 32 */	{{689,690,691,692,693,694,695,696,697,698,699,700,701,702,703,704,705,706,707,708,							// Volcano
			  709,710,711,712,713,714,715,716,717,718,719,720,721,722,723,724,725,726,727,728,
			  729,730,731,732,733,734,735,736,737,738,739,740,741,742,743,744,745,746,747,748,
			  749,750,751,752,753,754,755,756,757,758,759,760,761,762,763,764,765,766,767,768,
			  769,770,771,772,773,774,775,776,777,778,779,780,781,782,783,784,785,786,787,788},10,10},
/* # 33 */	{{608,609,610,611,612,613,614,615,616,617,618,619,620,621,622,623,624,625,626,627,							// Small volcano (variant 1)
			  628,629,630,631,632,633,634,635,636,637,638,639,640,641,642,643},6,6},
/* # 34 */	{{653,654,655,656,657,658,659,660,661,662,663,664,665,666,667,668,669,670,671,672,							// Small volcano (variant 2)
			  673,674,675,676,677,678,679,680,681,682,683,684,685,686,687,688},6,6},
/* # 35 */	{{644,645,646,647,648,649,650,651,652},3,3},																// Tiny volcano
/* # 36 */	{{537,538,539,540,541,542,543,544,545},3,3},																// Volcanic boulder (3x3) (variant 1)
/* # 37 */	{{550,551,552,553,554,555,556,557,558},3,3},																// Volcanic bolder (3x3) (variant 2)
/* # 38 */	{{546,547,548,549},4,1},																					// Volcanic bolder (4x1)
/* # 39 */	{{505,506,507,508,509,510,511,512,513,514},2,5},															// Volcanic cracks (2x5)
/* # 40 */	{{521,522,523,524,525,526,527,528,529,530,531,532,533,534},2,7},											// Volcanic cracks (2x7)
/* # 41 */	{{515,516},1,2},																							// Volcanic cracks (1x2)
/* # 42 */	{{517,518},2,1},																							// Volcanic cracks (2x1) (Variant 1)
/* # 43 */	{{519,520},2,1},																							// Volcanic cracks (2x1) (Variant 2)
/* # 44 */	{{901,902},1,2},																							// Volcanic wreckage (1x2)
/* # 45 */	{{904,905,906,907},2,2},																					// Volcanic wreckage (2x2) (Variant 1)
/* # 46 */	{{908,909,910,911},2,2},																					// Volcanic wreckage (2x2) (Variant 2)
/* # 47 */	{{912,913,914,915,916,917,918,919,920},3,3}																	// Volcanic wreckage (3x3)

	// Sand dunes features
	


};
