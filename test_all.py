import os
class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'

test_d = {0 : "Memory", 1 : "Call", 2 : "Send/Recv"}
particle_d = {0 : "ParticleArray", 1 : "ParticleMap", 2 : "ParticleStatic"}

for test_type in [1, 2]: #[0, 1, 2]:
    for particle_type in [0, 1, 2]:
        for particle_count in [10000, 100000, 1000000, 2000000]:
            
            print bcolors.FAIL + test_d[test_type] + bcolors.ENDC, \
                  bcolors.OKBLUE + particle_d[particle_type] + bcolors.ENDC,\
                  bcolors.OKGREEN + str(particle_count) + bcolors.ENDC
            
            if(test_type == 0):
                os.system("valgrind ./particles.x %d %d %d" % (particle_type,
                                                               particle_count,
                                                               test_type))
            elif(test_type == 2):
                os.system("mpirun -np 2 ./particles.x %d %d %d" % (particle_type,
                                                                   particle_count,
                                                                    test_type))
            else:
                os.system("./particles.x %d %d %d" % (particle_type,
                                                      particle_count,
                                                      test_type))


