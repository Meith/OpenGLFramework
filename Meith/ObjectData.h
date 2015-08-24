#ifndef __ObjectData__
#define __ObjectData__

static const int object_data[6][6] = 
{
	{16, 48, 48, 72, 72, 1},		// cube
	{21, 114, 28, 114, 114, 1},		// cone
	{84, 480, 88, 480, 480, 1},		// cylinder
	{764, 4560, 802, 4560, 4560, 1},	// sphere
	{400, 1600, 441, 2400, 2400, 1},	// torus
	{2416, 14472, 2464, 14472, 14472, 1}	// helix
};

static const char *object_name[6] = 
{
	"objects/cube.obj",
	"objects/cone.obj",
	"objects/cylinder.obj",
	"objects/sphere.obj",
	"objects/torus.obj",
	"objects/helix.obj"
};

#endif
