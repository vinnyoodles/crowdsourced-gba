import mgba.core, mgba.image

def run(rom_path):
    core = mgba.core.loadPath(rom_path)
    width, height = core.desiredVideoDimensions()
    image = mgba.image.Image(width, height)
    core.setVideoBuffer(image)

    # Reset the core. This is needed before it can run.
    core.reset()

    while True:
        core.runFrame()
        print(image.toPIL())