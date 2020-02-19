CANNY_TEST_DIR = CannyTest/
CANNY_TEST_SRC = $(CANNY_TEST_DIR)cannyTest.cpp
CANNY_TEST_EXEC = cannyTest

GROOVE_DIR = GrooveDetection/
GROOVE_SRC = $(GROOVE_DIR)grooveDetection.cpp
GROOVE_EXEC = grooveDetection

default: 

cannyTest: $(CANNY_TEST_SRC)
	cd $(CANNY_TEST_DIR) && make && cd ..

runCannyTest: cannyTest 
	./$(CANNY_TEST_DIR)$(CANNY_TEST_EXEC) $(SRC)

cleanCannyTest: 
	cd $(CANNY_TEST_DIR) && rm -rf CMakeFiles && rm -f cmake_install.cmake CMakeCache.txt $(CANNY_TEST_EXEC)


grooveDetection: $(GROOVE_SRC)
	cd $(GROOVE_DIR) && make && cd ..

runGrooveDetection: grooveDetection
	./$(GROOVE_DIR)$(GROOVE_EXEC) $(SRC)

cleanGrooveDetection: 
	cd $(GROOVE_DIR) && rm -rf CMakeFiles && rm -f cmake_install.cmake CMakeCache.txt $(GROOVE_EXEC)

cleanAll: cleanCannyTest cleanGrooveDetection

