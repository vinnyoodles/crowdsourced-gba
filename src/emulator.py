import mgba.core, mgba.image

class Emulator(object):
    def __init__(self, rom_path, web_server):
        self.web_server = web_server
        self.core = mgba.core.loadPath(rom_path)
        self.width, self.height = self.core.desiredVideoDimensions()
        self.image = mgba.image.Image(self.width, self.height)
        self.core.setVideoBuffer(self.image)

        # Reset the core. This is needed before it can run.
        self.core.reset()
        self.enabled = True

    def run(self):
        while self.enabled:
            self.core.runFrame()
            self.web_server.emit_frame(self.image.raw_buffer())