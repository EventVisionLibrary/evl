# libcaer
git clone https://github.com/inilabs/libcaer.git
cd libcaer
git reset --hard b77bc2b54f45efd8f8b6ef05074bf06491c33e3b
mkdir build
cd build
cmake -DCMAKE_INSTALL_PREFIX=/usr ..
make
make install
