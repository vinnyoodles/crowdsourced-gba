from mgba.core import Core, find

class Image(object):
    def __init__(self, width):
        self.buffer = []
        self.stride = width

def run(rom_path):
    core = find(rom_path)
    width, height = core.desiredVideoDimensions()
    image = Image(width)
    core.setVideoBuffer(width)
    core.loadFile(rom_path)
    core.loadCoreConfig()

    # Reset the core. This is needed before it can run.
    core.reset()

    # while True:
    #     core.runFrame()
