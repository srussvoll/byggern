#pragma once

void init_memory() __attribute__((naked)) __attribute__((section(".init3")));
void init_hardware_drivers() __attribute__((naked)) __attribute__((section(".init8")));