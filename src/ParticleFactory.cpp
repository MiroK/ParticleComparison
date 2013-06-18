#include "ParticleFactory.h"
#include <cmath>

ParticleFactory& ParticleFactory::operator()(const std::string& var_name,
                                             std::size_t var_rank)
{
    // allow only one insert per variable_name
    if(lookup_table.find(var_name) == lookup_table.end())
        // first variable is position 
        if(position_variable.empty())
            position_variable = var_name;
    
        std::size_t count = pow(gdim, var_rank);
    
        lookup_table[var_name].get<0>() = size ;
        size += count;

        lookup_table[var_name].get<1>() = size ;
        size += count;

        lookup_table[var_name].get<2>() = count;
    
    return *this;
}
