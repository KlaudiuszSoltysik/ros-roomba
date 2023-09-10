// Connect with web bridge
let ros = new ROSLIB.Ros({
    url: 'ws://192.168.33.10:9090'
});

ros.on('connection', function () {
    console.log('Connected to websocket server.');
});


// Get buttons and forms
const map_room_btn = document.getElementsByClassName('el2')[0];
const go_to_btn = document.getElementsByClassName('el3')[0];
const clean_room_btn = document.getElementsByClassName('el6')[0];
const forward_btn = document.getElementsByClassName('el7')[0];
const left_btn = document.getElementsByClassName('el8')[0];
const right_btn = document.getElementsByClassName('el9')[0];
const backward_btn = document.getElementsByClassName('el10')[0];
const reset_speed_btn = document.getElementsByClassName('el11')[0];
const speedometer = document.getElementsByClassName('el13')[0];
const slider = document.getElementById('slider');
const x_input = document.getElementById('x-input');
const y_input = document.getElementById('y-input');
const img = document.getElementById('camera-img');


// Initialize variables
let is_mobile = /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent) ? true : false;
let linear_vel = 0;
let angular_vel = 0;
let is_working = false;
speedometer.innerHTML = linear_vel;


// Create topic variables
let cmd_vel = new ROSLIB.Topic({
    ros: ros,
    name: '/cmd_vel',
    messageType: 'geometry_msgs/Twist'
});

let camera = new ROSLIB.Topic({
    ros: ros,
    name: '/camera_translated',
    messageType: 'std_msgs/String'
});

let go_to_cli = new ROSLIB.Service({
    ros: ros,
    name: '/go_to',
    serviceType: 'custom_interfaces/GoTo'
});


// Subscribe to camra topic and update img tag
camera.subscribe(function (msg) {
    const imageDataUri = `data:image/jpeg;base64,${msg.data}`;
    img.src = imageDataUri;
});


// Add event listeners for PC and mobile users
if (is_mobile) {
    forward_btn.addEventListener('touchstart', function () {
        linear_vel = 1;
    });

    forward_btn.addEventListener('touchend', function () {
        linear_vel = 0;
    });

    left_btn.addEventListener('touchstart', function () {
        angular_vel = 1;
        if (slider.value == 0) {
            linear_vel = 0;
        }
    });

    left_btn.addEventListener('touchend', function () {
        angular_vel = 0;
    });

    right_btn.addEventListener('touchstart', function () {
        angular_vel = -1;
        if (slider.value == 0) {
            linear_vel = 0;
        }
    });

    right_btn.addEventListener('touchend', function () {
        angular_vel = 0;
    });

    backward_btn.addEventListener('touchstart', function () {
        linear_vel = -1;
    });
} else {
    forward_btn.addEventListener('mousedown', function () {
        linear_vel = 1;
    });

    forward_btn.addEventListener('mouseup', function () {
        linear_vel = 0;
    });

    left_btn.addEventListener('mousedown', function () {
        angular_vel = 1;
        if (slider.value == 0) {
            linear_vel = 0;
        }
    });

    left_btn.addEventListener('mouseup', function () {
        angular_vel = 0;
    });

    right_btn.addEventListener('mousedown', function () {
        angular_vel = -1;
        if (slider.value == 0) {
            linear_vel = 0;
        }
    });

    right_btn.addEventListener('mouseup', function () {
        angular_vel = 0;
    });

    backward_btn.addEventListener('mousedown', function () {
        linear_vel = -1;
    });

    backward_btn.addEventListener('mouseup', function () {
        linear_vel = 0;
    });
}

map_room_btn.addEventListener('mousedown', function () {
    stopRoomba();

    if (!is_working) {
        map_room_btn.innerHTML = "STOP";
        slider.value = 0;
        linear_vel = 0;
        speedometer.innerHTML = linear_vel;

        go_to_btn.disabled = true;
        clean_room_btn.disabled = true;

        enableDisableButtons(true);
    } else {
        map_room_btn.innerHTML = "MAP ROOM";

        go_to_btn.disabled = false;
        clean_room_btn.disabled = false;

        enableDisableButtons(false);
    }
});

go_to_btn.addEventListener('mousedown', function () {
    stopRoomba();

    if (!is_working && x_input.value != "" && y_input != "") {
        is_working = true;

        go_to_btn.innerHTML = "STOP";
        slider.value = 0;
        linear_vel = 0;
        speedometer.innerHTML = linear_vel;

        map_room_btn.disabled = true;
        clean_room_btn.disabled = true;

        enableDisableButtons(true);

        let request = new ROSLIB.ServiceRequest({
            x: parseFloat(x_input.value),
            y: parseFloat(y_input.value)
        });

        go_to_cli.callService(request, function (response) {
            console.log('Result for service call on '
                + go_to_cli.name
                + ': '
                + response.result);

            is_working = false;

            go_to_btn.innerHTML = "GO TO";
            x_input.value = "";
            y_input.value = "";

            map_room_btn.disabled = false;
            clean_room_btn.disabled = false;

            enableDisableButtons(true);
        });
    } else {
        go_to_btn.innerHTML = "GO TO";
        x_input.value = "";
        y_input.value = "";

        map_room_btn.disabled = false;
        clean_room_btn.disabled = false;

        enableDisableButtons(false);
    }
});

clean_room_btn.addEventListener('mousedown', function () {
    stopRoomba();

    if (!is_working) {
        clean_room_btn.innerHTML = "STOP";
        slider.value = 0;
        linear_vel = 0;
        speedometer.innerHTML = linear_vel;

        map_room_btn.disabled = true;
        go_to_btn.disabled = true;

        enableDisableButtons(true);
    } else {
        clean_room_btn.innerHTML = "CLEAN ROOM";

        map_room_btn.disabled = false;
        go_to_btn.disabled = false;

        enableDisableButtons(false);
    }
});

backward_btn.addEventListener('touchend', function () {
    linear_vel = 0;
});

slider.addEventListener('input', function () {
    linear_vel = slider.value / 100;
    speedometer.innerHTML = linear_vel;

    forward_btn.disabled = true;
    backward_btn.disabled = true;
});

reset_speed_btn.addEventListener('mousedown', function () {
    slider.value = 0;
    linear_vel = 0;
    speedometer.innerHTML = linear_vel;

    forward_btn.disabled = false;
    backward_btn.disabled = false;
});


// Other functions
setInterval(function () {
    if (!is_working) {
        const msg = new ROSLIB.Message({
            linear: { x: linear_vel, y: 0, z: 0 },
            angular: { x: 0, y: 0, z: angular_vel }
        });
        cmd_vel.publish(msg);
    }
}, 100);

function stopRoomba() {
    const msg = new ROSLIB.Message({
        linear: { x: 0, y: 0, z: 0 },
        angular: { x: 0, y: 0, z: 0 }
    });
    cmd_vel.publish(msg);
}

function enableDisableButtons(b) {
    forward_btn.disabled = b;
    left_btn.disabled = b;
    right_btn.disabled = b;
    backward_btn.disabled = b;
    reset_speed_btn.disabled = b;
}