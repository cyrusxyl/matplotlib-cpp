# C++ Template Project

This repo aims to serve as a template for C++ projects to fork off. It integrates static analyzers with the 
CI and build systems to support development of high quality code.

## Prerequisites

All the pre-requisites to compile this code are documented in the [Dockerfile](./docker/Dockerfile). The CI
system uses an image built with this Dockerfile to test changes.

Notes
1. If installing pip3 depedencies, ensure you either use `pipenv` or `pip3 install --user <lib>` 
so that you do not pollute your system python space.
2. If adding dependencies, prefer using `conan` for custom packages.

### Conan Setup

The [Conan](https://conan.io) package manager is used to manage project's external
dependencies. This section describes the process of setting it up.  Installation is as simple as running

```
pip3 install --user conan
```

#### Creating Profiles
We need to setup a Conan profile — a list of properties that characterize the
environment.  The following commands will create a new profile called `default` and set it up
for Ubuntu 16.04 environment.  If you are using several profiles, you may want to choose a
more descriptive name for it.

```
# create new profile called 'default'
conan profile new default --detect
# modify settings of the 'default' profile
conan profile update settings.compiler.version=5.4 default
conan profile update settings.compiler.libcxx=libstdc++11 default
```

At the moment, there exist precompiled versions of the packages needed by
the project for this particular profile:

```
os=Linux
arch=x86_64
compiler=gcc
compiler.version=5.4
compiler.libcxx=libstdc++11
build_type=Release
```

Note that you can have multiple profiles and also modify existing ones when needed.
For more details see the Conan [Getting Started](https://docs.conan.io/en/latest/getting_started.html) guide.

#### Setting up Remotes

Using the `setupConan.sh` script in [Ottomatika Scripts](https://bitbucket.ptc.delphiauto.net/bitbucket/projects/OTTO/repos/scripts/browse/setupConan.sh),
set up your conan instance locally to connect to the registry.

## Compilation
The first step is to get the source code, which you can do by
```
git clone <repository>
```

Go somewhere preferably _outside_ of the source tree and create a folder for the build:

```
mkdir build && cd build
```

By default, CMake will take care of installing Conan dependencies, so you just need to run:

```
cmake <repo_directory>
```

 If you want to use a Conan profile other then the default one, it can be passed to
 CMake via the `CONAN_PROFILE` option
```
cmake <repo_directory> -DCONAN_PROFILE=<profile>
```

If you prefer to use [Ninja](https://ninja-build.org/) as your build system,
remember to pass `-G Ninja` to `cmake`.  
```
cmake <repo_directory> -G Ninja -DCONAN_PROFILE=<profile>
```

You can now build the project :

```
cmake --build . [-- -jN]
ctest
```

As always, you may use your build system command directly (e.g., `make` or `ninja`) instead
of calling the more cumbersome `cmake --build .`.

CMake will use the profile name specified by  `CONAN_PROFILE` option.  If this variable
is not explicitly set, the `default` profile is used. Alternatively, if you do not want
CMake to handle the Conan installation, you can disable this functionality
by setting `CONAN_AUTO_INSTALL` to `OFF`.

## Misc

### Code Sanitizers

Code Sanitizing is an instrumentation technique that adds additional assembly instructions
into the generated code to catch particular events, which typically correspond to
bugs and errors.  There are a number of different sanitizers that can be enabled
at the moment by passing the corresponding `-DSANITIZE_xxx=ON` to the configuration stage.

- `SANITIZE_ADDRESS`
- `SANITIZE_LEAK`
- `SANITIZE_THREAD`
- `SANITIZE_UNDEFINED`

Leak Sanitizers is a subset of the Address Sanitizer, so running the
latter already includes all the checks of the former.  Note also, that most
sanitizers are incompatible with each other, and it is preferable not to combine them.
An exception is the pair of Undefined Behavior and Thread sanitizers, which may
be combined.

The project is setup in a way whereby when any of the `SANITIZE_xxx` options is enabled,
all libraries and executables will be compiled with the appropriate flags.
If you need to enable the sanitizing tools for a subset of the targets only,
set `SANITIZE_ALL_TARGETS=OFF` and then add the call `add_sanitizers(<target>)`
for your targets of interest.

If you wish to exclude specific targets from being sanitized, you can use the 
`EXCLUDE_FROM_SANITIZE_<SAN_NAME>` option. 

Note: Sanitizers are useful for catching bugs while you verify your code, but are not 
suitable for use in production.