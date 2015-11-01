EXEC = Raspicop
EXEC3D = 3Dtest
EXECVISU = visu

#SRC_MPU= Libs/PiNetDMP/I2Cdev.cpp Libs/PiNetDMP/MPU6050.cpp

#source : rech de .cpp
#SRC= Libs/Timer/Timer.cpp $(SRC_MPU) $(wildcard *.cpp)
ALLCPP=$(wildcard *.cpp)
EXCLUDE=main.cpp demo_3d.cpp main_3d.cpp visu.cpp

SRC= $(filter-out $(EXCLUDE),$(ALLCPP))
#construction des objets
//OBJ=$(SRC:.cpp=.o)
OBJ=$(patsubst %.cpp,obj/%.o,$(SRC)) 

#-----------------------------------------------------------
#compilation :
CC = g++
#LIBS= -lm -lrt  -lpthread -lboost_system #-llapack 
LIBS= -l bcm2835 -lrt  -std=c++0x 


#option de debugage :
OPT_Debug= -O2 -Wall -g 

# Set DMP FIFO rate to 20Hz to avoid overflows on 3d demo.  See comments in
# MPU6050_6Axis_MotionApps20.h for details.
# F = 200/(DDMP_FIFO_RATE + 1)
#the program is not fast enough to read DMP at 200Hz => DDMP_FIFO_RATE = 1

#CXXFLAGS = -DDMP_FIFO_RATE=2
#CXXFLAGS = -DDMP_FIFO_RATE=4
CXXFLAGS3D=`pkg-config gtkmm-3.0 --cflags --libs`
CXXFLAGS=


ALL:$(EXEC)


$(EXEC):$(OBJ) obj/main.o
	$(CC) $(OPT_Debug) -o $@ $^ $(LIBS) $(CXXFLAGS3D)

DEMO_3D:$(EXEC3D)

$(EXEC3D):$(OBJ) obj/demo_3d.o obj/main_3d.o
	$(CC) $(CXXFLAGS3D) $(OPT_Debug) -o $@ $^ $(LIBS)

VISU:$(EXECVISU)

$(EXECVISU):$(OBJ) obj/visu.o
	$(CC) $(CXXFLAGS3D) $(OPT_Debug) -o $@ $^ $(LIBS)

	
#obj/demo_3d.o:demo_3d.cpp
#	$(CC) $(CXXFLAGS3D) $(OPT_Debug) -o $@ $^ $(LIBS)
	
#obj/main_3d.o:main_3d.cpp
#	$(CC) $(CXXFLAGS3D) $(OPT_Debug) -o $@ $^ $(LIBS)

obj/%.o: %.cpp
	$(CC) $(OPT_Debug) -o $@ -c $^ $(LIBS) $(CXXFLAGS3D)

clean:
	rm -rf obj/*.o 
	#rm *~ *.mod
	rm -rf $(EXEC)
