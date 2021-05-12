# GafferDenoise

Denoising for Gaffer. Support for Intel OpenImageDenoise, Nvidia OptiX in the future.

> Note : Needs tests written, some cleanup.

### Build Instructions

**Requires:**

* cmake
* Gaffer Install
* OpenImageDenoise Install (or build from dependencies dir, it needs the ISPC compiler)

**In a terminal:**

Environment:
```
export GAFFER_ROOT <gaffer install path>
export GAFFERDENOISE_INSTALL_PREFIX <denoise extension install path>
```
Optional OpenImageDenoise build:
```
cd GafferDenoise/dependencies
build/build.py --project OpenImageDenoise --buildDir $GAFFERDENOISE_INSTALL_PREFIX --gafferRoot $GAFFER_ROOT --buildType release
cd ../..
```
Build/Install:
```
mkdir GafferDenoise/cmake-build-default
cd GafferDenoise/cmake-build-default
cmake -DGAFFER_ROOT=$GAFFER_ROOT -DCMAKE_INSTALL_PREFIX=$GAFFERDENOISE_INSTALL_PREFIX -DPYTHON_VERSION=3.7 ..
make install -j <num cores>
```

### Runtime Instructions

Now that you've installed the extension to `$GAFFERDENOISE_INSTALL_PREFIX`, you need to tell Gaffer about it:

`export GAFFER_EXTENSION_PATHS '$GAFFERDENOISE_INSTALL_PREFIX:$GAFFER_EXTENSION_PATHS'`

Now run `gaffer` as normal.