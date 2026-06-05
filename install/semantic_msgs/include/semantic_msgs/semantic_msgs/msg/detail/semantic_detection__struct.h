// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from semantic_msgs:msg/SemanticDetection.idl
// generated code does not contain a copyright notice

#ifndef SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION__STRUCT_H_
#define SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION__STRUCT_H_

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

/// Struct defined in msg/SemanticDetection in the package semantic_msgs.
typedef struct semantic_msgs__msg__SemanticDetection
{
  rosidl_runtime_c__String label;
  double bearing;
  double range;
  double confidence;
} semantic_msgs__msg__SemanticDetection;

// Struct for a sequence of semantic_msgs__msg__SemanticDetection.
typedef struct semantic_msgs__msg__SemanticDetection__Sequence
{
  semantic_msgs__msg__SemanticDetection * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} semantic_msgs__msg__SemanticDetection__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION__STRUCT_H_
