FROM python:3.6-jessie

RUN apt-get update && \
    yes | apt-get install qtbase5-dev \
    libsdl2-2.0 \
    zlib1g-dev \
    libedit-dev \
    libav-tools \
    libzip-dev \
    imagemagick \
    sqlite3 libsqlite3-dev \
    libelf-dev \
    libpng-dev \