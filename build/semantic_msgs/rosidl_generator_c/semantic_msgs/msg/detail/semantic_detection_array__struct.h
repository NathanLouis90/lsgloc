// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from semantic_msgs:msg/SemanticDetectionArray.idl
// generated code does not contain a copyright notice

#ifndef SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION_ARRAY__STRUCT_H_
#define SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION_ARRAY__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'detections'
#include "semantic_msgs/msg/detail/semantic_detection__struct.h"

/// Struct defined in msg/SemanticDetectionArray in the package semantic_msgs.
typedef struct semantic_msgs__msg__SemanticDetectionArray
{
  std_msgs__msg__Header header;
  semantic_msgs__msg__SemanticDetection__Sequence detections;
} semantic_msgs__msg__SemanticDetectionArray;

// Struct for a sequence of semantic_msgs__msg__SemanticDetectionArray.
typedef struct semantic_msgs__msg__SemanticDetectionArray__Sequence
{
  semantic_msgs__msg__SemanticDetectionArray * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} semantic_msgs__msg__SemanticDetectionArray__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION_ARRAY__STRUCT_H_
