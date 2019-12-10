
EIGEN_CFLAGS := -I /usr/include/eigen3
JSONCPP_CFLAGS := -I /usr/include/jsoncpp

CC := gcc
CXX := g++

INCLUDES = -I include -I include/Functions -I include/Image -I include/Geometry -I external/easylogging
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
LDFLAGS += -fopenmp
ifdef USE_INTEL_MKL
INCLUDES += -I /opt/intel/compilers_and_libraries_2019/linux/mkl/include/fftw
LIBS +=
else
LIBS += -l fftw3 -l fftw3_threads
endif
ifdef USE_INTEL_MPI
INCLUDES += -I /opt/intel/compilers_and_libraries_2019/linux/mpi/intel64/include
LDFLAGS += -L /opt/intel/compilers_and_libraries_2019/linux/mpi/intel64/lib/release -L /opt/intel/compilers_and_libraries_2019/linux/mpi/intel64/libfabric/lib
LIBS += -l fabric
endif
LIBS += -l gsl -l gslcblas -l jsoncpp -l pthread -l mpi

MAIN_OBJECTS := \

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

default:	bin $(addprefix bin/,$(TARGETS))

.SECONDARY:	$(addsuffix .o,$(basename $(wildcard appsrc/*.cpp))) $(OBJECTS)

bin/%:	appsrc/%.o $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LIBS)

bin:
	mkdir bin

clean:
	rm -f appsrc/*.o $(OBJECTS)
	rm -rf bin
