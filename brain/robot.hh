#ifndef ROBOT_HH
#define ROBOT_HH

#include <gazebo/gazebo_config.h>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo_client.hh>

#include <opencv2/core/mat.hpp>

class Robot {
  public:
    virtual ~Robot() = 0;
    virtual int get_line_status() = 0;
    virtual double get_noise_sensor() = 0;
    virtual double get_robot_theta() = 0;
    virtual void set_vel(double lvel, double rvel) = 0;
    virtual void do_stuff() = 0;
    virtual double get_robot_x() = 0;
    virtual double get_robot_y() = 0;

	


};

class GzRobot : public Robot {
  public:
    void (*on_update)(Robot*);

    float pos_x;
    float pos_y;
    float range;
    float pos_t;
    int noise;
    int line_status;
    cv::Mat frame;

    GzRobot(int argc, char* argv[], void (*cb)(Robot*));
    ~GzRobot();

    bool at_goal();
    void done();

    int get_line_status();
    double get_noise_sensor();
    double get_robot_theta();
    double get_robot_y();
    double get_robot_x(); 
    void set_vel(double lvel, double rvel);
    void do_stuff();

    void on_frame(ConstImageStampedPtr &msg);
    void on_scan(ConstSonarStampedPtr &msg);
    void on_sound(ConstIntPtr &msg);
    void on_pose(ConstPoseStampedPtr &msg);

  private:
    bool task_done;

    gazebo::transport::NodePtr node;
    gazebo::transport::PublisherPtr vel_pub;
    gazebo::transport::SubscriberPtr scan_sub;
    gazebo::transport::SubscriberPtr mic_sub;
    gazebo::transport::SubscriberPtr pose_sub;
    gazebo::transport::SubscriberPtr frame_sub;
};

class RgRobot : public Robot {
    public:
      RgRobot(int argc, char* argb[], void (*cb)(Robot*));
      ~RgRobot();

      int get_line_status();
      double get_noise_sensor();
      void set_vel(double lvel, double rvel);
      double get_robot_theta();
      double get_robot_y();
      double get_robot_x();
      void do_stuff();

      void (*on_update)(Robot*);
};

static
double
clamp(double xmin, double xx, double xmax)
{
    if (xx < xmin) return xmin;
    return xx;
}

#endif
