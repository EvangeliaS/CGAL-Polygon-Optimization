#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Search_traits_2.h>
#include <CGAL/Iso_rectangle_2.h>
#include <CGAL/Fuzzy_iso_box.h>
#include <CGAL/convex_hull_2.h>
#include <CGAL/intersections.h>
#include <CGAL/Polygon_2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <CGAL/Kd_tree.h>

#include "sharedLib.h"
#include "incrementalAlgorithm.h"
#include "convexHullAlgorithm.h"

bool checkSegmentsIntersect(Polygon_2::Segment_2 segment, Polygon_2::Segment_2 pr);

int swap_polygon_points(Polygon_2 &polygon, int indexA, int indexB, bool islastpoint);

//local transition step: swaps the position of two adjacent polygon points
int local_transition_step(Polygon_2 &polygon, int indexA);

//local transition step: swaps the position of two randomply selected polygon points
bool globalStep(Polygon_2 &polygon);

double Energy_of_State_P(Polygon_2 polygon, Polygon_2 convex_hull, bool is_Maximization);

double compute_deltaEnergy(double finalEnergy, double initialEnergy);

bool Metropolis_criterion(double deltaEnergy, double T);

int minimization_algorithm(Polygon_2 &polygon, bool isLocalStep, int L, bool is_Maximization, int &constructionTime, int remainingTime);

void spatialSubdivision(Polygon_2 &pointset, int& constructionTime);

void splitIntoSubsets(Polygon_2& pointset, std::vector<Polygon_2>& subsets);

bool validityCheckLocal(Polygon_2 &A, Point_2 &p, Point_2 &q, Point_2 &r, Point_2 &s, CGAL::Kd_tree<CGAL::Search_traits_2<Kernel>> &kdTree);

bool validityCheckGlobal(Polygon_2 &A, Point_2& p, Point_2& q, Point_2& r, Point_2& s, Point_2& t);

bool localStep(Polygon_2 &A, CGAL::Kd_tree<CGAL::Search_traits_2<Kernel>> &kdTree);



