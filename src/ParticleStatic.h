#include <map>
#include <string>
#include <cstring>                  // memcpy
#include <boost/tuple/tuple.hpp>
#include "mpi.h"

class ParticleStatic
{

/*
    In this implementation particle has data in double* _data, but all 
    lookup table and other common variables are made in class variables
    and are shared between all the instances.
*/

public:
    // constructor
    ParticleStatic() { _data = new double[_size](); }
   
    // copy constructor
    ParticleStatic(const ParticleStatic& other)
    {
        _data = new double[_size];
        memcpy(_data, other._data, _size*sizeof(double));
    }

    // destructor
    virtual ~ParticleStatic(){ delete[] _data; }

    // get pointers first element of variable's data in _data
    // current state, modify
    double* operator[](const std::string& var_name)
    { return &_data[_lookup_table.find(var_name)->second.get<0>()]; }

    // id
    double& id() { return _data[0]; }
    
    // MPI_Send wrapper
    void send(std::size_t dest, MPI_Comm comm = MPI_COMM_WORLD)
    { MPI_Send(_data, _size, MPI_DOUBLE, dest, _data[0], comm); }
    
    // MPI_Recv wrapper
    int recv(std::size_t source, MPI_Comm comm = MPI_COMM_WORLD)
    {
        MPI_Status status;
        return MPI_Recv(_data,
                        _size,
                        MPI_DOUBLE,
                        source,
                        MPI_ANY_TAG,
                        comm,
                        &status);
    }

private:
    // geometric. dimension of space where particles are located
    const static std::size_t _gdim;

    // size of _data array
    const static std::size_t _size;

    // positional variable used for dolfin::Point and str()
    const static std::string _position_variable;

    // lookup table of starting indices and counts of variables in _data 
    const static std::map<std::string,
                          boost::tuple<std::size_t,
                                       std::size_t,
                                       std::size_t> > _lookup_table;
    
    // all particle's data
    double* _data;

};
