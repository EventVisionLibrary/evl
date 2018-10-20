sudo apt-get update && apt-get install -y sudo && sudo -E apt-get update
sudo -E apt-get install -y -qq git cmake libusb-dev libusb-1.0-0-dev
sudo -E apt-get install -y -qq libboost-all-dev
sudo -E apt-get install libeigen3-dev
sudo -E apt-get install -y -qq libopencv-dev

sudo -E apt-get update && bash install_dependencies.sh
cd examples; mkdir build; cd build; cmake ..; make
