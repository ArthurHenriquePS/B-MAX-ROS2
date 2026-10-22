// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from baymax_interfaces:msg/Emotion.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "baymax_interfaces/msg/emotion.hpp"


#ifndef BAYMAX_INTERFACES__MSG__DETAIL__EMOTION__TRAITS_HPP_
#define BAYMAX_INTERFACES__MSG__DETAIL__EMOTION__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "baymax_interfaces/msg/detail/emotion__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__traits.hpp"

namespace baymax_interfaces
{

namespace msg
{

inline void to_flow_style_yaml(
  const Emotion & msg,
  std::ostream & out)
{
  out << "{";
  // member: label
  {
    out << "label: ";
    rosidl_generator_traits::value_to_yaml(msg.label, out);
    out << ", ";
  }

  // member: confidence
  {
    out << "confidence: ";
    rosidl_generator_traits::value_to_yaml(msg.confidence, out);
    out << ", ";
  }

  // member: stamp
  {
    out << "stamp: ";
    to_flow_style_yaml(msg.stamp, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Emotion & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: label
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "label: ";
    rosidl_generator_traits::value_to_yaml(msg.label, out);
    out << "\n";
  }

  // member: confidence
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "confidence: ";
    rosidl_generator_traits::value_to_yaml(msg.confidence, out);
    out << "\n";
  }

  // member: stamp
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "stamp:\n";
    to_block_style_yaml(msg.stamp, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Emotion & msg, bool use_flow_style = false)
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
  const baymax_interfaces::msg::Emotion & msg,
  std::ostream & out, size_t indentation = 0)
{
  baymax_interfaces::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use baymax_interfaces::msg::to_yaml() instead")]]
inline std::string to_yaml(const baymax_interfaces::msg::Emotion & msg)
{
  return baymax_interfaces::msg::to_yaml(msg);
}

template<>
inline const char * data_type<baymax_interfaces::msg::Emotion>()
{
  return "baymax_interfaces::msg::Emotion";
}

template<>
inline const char * name<baymax_interfaces::msg::Emotion>()
{
  return "baymax_interfaces/msg/Emotion";
}

template<>
struct has_fixed_size<baymax_interfaces::msg::Emotion>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<baymax_interfaces::msg::Emotion>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<baymax_interfaces::msg::Emotion>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // BAYMAX_INTERFACES__MSG__DETAIL__EMOTION__TRAITS_HPP_
