#pragma once
#define KIC_CONPATIBLE ((unsigned char)0)
#define KIC_INCONPATIBLE ((unsigned char)1)
unsigned char check_kic_compatibility(const char *string);

#define KIC_SYNTAX_CORRECT ((unsigned char)0)
#define KIC_SYNTAX_ERROR ((unsigned char)1)
// this function will call check_kic_compatibility() with the argument
unsigned char check_kic_syntax(const char *string);
