// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from baymax_interfaces:msg/MotionCommand.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "baymax_interfaces/msg/motion_command.hpp"


#ifndef BAYMAX_INTERFACES__MSG__DETAIL__MOTION_COMMAND__TRAITS_HPP_
#define BAYMAX_INTERFACES__MSG__DETAIL__MOTION_COMMAND__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "baymax_interfaces/msg/detail/motion_command__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace baymax_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const MotionCommand & msg,
  std::ostream & out)
{
  out << "{";
  // member: target
  {
    out << "target: ";
    rosidl_generator_traits::value_to_yaml(msg.target, out);
    out << ", ";
  }

  // member: kind
  {
    out << "kind: ";
    rosidl_generator_traits::value_to_yaml(msg.kind, out);
    out << ", ";
  }

  // member: value
  {
    out << "value: ";
    rosidl_generator_traits::value_to_yaml(msg.value, out);
    out << ", ";
  }

  // member: name
  {
    out << "name: ";
    rosidl_generator_traits::value_to_yaml(msg.name, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MotionCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: target
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "target: ";
    rosidl_generator_traits::value_to_yaml(msg.target, out);
    out << "\n";
  }

  // member: kind
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "kind: ";
    rosidl_generator_traits::value_to_yaml(msg.kind, out);
    out << "\n";
  }

  // member: value
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "value: ";
    rosidl_generator_traits::value_to_yaml(msg.value, out);
    out << "\n";
  }

  // member: name
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "name: ";
    rosidl_generator_traits::value_to_yaml(msg.name, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MotionCommand & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace baymax_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use baymax_interfaces::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const baymax_interfaces::msg::MotionCommand & msg,
  std::ostream & out, size_t indentation = 0)
{
  baymax_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use baymax_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const baymax_interfaces::msg::MotionCommand & msg)
{
  return baymax_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<baymax_interfaces::msg::MotionCommand>()
{
  return "baymax_interfaces::msg::MotionCommand";
}

template<>
inline const char * name<baymax_interfaces::msg::MotionCommand>()
{
  return "baymax_interfaces/msg/MotionCommand";
}

template<>
struct has_fixed_size<baymax_interfaces::msg::MotionCommand>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<baymax_interfaces::msg::MotionCommand>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<baymax_interfaces::msg::MotionCommand>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // BAYMAX_INTERFACES__MSG__DETAIL__MOTION_COMMAND__TRAITS_HPP_
