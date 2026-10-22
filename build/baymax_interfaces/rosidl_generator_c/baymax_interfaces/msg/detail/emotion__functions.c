// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from baymax_interfaces:msg/Emotion.idl
// generated code does not contain a copyright notice
#include "baymax_interfaces/msg/detail/emotion__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `label`
#include "rosidl_runtime_c/string_functions.h"
// Member `stamp`
#include "builtin_interfaces/msg/detail/time__functions.h"

bool
baymax_interfaces__msg__Emotion__init(baymax_interfaces__msg__Emotion * msg)
{
  if (!msg) {
    return false;
  }
  // label
  if (!rosidl_runtime_c__String__init(&msg->label)) {
    baymax_interfaces__msg__Emotion__fini(msg);
    return false;
  }
  // confidence
  // stamp
  if (!builtin_interfaces__msg__Time__init(&msg->stamp)) {
    baymax_interfaces__msg__Emotion__fini(msg);
    return false;
  }
  return true;
}

void
baymax_interfaces__msg__Emotion__fini(baymax_interfaces__msg__Emotion * msg)
{
  if (!msg) {
    return;
  }
  // label
  rosidl_runtime_c__String__fini(&msg->label);
  // confidence
  // stamp
  builtin_interfaces__msg__Time__fini(&msg->stamp);
}

bool
baymax_interfaces__msg__Emotion__are_equal(const baymax_interfaces__msg__Emotion * lhs, const baymax_interfaces__msg__Emotion * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // label
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->label), &(rhs->label)))
  {
    return false;
  }
  // confidence
  if (lhs->confidence != rhs->confidence) {
    return false;
  }
  // stamp
  if (!builtin_interfaces__msg__Time__are_equal(
      &(lhs->stamp), &(rhs->stamp)))
  {
    return false;
  }
  return true;
}

bool
baymax_interfaces__msg__Emotion__copy(
  const baymax_interfaces__msg__Emotion * input,
  baymax_interfaces__msg__Emotion * output)
{
  if (!input || !output) {
    return false;
  }
  // label
  if (!rosidl_runtime_c__String__copy(
      &(input->label), &(output->label)))
  {
    return false;
  }
  // confidence
  output->confidence = input->confidence;
  // stamp
  if (!builtin_interfaces__msg__Time__copy(
      &(input->stamp), &(output->stamp)))
  {
    return false;
  }
  return true;
}

baymax_interfaces__msg__Emotion *
baymax_interfaces__msg__Emotion__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  baymax_interfaces__msg__Emotion * msg = (baymax_interfaces__msg__Emotion *)allocator.allocate(sizeof(baymax_interfaces__msg__Emotion), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(baymax_interfaces__msg__Emotion));
  bool success = baymax_interfaces__msg__Emotion__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
baymax_interfaces__msg__Emotion__destroy(baymax_interfaces__msg__Emotion * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    baymax_interfaces__msg__Emotion__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
baymax_interfaces__msg__Emotion__Sequence__init(baymax_interfaces__msg__Emotion__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  baymax_interfaces__msg__Emotion * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(baymax_interfaces__msg__Emotion)) {
      return false;
    }
    data = (baymax_interfaces__msg__Emotion *)allocator.zero_allocate(size, sizeof(baymax_interfaces__msg__Emotion), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = baymax_interfaces__msg__Emotion__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        baymax_interfaces__msg__Emotion__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
baymax_interfaces__msg__Emotion__Sequence__fini(baymax_interfaces__msg__Emotion__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      baymax_interfaces__msg__Emotion__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

baymax_interfaces__msg__Emotion__Sequence *
baymax_interfaces__msg__Emotion__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  baymax_interfaces__msg__Emotion__Sequence * array = (baymax_interfaces__msg__Emotion__Sequence *)allocator.allocate(sizeof(baymax_interfaces__msg__Emotion__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = baymax_interfaces__msg__Emotion__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
baymax_interfaces__msg__Emotion__Sequence__destroy(baymax_interfaces__msg__Emotion__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    baymax_interfaces__msg__Emotion__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
baymax_interfaces__msg__Emotion__Sequence__are_equal(const baymax_interfaces__msg__Emotion__Sequence * lhs, const baymax_interfaces__msg__Emotion__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!baymax_interfaces__msg__Emotion__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
baymax_interfaces__msg__Emotion__Sequence__copy(
  const baymax_interfaces__msg__Emotion__Sequence * input,
  baymax_interfaces__msg__Emotion__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(baymax_interfaces__msg__Emotion)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(baymax_interfaces__msg__Emotion);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    baymax_interfaces__msg__Emotion * data =
      (baymax_interfaces__msg__Emotion *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!baymax_interfaces__msg__Emotion__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          baymax_interfaces__msg__Emotion__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!baymax_interfaces__msg__Emotion__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
