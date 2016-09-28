#pragma once

#include <stdlib.h>

void *operator new(size_t size);
void operator delete(void *ptr);