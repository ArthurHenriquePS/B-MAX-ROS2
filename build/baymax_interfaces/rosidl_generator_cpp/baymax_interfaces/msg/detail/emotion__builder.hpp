// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from baymax_interfaces:msg/Emotion.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "baymax_interfaces/msg/emotion.hpp"


#ifndef BAYMAX_INTERFACES__MSG__DETAIL__EMOTION__BUILDER_HPP_
#define BAYMAX_INTERFACES__MSG__DETAIL__EMOTION__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "baymax_interfaces/msg/detail/emotion__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace baymax_interfaces
{

namespace msg
{

namespace builder
{

class Init_Emotion_stamp
{
public:
  explicit Init_Emotion_stamp(::baymax_interfaces::msg::Emotion & msg)
  : msg_(msg)
  {}
  ::baymax_interfaces::msg::Emotion stamp(::baymax_interfaces::msg::Emotion::_stamp_type arg)
  {
    msg_.stamp = std::move(arg);
    return std::move(msg_);
  }

private:
  ::baymax_interfaces::msg::Emotion msg_;
};

class Init_Emotion_confidence
{
public:
  explicit Init_Emotion_confidence(::baymax_interfaces::msg::Emotion & msg)
  : msg_(msg)
  {}
  Init_Emotion_stamp confidence(::baymax_interfaces::msg::Emotion::_confidence_type arg)
  {
    msg_.confidence = std::move(arg);
    return Init_Emotion_stamp(msg_);
  }

private:
  ::baymax_interfaces::msg::Emotion msg_;
};

class Init_Emotion_label
{
public:
  Init_Emotion_label()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Emotion_confidence label(::baymax_interfaces::msg::Emotion::_label_type arg)
  {
    msg_.label = std::move(arg);
    return Init_Emotion_confidence(msg_);
  }

private:
  ::baymax_interfaces::msg::Emotion msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::baymax_interfaces::msg::Emotion>()
{
  return baymax_interfaces::msg::builder::Init_Emotion_label();
}

}  // namespace baymax_interfaces

#endif  // BAYMAX_INTERFACES__MSG__DETAIL__EMOTION__BUILDER_HPP_
