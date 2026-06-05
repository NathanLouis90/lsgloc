// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from semantic_msgs:msg/SemanticDetectionArray.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "semantic_msgs/msg/detail/semantic_detection_array__rosidl_typesupport_introspection_c.h"
#include "semantic_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "semantic_msgs/msg/detail/semantic_detection_array__functions.h"
#include "semantic_msgs/msg/detail/semantic_detection_array__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `detections`
#include "semantic_msgs/msg/semantic_detection.h"
// Member `detections`
#include "semantic_msgs/msg/detail/semantic_detection__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__SemanticDetectionArray_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  semantic_msgs__msg__SemanticDetectionArray__init(message_memory);
}

void semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__SemanticDetectionArray_fini_function(void * message_memory)
{
  semantic_msgs__msg__SemanticDetectionArray__fini(message_memory);
}

size_t semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__size_function__SemanticDetectionArray__detections(
  const void * untyped_member)
{
  const semantic_msgs__msg__SemanticDetection__Sequence * member =
    (const semantic_msgs__msg__SemanticDetection__Sequence *)(untyped_member);
  return member->size;
}

const void * semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__get_const_function__SemanticDetectionArray__detections(
  const void * untyped_member, size_t index)
{
  const semantic_msgs__msg__SemanticDetection__Sequence * member =
    (const semantic_msgs__msg__SemanticDetection__Sequence *)(untyped_member);
  return &member->data[index];
}

void * semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__get_function__SemanticDetectionArray__detections(
  void * untyped_member, size_t index)
{
  semantic_msgs__msg__SemanticDetection__Sequence * member =
    (semantic_msgs__msg__SemanticDetection__Sequence *)(untyped_member);
  return &member->data[index];
}

void semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__fetch_function__SemanticDetectionArray__detections(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const semantic_msgs__msg__SemanticDetection * item =
    ((const semantic_msgs__msg__SemanticDetection *)
    semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__get_const_function__SemanticDetectionArray__detections(untyped_member, index));
  semantic_msgs__msg__SemanticDetection * value =
    (semantic_msgs__msg__SemanticDetection *)(untyped_value);
  *value = *item;
}

void semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__assign_function__SemanticDetectionArray__detections(
  void * untyped_member, size_t index, const void * untyped_value)
{
  semantic_msgs__msg__SemanticDetection * item =
    ((semantic_msgs__msg__SemanticDetection *)
    semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__get_function__SemanticDetectionArray__detections(untyped_member, index));
  const semantic_msgs__msg__SemanticDetection * value =
    (const semantic_msgs__msg__SemanticDetection *)(untyped_value);
  *item = *value;
}

bool semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__resize_function__SemanticDetectionArray__detections(
  void * untyped_member, size_t size)
{
  semantic_msgs__msg__SemanticDetection__Sequence * member =
    (semantic_msgs__msg__SemanticDetection__Sequence *)(untyped_member);
  semantic_msgs__msg__SemanticDetection__Sequence__fini(member);
  return semantic_msgs__msg__SemanticDetection__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__SemanticDetectionArray_message_member_array[2] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(semantic_msgs__msg__SemanticDetectionArray, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "detections",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(semantic_msgs__msg__SemanticDetectionArray, detections),  // bytes offset in struct
    NULL,  // default value
    semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__size_function__SemanticDetectionArray__detections,  // size() function pointer
    semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__get_const_function__SemanticDetectionArray__detections,  // get_const(index) function pointer
    semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__get_function__SemanticDetectionArray__detections,  // get(index) function pointer
    semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__fetch_function__SemanticDetectionArray__detections,  // fetch(index, &value) function pointer
    semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__assign_function__SemanticDetectionArray__detections,  // assign(index, value) function pointer
    semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__resize_function__SemanticDetectionArray__detections  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__SemanticDetectionArray_message_members = {
  "semantic_msgs__msg",  // message namespace
  "SemanticDetectionArray",  // message name
  2,  // number of fields
  sizeof(semantic_msgs__msg__SemanticDetectionArray),
  semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__SemanticDetectionArray_message_member_array,  // message members
  semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__SemanticDetectionArray_init_function,  // function to initialize message memory (memory has to be allocated)
  semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__SemanticDetectionArray_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__SemanticDetectionArray_message_type_support_handle = {
  0,
  &semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__SemanticDetectionArray_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_semantic_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, semantic_msgs, msg, SemanticDetectionArray)() {
  semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__SemanticDetectionArray_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__SemanticDetectionArray_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, semantic_msgs, msg, SemanticDetection)();
  if (!semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__SemanticDetectionArray_message_type_support_handle.typesupport_identifier) {
    semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__SemanticDetectionArray_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &semantic_msgs__msg__SemanticDetectionArray__rosidl_typesupport_introspection_c__SemanticDetectionArray_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
