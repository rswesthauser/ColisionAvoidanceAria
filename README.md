# MobileSim install:

sudo apt install ./mobilesim_0.9.8+ubuntu16_amd64.deb 



# C++:
sudo apt update && sudo apt install g++
C/C++ vscode extension (Microsoft)


# ARIA
sudo dpkg -i libaria_2.9.4+ubuntu16_amd64.deb 

# Build
make
chmod +x ./build/main

# Run
./build/main
  
# Detele the build
make clean
