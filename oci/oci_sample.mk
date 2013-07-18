#
# Example for building demo OCI programs:
#
# 1. All OCI demos:
#
#    make -f oci_sample.mk demos
#
# 2. A single OCI demo:
#
#    make -f oci_sample.mk build EXE=demo 
#    e.g. make -f oci_sample.mk build EXE=readgeom 
#
# 3. A single OCI demo with static libraries:
#
#    make -f oci_sample.mk build_static EXE=demo 
#    e.g. make -f oci_sample.mk build_static EXE=readgeom 
#
# 4. To re-generate shared library:
#
#    make -f oci_sample.mk generate_sharedlib
#
#
# NOTE: 1. ORACLE_HOME must be either:
#		   . set in the user's environment
#		   . passed in on the command line
#		   . defined in a modified version of this makefile
#
#       2. If the target platform support shared libraries (e.g. Solaris)
#          look in the platform specific documentation for information
#          about environment variables that need to be properly
#          defined (e.g. LD_LIBRARY_PATH in Solaris).
#


include $(ORACLE_HOME)/rdbms/lib/env_rdbms.mk


RDBMSLIB=$(ORACLE_HOME)/rdbms/lib/

LDFLAGS=-L$(LIBHOME) -L$(ORACLE_HOME)/rdbms/lib
LLIBPSO=`cat $(ORACLE_HOME)/rdbms/lib/psoliblist`

# directory that contain oratypes.h and other oci demo program header files
INCLUDE= -I$(ORACLE_HOME)/rdbms/demo -I$(ORACLE_HOME)/rdbms/public -
I$(ORACLE_HOME)/plsql/public -I$(ORACLE_HOME)/network/public -I.

#
CONFIG = $(ORACLE_HOME)/rdbms/lib/config.o

# module to be used for linking with non-deferred option

# flag for linking with non-deferred option (default is deferred mode)
NONDEFER=false

# libraries for linking oci programs
OCISHAREDLIBS=$(TTLIBS) $(DEF_ON) $(LLIBTHREAD)
OCISTATICLIBS=$(STATICTTLIBS) $(LLIBTHREAD)

PSOLIBLIST=$(ORACLE_HOME)/rdbms/lib/psoliblist
CLEANPSO=rm -f $(PSOLIBLIST); $(GENPSOLIB)

DOLIB=$(ORACLE_HOME)/lib/liborcaccel.a
DUMSDOLIB=$(ORACLE_HOME)/lib/liborcaccel_stub.a
REALSDOLIB=/usr/lpp/orcaccel/liborcaccel.a


LLIBSQL= -lsql
PROLDLIBS= $(LLIBSQL) $(TTLIBS)

DEMO_MAKEFILE = oci_sample.mk
 
DEMOS = readgeom writegeom 

OBJECTS=geomops.o readgeom.o writegeom.o 

OBJ1=geomops.o readgeom.o
OBJ2=writegeom.o

demos: $(DEMOS)

generate_sharedlib:
	$(SILENT)$(ECHO) "Building client shared library ..."
	$(SILENT)$(ECHO) "Calling script $$ORACLE_HOME/bin/genclntsh ..."
	$(GENCLNTSH)
	$(SILENT)$(ECHO) "The library is $$ORACLE_HOME/lib/libclntsh.so... DONE"

$(DEMOS):
	$(MAKE) -f $(DEMO_MAKEFILE) build EXE=$@ 

readgeom: $(OBJ1)

writegeom:$(OBJ2)

$(OBJ1): readgeom.c geomops.c readgeom.h geomops.h

$(OBJ2): writegeom.c writegeom.h

build: $(LIBCLNTSH) $(OBJECTS)
	$(ECHODO) $(CC) $(LDFLAGS) -o readgeom $(OBJ1) $(OCISHAREDLIBS)
	$(ECHODO) $(CC) $(LDFLAGS) -o writegeom $(OBJ2) $(OCISHAREDLIBS)

build_static: $(OBJECTS)
	$(ECHODO) $(CC) $(LDFLAGS) -o readgeom $(OBJ1) $(SSDBED) $(DEF_ON) \
	$(OCISTATICLIBS)
	$(ECHODO) $(CC) $(LDFLAGS) -o writegeom $(OBJ2) $(SSDBED) $(DEF_ON) \
	$(OCISTATICLIBS)
clean:
	$(RM) -f $(DEMOS) *.o *.so


