// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from semantic_msgs:msg/SemanticDetectionArray.idl
// generated code does not contain a copyright notice
#include "semantic_msgs/msg/detail/semantic_detection_array__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `detections`
#include "semantic_msgs/msg/detail/semantic_detection__functions.h"

bool
semantic_msgs__msg__SemanticDetectionArray__init(semantic_msgs__msg__SemanticDetectionArray * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    semantic_msgs__msg__SemanticDetectionArray__fini(msg);
    return false;
  }
  // detections
  if (!semantic_msgs__msg__SemanticDetection__Sequence__init(&msg->detections, 0)) {
    semantic_msgs__msg__SemanticDetectionArray__fini(msg);
    return false;
  }
  return true;
}

void
semantic_msgs__msg__SemanticDetectionArray__fini(semantic_msgs__msg__SemanticDetectionArray * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // detections
  semantic_msgs__msg__SemanticDetection__Sequence__fini(&msg->detections);
}

bool
semantic_msgs__msg__SemanticDetectionArray__are_equal(const semantic_msgs__msg__SemanticDetectionArray * lhs, const semantic_msgs__msg__SemanticDetectionArray * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // detections
  if (!semantic_msgs__msg__SemanticDetection__Sequence__are_equal(
      &(lhs->detections), &(rhs->detections)))
  {
    return false;
  }
  return true;
}

bool
semantic_msgs__msg__SemanticDetectionArray__copy(
  const semantic_msgs__msg__SemanticDetectionArray * input,
  semantic_msgs__msg__SemanticDetectionArray * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // detections
  if (!semantic_msgs__msg__SemanticDetection__Sequence__copy(
      &(input->detections), &(output->detections)))
  {
    return false;
  }
  return true;
}

semantic_msgs__msg__SemanticDetectionArray *
semantic_msgs__msg__SemanticDetectionArray__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  semantic_msgs__msg__SemanticDetectionArray * msg = (semantic_msgs__msg__SemanticDetectionArray *)allocator.allocate(sizeof(semantic_msgs__msg__SemanticDetectionArray), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(semantic_msgs__msg__SemanticDetectionArray));
  bool success = semantic_msgs__msg__SemanticDetectionArray__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
semantic_msgs__msg__SemanticDetectionArray__destroy(semantic_msgs__msg__SemanticDetectionArray * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    semantic_msgs__msg__SemanticDetectionArray__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
semantic_msgs__msg__SemanticDetectionArray__Sequence__init(semantic_msgs__msg__SemanticDetectionArray__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  semantic_msgs__msg__SemanticDetectionArray * data = NULL;

  if (size) {
    data = (semantic_msgs__msg__SemanticDetectionArray *)allocator.zero_allocate(size, sizeof(semantic_msgs__msg__SemanticDetectionArray), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = semantic_msgs__msg__SemanticDetectionArray__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        semantic_msgs__msg__SemanticDetectionArray__fini(&data[i - 1]);
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
semantic_msgs__msg__SemanticDetectionArray__Sequence__fini(semantic_msgs__msg__SemanticDetectionArray__Sequence * array)
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
      semantic_msgs__msg__SemanticDetectionArray__fini(&array->data[i]);
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

semantic_msgs__msg__SemanticDetectionArray__Sequence *
semantic_msgs__msg__SemanticDetectionArray__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  semantic_msgs__msg__SemanticDetectionArray__Sequence * array = (semantic_msgs__msg__SemanticDetectionArray__Sequence *)allocator.allocate(sizeof(semantic_msgs__msg__SemanticDetectionArray__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = semantic_msgs__msg__SemanticDetectionArray__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
semantic_msgs__msg__SemanticDetectionArray__Sequence__destroy(semantic_msgs__msg__SemanticDetectionArray__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    semantic_msgs__msg__SemanticDetectionArray__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
semantic_msgs__msg__SemanticDetectionArray__Sequence__are_equal(const semantic_msgs__msg__SemanticDetectionArray__Sequence * lhs, const semantic_msgs__msg__SemanticDetectionArray__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!semantic_msgs__msg__SemanticDetectionArray__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
semantic_msgs__msg__SemanticDetectionArray__Sequence__copy(
  const semantic_msgs__msg__SemanticDetectionArray__Sequence * input,
  semantic_msgs__msg__SemanticDetectionArray__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(semantic_msgs__msg__SemanticDetectionArray);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    semantic_msgs__msg__SemanticDetectionArray * data =
      (semantic_msgs__msg__SemanticDetectionArray *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!semantic_msgs__msg__SemanticDetectionArray__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          semantic_msgs__msg__SemanticDetectionArray__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!semantic_msgs__msg__SemanticDetectionArray__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
