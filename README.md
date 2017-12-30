## Crowdsourced Gameboy Advanced

Inspired by [Twitch Plays Pokemon](https://www.twitch.tv/twitchplayspokemon).

This brings the idea of crowdsourced gaming off of twitch and onto a more global platform, the web browser.

### Compiling

#### OSX

mgba, the emulator, has its own dependencies.
```bash
brew install cmake ffmpeg imagemagick libzip qt5 sdl2 libedit
```

The following commands are to produce the `mgba-server` executable.
```bash
cd emulator
./osx_make.sh
make mgba-server
```

To run the server executable, load it with your desired GBA binary (denoted as GBA_BINARY).
```bash
./mgba-server GBA_BINARY
```

### Dependencies

- GBA Emulator, [mgba](https://github.com/mgba-emu/mgba) (Distributed under the [Mozilla Public License version 2.0](https://www.mozilla.org/en-US/MPL/2.0/))
