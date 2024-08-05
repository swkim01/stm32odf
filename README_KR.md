STM32 개방형 개발 프레임워크
===================

[영문](./README.md)

이 프레임워크는 [Neutree C CPP Framework](https://github.com/Neutree/c_cpp_project_framework) 및 [Kconfiglib](https://github.com/ulfalizer/Kconfiglib/) 에 기반하고 있으며 STMicroelectronics 사의 MCU를 위해 `CMake` 빌드 시스템과 build system and `Kconfig` 설정기를 활용한 **설정가능한** 프로젝트/SDK 템플릿을 제공한다.

현재 이 프레임워크는 ARM Cortex-M F1과 F4 계열 장치를 지원한다.

## 요구사항

* [arm embedded toolchain](https://developer.arm.com/downloads/-/gnu-rm)
* [cmake](https://cmake.org/) >= 3.22
* GNU Make or/and [ninja-build](https://ninja-build.org/)
* [openocd](https://openocd.org/)
* [python](https://www.python.org/downloads/)

## 설치

### 우분투 리눅스에서

* 다음 명령으로 ARM 프로세서를 위한 GCC 크로스 컴파일러, CMake, GNU Make, ninja-build, 및 python 을 설치한다:
```
sudo apt install build-essential make cmake gcc-arm-none-eabi ninja-build python3 python-is-python3 git
```

* 다음 명령으로 SDK 소스코드를 복사한다:
```
git clone https://github.com/swkim01/stm32odf.git
```

* `~/.bashrc` 파일을 편집하여 SDK 소스 디렉토리 아래의 `tools` 디렉토리를 PATH 환경변수에 추가한다.
```
nano ~/.bashrc
...
PATH=$PATH:[stm32odf directory]/tools
```

* `~/.bashrc` 스크립트 파일을 재실행한다:
```
source ~/.bashrc
```

### MS 윈도에서

* 다음 링크로부터 ARM 프로세서를 위한 GCC 크로스 컴파일러의 바이너리 버전을 다운로드하고 설치한다:
[https://developer.arm.com/downloads/-/gnu-rm](https://developer.arm.com/downloads/-/gnu-rm)

* 다음 링크로부터 CMake 빑드 도구의 바이너리 버전을 다운로드하고 설치한다:
[https://cmake.org/download/](https://cmake.org/download/)

* 다음 링크로부터 GNU Make 빑드 도구의 바이너리 버전을 다운로드하고 설치한다:
[https://gnuwin32.sourceforge.net/packages/make.htm](https://gnuwin32.sourceforge.net/packages/make.htm)

* 다음 링크로부터 Ninja 빑드 도구를 다운로드하고 압축을 풀고 경로를 PATH 환경변수에 추가한다:
[https://github.com/ninja-build/ninja/releases](https://github.com/ninja-build/ninja/releases)

* 다음 링크로부터 OpenOCD 펌웨어 다운로드 도구를 다운로드하고 7z 파일의 압축을 풀고 bin 실행 경로를 PATH 환경변수에 추가한다:
[https://gnutoolchains.com/arm-eabi/openocd/](https://gnutoolchains.com/arm-eabi/openocd/)

* 다음 링크로부터 파이썬을 다운로드하고 설치 프로그램을 실행한 후에 `Add python.exe to PATH` 항목을 체크하여 `python.exe` 실행 경로를 PATH 환경변수에 추가하도록 설치한다:
[https://www.python.org/downloads/](https://www.python.org/downloads/)

* 파워쉘을 실행한 후에 다음 명령을 입력하여 `windows-curses` 패키지를 설치한다:
```
pip install windows-curses
```

* 다음 링크로부터 SDK 소스코드를 다운로드하고 압축을 푼다:
[https://github.com/swkim01/stm32odf](https://github.com/swkim01/stm32odf)

* SDK 소스 디렉토리 아래의 `tools` 디렉토리(`[stm32odf 디렉토리]/tools`) 를 PATH 환경변수에 추가한다.

## 시작하기

* Nucleo STM32F446 보드 또는 STLink 보드가 포함된 STM32F1/F4 호환 보드를 준비한다.

* SDK 소스 디렉토리의 `hello_world` 예제 디렉토리로 이동한다.
```
cd [stm32odf directory]
cd examples/hello_world
```

* `configs_default.mk` 파일을 편집하여 툴체인 경로 `CONFIG_TOOLCHAIN_PATH` 를 GCC 크로스 컴파일러(`arm-none-eabi-gcc`)가 설치된 디렉토리로, `CONFIG_TOOLCHAIN_PREFIX` 를 `arm-none-eabi-` 로 수정한다.
```
nano config_defaults.mk
CONFIG_TOOLCHAIN_PATH="[arm-none-eabi-gcc 설치 디렉토리]"
CONFIG_TOOLCHAIN_PREFIX="arm-none-eabi-"
```

* `stm32odf.py` 스크립트를 사용하여 소프트웨어를 설정하고 펌웨어를 빌드한다. MS 윈도우에서는 cmd 쉘 대신 파워쉘을 사용한다.
```
stm32odf.py menuconfig
stm32odf.py build
# 컴파일 정보를 보기 위해 --verbose 인자를 사용하면, 오류가 발생했을 때 유용하다
# stm32odf.py build --verbose
stm32odf.py clean
stm32odf.py distclean
```

* 여기서,
* `stm32odf.py menuconfig` 명령으로 프로젝트를 설정하면, `build/config` 디렉터리에 `global_config` 파일을 생성하고, 각 구성요소의 `CMakelists.txt` 에서 직접 사용하거나, `C/CPP` 소스 파일에서 `#include "global_config.h"` 코드로 사용할 수 있다.
* `stm32odf.py build` 명령으로 프로젝트를 빌드하거나 `stm32odf.py build --verbose` 명령으로 자세한 빌드 정보를 출력한다.
* `stm32odf.py clean` 명령으로 빌드를 청소하면 툴체인 설정은 삭제하지 않지만, `stm32odf.py distclean` 을 실행하면 `menuconfig` 로 생성된 설정도 청소한다.

* MCU 장치가 연결된 stlink 보드를 USB 포트에 연결하고, 다음 명령으로 펌웨어를 장치에 플래시한다
```
stm32odf.py flash
```

## 디렉토리 구조

| directory/file | function |
| -------------- | -------- |
| root directory | 프로젝트 루트 디렉토리, 또한 `SDK` 디렉토리 |
| components     | 컴포넌트/라이브러리 |
| examples       | 프로젝트 디렉토리； `SDK` 예제 디렉토리, 이 디렉토리는 `STM32ODF_PATH` 환경변수로 설정한 `SDK` 디렉토리 경로와 분리하여 별도의 워크스페이스에 저장할 수 있다. |
| tools          | `stm32odf.py`, `cmake`、`kconfig`를 포함한 도구 |
| Kconfig        | 루트`Kconfig` 설정 |


### 1) 컴포넌트

모든 라이브러리는 `components` 디렉토리의 컴포넌트 또는 프로젝트 디렉토리 아래에 둔다. 각 컴포넌트는 하나의 디렉토리를 사용하며 이름은 디렉토리와 같다. 프로젝트를 더욱 간결하게 하기 위해 컴포넌트들은 중첩되지 않는다. 모든 컴포넌트는 계층적이며 의존성에 따라 컴포넌트들 사이의 관계가 결정된다.

모든 소스 파일은 컴포넌트 안에 있어야 한다. 각 프로젝트는 `main`이라는 컴포넌트(예, `examples/hello_world/main` 디렉토리)를 가져야 한다. 각 컴포넌트는 다음 파일을 가진다:

* `CMakeLists.txt`: 반드시 존재해야 하고, 컴포넌트 소스 파일과 의존 컴포넌트를 정의하고, 자신을 등록하는 등록 함수(`register_component`) 를 호출한다. 자세한 것은 `components/stm32hal` 및 `components/boards` 의 `CMakeLists.txt` 를 참고한다.

* `Kconfig`: 옵션이며, 이 컴포넌트에 대한 설정 옵션들을 포함한다. 이 컴포넌트와 의존하는 하위 컴포넌트의 `CMakeLists.txt` 파일에서는, 설정 옵션들에 `CONFIG_` 접두사를 붙여서 사용할 수 있다. 예를 들어, `components/stm32hal` 디렉토리에서, `Kconfig` 파일에 `LIB_STM32HAL_ENABLED` 항목이 있다면, `CMakeLists.txt` 파일에서는 `if(CONFIG_LIB_STM32HAL_ENABLED)` 문장을 사용하여 사용자 설정이 이 컴포넌트를 포함하는지 말지를 결정하도록 할 수 있다.

### 2) 프로젝트 디렉토리

예제 프로젝트는 `examples` 디렉토리 안에 있다. 물론, 프로젝트 디렉토리는 디스크의 아무 디렉토리에나 둘 수 있다. 프로젝트 디렉토리는 `main` 컴포넌트가 있어야 하고, 커스텀 컴포넌트를 포함할 수 있다. 자세한 사항은 `examples/hello_world` 디렉토리를 참고한다.

프로젝트 디렉토리 아래의 파일들:

* `CMakeLists.txt`: 프로젝트 속성 파일이어서 반드시 존재해야 하며, 먼저 `include(${SDK_PATH}/tools/cmake/compile.cmake)` 를 포함한 후에, `project(hello_world)` 처럼 프로젝트 이름을 인자로 `project` 함수를 호출한다. 물론, `CMake` 문법을 사용하여 다른 조건이나 변수를 추가할 수 있다. 자세한 사항은 `examples/hello_world/CMakeLists.txt` 을 참고한다.

* `config_defaults.mk`: 프로젝트 기본 설정 파일이며 옵션이다. 설정 형식은 `Makefile` 이다. 터미널 GUI 설정 (`stm32odf.py menuconfig`) 을 사용하면 설정 파일을 생성하는데, 생성된 설정 파일은 `build/config/global_config.mk` 이며, 이 파일을 `config_defaults.mk` 파일로 복사하면 된다.
> 주의: 현재 빌드 시스템은 이미 존재하는 설정 파일 (`build/config/global_config.mk`) 을 사용하므로,  `config_defaults.mk` 파일을 수정한 후에 설정 파일을 재생성하려면  `build` 디렉토리의 파일들 (또는 `build/config` 디렉토리의 `mk` 파일) 을 삭제해야 한다.

프로젝트 디렉토리를 디스크의 임의 위치에 두는 방법:

* `STM32ODF_PATH` 환경변수를 SDK 디렉토리 경로로 설정한 후에 빌드하면 된다

## 새로운 프로젝트를 생성하는 방법

일반적으로, 새로운 프로젝트는 원하는 대로 프로젝트 디렉토리 아래에 빈 디렉토리 (예, `stm32_ws/blinky`) 를 생성한 후에 `examples/hello_world` 안의 파일들을 복사하면 된다.

여기서 `configs_default.mk` 파일을 편집하여 툴체인 경로 `CONFIG_TOOLCHAIN_PATH` 를 GCC 크로스 컴파일러(`arm-none-eabi-gcc`)가 설치된 디렉토리로, `CONFIG_TOOLCHAIN_PREFIX` 를 `arm-none-eabi-` 로 수정한다.
```
nano config_defaults.mk
CONFIG_TOOLCHAIN_PATH="[arm-none-eabi-gcc 설치 디렉토리]"
CONFIG_TOOLCHAIN_PREFIX="arm-none-eabi-"
```

추가로, 프로젝트 디렉토리와 SDK 디렉토리는 분리 저장할 수 있다. 이것은 SDK  를 별도로 복사해서 개발하는 오픈 소스 프로젝트들에 특히 유용하다.
이렇게 하려면 다음이 필요하다:

* `SDK` 를 다운로드하고 `/home/linux/stm32odf` 과 같은 특정한 디렉토리에 둔다
```
git clone https://github.com/swkim01/stm32odf.git
```

* 다음은 터미널에서 `export STM32ODF_PATH=/home/linux/stm32_odf`을 실행하여 환경변수를 export 한다. 아니면, `~/.bashrc` 또는 `~/.zshrc` 파일에 저장하여 터미널이 시작될 때마다 자동으로 추가되도록 한다.
* 다음은 `example/hello_world` 디렉토리의 내용을 전부 복사해서 `/home/linux/stm32_ws/hello_world` 처럼 임의 위치에 프로젝트를 생성한다.

* 다음은 이전 빌드 캐쉬를 클리어한다.
```
stm32odf.py distclean
```

* 그런 다음 설정하고 빌드하고 펌웨어를 다운로드하면 된다.
```
stm32odf.py menuconfig
stm32odf.py build
stm32odf.py flash
```

## 프로젝트 생성기 바꾸기

때로는 빌드 속도가 더 빠르기를 원하거나 비쥬얼 스튜디오와 같은 IDE 용으로 프로젝트를 생성하고 싶을 때가 있다. 이렇게 하기 위해 생성기를 바꿀 수 있다. 기본 생성기는 `Unix Makefiles` 이다.

`Ninja`, `Visual Studio`, `Xcode`, `Eclipse`, `Unix Makefiles` 등과 같은 수많은 생성기가 있다. `cmake --help` 명령을 실행하면 생성기 선택 사항과 다른 시스템에서 지원하는 생성기들을 볼 수 있다.
예를 들면 리눅스는 다음과 같다:
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
생성기를 바꾸려면 프로젝트 디렉토리의 `.config.mk` 파일에 `CONFIG_CMAKE_GENERATOR=Ninja` 와 같이 설정하거나 `config` 명령을 실행하면 된다.
```
# clean all build files first(remove build dir)
stm32odf.py distclean

stm32odf.py -G Ninja config
# stm32odf.py -G "Eclipse CDT4 - Ninja" config

stm32odf.py build
```

## 라이센스

see [LICENSE](./LICENSE)


## 이 프로젝트에 사용된 오픈 소스 프로젝트들

* [Kconfiglib](https://github.com/ulfalizer/Kconfiglib)： `Kconfig`'s `Python` implementation
* [STM32CubeF1](https://github.com/STMicroelectronics/STM32CubeF1)： `STMicroelectronics`'s SDK for STM32F4 MCUs
* [STM32CubeF4](https://github.com/STMicroelectronics/STM32CubeF4)： `STMicroelectronics`'s SDK for STM32F4 MCUs
* [FreeRTOS](https://github.com/FreeRTOS/FreeRTOS-Kernel): FreeRTOS 커널

## 다른 비슷한 소프트웨어

* [ESP_IDF](https://github.com/espressif/esp-idf)：  `SDK` of `ESP32`, Written very well
* [nRF Connect SDK](https://github.com/nrfconnect/sdk-nrf): nRF Connect SDk by Nordic Semiconductors

