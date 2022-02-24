#pragma once

int queue[4][3];

void set_initial_condition(); //definerer start tilstand
void set_current_floor(int* current_floor); 

void add_order(); 
void cab_order(int floor, int current_floor); //heispanel
void floor_order();
void delete_order(int f,int b); 

void go(int current_floor);

