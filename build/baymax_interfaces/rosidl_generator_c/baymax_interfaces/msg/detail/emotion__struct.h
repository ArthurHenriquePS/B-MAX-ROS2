// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from baymax_interfaces:msg/Emotion.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "baymax_interfaces/msg/emotion.h"


#ifndef BAYMAX_INTERFACES__MSG__DETAIL__EMOTION__STRUCT_H_
#define BAYMAX_INTERFACES__MSG__DETAIL__EMOTION__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Constants defined in the message

// Include directives for member types
// Member 'label'
#include "rosidl_runtime_c/string.h"
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__struct.h"

/// Struct defined in msg/Emotion in the package baymax_interfaces.
/**
  * Expressão/emoção detectada pelo vision_node.
  * label: uma de {neutral, happy, sad, angry, surprised, fearful, disgusted}
 */
typedef struct baymax_interfaces__msg__Emotion
{
  rosidl_runtime_c__String label;
  /// confidence: 0.0 a 1.0
  float confidence;
  /// timestamp da captura
  builtin_interfaces__msg__Time stamp;
} baymax_interfaces__msg__Emotion;

// Struct for a sequence of baymax_interfaces__msg__Emotion.
typedef struct baymax_interfaces__msg__Emotion__Sequence
{
  baymax_interfaces__msg__Emotion * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} baymax_interfaces__msg__Emotion__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // BAYMAX_INTERFACES__MSG__DETAIL__EMOTION__STRUCT_H_
