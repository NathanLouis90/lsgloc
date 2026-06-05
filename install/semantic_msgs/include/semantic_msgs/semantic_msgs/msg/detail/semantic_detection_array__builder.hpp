// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from semantic_msgs:msg/SemanticDetectionArray.idl
// generated code does not contain a copyright notice

#ifndef SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION_ARRAY__BUILDER_HPP_
#define SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION_ARRAY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "semantic_msgs/msg/detail/semantic_detection_array__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace semantic_msgs
{

namespace msg
{

namespace builder
{

class Init_SemanticDetectionArray_detections
{
public:
  explicit Init_SemanticDetectionArray_detections(::semantic_msgs::msg::SemanticDetectionArray & msg)
  : msg_(msg)
  {}
  ::semantic_msgs::msg::SemanticDetectionArray detections(::semantic_msgs::msg::SemanticDetectionArray::_detections_type arg)
  {
    msg_.detections = std::move(arg);
    return std::move(msg_);
  }

private:
  ::semantic_msgs::msg::SemanticDetectionArray msg_;
};

class Init_SemanticDetectionArray_header
{
public:
  Init_SemanticDetectionArray_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_SemanticDetectionArray_detections header(::semantic_msgs::msg::SemanticDetectionArray::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_SemanticDetectionArray_detections(msg_);
  }

private:
  ::semantic_msgs::msg::SemanticDetectionArray msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::semantic_msgs::msg::SemanticDetectionArray>()
{
  return semantic_msgs::msg::builder::Init_SemanticDetectionArray_header();
}

}  // namespace semantic_msgs

#endif  // SEMANTIC_MSGS__MSG__DETAIL__SEMANTIC_DETECTION_ARRAY__BUILDER_HPP_
