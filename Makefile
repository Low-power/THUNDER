
#EIGEN_CFLAGS := -I /usr/include/eigen3
EIGEN_CFLAGS := -I external/Eigen3
#JSONCPP_CFLAGS := -I /usr/include/jsoncpp
JSONCPP_CFLAGS := -I external/jsoncpp
#JSONCPP_LDFLAGS := -L external/jsoncpp

ifeq ($(CC),cc)
CC := gcc
endif
ifeq ($(CXX),c++)
CXX := g++
endif

INCLUDES += -I include -I include/Functions -I include/Image -I include/Geometry -I external/easylogging
CFLAGS_WARNING := -Wall -Wno-uninitialized -Wno-deprecated-declarations -Wno-sign-compare
CFLAGS_OPTIMIZING := -O2
ifdef USE_AVX512
DEFINES += -D ENABLE_SIMD_512=1 -D ENABLE_SIMD_256=1
CFLAGS_MACHINE := -mavx512f -mavx512cd
# May be need to add -xCORE-AVX512 for icc and icpc?
else
DEFINES += -D ENABLE_SIMD_256=1
CFLAGS_MACHINE := -mavx
endif
CFLAGS += $(CFLAGS_WARNING) $(CFLAGS_MACHINE) -fopenmp $(CFLAGS_OPTIMIZING) $(DEFINES) $(INCLUDES) $(EIGEN_CFLAGS) $(JSONCPP_CFLAGS)
CXXFLAGS += $(CFLAGS_WARNING) $(CFLAGS_MACHINE) -fopenmp $(CFLAGS_OPTIMIZING) $(DEFINES) $(INCLUDES) $(EIGEN_CFLAGS) $(JSONCPP_CFLAGS)
LDFLAGS += -fopenmp -L lib
ifdef USE_INTEL_MKL
##DEFINES += -D MKL_ILP64=1
INCLUDES += -I /opt/intel/compilers_and_libraries_2019/linux/mkl/include/fftw
LDFLAGS += -L /opt/intel/compilers_and_libraries_2019/linux/mkl/lib/intel64
##LIBS += -l mkl_intel_ilp64 -l mkl_gnu_thread -l mkl_core
#LIBS += -l mkl_intel_lp64 -l mkl_gnu_thread -l mkl_core
LIBS += -l mkl_rt
else
LIBS += -l fftw3 -l fftw3_threads
endif
ifdef USE_INTEL_MPI
INCLUDES += -I /opt/intel/compilers_and_libraries_2019/linux/mpi/intel64/include
LDFLAGS += -L /opt/intel/compilers_and_libraries_2019/linux/mpi/intel64/lib/release -L /opt/intel/compilers_and_libraries_2019/linux/mpi/intel64/libfabric/lib
LIBS += -l fabric
endif
LIBS += -l gsl -l gslcblas -l jsoncpp -l pthread -l mpi

OBJECTS := \
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
	src/Image/BMP.o

#TARGETS := \
	thunder \
	thunder_alignZ \
	thunder_average \
	thunder_bfactor \
	thunder_genmask \
	thunder_genmask_shell \
	thunder_lowpass \
	thunder_mask \
	thunder_minus \
	thunder_postprocess \
	thunder_project \
	thunder_reconstruct \
	thunder_resize \
	thunder_view

TARGETS = $(basename $(notdir $(wildcard appsrc/*.cpp)))

default:	bin lib/libjsoncpp.a $(addprefix bin/,$(TARGETS))

.SECONDARY:	$(addsuffix .o,$(basename $(wildcard appsrc/*.cpp))) $(OBJECTS)

bin/%:	appsrc/%.o $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

bin lib:
	mkdir $@

lib/libjsoncpp.a:	lib external/jsoncpp/jsoncpp.o
	$(AR) -r $@ external/jsoncpp/jsoncpp.o

clean:
	rm -f appsrc/*.o $(OBJECTS)
	rm -rf bin
