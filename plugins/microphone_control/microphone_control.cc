
#include <iostream>

#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>

using std::string;
using std::vector;
using namespace gazebo;
using physics::ModelPtr;
using physics::JointControllerPtr;
using ignition::math::Pose3d;
using common::Time;

int SOURCE_X = 10;
int SOURCE_Y = 10;
int SOURCE_DECIBALS = 100;

class MicrophoneControlPlugin : public ModelPlugin
{
public:
    physics::ModelPtr model;

    transport::NodePtr node;

    transport::SubscriberPtr vel_sub;
    transport::SubscriberPtr stat_sub;

    transport::PublisherPtr  mic_pub;

    MicrophoneControlPlugin() {}

    virtual void
    Load(physics::ModelPtr model, sdf::ElementPtr sdf)
    {
        this->model = model;

        auto model_name = this->model->GetName();
        auto world_name = this->model->GetWorld()->Name();

        std::cerr << "hello from MicControlPlugin" << std::endl;
        std::cerr << "world: " << world_name << std::endl;
        std::cerr << "model: " << model_name << std::endl;

        //this->node->Init(world_name);

        // TODO: Figure out a position to go towards


        this->node = transport::NodePtr(new transport::Node());
        this->node->Init(world_name);

        string stats_topic = "~/world_stats";
        this->stat_sub = this->node->Subscribe(
            stats_topic, &MicrophoneControlPlugin::OnStats, this);
        std::cerr << "Subscribed world_stats: "
                  << this->stat_sub->GetTopic() << std::endl;

        string mic_topic = "~/" + model_name + "/mic";
        this->mic_pub = this->node->Advertise<gazebo::msgs::Int>(mic_topic);
        std::cout << "Advertised microphone" << std::endl;

        std::cout << "microphone control loaded" << std::endl;
    }

    double distanceCalculate(double x1, double y1, double x2, double y2)
    {
    	double x = x1 - x2; //calculating number to square in next step
    	double y = y1 - y2;
    	double dist;

    	dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
    	dist = sqrt(dist);

    	return dist;
    }

    int
    make_mic_msg(int x, int y)
    {
        double dist = distanceCalculate(x, y, SOURCE_X, SOURCE_Y);
        int dist_int = round(dist);
        int decibals = SOURCE_DECIBALS - dist_int;

        return decibals;
    }

    void
    OnStats(ConstAnyPtr &_msg)
    {
        //std::cout << "OnStats for mic called" << std::endl;
        auto pose = this->model->WorldPose();
        int distance = make_mic_msg(pose.X(), pose.Y());
        gazebo::msgs::Int msg;
        msg.set_data(distance);
        this->mic_pub->Publish(msg);
    }
};

GZ_REGISTER_MODEL_PLUGIN(MicrophoneControlPlugin)
