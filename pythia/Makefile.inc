# PYTHIA configuration file.
# Generated on ven 24 mar 2023, 12:53:31, CET with the user supplied options:
# --with-hepmc2
# --with-hepmc3
# --with-root-bin=/usr/local/root/bin
# --with-root-lib=/usr/local/root/lib
# --with-root-include=/usr/local/root/include
# --with-gzip
# --with-python-bin=/usr/bin/python3.10
# --with-python-lib=/usr/local/lib/python3.10
# --with-python-include=/usr/include/python3.10
# --with-openmp
# --with-hepmc2-lib=/usr/local/HepMC-2/lib
# --with-hepmc2-include=/usr/local/HepMC-2/include

# Install directory prefixes.
PREFIX_BIN=/usr/local/pythia8309/bin
PREFIX_INCLUDE=/usr/local/pythia8309/include
PREFIX_LIB=/usr/local/pythia8309/lib
PREFIX_SHARE=/usr/local/pythia8309/share/Pythia8

# Compilation flags (see ./configure --help for further documentation).
CXX=g++
CXX_COMMON=-O2 -std=c++11 -pedantic -W -Wall -Wshadow -fPIC -pthread -DGZIP -fopenmp -DOPENMP
CXX_SHARED=-shared
CXX_SONAME=-Wl,-soname,
LIB_SUFFIX=.so
OBJ_COMMON=

EVTGEN_USE=false
EVTGEN_CONFIG=
EVTGEN_BIN=
EVTGEN_INCLUDE=
EVTGEN_LIB=

FASTJET3_USE=false
FASTJET3_CONFIG=
FASTJET3_BIN=
FASTJET3_INCLUDE=
FASTJET3_LIB=

HEPMC2_USE=true
HEPMC2_CONFIG=
HEPMC2_BIN=
HEPMC2_INCLUDE=-I/usr/local/HepMC-2/include
HEPMC2_LIB=-L/usr/local/HepMC-2/lib -Wl,-rpath,/usr/local/HepMC-2/lib -lHepMC

HEPMC3_USE=true
HEPMC3_CONFIG=HepMC3-config
HEPMC3_BIN=
HEPMC3_INCLUDE=
HEPMC3_LIB=-lHepMC3

LHAPDF5_USE=false
LHAPDF5_CONFIG=
LHAPDF5_BIN=
LHAPDF5_INCLUDE=
LHAPDF5_LIB=

LHAPDF6_USE=false
LHAPDF6_CONFIG=
LHAPDF6_BIN=
LHAPDF6_INCLUDE=
LHAPDF6_LIB=

POWHEG_USE=false
POWHEG_CONFIG=
POWHEG_BIN=
POWHEG_INCLUDE=
POWHEG_LIB=

RIVET_USE=false
RIVET_CONFIG=
RIVET_BIN=
RIVET_INCLUDE=
RIVET_LIB=

ROOT_USE=true
ROOT_CONFIG=root-config
ROOT_BIN=/usr/local/root/bin/
ROOT_INCLUDE=-I/usr/local/root/include
ROOT_LIB=-L/usr/local/root/lib -Wl,-rpath,/usr/local/root/lib -lCore

GZIP_USE=true
GZIP_CONFIG=
GZIP_BIN=
GZIP_INCLUDE=
GZIP_LIB=-lz

PYTHON_USE=true
PYTHON_CONFIG=python-config
PYTHON_BIN=/usr/bin/python3.10/
PYTHON_INCLUDE=-I/usr/include/python3.10
PYTHON_LIB=-L/usr/local/lib/python3.10 -Wl,-rpath,/usr/local/lib/python3.10

MG5MES_USE=false
MG5MES_CONFIG=
MG5MES_BIN=
MG5MES_INCLUDE=
MG5MES_LIB=

OPENMP_USE=true
OPENMP_CONFIG=
OPENMP_BIN=
OPENMP_INCLUDE=
OPENMP_LIB=-lgomp

MPICH_USE=false
MPICH_CONFIG=
MPICH_BIN=
MPICH_INCLUDE=
MPICH_LIB=

HDF5_USE=false
HDF5_CONFIG=
HDF5_BIN=
HDF5_INCLUDE=
HDF5_LIB=

HIGHFIVE_USE=false
HIGHFIVE_CONFIG=
HIGHFIVE_BIN=
HIGHFIVE_INCLUDE=
HIGHFIVE_LIB=
