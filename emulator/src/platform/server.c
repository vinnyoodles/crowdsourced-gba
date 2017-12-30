// This source file is placed into the public domain.
#include <mgba/core/core.h>
#include <mgba/feature/commandline.h>
#include <mgba-util/socket.h>

#define DEFAULT_PORT 2578

static bool run_loop(const struct mArguments* args, Socket client);

int main(int argc, char** argv) {
	bool did_fail = false;

	// Arguments from the command line are parsed by the parseArguments function.
	// The NULL here shows that we don't give it any arguments beyond the default ones.
	struct mArguments args = {};
	bool parsed = parseArguments(&args, argc, argv, NULL);
	// Parsing can succeed without finding a filename, but we need one.
	if (!args.fname) {
		parsed = false;
	}
	if (!parsed || args.showHelp) {
		// If parsing failed, or the user passed --help, show usage.
		usage(argv[0], NULL);
		did_fail = !parsed;
		goto cleanup;
	}

	if (args.showVersion) {
		// If the user passed --version, show version.
		version(argv[0]);
		goto cleanup;
	}

	// Initialize the socket layer and listen on the default port for this protocol.
	SocketSubsystemInit();
	Socket sock = SocketOpenTCP(DEFAULT_PORT, NULL);
	if (SOCKET_FAILED(sock) || SOCKET_FAILED(SocketListen(sock, 0))) {
		SocketSubsystemDeinit();
		did_fail = true;
		goto cleanup;
	}

	// We only grab one client.
	Socket client = SocketAccept(sock, NULL);
	if (SOCKET_FAILED(client)) {
		SocketClose(sock);
		SocketSubsystemDeinit();
		did_fail = true;
		goto cleanup;		
	}

	// Run the server
	did_fail = run_loop(&args, client);

	// Clean up the sockets.
	SocketClose(client);
	SocketClose(sock);
	SocketSubsystemDeinit();

	cleanup:
	freeArguments(&args);

	return did_fail;
}

bool run_loop(const struct mArguments* args, Socket client) {
	// First, we need to find the mCore that's appropriate for this type of file.
	// If one doesn't exist, it returns NULL and we can't continue.
	struct mCore* core = mCoreFind(args->fname);
	if (!core) {
		return false;
	}

	// Initialize the received core.
	core->init(core);

	// Get the dimensions required for this core and send them to the client.
	unsigned width, height;
	core->desiredVideoDimensions(core, &width, &height);
	ssize_t bufferSize = width * height * BYTES_PER_PIXEL;
	uint32_t sendNO;
	sendNO = htonl(width);
	SocketSend(client, &sendNO, sizeof(sendNO));
	sendNO = htonl(height);
	SocketSend(client, &sendNO, sizeof(sendNO));
	sendNO = htonl(BYTES_PER_PIXEL);
	SocketSend(client, &sendNO, sizeof(sendNO));

	// Create a video buffer and tell the core to use it.
	// If a core isn't told to use a video buffer, it won't render any graphics.
	// This may be useful in situations where everything except for displayed
	// output is desired.
	void* videoOutputBuffer = malloc(bufferSize);
	core->setVideoBuffer(core, videoOutputBuffer, width);

	// Tell the core to actually load the file.
	mCoreLoadFile(core, args->fname);

	// Initialize the configuration system and load any saved settings for
	// this frontend. The second argument to mCoreConfigInit should either be
	// the name of the frontend, or NULL if you're not loading any saved
	// settings from disk.
	mCoreConfigInit(&core->config, "client-server");
	mCoreConfigLoad(&core->config);

	// Take any settings overrides from the command line and make sure they get
	// loaded into the config system, as well as manually overriding the
	// "idleOptimization" setting to ensure cores that can detect idle loops
	// will attempt the detection.
	applyArguments(args, NULL, &core->config);
	mCoreConfigSetDefaultValue(&core->config, "idleOptimization", "detect");

	// Tell the core to apply the configuration in the associated config object.
	mCoreLoadConfig(core);

	// Reset the core. This is needed before it can run.
	core->reset(core);

	uint16_t inputNO;
	while (SocketRecv(client, &inputNO, sizeof(inputNO)) == sizeof(inputNO)) {
		// After receiving the keys from the client, tell the core that these are
		// the keys for the current input.
		core->setKeys(core, ntohs(inputNO));

		// Emulate a single frame.
		core->runFrame(core);

		// Send back the video buffer.
		if (SocketSend(client, videoOutputBuffer, bufferSize) != bufferSize) {
			break;
		}
	}

	// Deinitialization associated with the core.
	mCoreConfigDeinit(&core->config);
	core->deinit(core);

	return true;
}
