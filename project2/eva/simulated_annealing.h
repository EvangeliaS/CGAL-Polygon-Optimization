#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/intersections.h>
#include <CGAL/Polygon_2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#include "sharedLib.h"
#include "incrementalAlgorithm.h"
#include "convexHullAlgorithm.h"


int swap_polygon_points(Polygon_2 &polygon, int indexA, int indexB, bool islastpoint);

//local transition step: swaps the position of two adjacent polygon points
int local_transition_step(Polygon_2 &polygon, int indexA);

//local transition step: swaps the position of two randomply selected polygon points
int global_transition_step(Polygon_2 &polygon);

double Energy_of_State_P(Polygon_2 polygon, Polygon_2 convex_hull, bool is_Maximization);

double compute_deltaEnergy(double finalEnergy, double initialEnergy);

bool Metropolis_criterion(double deltaEnergy, double T);

int minimization_algorithm(Polygon_2 &polygon, bool isLocalStep, int L, bool is_Maximization);
