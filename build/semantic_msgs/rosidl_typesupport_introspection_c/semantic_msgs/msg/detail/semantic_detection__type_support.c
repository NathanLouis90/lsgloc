// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from semantic_msgs:msg/SemanticDetection.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "semantic_msgs/msg/detail/semantic_detection__rosidl_typesupport_introspection_c.h"
#include "semantic_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "semantic_msgs/msg/detail/semantic_detection__functions.h"
#include "semantic_msgs/msg/detail/semantic_detection__struct.h"


// Include directives for member types
// Member `label`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void semantic_msgs__msg__SemanticDetection__rosidl_typesupport_introspection_c__SemanticDetection_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  semantic_msgs__msg__SemanticDetection__init(message_memory);
}

void semantic_msgs__msg__SemanticDetection__rosidl_typesupport_introspection_c__SemanticDetection_fini_function(void * message_memory)
{
  semantic_msgs__msg__SemanticDetection__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember semantic_msgs__msg__SemanticDetection__rosidl_typesupport_introspection_c__SemanticDetection_message_member_array[4] = {
  {
    "label",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(semantic_msgs__msg__SemanticDetection, label),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "bearing",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(semantic_msgs__msg__SemanticDetection, bearing),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "range",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(semantic_msgs__msg__SemanticDetection, range),  // bytes offset in struct
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
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(semantic_msgs__msg__SemanticDetection, confidence),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers semantic_msgs__msg__SemanticDetection__rosidl_typesupport_introspection_c__SemanticDetection_message_members = {
  "semantic_msgs__msg",  // message namespace
  "SemanticDetection",  // message name
  4,  // number of fields
  sizeof(semantic_msgs__msg__SemanticDetection),
  semantic_msgs__msg__SemanticDetection__rosidl_typesupport_introspection_c__SemanticDetection_message_member_array,  // message members
  semantic_msgs__msg__SemanticDetection__rosidl_typesupport_introspection_c__SemanticDetection_init_function,  // function to initialize message memory (memory has to be allocated)
  semantic_msgs__msg__SemanticDetection__rosidl_typesupport_introspection_c__SemanticDetection_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t semantic_msgs__msg__SemanticDetection__rosidl_typesupport_introspection_c__SemanticDetection_message_type_support_handle = {
  0,
  &semantic_msgs__msg__SemanticDetection__rosidl_typesupport_introspection_c__SemanticDetection_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_semantic_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, semantic_msgs, msg, SemanticDetection)() {
  if (!semantic_msgs__msg__SemanticDetection__rosidl_typesupport_introspection_c__SemanticDetection_message_type_support_handle.typesupport_identifier) {
    semantic_msgs__msg__SemanticDetection__rosidl_typesupport_introspection_c__SemanticDetection_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &semantic_msgs__msg__SemanticDetection__rosidl_typesupport_introspection_c__SemanticDetection_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
