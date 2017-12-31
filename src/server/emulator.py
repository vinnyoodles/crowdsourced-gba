from mgba.core import Core, find

def run(rom_path):
    core = find(rom_path)
    width, height = core.desiredVideoDimensions()
    core.setVideoBuffer(width)
    if not core.loadFile(rom_path):
        print('Failed to load rom file')
        return

    core.loadCoreConfig()

    # Reset the core. This is needed before it can run.
    core.reset()

    # while True:
    #     core.runFrame()
