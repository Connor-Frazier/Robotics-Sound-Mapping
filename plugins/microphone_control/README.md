# 🤖 Microphone Plugin for mBot Ranger.
## Description
The [mBot ranger](https://www.makeblock.com/steam-kits/mbot-ranger) has a built in microphone that measures sound from the environment. This plugin mimics that microphone and is intended to be included in a model for the mBot ranger.
## Steps to use
1. Make a world. This plugin has an [example world](../../worlds/project.world) using the microphone and a tankbot. There is also a [startup script](../../world.sh) for that world.\
    a. If you want to use the example world, it's probably easiest to clone the entire repo and make from the parent directory. Then you can call ./world.sh and the world will load.

2. If you chose to make your own world, load your models for a car or tankbot or whatever vehicle into the world.

3. Now, attach the microphone model to the tankbot.\
    a. To do this, you'll need to include the microphone sensor in the tankbot model.\
    b. An example of this is done in the [example world](../../world/project.world), but you can also just add the following lines into your vehicle model:
    ```xml
    <include>
      <uri>model://microphone_sensor</uri>
      <pose>
        0.0 0.0 0.23
        0.0 0.0 0.0
      </pose>
    </include>

    <joint name="microphone_joint" type="fixed">
      <child>microphone_sensor::link</child>
      <parent>chassis</parent>
    </joint>
    ```
    **NOTE**: you'll want to edit the pose of your microphone_sensor to sit atop your vehicle model. The example pose is for the tankbot in world.sh.
