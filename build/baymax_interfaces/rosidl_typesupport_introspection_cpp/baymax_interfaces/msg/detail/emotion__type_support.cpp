// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from baymax_interfaces:msg/Emotion.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "baymax_interfaces/msg/detail/emotion__functions.h"
#include "baymax_interfaces/msg/detail/emotion__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace baymax_interfaces
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void Emotion_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) baymax_interfaces::msg::Emotion(_init);
}

void Emotion_fini_function(void * message_memory)
{
  auto typed_message = static_cast<baymax_interfaces::msg::Emotion *>(message_memory);
  typed_message->~Emotion();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember Emotion_message_member_array[3] = {
  {
    "label",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(baymax_interfaces::msg::Emotion, label),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "confidence",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(baymax_interfaces::msg::Emotion, confidence),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "stamp",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<builtin_interfaces::msg::Time>(),  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(baymax_interfaces::msg::Emotion, stamp),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers Emotion_message_members = {
  "baymax_interfaces::msg",  // message namespace
  "Emotion",  // message name
  3,  // number of fields
  sizeof(baymax_interfaces::msg::Emotion),
  false,  // has_any_key_member_
  Emotion_message_member_array,  // message members
  Emotion_init_function,  // function to initialize message memory (memory has to be allocated)
  Emotion_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t Emotion_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &Emotion_message_members,
  get_message_typesupport_handle_function,
  &baymax_interfaces__msg__Emotion__get_type_hash,
  &baymax_interfaces__msg__Emotion__get_type_description,
  &baymax_interfaces__msg__Emotion__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace baymax_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<baymax_interfaces::msg::Emotion>()
{
  return &::baymax_interfaces::msg::rosidl_typesupport_introspection_cpp::Emotion_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, baymax_interfaces, msg, Emotion)() {
  return &::baymax_interfaces::msg::rosidl_typesupport_introspection_cpp::Emotion_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
