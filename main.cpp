/**
CSCI 235 Project 7 
Created by Allison Lee on December 10, 2023
main.cpp tests the functions from Project 7
*/

#include "Inventory.hpp"

int main(){
    Item* a = new Item("TIRED GAUNTLETS", ARMOR, 3, 25, 1);
    Item* b = new Item("SMALL HEALTH POTION", CONSUMABLE, 1, 10, 2);
    Item* c = new Item("A", CONSUMABLE, 0, 0, 10);
    Item* d = new Item("D", CONSUMABLE, 0, 0, 10);
    Inventory backpack;
    backpack.addItem(a);
    backpack.addItem(b);
    // backpack.addItem(c);
    // backpack.addItem(d);
    backpack.printInventoryInOrder();
}