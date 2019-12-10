
EIGEN_CFLAGS := -I /usr/include/eigen3
JSONCPP_CFLAGS := -I /usr/include/jsoncpp

CC := gcc
CXX := g++

INCLUDES = -I include -I include/Functions -I include/Image -I include/Geometry -I external/easylogging
INCLUDES += -I /opt/intel/compilers_and_libraries_2019/linux/mpi/intel64/include
CFLAGS_WARNING := -Wall -Wno-uninitialized -Wno-deprecated-declarations -Wno-sign-compare
ifdef USE_AVX512
DEFINES += -D ENABLE_SIMD_512=1 -D ENABLE_SIMD_256=1
CFLAGS_MACHINE := -mavx512f -mavx512cd
else
DEFINES += -D ENABLE_SIMD_256=1
CFLAGS_MACHINE := -mavx
endif
CFLAGS += $(CFLAGS_WARNING) $(CFLAGS_MACHINE) -fopenmp -O3 $(DEFINES) $(INCLUDES) $(EIGEN_CFLAGS) $(JSONCPP_CFLAGS)
CXXFLAGS += $(CFLAGS_WARNING) $(CFLAGS_MACHINE) -fopenmp -O3 $(DEFINES) $(INCLUDES) $(EIGEN_CFLAGS) $(JSONCPP_CFLAGS)
LDFLAGS += -fopenmp -L /opt/intel/compilers_and_libraries_2019/linux/mpi/intel64/lib/release
LIBS += -l fftw3 -l fftw3_threads -l gsl -l gslcblas -l jsoncpp -l pthread -l mpi

OTHER_OBJECTS := \
	appsrc/thunder_alignZ.o \
	appsrc/thunder_average.o \
	appsrc/thunder_bfactor.o \
	appsrc/thunder_genmask.o \
	appsrc/thunder_genmask_shell.o \
	appsrc/thunder_lowpass.o \
	appsrc/thunder_mask.o \
	appsrc/thunder_minus.o \
	appsrc/thunder_postprocess.o \
	appsrc/thunder_project.o \
	appsrc/thunder_reconstruct.o \
	appsrc/thunder_resize.o \
	appsrc/thunder_view.o
OBJECTS := \
	appsrc/thunder.o \
	src/Coordinate5D.o \
	src/CTF.o \
	src/Database.o \
	src/FFT.o \
	src/LinearAlgebra.o \
	src/Logging.o \
	src/Model.o \
	src/Optimiser.o \
	src/Parallel.o \
	src/Particle.o \
	src/Postprocess.o \
	src/Precision.o \
	src/Projector.o \
	src/Reconstructor.o \
	src/TabFunction.o \
	src/Utils.o \
	src/Functions/Random.o \
	src/Functions/Functions.o \
	src/Functions/Filter.o \
	src/Functions/Spectrum.o \
	src/Functions/Mask.o \
	src/Geometry/DirectionalStat.o \
	src/Geometry/SymmetryOperation.o \
	src/Geometry/Symmetry.o \
	src/Geometry/Euler.o \
	src/Geometry/SymmetryFunctions.o \
	src/Image/ImageBase.o \
	src/Image/ImageFile.o \
	src/Image/ImageFunctions.o \
	src/Image/Image.o \
	src/Image/Volume.o \
	src/Image/BMP.o \

thunder:	$(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

clean:
	rm -f $(OBJECTS) thunder
