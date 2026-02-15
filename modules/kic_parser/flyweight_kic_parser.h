#pragma once
#define KIC_UNAVAILABLE ((unsigned char)0)
#define KIC_AVAILABLE ((unsigned char)1)
// check availability of kic with version and syntax
unsigned char check_availability_of_kic(const char *const string);
