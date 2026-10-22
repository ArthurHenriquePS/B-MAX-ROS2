// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from baymax_interfaces:msg/MotionCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "baymax_interfaces/msg/motion_command.hpp"


#ifndef BAYMAX_INTERFACES__MSG__DETAIL__MOTION_COMMAND__BUILDER_HPP_
#define BAYMAX_INTERFACES__MSG__DETAIL__MOTION_COMMAND__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "baymax_interfaces/msg/detail/motion_command__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace baymax_interfaces
{

namespace msg
{

namespace builder
{

class Init_MotionCommand_name
{
public:
  explicit Init_MotionCommand_name(::baymax_interfaces::msg::MotionCommand & msg)
  : msg_(msg)
  {}
  ::baymax_interfaces::msg::MotionCommand name(::baymax_interfaces::msg::MotionCommand::_name_type arg)
  {
    msg_.name = std::move(arg);
    return std::move(msg_);
  }

private:
  ::baymax_interfaces::msg::MotionCommand msg_;
};

class Init_MotionCommand_value
{
public:
  explicit Init_MotionCommand_value(::baymax_interfaces::msg::MotionCommand & msg)
  : msg_(msg)
  {}
  Init_MotionCommand_name value(::baymax_interfaces::msg::MotionCommand::_value_type arg)
  {
    msg_.value = std::move(arg);
    return Init_MotionCommand_name(msg_);
  }

private:
  ::baymax_interfaces::msg::MotionCommand msg_;
};

class Init_MotionCommand_kind
{
public:
  explicit Init_MotionCommand_kind(::baymax_interfaces::msg::MotionCommand & msg)
  : msg_(msg)
  {}
  Init_MotionCommand_value kind(::baymax_interfaces::msg::MotionCommand::_kind_type arg)
  {
    msg_.kind = std::move(arg);
    return Init_MotionCommand_value(msg_);
  }

private:
  ::baymax_interfaces::msg::MotionCommand msg_;
};

class Init_MotionCommand_target
{
public:
  Init_MotionCommand_target()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MotionCommand_kind target(::baymax_interfaces::msg::MotionCommand::_target_type arg)
  {
    msg_.target = std::move(arg);
    return Init_MotionCommand_kind(msg_);
  }

private:
  ::baymax_interfaces::msg::MotionCommand msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::baymax_interfaces::msg::MotionCommand>()
{
  return baymax_interfaces::msg::builder::Init_MotionCommand_target();
}

}  // namespace baymax_interfaces

#endif  // BAYMAX_INTERFACES__MSG__DETAIL__MOTION_COMMAND__BUILDER_HPP_
