#!/usr/bin/env bash
cmake -DCMAKE_PREFIX_PATH=`brew --prefix qt5` . -DBUILD_PYTHON=ON -DBUILD_SERVER=ON
