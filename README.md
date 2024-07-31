STM32 Open Development Framework
===================

[Korean](./README_KO.md)

This framework is based on [Neutree C CPP Framework](https://github.com/Neutree/c_cpp_project_framework) with [Kconfiglib](https://github.com/ulfalizer/Kconfiglib/) and provides **configurable** `C/C++` project/SDK template with `CMake` build system and `Kconfig` configuration for STMicroelectronics's MCUs.

This framework currently supports ARM Cortex-M F1 and F4 device families.

## Requirements

* [arm embedded toolchain](https://developer.arm.com/downloads/-/gnu-rm)
* [cmake](https://cmake.org/) >= 3.22
* GNU Make or/and [ninja-build](https://ninja-build.org/)
* [openocd](https://openocd.org/)
* [python](https://www.python.org/downloads/)

## Install

### On Ubuntu Linux

* Install GCC Cross compiler for ARM processor, CMake, GNU Make, ninja-build, and python all together by:
```
sudo apt install build-essential make cmake gcc-arm-none-eabi ninja-build python3 python-is-python3
```

* Clone code by:
```
git clone https://github.com/swkim01/stm32odf
```

* Edit '~/.bashrc' file to add `tools` directory under the SDK source to PATH enviroment variable.
```
nano ~/.bashrc
...
PATH=$PATH:[stm32odf directory]/tools
```

* Rerun `~/.bashrc` script file:
```
source ~/.bashrc
```

### On MS Window

* Install GCC Cross compiler for ARM processor, downloading and installing the binary version from:
[https://developer.arm.com/downloads/-/gnu-rm](https://developer.arm.com/downloads/-/gnu-rm)

* Install CMake build tool, downloading and installing the binary version from:
[https://cmake.org/download/](https://cmake.org/download/)

* Install GNU Make build tool, downloading and installing the binary version from:
[https://gnuwin32.sourceforge.net/packages/make.htm](https://gnuwin32.sourceforge.net/packages/make.htm)

* Install Ninja build tool, downloading, unzipping the zip file, and adding the execution path to PATH variable from:
[https://github.com/ninja-build/ninja/releases](https://github.com/ninja-build/ninja/releases)

* Install OpenOCD firmware download tool, downloading, unzipping the 7z file, and adding the bin execution path to PATH variable from:
[https://gnutoolchains.com/arm-eabi/openocd/](https://gnutoolchains.com/arm-eabi/openocd/)

* Install python, downloading from the next link, executing it and checking `Add python.exe to PATH` item to add`python.exe` path to PATH variable:
[https://www.python.org/downloads/](https://www.python.org/downloads/)

* After executing CMD shell, then input the following command to install `windows-curses` package:
```
cmd> pip install windows-curses
```

* Clone code by:
```
git clone https://github.com/swkim01/stm32odf
```

* Add `tools` directory under the SDK source to PATH enviroment variable.

## Getting Started

* Prepare Nucleo STM32F446 board or STM32F1/F4 compatible board with STLink board.

* Go to the directory of `hello_world` example.
```
cd [stm32odf directory]
cd examples/hello_world
```

* Edit `configs_default.mk` file to modify toolchain path variable `CONFIG_TOOLCHAIN_PATH` to the directory where the GCC cross compiler (`arm-none-eabi-gcc`) was installed, and `CONFIG_TOOLCHAIN_PREFIX` to `arm-none-eabi-` correspondingly.
```
nano config_defaults.mk
CONFIG_TOOLCHAIN_PATH="[arm-none-eabi-gcc installed directory]"
CONFIG_TOOLCHAIN_PREFIX="arm-none-eabi-"
```

* Configure the software and build by using `stm32odf.py` script.
```
stm32odf.py menuconfig
stm32odf.py build
# you can use --verbose arg to see more compile info, this is useful when error occurs
# stm32odf.py build --verbose
stm32odf.py clean
tools/stm32odf.py distclean
# stm32odf.py clean_conf
```

* Here,
* Config project by command `stm32odf.py menuconfig`, it will generate `global_config` files at `build/config` directory, so we can use it in component's `CMakelists.txt` directly， or in `C/CPP` source files by `#include "global_config.h"`
* Build stm32odf by command `stm32odf.py build`, or output verbose build info with command `python stm32odf.py build --verbose`
* Clean build by `stm32odf.py clean`, clean config generated by `menuconfig` by `python stm32odf.py distclean`, this command will not clean toolchain config

* Connect the mcu device through stlink board to USB port, and flash the firmware to it by
```
stm32odf.py flash
```

## Structure

| directory/file | function |
| -------------- | -------- |
| root directory | root directory of this project, also `SDK` projects' `SDK` directory |
| components     | as a component/lib |
| examples       | project dir； `SDK` projects' example/project dir, this directory can be separated from the `SDK` directory, just set environment `STM32ODF_PATH` to `SDK` directory's path. |
| tools          | tools like `cmake`、`kconfig`、`burn tool` etc. |
| Kconfig        | root `Kconfig` configuration |


### 1) Component

All libraries are placed as components in the `components` directory or under the project directory. Each component uses a directory. This directory is the name of the component. In order to make the project look more concise, the components are not nested. All components are a hierarchy, and the relationships between components depend on dependencies to maintain

All source files must be in a component. Each project must contain a component called `main` (ie `examples/hello_world/main` directory). Each component contains the following files:

* `CMakeLists.txt`: Must exist, declare the component source file and the dependent component, and call the registration function `register_component` to register itself. For details, please refer to `CMakeLists.txt` of `components/stm32hal` and `components/boards`.

* `Kconfig`: Optional, contains configuration options for this component. In this component or other `CMakeLists.txt` that depends on the component of this component, you can use these configuration items after adding a `CONFIG_` prefix. e.g. In `components/stm32hal`, there is a `LIB_STM32HAL_ENABLED` option in `Kconfig`. We use this variable `if(CONFIG_LIB_STM32HAL_ENABLED)` in its `CMakeLists.txt` to determine if the user configuration want to register this component or not.

### 2) Project Directory

The project directory is in the `examples` directory. Of course, you can pu the project directory anywhere on the disk. As mentioned above, there must be a `main` component in each project directory. Of course, you can also put a lot of custom components. More refer to the `examples/hello_world` project directory.

Files under the project directory:

* `CMakeLists.txt`: must exist, project properties file, you must first include `include(${SDK_PATH}/tools/cmake/compile.cmake)`, then use the `project` function to declare project name, such as `project(hello_world)`, Of course, you can also write other conditions or variables, etc., using the `CMake` syntax, refer to the `examples/hello_world/CMakeLists.txt`

* `config_defaults.mk`: Optional, project default configuration file, the default configuration will be loaded when `cmake` execute. The format of the configuration is `Makefile`. You can use the terminal GUI configuration (`make menuconfig`) to generate the configuration file, the generated configuration file is in `build/config/global_config.mk`, then copy to `config_defaults.mk`.
> Note: After modifying `config_defaults.mk`, you need to delete the files in the `build` directory (or just delete the `mk` file in the `build/config` directory) to regenerate, because the current build system will use the existing configuration file (`build/config/global_config.mk`)

How to put the project directory anywhere on the disk:

* Set the `STM32ODF_PATH` environment variable in the terminal to the path of the `SDK`, then you can build

## How to make new project

Normally, you can create a new directory in the project root directory in any place on disk, such as `stm32_ws/blinky`, and copy files in the `examples/hello_world`'s content to start new project

Then, edit `configs_default.mk` file to modify toolchain path variable `CONFIG_TOOLCHAIN_PATH` to the directory where the GCC cross compiler (`arm-none-eabi-gcc`) was installed, and `CONFIG_TOOLCHAIN_PREFIX` to `arm-none-eabi-` correspondingly.
```
nano config_defaults.mk
CONFIG_TOOLCHAIN_PATH="[arm-none-eabi-gcc installed directory]"
CONFIG_TOOLCHAIN_PREFIX="arm-none-eabi-"
```

In addition, the project directory and the SDK directory can also be stored separately. This is especially used for open source projects, where uses a copy of SDK.
To do this, only need:

* Download `SDK` and put it in a directory, such as `/home/linux/stm32odf`

```
git clone https://github.com/swkim01/stm32odf
```

* Then export the variable `export STM32ODF_PATH=/home/linux/stm32odf` in the terminal, which can be placed in the `~/.bashrc` or `~/.zshrc` file, so that this variable will be automatically added every time the terminal is started
* Then create a project anywhere, such as copy the entire content of the folder of `examples/hello_world` to `/home/llinux/stm32_ws/hello_world`
* Then clear the previous build cache (if there is one, ignore it if there is none)
```
stm32odf.py distclean
```

* Then configure, build and flash
```
stm32odf.py menuconfig
stm32odf.py build
stm32odf.py flash
```

## Change project generator

Sometimes you want to faster build speed or generate project for some IDE like Visual Studio,
you can change generator to achieve this, default generator is `Unix Makefiles`.

There are many generator choices, such as `Ninja`, `Visual Studio`, `Xcode`, `Eclipse`, `Unix Makefiles` etc.
Execute command `cmake --help` to see the generator choices, different system support different generators.
Linux for example:
```
Generators

The following generators are available on this platform (* marks default):
* Unix Makefiles               = Generates standard UNIX makefiles.
  Ninja                        = Generates build.ninja files.
  Ninja Multi-Config           = Generates build-<Config>.ninja files.
  Watcom WMake                 = Generates Watcom WMake makefiles.
  CodeBlocks - Ninja           = Generates CodeBlocks project files.
  CodeBlocks - Unix Makefiles  = Generates CodeBlocks project files.
  CodeLite - Ninja             = Generates CodeLite project files.
  CodeLite - Unix Makefiles    = Generates CodeLite project files.
  Eclipse CDT4 - Ninja         = Generates Eclipse CDT 4.0 project files.
  Eclipse CDT4 - Unix Makefiles= Generates Eclipse CDT 4.0 project files.
  Kate - Ninja                 = Generates Kate project files.
  Kate - Unix Makefiles        = Generates Kate project files.
  Sublime Text 2 - Ninja       = Generates Sublime Text 2 project files.
  Sublime Text 2 - Unix Makefiles
                               = Generates Sublime Text 2 project files.
```

You can change it by adding like `CONFIG_CMAKE_GENERATOR=Ninja` to `.config.mk` in the project directory or `config` command
```
# clean all build files first(remove build dir)
stm32odf.py distclean

stm32odf.py -G Ninja config
# stm32odf.py -G "Eclipse CDT4 - Ninja" config

stm32odf.py build
```

## License

see [LICENSE](./LICENSE)


## Open source projects used by this project

* [Kconfiglib](https://github.com/ulfalizer/Kconfiglib)： `Kconfig`'s `Python` implementation
* [STM32CubeF1](https://github.com/STMicroelectronics/STM32CubeF1)： `STMicroelectronics`'s SDK for STM32F4 MCUs
* [STM32CubeF4](https://github.com/STMicroelectronics/STM32CubeF4)： `STMicroelectronics`'s SDK for STM32F4 MCUs
* [FreeRTOS](https://github.com/FreeRTOS/FreeRTOS-Kernel): FreeRTOS Kernel

## Other Similar Reference

* [ESP_IDF](https://github.com/espressif/esp-idf)：  `SDK` of `ESP32`, Written very well
* [nRF Connect SDK](https://github.com/nrfconnect/sdk-nrf): nRF Connect SDk by Nordic Semiconductors

