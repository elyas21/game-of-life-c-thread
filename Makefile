# include ./arch
#
# OBJ	= life.o real_rand.o plot.o timer.o
#
# LDLIBS = -lpthread 
#
# CFLAGS = -g 
#
# life:	        $(OBJ)
# 		$(CC) $(LDFLAGS) -o $@ $(OBJ)  $(LDLIBS)
#
#
#
# clean:	
# 	$(RM) *.o life *~;
# 	$(RM) core;
# 	$(RM) PI*;
include ./arch

# Common object files used by all programs
COMMON_OBJ = real_rand.o plot.o timer.o

# Object files specific to each version of the program
LIFE_OBJ = life.o
OPENMP_OBJ = openmp_life.o
PTHREAD_OBJ = pthread_life.o

LDLIBS = -lpthread
CFLAGS = -g -fopenmp  # -fopenmp for OpenMP compilation

# Build all executables
all: life openmp_life pthread_life

# Compile the original serial version
life: $(LIFE_OBJ) $(COMMON_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(LIFE_OBJ) $(COMMON_OBJ) $(LDLIBS)

# Compile the OpenMP version
openmp_life: $(OPENMP_OBJ) $(COMMON_OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OPENMP_OBJ) $(COMMON_OBJ) $(LDLIBS)

# Compile the Pthreads version
pthread_life: $(PTHREAD_OBJ) $(COMMON_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(PTHREAD_OBJ) $(COMMON_OBJ) $(LDLIBS)

# Clean the build files
clean:
	$(RM) *.o life openmp_life pthread_life *~;
	$(RM) core;
	$(RM) PI*;
