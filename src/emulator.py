import mgba.core, mgba.image, io, sentry


class Emulator(object):
    def __init__(self, rom_path, web_server):
        self.web_server = web_server
        self.core = mgba.core.loadPath(rom_path)
        self.width, self.height = self.core.desiredVideoDimensions()
        self.image = mgba.image.Image(self.width, self.height)
        self.core.setVideoBuffer(self.image)
        self.core.autoloadSave()

        # Reset the core. This is needed before it can run.
        self.core.reset()
        self.enabled = True

        # The actions will be stored in a queue.
        self.queue = []

    def run(self):
        try:
            while self.enabled:
                key = 0 if len(self.queue) == 0 else self.queue.pop(0)
                self.core.setKeys(key)
                self.core.runFrame()
                self.web_server.emit_frame(self.get_frame())
        except:
            sentry.client.captureException()


    def get_frame(self):
        try:
            image = self.image.toPIL().convert('RGB')
            buf = io.BytesIO()
            image.save(buf, format='PNG')
            return buf.getvalue()[:]
        except:
            sentry.client.captureException()
            return []

    def push_key(self, key):
        self.queue.append(key)