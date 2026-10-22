// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from baymax_interfaces:msg/Emotion.idl
// generated code does not contain a copyright notice

#include "baymax_interfaces/msg/detail/emotion__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
const rosidl_type_hash_t *
baymax_interfaces__msg__Emotion__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x4e, 0xb2, 0xc4, 0xd0, 0x84, 0x54, 0xf0, 0x3a,
      0xd8, 0xbe, 0xe6, 0x44, 0x22, 0x85, 0x38, 0x21,
      0x3a, 0x54, 0x44, 0x16, 0x80, 0x99, 0x8d, 0x44,
      0xda, 0x47, 0xa0, 0x17, 0xc3, 0xdb, 0x47, 0xee,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "builtin_interfaces/msg/detail/time__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
#endif

static char baymax_interfaces__msg__Emotion__TYPE_NAME[] = "baymax_interfaces/msg/Emotion";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";

// Define type names, field names, and default values
static char baymax_interfaces__msg__Emotion__FIELD_NAME__label[] = "label";
static char baymax_interfaces__msg__Emotion__FIELD_NAME__confidence[] = "confidence";
static char baymax_interfaces__msg__Emotion__FIELD_NAME__stamp[] = "stamp";

static rosidl_runtime_c__type_description__Field baymax_interfaces__msg__Emotion__FIELDS[] = {
  {
    {baymax_interfaces__msg__Emotion__FIELD_NAME__label, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_STRING,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {baymax_interfaces__msg__Emotion__FIELD_NAME__confidence, 10, 10},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {baymax_interfaces__msg__Emotion__FIELD_NAME__stamp, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription baymax_interfaces__msg__Emotion__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
baymax_interfaces__msg__Emotion__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {baymax_interfaces__msg__Emotion__TYPE_NAME, 29, 29},
      {baymax_interfaces__msg__Emotion__FIELDS, 3, 3},
    },
    {baymax_interfaces__msg__Emotion__REFERENCED_TYPE_DESCRIPTIONS, 1, 1},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# Express\\xc3\\xa3o/emo\\xc3\\xa7\\xc3\\xa3o detectada pelo vision_node.\n"
  "# label: uma de {neutral, happy, sad, angry, surprised, fearful, disgusted}\n"
  "string label\n"
  "# confidence: 0.0 a 1.0\n"
  "float32 confidence\n"
  "# timestamp da captura\n"
  "builtin_interfaces/Time stamp";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
baymax_interfaces__msg__Emotion__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {baymax_interfaces__msg__Emotion__TYPE_NAME, 29, 29},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 232, 232},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
baymax_interfaces__msg__Emotion__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[2];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 2, 2};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *baymax_interfaces__msg__Emotion__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
