##Compile & build Instructions##
```
.../optimizationCode$ mkdir build
.../optimizationCode$ cd build
.../optimizationCode/build$ cmake  -DOptiX_INSTALL_DIR=/home/cs/software/sivelab/NVIDIA-OptiX-SDK-3.6.2-linux64 -DCUDA_TOOLKIT_ROOT_DIR=/home/cs/software/sivelab/cuda_6.0 ..
.../optimizationCode/build$ make
```

Directory Structure
-------------------

optimizationCode

* docs  (documentation)

* fitness_lib (fitness function shared lib)

* include (Contains symbolic links to include directories all libraries in src)

* inputs (sample optfiles and hostsfile)

* src (source code)--|

   * MPI_Framework (Code which needs to be implemented by all jobs)

   * mpi_gpuplume  (GPU Plume code implementing MPI_framework)

   * opt_grammar   (ANTLR grammar for reading optimization files)

   * qes_spf_mpi   (TODO)

   * Utils (Utility code)

* thesisDoc(Thesis story)

* CMakeLists.txt (Top level cmake file)

* README.md (you are reading it right now!)

