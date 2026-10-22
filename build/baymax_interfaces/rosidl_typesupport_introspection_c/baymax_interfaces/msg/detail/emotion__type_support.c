// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from baymax_interfaces:msg/Emotion.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "baymax_interfaces/msg/detail/emotion__rosidl_typesupport_introspection_c.h"
#include "baymax_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "baymax_interfaces/msg/detail/emotion__functions.h"
#include "baymax_interfaces/msg/detail/emotion__struct.h"


// Include directives for member types
// Member `label`
#include "rosidl_runtime_c/string_functions.h"
// Member `stamp`
#include "builtin_interfaces/msg/time.h"
// Member `stamp`
#include "builtin_interfaces/msg/detail/time__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void baymax_interfaces__msg__Emotion__rosidl_typesupport_introspection_c__Emotion_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  baymax_interfaces__msg__Emotion__init(message_memory);
}

void baymax_interfaces__msg__Emotion__rosidl_typesupport_introspection_c__Emotion_fini_function(void * message_memory)
{
  baymax_interfaces__msg__Emotion__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember baymax_interfaces__msg__Emotion__rosidl_typesupport_introspection_c__Emotion_message_member_array[3] = {
  {
    "label",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(baymax_interfaces__msg__Emotion, label),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "confidence",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(baymax_interfaces__msg__Emotion, confidence),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "stamp",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(baymax_interfaces__msg__Emotion, stamp),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers baymax_interfaces__msg__Emotion__rosidl_typesupport_introspection_c__Emotion_message_members = {
  "baymax_interfaces__msg",  // message namespace
  "Emotion",  // message name
  3,  // number of fields
  sizeof(baymax_interfaces__msg__Emotion),
  false,  // has_any_key_member_
  baymax_interfaces__msg__Emotion__rosidl_typesupport_introspection_c__Emotion_message_member_array,  // message members
  baymax_interfaces__msg__Emotion__rosidl_typesupport_introspection_c__Emotion_init_function,  // function to initialize message memory (memory has to be allocated)
  baymax_interfaces__msg__Emotion__rosidl_typesupport_introspection_c__Emotion_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t baymax_interfaces__msg__Emotion__rosidl_typesupport_introspection_c__Emotion_message_type_support_handle = {
  0,
  &baymax_interfaces__msg__Emotion__rosidl_typesupport_introspection_c__Emotion_message_members,
  get_message_typesupport_handle_function,
  &baymax_interfaces__msg__Emotion__get_type_hash,
  &baymax_interfaces__msg__Emotion__get_type_description,
  &baymax_interfaces__msg__Emotion__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_baymax_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, baymax_interfaces, msg, Emotion)() {
  baymax_interfaces__msg__Emotion__rosidl_typesupport_introspection_c__Emotion_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, builtin_interfaces, msg, Time)();
  if (!baymax_interfaces__msg__Emotion__rosidl_typesupport_introspection_c__Emotion_message_type_support_handle.typesupport_identifier) {
    baymax_interfaces__msg__Emotion__rosidl_typesupport_introspection_c__Emotion_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &baymax_interfaces__msg__Emotion__rosidl_typesupport_introspection_c__Emotion_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
