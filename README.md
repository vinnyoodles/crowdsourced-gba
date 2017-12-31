## Crowdsourced Gameboy Advanced

Inspired by [Twitch Plays Pokemon](https://www.twitch.tv/twitchplayspokemon).

This brings the idea of crowdsourced gaming off of twitch and onto a more global platform, the web browser.

### Building Python Emulator

#### OSX

mgba, the emulator, has its own dependencies.
```bash
brew install cmake ffmpeg imagemagick libzip qt5 sdl2 libedit
```

The following commands are to produce the `mgba` python library.
```bash
cd emulator
./osx_make.sh
make mgba-py
```

There should now be a `python` directory, this contains the source code to install as a python module.
```bash
pip3 install -e ./python
```

To test, running the following command should not produce any errors.

```bash
python -c "import mgba"
```

__Note__: If you're getting an `ImportError` due to importing mgba, try setting the following environment variable to the directory where the dylib files are stored (denoted as `MGBA_DYLIB_PATH`).

```bash
export DYLD_LIBRARY_PATH=MGBA_DYLIB_PATH
```

### Dependencies

- [mgba](https://github.com/mgba-emu/mgba), GBA emulator with exposed python APIs (Distributed under the [Mozilla Public License version 2.0](https://www.mozilla.org/en-US/MPL/2.0/))
- [Tornado](https://github.com/tornadoweb/tornado), python web server that supports websockets natively
- [Pillow](https://github.com/python-pillow/Pillow), python imaging library
