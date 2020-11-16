# 🤖 Line follow simulator for the mBot Ranger
## Description
The [mBot ranger](https://www.makeblock.com/steam-kits/mbot-ranger) has a built in line follower that detects black lines on white backgrounds and white lines on black backgrounds. This plugin simulates that sensor with a downward facing camera that has 4 states: 0, 1, 2, and 3. 0 represents no line being found, 1 represents a line on the left side of the sensor, 2 represents a line on the right side of the sensor, and 3 represents a line on both sides of the sensor. This is meant to mimic the mBot's line follower which uses two downward cameras and has the same states.
## Disclaimer
I don't think this is a perfect replica of the mBot ranger's sensors since this only uses one camera. A more accurate simulation would have two cameras that only know if they are on a line or not. The states would be determined in the control plugin and would (I think) have to override the built in camera sensor that Gazebo gives us. I think that the current sensor plugin is sufficient to simulate the mBot's sensor, but it would definitely be improved by having two cameras.
## Dependencies
1. Make sure you have [opencv2](https://opencv.org/opencv-2-4-8/)
2. This plugin expands upon the [microphone plugin](https://github.com/nbock/cs5335-nm/tree/plugins/plugins/microphone_control), so all of its dependencies are required for this, as well.

## Set up
0. The easiest way (and therefore the recommended set up process) is to clone [this repo](https://github.com/nbock/cs5335-nm/tree/linefollow), make, and execute world.sh.
1. Make a world. This plugin has an [example world](https://github.com/nbock/cs5335-nm/blob/linefollow/worlds/line.world) using the microphone and a tankbot. There is also a [startup script](https://github.com/nbock/cs5335-nm/blob/linefollow/world.sh) for that world.\
    a. If you want to use the example world, it's probably easiest to clone the entire repo and make from the parent directory. Then you can call `./world.sh` and the world will load.

2. If you chose to make your own world, load your models for a car or tankbot or whatever vehicle into the world.

3. Now, attach the camera model to the tankbot.\
    a. To do this, you'll need to include the camera sensor model in the tankbot model.\
    b. The camera model is very simple because it's a built in plugin, but will need to be added to your models directory. Add the entire camera_sensor directory.\
    b. An example of this is done in the example world\
    c. You should add the following lines into your vehicle model (not the world model):
    ```xml
    <include>
      <uri>model://camera_sensor</uri>
      <pose>
        0.2 0.0 0.04
        0.0 1.57 0.0
      </pose>
    </include>

    <joint name="camera_joint" type="fixed">
      <child>camera_sensor::link</child>
      <parent>chassis</parent>
    </joint>
    ```
    **NOTE**: you'll want to edit the pose of your camera_sensor to sit just below your vehicle model. The example pose is for the tankbot in world.sh.\
    **NOTE**: The naming convention for your models matters and will influence what the topic names you're using are. Mimic the examples as close as possible.

4. Make sure you followed the second note above, it's super important (this is just a reminder)
5. Edit your Makefile. Your Makefile should mimic the one included in [plugins](../).\
    a. You need to create a .so for your plugin. Check out the example Makefile for guidance on this.\
    b. You'll also need a Makefile for your parent directory, which can mimic the example Makefile in the parent directorys.

6. If you've done all this, you should be ready to initialize your world and see the camera plugin (since it's facing down, the preview is underground, but you can still zoom underground to see it).

## Setting up lines
1. Easiest to just do this from the example world
2. If you don't want to use the example world, insert a block, color it your preferred color and put it in the robots path.


## Subscribing from robot.cc (or something like it)
0. Check out robot.cc and robot.cc in [brain](https://github.com/nbock/cs5335-nm/tree/linefollow/brain) for examples on how this is done.\
    a. [brain.cc](https://github.com/nbock/cs5335-nm/blob/plugins/brain/brain.cc) also has a reference to `robot->noise`
1. Add a `cv::Mat frame` value to robot.hh or your equivalent
2. Add a SubscriberPtr as a private variable to robot.hh or equivalent
3. Use your SubscriberPtr to subscribe to the camera topic `~/tankbot0/tankbot/camera_sensor/link/camera/image`:
```cpp
frame_sub = node->Subscribe(
    string("~/tankbot0/tankbot/camera_sensor/link/camera/image"),
    &Robot::on_frame,
    this,
    false
);
```
4. Now, `Robot->on_frame` will be called on each message that is published to the frame topic
5. Store the frame data from your on_frame function
```cpp
void
Robot::on_frame(ConstImageStampedPtr &msg)
{
    msgs::Image image = msg->image();

    char* data = (char*)malloc(image.data().size());
    memcpy(data, image.data().c_str(), image.data().size());
    cv::Mat temp(image.height(), image.width(), CV_8UC3, data);
    cv::Mat temp2 = temp.clone();
    cv::cvtColor(temp, temp2, cv::COLOR_RGBA2BGRA);
    this->frame = temp2.clone();
    assert(this->frame.size().height > 0);
    free(data);

    this->on_update(this);
}
```
6. Now you can reference the frame from `Robot->frame`

## Running it
1. If you pull the entire cs5335-nm branch called "linefollow" you can make and `./world.sh` will start the world
2. Then executing `./brain` will move the bot in a boring way, but you'll see line status change as you go over black lines

## Implementation details
1. The frame is a 2x2 grid.
2. What type of lines your looking for is controlled in brain.cc in `get_line_status(Robot* robot)`\
    a. By default, it looks for dark lines (r,g,b) < (100,100,100)\
    b. If you want to change that, do it in `get_line_status`
3. Line states are as follows:\
    0: no line found\
    1: line on the left side of the frame (left side of the bot, left two pixels in the 2x2 grid that is the frame)\
    2: line on the right side of the frame\
    3: line on both sides of the frame\
4. If you aren't sure what topic your mic is on, use the following to list all topics:
```terminal
gz topic -l
```
5. The frame publishes colors in quadruples (I believe) that all end in 255. So true black is [0,0,0,255]. As such, the plugin gets a quadruple to represent each pixel of the frame: `cv::Vec4b left_front = robot->frame.at<Vec4b>(0, 0);`. In this case, `left_front[3]` is ignored as it is treated as a delimeter.

## Attributions
1. Lots of the pixel processing code was expanded upon from Professor Tuck's [scratch code](https://github.com/NatTuck/scratch-2020-09/tree/master/5335/11/hw08). 
