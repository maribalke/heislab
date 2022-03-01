#pragma once

void undefined_floor();

void set_initial_condition(); //definerer start tilstand
void set_current_floor(int* current_floor); 

void add_order(); 
void cab_order(int floor, int current_floor); //heispanel
void floor_order(int floor, int current_floor);
void delete_order(int f,int b); 

void queue_orders(int floor, int current_floor);
void go(int current_floor);

