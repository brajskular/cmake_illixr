sudo apt update
sudo apt-get install libglew-dev libglu1-mesa-dev libsqlite3-dev libx11-dev libgl-dev pkg-config libopencv-dev libeigen3-dev libboost-all-dev graphviz zlib1g-dev qtbase5-dev libhdf5-dev libcurl4-openssl-dev libqwt-qt5-dev libyaml-cpp-dev libfmt-dev libgflags-dev git libglfw3-dev libgoogle-glog-dev glslang-dev glslang-tools libjpeg-dev libusb-1.0.0-dev patch libpng-dev libprotobuf-dev libprotoc-dev protobuf-compiler libsdl2-dev libtiff-dev udev libudev-dev libvulkan-dev libwayland-dev wayland-protocols libx11-xcb-dev libxcb-glx0-dev libxcb-randr0-dev libxkbcommon-dev
# git clone https://github.com/ILLIXR/ILLIXR -b convertBuildToCmake
# cd ILLIXR
sudo apt-get install cmake
sudo apt install xvfb
mkdir build
cd build
# set install to previous directory
cmake .. -DCMAKE_INSTALL_PREFIX=/users/spakalap/ILLIXR -DCMAKE_BUILD_TYPE=RELEASE -DUSE_AUDIO_PIPELINE=1 -DUSE_GLDEMO=1 -DUSE_GROUND_TRUTH_SLAM=1 -DUSE_GTSAM_INTEGRATOR=1  -DUSE_OFFLINE_CAM=1 -DUSE_OFFLINE_IMU=1 -DUSE_POSE_LOOKUP=1 -DUSE_POSE_PREDICTION=1 -DUSE_TIMEWARP_GL=1 -DUSE_OPENVINS=1
make -j10
make install

Xvfb :99 -screen 0 1024x768x16 &
export DISPLAY=:99