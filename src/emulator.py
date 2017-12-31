import mgba.core, mgba.image, io


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
            self.web_server.emit_frame(self.get_frame())

    def get_frame(self):
        image = self.image.toPIL().convert('RGB')
        buf = io.BytesIO()
        image.save(buf, format='PNG')
        return buf.getvalue()