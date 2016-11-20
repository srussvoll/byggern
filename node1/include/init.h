#pragma once
/**
 * \brief Initializes the external memory map.
 *
 * This runs at .init3 before the .data-section is written.
 */
void init_memory() __attribute__((naked)) __attribute((used)) __attribute__((section(".init3")));

/**
 * \brief Initializes all hardware drivers and the network stack.
 */
void init_hardware_drivers() __attribute__((naked)) __attribute((used)) __attribute__((section(".init8")));