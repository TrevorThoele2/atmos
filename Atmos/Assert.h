#pragma once

#ifdef ATMOS_DEBUG
#define ATMOS_ASSERT(x) if(!(x)) abort();
#define ATMOS_ASSERT_MESSAGE(x, message) if(!(x)) abort();
#else
#define ATMOS_ASSERT(x) (void)0;
#define ATMOS_ASSERT_MESSAGE(x, message) (void)0;
#endif