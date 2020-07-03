#include "sine.h"

const int16_t sine512[] = {
#include "sine_data/sine512.h"
};

const int16_t sine448[] = {
#include "sine_data/sine448.h"
};

const int16_t sine384[] = {
#include "sine_data/sine384.h"
};
const int16_t sine320[] = {
#include "sine_data/sine320.h"
};
const int16_t sine256[] = {
#include "sine_data/sine256.h"
};

const Sine SINES[] = {
	{
		.data = sine512,
		.len = 512,
	},
	{
		.data = sine448,
		.len = 448,
	},
	{
		.data = sine384,
		.len = 384,
	},
	{
		.data = sine320,
		.len = 320,
	},
	{
		.data = sine256,
		.len = 256,
	},
};

const size_t N_SINES = sizeof(SINES)/sizeof(SINES[0]);
