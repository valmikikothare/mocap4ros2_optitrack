// Copyright 2021 Institute for Robotics and Intelligent Machines,
//                Georgia Institute of Technology
// Copyright 2024 Intelligent Robotics Lab
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: Christian Llanes <christian.llanes@gatech.edu>
// Author: David Vargas Frutos <david.vargas@urjc.es>
// Author: Francisco Martín <fmrico@urjc.es>

#ifndef MOCAP4R2_OPTITRACK_DRIVER__MOCAP4R2_OPTITRACK_DRIVER_HPP_
#define MOCAP4R2_OPTITRACK_DRIVER__MOCAP4R2_OPTITRACK_DRIVER_HPP_

#include <NatNetCAPI.h>
#include <NatNetClient.h>
#include <NatNetTypes.h>

#include <chrono>
#include <memory>
#include <string>

#include "mocap4r2_control/ControlledLifecycleNode.hpp"
#include "mocap4r2_msgs/msg/markers.hpp"
#include "mocap4r2_msgs/msg/rigid_bodies.hpp"

namespace mocap4r2_optitrack_driver
{

class OptitrackDriverNode : public mocap4r2_control::ControlledLifecycleNode
{
public:
  OptitrackDriverNode();
  ~OptitrackDriverNode();

  using CallbackReturnT = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

  CallbackReturnT on_configure(const rclcpp_lifecycle::State& state) override;
  CallbackReturnT on_activate(const rclcpp_lifecycle::State& state) override;
  CallbackReturnT on_deactivate(const rclcpp_lifecycle::State& state) override;
  CallbackReturnT on_cleanup(const rclcpp_lifecycle::State& state) override;
  CallbackReturnT on_shutdown(const rclcpp_lifecycle::State& state) override;
  CallbackReturnT on_error(const rclcpp_lifecycle::State& state) override;

  bool connect_optitrack();
  bool disconnect_optitrack();
  void set_settings_optitrack();
  void initParameters();

  void process_frame(sFrameOfMocapData* data);

protected:
  void control_start(const mocap4r2_control_msgs::msg::Control::SharedPtr msg) override;
  void control_stop(const mocap4r2_control_msgs::msg::Control::SharedPtr msg) override;

  std::unique_ptr<NatNetClient> client;

  std::chrono::nanoseconds get_optitrack_latency(sFrameOfMocapData* data);

  sNatNetClientConnectParams client_params;
  sServerDescription server_description;
  sDataDescriptions* data_descriptions{ nullptr };
  sFrameOfMocapData latest_data;
  sRigidBodyData latest_body_frame_data;

  rclcpp_lifecycle::LifecyclePublisher<mocap4r2_msgs::msg::Markers>::SharedPtr mocap4r2_markers_pub_;
  rclcpp_lifecycle::LifecyclePublisher<mocap4r2_msgs::msg::RigidBodies>::SharedPtr mocap4r2_rigid_body_pub_;

  std::string connection_type_;
  std::string server_address_;
  std::string local_address_;
  std::string multicast_address_;
  std::string frame_id_;
  std::string marker_topic_;
  std::string rb_topic_;
  uint16_t server_command_port_;
  uint16_t server_data_port_;
  double large_latency_threshold_;

  uint32_t frame_number_{ 0 };
};

void NATNET_CALLCONV process_frame_callback(sFrameOfMocapData* data, void* pUserData);

}  // namespace mocap4r2_optitrack_driver

#endif  // MOCAP4R2_OPTITRACK_DRIVER__MOCAP4R2_OPTITRACK_DRIVER_HPP_
