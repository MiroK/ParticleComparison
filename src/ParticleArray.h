#include <map>
#include <string>
#include <boost/tuple/tuple.hpp>
#include <cstring> // memcpy
#include "mpi.h"

class ParticleArray
{

/*
    This class has all the data in double* _data and there is a lightweight
    map which serves as lookup table for where particle's properties are stored
    within _data. This example creates a particle with position, normal and
    gradient in 3D.
*/

public:
    // constructor, _size is 1 + 2*3 + 2*3 + 2*9 for id, x, x0, n, n0, dn, dn0
    ParticleArray() : _gdim(3), _size(31)
    {
        _data = new double[_size]();

        _lookup_table["x"].get<0>() = 1;
        _lookup_table["x"].get<1>() = 4;
      
        _lookup_table["n"].get<0>() = 7;
        _lookup_table["n"].get<1>() = 10;
       
        _lookup_table["dn"].get<0>() = 13;
        _lookup_table["dn"].get<1>() = 22;
    }

    // copy constructor
    ParticleArray(const ParticleArray& other)
    {
        _gdim = other._gdim;
        _size = other._size;
        _data = new double[_size];

        memcpy(_data, other._data, _size*sizeof(double));

        _lookup_table = other._lookup_table;
    }

    // destructor
    ~ParticleArray() { delete[] _data; _lookup_table.clear(); }

    // data access
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
    // data
    double* _data;

    // geometrical dimension
    std::size_t _gdim;

    // size of data
    std::size_t _size;

    // lookup table, pointer to current state, previous state and count
    std::map<std::string, boost::tuple<std::size_t, std::size_t> >
    _lookup_table;

};
