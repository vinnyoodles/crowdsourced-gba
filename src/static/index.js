var SIZE_MODIFIER = 2;
var KEY_MAP = {
    88: 0, // a
    90: 1, // b
    8:  2, // select
    13: 3, // start
    39: 4, // right
    37: 5, // left
    38: 6, // up
    40: 7, // down
    83: 8, // r
    65: 9, // l
};

var ws = new WebSocket('ws://' + window.location.host + '/ws');
var connected = false;

document.addEventListener('DOMContentLoaded', onLoad);
document.addEventListener('keydown', onKey);

function onLoad(event) {
    // Handle incoming websocket message callback
    ws.onmessage = function(event) {
        if (event.data instanceof Blob) {
            updateFrame(event.data)
        } else {
            try {
                var json = JSON.parse(event.data);
                // print out the clients commands if we have any
                if (json.event === 'new action' || json.event === 'all actions') {
                  console.log(json.event)
                  console.log(json.data)
                }
                setCanvas(json.width, json.height);
            } catch (err) {
                console.log('Failed to handle', event.data)
            }
        }
    };

    ws.onopen = function() {
        connected = true;
    }

    ws.onclose = function() {
        connected = false;
    }
}

function onKey(event) {
    if (!connected || !KEY_MAP[event.keyCode])
        return

    ws.send(KEY_MAP[event.keyCode]);
}

function updateFrame(frame) {
    var canvas = document.getElementById('canvas');
    var context = canvas.getContext('2d');
    var img = new Image();
    img.onload = function() {
        context.drawImage(img, 0, 0, img.width * SIZE_MODIFIER, img.height * SIZE_MODIFIER);
    }
    img.src = URL.createObjectURL(frame);
}

function setCanvas(width, height) {
    var canvas = document.getElementById('canvas');
    canvas.width = width * SIZE_MODIFIER;
    canvas.height = height * SIZE_MODIFIER;
}
