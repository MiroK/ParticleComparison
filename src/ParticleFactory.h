#ifndef _PARTICLE_FACTORY_H
#define _PARTICLE_FACTORY_H

#include <map>
#include <string>
#include <boost/tuple/tuple.hpp>

class ParticleFactory
{

/*
    Compute static data for particles.
*/

public:
    // constructor, initialize with geometrical dimension 
    ParticleFactory(std::size_t g_dim) : gdim(g_dim), size(1) { }
 
    // update look up table with entries for variable with var_name whose
    // rank is var_rank
    ParticleFactory& operator()(const std::string& var_name,
                                std::size_t var_rank );

    // convert 
    operator std::map<std::string, boost::tuple<std::size_t,
                                                std::size_t,
                                                std::size_t> >()
    { return lookup_table; }
    
public:
    // geom. dimension 
    std::size_t gdim;

    // size of data that particle will carry
    std::size_t size;

    // name of variable that specifies position
    std::string position_variable;

    // lookup table for variables and their data
    // var_name : current state index, previous state index, count
    std::map<std::string, boost::tuple<std::size_t,
                                       std::size_t,
                                       std::size_t> > lookup_table;

};

#endif // _PARTICLE_FACTORY_H
