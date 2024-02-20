***************
Build for Kafka
***************

git clone https://github.com/catch/Catch2
cd Catch2
mkdir build
cd build
cmake ../.
make 
sudo make install
sudo dnf install spdlog spdlog-devel
sudo dnf install libyaml-devel yaml-cpp yaml-cpp-devel
sudo dnf install ts_sal_utils MTM1M3 MTMount MTRotator
export PKG_CONFIG_PATH=/usr/local/share/pkgconfig



