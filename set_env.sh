. ./root_path.sh
export LIBRARY_PATH="/prefix/lib:/prefix/lib64/:$LIBRARY_PATH"
export PKG_CONFIG_PATH="/prefix/lib/pkgconfig:/prefix/lib64/pkgconfig:$PKG_CONFIG_PATH"
export PYBOMBS_PREFIX="/prefix"
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=$root_path/python
export GR_CONF_CONTROLPORT_ON=False
export PATH="/prefix/bin:$root_path/build/python:$PATH"
export LD_LIBRARY_PATH="$root_path/build/lib:/prefix/lib:/prefix/lib64/:$LD_LIBRARY_PATH"
export PYTHONPATH="$root_path/build/swig:/prefix/python:/prefix/lib/python2.6/site-packages:/prefix/lib64/python2.6/site-packages:/prefix/lib/python2.6/dist-packages:/prefix/lib64/python2.6/dist-packages:/prefix/lib/python2.7/site-packages:/prefix/lib64/python2.7/site-packages:/prefix/lib/python2.7/dist-packages:/prefix/lib64/python2.7/dist-packages:$PYTHONPATH"

. /prefix/setup_env.sh