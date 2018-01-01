FROM ubuntu

MAINTAINER Puyush Goel <puyushgoel@gmail.com>

RUN apt-get update -y \
    apt-get install -y python3 \ 
    apt-get install -y pip3 \
    pip3 install -r requirements.txt \
    apt-get install qtbase5-dev \
    apt-get install libsdl2-2.0 \
    apt-get install zlib1g-dev \
    apt install libedit-dev \
    apt-get install -y ffmpeg \
    apt-get install -y libzip-dev \
    apt-get install -y imagemagick \
    apt-get install -y sqlite3 libsqlite3-dev \
    apt-get install -y libelf-dev \
    apt-get install -y libpng-dev \
    cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr . -DBUILD_PYTHON=ON -DBUILD_SERVER=OFF \
    make mgba-py \
    pip3 install -e ./python \
    python3 -c "import mgba"
