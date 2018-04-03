
gcc = meson.get_compiler('cpp')
udev=dependency('libudev')
libinput=dependency('libinput')

executable('display-server', ['main.cpp','server.cpp','gpu.cpp','connector.cpp','encoder.cpp','crtc.cpp','surface.cpp','dumb.cpp'],
    cpp_args:'-std=c++11',
    dependencies:[udev,libinput]
    )
