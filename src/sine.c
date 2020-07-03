#include "sine.h"

const int16_t sine512[] = {
#include "sine512.h"
};

const int16_t sine256[] = {
#include "sine256.h"
};

const int16_t sine128[] = {
#include "sine128.h"
};
const int16_t sine64[] = {
#include "sine64.h"
};

const Sine SINES[] = {
	{
		.data = sine512,
		.len = 512,
	},
	{
		.data = sine256,
		.len = 256,
	},
	{
		.data = sine128,
		.len = 128,
	},
	{
		.data = sine64,
		.len = 64,
	},
};

const size_t N_SINES = sizeof(SINES)/sizeof(SINES[0]);
