\page install Installation

# Overview

This tutorial describes how to install Gazebo Test on Linux, Mac OS X and
Windows via either a binary distribution or from source.

[Install](#install)

* [Binary Install](#binary-install)

* [Source Install](#source-install)

    * [Prerequisites](#prerequisites)

    * [Building from Source](#building-from-source)

# Install

We recommend following the [Binary Install](#binary-install) instructions to get up and running as quickly and painlessly as possible.

The [Source Install](#source-install) instructions should be used if you need the very latest software improvements, you need to modify the code, or you plan to make a contribution.

## Binary Install

### Ubuntu Linux

Setup your computer to accept software from *packages.osrfoundation.org*:

```{.sh}
sudo sh -c 'echo "deb http://packages.osrfoundation.org/gazebo/ubuntu-stable `lsb_release -cs` main" > /etc/apt/sources.list.d/gazebo-stable.list'
```

Setup keys:

```{.sh}
wget http://packages.osrfoundation.org/gazebo.key -O - | sudo apt-key add -
```

Install Gazebo Test:

```
sudo apt install libgz-test<#>-dev
```

Be sure to replace `<#>` with a number value, such as 1 or 2, depending on
which version you need.

## Source Install

Source installation can be performed in UNIX systems by first installing the
necessary prerequisites followed by building from source.

### Building from source

1. Clone the repository

    ```
    git clone https://github.com/gazebosim/ign-test -b ign-test<#>
    ```
    Be sure to replace `<#>` with a number value, such as 1 or 2, depending on
    which version you need.

2. Install dependencies

    ```
    export SYSTEM_VERSION=bionic
    sudo apt -y install \
      $(sort -u $(find . -iname 'packages-'$SYSTEM_VERSION'.apt' -o -iname 'packages.apt') | tr '\n' ' ')
    ```

3. Configure and build

    ```
    cd ign-test; mkdir build; cd build; cmake ..; make
    ```

4. Optionally, install Gazebo Test

    ```
    sudo make install
    ```
