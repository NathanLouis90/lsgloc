// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from semantic_msgs:msg/SemanticDetectionArray.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "semantic_msgs/msg/detail/semantic_detection_array__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace semantic_msgs
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void SemanticDetectionArray_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) semantic_msgs::msg::SemanticDetectionArray(_init);
}

void SemanticDetectionArray_fini_function(void * message_memory)
{
  auto typed_message = static_cast<semantic_msgs::msg::SemanticDetectionArray *>(message_memory);
  typed_message->~SemanticDetectionArray();
}

size_t size_function__SemanticDetectionArray__detections(const void * untyped_member)
{
  const auto * member = reinterpret_cast<const std::vector<semantic_msgs::msg::SemanticDetection> *>(untyped_member);
  return member->size();
}

const void * get_const_function__SemanticDetectionArray__detections(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::vector<semantic_msgs::msg::SemanticDetection> *>(untyped_member);
  return &member[index];
}

void * get_function__SemanticDetectionArray__detections(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::vector<semantic_msgs::msg::SemanticDetection> *>(untyped_member);
  return &member[index];
}

void fetch_function__SemanticDetectionArray__detections(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const semantic_msgs::msg::SemanticDetection *>(
    get_const_function__SemanticDetectionArray__detections(untyped_member, index));
  auto & value = *reinterpret_cast<semantic_msgs::msg::SemanticDetection *>(untyped_value);
  value = item;
}

void assign_function__SemanticDetectionArray__detections(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<semantic_msgs::msg::SemanticDetection *>(
    get_function__SemanticDetectionArray__detections(untyped_member, index));
  const auto & value = *reinterpret_cast<const semantic_msgs::msg::SemanticDetection *>(untyped_value);
  item = value;
}

void resize_function__SemanticDetectionArray__detections(void * untyped_member, size_t size)
{
  auto * member =
    reinterpret_cast<std::vector<semantic_msgs::msg::SemanticDetection> *>(untyped_member);
  member->resize(size);
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember SemanticDetectionArray_message_member_array[2] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(semantic_msgs::msg::SemanticDetectionArray, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "detections",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<semantic_msgs::msg::SemanticDetection>(),  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(semantic_msgs::msg::SemanticDetectionArray, detections),  // bytes offset in struct
    nullptr,  // default value
    size_function__SemanticDetectionArray__detections,  // size() function pointer
    get_const_function__SemanticDetectionArray__detections,  // get_const(index) function pointer
    get_function__SemanticDetectionArray__detections,  // get(index) function pointer
    fetch_function__SemanticDetectionArray__detections,  // fetch(index, &value) function pointer
    assign_function__SemanticDetectionArray__detections,  // assign(index, value) function pointer
    resize_function__SemanticDetectionArray__detections  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers SemanticDetectionArray_message_members = {
  "semantic_msgs::msg",  // message namespace
  "SemanticDetectionArray",  // message name
  2,  // number of fields
  sizeof(semantic_msgs::msg::SemanticDetectionArray),
  SemanticDetectionArray_message_member_array,  // message members
  SemanticDetectionArray_init_function,  // function to initialize message memory (memory has to be allocated)
  SemanticDetectionArray_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t SemanticDetectionArray_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &SemanticDetectionArray_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace semantic_msgs


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<semantic_msgs::msg::SemanticDetectionArray>()
{
  return &::semantic_msgs::msg::rosidl_typesupport_introspection_cpp::SemanticDetectionArray_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, semantic_msgs, msg, SemanticDetectionArray)() {
  return &::semantic_msgs::msg::rosidl_typesupport_introspection_cpp::SemanticDetectionArray_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
