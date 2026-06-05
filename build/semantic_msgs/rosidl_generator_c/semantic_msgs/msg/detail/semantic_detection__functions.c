// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from semantic_msgs:msg/SemanticDetection.idl
// generated code does not contain a copyright notice
#include "semantic_msgs/msg/detail/semantic_detection__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `label`
#include "rosidl_runtime_c/string_functions.h"

bool
semantic_msgs__msg__SemanticDetection__init(semantic_msgs__msg__SemanticDetection * msg)
{
  if (!msg) {
    return false;
  }
  // label
  if (!rosidl_runtime_c__String__init(&msg->label)) {
    semantic_msgs__msg__SemanticDetection__fini(msg);
    return false;
  }
  // bearing
  // range
  // confidence
  return true;
}

void
semantic_msgs__msg__SemanticDetection__fini(semantic_msgs__msg__SemanticDetection * msg)
{
  if (!msg) {
    return;
  }
  // label
  rosidl_runtime_c__String__fini(&msg->label);
  // bearing
  // range
  // confidence
}

bool
semantic_msgs__msg__SemanticDetection__are_equal(const semantic_msgs__msg__SemanticDetection * lhs, const semantic_msgs__msg__SemanticDetection * rhs)
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
  // bearing
  if (lhs->bearing != rhs->bearing) {
    return false;
  }
  // range
  if (lhs->range != rhs->range) {
    return false;
  }
  // confidence
  if (lhs->confidence != rhs->confidence) {
    return false;
  }
  return true;
}

bool
semantic_msgs__msg__SemanticDetection__copy(
  const semantic_msgs__msg__SemanticDetection * input,
  semantic_msgs__msg__SemanticDetection * output)
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
  // bearing
  output->bearing = input->bearing;
  // range
  output->range = input->range;
  // confidence
  output->confidence = input->confidence;
  return true;
}

semantic_msgs__msg__SemanticDetection *
semantic_msgs__msg__SemanticDetection__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  semantic_msgs__msg__SemanticDetection * msg = (semantic_msgs__msg__SemanticDetection *)allocator.allocate(sizeof(semantic_msgs__msg__SemanticDetection), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(semantic_msgs__msg__SemanticDetection));
  bool success = semantic_msgs__msg__SemanticDetection__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
semantic_msgs__msg__SemanticDetection__destroy(semantic_msgs__msg__SemanticDetection * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    semantic_msgs__msg__SemanticDetection__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
semantic_msgs__msg__SemanticDetection__Sequence__init(semantic_msgs__msg__SemanticDetection__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  semantic_msgs__msg__SemanticDetection * data = NULL;

  if (size) {
    data = (semantic_msgs__msg__SemanticDetection *)allocator.zero_allocate(size, sizeof(semantic_msgs__msg__SemanticDetection), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = semantic_msgs__msg__SemanticDetection__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        semantic_msgs__msg__SemanticDetection__fini(&data[i - 1]);
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
semantic_msgs__msg__SemanticDetection__Sequence__fini(semantic_msgs__msg__SemanticDetection__Sequence * array)
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
      semantic_msgs__msg__SemanticDetection__fini(&array->data[i]);
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

semantic_msgs__msg__SemanticDetection__Sequence *
semantic_msgs__msg__SemanticDetection__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  semantic_msgs__msg__SemanticDetection__Sequence * array = (semantic_msgs__msg__SemanticDetection__Sequence *)allocator.allocate(sizeof(semantic_msgs__msg__SemanticDetection__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = semantic_msgs__msg__SemanticDetection__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
semantic_msgs__msg__SemanticDetection__Sequence__destroy(semantic_msgs__msg__SemanticDetection__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    semantic_msgs__msg__SemanticDetection__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
semantic_msgs__msg__SemanticDetection__Sequence__are_equal(const semantic_msgs__msg__SemanticDetection__Sequence * lhs, const semantic_msgs__msg__SemanticDetection__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!semantic_msgs__msg__SemanticDetection__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
semantic_msgs__msg__SemanticDetection__Sequence__copy(
  const semantic_msgs__msg__SemanticDetection__Sequence * input,
  semantic_msgs__msg__SemanticDetection__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(semantic_msgs__msg__SemanticDetection);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    semantic_msgs__msg__SemanticDetection * data =
      (semantic_msgs__msg__SemanticDetection *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!semantic_msgs__msg__SemanticDetection__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          semantic_msgs__msg__SemanticDetection__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!semantic_msgs__msg__SemanticDetection__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
