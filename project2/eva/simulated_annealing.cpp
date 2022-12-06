#include "simulated_annealing.h"

//swaps the position of two polygon points
int swap_polygon_points(Polygon_2 &polygon, int indexA, int indexB, bool last_point){
    if(indexA > polygon.size() - 1  || indexB > polygon.size() - 1){
        //std::cout << "(indexA: " << indexA << "  , indexB: " << indexB << ") " << std::endl;
        //std::cout << "The indices are out of bound. Please enter different indexA and/or indexB" << std::endl;
        return ERROR;
    }
   if(last_point){   //A is the last point of the polygon
        Point_2 first_vertex = polygon[indexB]; //indexB = 0
        polygon.insert(polygon.vertices_begin() + indexB, polygon[indexA]);
        polygon.insert(polygon.vertices_end() , first_vertex);
        polygon.erase(polygon.vertices_begin() + indexA +1);
        polygon.erase(polygon.vertices_begin() + 1);
        return 0;
    }
        polygon.insert(polygon.vertices_begin() + indexB + 1, polygon[indexA]);
        polygon.insert(polygon.vertices_begin() + indexA + 1, polygon[indexB]);
        polygon.erase(polygon.vertices_begin() + indexA);
        polygon.erase(polygon.vertices_begin() + indexB);
        return 0;
}

//local transition step: swaps the position of two adjacent polygon points
int local_transition_step(Polygon_2 &polygon, int indexA){
    if(indexA==polygon.size()-1){
        if(!swap_polygon_points(polygon, indexA, 0, true)){    //if A is the last value, swap it with polygon[0]
        //check validity of polygon
        //Construct a kd-Tree at initialization; all points inserted (CGAL class)
            return 0;
        }
        return ERROR;
    }
    if(!swap_polygon_points(polygon, indexA, indexA+1, false)){
        //check validity of polygon
        //Construct a kd-Tree at initialization; all points inserted (CGAL class)
        return 0;
    }
    else{
        return ERROR;
    }
}
//local transition step: swaps the position of two randomply selected polygon points
int global_transition_step(Polygon_2 &polygon){
    //Choose q and s randomly
    int index_q = 0;
    int index_s = 0;
    index_q = rand()%(polygon.size()-1);
    while(1){
        index_s = rand()%(polygon.size()-1);
        if(index_q!=index_s){
            break;
        }
    }
    //std::cout << "index_s " << index_s << " " << " index_q " << index_q << std::endl;

    //Insert vertex q after vertex s
    polygon.insert(polygon.vertices_begin() + index_s + 1, polygon[index_q]);
    //Erase point q from polygon(initial place)
    polygon.erase(polygon.vertices_begin() + index_q);
    return 0;
}

//NEEDS CHANGE
double Energy_of_State_P(Polygon_2 polygon, Polygon_2 convex_hull, bool is_Maximization){
    if(!is_Maximization){
        return (double)(polygon.size()*((std::abs(polygon.area()))/std::abs(convex_hull.area())));
    }
    else{
        return (double)(polygon.size()*(1 - (std::abs(polygon.area())/std::abs(convex_hull.area()))));
    }
}

double compute_deltaEnergy(double finalEnergy, double initialEnergy){
    return (double)finalEnergy - initialEnergy;
}

bool Metropolis_criterion(double deltaEnergy, double T){
    if(T==0.0){
        std::cout << "T is zero. Metropolis is not defined" << std::endl;
        return false;
    }
    double R = (double)rand()/(RAND_MAX + 1.);
    if(R>=1.0){
        std::cout << "ERROR_____________________R___________________________: " << R  << std::endl;
        exit(1);
    }

    //std::cout << "R: " << R  << std::endl;
    
    double exp = std::exp(-deltaEnergy/T);
    if(exp>R){
        //std::cout <<"_____________________exp___________________________: " << exp  << std::endl;
        return true;
    }
    return false;
}

int minimization_algorithm(Polygon_2 &polygon, bool isLocalStep, int L, bool is_Maximization){
    double T = 1.0;
    Polygon_2 save_polygon = polygon;
    Polygon_2 save_polygon_for_Metropolis = polygon; //if polygon is not accepted by metropolis criterion

    std::cout << " Initial area of polygon: " << std::abs(polygon.area()) << std::endl;
    Polygon_2 convex_hull;
    CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convex_hull));

    double initialEnergy = Energy_of_State_P(polygon, convex_hull, is_Maximization);
    double deltaEnergy   = 0.0;

    while(T>0){
        if(isLocalStep){
            int index = rand()%polygon.size();
            if(!local_transition_step(polygon, index) && polygon.is_simple()){  //check validity with kd-tree
                //std::cout << " Local transision step is successful " << index << std::endl;
            }
            else{   //Polygon is not valid: (goto 4)
                //std::cout << "Not valid " << index << std::endl;
                polygon = save_polygon;
                continue;
            }
        }
        else{   //global transition step -- //check validity with intersect
            if(!global_transition_step(polygon) && polygon.is_simple()){   
                //std::cout << " Global transision step is successful " << std::endl;
            }
            else{   //Polygon is not valid: (goto 4)
                polygon = save_polygon;
                continue;
            }
        }

        Polygon_2 convex_hull;
        CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convex_hull));

        deltaEnergy = compute_deltaEnergy(Energy_of_State_P(polygon, convex_hull, is_Maximization), initialEnergy);
        //std::cout << deltaEnergy << "**************delta energy" << std::endl;
        if(deltaEnergy < 0.0 || Metropolis_criterion(deltaEnergy,T)){ //the change is accepted
            Polygon_2 convex_hull;
            CGAL::convex_hull_2(polygon.begin(), polygon.end(), std::back_inserter(convex_hull));

            initialEnergy = Energy_of_State_P(polygon, convex_hull, is_Maximization);   //update the energy
            //std::cout << initialEnergy << "**************final energy" << std::endl;
            save_polygon = polygon; //update saved polygon

        }
        else{   //the change is not accepted, and we set the polygon to its previous state
            polygon = save_polygon;
        }
        T-=1/(double)L;
    }
    return 0;
}