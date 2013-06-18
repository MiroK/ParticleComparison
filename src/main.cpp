#include <iostream>
#include <cstdlib>
#include <ctime>
#include "mpi.h"
#include "ParticleArray.h"
#include "ParticleMap.h"
#include "ParticleStatic.h"
#include "ParticleFactory.h"


// ------------------------------
ParticleFactory factory(3);

template<>
const std::map<std::string, boost::tuple<std::size_t, std::size_t, std::size_t> >
ParticleStatic::_lookup_table = factory("x", 1)("n", 1)("dn", 2);

template<>
const std::size_t ParticleStatic::_gdim = factory.gdim;

template<>
const std::size_t ParticleStatic::_size = factory.size;

template<>
const std::string ParticleStatic::_position_variable =
factory.position_variable;
// -------------------------------

// memory usage for allocating N particles to be measure with valgrind
template<typename ParticleType>
void allocate(int N)
{ std::vector<ParticleType> particles(N); }

// time calls to operator[]
template<typename ParticleType>
void time_call(int N)
{
    std::vector<ParticleType> particles(N);
    clock_t cstart, cstop;
    cstart = clock();
    for(int j = 0; j < 100; j++)
        for(int i = 0; i < particles.size(); i++)
            particles.at(i)["n"][0] = 1;
       
    cstop = clock() ;
    std::cout << "Call to [] took " <<
             (double)(cstop - cstart)/CLOCKS_PER_SEC << std::endl;
}

// time correct send_receive
template<typename ParticleType>
void send_receive(int N)
{
    MPI_Init(NULL, NULL);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    if(world_size == 2)
    {
        // allocate N particles on both processes and give them unique id
        std::vector<ParticleType> particles(N);
        for(int i = 0; i < particles.size(); i++)
            particles.at(i).id() = i;

        // chage normal component of every particle on master to its id
        // change dn[3] to id+100;
        if(world_rank == 0)
        {
            for(int i = 0; i < particles.size(); i++)
            {
                particles.at(i)["n"][0] = particles.at(i).id();
                particles.at(i)["dn"][3] = particles.at(i).id()+100;
            }

            clock_t cstart, cstop;
            cstart = clock();
            // send the particles to slave
            for(int i = 0; i < particles.size(); i++)
            {
                particles.at(i).send(1);
            }
            cstop = clock();
            std::cout << "Sending took " << 
                    (double)(cstop - cstart)/CLOCKS_PER_SEC << std::endl;
        }
        else
        {
            clock_t cstart, cstop;
            cstart = clock();
            // receive from master and check good receive
            for(int i = 0; i < particles.size(); i++)
            {
                //std::cout << "Before " << particles.at(i)["n"][0] << std::endl;
                
                particles.at(i).recv(0);
                
                //std::cout << "After " << particles.at(i)["n"][0] << std::endl;

                
                if( (particles.at(i)["n"][0] != particles.at(i).id()) or
                    (particles.at(i)["dn"][3] != particles.at(i).id()+100) )
                    throw "Bad receive!";
            }
            cstop = clock();
            std::cout << "Receiving took " << 
                    (double)(cstop - cstart)/CLOCKS_PER_SEC << std::endl;
        }
    }
    MPI_Finalize();
}

int main(int argc, char** argv)
{
    if(argc == 4)
    {   
        int particle_type = std::atoi(argv[1]);
        int particle_count = std::atoi(argv[2]);
        int test_type = std::atoi(argv[3]);

        if(particle_type == 0) // ParticleArray
        {
            if(test_type == 0) // alloc
                allocate<ParticleArray>(particle_count);
            
            else if(test_type == 1) // call
                time_call<ParticleArray>(particle_count);
            
            else if(test_type == 2) // send_receive
                send_receive<ParticleArray>(particle_count);
        }
        else if(particle_type == 1) // ParticleMap
        {
            if(test_type == 0) // alloc
                allocate<ParticleMap>(particle_count);
            
            else if(test_type == 1) // call
                time_call<ParticleMap>(particle_count);
            
            else if(test_type == 2)
                send_receive<ParticleMap>(particle_count);
        }
        else if(particle_type == 2) //ParticleStatic
        {
            if(test_type == 0) // alloc
                allocate<ParticleStatic>(particle_count);
            
            else if(test_type == 1) // call
                time_call<ParticleStatic>(particle_count);
            
            else if(test_type == 2)
                send_receive<ParticleStatic>(particle_count);
        }
        return 0;
    }
    else{
        throw "Valid number of arguments is 2!";
    }
}
