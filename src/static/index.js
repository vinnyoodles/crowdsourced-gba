var host = window.location.host;
var ws = new WebSocket('ws://'+host+'/ws');
var SIZE_MODIFIER = 2;

document.addEventListener('DOMContentLoaded', function(event) { 
    // Handle incoming websocket message callback
    ws.onmessage = function(event) {
        if (event.data instanceof Blob) {
            updateFrame(event.data)
        } else {
            try {
                var json = JSON.parse(event.data);
                setCanvas(json.width, json.height);
            } catch (err) {
                console.log('Failed to handle', event.data)
            }
        }
    };
});

function updateFrame(frame) {
    var canvas = document.getElementById('canvas');
    var context = canvas.getContext('2d');
    var img = new Image();
    img.onload = function () {
        context.drawImage(img, 0, 0, img.width * SIZE_MODIFIER, img.height * SIZE_MODIFIER);
    }
    img.src = URL.createObjectURL(frame);
}

function setCanvas(width, height) {
    var canvas = document.getElementById('canvas');
    canvas.width = width * SIZE_MODIFIER;
    canvas.height = height * SIZE_MODIFIER;
}