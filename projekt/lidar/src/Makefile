all: lidar


VL53L0X_rasp/bin/libVL53L0X_Rasp.a:
	API_DIR=../VL53L0X $(MAKE) -C VL53L0X_rasp

lidar : lidar.cpp VL53L0X_rasp/bin/libVL53L0X_Rasp.a
	$(CXX) -I VL53L0X/Api/core/inc -I VL53L0X_rasp/platform/inc $(CXXFLAGS) -o lidar lidar.cpp -L VL53L0X_rasp/bin -l VL53L0X_Rasp

clean:
	rm -f *.o lidar
