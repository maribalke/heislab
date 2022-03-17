/**
 * @file timer.h
 * @author Hermine Alfsen, Mari Balke Fjellang
 * @brief functions to control the timer
 * @version 0.1
 * @date 2022-03-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#pragma once


/**
 * @brief Starts the timer
 * 
 */
void timer_start();

/**
 * @brief Checks if the timer is expired
 * 
 * @return 1 if success, 0 if else
 */
int timer_expired();