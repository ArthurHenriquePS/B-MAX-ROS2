// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from baymax_interfaces:msg/MotionCommand.idl
// generated code does not contain a copyright notice

#include "baymax_interfaces/msg/detail/motion_command__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
const rosidl_type_hash_t *
baymax_interfaces__msg__MotionCommand__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x3d, 0xba, 0xea, 0xe5, 0xf3, 0xcd, 0xc8, 0xa8,
      0x20, 0xf1, 0x48, 0xf0, 0xcb, 0x2c, 0xbd, 0xec,
      0x75, 0x1b, 0xb6, 0x8f, 0x7f, 0xcd, 0x6d, 0x2e,
      0xb9, 0x5c, 0x5b, 0x13, 0xb9, 0xba, 0x09, 0x10,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char baymax_interfaces__msg__MotionCommand__TYPE_NAME[] = "baymax_interfaces/msg/MotionCommand";

// Define type names, field names, and default values
static char baymax_interfaces__msg__MotionCommand__FIELD_NAME__target[] = "target";
static char baymax_interfaces__msg__MotionCommand__FIELD_NAME__kind[] = "kind";
static char baymax_interfaces__msg__MotionCommand__FIELD_NAME__value[] = "value";
static char baymax_interfaces__msg__MotionCommand__FIELD_NAME__name[] = "name";

static rosidl_runtime_c__type_description__Field baymax_interfaces__msg__MotionCommand__FIELDS[] = {
  {
    {baymax_interfaces__msg__MotionCommand__FIELD_NAME__target, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {baymax_interfaces__msg__MotionCommand__FIELD_NAME__kind, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {baymax_interfaces__msg__MotionCommand__FIELD_NAME__value, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT32,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {baymax_interfaces__msg__MotionCommand__FIELD_NAME__name, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
baymax_interfaces__msg__MotionCommand__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {baymax_interfaces__msg__MotionCommand__TYPE_NAME, 35, 35},
      {baymax_interfaces__msg__MotionCommand__FIELDS, 4, 4},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# Comando de movimento emitido pelo brain_node para o motor_bridge_node.\n"
  "#\n"
  "# target: qual MCU/subsistema. Um de {head, arm}\n"
  "string target\n"
  "\n"
  "# kind: tipo de comando. Um de {pan, tilt, head_gesture,\n"
  "#       shoulder, elbow, grip, arm_gesture, stop, torque}\n"
  "string kind\n"
  "\n"
  "# value: \\xc3\\xa2ngulo (0-180), garra (0-100) ou torque (0/1), quando num\\xc3\\xa9rico\n"
  "int32 value\n"
  "\n"
  "# name: nome do gesto (ex. \"nod\", \"wave\"), quando kind for *_gesture\n"
  "string name";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
baymax_interfaces__msg__MotionCommand__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {baymax_interfaces__msg__MotionCommand__TYPE_NAME, 35, 35},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 433, 433},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
baymax_interfaces__msg__MotionCommand__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *baymax_interfaces__msg__MotionCommand__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
