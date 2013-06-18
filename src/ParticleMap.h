#include <map>
#include <string>
#include <vector>
#include <boost/tuple/tuple.hpp>
#include <cstring> // memcpy

class ParticleMap 
{

/*
    This class has all the data apart from position organized in map. For an
    example a particle with position, normal and gradient in 3D is created.
*/

public:
    // constructor
    ParticleMap() : _id(0), _size(31), _gdim(3), _x(new double[6])
    {
        _data["n"].get<0>().resize(3);
        _data["n"].get<1>().resize(3);

        _data["dn"].get<0>().resize(9);
        _data["dn"].get<1>().resize(9);
    }

    // copy constructor         
    ParticleMap(const ParticleMap& other)
    {
        _id = other._id;
        _size = other._size;
        _gdim = other._gdim;
        
        _x = new double[6];
        memcpy(_x, other._x, 6*sizeof(double));
        
        _data = other._data;
    }
 
    // destructor
    ~ParticleMap() { delete[] _x; _data.clear(); }

    // data access
    double* operator[](const std::string& name)
    {return &(_data.find(name)->second).get<0>().front(); }
   
    //id 
    double& id() { return _id; }

    // MPI_Send wrapper
    void send(std::size_t dest, MPI_Comm comm = MPI_COMM_WORLD)
    { 
        // collapse particle's data to send_array
        double* send_array = new double[_size];

        send_array[0] = _id;
  
        memcpy(&send_array[1], _x, 6*sizeof(double));
        
        memcpy(&send_array[7],
               &_data["n"].get<0>().front(),
               3*sizeof(double));

        memcpy(&send_array[10],
               &_data["n"].get<1>().front(),
               3*sizeof(double));
        
        memcpy(&send_array[13],
               &_data["dn"].get<0>().front(),
               9*sizeof(double));
        
        memcpy(&send_array[22],
               &_data["dn"].get<1>().front(),
               9*sizeof(double));
        
        // send
        MPI_Send(send_array, _size, MPI_DOUBLE, dest, _id, comm);
        delete[] send_array;
    }
    
    // MPI_Recv wrapper
    int recv(std::size_t source, MPI_Comm comm = MPI_COMM_WORLD)
    {
        double* recv_array = new double[_size];
        
        MPI_Status status;
        MPI_Recv(recv_array, _size, MPI_DOUBLE, source, MPI_ANY_TAG, comm,
                 &status);

        // extract data from recv_array
        _id = recv_array[0];
  
        memcpy(_x, &recv_array[1], 6*sizeof(double));
        
        memcpy(&_data["n"].get<0>().front(),
               &recv_array[7],
               3*sizeof(double));

        memcpy(&_data["n"].get<1>().front(),
               &recv_array[10],
               3*sizeof(double));

        memcpy(&_data["dn"].get<0>().front(),
               &recv_array[13],
               9*sizeof(double));

        memcpy(&_data["dn"].get<1>().front(),
               &recv_array[22],
               9*sizeof(double));
        
        delete[] recv_array;
        return 0;
    }

private:
    // additional data
    std::map<std::string, 
             boost::tuple<std::vector<double>, std::vector<double> > > _data;
    
    // particle's ID
    double _id; 

    // size of data for MPI_*
    std::size_t _size;

    // geometrical dimension
    std::size_t _gdim;
    
    // position
    double* _x;

};

