// generated from rosidl_generator_c/resource/idl__functions.h.em
// with input from baymax_interfaces:msg/Emotion.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "baymax_interfaces/msg/emotion.h"


#ifndef BAYMAX_INTERFACES__MSG__DETAIL__EMOTION__FUNCTIONS_H_
#define BAYMAX_INTERFACES__MSG__DETAIL__EMOTION__FUNCTIONS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stdlib.h>

#include "rosidl_runtime_c/action_type_support_struct.h"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_runtime_c/service_type_support_struct.h"
#include "rosidl_runtime_c/type_description/type_description__struct.h"
#include "rosidl_runtime_c/type_description/type_source__struct.h"
#include "rosidl_runtime_c/type_hash.h"
#include "rosidl_runtime_c/visibility_control.h"
#include "baymax_interfaces/msg/rosidl_generator_c__visibility_control.h"

#include "baymax_interfaces/msg/detail/emotion__struct.h"

/// Initialize msg/Emotion message.
/**
 * If the init function is called twice for the same message without
 * calling fini inbetween previously allocated memory will be leaked.
 * \param[in,out] msg The previously allocated message pointer.
 * Fields without a default value will not be initialized by this function.
 * You might want to call memset(msg, 0, sizeof(
 * baymax_interfaces__msg__Emotion
 * )) before or use
 * baymax_interfaces__msg__Emotion__create()
 * to allocate and initialize the message.
 * \return true if initialization was successful, otherwise false
 */
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
bool
baymax_interfaces__msg__Emotion__init(baymax_interfaces__msg__Emotion * msg);

/// Finalize msg/Emotion message.
/**
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
void
baymax_interfaces__msg__Emotion__fini(baymax_interfaces__msg__Emotion * msg);

/// Create msg/Emotion message.
/**
 * It allocates the memory for the message, sets the memory to zero, and
 * calls
 * baymax_interfaces__msg__Emotion__init().
 * \return The pointer to the initialized message if successful,
 * otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
baymax_interfaces__msg__Emotion *
baymax_interfaces__msg__Emotion__create(void);

/// Destroy msg/Emotion message.
/**
 * It calls
 * baymax_interfaces__msg__Emotion__fini()
 * and frees the memory of the message.
 * \param[in,out] msg The allocated message pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
void
baymax_interfaces__msg__Emotion__destroy(baymax_interfaces__msg__Emotion * msg);

/// Check for msg/Emotion message equality.
/**
 * \param[in] lhs The message on the left hand size of the equality operator.
 * \param[in] rhs The message on the right hand size of the equality operator.
 * \return true if messages are equal, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
bool
baymax_interfaces__msg__Emotion__are_equal(const baymax_interfaces__msg__Emotion * lhs, const baymax_interfaces__msg__Emotion * rhs);

/// Copy a msg/Emotion message.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source message pointer.
 * \param[out] output The target message pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer is null
 *   or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
bool
baymax_interfaces__msg__Emotion__copy(
  const baymax_interfaces__msg__Emotion * input,
  baymax_interfaces__msg__Emotion * output);

/// Retrieve pointer to the hash of the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
const rosidl_type_hash_t *
baymax_interfaces__msg__Emotion__get_type_hash(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
const rosidl_runtime_c__type_description__TypeDescription *
baymax_interfaces__msg__Emotion__get_type_description(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the single raw source text that defined this type.
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
const rosidl_runtime_c__type_description__TypeSource *
baymax_interfaces__msg__Emotion__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support);

/// Retrieve pointer to the recursive raw sources that defined the description of this type.
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
const rosidl_runtime_c__type_description__TypeSource__Sequence *
baymax_interfaces__msg__Emotion__get_type_description_sources(
  const rosidl_message_type_support_t * type_support);

/// Initialize array of msg/Emotion messages.
/**
 * It allocates the memory for the number of elements and calls
 * baymax_interfaces__msg__Emotion__init()
 * for each element of the array.
 * \param[in,out] array The allocated array pointer.
 * \param[in] size The size / capacity of the array.
 * \return true if initialization was successful, otherwise false
 * If the array pointer is valid and the size is zero it is guaranteed
 # to return true.
 */
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
bool
baymax_interfaces__msg__Emotion__Sequence__init(baymax_interfaces__msg__Emotion__Sequence * array, size_t size);

/// Finalize array of msg/Emotion messages.
/**
 * It calls
 * baymax_interfaces__msg__Emotion__fini()
 * for each element of the array and frees the memory for the number of
 * elements.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
void
baymax_interfaces__msg__Emotion__Sequence__fini(baymax_interfaces__msg__Emotion__Sequence * array);

/// Create array of msg/Emotion messages.
/**
 * It allocates the memory for the array and calls
 * baymax_interfaces__msg__Emotion__Sequence__init().
 * \param[in] size The size / capacity of the array.
 * \return The pointer to the initialized array if successful, otherwise NULL
 */
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
baymax_interfaces__msg__Emotion__Sequence *
baymax_interfaces__msg__Emotion__Sequence__create(size_t size);

/// Destroy array of msg/Emotion messages.
/**
 * It calls
 * baymax_interfaces__msg__Emotion__Sequence__fini()
 * on the array,
 * and frees the memory of the array.
 * \param[in,out] array The initialized array pointer.
 */
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
void
baymax_interfaces__msg__Emotion__Sequence__destroy(baymax_interfaces__msg__Emotion__Sequence * array);

/// Check for msg/Emotion message array equality.
/**
 * \param[in] lhs The message array on the left hand size of the equality operator.
 * \param[in] rhs The message array on the right hand size of the equality operator.
 * \return true if message arrays are equal in size and content, otherwise false.
 */
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
bool
baymax_interfaces__msg__Emotion__Sequence__are_equal(const baymax_interfaces__msg__Emotion__Sequence * lhs, const baymax_interfaces__msg__Emotion__Sequence * rhs);

/// Copy an array of msg/Emotion messages.
/**
 * This functions performs a deep copy, as opposed to the shallow copy that
 * plain assignment yields.
 *
 * \param[in] input The source array pointer.
 * \param[out] output The target array pointer, which must
 *   have been initialized before calling this function.
 * \return true if successful, or false if either pointer
 *   is null or memory allocation fails.
 */
ROSIDL_GENERATOR_C_PUBLIC_baymax_interfaces
bool
baymax_interfaces__msg__Emotion__Sequence__copy(
  const baymax_interfaces__msg__Emotion__Sequence * input,
  baymax_interfaces__msg__Emotion__Sequence * output);

#ifdef __cplusplus
}
#endif

#endif  // BAYMAX_INTERFACES__MSG__DETAIL__EMOTION__FUNCTIONS_H_
