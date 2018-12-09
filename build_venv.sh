rm -Rf build
mkdir build
cd build
install_path=$1
PYBOMBS_PREFIX=$install_path
mkdir -p $install_path
cmake -DCMAKE_INSTALL_PREFIX=$install_path ..
make
make install

cat << EOF > $install_path/activate
#!/bin/bash
export PYTHONPATH="$install_path/python:$install_path/hier_blocs:$install_path/lib/python2.6/site-packages:$install_path/lib64/python2.6/site-packages:$install_path/lib/python2.6/dist-packages:$install_path/lib64/python2.6/dist-packages:$install_path/lib/python2.7/site-packages:$install_path/lib64/python2.7/site-packages:$install_path/lib/python2.7/dist-packages:$install_path/lib64/python2.7/dist-packages:\$PYTHONPATH"
export LD_LIBRARY_PATH="$install_path/lib:$install_path/lib64/:\$LD_LIBRARY_PATH"
export LIBRARY_PATH="$install_path/lib:$install_path/lib64/:\$LIBRARY_PATH"
export PKG_CONFIG_PATH="$install_path/lib/pkgconfig:$install_path/lib64/pkgconfig:\$PKG_CONFIG_PATH"
#export PYBOMBS_PREFIX="$install_path"
export GRC_BLOCKS_PATH="$install_path/share/gnuradio/grc/blocks:$install_path/hier_blocs/"
EOF

chmod +x $install_path/activate