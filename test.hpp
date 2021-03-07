#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <iterator>
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

#include "nbt.hpp"
#include "ios-bin.hpp"

int main();
void run_all_tests();

void sbin_test();
void sbin_manip_stress();
void sbin_primitive();
void sbin_float();
void sbin_vector();
void sbin_string();

void rtti_test();
void rtti_primitive();
void rtti_array();
void rtti_list();
void rtti_etc();

void parsing_nbt();
void parsing_primitive();
void parsing_array();
void parsing_list();
