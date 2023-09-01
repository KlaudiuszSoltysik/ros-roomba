// Connect with web bridge
var ros = new ROSLIB.Ros({
    url : 'ws://192.168.33.10:9090'
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
const xInput = document.getElementById('x-input');
const yInput = document.getElementById('y-input');

// Initialize variables
let linear_vel = 0;
let angular_vel = 0;
let x_goal;
let y_goal;
let is_working = false;
speedometer.innerHTML = linear_vel;

// Create /cmd_vel topic variable
var cmd_vel = new ROSLIB.Topic({
    ros: ros,
    name: '/cmd_vel',
    messageType: 'geometry_msgs/Twist'
});

// Add event listeners
map_room_btn.addEventListener('mousedown', function () {
    stopRoomba();

    if (!is_working) {
        map_room_btn.innerHTML = "STOP";
        go_to_btn.disabled = true;
        clean_room_btn.disabled = true;
        forward_btn.disabled = true;
        left_btn.disabled = true;
        right_btn.disabled = true;
        backward_btn.disabled = true;
        reset_speed_btn.disabled = true;

        slider.value = 0;
        linear_vel = 0;
        speedometer.innerHTML = linear_vel;
    } else {
        map_room_btn.innerHTML = "MAP ROOM";
        go_to_btn.disabled = false;
        clean_room_btn.disabled = false;
        forward_btn.disabled = false;
        left_btn.disabled = false;
        right_btn.disabled = false;
        backward_btn.disabled = false;
        reset_speed_btn.disabled = false;
    }

    is_working = !is_working;
});

go_to_btn.addEventListener('mousedown', function () {
    stopRoomba();

    if (!is_working) {
        map_room_btn.disabled = true;
        go_to_btn.innerHTML = "STOP";
        clean_room_btn.disabled = true;
        forward_btn.disabled = true;
        left_btn.disabled = true;
        right_btn.disabled = true;
        backward_btn.disabled = true;
        reset_speed_btn.disabled = true;

        slider.value = 0;
        linear_vel = 0;
        speedometer.innerHTML = linear_vel;

        xInput.value = '';
        yInput.value = '';
    } else {
        map_room_btn.disabled = false;
        go_to_btn.innerHTML = "GO TO";
        clean_room_btn.disabled = false;
        forward_btn.disabled = false;
        left_btn.disabled = false;
        right_btn.disabled = false;
        backward_btn.disabled = false;
        reset_speed_btn.disabled = false;
    }

    is_working = !is_working;
});

xInput.addEventListener('change', function () {
    x_goal = xInput.value;
});

yInput.addEventListener('change', function () {
    y_goal = yInput.value;
});

clean_room_btn.addEventListener('mousedown', function () {
    stopRoomba();

    if (!is_working) {
        map_room_btn.disabled = true;
        go_to_btn.disabled = true;
        clean_room_btn.innerHTML = "STOP";
        forward_btn.disabled = true;
        left_btn.disabled = true;
        right_btn.disabled = true;
        backward_btn.disabled = true;
        reset_speed_btn.disabled = true;

        slider.value = 0;
        linear_vel = 0;
        speedometer.innerHTML = linear_vel;
    } else {
        map_room_btn.disabled = false;
        go_to_btn.disabled = false;
        clean_room_btn.innerHTML = "CLEAN ROOM";
        forward_btn.disabled = false;
        left_btn.disabled = false;
        right_btn.disabled = false;
        backward_btn.disabled = false;
        reset_speed_btn.disabled = false;
    }

    is_working = !is_working;
});

forward_btn.addEventListener('mousedown', function () {
    linear_vel = 1;
});

forward_btn.addEventListener('touchstart', function () {
    linear_vel = 1;
});

forward_btn.addEventListener('mouseup', function () {
    linear_vel = 0;
});

forward_btn.addEventListener('touchend', function () {
    linear_vel = 0;
});

left_btn.addEventListener('mousedown', function () {
    angular_vel = 1;
    if (slider.value == 0) {
        linear_vel = 0;
    }
});

left_btn.addEventListener('touchstart', function () {
    angular_vel = 1;
    if (slider.value == 0) {
        linear_vel = 0;
    }
});

left_btn.addEventListener('mouseup', function () {
    angular_vel = 0;
});

left_btn.addEventListener('touchend', function () {
    angular_vel = 0;
});

right_btn.addEventListener('mousedown', function () {
    angular_vel = -1;
    if (slider.value == 0) {
        linear_vel = 0;
    }
});

right_btn.addEventListener('touchstart', function () {
    angular_vel = -1;
    if (slider.value == 0) {
        linear_vel = 0;
    }
});

right_btn.addEventListener('mouseup', function () {
    angular_vel = 0;
});

right_btn.addEventListener('touchend', function () {
    angular_vel = 0;
});

backward_btn.addEventListener('mousedown', function () {
    linear_vel = -1;
});

backward_btn.addEventListener('touchstart', function () {
    linear_vel = -1;
});

backward_btn.addEventListener('mouseup', function () {
    linear_vel = 0;
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