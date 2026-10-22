// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from baymax_interfaces:msg/MotionCommand.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "baymax_interfaces/msg/detail/motion_command__functions.h"
#include "baymax_interfaces/msg/detail/motion_command__struct.hpp"
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

void MotionCommand_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) baymax_interfaces::msg::MotionCommand(_init);
}

void MotionCommand_fini_function(void * message_memory)
{
  auto typed_message = static_cast<baymax_interfaces::msg::MotionCommand *>(message_memory);
  typed_message->~MotionCommand();
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember MotionCommand_message_member_array[4] = {
  {
    "target",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(baymax_interfaces::msg::MotionCommand, target),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "kind",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(baymax_interfaces::msg::MotionCommand, kind),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "value",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(baymax_interfaces::msg::MotionCommand, value),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "name",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(baymax_interfaces::msg::MotionCommand, name),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers MotionCommand_message_members = {
  "baymax_interfaces::msg",  // message namespace
  "MotionCommand",  // message name
  4,  // number of fields
  sizeof(baymax_interfaces::msg::MotionCommand),
  false,  // has_any_key_member_
  MotionCommand_message_member_array,  // message members
  MotionCommand_init_function,  // function to initialize message memory (memory has to be allocated)
  MotionCommand_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t MotionCommand_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &MotionCommand_message_members,
  get_message_typesupport_handle_function,
  &baymax_interfaces__msg__MotionCommand__get_type_hash,
  &baymax_interfaces__msg__MotionCommand__get_type_description,
  &baymax_interfaces__msg__MotionCommand__get_type_description_sources,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace baymax_interfaces


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<baymax_interfaces::msg::MotionCommand>()
{
  return &::baymax_interfaces::msg::rosidl_typesupport_introspection_cpp::MotionCommand_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, baymax_interfaces, msg, MotionCommand)() {
  return &::baymax_interfaces::msg::rosidl_typesupport_introspection_cpp::MotionCommand_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
